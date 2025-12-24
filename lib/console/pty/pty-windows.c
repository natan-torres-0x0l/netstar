#define _WIN32_WINNT 0x0A00

#include <console/console.h>
#include "pty.h"

#include <strings/strings.h>
#include <stdlib.h>


static inline HANDLE
console_pty_output_handle(void) {
  return GetStdHandle(STD_OUTPUT_HANDLE);
}

static inline HANDLE
console_pty_input_handle(void) {
  return GetStdHandle(STD_INPUT_HANDLE);
}

int
console_pty_spawn(console_pty_t *pty, char **command) {
  SECURITY_ATTRIBUTES security_attr = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };
  size_t thread_attr_list_size = 0;

  COORD console_size = { console_width(), console_height() };
  char *command_line = NULL;

  HRESULT result;

  if (!CreatePipe(&pty->conpty_pty_input, &pty->conpty_app_output, NULL, 0))
    return -1;
  if (!CreatePipe(&pty->conpty_app_input, &pty->conpty_pty_output, NULL, 0))
    return -1;

  result = CreatePseudoConsole(console_size, pty->conpty_pty_input, pty->conpty_pty_output, 0, &pty->conpty);
  if (FAILED(result))
    return -1;

  InitializeProcThreadAttributeList(NULL, 1, 0, &thread_attr_list_size);

  if (!(pty->startup_info.lpAttributeList = (LPPROC_THREAD_ATTRIBUTE_LIST)malloc(thread_attr_list_size)))
    return -1;

  InitializeProcThreadAttributeList(pty->startup_info.lpAttributeList, 1, 0, &thread_attr_list_size);

  UpdateProcThreadAttribute(pty->startup_info.lpAttributeList, 0, PROC_THREAD_ATTRIBUTE_PSEUDOCONSOLE, pty->conpty, sizeof(HPCON), NULL, NULL);
  pty->startup_info.StartupInfo.cb = sizeof(STARTUPINFOEXA);

  command_line = string_join((const char **)command, " ");

  if (!CreateProcessA(NULL, command_line, NULL, NULL, FALSE, 0 /*EXTENDED_STARTUPINFO_PRESENT*/, NULL, NULL, &pty->startup_info.StartupInfo, &pty->process_info))
    return -1;

  free(command_line);

// CloseHandle(pty->conpty_input_read);
// CloseHandle(pty->conpty_output_write);  

  return 1;
}

ssize_t
console_pty_read(console_pty_t *pty, void *buffer, size_t length) {
  DWORD bytes = 0;

  if (!ReadFile(pty->conpty_app_input, buffer, length, &bytes, NULL))
    return -1;

  return (ssize_t)bytes;
}

ssize_t
console_pty_write(console_pty_t *pty, const void *buffer, size_t length) {
  DWORD bytes = 0;

  if (!WriteFile(pty->conpty_app_output, buffer, length, &bytes, NULL))
    return -1;

  return (ssize_t)bytes;
}

void
console_pty_close(console_pty_t *pty) {
  if (pty) {
    ClosePseudoConsole(pty->conpty);
    CloseHandle(pty->process_info.hThread);
    CloseHandle(pty->process_info.hProcess);

    CloseHandle(pty->conpty_pty_input);
    CloseHandle(pty->conpty_pty_output);  

    DeleteProcThreadAttributeList(pty->startup_info.lpAttributeList);
    free(pty->startup_info.lpAttributeList);
  }
}


#define CONSOLE_PTY_PROCESS_BUFFER_LENGTH 520

struct console_pty_process {
  char input_buffer[CONSOLE_PTY_PROCESS_BUFFER_LENGTH], output_buffer[CONSOLE_PTY_PROCESS_BUFFER_LENGTH];
  DWORD input_bytes_read, output_bytes_read;

  CONDITION_VARIABLE synchronize_input_signal, synchronize_output_signal;
  CRITICAL_SECTION input_critical_section, output_critical_section;

  bool synchronized_input_event, synchronized_output_event;
  HANDLE threads[2], events[3], handles[2];
  DWORD threads_ids[2];

  HANDLE process;
  bool status;
};


static DWORD WINAPI
console_pty_process_input_thread(LPVOID args) {
  struct console_pty_process *process = (struct console_pty_process *)args;

  for (; process->status;) {
    ZeroMemory(process->input_buffer, sizeof(process->input_buffer));
    process->input_bytes_read = 0;
    if (!ReadFile(process->handles[1], process->input_buffer, sizeof(process->input_buffer), &process->input_bytes_read, NULL)) {
      process->status = false;
      SetEvent(process->events[1]);
      break;
    }

    SetEvent(process->events[1]);

    EnterCriticalSection(&process->input_critical_section);
    process->synchronized_input_event = false;
    SleepConditionVariableCS(&process->synchronize_input_signal, &process->input_critical_section, INFINITE);
    LeaveCriticalSection(&process->input_critical_section);
  }

  return 0;
}

static DWORD WINAPI
console_pty_process_output_thread(LPVOID args) {
  struct console_pty_process *process = (struct console_pty_process *)args;

  for (; process->status;) {
    ZeroMemory(process->output_buffer, sizeof(process->output_buffer));
    process->output_bytes_read = 0;
    if (!ReadFile(process->handles[0], process->output_buffer, sizeof(process->output_buffer), &process->output_bytes_read, NULL)) {
      process->status = false;
      SetEvent(process->events[0]);
      break;
    }

    SetEvent(process->events[0]);

    EnterCriticalSection(&process->output_critical_section);
    process->synchronized_output_event = false;
    SleepConditionVariableCS(&process->synchronize_output_signal, &process->output_critical_section, INFINITE);
    LeaveCriticalSection(&process->output_critical_section);
  }

  return 0;
}

#define CONSOLE_PTY_STDIN  0x00
#define CONSOLE_PTY_MASTER 0x01

static int
console_pty_process_wait_for_multiple_objects(struct console_pty_process *process) {
  if (!process->synchronized_input_event)
    return CONSOLE_PTY_STDIN;

  if (!process->synchronized_output_event)
    return CONSOLE_PTY_MASTER;

  if (process->status == false)
    return -1;

  ResetEvent(process->events[0]);
  ResetEvent(process->events[1]);

  switch (WaitForMultipleObjects(3, process->events, FALSE, INFINITE)) {
    case WAIT_OBJECT_0:
      return CONSOLE_PTY_MASTER;
  
    case WAIT_OBJECT_0+1:
      return CONSOLE_PTY_STDIN;
  }

  return -1;
}

static inline int
console_pty_process_threads_initialize(struct console_pty_process *process) {
  InitializeCriticalSection(&process->output_critical_section);
  InitializeConditionVariable(&process->synchronize_output_signal);
  process->events[0] = CreateEvent(NULL, TRUE, FALSE, NULL);
  process->synchronized_output_event = true;
  process->threads[0] = CreateThread(NULL, 0, console_pty_process_output_thread, process, 0, &process->threads_ids[0]);

  InitializeCriticalSection(&process->input_critical_section);
  InitializeConditionVariable(&process->synchronize_input_signal);
  process->events[1] = CreateEvent(NULL, TRUE, FALSE, NULL);
  process->synchronized_input_event = true;
  process->threads[1] = CreateThread(NULL, 0, console_pty_process_input_thread, process, 0, &process->threads_ids[1]);
}

static inline void
console_pty_process_initialize(struct console_pty_process *process, console_pty_t *pty) {
  process->handles[0] = pty->conpty_app_input;
  process->handles[1] = console_pty_input_handle();

  process->process = pty->process_info.hProcess;
  process->events[2] = process->process;

  process->status = true;

  console_pty_process_threads_initialize(process);
}

int
console_pty_login(console_pty_t *pty, console_pty_input_t input, console_pty_output_t output, void *args) {
  struct console_pty_process process = {0};

  console_pty_process_initialize(&process, pty);

  for (; process.status;) {
 // DWORD process_exit_code = -1;
 //
 // if (!GetExitCodeProcess(pty->process_info.hProcess, &process_exit_code) || process_exit_code != STILL_ACTIVE) {
 //   break;
 // }

    switch (console_pty_process_wait_for_multiple_objects(&process)) {
      case CONSOLE_PTY_MASTER:
        output(pty, process.output_buffer, process.output_bytes_read, args);

        EnterCriticalSection(&process.output_critical_section);
        process.synchronized_output_event = true;
        WakeConditionVariable(&process.synchronize_output_signal);
        LeaveCriticalSection(&process.output_critical_section);
        break;

      case CONSOLE_PTY_STDIN:
        input(pty, process.input_buffer, process.input_bytes_read, args);

        EnterCriticalSection(&process.input_critical_section);
        process.synchronized_input_event = true;
        WakeConditionVariable(&process.synchronize_input_signal);
        LeaveCriticalSection(&process.input_critical_section);
        break;

      default:
        process.status = false;
        break;
    }
  }

  CloseHandle(process.threads[0]);
  CloseHandle(process.threads[1]);

  return 1;
}

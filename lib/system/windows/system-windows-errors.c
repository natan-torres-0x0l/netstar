#include "errors.h"


const char *
system_error_reason(system_error_t error) {
  if (!error) // ERROR_SUCCESS
    return "The operation completed successfully";

  switch (error) {
    case ERROR_INVALID_FUNCTION:
      return "Incorrect function";

    case ERROR_FILE_NOT_FOUND:
      return "The system cannot find the file specified";

    case ERROR_PATH_NOT_FOUND:
      return "The system cannot find the path specified";

    case ERROR_TOO_MANY_OPEN_FILES:
      return "The system cannot open the file";

    case ERROR_ACCESS_DENIED:
      return "Access is denied";

    case ERROR_INVALID_HANDLE:
      return "The handle is invalid";

    case ERROR_ARENA_TRASHED:
      return "The storage control blocks were destroyed";

    case ERROR_NOT_ENOUGH_MEMORY:
      return "Not enough storage is available to process this command";

    case ERROR_INVALID_BLOCK:
      return "The storage control block address is invalid";

    case ERROR_BAD_ENVIRONMENT:
      return "The environment is incorrect";

    case ERROR_BAD_FORMAT:
      return "An attempt was made to load a program with an incorrect format";

    case ERROR_INVALID_ACCESS:
      return "The access code is invalid";

    case ERROR_INVALID_DATA:
      return "The data is invalid";

    case ERROR_OUTOFMEMORY:
      return "Not enough storage is available to complete this operation";

    case ERROR_INVALID_DRIVE:
      return "The system cannot find the drive specified";

    case ERROR_CURRENT_DIRECTORY:
      return "The directory cannot be removed";

    case ERROR_NOT_SAME_DEVICE:
      return "The system cannot move the file to a different disk drive";

    case ERROR_NO_MORE_FILES:
      return "There are no more files";

    case ERROR_WRITE_PROTECT:
      return "The media is write protected";

    case ERROR_BAD_UNIT:
      return "The system cannot find the device specified";

    case ERROR_NOT_READY:
      return "The device is not ready";

    case ERROR_BAD_COMMAND:
      return "The device does not recognize the command";

    case ERROR_CRC:
      return "Data error (cyclic redundancy check)";

    case ERROR_BAD_LENGTH:
      return "The program issued a command but the command length is incorrect";

    case ERROR_SEEK:
      return "The drive cannot locate a specific area or track on the disk";

    case ERROR_NOT_DOS_DISK:
      return "The specified disk or diskette cannot be accessed";

    case ERROR_SECTOR_NOT_FOUND:
      return "The drive cannot find the sector requested";

    case ERROR_OUT_OF_PAPER:
      return "The printer is out of paper";

    case ERROR_WRITE_FAULT:
      return "The system cannot write to the specified device";

    case ERROR_READ_FAULT:
      return "The system cannot read from the specified device";

    case ERROR_GEN_FAILURE:
      return "A device attached to the system is not functioning";

    case ERROR_SHARING_VIOLATION:
      return "The process cannot access the file because it is being used by another process";

    case ERROR_LOCK_VIOLATION:
      return "The process cannot access the file because another process has locked a portion of the file";

    case ERROR_WRONG_DISK:
      return "The wrong diskette is in the drive";

    case ERROR_SHARING_BUFFER_EXCEEDED:
      return "Too many files opened for sharing";

    case ERROR_HANDLE_EOF:
      return "Reached the end of the file";

    case ERROR_HANDLE_DISK_FULL:
      return "The disk is full";

    case ERROR_NOT_SUPPORTED:
      return "The request is not supported";

    case ERROR_REM_NOT_LIST:
      return "Windows cannot find the network path. Verify that the network path is correct and the destination computer is not busy or turned off. If Windows still cannot find the network path, contact your network administrator";

    case ERROR_DUP_NAME:
      return "You were not connected because a duplicate name exists on the network. Go to System in Control Panel to change the computer name and try again";

    case ERROR_BAD_NETPATH:
      return "The network path was not found";

    case ERROR_NETWORK_BUSY:
      return "The network is busy";

    case ERROR_DEV_NOT_EXIST:
      return "The specified network resource or device is no longer available";

    case ERROR_TOO_MANY_CMDS:
      return "The network BIOS command limit has been reached";

    case ERROR_ADAP_HDW_ERR:
      return "A network adapter hardware error occurred";

    case ERROR_BAD_NET_RESP:
      return "The specified server cannot perform the requested operation";

    case ERROR_UNEXP_NET_ERR:
      return "An unexpected network error occurred";

    case ERROR_BAD_REM_ADAP:
      return "The remote adapter is not compatible";

    case ERROR_PRINTQ_FULL:
      return "The printer queue is full";

    case ERROR_NO_SPOOL_SPACE:
      return "Space to store the file waiting to be printed is not available on the server";

    case ERROR_PRINT_CANCELLED:
      return "Your file waiting to be printed was deleted";

    case ERROR_NETNAME_DELETED:
      return "The specified network name is no longer available";

    case ERROR_NETWORK_ACCESS_DENIED:
      return "Network access is denied";

    case ERROR_BAD_DEV_TYPE:
      return "The network resource type is not correct";

    case ERROR_BAD_NET_NAME:
      return "The network name cannot be found";

    case ERROR_TOO_MANY_NAMES:
      return "The name limit for the local computer network adapter card was exceeded";

    case ERROR_TOO_MANY_SESS:
      return "The network BIOS session limit was exceeded";

    case ERROR_SHARING_PAUSED:
      return "The remote server has been paused or is in the process of being started";

    case ERROR_REQ_NOT_ACCEP:
      return "No more connections can be made to this remote computer at this time because there are already as many connections as the computer can accept";

    case ERROR_REDIR_PAUSED:
      return "The specified printer or disk device has been paused";

    case ERROR_FILE_EXISTS:
      return "The file exists";

    case ERROR_CANNOT_MAKE:
      return "The directory or file cannot be created";

    case ERROR_FAIL_I24:
      return "Fail on INT 24";

    case ERROR_OUT_OF_STRUCTURES:
      return "Storage to process this request is not available";

    case ERROR_ALREADY_ASSIGNED:
      return "The local device name is already in use";

    case ERROR_INVALID_PASSWORD:
      return "The specified network password is not correct";

    case ERROR_INVALID_PARAMETER:
      return "The parameter is incorrect";

    case ERROR_NET_WRITE_FAULT:
      return "A write fault occurred on the network";

    case ERROR_NO_PROC_SLOTS:
      return "The system cannot start another process at this time";

    case ERROR_TOO_MANY_SEMAPHORES:
      return "Cannot create another system semaphore";

    case ERROR_EXCL_SEM_ALREADY_OWNED:
      return "The exclusive semaphore is owned by another process";

    case ERROR_SEM_IS_SET:
      return "The semaphore is set and cannot be closed";

    case ERROR_TOO_MANY_SEM_REQUESTS:
      return "The semaphore cannot be set again";

    case ERROR_INVALID_AT_INTERRUPT_TIME:
      return "Cannot request exclusive semaphores at interrupt time";

    case ERROR_SEM_OWNER_DIED:
      return "The previous ownership of this semaphore has ended";

    case ERROR_SEM_USER_LIMIT:
      return "Insert the diskette for drive %1";

    case ERROR_DISK_CHANGE:
      return "The program stopped because an alternate diskette was not inserted";

    case ERROR_DRIVE_LOCKED:
      return "The disk is in use or locked by another process";

    case ERROR_BROKEN_PIPE:
      return "The pipe has been ended";

    case ERROR_OPEN_FAILED:
      return "The system cannot open the device or file specified";

    case ERROR_BUFFER_OVERFLOW:
      return "The file name is too long";

    case ERROR_DISK_FULL:
      return "There is not enough space on the disk";

    case ERROR_NO_MORE_SEARCH_HANDLES:
      return "No more internal file identifiers available";

    case ERROR_INVALID_TARGET_HANDLE:
      return "The target internal file identifier is incorrect";

    case ERROR_INVALID_CATEGORY:
      return "The IOCTL call made by the application program is not correct";

    case ERROR_INVALID_VERIFY_SWITCH:
      return "The verify-on-write switch parameter value is not correct";

    case ERROR_BAD_DRIVER_LEVEL:
      return "The system does not support the command requested";

    case ERROR_CALL_NOT_IMPLEMENTED:
      return "This function is not supported on this system";

    case ERROR_SEM_TIMEOUT:
      return "The semaphore timeout period has expired";

    case ERROR_INSUFFICIENT_BUFFER:
      return "The data area passed to a system call is too small";

    case ERROR_INVALID_NAME:
      return "The filename, directory name, or volume label syntax is incorrect";

    case ERROR_INVALID_LEVEL:
      return "The system call level is not correct";

    case ERROR_NO_VOLUME_LABEL:
      return "The disk has no volume label";

    case ERROR_MOD_NOT_FOUND:
      return "The specified module could not be found";

    case ERROR_PROC_NOT_FOUND:
      return "The specified procedure could not be found";

    case ERROR_WAIT_NO_CHILDREN:
      return "There are no child processes to wait for";

    case ERROR_CHILD_NOT_COMPLETE:
      return "The %1 application cannot be run in Win32 mode";

    case ERROR_DIRECT_ACCESS_HANDLE:
      return "Attempt to use a file handle to an open disk partition for an operation other than raw disk I/O";

    case ERROR_NEGATIVE_SEEK:
      return "An attempt was made to move the file pointer before the beginning of the file";

    case ERROR_SEEK_ON_DEVICE:
      return "The file pointer cannot be set on the specified device or file";

    case ERROR_IS_JOIN_TARGET:
      return "A JOIN or SUBST command cannot be used for a drive that contains previously joined drives";

    case ERROR_IS_JOINED:
      return "An attempt was made to use a JOIN or SUBST command on a drive that has already been joined";

    case ERROR_IS_SUBSTED:
      return "An attempt was made to use a JOIN or SUBST command on a drive that has already been substituted";

    case ERROR_NOT_JOINED:
      return "The system tried to delete the JOIN of a drive that is not joined";

    case ERROR_NOT_SUBSTED:
      return "The system tried to delete the substitution of a drive that is not substituted";

    case ERROR_JOIN_TO_JOIN:
      return "The system tried to join a drive to a directory on a joined drive";

    case ERROR_SUBST_TO_SUBST:
      return "The system tried to substitute a drive to a directory on a substituted drive";

    case ERROR_JOIN_TO_SUBST:
      return "The system tried to join a drive to a directory on a substituted drive";

    case ERROR_SUBST_TO_JOIN:
      return "The system tried to SUBST a drive to a directory on a joined drive";

    case ERROR_BUSY_DRIVE:
      return "The system cannot perform a JOIN or SUBST at this time";

    case ERROR_SAME_DRIVE:
      return "The system cannot join or substitute a drive to or for a directory on the same drive";

    case ERROR_DIR_NOT_ROOT:
      return "The directory is not a subdirectory of the root directory";

    case ERROR_DIR_NOT_EMPTY:
      return "The directory is not empty";

    case ERROR_IS_SUBST_PATH:
      return "The path specified is being used in a substitute";

    case ERROR_IS_JOIN_PATH:
      return "Not enough resources are available to process this command";

    case ERROR_PATH_BUSY:
      return "The path specified cannot be used at this time";

    case ERROR_IS_SUBST_TARGET:
      return "An attempt was made to join or substitute a drive for which a directory on the drive is the target of a previous substitute";

    case ERROR_SYSTEM_TRACE:
      return "System trace information was not specified in your CONFIG.SYS file, or tracing is disallowed";

    case ERROR_INVALID_EVENT_COUNT:
      return "The number of specified semaphore events for DosMuxSemWait is not correct";

    case ERROR_TOO_MANY_MUXWAITERS:
      return "DosMuxSemWait did not execute; too many semaphores are already set";

    case ERROR_INVALID_LIST_FORMAT:
      return "The DosMuxSemWait list is not correct";

    case ERROR_LABEL_TOO_LONG:
      return "The volume label you entered exceeds the label character limit of the target file system";

    case ERROR_TOO_MANY_TCBS:
      return "Cannot create another thread";

    case ERROR_SIGNAL_REFUSED:
      return "The recipient process has refused the signal";

    case ERROR_DISCARDED:
      return "The segment is already discarded and cannot be locked";

    case ERROR_NOT_LOCKED:
      return "The segment is already unlocked";

    case ERROR_BAD_THREADID_ADDR:
      return "The address for the thread ID is not correct";

    case ERROR_BAD_ARGUMENTS:
      return "One or more arguments are not correct";

    case ERROR_BAD_PATHNAME:
      return "The specified path is invalid";

    case ERROR_SIGNAL_PENDING:
      return "A signal is already pending";

    case ERROR_MAX_THRDS_REACHED:
      return "No more threads can be created in the system";

    case ERROR_LOCK_FAILED:
      return "Unable to lock a region of a file";

    case ERROR_BUSY:
      return "The requested resource is in use";

    case ERROR_CANCEL_VIOLATION:
      return "A lock request was not outstanding for the supplied cancel region";

    case ERROR_ATOMIC_LOCKS_NOT_SUPPORTED:
      return "The file system does not support atomic changes to the lock type";

    case ERROR_INVALID_SEGMENT_NUMBER:
      return "The system detected a segment number that was not correct";

    case ERROR_INVALID_ORDINAL:
      return "The operating system cannot run %1";

    case ERROR_ALREADY_EXISTS:
      return "Cannot create a file when that file already exists";

    case ERROR_INVALID_FLAG_NUMBER:
      return "The flag passed is not correct";

    case ERROR_SEM_NOT_FOUND:
      return "The specified system semaphore name was not found";

    case ERROR_INVALID_STARTING_CODESEG:
      return "The operating system cannot run %1";

    case ERROR_INVALID_STACKSEG:
      return "The operating system cannot run %1";

    case ERROR_INVALID_MODULETYPE:
      return "The operating system cannot run %1";

    case ERROR_INVALID_EXE_SIGNATURE:
      return "Cannot run %1 in Win32 mode";

    case ERROR_EXE_MARKED_INVALID:
      return "The operating system cannot run %1";

    case ERROR_BAD_EXE_FORMAT:
      return "%1 is not a valid Win32 application";

    case ERROR_ITERATED_DATA_EXCEEDS_64k:
      return "The operating system cannot run %1";

    case ERROR_INVALID_MINALLOCSIZE:
      return "The operating system cannot run %1";

    case ERROR_DYNLINK_FROM_INVALID_RING:
      return "The operating system cannot run this application program";

    case ERROR_IOPL_NOT_ENABLED:
      return "The operating system is not presently configured to run this application";

    case ERROR_INVALID_SEGDPL:
      return "The operating system cannot run %1";

    case ERROR_AUTODATASEG_EXCEEDS_64k:
      return "The operating system cannot run this application program";

    case ERROR_RING2SEG_MUST_BE_MOVABLE:
      return "The code segment cannot be greater than or equal to 64K";

    case ERROR_RELOC_CHAIN_XEEDS_SEGLIM:
      return "The operating system cannot run %1";

    case ERROR_INFLOOP_IN_RELOC_CHAIN:
      return "The operating system cannot run %1";

    case ERROR_ENVVAR_NOT_FOUND:
      return "The system could not find the environment option that was entered";

    case ERROR_NO_SIGNAL_SENT:
      return "No process in the command subtree has a signal handler";

    case ERROR_FILENAME_EXCED_RANGE:
      return "The filename or extension is too long";

    case ERROR_RING2_STACK_IN_USE:
      return "The ring 2 stack is in use";

    case ERROR_META_EXPANSION_TOO_LONG:
      return "The global filename characters, * or ?, are entered incorrectly or too many global filename characters are specified";

    case ERROR_INVALID_SIGNAL_NUMBER:
      return "The signal being posted is not correct";

    case ERROR_THREAD_1_INACTIVE:
      return "The signal handler cannot be set";

    case ERROR_LOCKED:
      return "The segment is locked and cannot be reallocated";

    case ERROR_TOO_MANY_MODULES:
      return "Too many dynamic-link modules are attached to this program or dynamic-link module";

    case ERROR_NESTING_NOT_ALLOWED:
      return "Cannot nest calls to LoadModule";

    case ERROR_EXE_MACHINE_TYPE_MISMATCH:
      return "The image file %1 is valid, but is for a machine type other than the current machine";

    case ERROR_EXE_CANNOT_MODIFY_SIGNED_BINARY:
      return "The image file %1 is signed, unable to modify";

    case ERROR_EXE_CANNOT_MODIFY_STRONG_SIGNED_BINARY:
      return "The image file %1 is strong signed, unable to modify";

    case ERROR_BAD_PIPE:
      return "The pipe state is invalid";

    case ERROR_PIPE_BUSY:
      return "All pipe instances are busy";

    case ERROR_NO_DATA:
      return "The pipe is being closed";

    case ERROR_PIPE_NOT_CONNECTED:
      return "No process is on the other end of the pipe";

    case ERROR_MORE_DATA:
      return "More data is available";

    case ERROR_VC_DISCONNECTED:
      return "The session was canceled";

    case ERROR_INVALID_EA_NAME:
      return "The specified extended attribute name was invalid";

    case ERROR_EA_LIST_INCONSISTENT:
      return "The extended attributes are inconsistent";

    case WAIT_TIMEOUT:
      return "The wait operation timed out";

    case ERROR_NO_MORE_ITEMS:
      return "No more data is available";

    case ERROR_CANNOT_COPY:
      return "The copy functions cannot be used";

    case ERROR_DIRECTORY:
      return "The directory name is invalid";

    case ERROR_EAS_DIDNT_FIT:
      return "The extended attributes did not fit in the buffer";

    case ERROR_EA_FILE_CORRUPT:
      return "The extended attribute file on the mounted file system is corrupt";

    case ERROR_EA_TABLE_FULL:
      return "The extended attribute table file is full";

    case ERROR_INVALID_EA_HANDLE:
      return "The specified extended attribute handle is invalid";

    case ERROR_EAS_NOT_SUPPORTED:
      return "The mounted file system does not support extended attributes";

    case ERROR_NOT_OWNER:
      return "Attempt to release mutex not owned by caller";

    case ERROR_TOO_MANY_POSTS:
      return "Too many posts were made to a semaphore";

    case ERROR_PARTIAL_COPY:
      return "Only part of a ReadProcessMemory or WriteProcessMemory request was completed";

    case ERROR_OPLOCK_NOT_GRANTED:
      return "The oplock request is denied";

    case ERROR_INVALID_OPLOCK_PROTOCOL:
      return "An invalid oplock acknowledgment was received by the system";

    case ERROR_DISK_TOO_FRAGMENTED:
      return "The volume is too fragmented to complete this operation";

    case ERROR_DELETE_PENDING:
      return "The file cannot be opened because it is in the process of being deleted";

    case ERROR_MR_MID_NOT_FOUND:
      return "The system cannot find message text for message number 0x%1 in the message file for %2";

    case ERROR_SCOPE_NOT_FOUND:
      return "The scope specified was not found";

    case ERROR_INVALID_ADDRESS:
      return "Attempt to access invalid address";

    case ERROR_ARITHMETIC_OVERFLOW:
      return "Arithmetic result exceeded 32 bits";

    case ERROR_PIPE_CONNECTED:
      return "There is a process on other end of the pipe";

    case ERROR_PIPE_LISTENING:
      return "Waiting for a process to open the other end of the pipe";

    case ERROR_EA_ACCESS_DENIED:
      return "Access to the extended attribute was denied";

    case ERROR_OPERATION_ABORTED:
      return "The I/O operation has been aborted because of either a thread exit or an application request";

    case ERROR_IO_INCOMPLETE:
      return "Overlapped I/O event is not in a signaled state";

    case ERROR_IO_PENDING:
      return "Overlapped I/O operation is in progress";

    case ERROR_NOACCESS:
      return "Invalid access to memory location";

    case ERROR_SWAPERROR:
      return "Error performing inpage operation";

    case ERROR_STACK_OVERFLOW:
      return "Recursion too deep; the stack overflowed";

    case ERROR_INVALID_MESSAGE:
      return "The window cannot act on the sent message";

    case ERROR_CAN_NOT_COMPLETE:
      return "Cannot complete this function";

    case ERROR_INVALID_FLAGS:
      return "Invalid flags";

    case ERROR_UNRECOGNIZED_VOLUME:
      return "The volume does not contain a recognized file system";

    case ERROR_FILE_INVALID:
      return "The volume for a file has been externally altered so that the opened file is no longer valid";

    case ERROR_FULLSCREEN_MODE:
      return "The requested operation cannot be performed in full-screen mode";

    case ERROR_NO_TOKEN:
      return "An attempt was made to reference a token that does not exist";

    case ERROR_BADDB:
      return "The configuration registry database is corrupt";

    case ERROR_BADKEY:
      return "The configuration registry key is invalid";

    case ERROR_CANTOPEN:
      return "The configuration registry key could not be opened";

    case ERROR_CANTREAD:
      return "The configuration registry key could not be read";

    case ERROR_CANTWRITE:
      return "The configuration registry key could not be written";

    case ERROR_REGISTRY_RECOVERED:
      return "One of the files in the registry database had to be recovered by use of a log or alternate copy. The recovery was successful";

    case ERROR_REGISTRY_CORRUPT:
      return "The registry is corrupted. The structure of one of the files containing registry data is corrupted, or the system's memory image of the file is corrupted, or the file could not be recovered because the alternate copy or log was absent or corrupted";

    case ERROR_REGISTRY_IO_FAILED:
      return "An I/O operation initiated by the registry failed unrecoverably. The registry could not read in, or write out, or flush, one of the files that contain the system's image of the registry";

    case ERROR_NOT_REGISTRY_FILE:
      return "The system has attempted to load or restore a file into the registry, but the specified file is not in a registry file format";

    case ERROR_KEY_DELETED:
      return "Illegal operation attempted on a registry key that has been marked for deletion";

    case ERROR_NO_LOG_SPACE:
      return "System could not allocate the required space in a registry log";

    case ERROR_KEY_HAS_CHILDREN:
      return "Cannot create a symbolic link in a registry key that already has subkeys or values";

    case ERROR_CHILD_MUST_BE_VOLATILE:
      return "Cannot create a stable subkey under a volatile parent key";

    case ERROR_NOTIFY_ENUM_DIR:
      return "A notify change request is being completed and the information is not being returned in the caller's buffer. The caller now needs to enumerate the files to find the changes";

    case ERROR_DEPENDENT_SERVICES_RUNNING:
      return "A stop control has been sent to a service that other running services are dependent on";

    case ERROR_INVALID_SERVICE_CONTROL:
      return "The requested control is not valid for this service";

    case ERROR_SERVICE_REQUEST_TIMEOUT:
      return "The service did not respond to the start or control request in a timely fashion";

    case ERROR_SERVICE_NO_THREAD:
      return "A thread could not be created for the service";

    case ERROR_SERVICE_DATABASE_LOCKED:
      return "The service database is locked";

    case ERROR_SERVICE_ALREADY_RUNNING:
      return "An instance of the service is already running";

    case ERROR_INVALID_SERVICE_ACCOUNT:
      return "The account name is invalid or does not exist, or the password is invalid for the account name specified";

    case ERROR_SERVICE_DISABLED:
      return "The service cannot be started, either because it is disabled or because it has no enabled devices associated with it";

    case ERROR_CIRCULAR_DEPENDENCY:
      return "Circular service dependency was specified";

    case ERROR_SERVICE_DOES_NOT_EXIST:
      return "The specified service does not exist as an installed service";

    case ERROR_SERVICE_CANNOT_ACCEPT_CTRL:
      return "The service cannot accept control messages at this time";

    case ERROR_SERVICE_NOT_ACTIVE:
      return "The service has not been started";

    case ERROR_FAILED_SERVICE_CONTROLLER_CONNECT:
      return "The service process could not connect to the service controller";

    case ERROR_EXCEPTION_IN_SERVICE:
      return "An exception occurred in the service when handling the control request";

    case ERROR_DATABASE_DOES_NOT_EXIST:
      return "The database specified does not exist";

    case ERROR_SERVICE_SPECIFIC_ERROR:
      return "The service has returned a service-specific error code";

    case ERROR_PROCESS_ABORTED:
      return "The process terminated unexpectedly";

    case ERROR_SERVICE_DEPENDENCY_FAIL:
      return "The dependency service or group failed to start";

    case ERROR_SERVICE_LOGON_FAILED:
      return "The service did not start due to a logon failure";

    case ERROR_SERVICE_START_HANG:
      return "After starting, the service hung in a start-pending state";

    case ERROR_INVALID_SERVICE_LOCK:
      return "The specified service database lock is invalid";

    case ERROR_SERVICE_MARKED_FOR_DELETE:
      return "The specified service has been marked for deletion";

    case ERROR_SERVICE_EXISTS:
      return "The specified service already exists";

    case ERROR_ALREADY_RUNNING_LKG:
      return "The system is currently running with the last-known-good configuration";

    case ERROR_SERVICE_DEPENDENCY_DELETED:
      return "The dependency service does not exist or has been marked for deletion";

    case ERROR_BOOT_ALREADY_ACCEPTED:
      return "The current boot has already been accepted for use as the last-known-good control set";

    case ERROR_SERVICE_NEVER_STARTED:
      return "No attempts to start the service have been made since the last boot";

    case ERROR_DUPLICATE_SERVICE_NAME:
      return "The name is already in use as either a service name or a service display name";

    case ERROR_DIFFERENT_SERVICE_ACCOUNT:
      return "The account specified for this service is different from the account specified for other services running in the same process";

    case ERROR_CANNOT_DETECT_DRIVER_FAILURE:
      return "Failure actions can only be set for Win32 services, not for drivers";

    case ERROR_CANNOT_DETECT_PROCESS_ABORT:
      return "This service runs in the same process as the service control manager";

    case ERROR_NO_RECOVERY_PROGRAM:
      return "No recovery program has been configured for this service";

    case ERROR_SERVICE_NOT_IN_EXE:
      return "The executable program that this service is configured to run in does not implement the service";

    case ERROR_NOT_SAFEBOOT_SERVICE:
      return "This service cannot be started in Safe Mode";

    case ERROR_END_OF_MEDIA:
      return "The physical end of the tape has been reached";

    case ERROR_FILEMARK_DETECTED:
      return "A tape access reached a filemark";

    case ERROR_BEGINNING_OF_MEDIA:
      return "The beginning of the tape or a partition was encountered";

    case ERROR_SETMARK_DETECTED:
      return "A tape access reached the end of a set of files";

    case ERROR_NO_DATA_DETECTED:
      return "No more data is on the tape";

    case ERROR_PARTITION_FAILURE:
      return "Tape could not be partitioned";

    case ERROR_INVALID_BLOCK_LENGTH:
      return "When accessing a new tape of a multivolume partition, the current block size is incorrect";

    case ERROR_DEVICE_NOT_PARTITIONED:
      return "Tape partition information could not be found when loading a tape";

    case ERROR_UNABLE_TO_LOCK_MEDIA:
      return "Unable to lock the media eject mechanism";

    case ERROR_UNABLE_TO_UNLOAD_MEDIA:
      return "Unable to unload the media";

    case ERROR_MEDIA_CHANGED:
      return "The media in the drive may have changed";

    case ERROR_BUS_RESET:
      return "The I/O bus was reset";

    case ERROR_NO_MEDIA_IN_DRIVE:
      return "No media in drive";

    case ERROR_NO_UNICODE_TRANSLATION:
      return "No mapping for the Unicode character exists in the target multi-byte code page";

    case ERROR_DLL_INIT_FAILED:
      return "A dynamic link library (DLL) initialization routine failed";

    case ERROR_SHUTDOWN_IN_PROGRESS:
      return "A system shutdown is in progress";

    case ERROR_NO_SHUTDOWN_IN_PROGRESS:
      return "Unable to abort the system shutdown because no shutdown was in progress";

    case ERROR_IO_DEVICE:
      return "The request could not be performed because of an I/O device error";

    case ERROR_SERIAL_NO_DEVICE:
      return "No serial device was successfully initialized. The serial driver will unload";

    case ERROR_IRQ_BUSY:
      return "Unable to open a device that was sharing an interrupt request (IRQ) with other devices. At least one other device that uses that IRQ was already opened";

    case ERROR_MORE_WRITES:
      return "A serial I/O operation was completed by another write to the serial port";

    case ERROR_COUNTER_TIMEOUT:
      return "A serial I/O operation completed because the timeout period expired";

    case ERROR_FLOPPY_ID_MARK_NOT_FOUND:
      return "No ID address mark was found on the floppy disk";

    case ERROR_FLOPPY_WRONG_CYLINDER:
      return "Mismatch between the floppy disk sector ID field and the floppy disk controller track address";

    case ERROR_FLOPPY_UNKNOWN_ERROR:
      return "The floppy disk controller reported an error that is not recognized by the floppy disk driver";

    case ERROR_FLOPPY_BAD_REGISTERS:
      return "The floppy disk controller returned inconsistent results in its registers";

    case ERROR_DISK_RECALIBRATE_FAILED:
      return "While accessing the hard disk, a recalibrate operation failed, even after retries";

    case ERROR_DISK_OPERATION_FAILED:
      return "While accessing the hard disk, a disk operation failed even after retries";

    case ERROR_DISK_RESET_FAILED:
      return "While accessing the hard disk, a disk controller reset was needed, but even that failed";

    case ERROR_EOM_OVERFLOW:
      return "Physical end of tape encountered";

    case ERROR_NOT_ENOUGH_SERVER_MEMORY:
      return "Not enough server storage is available to process this command";

    case ERROR_POSSIBLE_DEADLOCK:
      return "A potential deadlock condition has been detected";

    case ERROR_MAPPED_ALIGNMENT:
      return "The base address or the file offset specified does not have the proper alignment";

    case ERROR_SET_POWER_STATE_VETOED:
      return "An attempt to change the system power state was vetoed by another application or driver";

    case ERROR_SET_POWER_STATE_FAILED:
      return "The system BIOS failed an attempt to change the system power state";

    case ERROR_TOO_MANY_LINKS:
      return "An attempt was made to create more links on a file than the file system supports";

    case ERROR_OLD_WIN_VERSION:
      return "The specified program requires a newer version of Windows";

    case ERROR_APP_WRONG_OS:
      return "The specified program is not a Windows or MS-DOS program";

    case ERROR_SINGLE_INSTANCE_APP:
      return "Cannot start more than one instance of the specified program";

    case ERROR_RMODE_APP:
      return "The specified program was written for an earlier version of Windows";

    case ERROR_INVALID_DLL:
      return "One of the library files needed to run this application is damaged";

    case ERROR_NO_ASSOCIATION:
      return "No application is associated with the specified file for this operation";

    case ERROR_DDE_FAIL:
      return "An error occurred in sending the command to the application";

    case ERROR_DLL_NOT_FOUND:
      return "One of the library files needed to run this application cannot be found";

    case ERROR_NO_MORE_USER_HANDLES:
      return "The current process has used all of its system allowance of handles for Window Manager objects";

    case ERROR_MESSAGE_SYNC_ONLY:
      return "The message can be used only with synchronous operations";

    case ERROR_SOURCE_ELEMENT_EMPTY:
      return "The indicated source element has no media";

    case ERROR_DESTINATION_ELEMENT_FULL:
      return "The indicated destination element already contains media";

    case ERROR_ILLEGAL_ELEMENT_ADDRESS:
      return "The indicated element does not exist";

    case ERROR_MAGAZINE_NOT_PRESENT:
      return "The indicated element is part of a magazine that is not present";

    case ERROR_DEVICE_REINITIALIZATION_NEEDED:
      return "The indicated device requires reinitialization due to hardware errors";

    case ERROR_DEVICE_REQUIRES_CLEANING:
      return "The device has indicated that cleaning is required before further operations are attempted";

    case ERROR_DEVICE_DOOR_OPEN:
      return "The device has indicated that its door is open";

    case ERROR_DEVICE_NOT_CONNECTED:
      return "The device is not connected";

    case ERROR_NOT_FOUND:
      return "Element not found";

    case ERROR_NO_MATCH:
      return "There was no match for the specified key in the index";

    case ERROR_SET_NOT_FOUND:
      return "The property set specified does not exist on the object";

    case ERROR_POINT_NOT_FOUND:
      return "The point passed to GetMouseMovePoints is not in the buffer";

    case ERROR_NO_TRACKING_SERVICE:
      return "The tracking (workstation) service is not running";

    case ERROR_NO_VOLUME_ID:
      return "The Volume ID could not be found";

    case ERROR_UNABLE_TO_REMOVE_REPLACED:
      return "Unable to remove the file to be replaced";

    case ERROR_UNABLE_TO_MOVE_REPLACEMENT:
      return "Unable to move the replacement file to the file to be replaced. The file to be replaced has retained its original name";

    case ERROR_UNABLE_TO_MOVE_REPLACEMENT_2:
      return "Unable to move the replacement file to the file to be replaced. The file to be replaced has been renamed using the backup name";

    case ERROR_JOURNAL_DELETE_IN_PROGRESS:
      return "The volume change journal is being deleted";

    case ERROR_JOURNAL_NOT_ACTIVE:
      return "The volume change journal is not active";

    case ERROR_POTENTIAL_FILE_FOUND:
      return "A file was found, but it may not be the correct file";

    case ERROR_JOURNAL_ENTRY_DELETED:
      return "The journal entry has been deleted from the journal";

    case ERROR_BAD_DEVICE:
      return "The specified device name is invalid";

    case ERROR_CONNECTION_UNAVAIL:
      return "The device is not currently connected but it is a remembered connection";

    case ERROR_DEVICE_ALREADY_REMEMBERED:
      return "The local device name has a remembered connection to another network resource";

    case ERROR_NO_NET_OR_BAD_PATH:
      return "No network provider accepted the given network path";

    case ERROR_BAD_PROVIDER:
      return "The specified network provider name is invalid";

    case ERROR_CANNOT_OPEN_PROFILE:
      return "Unable to open the network connection profile";

    case ERROR_BAD_PROFILE:
      return "The network connection profile is corrupted";

    case ERROR_NOT_CONTAINER:
      return "Cannot enumerate a noncontainer";

    case ERROR_EXTENDED_ERROR:
      return "An extended error has occurred";

    case ERROR_INVALID_GROUPNAME:
      return "The format of the specified group name is invalid";

    case ERROR_INVALID_COMPUTERNAME:
      return "The format of the specified computer name is invalid";

    case ERROR_INVALID_EVENTNAME:
      return "The format of the specified event name is invalid";

    case ERROR_INVALID_DOMAINNAME:
      return "The format of the specified domain name is invalid";

    case ERROR_INVALID_SERVICENAME:
      return "The format of the specified service name is invalid";

    case ERROR_INVALID_NETNAME:
      return "The format of the specified network name is invalid";

    case ERROR_INVALID_SHARENAME:
      return "The format of the specified share name is invalid";

    case ERROR_INVALID_PASSWORDNAME:
      return "The format of the specified password is invalid";

    case ERROR_INVALID_MESSAGENAME:
      return "The format of the specified message name is invalid";

    case ERROR_INVALID_MESSAGEDEST:
      return "The format of the specified message destination is invalid";

    case ERROR_SESSION_CREDENTIAL_CONFLICT:
      return "Multiple connections to a server or shared resource by the same user, using more than one user name, are not allowed. Disconnect all previous connections to the server or shared resource and try again";

    case ERROR_REMOTE_SESSION_LIMIT_EXCEEDED:
      return "An attempt was made to establish a session to a network server, but there are already too many sessions established to that server";

    case ERROR_DUP_DOMAINNAME:
      return "The workgroup or domain name is already in use by another computer on the network";

    case ERROR_NO_NETWORK:
      return "The network is not present or not started";

    case ERROR_CANCELLED:
      return "The operation was canceled by the user";

    case ERROR_USER_MAPPED_FILE:
      return "The requested operation cannot be performed on a file with a user-mapped section open";

    case ERROR_CONNECTION_REFUSED:
      return "The remote system refused the network connection";

    case ERROR_GRACEFUL_DISCONNECT:
      return "The network connection was gracefully closed";

    case ERROR_ADDRESS_ALREADY_ASSOCIATED:
      return "The network transport endpoint already has an address associated with it";

    case ERROR_ADDRESS_NOT_ASSOCIATED:
      return "An address has not yet been associated with the network endpoint";

    case ERROR_CONNECTION_INVALID:
      return "An operation was attempted on a nonexistent network connection";

    case ERROR_CONNECTION_ACTIVE:
      return "An invalid operation was attempted on an active network connection";

    case ERROR_NETWORK_UNREACHABLE:
      return "The network location cannot be reached. For information about network troubleshooting, see Windows Help";

    case ERROR_HOST_UNREACHABLE:
      return "The network location cannot be reached. For information about network troubleshooting, see Windows Help";

    case ERROR_PROTOCOL_UNREACHABLE:
      return "The network location cannot be reached. For information about network troubleshooting, see Windows Help";

    case ERROR_PORT_UNREACHABLE:
      return "No service is operating at the destination network endpoint on the remote system";

    case ERROR_REQUEST_ABORTED:
      return "The request was aborted";

    case ERROR_CONNECTION_ABORTED:
      return "The network connection was aborted by the local system";

    case ERROR_RETRY:
      return "The operation could not be completed. A retry should be performed";

    case ERROR_CONNECTION_COUNT_LIMIT:
      return "A connection to the server could not be made because the limit on the number of concurrent connections for this account has been reached";

    case ERROR_LOGIN_TIME_RESTRICTION:
      return "Attempting to log in during an unauthorized time of day for this account";

    case ERROR_LOGIN_WKSTA_RESTRICTION:
      return "The account is not authorized to log in from this station";

    case ERROR_INCORRECT_ADDRESS:
      return "The network address could not be used for the operation requested";

    case ERROR_ALREADY_REGISTERED:
      return "The service is already registered";

    case ERROR_SERVICE_NOT_FOUND:
      return "The specified service does not exist";

    case ERROR_NOT_AUTHENTICATED:
      return "The operation being requested was not performed because the user has not been authenticated";

    case ERROR_NOT_LOGGED_ON:
      return "The operation being requested was not performed because the user has not logged on to the network";

    case ERROR_CONTINUE:
      return "Continue with work in progress";

    case ERROR_ALREADY_INITIALIZED:
      return "An attempt was made to perform an initialization operation when initialization has already been completed";

    case ERROR_NO_MORE_DEVICES:
      return "No more local devices";

    case ERROR_NO_SUCH_SITE:
      return "The specified site does not exist";

    case ERROR_DOMAIN_CONTROLLER_EXISTS:
      return "A domain controller with the specified name already exists";

    case ERROR_ONLY_IF_CONNECTED:
      return "This operation is supported only when you are connected to the server";

    case ERROR_OVERRIDE_NOCHANGES:
      return "The group policy framework should call the extension even if there are no changes";

    case ERROR_BAD_USER_PROFILE:
      return "The specified user does not have a valid profile";

    case ERROR_NOT_SUPPORTED_ON_SBS:
      return "This operation is not supported on a computer running Windows Server 2003 for Small Business Server";

    case ERROR_SERVER_SHUTDOWN_IN_PROGRESS:
      return "The server machine is shutting down";

    case ERROR_HOST_DOWN:
      return "The remote system is not available. For information about network troubleshooting, see Windows Help";

    case ERROR_NON_ACCOUNT_SID:
      return "The security identifier provided is not from an account domain";

    case ERROR_NON_DOMAIN_SID:
      return "The security identifier provided does not have a domain component";

    case ERROR_APPHELP_BLOCK:
      return "AppHelp dialog canceled thus preventing the application from starting";

    case ERROR_ACCESS_DISABLED_BY_POLICY:
      return "Windows cannot open this program because it has been prevented by a software restriction policy. For more information, open Event Viewer or contact your system administrator";

    case ERROR_REG_NAT_CONSUMPTION:
      return "A program attempt to use an invalid value.  Normally caused by an uninitialized register. This error is Itanium specific";

    case ERROR_CSCSHARE_OFFLINE:
      return "The share is currently offline or does not exist";

    case ERROR_PKINIT_FAILURE:
      return "The kerberos protocol encountered an error while validating the";

    case ERROR_SMARTCARD_SUBSYSTEM_FAILURE:
      return "The kerberos protocol encountered an error while attempting to utilize";

    case ERROR_DOWNGRADE_DETECTED:
      return "The system detected a possible attempt to compromise security. Please ensure that you can contact the server that authenticated you";

    case ERROR_MACHINE_LOCKED:
      return "The machine is locked and can not be shut down without the force option";

    case ERROR_CALLBACK_SUPPLIED_INVALID_DATA:
      return "An application-defined callback gave invalid data when called";

    case ERROR_SYNC_FOREGROUND_REFRESH_REQUIRED:
      return "The group policy framework should call the extension in the synchronous foreground policy refresh";

    case ERROR_DRIVER_BLOCKED:
      return "This driver has been blocked from loading";

    case ERROR_INVALID_IMPORT_OF_NON_DLL:
      return "A dynamic link library (DLL) referenced a module that was neither a DLL nor the process's executable image";

    case ERROR_ACCESS_DISABLED_WEBBLADE:
      return "Windows cannot open this program since it has been disabled";

    case ERROR_ACCESS_DISABLED_WEBBLADE_TAMPER:
      return "Windows cannot open this program because the license enforcement system has been tampered with or become corrupted";

    case ERROR_RECOVERY_FAILURE:
      return "A transaction recover failed";

    case ERROR_ALREADY_FIBER:
      return "The current thread has already been converted to a fiber";

    case ERROR_ALREADY_THREAD:
      return "The current thread has already been converted from a fiber";

    case ERROR_STACK_BUFFER_OVERRUN:
      return "The system detected an overrun of a stack-based buffer in this application.  This";

    case ERROR_PARAMETER_QUOTA_EXCEEDED:
      return "Data present in one of the parameters is more than the function can operate on";

    case ERROR_DEBUGGER_INACTIVE:
      return "An attempt to do an operation on a debug object failed because the object is in the process of being deleted";

    case ERROR_DELAY_LOAD_FAILED:
      return "An attempt to delay-load a .dll or get a function address in a delay-loaded .dll failed";

    case ERROR_VDM_DISALLOWED:
      return "%1 is a 16-bit application. You do not have permissions to execute 16-bit applications. Check your permissions with your system administrator";

    case ERROR_UNIDENTIFIED_ERROR:
      return "Insufficient information exists to identify the cause of failure";

    case ERROR_NOT_ALL_ASSIGNED:
      return "Not all privileges referenced are assigned to the caller";

    case ERROR_SOME_NOT_MAPPED:
      return "Some mapping between account names and security IDs was not done";

    case ERROR_NO_QUOTAS_FOR_ACCOUNT:
      return "No system quota limits are specifically set for this account";

    case ERROR_LOCAL_USER_SESSION_KEY:
      return "No encryption key is available. A well-known encryption key was returned";

    case ERROR_NULL_LM_PASSWORD:
      return "The password is too complex to be converted to a LAN Manager password. The LAN Manager password returned is a NULL string";

    case ERROR_UNKNOWN_REVISION:
      return "The revision level is unknown";

    case ERROR_REVISION_MISMATCH:
      return "Indicates two revision levels are incompatible";

    case ERROR_INVALID_OWNER:
      return "This security ID may not be assigned as the owner of this object";

    case ERROR_INVALID_PRIMARY_GROUP:
      return "This security ID may not be assigned as the primary group of an object";

    case ERROR_NO_IMPERSONATION_TOKEN:
      return "An attempt has been made to operate on an impersonation token by a thread that is not currently impersonating a client";

    case ERROR_CANT_DISABLE_MANDATORY:
      return "The group may not be disabled";

    case ERROR_NO_LOGON_SERVERS:
      return "There are currently no logon servers available to service the logon request";

    case ERROR_NO_SUCH_LOGON_SESSION:
      return "A specified logon session does not exist. It may already have been terminated";

    case ERROR_NO_SUCH_PRIVILEGE:
      return "A specified privilege does not exist";

    case ERROR_PRIVILEGE_NOT_HELD:
      return "A required privilege is not held by the client";

    case ERROR_INVALID_ACCOUNT_NAME:
      return "The name provided is not a properly formed account name";

    case ERROR_USER_EXISTS:
      return "The specified user already exists";

    case ERROR_NO_SUCH_USER:
      return "The specified user does not exist";

    case ERROR_GROUP_EXISTS:
      return "The specified group already exists";

    case ERROR_NO_SUCH_GROUP:
      return "The specified group does not exist";

    case ERROR_MEMBER_IN_GROUP:
      return "Either the specified user account is already a member of the specified group, or the specified group cannot be deleted because it contains a member";

    case ERROR_MEMBER_NOT_IN_GROUP:
      return "The specified user account is not a member of the specified group account";

    case ERROR_LAST_ADMIN:
      return "The last remaining administration account cannot be disabled or deleted";

    case ERROR_WRONG_PASSWORD:
      return "Unable to update the password. The value provided as the current password is incorrect";

    case ERROR_ILL_FORMED_PASSWORD:
      return "Unable to update the password. The value provided for the new password contains values that are not allowed in passwords";

    case ERROR_PASSWORD_RESTRICTION:
      return "Unable to update the password. The value provided for the new password does not meet the length, complexity, or history requirement of the domain";

    case ERROR_LOGON_FAILURE:
      return "Logon failure: unknown user name or bad password";

    case ERROR_ACCOUNT_RESTRICTION:
      return "Logon failure: user account restriction.  Possible reasons are blank passwords not allowed, logon hour restrictions, or a policy restriction has been enforced";

    case ERROR_INVALID_LOGON_HOURS:
      return "Logon failure: account logon time restriction violation";

    case ERROR_INVALID_WORKSTATION:
      return "Logon failure: user not allowed to log on to this computer";

    case ERROR_PASSWORD_EXPIRED:
      return "Logon failure: the specified account password has expired";

    case ERROR_ACCOUNT_DISABLED:
      return "Logon failure: account currently disabled";

    case ERROR_NONE_MAPPED:
      return "No mapping between account names and security IDs was done";

    case ERROR_TOO_MANY_LUIDS_REQUESTED:
      return "Too many local user identifiers (LUIDs) were requested at one time";

    case ERROR_LUIDS_EXHAUSTED:
      return "No more local user identifiers (LUIDs) are available";

    case ERROR_INVALID_SUB_AUTHORITY:
      return "The subauthority part of a security ID is invalid for this particular use";

    case ERROR_INVALID_ACL:
      return "The access control list (ACL) structure is invalid";

    case ERROR_INVALID_SID:
      return "The security ID structure is invalid";

    case ERROR_INVALID_SECURITY_DESCR:
      return "The security descriptor structure is invalid";

    case ERROR_BAD_INHERITANCE_ACL:
      return "The inherited access control list (ACL) or access control entry (ACE) could not be built";

    case ERROR_SERVER_DISABLED:
      return "The server is currently disabled";

    case ERROR_SERVER_NOT_DISABLED:
      return "The server is currently enabled";

    case ERROR_INVALID_ID_AUTHORITY:
      return "The value provided was an invalid value for an identifier authority";

    case ERROR_ALLOTTED_SPACE_EXCEEDED:
      return "No more memory is available for security information updates";

    case ERROR_INVALID_GROUP_ATTRIBUTES:
      return "The specified attributes are invalid, or incompatible with the attributes for the group as a whole";

    case ERROR_BAD_IMPERSONATION_LEVEL:
      return "Either a required impersonation level was not provided, or the provided impersonation level is invalid";

    case ERROR_CANT_OPEN_ANONYMOUS:
      return "Cannot open an anonymous level security token";

    case ERROR_BAD_VALIDATION_CLASS:
      return "The validation information class requested was invalid";

    case ERROR_BAD_TOKEN_TYPE:
      return "The type of the token is inappropriate for its attempted use";

    case ERROR_NO_SECURITY_ON_OBJECT:
      return "Unable to perform a security operation on an object that has no associated security";

    case ERROR_CANT_ACCESS_DOMAIN_INFO:
      return "Configuration information could not be read from the domain controller, either because the machine is unavailable, or access has been denied";

    case ERROR_INVALID_SERVER_STATE:
      return "The security account manager (SAM) or local security authority (LSA) server was in the wrong state to perform the security operation";

    case ERROR_INVALID_DOMAIN_STATE:
      return "The domain was in the wrong state to perform the security operation";

    case ERROR_INVALID_DOMAIN_ROLE:
      return "This operation is only allowed for the Primary Domain Controller of the domain";

    case ERROR_NO_SUCH_DOMAIN:
      return "The specified domain either does not exist or could not be contacted";

    case ERROR_DOMAIN_EXISTS:
      return "The specified domain already exists";

    case ERROR_DOMAIN_LIMIT_EXCEEDED:
      return "An attempt was made to exceed the limit on the number of domains per server";

    case ERROR_INTERNAL_DB_CORRUPTION:
      return "Unable to complete the requested operation because of either a catastrophic media failure or a data structure corruption on the disk";

    case ERROR_INTERNAL_ERROR:
      return "An internal error occurred";

    case ERROR_GENERIC_NOT_MAPPED:
      return "Generic access types were contained in an access mask which should already be mapped to nongeneric types";

    case ERROR_BAD_DESCRIPTOR_FORMAT:
      return "A security descriptor is not in the right format (absolute or self-relative)";

    case ERROR_NOT_LOGON_PROCESS:
      return "The requested action is restricted for use by logon processes only. The calling process has not registered as a logon process";

    case ERROR_LOGON_SESSION_EXISTS:
      return "Cannot start a new logon session with an ID that is already in use";

    case ERROR_NO_SUCH_PACKAGE:
      return "A specified authentication package is unknown";

    case ERROR_BAD_LOGON_SESSION_STATE:
      return "The logon session is not in a state that is consistent with the requested operation";

    case ERROR_LOGON_SESSION_COLLISION:
      return "The logon session ID is already in use";

    case ERROR_INVALID_LOGON_TYPE:
      return "A logon request contained an invalid logon type value";

    case ERROR_CANNOT_IMPERSONATE:
      return "Unable to impersonate using a named pipe until data has been read from that pipe";

    case ERROR_RXACT_INVALID_STATE:
      return "The transaction state of a registry subtree is incompatible with the requested operation";

    case ERROR_RXACT_COMMIT_FAILURE:
      return "An internal security database corruption has been encountered";

    case ERROR_SPECIAL_ACCOUNT:
      return "Cannot perform this operation on built-in accounts";

    case ERROR_SPECIAL_GROUP:
      return "Cannot perform this operation on this built-in special group";

    case ERROR_SPECIAL_USER:
      return "Cannot perform this operation on this built-in special user";

    case ERROR_MEMBERS_PRIMARY_GROUP:
      return "The user cannot be removed from a group because the group is currently the user's primary group";

    case ERROR_TOKEN_ALREADY_IN_USE:
      return "The token is already in use as a primary token";

    case ERROR_NO_SUCH_ALIAS:
      return "The specified local group does not exist";

    case ERROR_MEMBER_NOT_IN_ALIAS:
      return "The specified account name is not a member of the local group";

    case ERROR_MEMBER_IN_ALIAS:
      return "The specified account name is already a member of the local group";

    case ERROR_ALIAS_EXISTS:
      return "The specified local group already exists";

    case ERROR_LOGON_NOT_GRANTED:
      return "Logon failure: the user has not been granted the requested logon type at this computer";

    case ERROR_TOO_MANY_SECRETS:
      return "The maximum number of secrets that may be stored in a single system has been exceeded";

    case ERROR_SECRET_TOO_LONG:
      return "The length of a secret exceeds the maximum length allowed";

    case ERROR_INTERNAL_DB_ERROR:
      return "The local security authority database contains an internal inconsistency";

    case ERROR_TOO_MANY_CONTEXT_IDS:
      return "During a logon attempt, the user's security context accumulated too many security IDs";

    case ERROR_LOGON_TYPE_NOT_GRANTED:
      return "Logon failure: the user has not been granted the requested logon type at this computer";

    case ERROR_NT_CROSS_ENCRYPTION_REQUIRED:
      return "A cross-encrypted password is necessary to change a user password";

    case ERROR_NO_SUCH_MEMBER:
      return "A member could not be added to or removed from the local group because the member does not exist";

    case ERROR_INVALID_MEMBER:
      return "A new member could not be added to a local group because the member has the wrong account type";

    case ERROR_TOO_MANY_SIDS:
      return "Too many security IDs have been specified";

    case ERROR_LM_CROSS_ENCRYPTION_REQUIRED:
      return "A cross-encrypted password is necessary to change this user password";

    case ERROR_NO_INHERITANCE:
      return "Indicates an ACL contains no inheritable components";

    case ERROR_FILE_CORRUPT:
      return "The file or directory is corrupted and unreadable";

    case ERROR_DISK_CORRUPT:
      return "The disk structure is corrupted and unreadable";

    case ERROR_NO_USER_SESSION_KEY:
      return "There is no user session key for the specified logon session";

    case ERROR_LICENSE_QUOTA_EXCEEDED:
      return "The service being accessed is licensed for a particular number of connections";

    case ERROR_WRONG_TARGET_NAME:
      return "Logon Failure: The target account name is incorrect";

    case ERROR_MUTUAL_AUTH_FAILED:
      return "Mutual Authentication failed. The server's password is out of date at the domain controller";

    case ERROR_TIME_SKEW:
      return "There is a time and/or date difference between the client and server";

    case ERROR_CURRENT_DOMAIN_NOT_ALLOWED:
      return "This operation can not be performed on the current domain";

    case ERROR_INVALID_WINDOW_HANDLE:
      return "Invalid window handle";

    case ERROR_INVALID_MENU_HANDLE:
      return "Invalid menu handle";

    case ERROR_INVALID_CURSOR_HANDLE:
      return "Invalid cursor handle";

    case ERROR_INVALID_ACCEL_HANDLE:
      return "Invalid accelerator table handle";

    case ERROR_INVALID_HOOK_HANDLE:
      return "Invalid hook handle";

    case ERROR_INVALID_DWP_HANDLE:
      return "Invalid handle to a multiple-window position structure";

    case ERROR_TLW_WITH_WSCHILD:
      return "Cannot create a top-level child window";

    case ERROR_CANNOT_FIND_WND_CLASS:
      return "Cannot find window class";

    case ERROR_WINDOW_OF_OTHER_THREAD:
      return "Invalid window; it belongs to other thread";

    case ERROR_HOTKEY_ALREADY_REGISTERED:
      return "Hot key is already registered";

    case ERROR_CLASS_ALREADY_EXISTS:
      return "Class already exists";

    case ERROR_CLASS_DOES_NOT_EXIST:
      return "Class does not exist";

    case ERROR_CLASS_HAS_WINDOWS:
      return "Class still has open windows";

    case ERROR_INVALID_INDEX:
      return "Invalid index";

    case ERROR_INVALID_ICON_HANDLE:
      return "Invalid icon handle";

    case ERROR_PRIVATE_DIALOG_INDEX:
      return "Using private DIALOG window words";

    case ERROR_LISTBOX_ID_NOT_FOUND:
      return "The list box identifier was not found";

    case ERROR_NO_WILDCARD_CHARACTERS:
      return "No wildcards were found";

    case ERROR_CLIPBOARD_NOT_OPEN:
      return "Thread does not have a clipboard open";

    case ERROR_HOTKEY_NOT_REGISTERED:
      return "Hot key is not registered";

    case ERROR_WINDOW_NOT_DIALOG:
      return "The window is not a valid dialog window";

    case ERROR_CONTROL_ID_NOT_FOUND:
      return "Control ID not found";

    case ERROR_INVALID_COMBOBOX_MESSAGE:
      return "Invalid message for a combo box because it does not have an edit control";

    case ERROR_WINDOW_NOT_COMBOBOX:
      return "The window is not a combo box";

    case ERROR_INVALID_EDIT_HEIGHT:
      return "Height must be less than 256";

    case ERROR_DC_NOT_FOUND:
      return "Invalid device context (DC) handle";

    case ERROR_INVALID_HOOK_FILTER:
      return "Invalid hook procedure type";

    case ERROR_INVALID_FILTER_PROC:
      return "Invalid hook procedure";

    case ERROR_HOOK_NEEDS_HMOD:
      return "Cannot set nonlocal hook without a module handle";

    case ERROR_GLOBAL_ONLY_HOOK:
      return "This hook procedure can only be set globally";

    case ERROR_JOURNAL_HOOK_SET:
      return "The journal hook procedure is already installed";

    case ERROR_HOOK_NOT_INSTALLED:
      return "The hook procedure is not installed";

    case ERROR_INVALID_LB_MESSAGE:
      return "Invalid message for single-selection list box";

    case ERROR_SETCOUNT_ON_BAD_LB:
      return "LB_SETCOUNT sent to non-lazy list box";

    case ERROR_LB_WITHOUT_TABSTOPS:
      return "This list box does not support tab stops";

    case ERROR_DESTROY_OBJECT_OF_OTHER_THREAD:
      return "Cannot destroy object created by another thread";

    case ERROR_CHILD_WINDOW_MENU:
      return "Child windows cannot have menus";

    case ERROR_NO_SYSTEM_MENU:
      return "The window does not have a system menu";

    case ERROR_INVALID_MSGBOX_STYLE:
      return "Invalid message box style";

    case ERROR_INVALID_SPI_VALUE:
      return "Invalid system-wide (SPI_*) parameter";

    case ERROR_SCREEN_ALREADY_LOCKED:
      return "Screen already locked";

    case ERROR_HWNDS_HAVE_DIFF_PARENT:
      return "All handles to windows in a multiple-window position structure must have the same parent";

    case ERROR_NOT_CHILD_WINDOW:
      return "The window is not a child window";

    case ERROR_INVALID_GW_COMMAND:
      return "Invalid GW_* command";

    case ERROR_INVALID_THREAD_ID:
      return "Invalid thread identifier";

    case ERROR_NON_MDICHILD_WINDOW:
      return "Cannot process a message from a window that is not a multiple document interface (MDI) window";

    case ERROR_POPUP_ALREADY_ACTIVE:
      return "Popup menu already active";

    case ERROR_NO_SCROLLBARS:
      return "The window does not have scroll bars";

    case ERROR_INVALID_SCROLLBAR_RANGE:
      return "Scroll bar range cannot be greater than MAXLONG";

    case ERROR_INVALID_SHOWWIN_COMMAND:
      return "Cannot show or remove the window in the way specified";

    case ERROR_NO_SYSTEM_RESOURCES:
      return "Insufficient system resources exist to complete the requested service";

    case ERROR_NONPAGED_SYSTEM_RESOURCES:
      return "Insufficient system resources exist to complete the requested service";

    case ERROR_PAGED_SYSTEM_RESOURCES:
      return "Insufficient system resources exist to complete the requested service";

    case ERROR_WORKING_SET_QUOTA:
      return "Insufficient quota to complete the requested service";

    case ERROR_PAGEFILE_QUOTA:
      return "Insufficient quota to complete the requested service";

    case ERROR_COMMITMENT_LIMIT:
      return "The paging file is too small for this operation to complete";

    case ERROR_MENU_ITEM_NOT_FOUND:
      return "A menu item was not found";

    case ERROR_INVALID_KEYBOARD_HANDLE:
      return "Invalid keyboard layout handle";

    case ERROR_HOOK_TYPE_NOT_ALLOWED:
      return "Hook type not allowed";

    case ERROR_REQUIRES_INTERACTIVE_WINDOWSTATION:
      return "This operation requires an interactive window station";

    case ERROR_TIMEOUT:
      return "This operation returned because the timeout period expired";

    case ERROR_INVALID_MONITOR_HANDLE:
      return "Invalid monitor handle";

    case ERROR_INCORRECT_SIZE:
      return "Incorrect size argument";

    case ERROR_EVENTLOG_FILE_CORRUPT:
      return "The event log file is corrupted";

    case ERROR_EVENTLOG_CANT_START:
      return "No event log file could be opened, so the event logging service did not start";

    case ERROR_LOG_FILE_FULL:
      return "The event log file is full";

    case ERROR_EVENTLOG_FILE_CHANGED:
      return "The event log file has changed between read operations";

    case ERROR_INSTALL_SERVICE_FAILURE:
      return "The Windows Installer Service could not be accessed. This can occur if you are running Windows in safe mode, or if the Windows Installer is not correctly installed. Contact your support personnel for assistance";

    case ERROR_INSTALL_USEREXIT:
      return "User cancelled installation";

    case ERROR_INSTALL_FAILURE:
      return "Fatal error during installation";

    case ERROR_INSTALL_SUSPEND:
      return "Installation suspended, incomplete";

    case ERROR_UNKNOWN_PRODUCT:
      return "This action is only valid for products that are currently installed";

    case ERROR_UNKNOWN_FEATURE:
      return "Feature ID not registered";

    case ERROR_UNKNOWN_COMPONENT:
      return "Component ID not registered";

    case ERROR_UNKNOWN_PROPERTY:
      return "Unknown property";

    case ERROR_INVALID_HANDLE_STATE:
      return "Handle is in an invalid state";

    case ERROR_BAD_CONFIGURATION:
      return "The configuration data for this product is corrupt.  Contact your support personnel";

    case ERROR_INDEX_ABSENT:
      return "Component qualifier not present";

    case ERROR_INSTALL_SOURCE_ABSENT:
      return "The installation source for this product is not available.  Verify that the source exists and that you can access it";

    case ERROR_INSTALL_PACKAGE_VERSION:
      return "This installation package cannot be installed by the Windows Installer service.  You must install a Windows service pack that contains a newer version of the Windows Installer service";

    case ERROR_PRODUCT_UNINSTALLED:
      return "Product is uninstalled";

    case ERROR_BAD_QUERY_SYNTAX:
      return "SQL query syntax invalid or unsupported";

    case ERROR_INVALID_FIELD:
      return "Record field does not exist";

    case ERROR_DEVICE_REMOVED:
      return "The device has been removed";

    case ERROR_INSTALL_ALREADY_RUNNING:
      return "Another installation is already in progress.  Complete that installation before proceeding with this install";

    case ERROR_INSTALL_PACKAGE_OPEN_FAILED:
      return "This installation package could not be opened.  Verify that the package exists and that you can access it, or contact the application vendor to verify that this is a valid Windows Installer package";

    case ERROR_INSTALL_PACKAGE_INVALID:
      return "This installation package could not be opened.  Contact the application vendor to verify that this is a valid Windows Installer package";

    case ERROR_INSTALL_UI_FAILURE:
      return "There was an error starting the Windows Installer service user interface.  Contact your support personnel";

    case ERROR_INSTALL_LOG_FAILURE:
      return "Error opening installation log file. Verify that the specified log file location exists and that you can write to it";

    case ERROR_INSTALL_LANGUAGE_UNSUPPORTED:
      return "The language of this installation package is not supported by your system";

    case ERROR_INSTALL_TRANSFORM_FAILURE:
      return "Error applying transforms.  Verify that the specified transform paths are valid";

    case ERROR_INSTALL_PACKAGE_REJECTED:
      return "This installation is forbidden by system policy.  Contact your system administrator";

    case ERROR_FUNCTION_NOT_CALLED:
      return "Function could not be executed";

    case ERROR_FUNCTION_FAILED:
      return "Function failed during execution";

    case ERROR_INVALID_TABLE:
      return "Invalid or unknown table specified";

    case ERROR_DATATYPE_MISMATCH:
      return "Data supplied is of wrong type";

    case ERROR_UNSUPPORTED_TYPE:
      return "Data of this type is not supported";

    case ERROR_CREATE_FAILED:
      return "The Windows Installer service failed to start.  Contact your support personnel";

    case ERROR_INSTALL_TEMP_UNWRITABLE:
      return "The Temp folder is on a drive that is full or is inaccessible. Free up space on the drive or verify that you have write permission on the Temp folder";

    case ERROR_INSTALL_PLATFORM_UNSUPPORTED:
      return "This installation package is not supported by this processor type. Contact your product vendor";

    case ERROR_INSTALL_NOTUSED:
      return "Component not used on this computer";

    case ERROR_PATCH_PACKAGE_OPEN_FAILED:
      return "This patch package could not be opened.  Verify that the patch package exists and that you can access it, or contact the application vendor to verify that this is a valid Windows Installer patch package";

    case ERROR_PATCH_PACKAGE_INVALID:
      return "This patch package could not be opened.  Contact the application vendor to verify that this is a valid Windows Installer patch package";

    case ERROR_PATCH_PACKAGE_UNSUPPORTED:
      return "This patch package cannot be processed by the Windows Installer service.  You must install a Windows service pack that contains a newer version of the Windows Installer service";

    case ERROR_PRODUCT_VERSION:
      return "Another version of this product is already installed.  Installation of this version cannot continue.  To configure or remove the existing version of this product, use Add/Remove Programs on the Control Panel";

    case ERROR_INVALID_COMMAND_LINE:
      return "Invalid command line argument.  Consult the Windows Installer SDK for detailed command line help";

    case ERROR_INSTALL_REMOTE_DISALLOWED:
      return "Only administrators have permission to add, remove, or configure server software during a Terminal services remote session. If you want to install or configure software on the server, contact your network administrator";

    case ERROR_SUCCESS_REBOOT_INITIATED:
      return "The requested operation completed successfully.  The system will be restarted so the changes can take effect";

    case ERROR_PATCH_TARGET_NOT_FOUND:
      return "The upgrade patch cannot be installed by the Windows Installer service because the program to be upgraded may be missing, or the upgrade patch may update a different version of the program. Verify that the program to be upgraded exists on your computer an";

    case ERROR_PATCH_PACKAGE_REJECTED:
      return "The patch package is not permitted by software restriction policy";

    case ERROR_INSTALL_TRANSFORM_REJECTED:
      return "One or more customizations are not permitted by software restriction policy";

    case ERROR_INSTALL_REMOTE_PROHIBITED:
      return "The Windows Installer does not permit installation from a Remote Desktop Connection";

    case RPC_S_INVALID_STRING_BINDING:
      return "The string binding is invalid";

    case RPC_S_WRONG_KIND_OF_BINDING:
      return "The binding handle is not the correct type";

    case RPC_S_INVALID_BINDING:
      return "The binding handle is invalid";

    case RPC_S_PROTSEQ_NOT_SUPPORTED:
      return "The RPC protocol sequence is not supported";

    case RPC_S_INVALID_RPC_PROTSEQ:
      return "The RPC protocol sequence is invalid";

    case RPC_S_INVALID_STRING_UUID:
      return "The string universal unique identifier (UUID) is invalid";

    case RPC_S_INVALID_ENDPOINT_FORMAT:
      return "The endpoint format is invalid";

    case RPC_S_INVALID_NET_ADDR:
      return "The network address is invalid";

    case RPC_S_NO_ENDPOINT_FOUND:
      return "No endpoint was found";

    case RPC_S_INVALID_TIMEOUT:
      return "The timeout value is invalid";

    case RPC_S_OBJECT_NOT_FOUND:
      return "The object universal unique identifier (UUID) was not found";

    case RPC_S_ALREADY_REGISTERED:
      return "The object universal unique identifier (UUID) has already been registered";

    case RPC_S_TYPE_ALREADY_REGISTERED:
      return "The type universal unique identifier (UUID) has already been registered";

    case RPC_S_ALREADY_LISTENING:
      return "The RPC server is already listening";

    case RPC_S_NO_PROTSEQS_REGISTERED:
      return "No protocol sequences have been registered";

    case RPC_S_NOT_LISTENING:
      return "The RPC server is not listening";

    case RPC_S_UNKNOWN_MGR_TYPE:
      return "The manager type is unknown";

    case RPC_S_UNKNOWN_IF:
      return "The interface is unknown";

    case RPC_S_NO_BINDINGS:
      return "There are no bindings";

    case RPC_S_NO_PROTSEQS:
      return "There are no protocol sequences";

    case RPC_S_CANT_CREATE_ENDPOINT:
      return "The endpoint cannot be created";

    case RPC_S_OUT_OF_RESOURCES:
      return "Not enough resources are available to complete this operation";

    case RPC_S_SERVER_UNAVAILABLE:
      return "The RPC server is unavailable";

    case RPC_S_SERVER_TOO_BUSY:
      return "The RPC server is too busy to complete this operation";

    case RPC_S_INVALID_NETWORK_OPTIONS:
      return "The network options are invalid";

    case RPC_S_NO_CALL_ACTIVE:
      return "There are no remote procedure calls active on this thread";

    case RPC_S_CALL_FAILED:
      return "The remote procedure call failed";

    case RPC_S_CALL_FAILED_DNE:
      return "The remote procedure call failed and did not execute";

    case RPC_S_PROTOCOL_ERROR:
      return "A remote procedure call (RPC) protocol error occurred";

    case RPC_S_UNSUPPORTED_TRANS_SYN:
      return "The transfer syntax is not supported by the RPC server";

    case RPC_S_UNSUPPORTED_TYPE:
      return "The universal unique identifier (UUID) type is not supported";

    case RPC_S_INVALID_TAG:
      return "The tag is invalid";

    case RPC_S_INVALID_BOUND:
      return "The array bounds are invalid";

    case RPC_S_NO_ENTRY_NAME:
      return "The binding does not contain an entry name";

    case RPC_S_INVALID_NAME_SYNTAX:
      return "The name syntax is invalid";

    case RPC_S_UNSUPPORTED_NAME_SYNTAX:
      return "The name syntax is not supported";

    case RPC_S_UUID_NO_ADDRESS:
      return "No network address is available to use to construct a universal unique identifier (UUID)";

    case RPC_S_DUPLICATE_ENDPOINT:
      return "The endpoint is a duplicate";

    case RPC_S_UNKNOWN_AUTHN_TYPE:
      return "The authentication type is unknown";

    case RPC_S_MAX_CALLS_TOO_SMALL:
      return "The maximum number of calls is too small";

    case RPC_S_STRING_TOO_LONG:
      return "The string is too long";

    case RPC_S_PROTSEQ_NOT_FOUND:
      return "The RPC protocol sequence was not found";

    case RPC_S_PROCNUM_OUT_OF_RANGE:
      return "The procedure number is out of range";

    case RPC_S_BINDING_HAS_NO_AUTH:
      return "The binding does not contain any authentication information";

    case RPC_S_UNKNOWN_AUTHN_SERVICE:
      return "The authentication service is unknown";

    case RPC_S_UNKNOWN_AUTHN_LEVEL:
      return "The authentication level is unknown";

    case RPC_S_INVALID_AUTH_IDENTITY:
      return "The security context is invalid";

    case RPC_S_UNKNOWN_AUTHZ_SERVICE:
      return "The authorization service is unknown";

    case EPT_S_INVALID_ENTRY:
      return "The entry is invalid";

    case EPT_S_CANT_PERFORM_OP:
      return "The server endpoint cannot perform the operation";

    case EPT_S_NOT_REGISTERED:
      return "There are no more endpoints available from the endpoint mapper";

    case RPC_S_NOTHING_TO_EXPORT:
      return "No interfaces have been exported";

    case RPC_S_INCOMPLETE_NAME:
      return "The entry name is incomplete";

    case RPC_S_INVALID_VERS_OPTION:
      return "The version option is invalid";

    case RPC_S_NO_MORE_MEMBERS:
      return "There are no more members";

    case RPC_S_NOT_ALL_OBJS_UNEXPORTED:
      return "There is nothing to unexport";

    case RPC_S_INTERFACE_NOT_FOUND:
      return "The interface was not found";

    case RPC_S_ENTRY_ALREADY_EXISTS:
      return "The entry already exists";

    case RPC_S_ENTRY_NOT_FOUND:
      return "The entry is not found";

    case RPC_S_NAME_SERVICE_UNAVAILABLE:
      return "The name service is unavailable";

    case RPC_S_INVALID_NAF_ID:
      return "The network address family is invalid";

    case RPC_S_CANNOT_SUPPORT:
      return "The requested operation is not supported";

    case RPC_S_NO_CONTEXT_AVAILABLE:
      return "No security context is available to allow impersonation";

    case RPC_S_INTERNAL_ERROR:
      return "An internal error occurred in a remote procedure call (RPC)";

    case RPC_S_ZERO_DIVIDE:
      return "The RPC server attempted an integer division by zero";

    case RPC_S_ADDRESS_ERROR:
      return "An addressing error occurred in the RPC server";

    case RPC_S_FP_DIV_ZERO:
      return "A floating-point operation at the RPC server caused a division by zero";

    case RPC_S_FP_UNDERFLOW:
      return "A floating-point underflow occurred at the RPC server";

    case RPC_S_FP_OVERFLOW:
      return "A floating-point overflow occurred at the RPC server";

    case RPC_X_NO_MORE_ENTRIES:
      return "The list of RPC servers available for the binding of auto handles has been exhausted";

    case RPC_X_SS_CHAR_TRANS_OPEN_FAIL:
      return "Unable to open the character translation table file";

    case RPC_X_SS_CHAR_TRANS_SHORT_FILE:
      return "The file containing the character translation table has fewer than 512 bytes";

    case RPC_X_SS_IN_NULL_CONTEXT:
      return "A null context handle was passed from the client to the host during a remote procedure call";

    case RPC_X_SS_CONTEXT_DAMAGED:
      return "The context handle changed during a remote procedure call";

    case RPC_X_SS_HANDLES_MISMATCH:
      return "The binding handles passed to a remote procedure call do not match";

    case RPC_X_SS_CANNOT_GET_CALL_HANDLE:
      return "The stub is unable to get the remote procedure call handle";

    case RPC_X_NULL_REF_POINTER:
      return "A null reference pointer was passed to the stub";

    case RPC_X_ENUM_VALUE_OUT_OF_RANGE:
      return "The enumeration value is out of range";

    case RPC_X_BYTE_COUNT_TOO_SMALL:
      return "The byte count is too small";

    case RPC_X_BAD_STUB_DATA:
      return "The stub received bad data";

    case ERROR_INVALID_USER_BUFFER:
      return "The supplied user buffer is not valid for the requested operation";

    case ERROR_UNRECOGNIZED_MEDIA:
      return "The disk media is not recognized. It may not be formatted";

    case ERROR_NO_TRUST_LSA_SECRET:
      return "The workstation does not have a trust secret";

    case ERROR_NO_TRUST_SAM_ACCOUNT:
      return "The security database on the server does not have a computer account for this workstation trust relationship";

    case ERROR_TRUSTED_DOMAIN_FAILURE:
      return "The trust relationship between the primary domain and the trusted domain failed";

    case ERROR_TRUSTED_RELATIONSHIP_FAILURE:
      return "The trust relationship between this workstation and the primary domain failed";

    case ERROR_TRUST_FAILURE:
      return "The network logon failed";

    case RPC_S_CALL_IN_PROGRESS:
      return "A remote procedure call is already in progress for this thread";

    case ERROR_NETLOGON_NOT_STARTED:
      return "An attempt was made to logon, but the network logon service was not started";

    case ERROR_ACCOUNT_EXPIRED:
      return "The user's account has expired";

    case ERROR_REDIRECTOR_HAS_OPEN_HANDLES:
      return "The redirector is in use and cannot be unloaded";

    case ERROR_PRINTER_DRIVER_ALREADY_INSTALLED:
      return "The specified printer driver is already installed";

    case ERROR_UNKNOWN_PORT:
      return "The specified port is unknown";

    case ERROR_UNKNOWN_PRINTER_DRIVER:
      return "The printer driver is unknown";

    case ERROR_UNKNOWN_PRINTPROCESSOR:
      return "The print processor is unknown";

    case ERROR_INVALID_SEPARATOR_FILE:
      return "The specified separator file is invalid";

    case ERROR_INVALID_PRIORITY:
      return "The specified priority is invalid";

    case ERROR_INVALID_PRINTER_NAME:
      return "The printer name is invalid";

    case ERROR_PRINTER_ALREADY_EXISTS:
      return "The printer already exists";

    case ERROR_INVALID_PRINTER_COMMAND:
      return "The printer command is invalid";

    case ERROR_INVALID_DATATYPE:
      return "The specified datatype is invalid";

    case ERROR_INVALID_ENVIRONMENT:
      return "The environment specified is invalid";

    case RPC_S_NO_MORE_BINDINGS:
      return "There are no more bindings";

    case ERROR_NOLOGON_INTERDOMAIN_TRUST_ACCOUNT:
      return "The account used is an interdomain trust account. Use your global user account or local user account to access this server";

    case ERROR_NOLOGON_WORKSTATION_TRUST_ACCOUNT:
      return "The account used is a computer account. Use your global user account or local user account to access this server";

    case ERROR_NOLOGON_SERVER_TRUST_ACCOUNT:
      return "The account used is a server trust account. Use your global user account or local user account to access this server";

    case ERROR_DOMAIN_TRUST_INCONSISTENT:
      return "The name or security ID (SID) of the domain specified is inconsistent with the trust information for that domain";

    case ERROR_SERVER_HAS_OPEN_HANDLES:
      return "The server is in use and cannot be unloaded";

    case ERROR_RESOURCE_DATA_NOT_FOUND:
      return "The specified image file did not contain a resource section";

    case ERROR_RESOURCE_TYPE_NOT_FOUND:
      return "The specified resource type cannot be found in the image file";

    case ERROR_RESOURCE_NAME_NOT_FOUND:
      return "The specified resource name cannot be found in the image file";

    case ERROR_RESOURCE_LANG_NOT_FOUND:
      return "The specified resource language ID cannot be found in the image file";

    case ERROR_NOT_ENOUGH_QUOTA:
      return "Not enough quota is available to process this command";

    case RPC_S_NO_INTERFACES:
      return "No interfaces have been registered";

    case RPC_S_CALL_CANCELLED:
      return "The remote procedure call was cancelled";

    case RPC_S_BINDING_INCOMPLETE:
      return "The binding handle does not contain all required information";

    case RPC_S_COMM_FAILURE:
      return "A communications failure occurred during a remote procedure call";

    case RPC_S_UNSUPPORTED_AUTHN_LEVEL:
      return "The requested authentication level is not supported";

    case RPC_S_NO_PRINC_NAME:
      return "No principal name registered";

    case RPC_S_NOT_RPC_ERROR:
      return "The error specified is not a valid Windows RPC error code";

    case RPC_S_UUID_LOCAL_ONLY:
      return "A UUID that is valid only on this computer has been allocated";

    case RPC_S_SEC_PKG_ERROR:
      return "A security package specific error occurred";

    case RPC_S_NOT_CANCELLED:
      return "Thread is not canceled";

    case RPC_X_INVALID_ES_ACTION:
      return "Invalid operation on the encoding/decoding handle";

    case RPC_X_WRONG_ES_VERSION:
      return "Incompatible version of the serializing package";

    case RPC_X_WRONG_STUB_VERSION:
      return "Incompatible version of the RPC stub";

    case RPC_X_INVALID_PIPE_OBJECT:
      return "The RPC pipe object is invalid or corrupted";

    case RPC_X_WRONG_PIPE_ORDER:
      return "An invalid operation was attempted on an RPC pipe object";

    case RPC_X_WRONG_PIPE_VERSION:
      return "Unsupported RPC pipe version";

    case RPC_S_GROUP_MEMBER_NOT_FOUND:
      return "The group member was not found";

    case EPT_S_CANT_CREATE:
      return "The endpoint mapper database entry could not be created";

    case RPC_S_INVALID_OBJECT:
      return "The object universal unique identifier (UUID) is the nil UUID";

    case ERROR_INVALID_TIME:
      return "The specified time is invalid";

    case ERROR_INVALID_FORM_NAME:
      return "The specified form name is invalid";

    case ERROR_INVALID_FORM_SIZE:
      return "The specified form size is invalid";

    case ERROR_ALREADY_WAITING:
      return "The specified printer handle is already being waited on";

    case ERROR_PRINTER_DELETED:
      return "The specified printer has been deleted";

    case ERROR_INVALID_PRINTER_STATE:
      return "The state of the printer is invalid";

    case ERROR_PASSWORD_MUST_CHANGE:
      return "The user's password must be changed before logging on the first time";

    case ERROR_DOMAIN_CONTROLLER_NOT_FOUND:
      return "Could not find the domain controller for this domain";

    case ERROR_ACCOUNT_LOCKED_OUT:
      return "The referenced account is currently locked out and may not be logged on to";

    case OR_INVALID_OXID:
      return "The object exporter specified was not found";

    case OR_INVALID_OID:
      return "The object specified was not found";

    case OR_INVALID_SET:
      return "The object resolver set specified was not found";

    case RPC_S_SEND_INCOMPLETE:
      return "Some data remains to be sent in the request buffer";

    case RPC_S_INVALID_ASYNC_HANDLE:
      return "Invalid asynchronous remote procedure call handle";

    case RPC_S_INVALID_ASYNC_CALL:
      return "Invalid asynchronous RPC call handle for this operation";

    case RPC_X_PIPE_CLOSED:
      return "The RPC pipe object has already been closed";

    case RPC_X_PIPE_DISCIPLINE_ERROR:
      return "The RPC call completed before all pipes were processed";

    case RPC_X_PIPE_EMPTY:
      return "No more data is available from the RPC pipe";

    case ERROR_NO_SITENAME:
      return "No site name is available for this machine";

    case ERROR_CANT_ACCESS_FILE:
      return "The file can not be accessed by the system";

    case ERROR_CANT_RESOLVE_FILENAME:
      return "The name of the file cannot be resolved by the system";

    case RPC_S_ENTRY_TYPE_MISMATCH:
      return "The entry is not of the expected type";

    case RPC_S_NOT_ALL_OBJS_EXPORTED:
      return "Not all object UUIDs could be exported to the specified entry";

    case RPC_S_INTERFACE_NOT_EXPORTED:
      return "Interface could not be exported to the specified entry";

    case RPC_S_PROFILE_NOT_ADDED:
      return "The specified profile entry could not be added";

    case RPC_S_PRF_ELT_NOT_ADDED:
      return "The specified profile element could not be added";

    case RPC_S_PRF_ELT_NOT_REMOVED:
      return "The specified profile element could not be removed";

    case RPC_S_GRP_ELT_NOT_ADDED:
      return "The group element could not be added";

    case RPC_S_GRP_ELT_NOT_REMOVED:
      return "The group element could not be removed";

    case ERROR_KM_DRIVER_BLOCKED:
      return "The printer driver is not compatible with a policy enabled on your computer that blocks NT 4.0 drivers";

    case ERROR_CONTEXT_EXPIRED:
      return "The context has expired and can no longer be used";

    case ERROR_PER_USER_TRUST_QUOTA_EXCEEDED:
      return "The current user's delegated trust creation quota has been exceeded";

    case ERROR_ALL_USER_TRUST_QUOTA_EXCEEDED:
      return "The total delegated trust creation quota has been exceeded";

    case ERROR_USER_DELETE_TRUST_QUOTA_EXCEEDED:
      return "The current user's delegated trust deletion quota has been exceeded";

    case ERROR_AUTHENTICATION_FIREWALL_FAILED:
      return "Logon Failure: The machine you are logging onto is protected by an authentication firewall.  The specified account is not allowed to authenticate to the machine";

    case ERROR_REMOTE_PRINT_CONNECTIONS_BLOCKED:
      return "Remote connections to the Print Spooler are blocked by a policy set on your machine";

    case ERROR_INVALID_PIXEL_FORMAT:
      return "The pixel format is invalid";

    case ERROR_BAD_DRIVER:
      return "The specified driver is invalid";

    case ERROR_INVALID_WINDOW_STYLE:
      return "The window style or class attribute is invalid for this operation";

    case ERROR_METAFILE_NOT_SUPPORTED:
      return "The requested metafile operation is not supported";

    case ERROR_TRANSFORM_NOT_SUPPORTED:
      return "The requested transformation operation is not supported";

    case ERROR_CLIPPING_NOT_SUPPORTED:
      return "The requested clipping operation is not supported";

    case ERROR_INVALID_CMM:
      return "The specified color management module is invalid";

    case ERROR_INVALID_PROFILE:
      return "The specified color profile is invalid";

    case ERROR_TAG_NOT_FOUND:
      return "The specified tag was not found";

    case ERROR_TAG_NOT_PRESENT:
      return "A required tag is not present";

    case ERROR_DUPLICATE_TAG:
      return "The specified tag is already present";

    case ERROR_PROFILE_NOT_ASSOCIATED_WITH_DEVICE:
      return "The specified color profile is not associated with any device";

    case ERROR_PROFILE_NOT_FOUND:
      return "The specified color profile was not found";

    case ERROR_INVALID_COLORSPACE:
      return "The specified color space is invalid";

    case ERROR_ICM_NOT_ENABLED:
      return "Image Color Management is not enabled";

    case ERROR_DELETING_ICM_XFORM:
      return "There was an error while deleting the color transform";

    case ERROR_INVALID_TRANSFORM:
      return "The specified color transform is invalid";

    case ERROR_COLORSPACE_MISMATCH:
      return "The specified transform does not match the bitmap's color space";

    case ERROR_INVALID_COLORINDEX:
      return "The specified named color index is not present in the profile";

    case ERROR_CONNECTED_OTHER_PASSWORD:
      return "The network connection was made successfully, but the user had to be prompted for a password other than the one originally specified";

    case ERROR_CONNECTED_OTHER_PASSWORD_DEFAULT:
      return "The network connection was made successfully using default credentials";

    case ERROR_BAD_USERNAME:
      return "The specified username is invalid";

    case ERROR_NOT_CONNECTED:
      return "This network connection does not exist";

    case ERROR_OPEN_FILES:
      return "This network connection has files open or requests pending";

    case ERROR_ACTIVE_CONNECTIONS:
      return "Active connections still exist";

    case ERROR_DEVICE_IN_USE:
      return "The device is in use by an active process and cannot be disconnected";

    case ERROR_UNKNOWN_PRINT_MONITOR:
      return "The specified print monitor is unknown";

    case ERROR_PRINTER_DRIVER_IN_USE:
      return "The specified printer driver is currently in use";

    case ERROR_SPOOL_FILE_NOT_FOUND:
      return "The spool file was not found";

    case ERROR_SPL_NO_STARTDOC:
      return "A StartDocPrinter call was not issued";

    case ERROR_SPL_NO_ADDJOB:
      return "An AddJob call was not issued";

    case ERROR_PRINT_PROCESSOR_ALREADY_INSTALLED:
      return "The specified print processor has already been installed";

    case ERROR_PRINT_MONITOR_ALREADY_INSTALLED:
      return "The specified print monitor has already been installed";

    case ERROR_INVALID_PRINT_MONITOR:
      return "The specified print monitor does not have the required functions";

    case ERROR_PRINT_MONITOR_IN_USE:
      return "The specified print monitor is currently in use";

    case ERROR_PRINTER_HAS_JOBS_QUEUED:
      return "The requested operation is not allowed when there are jobs queued to the printer";

    case ERROR_SUCCESS_REBOOT_REQUIRED:
      return "The requested operation is successful. Changes will not be effective until the system is rebooted";

    case ERROR_SUCCESS_RESTART_REQUIRED:
      return "The requested operation is successful. Changes will not be effective until the service is restarted";

    case ERROR_PRINTER_NOT_FOUND:
      return "No printers were found";

    case ERROR_PRINTER_DRIVER_WARNED:
      return "The printer driver is known to be unreliable";

    case ERROR_PRINTER_DRIVER_BLOCKED:
      return "The printer driver is known to harm the system";

    case ERROR_WINS_INTERNAL:
      return "WINS encountered an error while processing the command";

    case ERROR_CAN_NOT_DEL_LOCAL_WINS:
      return "The local WINS can not be deleted";

    case ERROR_STATIC_INIT:
      return "The importation from the file failed";

    case ERROR_INC_BACKUP:
      return "The backup failed. Was a full backup done before?";

    case ERROR_FULL_BACKUP:
      return "The backup failed. Check the directory to which you are backing the database";

    case ERROR_REC_NON_EXISTENT:
      return "The name does not exist in the WINS database";

    case ERROR_RPL_NOT_ALLOWED:
      return "Replication with a nonconfigured partner is not allowed";

    case ERROR_DHCP_ADDRESS_CONFLICT:
      return "The DHCP client has obtained an IP address that is already in use on the network. The local interface will be disabled until the DHCP client can obtain a new address";

    case ERROR_WMI_GUID_NOT_FOUND:
      return "The GUID passed was not recognized as valid by a WMI data provider";

    case ERROR_WMI_INSTANCE_NOT_FOUND:
      return "The instance name passed was not recognized as valid by a WMI data provider";

    case ERROR_WMI_ITEMID_NOT_FOUND:
      return "The data item ID passed was not recognized as valid by a WMI data provider";

    case ERROR_WMI_TRY_AGAIN:
      return "The WMI request could not be completed and should be retried";

    case ERROR_WMI_DP_NOT_FOUND:
      return "The WMI data provider could not be located";

    case ERROR_WMI_UNRESOLVED_INSTANCE_REF:
      return "The WMI data provider references an instance set that has not been registered";

    case ERROR_WMI_ALREADY_ENABLED:
      return "The WMI data block or event notification has already been enabled";

    case ERROR_WMI_GUID_DISCONNECTED:
      return "The WMI data block is no longer available";

    case ERROR_WMI_SERVER_UNAVAILABLE:
      return "The WMI data service is not available";

    case ERROR_WMI_DP_FAILED:
      return "The WMI data provider failed to carry out the request";

    case ERROR_WMI_INVALID_MOF:
      return "The WMI MOF information is not valid";

    case ERROR_WMI_INVALID_REGINFO:
      return "The WMI registration information is not valid";

    case ERROR_WMI_ALREADY_DISABLED:
      return "The WMI data block or event notification has already been disabled";

    case ERROR_WMI_READ_ONLY:
      return "The WMI data item or data block is read only";

    case ERROR_WMI_SET_FAILURE:
      return "The WMI data item or data block could not be changed";

    case ERROR_INVALID_MEDIA:
      return "The media identifier does not represent a valid medium";

    case ERROR_INVALID_LIBRARY:
      return "The library identifier does not represent a valid library";

    case ERROR_INVALID_MEDIA_POOL:
      return "The media pool identifier does not represent a valid media pool";

    case ERROR_DRIVE_MEDIA_MISMATCH:
      return "The drive and medium are not compatible or exist in different libraries";

    case ERROR_MEDIA_OFFLINE:
      return "The medium currently exists in an offline library and must be online to perform this operation";

    case ERROR_LIBRARY_OFFLINE:
      return "The operation cannot be performed on an offline library";

    case ERROR_EMPTY:
      return "The library, drive, or media pool is empty";

    case ERROR_NOT_EMPTY:
      return "The library, drive, or media pool must be empty to perform this operation";

    case ERROR_MEDIA_UNAVAILABLE:
      return "No media is currently available in this media pool or library";

    case ERROR_RESOURCE_DISABLED:
      return "A resource required for this operation is disabled";

    case ERROR_INVALID_CLEANER:
      return "The media identifier does not represent a valid cleaner";

    case ERROR_UNABLE_TO_CLEAN:
      return "The drive cannot be cleaned or does not support cleaning";

    case ERROR_OBJECT_NOT_FOUND:
      return "The object identifier does not represent a valid object";

    case ERROR_DATABASE_FAILURE:
      return "Unable to read from or write to the database";

    case ERROR_DATABASE_FULL:
      return "The database is full";

    case ERROR_MEDIA_INCOMPATIBLE:
      return "The medium is not compatible with the device or media pool";

    case ERROR_RESOURCE_NOT_PRESENT:
      return "The resource required for this operation does not exist";

    case ERROR_INVALID_OPERATION:
      return "The operation identifier is not valid";

    case ERROR_MEDIA_NOT_AVAILABLE:
      return "The media is not mounted or ready for use";

    case ERROR_DEVICE_NOT_AVAILABLE:
      return "The device is not ready for use";

    case ERROR_REQUEST_REFUSED:
      return "The operator or administrator has refused the request";

    case ERROR_INVALID_DRIVE_OBJECT:
      return "The drive identifier does not represent a valid drive";

    case ERROR_LIBRARY_FULL:
      return "Library is full.  No slot is available for use";

    case ERROR_MEDIUM_NOT_ACCESSIBLE:
      return "The transport cannot access the medium";

    case ERROR_UNABLE_TO_LOAD_MEDIUM:
      return "Unable to load the medium into the drive";

    case ERROR_UNABLE_TO_INVENTORY_DRIVE:
      return "Unable to retrieve the drive status";

    case ERROR_UNABLE_TO_INVENTORY_SLOT:
      return "Unable to retrieve the slot status";

    case ERROR_UNABLE_TO_INVENTORY_TRANSPORT:
      return "Unable to retrieve status about the transport";

    case ERROR_TRANSPORT_FULL:
      return "Cannot use the transport because it is already in use";

    case ERROR_CONTROLLING_IEPORT:
      return "Unable to open or close the inject/eject port";

    case ERROR_UNABLE_TO_EJECT_MOUNTED_MEDIA:
      return "Unable to eject the medium because it is in a drive";

    case ERROR_CLEANER_SLOT_SET:
      return "A cleaner slot is already reserved";

    case ERROR_CLEANER_SLOT_NOT_SET:
      return "A cleaner slot is not reserved";

    case ERROR_CLEANER_CARTRIDGE_SPENT:
      return "The cleaner cartridge has performed the maximum number of drive cleanings";

    case ERROR_UNEXPECTED_OMID:
      return "Unexpected on-medium identifier";

    case ERROR_CANT_DELETE_LAST_ITEM:
      return "The last remaining item in this group or resource cannot be deleted";

    case ERROR_MESSAGE_EXCEEDS_MAX_SIZE:
      return "The message provided exceeds the maximum size allowed for this parameter";

    case ERROR_VOLUME_CONTAINS_SYS_FILES:
      return "The volume contains system or paging files";

    case ERROR_INDIGENOUS_TYPE:
      return "The media type cannot be removed from this library since at least one drive in the library reports it can support this media type";

    case ERROR_NO_SUPPORTING_DRIVES:
      return "This offline media cannot be mounted on this system since no enabled drives are present which can be used";

    case ERROR_CLEANER_CARTRIDGE_INSTALLED:
      return "A cleaner cartridge is present in the tape library";

    case ERROR_IEPORT_FULL:
      return "Cannot use the ieport because it is not empty";

    case ERROR_FILE_OFFLINE:
      return "The remote storage service was not able to recall the file";

    case ERROR_REMOTE_STORAGE_NOT_ACTIVE:
      return "The remote storage service is not operational at this time";

    case ERROR_REMOTE_STORAGE_MEDIA_ERROR:
      return "The remote storage service encountered a media error";

    case ERROR_NOT_A_REPARSE_POINT:
      return "The file or directory is not a reparse point";

    case ERROR_REPARSE_ATTRIBUTE_CONFLICT:
      return "The reparse point attribute cannot be set because it conflicts with an existing attribute";

    case ERROR_INVALID_REPARSE_DATA:
      return "The data present in the reparse point buffer is invalid";

    case ERROR_REPARSE_TAG_INVALID:
      return "The tag present in the reparse point buffer is invalid";

    case ERROR_REPARSE_TAG_MISMATCH:
      return "There is a mismatch between the tag specified in the request and the tag present in the reparse point";

    case ERROR_VOLUME_NOT_SIS_ENABLED:
      return "Single Instance Storage is not available on this volume";

    case ERROR_DEPENDENT_RESOURCE_EXISTS:
      return "The cluster resource cannot be moved to another group because other resources are dependent on it";

    case ERROR_DEPENDENCY_NOT_FOUND:
      return "The cluster resource dependency cannot be found";

    case ERROR_DEPENDENCY_ALREADY_EXISTS:
      return "The cluster resource cannot be made dependent on the specified resource because it is already dependent";

    case ERROR_RESOURCE_NOT_ONLINE:
      return "The cluster resource is not online";

    case ERROR_HOST_NODE_NOT_AVAILABLE:
      return "A cluster node is not available for this operation";

    case ERROR_RESOURCE_NOT_AVAILABLE:
      return "The cluster resource is not available";

    case ERROR_RESOURCE_NOT_FOUND:
      return "The cluster resource could not be found";

    case ERROR_SHUTDOWN_CLUSTER:
      return "The cluster is being shut down";

    case ERROR_CANT_EVICT_ACTIVE_NODE:
      return "A cluster node cannot be evicted from the cluster unless the node is down or it is the last node";

    case ERROR_OBJECT_ALREADY_EXISTS:
      return "The object already exists";

    case ERROR_OBJECT_IN_LIST:
      return "The object is already in the list";

    case ERROR_GROUP_NOT_AVAILABLE:
      return "The cluster group is not available for any new requests";

    case ERROR_GROUP_NOT_FOUND:
      return "The cluster group could not be found";

    case ERROR_GROUP_NOT_ONLINE:
      return "The operation could not be completed because the cluster group is not online";

    case ERROR_HOST_NODE_NOT_RESOURCE_OWNER:
      return "The cluster node is not the owner of the resource";

    case ERROR_HOST_NODE_NOT_GROUP_OWNER:
      return "The cluster node is not the owner of the group";

    case ERROR_RESMON_CREATE_FAILED:
      return "The cluster resource could not be created in the specified resource monitor";

    case ERROR_RESMON_ONLINE_FAILED:
      return "The cluster resource could not be brought online by the resource monitor";

    case ERROR_RESOURCE_ONLINE:
      return "The operation could not be completed because the cluster resource is online";

    case ERROR_QUORUM_RESOURCE:
      return "The cluster resource could not be deleted or brought offline because it is the quorum resource";

    case ERROR_NOT_QUORUM_CAPABLE:
      return "The cluster could not make the specified resource a quorum resource because it is not capable of being a quorum resource";

    case ERROR_CLUSTER_SHUTTING_DOWN:
      return "The cluster software is shutting down";

    case ERROR_INVALID_STATE:
      return "The group or resource is not in the correct state to perform the requested operation";

    case ERROR_RESOURCE_PROPERTIES_STORED:
      return "The properties were stored but not all changes will take effect until the next time the resource is brought online";

    case ERROR_NOT_QUORUM_CLASS:
      return "The cluster could not make the specified resource a quorum resource because it does not belong to a shared storage class";

    case ERROR_CORE_RESOURCE:
      return "The cluster resource could not be deleted since it is a core resource";

    case ERROR_QUORUM_RESOURCE_ONLINE_FAILED:
      return "The quorum resource failed to come online";

    case ERROR_QUORUMLOG_OPEN_FAILED:
      return "The quorum log could not be created or mounted successfully";

    case ERROR_CLUSTERLOG_CORRUPT:
      return "The cluster log is corrupt";

    case ERROR_CLUSTERLOG_RECORD_EXCEEDS_MAXSIZE:
      return "The record could not be written to the cluster log since it exceeds the maximum size";

    case ERROR_CLUSTERLOG_EXCEEDS_MAXSIZE:
      return "The cluster log exceeds its maximum size";

    case ERROR_CLUSTERLOG_CHKPOINT_NOT_FOUND:
      return "No checkpoint record was found in the cluster log";

    case ERROR_CLUSTERLOG_NOT_ENOUGH_SPACE:
      return "The minimum required disk space needed for logging is not available";

    case ERROR_QUORUM_OWNER_ALIVE:
      return "The cluster node failed to take control of the quorum resource because the resource is owned by another active node";

    case ERROR_NETWORK_NOT_AVAILABLE:
      return "A cluster network is not available for this operation";

    case ERROR_NODE_NOT_AVAILABLE:
      return "A cluster node is not available for this operation";

    case ERROR_ALL_NODES_NOT_AVAILABLE:
      return "All cluster nodes must be running to perform this operation";

    case ERROR_RESOURCE_FAILED:
      return "A cluster resource failed";

    case ERROR_CLUSTER_INVALID_NODE:
      return "The cluster node is not valid";

    case ERROR_CLUSTER_NODE_EXISTS:
      return "The cluster node already exists";

    case ERROR_CLUSTER_JOIN_IN_PROGRESS:
      return "A node is in the process of joining the cluster";

    case ERROR_CLUSTER_NODE_NOT_FOUND:
      return "The cluster node was not found";

    case ERROR_CLUSTER_LOCAL_NODE_NOT_FOUND:
      return "The cluster local node information was not found";

    case ERROR_CLUSTER_NETWORK_EXISTS:
      return "The cluster network already exists";

    case ERROR_CLUSTER_NETWORK_NOT_FOUND:
      return "The cluster network was not found";

    case ERROR_CLUSTER_NETINTERFACE_EXISTS:
      return "The cluster network interface already exists";

    case ERROR_CLUSTER_NETINTERFACE_NOT_FOUND:
      return "The cluster network interface was not found";

    case ERROR_CLUSTER_INVALID_REQUEST:
      return "The cluster request is not valid for this object";

    case ERROR_CLUSTER_INVALID_NETWORK_PROVIDER:
      return "The cluster network provider is not valid";

    case ERROR_CLUSTER_NODE_DOWN:
      return "The cluster node is down";

    case ERROR_CLUSTER_NODE_UNREACHABLE:
      return "The cluster node is not reachable";

    case ERROR_CLUSTER_NODE_NOT_MEMBER:
      return "The cluster node is not a member of the cluster";

    case ERROR_CLUSTER_JOIN_NOT_IN_PROGRESS:
      return "A cluster join operation is not in progress";

    case ERROR_CLUSTER_INVALID_NETWORK:
      return "The cluster network is not valid";

    case ERROR_CLUSTER_NODE_UP:
      return "The cluster node is up";

    case ERROR_CLUSTER_IPADDR_IN_USE:
      return "The cluster IP address is already in use";

    case ERROR_CLUSTER_NODE_NOT_PAUSED:
      return "The cluster node is not paused";

    case ERROR_CLUSTER_NO_SECURITY_CONTEXT:
      return "No cluster security context is available";

    case ERROR_CLUSTER_NETWORK_NOT_INTERNAL:
      return "The cluster network is not configured for internal cluster communication";

    case ERROR_CLUSTER_NODE_ALREADY_UP:
      return "The cluster node is already up";

    case ERROR_CLUSTER_NODE_ALREADY_DOWN:
      return "The cluster node is already down";

    case ERROR_CLUSTER_NETWORK_ALREADY_ONLINE:
      return "The cluster network is already online";

    case ERROR_CLUSTER_NETWORK_ALREADY_OFFLINE:
      return "The cluster network is already offline";

    case ERROR_CLUSTER_NODE_ALREADY_MEMBER:
      return "The cluster node is already a member of the cluster";

    case ERROR_CLUSTER_LAST_INTERNAL_NETWORK:
      return "The cluster network is the only one configured for internal cluster communication between two or more active cluster nodes. The internal communication capability cannot be removed from the network";

    case ERROR_CLUSTER_NETWORK_HAS_DEPENDENTS:
      return "One or more cluster resources depend on the network to provide service to clients. The client access capability cannot be removed from the network";

    case ERROR_INVALID_OPERATION_ON_QUORUM:
      return "This operation cannot be performed on the cluster resource as it the quorum resource. You may not bring the quorum resource offline or modify its possible owners list";

    case ERROR_DEPENDENCY_NOT_ALLOWED:
      return "The cluster quorum resource is not allowed to have any dependencies";

    case ERROR_CLUSTER_NODE_PAUSED:
      return "The cluster node is paused";

    case ERROR_NODE_CANT_HOST_RESOURCE:
      return "The cluster resource cannot be brought online. The owner node cannot run this resource";

    case ERROR_CLUSTER_NODE_NOT_READY:
      return "The cluster node is not ready to perform the requested operation";

    case ERROR_CLUSTER_NODE_SHUTTING_DOWN:
      return "The cluster node is shutting down";

    case ERROR_CLUSTER_JOIN_ABORTED:
      return "The cluster join operation was aborted";

    case ERROR_CLUSTER_INCOMPATIBLE_VERSIONS:
      return "The cluster join operation failed due to incompatible software versions between the joining node and its sponsor";

    case ERROR_CLUSTER_MAXNUM_OF_RESOURCES_EXCEEDED:
      return "This resource cannot be created because the cluster has reached the limit on the number of resources it can monitor";

    case ERROR_CLUSTER_SYSTEM_CONFIG_CHANGED:
      return "The system configuration changed during the cluster join or form operation. The join or form operation was aborted";

    case ERROR_CLUSTER_RESOURCE_TYPE_NOT_FOUND:
      return "The specified resource type was not found";

    case ERROR_CLUSTER_RESTYPE_NOT_SUPPORTED:
      return "The specified node does not support a resource of this type.  This may be due to version inconsistencies or due to the absence of the resource DLL on this node";

    case ERROR_CLUSTER_RESNAME_NOT_FOUND:
      return "The specified resource name is not supported by this resource DLL. This may be due to a bad (or changed) name supplied to the resource DLL";

    case ERROR_CLUSTER_NO_RPC_PACKAGES_REGISTERED:
      return "No authentication package could be registered with the RPC server";

    case ERROR_CLUSTER_OWNER_NOT_IN_PREFLIST:
      return "You cannot bring the group online because the owner of the group is not in the preferred list for the group. To change the owner node for the group, move the group";

    case ERROR_CLUSTER_DATABASE_SEQMISMATCH:
      return "The join operation failed because the cluster database sequence number has changed or is incompatible with the locker node. This may happen during a join operation if the cluster database was changing during the join";

    case ERROR_RESMON_INVALID_STATE:
      return "The resource monitor will not allow the fail operation to be performed while the resource is in its current state. This may happen if the resource is in a pending state";

    case ERROR_CLUSTER_GUM_NOT_LOCKER:
      return "A non locker code got a request to reserve the lock for making global updates";

    case ERROR_QUORUM_DISK_NOT_FOUND:
      return "The quorum disk could not be located by the cluster service";

    case ERROR_DATABASE_BACKUP_CORRUPT:
      return "The backed up cluster database is possibly corrupt";

    case ERROR_CLUSTER_NODE_ALREADY_HAS_DFS_ROOT:
      return "A DFS root already exists in this cluster node";

    case ERROR_RESOURCE_PROPERTY_UNCHANGEABLE:
      return "An attempt to modify a resource property failed because it conflicts with another existing property";

    case ERROR_CLUSTER_MEMBERSHIP_INVALID_STATE:
      return "An operation was attempted that is incompatible with the current membership state of the node";

    case ERROR_CLUSTER_QUORUMLOG_NOT_FOUND:
      return "The quorum resource does not contain the quorum log";

    case ERROR_CLUSTER_MEMBERSHIP_HALT:
      return "The membership engine requested shutdown of the cluster service on this node";

    case ERROR_CLUSTER_INSTANCE_ID_MISMATCH:
      return "The join operation failed because the cluster instance ID of the joining node does not match the cluster instance ID of the sponsor node";

    case ERROR_CLUSTER_NETWORK_NOT_FOUND_FOR_IP:
      return "A matching network for the specified IP address could not be found. Please also specify a subnet mask and a cluster network";

    case ERROR_CLUSTER_PROPERTY_DATA_TYPE_MISMATCH:
      return "The actual data type of the property did not match the expected data type of the property";

    case ERROR_CLUSTER_EVICT_WITHOUT_CLEANUP:
      return "The cluster node was evicted from the cluster successfully, but the node was not cleaned up.  Extended status information explaining why the node was not cleaned up is available";

    case ERROR_CLUSTER_PARAMETER_MISMATCH:
      return "Two or more parameter values specified for a resource's properties are in conflict";

    case ERROR_NODE_CANNOT_BE_CLUSTERED:
      return "This computer cannot be made a member of a cluster";

    case ERROR_CLUSTER_WRONG_OS_VERSION:
      return "This computer cannot be made a member of a cluster because it does not have the correct version of Windows installed";

    case ERROR_CLUSTER_CANT_CREATE_DUP_CLUSTER_NAME:
      return "A cluster cannot be created with the specified cluster name because that cluster name is already in use. Specify a different name for the cluster";

    case ERROR_CLUSCFG_ALREADY_COMMITTED:
      return "The cluster configuration action has already been committed";

    case ERROR_CLUSCFG_ROLLBACK_FAILED:
      return "The cluster configuration action could not be rolled back";

    case ERROR_CLUSCFG_SYSTEM_DISK_DRIVE_LETTER_CONFLICT:
      return "The drive letter assigned to a system disk on one node conflicted with the drive letter assigned to a disk on another node";

    case ERROR_CLUSTER_OLD_VERSION:
      return "One or more nodes in the cluster are running a version of Windows that does not support this operation";

    case ERROR_CLUSTER_MISMATCHED_COMPUTER_ACCT_NAME:
      return "The name of the corresponding computer account doesn't match the Network Name for this resource";

    case ERROR_ENCRYPTION_FAILED:
      return "The specified file could not be encrypted";

    case ERROR_DECRYPTION_FAILED:
      return "The specified file could not be decrypted";

    case ERROR_FILE_ENCRYPTED:
      return "The specified file is encrypted and the user does not have the ability to decrypt it";

    case ERROR_NO_RECOVERY_POLICY:
      return "There is no valid encryption recovery policy configured for this system";

    case ERROR_NO_EFS:
      return "The required encryption driver is not loaded for this system";

    case ERROR_WRONG_EFS:
      return "The file was encrypted with a different encryption driver than is currently loaded";

    case ERROR_NO_USER_KEYS:
      return "There are no EFS keys defined for the user";

    case ERROR_FILE_NOT_ENCRYPTED:
      return "The specified file is not encrypted";

    case ERROR_NOT_EXPORT_FORMAT:
      return "The specified file is not in the defined EFS export format";

    case ERROR_FILE_READ_ONLY:
      return "The specified file is read only";

    case ERROR_DIR_EFS_DISALLOWED:
      return "The directory has been disabled for encryption";

    case ERROR_EFS_SERVER_NOT_TRUSTED:
      return "The server is not trusted for remote encryption operation";

    case ERROR_BAD_RECOVERY_POLICY:
      return "Recovery policy configured for this system contains invalid recovery certificate";

    case ERROR_EFS_ALG_BLOB_TOO_BIG:
      return "The encryption algorithm used on the source file needs a bigger key buffer than the one on the destination file";

    case ERROR_VOLUME_NOT_SUPPORT_EFS:
      return "The disk partition does not support file encryption";

    case ERROR_EFS_DISABLED:
      return "This machine is disabled for file encryption";

    case ERROR_EFS_VERSION_NOT_SUPPORT:
      return "A newer system is required to decrypt this encrypted file";

    case ERROR_NO_BROWSER_SERVERS_FOUND:
      return "The list of servers for this workgroup is not currently available";

    case SCHED_E_SERVICE_NOT_LOCALSYSTEM:
      return "The Task Scheduler service must be configured to run in the System account to function properly.  Individual tasks may be configured to run in other accounts";

    case ERROR_CTX_WINSTATION_NAME_INVALID:
      return "The specified session name is invalid";

    case ERROR_CTX_INVALID_PD:
      return "The specified protocol driver is invalid";

    case ERROR_CTX_PD_NOT_FOUND:
      return "The specified protocol driver was not found in the system path";

    case ERROR_CTX_WD_NOT_FOUND:
      return "The specified terminal connection driver was not found in the system path";

    case ERROR_CTX_CANNOT_MAKE_EVENTLOG_ENTRY:
      return "A registry key for event logging could not be created for this session";

    case ERROR_CTX_SERVICE_NAME_COLLISION:
      return "A service with the same name already exists on the system";

    case ERROR_CTX_CLOSE_PENDING:
      return "A close operation is pending on the session";

    case ERROR_CTX_NO_OUTBUF:
      return "There are no free output buffers available";

    case ERROR_CTX_MODEM_INF_NOT_FOUND:
      return "The MODEM.INF file was not found";

    case ERROR_CTX_INVALID_MODEMNAME:
      return "The modem name was not found in MODEM.INF";

    case ERROR_CTX_MODEM_RESPONSE_ERROR:
      return "The modem did not accept the command sent to it. Verify that the configured modem name matches the attached modem";

    case ERROR_CTX_MODEM_RESPONSE_TIMEOUT:
      return "The modem did not respond to the command sent to it. Verify that the modem is properly cabled and powered on";

    case ERROR_CTX_MODEM_RESPONSE_NO_CARRIER:
      return "Carrier detect has failed or carrier has been dropped due to disconnect";

    case ERROR_CTX_MODEM_RESPONSE_NO_DIALTONE:
      return "Dial tone not detected within the required time. Verify that the phone cable is properly attached and functional";

    case ERROR_CTX_MODEM_RESPONSE_BUSY:
      return "Busy signal detected at remote site on callback";

    case ERROR_CTX_MODEM_RESPONSE_VOICE:
      return "Voice detected at remote site on callback";

    case ERROR_CTX_TD_ERROR:
      return "Transport driver error";

    case ERROR_CTX_WINSTATION_NOT_FOUND:
      return "The specified session cannot be found";

    case ERROR_CTX_WINSTATION_ALREADY_EXISTS:
      return "The specified session name is already in use";

    case ERROR_CTX_WINSTATION_BUSY:
      return "The requested operation cannot be completed because the terminal connection is currently busy processing a connect, disconnect, reset, or delete operation";

    case ERROR_CTX_BAD_VIDEO_MODE:
      return "An attempt has been made to connect to a session whose video mode is not supported by the current client";

    case ERROR_CTX_GRAPHICS_INVALID:
      return "The application attempted to enable DOS graphics mode";

    case ERROR_CTX_LOGON_DISABLED:
      return "Your interactive logon privilege has been disabled";

    case ERROR_CTX_NOT_CONSOLE:
      return "The requested operation can be performed only on the system console";

    case ERROR_CTX_CLIENT_QUERY_TIMEOUT:
      return "The client failed to respond to the server connect message";

    case ERROR_CTX_CONSOLE_DISCONNECT:
      return "Disconnecting the console session is not supported";

    case ERROR_CTX_CONSOLE_CONNECT:
      return "Reconnecting a disconnected session to the console is not supported";

    case ERROR_CTX_SHADOW_DENIED:
      return "The request to control another session remotely was denied";

    case ERROR_CTX_WINSTATION_ACCESS_DENIED:
      return "The requested session access is denied";

    case ERROR_CTX_INVALID_WD:
      return "The specified terminal connection driver is invalid";

    case ERROR_CTX_SHADOW_INVALID:
      return "The requested session cannot be controlled remotely";

    case ERROR_CTX_SHADOW_DISABLED:
      return "The requested session is not configured to allow remote control";

    case ERROR_CTX_CLIENT_LICENSE_IN_USE:
      return "Your request to connect to this Terminal Server has been rejected. Your Terminal Server client license number is currently being used by another user";

    case ERROR_CTX_CLIENT_LICENSE_NOT_SET:
      return "Your request to connect to this Terminal Server has been rejected. Your Terminal Server client license number has not been entered for this copy of the Terminal Server client";

    case ERROR_CTX_LICENSE_NOT_AVAILABLE:
      return "The system has reached its licensed logon limit";

    case ERROR_CTX_LICENSE_CLIENT_INVALID:
      return "The client you are using is not licensed to use this system.  Your logon request is denied";

    case ERROR_CTX_LICENSE_EXPIRED:
      return "The system license has expired.  Your logon request is denied";

    case ERROR_CTX_SHADOW_NOT_RUNNING:
      return "Remote control could not be terminated because the specified session is not currently being remotely controlled";

    case ERROR_CTX_SHADOW_ENDED_BY_MODE_CHANGE:
      return "The remote control of the console was terminated because the display mode was changed. Changing the display mode in a remote control session is not supported";

    case ERROR_ACTIVATION_COUNT_EXCEEDED:
      return "Activation has already been reset the maximum number of times for this installation. Your activation timer will not be cleared";

    case FRS_ERR_INVALID_API_SEQUENCE:
      return "The file replication service API was called incorrectly";

    case FRS_ERR_STARTING_SERVICE:
      return "The file replication service cannot be started";

    case FRS_ERR_STOPPING_SERVICE:
      return "The file replication service cannot be stopped";

    case FRS_ERR_INTERNAL_API:
      return "The file replication service API terminated the request";

    case FRS_ERR_INTERNAL:
      return "The file replication service terminated the request";

    case FRS_ERR_SERVICE_COMM:
      return "The file replication service cannot be contacted";

    case FRS_ERR_INSUFFICIENT_PRIV:
      return "The file replication service cannot satisfy the request because the user has insufficient privileges";

    case FRS_ERR_AUTHENTICATION:
      return "The file replication service cannot satisfy the request because authenticated RPC is not available";

    case FRS_ERR_PARENT_INSUFFICIENT_PRIV:
      return "The file replication service cannot satisfy the request because the user has insufficient privileges on the domain controller";

    case FRS_ERR_PARENT_AUTHENTICATION:
      return "The file replication service cannot satisfy the request because authenticated RPC is not available on the domain controller";

    case FRS_ERR_CHILD_TO_PARENT_COMM:
      return "The file replication service cannot communicate with the file replication service on the domain controller";

    case FRS_ERR_PARENT_TO_CHILD_COMM:
      return "The file replication service on the domain controller cannot communicate with the file replication service on this computer";

    case FRS_ERR_SYSVOL_POPULATE:
      return "The file replication service cannot populate the system volume because of an internal error";

    case FRS_ERR_SYSVOL_POPULATE_TIMEOUT:
      return "The file replication service cannot populate the system volume because of an internal timeout";

    case FRS_ERR_SYSVOL_IS_BUSY:
      return "The file replication service cannot process the request. The system volume is busy with a previous request";

    case FRS_ERR_SYSVOL_DEMOTE:
      return "The file replication service cannot stop replicating the system volume because of an internal error";

    case FRS_ERR_INVALID_SERVICE_PARAMETER:
      return "The file replication service detected an invalid parameter";

    case ERROR_DS_NOT_INSTALLED:
      return "An error occurred while installing the directory service. For more information, see the event log";

    case ERROR_DS_MEMBERSHIP_EVALUATED_LOCALLY:
      return "The directory service evaluated group memberships locally";

    case ERROR_DS_NO_ATTRIBUTE_OR_VALUE:
      return "The specified directory service attribute or value does not exist";

    case ERROR_DS_INVALID_ATTRIBUTE_SYNTAX:
      return "The attribute syntax specified to the directory service is invalid";

    case ERROR_DS_ATTRIBUTE_TYPE_UNDEFINED:
      return "The attribute type specified to the directory service is not defined";

    case ERROR_DS_ATTRIBUTE_OR_VALUE_EXISTS:
      return "The specified directory service attribute or value already exists";

    case ERROR_DS_BUSY:
      return "The directory service is busy";

    case ERROR_DS_UNAVAILABLE:
      return "The directory service is unavailable";

    case ERROR_DS_NO_RIDS_ALLOCATED:
      return "The directory service was unable to allocate a relative identifier";

    case ERROR_DS_NO_MORE_RIDS:
      return "The directory service has exhausted the pool of relative identifiers";

    case ERROR_DS_INCORRECT_ROLE_OWNER:
      return "The requested operation could not be performed because the directory service is not the master for that type of operation";

    case ERROR_DS_RIDMGR_INIT_ERROR:
      return "The directory service was unable to initialize the subsystem that allocates relative identifiers";

    case ERROR_DS_OBJ_CLASS_VIOLATION:
      return "The requested operation did not satisfy one or more constraints associated with the class of the object";

    case ERROR_DS_CANT_ON_NON_LEAF:
      return "The directory service can perform the requested operation only on a leaf object";

    case ERROR_DS_CANT_ON_RDN:
      return "The directory service cannot perform the requested operation on the RDN attribute of an object";

    case ERROR_DS_CANT_MOD_OBJ_CLASS:
      return "The directory service detected an attempt to modify the object class of an object";

    case ERROR_DS_CROSS_DOM_MOVE_ERROR:
      return "The requested cross-domain move operation could not be performed";

    case ERROR_DS_GC_NOT_AVAILABLE:
      return "Unable to contact the global catalog server";

    case ERROR_SHARED_POLICY:
      return "The policy object is shared and can only be modified at the root";

    case ERROR_POLICY_OBJECT_NOT_FOUND:
      return "The policy object does not exist";

    case ERROR_POLICY_ONLY_IN_DS:
      return "The requested policy information is only in the directory service";

    case ERROR_PROMOTION_ACTIVE:
      return "A domain controller promotion is currently active";

    case ERROR_NO_PROMOTION_ACTIVE:
      return "A domain controller promotion is not currently active";

    case ERROR_DS_OPERATIONS_ERROR:
      return "An operations error occurred";

    case ERROR_DS_PROTOCOL_ERROR:
      return "A protocol error occurred";

    case ERROR_DS_TIMELIMIT_EXCEEDED:
      return "The time limit for this request was exceeded";

    case ERROR_DS_SIZELIMIT_EXCEEDED:
      return "The size limit for this request was exceeded";

    case ERROR_DS_ADMIN_LIMIT_EXCEEDED:
      return "The administrative limit for this request was exceeded";

    case ERROR_DS_COMPARE_FALSE:
      return "The compare response was false";

    case ERROR_DS_COMPARE_TRUE:
      return "The compare response was true";

    case ERROR_DS_AUTH_METHOD_NOT_SUPPORTED:
      return "The requested authentication method is not supported by the server";

    case ERROR_DS_STRONG_AUTH_REQUIRED:
      return "A more secure authentication method is required for this server";

    case ERROR_DS_INAPPROPRIATE_AUTH:
      return "Inappropriate authentication";

    case ERROR_DS_AUTH_UNKNOWN:
      return "The authentication mechanism is unknown";

    case ERROR_DS_REFERRAL:
      return "A referral was returned from the server";

    case ERROR_DS_UNAVAILABLE_CRIT_EXTENSION:
      return "The server does not support the requested critical extension";

    case ERROR_DS_CONFIDENTIALITY_REQUIRED:
      return "This request requires a secure connection";

    case ERROR_DS_INAPPROPRIATE_MATCHING:
      return "Inappropriate matching";

    case ERROR_DS_CONSTRAINT_VIOLATION:
      return "A constraint violation occurred";

    case ERROR_DS_NO_SUCH_OBJECT:
      return "There is no such object on the server";

    case ERROR_DS_ALIAS_PROBLEM:
      return "There is an alias problem";

    case ERROR_DS_INVALID_DN_SYNTAX:
      return "An invalid dn syntax has been specified";

    case ERROR_DS_IS_LEAF:
      return "The object is a leaf object";

    case ERROR_DS_ALIAS_DEREF_PROBLEM:
      return "There is an alias dereferencing problem";

    case ERROR_DS_UNWILLING_TO_PERFORM:
      return "The server is unwilling to process the request";

    case ERROR_DS_LOOP_DETECT:
      return "A loop has been detected";

    case ERROR_DS_NAMING_VIOLATION:
      return "There is a naming violation";

    case ERROR_DS_OBJECT_RESULTS_TOO_LARGE:
      return "The result set is too large";

    case ERROR_DS_AFFECTS_MULTIPLE_DSAS:
      return "The operation affects multiple DSAs";

    case ERROR_DS_SERVER_DOWN:
      return "The server is not operational";

    case ERROR_DS_LOCAL_ERROR:
      return "A local error has occurred";

    case ERROR_DS_ENCODING_ERROR:
      return "An encoding error has occurred";

    case ERROR_DS_DECODING_ERROR:
      return "A decoding error has occurred";

    case ERROR_DS_FILTER_UNKNOWN:
      return "The search filter cannot be recognized";

    case ERROR_DS_PARAM_ERROR:
      return "One or more parameters are illegal";

    case ERROR_DS_NOT_SUPPORTED:
      return "The specified method is not supported";

    case ERROR_DS_NO_RESULTS_RETURNED:
      return "No results were returned";

    case ERROR_DS_CONTROL_NOT_FOUND:
      return "The specified control is not supported by the server";

    case ERROR_DS_CLIENT_LOOP:
      return "A referral loop was detected by the client";

    case ERROR_DS_REFERRAL_LIMIT_EXCEEDED:
      return "The preset referral limit was exceeded";

    case ERROR_DS_SORT_CONTROL_MISSING:
      return "The search requires a SORT control";

    case ERROR_DS_OFFSET_RANGE_ERROR:
      return "The search results exceed the offset range specified";

    case ERROR_DS_ROOT_MUST_BE_NC:
      return "The root object must be the head of a naming context. The root object cannot have an instantiated parent";

    case ERROR_DS_ADD_REPLICA_INHIBITED:
      return "The add replica operation cannot be performed. The naming context must be writeable in order to create the replica";

    case ERROR_DS_ATT_NOT_DEF_IN_SCHEMA:
      return "A reference to an attribute that is not defined in the schema occurred";

    case ERROR_DS_MAX_OBJ_SIZE_EXCEEDED:
      return "The maximum size of an object has been exceeded";

    case ERROR_DS_OBJ_STRING_NAME_EXISTS:
      return "An attempt was made to add an object to the directory with a name that is already in use";

    case ERROR_DS_NO_RDN_DEFINED_IN_SCHEMA:
      return "An attempt was made to add an object of a class that does not have an RDN defined in the schema";

    case ERROR_DS_RDN_DOESNT_MATCH_SCHEMA:
      return "An attempt was made to add an object using an RDN that is not the RDN defined in the schema";

    case ERROR_DS_NO_REQUESTED_ATTS_FOUND:
      return "None of the requested attributes were found on the objects";

    case ERROR_DS_USER_BUFFER_TO_SMALL:
      return "The user buffer is too small";

    case ERROR_DS_ATT_IS_NOT_ON_OBJ:
      return "The attribute specified in the operation is not present on the object";

    case ERROR_DS_ILLEGAL_MOD_OPERATION:
      return "Illegal modify operation. Some aspect of the modification is not permitted";

    case ERROR_DS_OBJ_TOO_LARGE:
      return "The specified object is too large";

    case ERROR_DS_BAD_INSTANCE_TYPE:
      return "The specified instance type is not valid";

    case ERROR_DS_MASTERDSA_REQUIRED:
      return "The operation must be performed at a master DSA";

    case ERROR_DS_OBJECT_CLASS_REQUIRED:
      return "The object class attribute must be specified";

    case ERROR_DS_MISSING_REQUIRED_ATT:
      return "A required attribute is missing";

    case ERROR_DS_ATT_NOT_DEF_FOR_CLASS:
      return "An attempt was made to modify an object to include an attribute that is not legal for its class";

    case ERROR_DS_ATT_ALREADY_EXISTS:
      return "The specified attribute is already present on the object";

    case ERROR_DS_CANT_ADD_ATT_VALUES:
      return "The specified attribute is not present, or has no values";

    case ERROR_DS_SINGLE_VALUE_CONSTRAINT:
      return "Multiple values were specified for an attribute that can have only one value";

    case ERROR_DS_RANGE_CONSTRAINT:
      return "A value for the attribute was not in the acceptable range of values";

    case ERROR_DS_ATT_VAL_ALREADY_EXISTS:
      return "The specified value already exists";

    case ERROR_DS_CANT_REM_MISSING_ATT:
      return "The attribute cannot be removed because it is not present on the object";

    case ERROR_DS_CANT_REM_MISSING_ATT_VAL:
      return "The attribute value cannot be removed because it is not present on the object";

    case ERROR_DS_ROOT_CANT_BE_SUBREF:
      return "The specified root object cannot be a subref";

    case ERROR_DS_NO_CHAINING:
      return "Chaining is not permitted";

    case ERROR_DS_NO_CHAINED_EVAL:
      return "Chained evaluation is not permitted";

    case ERROR_DS_NO_PARENT_OBJECT:
      return "The operation could not be performed because the object's parent is either uninstantiated or deleted";

    case ERROR_DS_PARENT_IS_AN_ALIAS:
      return "Having a parent that is an alias is not permitted. Aliases are leaf objects";

    case ERROR_DS_CANT_MIX_MASTER_AND_REPS:
      return "The object and parent must be of the same type, either both masters or both replicas";

    case ERROR_DS_CHILDREN_EXIST:
      return "The operation cannot be performed because child objects exist. This operation can only be performed on a leaf object";

    case ERROR_DS_OBJ_NOT_FOUND:
      return "Directory object not found";

    case ERROR_DS_ALIASED_OBJ_MISSING:
      return "The aliased object is missing";

    case ERROR_DS_BAD_NAME_SYNTAX:
      return "The object name has bad syntax";

    case ERROR_DS_ALIAS_POINTS_TO_ALIAS:
      return "It is not permitted for an alias to refer to another alias";

    case ERROR_DS_CANT_DEREF_ALIAS:
      return "The alias cannot be dereferenced";

    case ERROR_DS_OUT_OF_SCOPE:
      return "The operation is out of scope";

    case ERROR_DS_OBJECT_BEING_REMOVED:
      return "The operation cannot continue because the object is in the process of being removed";

    case ERROR_DS_CANT_DELETE_DSA_OBJ:
      return "The DSA object cannot be deleted";

    case ERROR_DS_GENERIC_ERROR:
      return "A directory service error has occurred";

    case ERROR_DS_DSA_MUST_BE_INT_MASTER:
      return "The operation can only be performed on an internal master DSA object";

    case ERROR_DS_CLASS_NOT_DSA:
      return "The object must be of class DSA";

    case ERROR_DS_INSUFF_ACCESS_RIGHTS:
      return "Insufficient access rights to perform the operation";

    case ERROR_DS_ILLEGAL_SUPERIOR:
      return "The object cannot be added because the parent is not on the list of possible superiors";

    case ERROR_DS_ATTRIBUTE_OWNED_BY_SAM:
      return "Access to the attribute is not permitted because the attribute is owned by the Security Accounts Manager (SAM)";

    case ERROR_DS_NAME_TOO_MANY_PARTS:
      return "The name has too many parts";

    case ERROR_DS_NAME_TOO_LONG:
      return "The name is too long";

    case ERROR_DS_NAME_VALUE_TOO_LONG:
      return "The name value is too long";

    case ERROR_DS_NAME_UNPARSEABLE:
      return "The directory service encountered an error parsing a name";

    case ERROR_DS_NAME_TYPE_UNKNOWN:
      return "The directory service cannot get the attribute type for a name";

    case ERROR_DS_NOT_AN_OBJECT:
      return "The name does not identify an object; the name identifies a phantom";

    case ERROR_DS_SEC_DESC_TOO_SHORT:
      return "The security descriptor is too short";

    case ERROR_DS_SEC_DESC_INVALID:
      return "The security descriptor is invalid";

    case ERROR_DS_NO_DELETED_NAME:
      return "Failed to create name for deleted object";

    case ERROR_DS_SUBREF_MUST_HAVE_PARENT:
      return "The parent of a new subref must exist";

    case ERROR_DS_NCNAME_MUST_BE_NC:
      return "The object must be a naming context";

    case ERROR_DS_CANT_ADD_SYSTEM_ONLY:
      return "It is not permitted to add an attribute which is owned by the system";

    case ERROR_DS_CLASS_MUST_BE_CONCRETE:
      return "The class of the object must be structural; you cannot instantiate an abstract class";

    case ERROR_DS_INVALID_DMD:
      return "The schema object could not be found";

    case ERROR_DS_OBJ_GUID_EXISTS:
      return "A local object with this GUID (dead or alive) already exists";

    case ERROR_DS_NOT_ON_BACKLINK:
      return "The operation cannot be performed on a back link";

    case ERROR_DS_NO_CROSSREF_FOR_NC:
      return "The cross reference for the specified naming context could not be found";

    case ERROR_DS_SHUTTING_DOWN:
      return "The operation could not be performed because the directory service is shutting down";

    case ERROR_DS_UNKNOWN_OPERATION:
      return "The directory service request is invalid";

    case ERROR_DS_INVALID_ROLE_OWNER:
      return "The role owner attribute could not be read";

    case ERROR_DS_COULDNT_CONTACT_FSMO:
      return "The requested FSMO operation failed. The current FSMO holder could not be contacted";

    case ERROR_DS_CROSS_NC_DN_RENAME:
      return "Modification of a DN across a naming context is not permitted";

    case ERROR_DS_CANT_MOD_SYSTEM_ONLY:
      return "The attribute cannot be modified because it is owned by the system";

    case ERROR_DS_REPLICATOR_ONLY:
      return "Only the replicator can perform this function";

    case ERROR_DS_OBJ_CLASS_NOT_DEFINED:
      return "The specified class is not defined";

    case ERROR_DS_OBJ_CLASS_NOT_SUBCLASS:
      return "The specified class is not a subclass";

    case ERROR_DS_NAME_REFERENCE_INVALID:
      return "The name reference is invalid";

    case ERROR_DS_CROSS_REF_EXISTS:
      return "A cross reference already exists";

    case ERROR_DS_CANT_DEL_MASTER_CROSSREF:
      return "It is not permitted to delete a master cross reference";

    case ERROR_DS_SUBTREE_NOTIFY_NOT_NC_HEAD:
      return "Subtree notifications are only supported on NC heads";

    case ERROR_DS_NOTIFY_FILTER_TOO_COMPLEX:
      return "Notification filter is too complex";

    case ERROR_DS_DUP_RDN:
      return "Schema update failed: duplicate RDN";

    case ERROR_DS_DUP_OID:
      return "Schema update failed: duplicate OID";

    case ERROR_DS_DUP_MAPI_ID:
      return "Schema update failed: duplicate MAPI identifier";

    case ERROR_DS_DUP_SCHEMA_ID_GUID:
      return "Schema update failed: duplicate schema-id GUID";

    case ERROR_DS_DUP_LDAP_DISPLAY_NAME:
      return "Schema update failed: duplicate LDAP display name";

    case ERROR_DS_SEMANTIC_ATT_TEST:
      return "Schema update failed: range-lower less than range upper";

    case ERROR_DS_SYNTAX_MISMATCH:
      return "Schema update failed: syntax mismatch";

    case ERROR_DS_EXISTS_IN_MUST_HAVE:
      return "Schema deletion failed: attribute is used in must-contain";

    case ERROR_DS_EXISTS_IN_MAY_HAVE:
      return "Schema deletion failed: attribute is used in may-contain";

    case ERROR_DS_NONEXISTENT_MAY_HAVE:
      return "Schema update failed: attribute in may-contain does not exist";

    case ERROR_DS_NONEXISTENT_MUST_HAVE:
      return "Schema update failed: attribute in must-contain does not exist";

    case ERROR_DS_AUX_CLS_TEST_FAIL:
      return "Schema update failed: class in aux-class list does not exist or is not an auxiliary class";

    case ERROR_DS_NONEXISTENT_POSS_SUP:
      return "Schema update failed: class in poss-superiors does not exist";

    case ERROR_DS_SUB_CLS_TEST_FAIL:
      return "Schema update failed: class in subclassof list does not exist or does not satisfy hierarchy rules";

    case ERROR_DS_BAD_RDN_ATT_ID_SYNTAX:
      return "Schema update failed: Rdn-Att-Id has wrong syntax";

    case ERROR_DS_EXISTS_IN_AUX_CLS:
      return "Schema deletion failed: class is used as auxiliary class";

    case ERROR_DS_EXISTS_IN_SUB_CLS:
      return "Schema deletion failed: class is used as sub class";

    case ERROR_DS_EXISTS_IN_POSS_SUP:
      return "Schema deletion failed: class is used as poss-superior";

    case ERROR_DS_RECALCSCHEMA_FAILED:
      return "Schema update failed in recalculating validation cache";

    case ERROR_DS_TREE_DELETE_NOT_FINISHED:
      return "The tree deletion is not finished.  The request must be made again to continue deleting the tree";

    case ERROR_DS_CANT_DELETE:
      return "The requested delete operation could not be performed";

    case ERROR_DS_ATT_SCHEMA_REQ_ID:
      return "Cannot read the governs class identifier for the schema record";

    case ERROR_DS_BAD_ATT_SCHEMA_SYNTAX:
      return "The attribute schema has bad syntax";

    case ERROR_DS_CANT_CACHE_ATT:
      return "The attribute could not be cached";

    case ERROR_DS_CANT_CACHE_CLASS:
      return "The class could not be cached";

    case ERROR_DS_CANT_REMOVE_ATT_CACHE:
      return "The attribute could not be removed from the cache";

    case ERROR_DS_CANT_REMOVE_CLASS_CACHE:
      return "The class could not be removed from the cache";

    case ERROR_DS_CANT_RETRIEVE_DN:
      return "The distinguished name attribute could not be read";

    case ERROR_DS_MISSING_SUPREF:
      return "No superior reference has been configured for the directory service. The directory service is therefore unable to issue referrals to objects outside this forest";

    case ERROR_DS_CANT_RETRIEVE_INSTANCE:
      return "The instance type attribute could not be retrieved";

    case ERROR_DS_CODE_INCONSISTENCY:
      return "An internal error has occurred";

    case ERROR_DS_DATABASE_ERROR:
      return "A database error has occurred";

    case ERROR_DS_GOVERNSID_MISSING:
      return "The attribute GOVERNSID is missing";

    case ERROR_DS_MISSING_EXPECTED_ATT:
      return "An expected attribute is missing";

    case ERROR_DS_NCNAME_MISSING_CR_REF:
      return "The specified naming context is missing a cross reference";

    case ERROR_DS_SECURITY_CHECKING_ERROR:
      return "A security checking error has occurred";

    case ERROR_DS_SCHEMA_NOT_LOADED:
      return "The schema is not loaded";

    case ERROR_DS_SCHEMA_ALLOC_FAILED:
      return "Schema allocation failed. Please check if the machine is running low on memory";

    case ERROR_DS_ATT_SCHEMA_REQ_SYNTAX:
      return "Failed to obtain the required syntax for the attribute schema";

    case ERROR_DS_GCVERIFY_ERROR:
      return "The global catalog verification failed. The global catalog is not available or does not support the operation. Some part of the directory is currently not available";

    case ERROR_DS_DRA_SCHEMA_MISMATCH:
      return "The replication operation failed because of a schema mismatch between the servers involved";

    case ERROR_DS_CANT_FIND_DSA_OBJ:
      return "The DSA object could not be found";

    case ERROR_DS_CANT_FIND_EXPECTED_NC:
      return "The naming context could not be found";

    case ERROR_DS_CANT_FIND_NC_IN_CACHE:
      return "The naming context could not be found in the cache";

    case ERROR_DS_CANT_RETRIEVE_CHILD:
      return "The child object could not be retrieved";

    case ERROR_DS_SECURITY_ILLEGAL_MODIFY:
      return "The modification was not permitted for security reasons";

    case ERROR_DS_CANT_REPLACE_HIDDEN_REC:
      return "The operation cannot replace the hidden record";

    case ERROR_DS_BAD_HIERARCHY_FILE:
      return "The hierarchy file is invalid";

    case ERROR_DS_BUILD_HIERARCHY_TABLE_FAILED:
      return "The attempt to build the hierarchy table failed";

    case ERROR_DS_CONFIG_PARAM_MISSING:
      return "The directory configuration parameter is missing from the registry";

    case ERROR_DS_COUNTING_AB_INDICES_FAILED:
      return "The attempt to count the address book indices failed";

    case ERROR_DS_HIERARCHY_TABLE_MALLOC_FAILED:
      return "The allocation of the hierarchy table failed";

    case ERROR_DS_INTERNAL_FAILURE:
      return "The directory service encountered an internal failure";

    case ERROR_DS_UNKNOWN_ERROR:
      return "The directory service encountered an unknown failure";

    case ERROR_DS_ROOT_REQUIRES_CLASS_TOP:
      return "A root object requires a class of 'top'";

    case ERROR_DS_REFUSING_FSMO_ROLES:
      return "This directory server is shutting down, and cannot take ownership of new floating single-master operation roles";

    case ERROR_DS_MISSING_FSMO_SETTINGS:
      return "The directory service is missing mandatory configuration information, and is unable to determine the ownership of floating single-master operation roles";

    case ERROR_DS_UNABLE_TO_SURRENDER_ROLES:
      return "The directory service was unable to transfer ownership of one or more floating single-master operation roles to other servers";

    case ERROR_DS_DRA_GENERIC:
      return "The replication operation failed";

    case ERROR_DS_DRA_INVALID_PARAMETER:
      return "An invalid parameter was specified for this replication operation";

    case ERROR_DS_DRA_BUSY:
      return "The directory service is too busy to complete the replication operation at this time";

    case ERROR_DS_DRA_BAD_DN:
      return "The distinguished name specified for this replication operation is invalid";

    case ERROR_DS_DRA_BAD_NC:
      return "The naming context specified for this replication operation is invalid";

    case ERROR_DS_DRA_DN_EXISTS:
      return "The distinguished name specified for this replication operation already exists";

    case ERROR_DS_DRA_INTERNAL_ERROR:
      return "The replication system encountered an internal error";

    case ERROR_DS_DRA_INCONSISTENT_DIT:
      return "The replication operation encountered a database inconsistency";

    case ERROR_DS_DRA_CONNECTION_FAILED:
      return "The server specified for this replication operation could not be contacted";

    case ERROR_DS_DRA_BAD_INSTANCE_TYPE:
      return "The replication operation encountered an object with an invalid instance type";

    case ERROR_DS_DRA_OUT_OF_MEM:
      return "The replication operation failed to allocate memory";

    case ERROR_DS_DRA_MAIL_PROBLEM:
      return "The replication operation encountered an error with the mail system";

    case ERROR_DS_DRA_REF_ALREADY_EXISTS:
      return "The replication reference information for the target server already exists";

    case ERROR_DS_DRA_REF_NOT_FOUND:
      return "The replication reference information for the target server does not exist";

    case ERROR_DS_DRA_OBJ_IS_REP_SOURCE:
      return "The naming context cannot be removed because it is replicated to another server";

    case ERROR_DS_DRA_DB_ERROR:
      return "The replication operation encountered a database error";

    case ERROR_DS_DRA_NO_REPLICA:
      return "The naming context is in the process of being removed or is not replicated from the specified server";

    case ERROR_DS_DRA_ACCESS_DENIED:
      return "Replication access was denied";

    case ERROR_DS_DRA_NOT_SUPPORTED:
      return "The requested operation is not supported by this version of the directory service";

    case ERROR_DS_DRA_RPC_CANCELLED:
      return "The replication remote procedure call was cancelled";

    case ERROR_DS_DRA_SOURCE_DISABLED:
      return "The source server is currently rejecting replication requests";

    case ERROR_DS_DRA_SINK_DISABLED:
      return "The destination server is currently rejecting replication requests";

    case ERROR_DS_DRA_NAME_COLLISION:
      return "The replication operation failed due to a collision of object names";

    case ERROR_DS_DRA_SOURCE_REINSTALLED:
      return "The replication source has been reinstalled";

    case ERROR_DS_DRA_MISSING_PARENT:
      return "The replication operation failed because a required parent object is missing";

    case ERROR_DS_DRA_PREEMPTED:
      return "The replication operation was preempted";

    case ERROR_DS_DRA_ABANDON_SYNC:
      return "The replication synchronization attempt was abandoned because of a lack of updates";

    case ERROR_DS_DRA_SHUTDOWN:
      return "The replication operation was terminated because the system is shutting down";

    case ERROR_DS_DRA_INCOMPATIBLE_PARTIAL_SET:
      return "Synchronization attempt failed because the destination DC is currently waiting to synchronize new partial attributes from source. This condition is normal if a recent schema change modified the partial attribute set. The destination partial attribute set is not a subset of source partial attribute set";

    case ERROR_DS_DRA_SOURCE_IS_PARTIAL_REPLICA:
      return "The replication synchronization attempt failed because a master replica attempted to sync from a partial replica";

    case ERROR_DS_DRA_EXTN_CONNECTION_FAILED:
      return "The server specified for this replication operation was contacted, but that server was unable to contact an additional server needed to complete the operation";

    case ERROR_DS_INSTALL_SCHEMA_MISMATCH:
      return "The version of the Active Directory schema of the source forest is not compatible with the version of Active Directory on this computer";

    case ERROR_DS_DUP_LINK_ID:
      return "Schema update failed: An attribute with the same link identifier already exists";

    case ERROR_DS_NAME_ERROR_RESOLVING:
      return "Name translation: Generic processing error";

    case ERROR_DS_NAME_ERROR_NOT_FOUND:
      return "Name translation: Could not find the name or insufficient right to see name";

    case ERROR_DS_NAME_ERROR_NOT_UNIQUE:
      return "Name translation: Input name mapped to more than one output name";

    case ERROR_DS_NAME_ERROR_NO_MAPPING:
      return "Name translation: Input name found, but not the associated output format";

    case ERROR_DS_NAME_ERROR_DOMAIN_ONLY:
      return "Name translation: Unable to resolve completely, only the domain was found";

    case ERROR_DS_NAME_ERROR_NO_SYNTACTICAL_MAPPING:
      return "Name translation: Unable to perform purely syntactical mapping at the client without going out to the wire";

    case ERROR_DS_CONSTRUCTED_ATT_MOD:
      return "Modification of a constructed attribute is not allowed";

    case ERROR_DS_WRONG_OM_OBJ_CLASS:
      return "The OM-Object-Class specified is incorrect for an attribute with the specified syntax";

    case ERROR_DS_DRA_REPL_PENDING:
      return "The replication request has been posted; waiting for reply";

    case ERROR_DS_DS_REQUIRED:
      return "The requested operation requires a directory service, and none was available";

    case ERROR_DS_INVALID_LDAP_DISPLAY_NAME:
      return "The LDAP display name of the class or attribute contains non-ASCII characters";

    case ERROR_DS_NON_BASE_SEARCH:
      return "The requested search operation is only supported for base searches";

    case ERROR_DS_CANT_RETRIEVE_ATTS:
      return "The search failed to retrieve attributes from the database";

    case ERROR_DS_BACKLINK_WITHOUT_LINK:
      return "The schema update operation tried to add a backward link attribute that has no corresponding forward link";

    case ERROR_DS_EPOCH_MISMATCH:
      return "Source and destination of a cross-domain move do not agree on the object's epoch number.  Either source or destination does not have the latest version of the object";

    case ERROR_DS_SRC_NAME_MISMATCH:
      return "Source and destination of a cross-domain move do not agree on the object's current name.  Either source or destination does not have the latest version of the object";

    case ERROR_DS_SRC_AND_DST_NC_IDENTICAL:
      return "Source and destination for the cross-domain move operation are identical.  Caller should use local move operation instead of cross-domain move operation";

    case ERROR_DS_DST_NC_MISMATCH:
      return "Source and destination for a cross-domain move are not in agreement on the naming contexts in the forest.  Either source or destination does not have the latest version of the Partitions container";

    case ERROR_DS_NOT_AUTHORITIVE_FOR_DST_NC:
      return "Destination of a cross-domain move is not authoritative for the destination naming context";

    case ERROR_DS_SRC_GUID_MISMATCH:
      return "Source and destination of a cross-domain move do not agree on the identity of the source object.  Either source or destination does not have the latest version of the source object";

    case ERROR_DS_CANT_MOVE_DELETED_OBJECT:
      return "Object being moved across-domains is already known to be deleted by the destination server.  The source server does not have the latest version of the source object";

    case ERROR_DS_PDC_OPERATION_IN_PROGRESS:
      return "Another operation which requires exclusive access to the PDC FSMO is already in progress";

    case ERROR_DS_CROSS_DOMAIN_CLEANUP_REQD:
      return "A cross-domain move operation failed such that two versions of the moved object exist - one each in the source and destination domains.  The destination object needs to be removed to restore the system to a consistent state";

    case ERROR_DS_ILLEGAL_XDOM_MOVE_OPERATION:
      return "This object may not be moved across domain boundaries either because cross-domain moves for this class are disallowed, or the object has some special characteristics, e.g.: trust account or restricted RID, which prevent its move";

    case ERROR_DS_CANT_WITH_ACCT_GROUP_MEMBERSHPS:
      return "Can't move objects with memberships across domain boundaries as once moved, this would violate the membership conditions of the account group.  Remove the object from any account group memberships and retry";

    case ERROR_DS_NC_MUST_HAVE_NC_PARENT:
      return "A naming context head must be the immediate child of another naming context head, not of an interior node";

    case ERROR_DS_CR_IMPOSSIBLE_TO_VALIDATE:
      return "The directory cannot validate the proposed naming context name because it does not hold a replica of the naming context above the proposed naming context.  Please ensure that the domain naming master role is held by a server that is configured as a global catalog server, and that the server is up to date with its replication partners. (Applies only to Windows 2000 Domain Naming masters)";

    case ERROR_DS_DST_DOMAIN_NOT_NATIVE:
      return "Destination domain must be in native mode";

    case ERROR_DS_MISSING_INFRASTRUCTURE_CONTAINER:
      return "The operation can not be performed because the server does not have an infrastructure container in the domain of interest";

    case ERROR_DS_CANT_MOVE_ACCOUNT_GROUP:
      return "Cross-domain move of non-empty account groups is not allowed";

    case ERROR_DS_CANT_MOVE_RESOURCE_GROUP:
      return "Cross-domain move of non-empty resource groups is not allowed";

    case ERROR_DS_INVALID_SEARCH_FLAG:
      return "The search flags for the attribute are invalid. The ANR bit is valid only on attributes of Unicode or Teletex strings";

    case ERROR_DS_NO_TREE_DELETE_ABOVE_NC:
      return "Tree deletions starting at an object which has an NC head as a descendant are not allowed";

    case ERROR_DS_COULDNT_LOCK_TREE_FOR_DELETE:
      return "The directory service failed to lock a tree in preparation for a tree deletion because the tree was in use";

    case ERROR_DS_COULDNT_IDENTIFY_OBJECTS_FOR_TREE_DELETE:
      return "The directory service failed to identify the list of objects to delete while attempting a tree deletion";

    case ERROR_DS_SAM_INIT_FAILURE:
      return "Security Accounts Manager initialization failed because of the following error: %1";

    case ERROR_DS_SENSITIVE_GROUP_VIOLATION:
      return "Only an administrator can modify the membership list of an administrative group";

    case ERROR_DS_CANT_MOD_PRIMARYGROUPID:
      return "Cannot change the primary group ID of a domain controller account";

    case ERROR_DS_ILLEGAL_BASE_SCHEMA_MOD:
      return "An attempt is made to modify the base schema";

    case ERROR_DS_NONSAFE_SCHEMA_CHANGE:
      return "Adding a new mandatory attribute to an existing class, deleting a mandatory attribute from an existing class, or adding an optional attribute to the special class Top that is not a backlink attribute (directly or through inheritance, for example, by adding or deleting an auxiliary class) is not allowed";

    case ERROR_DS_SCHEMA_UPDATE_DISALLOWED:
      return "Schema update is not allowed on this DC because the DC is not the schema FSMO Role Owner";

    case ERROR_DS_CANT_CREATE_UNDER_SCHEMA:
      return "An object of this class cannot be created under the schema container. You can only create attribute-schema and class-schema objects under the schema container";

    case ERROR_DS_INSTALL_NO_SRC_SCH_VERSION:
      return "The replica/child install failed to get the objectVersion attribute on the schema container on the source DC. Either the attribute is missing on the schema container or the credentials supplied do not have permission to read it";

    case ERROR_DS_INSTALL_NO_SCH_VERSION_IN_INIFILE:
      return "The replica/child install failed to read the objectVersion attribute in the SCHEMA section of the file schema.ini in the system32 directory";

    case ERROR_DS_INVALID_GROUP_TYPE:
      return "The specified group type is invalid";

    case ERROR_DS_NO_NEST_GLOBALGROUP_IN_MIXEDDOMAIN:
      return "You cannot nest global groups in a mixed domain if the group is security-enabled";

    case ERROR_DS_NO_NEST_LOCALGROUP_IN_MIXEDDOMAIN:
      return "You cannot nest local groups in a mixed domain if the group is security-enabled";

    case ERROR_DS_GLOBAL_CANT_HAVE_LOCAL_MEMBER:
      return "A global group cannot have a local group as a member";

    case ERROR_DS_GLOBAL_CANT_HAVE_UNIVERSAL_MEMBER:
      return "A global group cannot have a universal group as a member";

    case ERROR_DS_UNIVERSAL_CANT_HAVE_LOCAL_MEMBER:
      return "A universal group cannot have a local group as a member";

    case ERROR_DS_GLOBAL_CANT_HAVE_CROSSDOMAIN_MEMBER:
      return "A global group cannot have a cross-domain member";

    case ERROR_DS_LOCAL_CANT_HAVE_CROSSDOMAIN_LOCAL_MEMBER:
      return "A local group cannot have another cross domain local group as a member";

    case ERROR_DS_HAVE_PRIMARY_MEMBERS:
      return "A group with primary members cannot change to a security-disabled group";

    case ERROR_DS_STRING_SD_CONVERSION_FAILED:
      return "The schema cache load failed to convert the string default SD on a class-schema object";

    case ERROR_DS_NAMING_MASTER_GC:
      return "Only DSAs configured to be Global Catalog servers should be allowed to hold the Domain Naming Master FSMO role. (Applies only to Windows 2000 servers)";

    case ERROR_DS_DNS_LOOKUP_FAILURE:
      return "The DSA operation is unable to proceed because of a DNS lookup failure";

    case ERROR_DS_COULDNT_UPDATE_SPNS:
      return "While processing a change to the DNS Host Name for an object, the Service Principal Name values could not be kept in sync";

    case ERROR_DS_CANT_RETRIEVE_SD:
      return "The Security Descriptor attribute could not be read";

    case ERROR_DS_KEY_NOT_UNIQUE:
      return "The object requested was not found, but an object with that key was found";

    case ERROR_DS_WRONG_LINKED_ATT_SYNTAX:
      return "The syntax of the linked attribute being added is incorrect. Forward links can only have syntax 2.5.5.1, 2.5.5.7, and 2.5.5.14, and backlinks can only have syntax 2.5.5.1";

    case ERROR_DS_SAM_NEED_BOOTKEY_PASSWORD:
      return "Security Account Manager needs to get the boot password";

    case ERROR_DS_SAM_NEED_BOOTKEY_FLOPPY:
      return "Security Account Manager needs to get the boot key from floppy disk";

    case ERROR_DS_CANT_START:
      return "Directory Service cannot start";

    case ERROR_DS_INIT_FAILURE:
      return "Directory Services could not start";

    case ERROR_DS_NO_PKT_PRIVACY_ON_CONNECTION:
      return "The connection between client and server requires packet privacy or better";

    case ERROR_DS_SOURCE_DOMAIN_IN_FOREST:
      return "The source domain may not be in the same forest as destination";

    case ERROR_DS_DESTINATION_DOMAIN_NOT_IN_FOREST:
      return "The destination domain must be in the forest";

    case ERROR_DS_DESTINATION_AUDITING_NOT_ENABLED:
      return "The operation requires that destination domain auditing be enabled";

    case ERROR_DS_CANT_FIND_DC_FOR_SRC_DOMAIN:
      return "The operation couldn't locate a DC for the source domain";

    case ERROR_DS_SRC_OBJ_NOT_GROUP_OR_USER:
      return "The source object must be a group or user";

    case ERROR_DS_SRC_SID_EXISTS_IN_FOREST:
      return "The source object's SID already exists in destination forest";

    case ERROR_DS_SRC_AND_DST_OBJECT_CLASS_MISMATCH:
      return "The source and destination object must be of the same type";

    case ERROR_SAM_INIT_FAILURE:
      return "Security Accounts Manager initialization failed because of the following error: %1";

    case ERROR_DS_DRA_SCHEMA_INFO_SHIP:
      return "Schema information could not be included in the replication request";

    case ERROR_DS_DRA_SCHEMA_CONFLICT:
      return "The replication operation could not be completed due to a schema incompatibility";

    case ERROR_DS_DRA_EARLIER_SCHEMA_CONFLICT:
      return "The replication operation could not be completed due to a previous schema incompatibility";

    case ERROR_DS_DRA_OBJ_NC_MISMATCH:
      return "The replication update could not be applied because either the source or the destination has not yet received information regarding a recent cross-domain move operation";

    case ERROR_DS_NC_STILL_HAS_DSAS:
      return "The requested domain could not be deleted because there exist domain controllers that still host this domain";

    case ERROR_DS_GC_REQUIRED:
      return "The requested operation can be performed only on a global catalog server";

    case ERROR_DS_LOCAL_MEMBER_OF_LOCAL_ONLY:
      return "A local group can only be a member of other local groups in the same domain";

    case ERROR_DS_NO_FPO_IN_UNIVERSAL_GROUPS:
      return "Foreign security principals cannot be members of universal groups";

    case ERROR_DS_CANT_ADD_TO_GC:
      return "The attribute is not allowed to be replicated to the GC because of security reasons";

    case ERROR_DS_NO_CHECKPOINT_WITH_PDC:
      return "The checkpoint with the PDC could not be taken because there too many modifications being processed currently";

    case ERROR_DS_SOURCE_AUDITING_NOT_ENABLED:
      return "The operation requires that source domain auditing be enabled";

    case ERROR_DS_CANT_CREATE_IN_NONDOMAIN_NC:
      return "Security principal objects can only be created inside domain naming contexts";

    case ERROR_DS_INVALID_NAME_FOR_SPN:
      return "A Service Principal Name (SPN) could not be constructed because the provided hostname is not in the necessary format";

    case ERROR_DS_FILTER_USES_CONTRUCTED_ATTRS:
      return "A Filter was passed that uses constructed attributes";

    case ERROR_DS_UNICODEPWD_NOT_IN_QUOTES:
      return "The unicodePwd attribute value must be enclosed in double quotes";

    case ERROR_DS_MACHINE_ACCOUNT_QUOTA_EXCEEDED:
      return "Your computer could not be joined to the domain. You have exceeded the maximum number of computer accounts you are allowed to create in this domain. Contact your system administrator to have this limit reset or increased";

    case ERROR_DS_MUST_BE_RUN_ON_DST_DC:
      return "For security reasons, the operation must be run on the destination DC";

    case ERROR_DS_SRC_DC_MUST_BE_SP4_OR_GREATER:
      return "For security reasons, the source DC must be NT4SP4 or greater";

    case ERROR_DS_CANT_TREE_DELETE_CRITICAL_OBJ:
      return "Critical Directory Service System objects cannot be deleted during tree delete operations.  The tree delete may have been partially performed";

    case ERROR_DS_INIT_FAILURE_CONSOLE:
      return "Directory Services could not start because of the following error: %1";

    case ERROR_DS_SAM_INIT_FAILURE_CONSOLE:
      return "Security Accounts Manager initialization failed because of the following error: %1";

    case ERROR_DS_FOREST_VERSION_TOO_HIGH:
      return "The version of the operating system installed is incompatible with the current forest functional level. You must upgrade to a new version of the operating system before this server can become a domain controller in this forest";

    case ERROR_DS_DOMAIN_VERSION_TOO_HIGH:
      return "The version of the operating system installed is incompatible with the current domain functional level. You must upgrade to a new version of the operating system before this server can become a domain controller in this domain";

    case ERROR_DS_FOREST_VERSION_TOO_LOW:
      return "The version of the operating system installed on this server no longer supports the current forest functional level. You must raise the forest functional level before this server can become a domain controller in this forest";

    case ERROR_DS_DOMAIN_VERSION_TOO_LOW:
      return "The version of the operating system installed on this server no longer supports the current domain functional level. You must raise the domain functional level before this server can become a domain controller in this domain";

    case ERROR_DS_INCOMPATIBLE_VERSION:
      return "The version of the operating system installed on this server is incompatible with the functional level of the domain or forest";

    case ERROR_DS_LOW_DSA_VERSION:
      return "The functional level of the domain (or forest) cannot be raised to the requested value, because there exist one or more domain controllers in the domain (or forest) that are at a lower incompatible functional level";

    case ERROR_DS_NO_BEHAVIOR_VERSION_IN_MIXEDDOMAIN:
      return "The forest functional level cannot be raised to the requested value since one or more domains are still in mixed domain mode. All domains in the forest must be in native mode, for you to raise the forest functional level";

    case ERROR_DS_NOT_SUPPORTED_SORT_ORDER:
      return "The sort order requested is not supported";

    case ERROR_DS_NAME_NOT_UNIQUE:
      return "The requested name already exists as a unique identifier";

    case ERROR_DS_MACHINE_ACCOUNT_CREATED_PRENT4:
      return "The machine account was created pre-NT4.  The account needs to be recreated";

    case ERROR_DS_OUT_OF_VERSION_STORE:
      return "The database is out of version store";

    case ERROR_DS_INCOMPATIBLE_CONTROLS_USED:
      return "Unable to continue operation because multiple conflicting controls were used";

    case ERROR_DS_NO_REF_DOMAIN:
      return "Unable to find a valid security descriptor reference domain for this partition";

    case ERROR_DS_RESERVED_LINK_ID:
      return "Schema update failed: The link identifier is reserved";

    case ERROR_DS_LINK_ID_NOT_AVAILABLE:
      return "Schema update failed: There are no link identifiers available";

    case ERROR_DS_AG_CANT_HAVE_UNIVERSAL_MEMBER:
      return "An account group can not have a universal group as a member";

    case ERROR_DS_MODIFYDN_DISALLOWED_BY_INSTANCE_TYPE:
      return "Rename or move operations on naming context heads or read-only objects are not allowed";

    case ERROR_DS_NO_OBJECT_MOVE_IN_SCHEMA_NC:
      return "Move operations on objects in the schema naming context are not allowed";

    case ERROR_DS_MODIFYDN_DISALLOWED_BY_FLAG:
      return "A system flag has been set on the object and does not allow the object to be moved or renamed";

    case ERROR_DS_MODIFYDN_WRONG_GRANDPARENT:
      return "This object is not allowed to change its grandparent container. Moves are not forbidden on this object, but are restricted to sibling containers";

    case ERROR_DS_NAME_ERROR_TRUST_REFERRAL:
      return "Unable to resolve completely, a referral to another forest is generated";

    case ERROR_NOT_SUPPORTED_ON_STANDARD_SERVER:
      return "The requested action is not supported on standard server";

    case ERROR_DS_CANT_ACCESS_REMOTE_PART_OF_AD:
      return "Could not access a partition of the Active Directory located on a remote server.  Make sure at least one server is running for the partition in question";

    case ERROR_DS_CR_IMPOSSIBLE_TO_VALIDATE_V2:
      return "The directory cannot validate the proposed naming context (or partition) name because it does not hold a replica nor can it contact a replica of the naming context above the proposed naming context.  Please ensure that the parent naming context is properly registered in DNS, and at least one replica of this naming context is reachable by the Domain Naming master";

    case ERROR_DS_THREAD_LIMIT_EXCEEDED:
      return "The thread limit for this request was exceeded";

    case ERROR_DS_NOT_CLOSEST:
      return "The Global catalog server is not in the closest site";

    case ERROR_DS_CANT_DERIVE_SPN_WITHOUT_SERVER_REF:
      return "The DS cannot derive a service principal name (SPN) with which to mutually authenticate the target server because the corresponding server object in the local DS database has no serverReference attribute";

    case ERROR_DS_SINGLE_USER_MODE_FAILED:
      return "The Directory Service failed to enter single user mode";

    case ERROR_DS_NTDSCRIPT_SYNTAX_ERROR:
      return "The Directory Service cannot parse the script because of a syntax error";

    case ERROR_DS_NTDSCRIPT_PROCESS_ERROR:
      return "The Directory Service cannot process the script because of an error";

    case ERROR_DS_DIFFERENT_REPL_EPOCHS:
      return "The directory service cannot perform the requested operation because the servers";

    case ERROR_DS_DRS_EXTENSIONS_CHANGED:
      return "The directory service binding must be renegotiated due to a change in the server";

    case ERROR_DS_REPLICA_SET_CHANGE_NOT_ALLOWED_ON_DISABLED_CR:
      return "Operation not allowed on a disabled cross ref";

    case ERROR_DS_NO_MSDS_INTID:
      return "Schema update failed: No values for msDS-IntId are available";

    case ERROR_DS_DUP_MSDS_INTID:
      return "Schema update failed: Duplicate msDS-INtId. Retry the operation";

    case ERROR_DS_EXISTS_IN_RDNATTID:
      return "Schema deletion failed: attribute is used in rDNAttID";

    case ERROR_DS_AUTHORIZATION_FAILED:
      return "The directory service failed to authorize the request";

    case ERROR_DS_INVALID_SCRIPT:
      return "The Directory Service cannot process the script because it is invalid";

    case ERROR_DS_REMOTE_CROSSREF_OP_FAILED:
      return "The remote create cross reference operation failed on the Domain Naming Master FSMO.  The operation's error is in the extended data";

    case ERROR_DS_CROSS_REF_BUSY:
      return "A cross reference is in use locally with the same name";

    case ERROR_DS_CANT_DERIVE_SPN_FOR_DELETED_DOMAIN:
      return "The DS cannot derive a service principal name (SPN) with which to mutually authenticate the target server because the server's domain has been deleted from the forest";

    case ERROR_DS_CANT_DEMOTE_WITH_WRITEABLE_NC:
      return "Writeable NCs prevent this DC from demoting";

    case ERROR_DS_DUPLICATE_ID_FOUND:
      return "The requested object has a non-unique identifier and cannot be retrieved";

    case ERROR_DS_INSUFFICIENT_ATTR_TO_CREATE_OBJECT:
      return "Insufficient attributes were given to create an object.  This object may not exist because it may have been deleted and already garbage collected";

    case ERROR_DS_GROUP_CONVERSION_ERROR:
      return "The group cannot be converted due to attribute restrictions on the requested group type";

    case ERROR_DS_CANT_MOVE_APP_BASIC_GROUP:
      return "Cross-domain move of non-empty basic application groups is not allowed";

    case ERROR_DS_CANT_MOVE_APP_QUERY_GROUP:
      return "Cross-domain move of non-empty query based application groups is not allowed";

    case ERROR_DS_ROLE_NOT_VERIFIED:
      return "The FSMO role ownership could not be verified because its directory partition has not replicated successfully with atleast one replication partner";

    case ERROR_DS_WKO_CONTAINER_CANNOT_BE_SPECIAL:
      return "The target container for a redirection of a well known object container cannot already be a special container";

    case ERROR_DS_DOMAIN_RENAME_IN_PROGRESS:
      return "The Directory Service cannot perform the requested operation because a domain rename operation is in progress";

    case ERROR_DS_EXISTING_AD_CHILD_NC:
      return "The Active Directory detected an Active Directory child partition below the";

    case ERROR_DS_REPL_LIFETIME_EXCEEDED:
      return "The Active Directory cannot replicate with this server because the time since the last replication with this server has exceeded the tombstone lifetime";

    case ERROR_DS_DISALLOWED_IN_SYSTEM_CONTAINER:
      return "The requested operation is not allowed on an object under the system container";

    case ERROR_DS_LDAP_SEND_QUEUE_FULL:
      return "The LDAP servers network send queue has filled up because the client is not";

    case ERROR_DS_DRA_OUT_SCHEDULE_WINDOW:
      return "The scheduled replication did not take place because the system was too busy to execute the request within the schedule window.  The replication queue is overloaded. Consider reducing the number of partners or decreasing the scheduled replication frequency";

    case DNS_ERROR_RCODE_FORMAT_ERROR:
      return "DNS server unable to interpret format";

    case DNS_ERROR_RCODE_SERVER_FAILURE:
      return "DNS server failure";

    case DNS_ERROR_RCODE_NAME_ERROR:
      return "DNS name does not exist";

    case DNS_ERROR_RCODE_NOT_IMPLEMENTED:
      return "DNS request not supported by name server";

    case DNS_ERROR_RCODE_REFUSED:
      return "DNS operation refused";

    case DNS_ERROR_RCODE_YXDOMAIN:
      return "DNS name that ought not exist, does exist";

    case DNS_ERROR_RCODE_YXRRSET:
      return "DNS RR set that ought not exist, does exist";

    case DNS_ERROR_RCODE_NXRRSET:
      return "DNS RR set that ought to exist, does not exist";

    case DNS_ERROR_RCODE_NOTAUTH:
      return "DNS server not authoritative for zone";

    case DNS_ERROR_RCODE_NOTZONE:
      return "DNS name in update or prereq is not in zone";

    case DNS_ERROR_RCODE_BADSIG:
      return "DNS signature failed to verify";

    case DNS_ERROR_RCODE_BADKEY:
      return "DNS bad key";

    case DNS_ERROR_RCODE_BADTIME:
      return "DNS signature validity expired";

    case DNS_INFO_NO_RECORDS:
      return "No records found for given DNS query";

    case DNS_ERROR_BAD_PACKET:
      return "Bad DNS packet";

    case DNS_ERROR_NO_PACKET:
      return "No DNS packet";

    case DNS_ERROR_RCODE:
      return "DNS error, check rcode";

    case DNS_ERROR_UNSECURE_PACKET:
      return "Unsecured DNS packet";

    case DNS_ERROR_INVALID_TYPE:
      return "Invalid DNS type";

    case DNS_ERROR_INVALID_IP_ADDRESS:
      return "Invalid IP address";

    case DNS_ERROR_INVALID_PROPERTY:
      return "Invalid property";

    case DNS_ERROR_TRY_AGAIN_LATER:
      return "Try DNS operation again later";

    case DNS_ERROR_NOT_UNIQUE:
      return "Record for given name and type is not unique";

    case DNS_ERROR_NON_RFC_NAME:
      return "DNS name does not comply with RFC specifications";

    case DNS_STATUS_FQDN:
      return "DNS name is a fully-qualified DNS name";

    case DNS_STATUS_DOTTED_NAME:
      return "DNS name is dotted (multi-label)";

    case DNS_STATUS_SINGLE_PART_NAME:
      return "DNS name is a single-part name";

    case DNS_ERROR_INVALID_NAME_CHAR:
      return "DNS name contains an invalid character";

    case DNS_ERROR_NUMERIC_NAME:
      return "DNS name is entirely numeric";

    case DNS_ERROR_NOT_ALLOWED_ON_ROOT_SERVER:
      return "The operation requested is not permitted on a DNS root server";

    case DNS_ERROR_NOT_ALLOWED_UNDER_DELEGATION:
      return "The record could not be created because this part of the DNS namespace has";

    case DNS_ERROR_CANNOT_FIND_ROOT_HINTS:
      return "The DNS server could not find a set of root hints";

    case DNS_ERROR_INCONSISTENT_ROOT_HINTS:
      return "The DNS server found root hints but they were not consistent across";

    case DNS_ERROR_ZONE_DOES_NOT_EXIST:
      return "DNS zone does not exist";

    case DNS_ERROR_NO_ZONE_INFO:
      return "DNS zone information not available";

    case DNS_ERROR_INVALID_ZONE_OPERATION:
      return "Invalid operation for DNS zone";

    case DNS_ERROR_ZONE_CONFIGURATION_ERROR:
      return "Invalid DNS zone configuration";

    case DNS_ERROR_ZONE_HAS_NO_SOA_RECORD:
      return "DNS zone has no start of authority (SOA) record";

    case DNS_ERROR_ZONE_HAS_NO_NS_RECORDS:
      return "DNS zone has no Name Server (NS) record";

    case DNS_ERROR_ZONE_LOCKED:
      return "DNS zone is locked";

    case DNS_ERROR_ZONE_CREATION_FAILED:
      return "DNS zone creation failed";

    case DNS_ERROR_ZONE_ALREADY_EXISTS:
      return "DNS zone already exists";

    case DNS_ERROR_AUTOZONE_ALREADY_EXISTS:
      return "DNS automatic zone already exists";

    case DNS_ERROR_INVALID_ZONE_TYPE:
      return "Invalid DNS zone type";

    case DNS_ERROR_SECONDARY_REQUIRES_MASTER_IP:
      return "Secondary DNS zone requires master IP address";

    case DNS_ERROR_ZONE_NOT_SECONDARY:
      return "DNS zone not secondary";

    case DNS_ERROR_NEED_SECONDARY_ADDRESSES:
      return "Need secondary IP address";

    case DNS_ERROR_WINS_INIT_FAILED:
      return "WINS initialization failed";

    case DNS_ERROR_NEED_WINS_SERVERS:
      return "Need WINS servers";

    case DNS_ERROR_NBSTAT_INIT_FAILED:
      return "NBTSTAT initialization call failed";

    case DNS_ERROR_SOA_DELETE_INVALID:
      return "Invalid delete of start of authority (SOA)";

    case DNS_ERROR_FORWARDER_ALREADY_EXISTS:
      return "A conditional forwarding zone already exists for that name";

    case DNS_ERROR_ZONE_REQUIRES_MASTER_IP:
      return "This zone must be configured with one or more master DNS server IP addresses";

    case DNS_ERROR_ZONE_IS_SHUTDOWN:
      return "The operation cannot be performed because this zone is shutdown";

    case DNS_ERROR_PRIMARY_REQUIRES_DATAFILE:
      return "Primary DNS zone requires datafile";

    case DNS_ERROR_INVALID_DATAFILE_NAME:
      return "Invalid datafile name for DNS zone";

    case DNS_ERROR_DATAFILE_OPEN_FAILURE:
      return "Failed to open datafile for DNS zone";

    case DNS_ERROR_FILE_WRITEBACK_FAILED:
      return "Failed to write datafile for DNS zone";

    case DNS_ERROR_DATAFILE_PARSING:
      return "Failure while reading datafile for DNS zone";

    case DNS_ERROR_RECORD_DOES_NOT_EXIST:
      return "DNS record does not exist";

    case DNS_ERROR_RECORD_FORMAT:
      return "DNS record format error";

    case DNS_ERROR_NODE_CREATION_FAILED:
      return "Node creation failure in DNS";

    case DNS_ERROR_UNKNOWN_RECORD_TYPE:
      return "Unknown DNS record type";

    case DNS_ERROR_RECORD_TIMED_OUT:
      return "DNS record timed out";

    case DNS_ERROR_NAME_NOT_IN_ZONE:
      return "Name not in DNS zone";

    case DNS_ERROR_CNAME_LOOP:
      return "CNAME loop detected";

    case DNS_ERROR_NODE_IS_CNAME:
      return "Node is a CNAME DNS record";

    case DNS_ERROR_CNAME_COLLISION:
      return "A CNAME record already exists for given name";

    case DNS_ERROR_RECORD_ONLY_AT_ZONE_ROOT:
      return "Record only at DNS zone root";

    case DNS_ERROR_RECORD_ALREADY_EXISTS:
      return "DNS record already exists";

    case DNS_ERROR_SECONDARY_DATA:
      return "Secondary DNS zone data error";

    case DNS_ERROR_NO_CREATE_CACHE_DATA:
      return "Could not create DNS cache data";

    case DNS_ERROR_NAME_DOES_NOT_EXIST:
      return "DNS name does not exist";

    case DNS_WARNING_PTR_CREATE_FAILED:
      return "Could not create pointer (PTR) record";

    case DNS_WARNING_DOMAIN_UNDELETED:
      return "DNS domain was undeleted";

    case DNS_ERROR_DS_UNAVAILABLE:
      return "The directory service is unavailable";

    case DNS_ERROR_DS_ZONE_ALREADY_EXISTS:
      return "DNS zone already exists in the directory service";

    case DNS_ERROR_NO_BOOTFILE_IF_DS_ZONE:
      return "DNS server not creating or reading the boot file for the directory service integrated DNS zone";

    case DNS_INFO_AXFR_COMPLETE:
      return "DNS AXFR (zone transfer) complete";

    case DNS_ERROR_AXFR:
      return "DNS zone transfer failed";

    case DNS_INFO_ADDED_LOCAL_WINS:
      return "Added local WINS server";

    case DNS_STATUS_CONTINUE_NEEDED:
      return "Secure update call needs to continue update request";

    case DNS_ERROR_NO_TCPIP:
      return "TCP/IP network protocol not installed";

    case DNS_ERROR_NO_DNS_SERVERS:
      return "No DNS servers configured for local system";

    case DNS_ERROR_DP_DOES_NOT_EXIST:
      return "The specified directory partition does not exist";

    case DNS_ERROR_DP_ALREADY_EXISTS:
      return "The specified directory partition already exists";

    case DNS_ERROR_DP_NOT_ENLISTED:
      return "This DNS server is not enlisted in the specified directory partition";

    case DNS_ERROR_DP_ALREADY_ENLISTED:
      return "This DNS server is already enlisted in the specified directory partition";

    case DNS_ERROR_DP_NOT_AVAILABLE:
      return "The directory partition is not available at this time. Please wait";

    case DNS_ERROR_DP_FSMO_ERROR:
      return "The application directory partition operation failed. The domain controller";

    case WSAEINTR:
      return "A blocking operation was interrupted by a call to WSACancelBlockingCall";

    case WSAEBADF:
      return "The file handle supplied is not valid";

    case WSAEACCES:
      return "An attempt was made to access a socket in a way forbidden by its access permissions";

    case WSAEFAULT:
      return "The system detected an invalid pointer address in attempting to use a pointer argument in a call";

    case WSAEINVAL:
      return "An invalid argument was supplied";

    case WSAEMFILE:
      return "Too many open sockets";

    case WSAEWOULDBLOCK:
      return "A non-blocking socket operation could not be completed immediately";

    case WSAEINPROGRESS:
      return "A blocking operation is currently executing";

    case WSAEALREADY:
      return "An operation was attempted on a non-blocking socket that already had an operation in progress";

    case WSAENOTSOCK:
      return "An operation was attempted on something that is not a socket";

    case WSAEDESTADDRREQ:
      return "A required address was omitted from an operation on a socket";

    case WSAEMSGSIZE:
      return "A message sent on a datagram socket was larger than the internal message buffer or some other network limit, or the buffer used to receive a datagram into was smaller than the datagram itself";

    case WSAEPROTOTYPE:
      return "A protocol was specified in the socket function call that does not support the semantics of the socket type requested";

    case WSAENOPROTOOPT:
      return "An unknown, invalid, or unsupported option or level was specified in a getsockopt or setsockopt call";

    case WSAEPROTONOSUPPORT:
      return "The requested protocol has not been configured into the system, or no implementation for it exists";

    case WSAESOCKTNOSUPPORT:
      return "The support for the specified socket type does not exist in this address family";

    case WSAEOPNOTSUPP:
      return "The attempted operation is not supported for the type of object referenced";

    case WSAEPFNOSUPPORT:
      return "The protocol family has not been configured into the system or no implementation for it exists";

    case WSAEAFNOSUPPORT:
      return "An address incompatible with the requested protocol was used";

    case WSAEADDRINUSE:
      return "Only one usage of each socket address (protocol/network address/port) is normally permitted";

    case WSAEADDRNOTAVAIL:
      return "The requested address is not valid in its context";

    case WSAENETDOWN:
      return "A socket operation encountered a dead network";

    case WSAENETUNREACH:
      return "A socket operation was attempted to an unreachable network";

    case WSAENETRESET:
      return "The connection has been broken due to keep-alive activity detecting a failure while the operation was in progress";

    case WSAECONNABORTED:
      return "An established connection was aborted by the software in your host machine";

    case WSAECONNRESET:
      return "An existing connection was forcibly closed by the remote host";

    case WSAENOBUFS:
      return "An operation on a socket could not be performed because the system lacked sufficient buffer space or because a queue was full";

    case WSAEISCONN:
      return "A connect request was made on an already connected socket";

    case WSAENOTCONN:
      return "A request to send or receive data was disallowed because the socket is not connected and (when sending on a datagram socket using a sendto call) no address was supplied";

    case WSAESHUTDOWN:
      return "A request to send or receive data was disallowed because the socket had already been shut down in that direction with a previous shutdown call";

    case WSAETOOMANYREFS:
      return "Too many references to some kernel object";

    case WSAETIMEDOUT:
      return "A connection attempt failed because the connected party did not properly respond after a period of time, or established connection failed because connected host has failed to respond";

    case WSAECONNREFUSED:
      return "No connection could be made because the target machine actively refused it";

    case WSAELOOP:
      return "Cannot translate name";

    case WSAENAMETOOLONG:
      return "Name component or name was too long";

    case WSAEHOSTDOWN:
      return "A socket operation failed because the destination host was down";

    case WSAEHOSTUNREACH:
      return "A socket operation was attempted to an unreachable host";

    case WSAENOTEMPTY:
      return "Cannot remove a directory that is not empty";

    case WSAEPROCLIM:
      return "A Windows Sockets implementation may have a limit on the number of applications that may use it simultaneously";

    case WSAEUSERS:
      return "Ran out of quota";

    case WSAEDQUOT:
      return "Ran out of disk quota";

    case WSAESTALE:
      return "File handle reference is no longer available";

    case WSAEREMOTE:
      return "Item is not available locally";

    case WSASYSNOTREADY:
      return "WSAStartup cannot function at this time because the underlying system it uses to provide network services is currently unavailable";

    case WSAVERNOTSUPPORTED:
      return "The Windows Sockets version requested is not supported";

    case WSANOTINITIALISED:
      return "Either the application has not called WSAStartup, or WSAStartup failed";

    case WSAEDISCON:
      return "Returned by WSARecv or WSARecvFrom to indicate the remote party has initiated a graceful shutdown sequence";

    case WSAENOMORE:
      return "No more results can be returned by WSALookupServiceNext";

    case WSAECANCELLED:
      return "A call to WSALookupServiceEnd was made while this call was still processing. The call has been canceled";

    case WSAEINVALIDPROCTABLE:
      return "The procedure call table is invalid";

    case WSAEINVALIDPROVIDER:
      return "The requested service provider is invalid";

    case WSAEPROVIDERFAILEDINIT:
      return "The requested service provider could not be loaded or initialized";

    case WSASYSCALLFAILURE:
      return "A system call that should never fail has failed";

    case WSASERVICE_NOT_FOUND:
      return "No such service is known. The service cannot be found in the specified name space";

    case WSATYPE_NOT_FOUND:
      return "The specified class was not found";

    case WSA_E_NO_MORE:
      return "No more results can be returned by WSALookupServiceNext";

    case WSA_E_CANCELLED:
      return "A call to WSALookupServiceEnd was made while this call was still processing. The call has been canceled";

    case WSAEREFUSED:
      return "A database query failed because it was actively refused";

    case WSAHOST_NOT_FOUND:
      return "No such host is known";

    case WSATRY_AGAIN:
      return "This is usually a temporary error during hostname resolution and means that the local server did not receive a response from an authoritative server";

    case WSANO_RECOVERY:
      return "A non-recoverable error occurred during a database lookup";

    case WSANO_DATA:
      return "The requested name is valid, but no data of the requested type was found";

    case WSA_QOS_RECEIVERS:
      return "At least one reserve has arrived";

    case WSA_QOS_SENDERS:
      return "At least one path has arrived";

    case WSA_QOS_NO_SENDERS:
      return "There are no senders";

    case WSA_QOS_NO_RECEIVERS:
      return "There are no receivers";

    case WSA_QOS_REQUEST_CONFIRMED:
      return "Reserve has been confirmed";

    case WSA_QOS_ADMISSION_FAILURE:
      return "Error due to lack of resources";

    case WSA_QOS_POLICY_FAILURE:
      return "Rejected for administrative reasons - bad credentials";

    case WSA_QOS_BAD_STYLE:
      return "Unknown or conflicting style";

    case WSA_QOS_BAD_OBJECT:
      return "Problem with some part of the filterspec or providerspecific buffer in general";

    case WSA_QOS_TRAFFIC_CTRL_ERROR:
      return "Problem with some part of the flowspec";

    case WSA_QOS_GENERIC_ERROR:
      return "General QOS error";

    case WSA_QOS_ESERVICETYPE:
      return "An invalid or unrecognized service type was found in the flowspec";

    case WSA_QOS_EFLOWSPEC:
      return "An invalid or inconsistent flowspec was found in the QOS structure";

    case WSA_QOS_EPROVSPECBUF:
      return "Invalid QOS provider-specific buffer";

    case WSA_QOS_EFILTERSTYLE:
      return "An invalid QOS filter style was used";

    case WSA_QOS_EFILTERTYPE:
      return "An invalid QOS filter type was used";

    case WSA_QOS_EFILTERCOUNT:
      return "An incorrect number of QOS FILTERSPECs were specified in the FLOWDESCRIPTOR";

    case WSA_QOS_EOBJLENGTH:
      return "An object with an invalid ObjectLength field was specified in the QOS provider-specific buffer";

    case WSA_QOS_EFLOWCOUNT:
      return "An incorrect number of flow descriptors was specified in the QOS structure";

    case WSA_QOS_EUNKOWNPSOBJ:
      return "An unrecognized object was found in the QOS provider-specific buffer";

    case WSA_QOS_EPOLICYOBJ:
      return "An invalid policy object was found in the QOS provider-specific buffer";

    case WSA_QOS_EFLOWDESC:
      return "An invalid QOS flow descriptor was found in the flow descriptor list";

    case WSA_QOS_EPSFLOWSPEC:
      return "An invalid or inconsistent flowspec was found in the QOS provider specific buffer";

    case WSA_QOS_EPSFILTERSPEC:
      return "An invalid FILTERSPEC was found in the QOS provider-specific buffer";

    case WSA_QOS_ESDMODEOBJ:
      return "An invalid shape discard mode object was found in the QOS provider specific buffer";

    case WSA_QOS_ESHAPERATEOBJ:
      return "An invalid shaping rate object was found in the QOS provider-specific buffer";

    case WSA_QOS_RESERVED_PETYPE:
      return "A reserved policy element was found in the QOS provider-specific buffer";

    case ERROR_SXS_SECTION_NOT_FOUND:
      return "The requested section was not present in the activation context";

    case ERROR_SXS_CANT_GEN_ACTCTX:
      return "This application has failed to start because the application configuration is incorrect. Reinstalling the application may fix this problem";

    case ERROR_SXS_INVALID_ACTCTXDATA_FORMAT:
      return "The application binding data format is invalid";

    case ERROR_SXS_ASSEMBLY_NOT_FOUND:
      return "The referenced assembly is not installed on your system";

    case ERROR_SXS_MANIFEST_FORMAT_ERROR:
      return "The manifest file does not begin with the required tag and format information";

    case ERROR_SXS_MANIFEST_PARSE_ERROR:
      return "The manifest file contains one or more syntax errors";

    case ERROR_SXS_ACTIVATION_CONTEXT_DISABLED:
      return "The application attempted to activate a disabled activation context";

    case ERROR_SXS_KEY_NOT_FOUND:
      return "The requested lookup key was not found in any active activation context";

    case ERROR_SXS_VERSION_CONFLICT:
      return "A component version required by the application conflicts with another component version already active";

    case ERROR_SXS_WRONG_SECTION_TYPE:
      return "The type requested activation context section does not match the query API used";

    case ERROR_SXS_THREAD_QUERIES_DISABLED:
      return "Lack of system resources has required isolated activation to be disabled for the current thread of execution";

    case ERROR_SXS_PROCESS_DEFAULT_ALREADY_SET:
      return "An attempt to set the process default activation context failed because the process default activation context was already set";

    case ERROR_SXS_UNKNOWN_ENCODING_GROUP:
      return "The encoding group identifier specified is not recognized";

    case ERROR_SXS_UNKNOWN_ENCODING:
      return "The encoding requested is not recognized";

    case ERROR_SXS_INVALID_XML_NAMESPACE_URI:
      return "The manifest contains a reference to an invalid URI";

    case ERROR_SXS_ROOT_MANIFEST_DEPENDENCY_NOT_INSTALLED:
      return "The application manifest contains a reference to a dependent assembly which is not installed";

    case ERROR_SXS_LEAF_MANIFEST_DEPENDENCY_NOT_INSTALLED:
      return "The manifest for an assembly used by the application has a reference to a dependent assembly which is not installed";

    case ERROR_SXS_INVALID_ASSEMBLY_IDENTITY_ATTRIBUTE:
      return "The manifest contains an attribute for the assembly identity which is not valid";

    case ERROR_SXS_MANIFEST_MISSING_REQUIRED_DEFAULT_NAMESPACE:
      return "The manifest is missing the required default namespace specification on the assembly element";

    case ERROR_SXS_MANIFEST_INVALID_REQUIRED_DEFAULT_NAMESPACE:
      return "The manifest has a default namespace specified on the assembly element but its value is not \"urn:schemas-microsoft-com:asm.v1\"";

    case ERROR_SXS_PRIVATE_MANIFEST_CROSS_PATH_WITH_REPARSE_POINT:
      return "The private manifest probed has crossed reparse-point-associated path";

    case ERROR_SXS_DUPLICATE_DLL_NAME:
      return "Two or more components referenced directly or indirectly by the application manifest have files by the same name";

    case ERROR_SXS_DUPLICATE_WINDOWCLASS_NAME:
      return "Two or more components referenced directly or indirectly by the application manifest have window classes with the same name";

    case ERROR_SXS_DUPLICATE_CLSID:
      return "Two or more components referenced directly or indirectly by the application manifest have the same COM server CLSIDs";

    case ERROR_SXS_DUPLICATE_IID:
      return "Two or more components referenced directly or indirectly by the application manifest have proxies for the same COM interface IIDs";

    case ERROR_SXS_DUPLICATE_TLBID:
      return "Two or more components referenced directly or indirectly by the application manifest have the same COM type library TLBIDs";

    case ERROR_SXS_DUPLICATE_PROGID:
      return "Two or more components referenced directly or indirectly by the application manifest have the same COM ProgIDs";

    case ERROR_SXS_DUPLICATE_ASSEMBLY_NAME:
      return "Two or more components referenced directly or indirectly by the application manifest are different versions of the same component which is not permitted";

    case ERROR_SXS_FILE_HASH_MISMATCH:
      return "A component's file does not match the verification information present in the";

    case ERROR_SXS_POLICY_PARSE_ERROR:
      return "The policy manifest contains one or more syntax errors";

    case ERROR_SXS_XML_E_MISSINGQUOTE:
      return "Manifest Parse Error : A string literal was expected, but no opening quote character was found";

    case ERROR_SXS_XML_E_COMMENTSYNTAX:
      return "Manifest Parse Error : Incorrect syntax was used in a comment";

    case ERROR_SXS_XML_E_BADSTARTNAMECHAR:
      return "Manifest Parse Error : A name was started with an invalid character";

    case ERROR_SXS_XML_E_BADNAMECHAR:
      return "Manifest Parse Error : A name contained an invalid character";

    case ERROR_SXS_XML_E_BADCHARINSTRING:
      return "Manifest Parse Error : A string literal contained an invalid character";

    case ERROR_SXS_XML_E_XMLDECLSYNTAX:
      return "Manifest Parse Error : Invalid syntax for an xml declaration";

    case ERROR_SXS_XML_E_BADCHARDATA:
      return "Manifest Parse Error : An Invalid character was found in text content";

    case ERROR_SXS_XML_E_MISSINGWHITESPACE:
      return "Manifest Parse Error : Required white space was missing";

    case ERROR_SXS_XML_E_EXPECTINGTAGEND:
      return "Manifest Parse Error : The character '>' was expected";

    case ERROR_SXS_XML_E_MISSINGSEMICOLON:
      return "Manifest Parse Error : A semi colon character was expected";

    case ERROR_SXS_XML_E_UNBALANCEDPAREN:
      return "Manifest Parse Error : Unbalanced parentheses";

    case ERROR_SXS_XML_E_INTERNALERROR:
      return "Manifest Parse Error : Internal error";

    case ERROR_SXS_XML_E_UNEXPECTED_WHITESPACE:
      return "Manifest Parse Error : Whitespace is not allowed at this location";

    case ERROR_SXS_XML_E_INCOMPLETE_ENCODING:
      return "Manifest Parse Error : End of file reached in invalid state for current encoding";

    case ERROR_SXS_XML_E_MISSING_PAREN:
      return "Manifest Parse Error : Missing parenthesis";

    case ERROR_SXS_XML_E_EXPECTINGCLOSEQUOTE:
      return "Manifest Parse Error : A single or double closing quote character (\' or \\") is missing";

    case ERROR_SXS_XML_E_MULTIPLE_COLONS:
      return "Manifest Parse Error : Multiple colons are not allowed in a name";

    case ERROR_SXS_XML_E_INVALID_DECIMAL:
      return "Manifest Parse Error : Invalid character for decimal digit";

    case ERROR_SXS_XML_E_INVALID_HEXIDECIMAL:
      return "Manifest Parse Error : Invalid character for hexadecimal digit";

    case ERROR_SXS_XML_E_INVALID_UNICODE:
      return "Manifest Parse Error : Invalid unicode character value for this platform";

    case ERROR_SXS_XML_E_WHITESPACEORQUESTIONMARK:
      return "Manifest Parse Error : Expecting whitespace or '?'";

    case ERROR_SXS_XML_E_UNEXPECTEDENDTAG:
      return "Manifest Parse Error : End tag was not expected at this location";

    case ERROR_SXS_XML_E_UNCLOSEDTAG:
      return "Manifest Parse Error : The following tags were not closed: %1";

    case ERROR_SXS_XML_E_DUPLICATEATTRIBUTE:
      return "Manifest Parse Error : Duplicate attribute";

    case ERROR_SXS_XML_E_MULTIPLEROOTS:
      return "Manifest Parse Error : Only one top level element is allowed in an XML document";

    case ERROR_SXS_XML_E_INVALIDATROOTLEVEL:
      return "Manifest Parse Error : Invalid at the top level of the document";

    case ERROR_SXS_XML_E_BADXMLDECL:
      return "Manifest Parse Error : Invalid xml declaration";

    case ERROR_SXS_XML_E_MISSINGROOT:
      return "Manifest Parse Error : XML document must have a top level element";

    case ERROR_SXS_XML_E_UNEXPECTEDEOF:
      return "Manifest Parse Error : Unexpected end of file";

    case ERROR_SXS_XML_E_BADPEREFINSUBSET:
      return "Manifest Parse Error : Parameter entities cannot be used inside markup declarations in an internal subset";

    case ERROR_SXS_XML_E_UNCLOSEDSTARTTAG:
      return "Manifest Parse Error : Element was not closed";

    case ERROR_SXS_XML_E_UNCLOSEDENDTAG:
      return "Manifest Parse Error : End element was missing the character '>'";

    case ERROR_SXS_XML_E_UNCLOSEDSTRING:
      return "Manifest Parse Error : A string literal was not closed";

    case ERROR_SXS_XML_E_UNCLOSEDCOMMENT:
      return "Manifest Parse Error : A comment was not closed";

    case ERROR_SXS_XML_E_UNCLOSEDDECL:
      return "Manifest Parse Error : A declaration was not closed";

    case ERROR_SXS_XML_E_UNCLOSEDCDATA:
      return "Manifest Parse Error : A CDATA section was not closed";

    case ERROR_SXS_XML_E_RESERVEDNAMESPACE:
      return "Manifest Parse Error : The namespace prefix is not allowed to start with the reserved string \"xml\"";

    case ERROR_SXS_XML_E_INVALIDENCODING:
      return "Manifest Parse Error : System does not support the specified encoding";

    case ERROR_SXS_XML_E_INVALIDSWITCH:
      return "Manifest Parse Error : Switch from current encoding to specified encoding not supported";

    case ERROR_SXS_XML_E_BADXMLCASE:
      return "Manifest Parse Error : The name 'xml' is reserved and must be lower case";

    case ERROR_SXS_XML_E_INVALID_STANDALONE:
      return "Manifest Parse Error : The standalone attribute must have the value 'yes' or 'no'";

    case ERROR_SXS_XML_E_UNEXPECTED_STANDALONE:
      return "Manifest Parse Error : The standalone attribute cannot be used in external entities";

    case ERROR_SXS_XML_E_INVALID_VERSION:
      return "Manifest Parse Error : Invalid version number";

    case ERROR_SXS_XML_E_MISSINGEQUALS:
      return "Manifest Parse Error : Missing equals sign between attribute and attribute value";

    case ERROR_SXS_PROTECTION_RECOVERY_FAILED:
      return "Assembly Protection Error : Unable to recover the specified assembly";

    case ERROR_SXS_PROTECTION_PUBLIC_KEY_TOO_SHORT:
      return "Assembly Protection Error : The public key for an assembly was too short to be allowed";

    case ERROR_SXS_PROTECTION_CATALOG_NOT_VALID:
      return "Assembly Protection Error : The catalog for an assembly is not valid, or does not match the assembly's manifest";

    case ERROR_SXS_UNTRANSLATABLE_HRESULT:
      return "An HRESULT could not be translated to a corresponding Win32 error code";

    case ERROR_SXS_PROTECTION_CATALOG_FILE_MISSING:
      return "Assembly Protection Error : The catalog for an assembly is missing";

    case ERROR_SXS_MISSING_ASSEMBLY_IDENTITY_ATTRIBUTE:
      return "The supplied assembly identity is missing one or more attributes which must be present in this context";

    case ERROR_SXS_INVALID_ASSEMBLY_IDENTITY_ATTRIBUTE_NAME:
      return "The supplied assembly identity has one or more attribute names that contain characters not permitted in XML names";

    case ERROR_IPSEC_QM_POLICY_EXISTS:
      return "The specified quick mode policy already exists";

    case ERROR_IPSEC_QM_POLICY_NOT_FOUND:
      return "The specified quick mode policy was not found";

    case ERROR_IPSEC_QM_POLICY_IN_USE:
      return "The specified quick mode policy is being used";

    case ERROR_IPSEC_MM_POLICY_EXISTS:
      return "The specified main mode policy already exists";

    case ERROR_IPSEC_MM_POLICY_NOT_FOUND:
      return "The specified main mode policy was not found";

    case ERROR_IPSEC_MM_POLICY_IN_USE:
      return "The specified main mode policy is being used";

    case ERROR_IPSEC_MM_FILTER_EXISTS:
      return "The specified main mode filter already exists";

    case ERROR_IPSEC_MM_FILTER_NOT_FOUND:
      return "The specified main mode filter was not found";

    case ERROR_IPSEC_TRANSPORT_FILTER_EXISTS:
      return "The specified transport mode filter already exists";

    case ERROR_IPSEC_TRANSPORT_FILTER_NOT_FOUND:
      return "The specified transport mode filter does not exist";

    case ERROR_IPSEC_MM_AUTH_EXISTS:
      return "The specified main mode authentication list exists";

    case ERROR_IPSEC_MM_AUTH_NOT_FOUND:
      return "The specified main mode authentication list was not found";

    case ERROR_IPSEC_MM_AUTH_IN_USE:
      return "The specified quick mode policy is being used";

    case ERROR_IPSEC_DEFAULT_MM_POLICY_NOT_FOUND:
      return "The specified main mode policy was not found";

    case ERROR_IPSEC_DEFAULT_MM_AUTH_NOT_FOUND:
      return "The specified quick mode policy was not found";

    case ERROR_IPSEC_DEFAULT_QM_POLICY_NOT_FOUND:
      return "The manifest file contains one or more syntax errors";

    case ERROR_IPSEC_TUNNEL_FILTER_EXISTS:
      return "The application attempted to activate a disabled activation context";

    case ERROR_IPSEC_TUNNEL_FILTER_NOT_FOUND:
      return "The requested lookup key was not found in any active activation context";

    case ERROR_IPSEC_MM_FILTER_PENDING_DELETION:
      return "The Main Mode filter is pending deletion";

    case ERROR_IPSEC_TRANSPORT_FILTER_PENDING_DELETION:
      return "The transport filter is pending deletion";

    case ERROR_IPSEC_TUNNEL_FILTER_PENDING_DELETION:
      return "The tunnel filter is pending deletion";

    case ERROR_IPSEC_MM_POLICY_PENDING_DELETION:
      return "The Main Mode policy is pending deletion";

    case ERROR_IPSEC_MM_AUTH_PENDING_DELETION:
      return "The Main Mode authentication bundle is pending deletion";

    case ERROR_IPSEC_QM_POLICY_PENDING_DELETION:
      return "The Quick Mode policy is pending deletion";

    case WARNING_IPSEC_MM_POLICY_PRUNED:
      return "The Main Mode policy was successfully added, but some of the requested offers are not supported";

    case WARNING_IPSEC_QM_POLICY_PRUNED:
      return "The Quick Mode policy was successfully added, but some of the requested offers are not supported";

    case ERROR_IPSEC_IKE_NEG_STATUS_BEGIN:
      return "ERROR_IPSEC_IKE_NEG_STATUS_BEGIN";

    case ERROR_IPSEC_IKE_AUTH_FAIL:
      return "IKE authentication credentials are unacceptable";

    case ERROR_IPSEC_IKE_ATTRIB_FAIL:
      return "IKE security attributes are unacceptable";

    case ERROR_IPSEC_IKE_NEGOTIATION_PENDING:
      return "IKE Negotiation in progress";

    case ERROR_IPSEC_IKE_GENERAL_PROCESSING_ERROR:
      return "General processing error";

    case ERROR_IPSEC_IKE_TIMED_OUT:
      return "Negotiation timed out";

    case ERROR_IPSEC_IKE_NO_CERT:
      return "IKE failed to find valid machine certificate";

    case ERROR_IPSEC_IKE_SA_DELETED:
      return "IKE SA deleted by peer before establishment completed";

    case ERROR_IPSEC_IKE_SA_REAPED:
      return "IKE SA deleted before establishment completed";

    case ERROR_IPSEC_IKE_MM_ACQUIRE_DROP:
      return "Negotiation request sat in Queue too long";

    case ERROR_IPSEC_IKE_QM_ACQUIRE_DROP:
      return "Negotiation request sat in Queue too long";

    case ERROR_IPSEC_IKE_QUEUE_DROP_MM:
      return "Negotiation request sat in Queue too long";

    case ERROR_IPSEC_IKE_QUEUE_DROP_NO_MM:
      return "Negotiation request sat in Queue too long";

    case ERROR_IPSEC_IKE_DROP_NO_RESPONSE:
      return "No response from peer";

    case ERROR_IPSEC_IKE_MM_DELAY_DROP:
      return "Negotiation took too long";

    case ERROR_IPSEC_IKE_QM_DELAY_DROP:
      return "Negotiation took too long";

    case ERROR_IPSEC_IKE_ERROR:
      return "Unknown error occurred";

    case ERROR_IPSEC_IKE_CRL_FAILED:
      return "Certificate Revocation Check failed";

    case ERROR_IPSEC_IKE_INVALID_KEY_USAGE:
      return "Invalid certificate key usage";

    case ERROR_IPSEC_IKE_INVALID_CERT_TYPE:
      return "Invalid certificate type";

    case ERROR_IPSEC_IKE_NO_PRIVATE_KEY:
      return "No private key associated with machine certificate";

    case ERROR_IPSEC_IKE_DH_FAIL:
      return "Failure in Diffie-Hellman computation";

    case ERROR_IPSEC_IKE_INVALID_HEADER:
      return "Invalid header";

    case ERROR_IPSEC_IKE_NO_POLICY:
      return "No policy configured";

    case ERROR_IPSEC_IKE_INVALID_SIGNATURE:
      return "Failed to verify signature";

    case ERROR_IPSEC_IKE_KERBEROS_ERROR:
      return "Failed to authenticate using kerberos";

    case ERROR_IPSEC_IKE_NO_PUBLIC_KEY:
      return "Peer's certificate did not have a public key";

    case ERROR_IPSEC_IKE_PROCESS_ERR:
      return "Error processing error payload";

    case ERROR_IPSEC_IKE_PROCESS_ERR_SA:
      return "Error processing SA payload";

    case ERROR_IPSEC_IKE_PROCESS_ERR_PROP:
      return "Error processing Proposal payload";

    case ERROR_IPSEC_IKE_PROCESS_ERR_TRANS:
      return "Error processing Transform payload";

    case ERROR_IPSEC_IKE_PROCESS_ERR_KE:
      return "Error processing KE payload";

    case ERROR_IPSEC_IKE_PROCESS_ERR_ID:
      return "Error processing ID payload";

    case ERROR_IPSEC_IKE_PROCESS_ERR_CERT:
      return "Error processing Cert payload";

    case ERROR_IPSEC_IKE_PROCESS_ERR_CERT_REQ:
      return "Error processing Certificate Request payload";

    case ERROR_IPSEC_IKE_PROCESS_ERR_HASH:
      return "Error processing Hash payload";

    case ERROR_IPSEC_IKE_PROCESS_ERR_SIG:
      return "Error processing Signature payload";

    case ERROR_IPSEC_IKE_PROCESS_ERR_NONCE:
      return "Error processing Nonce payload";

    case ERROR_IPSEC_IKE_PROCESS_ERR_NOTIFY:
      return "Error processing Notify payload";

    case ERROR_IPSEC_IKE_PROCESS_ERR_DELETE:
      return "Error processing Delete Payload";

    case ERROR_IPSEC_IKE_PROCESS_ERR_VENDOR:
      return "Error processing VendorId payload";

    case ERROR_IPSEC_IKE_INVALID_PAYLOAD:
      return "Invalid payload received";

    case ERROR_IPSEC_IKE_LOAD_SOFT_SA:
      return "Soft SA loaded";

    case ERROR_IPSEC_IKE_SOFT_SA_TORN_DOWN:
      return "Soft SA torn down";

    case ERROR_IPSEC_IKE_INVALID_COOKIE:
      return "Invalid cookie received";

    case ERROR_IPSEC_IKE_NO_PEER_CERT:
      return "Peer failed to send valid machine certificate";

    case ERROR_IPSEC_IKE_PEER_CRL_FAILED:
      return "Certification Revocation check of peer's certificate failed";

    case ERROR_IPSEC_IKE_POLICY_CHANGE:
      return "New policy invalidated SAs formed with old policy";

    case ERROR_IPSEC_IKE_NO_MM_POLICY:
      return "There is no available Main Mode IKE policy";

    case ERROR_IPSEC_IKE_NOTCBPRIV:
      return "Failed to enabled TCB privilege";

    case ERROR_IPSEC_IKE_SECLOADFAIL:
      return "Failed to load SECURITY.DLL";

    case ERROR_IPSEC_IKE_FAILSSPINIT:
      return "Failed to obtain security function table dispatch address from SSPI";

    case ERROR_IPSEC_IKE_FAILQUERYSSP:
      return "Failed to query Kerberos package to obtain max token size";

    case ERROR_IPSEC_IKE_SRVACQFAIL:
      return "Failed to obtain Kerberos server credentials for ISAKMP/ERROR_IPSEC_IKE service.  Kerberos authentication will not function.  The most likely reason for this is lack of domain membership.  This is normal if your computer is a member of a workgroup";

    case ERROR_IPSEC_IKE_SRVQUERYCRED:
      return "Failed to determine SSPI principal name for ISAKMP/ERROR_IPSEC_IKE service (QueryCredentialsAttributes)";

    case ERROR_IPSEC_IKE_GETSPIFAIL:
      return "Failed to obtain new SPI for the inbound SA from Ipsec driver.  The most common cause for this is that the driver does not have the correct filter.  Check your policy to verify the filters";

    case ERROR_IPSEC_IKE_INVALID_FILTER:
      return "Given filter is invalid";

    case ERROR_IPSEC_IKE_OUT_OF_MEMORY:
      return "Memory allocation failed";

    case ERROR_IPSEC_IKE_ADD_UPDATE_KEY_FAILED:
      return "Failed to add Security Association to IPSec Driver.  The most common cause for this is if the IKE negotiation took too long to complete.  If the problem persists, reduce the load on the faulting machine";

    case ERROR_IPSEC_IKE_INVALID_POLICY:
      return "Invalid policy";

    case ERROR_IPSEC_IKE_UNKNOWN_DOI:
      return "Invalid DOI";

    case ERROR_IPSEC_IKE_INVALID_SITUATION:
      return "Invalid situation";

    case ERROR_IPSEC_IKE_DH_FAILURE:
      return "Diffie-Hellman failure";

    case ERROR_IPSEC_IKE_INVALID_GROUP:
      return "Invalid Diffie-Hellman group";

    case ERROR_IPSEC_IKE_ENCRYPT:
      return "Error encrypting payload";

    case ERROR_IPSEC_IKE_DECRYPT:
      return "Error decrypting payload";

    case ERROR_IPSEC_IKE_POLICY_MATCH:
      return "Policy match error";

    case ERROR_IPSEC_IKE_UNSUPPORTED_ID:
      return "Unsupported ID";

    case ERROR_IPSEC_IKE_INVALID_HASH:
      return "Hash verification failed";

    case ERROR_IPSEC_IKE_INVALID_HASH_ALG:
      return "Invalid hash algorithm";

    case ERROR_IPSEC_IKE_INVALID_HASH_SIZE:
      return "Invalid hash size";

    case ERROR_IPSEC_IKE_INVALID_ENCRYPT_ALG:
      return "Invalid encryption algorithm";

    case ERROR_IPSEC_IKE_INVALID_AUTH_ALG:
      return "Invalid authentication algorithm";

    case ERROR_IPSEC_IKE_INVALID_SIG:
      return "Invalid certificate signature";

    case ERROR_IPSEC_IKE_LOAD_FAILED:
      return "Load failed";

    case ERROR_IPSEC_IKE_RPC_DELETE:
      return "Deleted via RPC call";

    case ERROR_IPSEC_IKE_BENIGN_REINIT:
      return "Temporary state created to perform reinit. This is not a real failure";

    case ERROR_IPSEC_IKE_INVALID_RESPONDER_LIFETIME_NOTIFY:
      return "The lifetime value received in the Responder Lifetime Notify is below the Windows 2000 configured minimum value.  Please fix the policy on the peer machine";

    case ERROR_IPSEC_IKE_INVALID_CERT_KEYLEN:
      return "Key length in certificate is too small for configured security requirements";

    case ERROR_IPSEC_IKE_MM_LIMIT:
      return "Max number of established MM SAs to peer exceeded";

    case ERROR_IPSEC_IKE_NEGOTIATION_DISABLED:
      return "IKE received a policy that disables negotiation";

    case ERROR_IPSEC_IKE_NEG_STATUS_END:
      return "ERROR_IPSEC_IKE_NEG_STATUS_END";

    case E_UNEXPECTED:
      return "Catastrophic failure";

    case E_NOTIMPL:
      return "Not implemented";

    case E_OUTOFMEMORY:
      return "Ran out of memory";

    case E_INVALIDARG:
      return "One or more arguments are invalid";

    case E_NOINTERFACE:
      return "No such interface supported";

    case E_POINTER:
      return "Invalid pointer";

    case E_HANDLE:
      return "Invalid handle";

    case E_ABORT:
      return "Operation aborted";

    case E_FAIL:
      return "Unspecified error";

    case E_ACCESSDENIED:
      return "General access denied error";

    case E_NOTIMPL:
      return "Not implemented";

    case E_OUTOFMEMORY:
      return "Ran out of memory";

    case E_INVALIDARG:
      return "One or more arguments are invalid";

    case E_NOINTERFACE:
      return "No such interface supported";

    case E_POINTER:
      return "Invalid pointer";

    case E_HANDLE:
      return "Invalid handle";

    case E_ABORT:
      return "Operation aborted";

    case E_FAIL:
      return "Unspecified error";

    case E_ACCESSDENIED:
      return "General access denied error";

    case E_PENDING:
      return "The data necessary to complete this operation is not yet available";

    case CO_E_INIT_TLS:
      return "Thread local storage failure";

    case CO_E_INIT_SHARED_ALLOCATOR:
      return "Get shared memory allocator failure";

    case CO_E_INIT_MEMORY_ALLOCATOR:
      return "Get memory allocator failure";

    case CO_E_INIT_CLASS_CACHE:
      return "Unable to initialize class cache";

    case CO_E_INIT_RPC_CHANNEL:
      return "Unable to initialize RPC services";

    case CO_E_INIT_TLS_SET_CHANNEL_CONTROL:
      return "Cannot set thread local storage channel control";

    case CO_E_INIT_TLS_CHANNEL_CONTROL:
      return "Could not allocate thread local storage channel control";

    case CO_E_INIT_UNACCEPTED_USER_ALLOCATOR:
      return "The user supplied memory allocator is unacceptable";

    case CO_E_INIT_SCM_MUTEX_EXISTS:
      return "The OLE service mutex already exists";

    case CO_E_INIT_SCM_FILE_MAPPING_EXISTS:
      return "The OLE service file mapping already exists";

    case CO_E_INIT_SCM_MAP_VIEW_OF_FILE:
      return "Unable to map view of file for OLE service";

    case CO_E_INIT_SCM_EXEC_FAILURE:
      return "Failure attempting to launch OLE service";

    case CO_E_INIT_ONLY_SINGLE_THREADED:
      return "There was an attempt to call CoInitialize a second time while single threaded";

    case CO_E_CANT_REMOTE:
      return "A Remote activation was necessary but was not allowed";

    case CO_E_BAD_SERVER_NAME:
      return "A Remote activation was necessary but the server name provided was invalid";

    case CO_E_WRONG_SERVER_IDENTITY:
      return "The class is configured to run as a security id different from the caller";

    case CO_E_OLE1DDE_DISABLED:
      return "Use of Ole1 services requiring DDE windows is disabled";

    case CO_E_RUNAS_SYNTAX:
      return "A RunAs specification must be <domain name>\<user name> or simply <user name>";

    case CO_E_CREATEPROCESS_FAILURE:
      return "The server process could not be started.  The pathname may be incorrect";

    case CO_E_RUNAS_CREATEPROCESS_FAILURE:
      return "The server process could not be started as the configured identity.  The pathname may be incorrect or unavailable";

    case CO_E_RUNAS_LOGON_FAILURE:
      return "The server process could not be started because the configured identity is incorrect.  Check the username and password";

    case CO_E_LAUNCH_PERMSSION_DENIED:
      return "The client is not allowed to launch this server";

    case CO_E_START_SERVICE_FAILURE:
      return "The service providing this server could not be started";

    case CO_E_REMOTE_COMMUNICATION_FAILURE:
      return "This computer was unable to communicate with the computer providing the server";

    case CO_E_SERVER_START_TIMEOUT:
      return "The server did not respond after being launched";

    case CO_E_CLSREG_INCONSISTENT:
      return "The registration information for this server is inconsistent or incomplete";

    case CO_E_IIDREG_INCONSISTENT:
      return "The registration information for this interface is inconsistent or incomplete";

    case CO_E_NOT_SUPPORTED:
      return "The operation attempted is not supported";

    case CO_E_RELOAD_DLL:
      return "A dll must be loaded";

    case CO_E_MSI_ERROR:
      return "A Microsoft Software Installer error was encountered";

    case CO_E_ATTEMPT_TO_CREATE_OUTSIDE_CLIENT_CONTEXT:
      return "The specified activation could not occur in the client context as specified";

    case CO_E_SERVER_PAUSED:
      return "Activations on the server are paused";

    case CO_E_SERVER_NOT_PAUSED:
      return "Activations on the server are not paused";

    case CO_E_CLASS_DISABLED:
      return "The component or application containing the component has been disabled";

    case CO_E_CLRNOTAVAILABLE:
      return "The common language runtime is not available";

    case CO_E_ASYNC_WORK_REJECTED:
      return "The thread-pool rejected the submitted asynchronous work";

    case CO_E_SERVER_INIT_TIMEOUT:
      return "The server started, but did not finish initializing in a timely fashion";

    case CO_E_NO_SECCTX_IN_ACTIVATE:
      return "Unable to complete the call since there is no COM+ security context inside IObjectControl.Activate";

    case CO_E_TRACKER_CONFIG:
      return "The provided tracker configuration is invalid";

    case CO_E_THREADPOOL_CONFIG:
      return "The provided thread pool configuration is invalid";

    case CO_E_SXS_CONFIG:
      return "The provided side-by-side configuration is invalid";

    case CO_E_MALFORMED_SPN:
      return "The server principal name (SPN) obtained during security negotiation is malformed";

    case OLE_E_OLEVERB:
      return "Invalid OLEVERB structure";

    case OLE_E_ADVF:
      return "Invalid advise flags";

    case OLE_E_ENUM_NOMORE:
      return "Can't enumerate any more, because the associated data is missing";

    case OLE_E_ADVISENOTSUPPORTED:
      return "This implementation doesn't take advises";

    case OLE_E_NOCONNECTION:
      return "There is no connection for this connection ID";

    case OLE_E_NOTRUNNING:
      return "Need to run the object to perform this operation";

    case OLE_E_NOCACHE:
      return "There is no cache to operate on";

    case OLE_E_BLANK:
      return "Uninitialized object";

    case OLE_E_CLASSDIFF:
      return "Linked object's source class has changed";

    case OLE_E_CANT_GETMONIKER:
      return "Not able to get the moniker of the object";

    case OLE_E_CANT_BINDTOSOURCE:
      return "Not able to bind to the source";

    case OLE_E_STATIC:
      return "Object is static; operation not allowed";

    case OLE_E_PROMPTSAVECANCELLED:
      return "User canceled out of save dialog";

    case OLE_E_INVALIDRECT:
      return "Invalid rectangle";

    case OLE_E_WRONGCOMPOBJ:
      return "compobj.dll is too old for the ole2.dll initialized";

    case OLE_E_INVALIDHWND:
      return "Invalid window handle";

    case OLE_E_NOT_INPLACEACTIVE:
      return "Object is not in any of the inplace active states";

    case OLE_E_CANTCONVERT:
      return "Not able to convert object";

    case OLE_E_NOSTORAGE:
      return "Not able to perform the operation because object is not given storage yet";

    case DV_E_FORMATETC:
      return "Invalid FORMATETC structure";

    case DV_E_DVTARGETDEVICE:
      return "Invalid DVTARGETDEVICE structure";

    case DV_E_STGMEDIUM:
      return "Invalid STDGMEDIUM structure";

    case DV_E_STATDATA:
      return "Invalid STATDATA structure";

    case DV_E_LINDEX:
      return "Invalid lindex";

    case DV_E_TYMED:
      return "Invalid tymed";

    case DV_E_CLIPFORMAT:
      return "Invalid clipboard format";

    case DV_E_DVASPECT:
      return "Invalid aspect(s)";

    case DV_E_DVTARGETDEVICE_SIZE:
      return "tdSize parameter of the DVTARGETDEVICE structure is invalid";

    case DV_E_NOIVIEWOBJECT:
      return "Object doesn't support IViewObject interface";

    case DRAGDROP_E_NOTREGISTERED:
      return "Trying to revoke a drop target that has not been registered";

    case DRAGDROP_E_ALREADYREGISTERED:
      return "This window has already been registered as a drop target";

    case DRAGDROP_E_INVALIDHWND:
      return "Invalid window handle";

    case CLASS_E_NOAGGREGATION:
      return "Class does not support aggregation (or class object is remote)";

    case CLASS_E_CLASSNOTAVAILABLE:
      return "ClassFactory cannot supply requested class";

    case CLASS_E_NOTLICENSED:
      return "Class is not licensed for use";

    case VIEW_E_DRAW:
      return "Error drawing view";

    case REGDB_E_READREGDB:
      return "Could not read key from registry";

    case REGDB_E_WRITEREGDB:
      return "Could not write key to registry";

    case REGDB_E_KEYMISSING:
      return "Could not find the key in the registry";

    case REGDB_E_INVALIDVALUE:
      return "Invalid value for registry";

    case REGDB_E_CLASSNOTREG:
      return "Class not registered";

    case REGDB_E_IIDNOTREG:
      return "Interface not registered";

    case REGDB_E_BADTHREADINGMODEL:
      return "Threading model entry is not valid";

    case CAT_E_CATIDNOEXIST:
      return "CATID does not exist";

    case CAT_E_NODESCRIPTION:
      return "Description not found";

    case CS_E_PACKAGE_NOTFOUND:
      return "No package in the software installation data in the Active Directory meets this criteria";

    case CS_E_NOT_DELETABLE:
      return "Deleting this will break the referential integrity of the software installation data in the Active Directory";

    case CS_E_CLASS_NOTFOUND:
      return "The CLSID was not found in the software installation data in the Active Directory";

    case CS_E_INVALID_VERSION:
      return "The software installation data in the Active Directory is corrupt";

    case CS_E_NO_CLASSSTORE:
      return "There is no software installation data in the Active Directory";

    case CS_E_OBJECT_NOTFOUND:
      return "There is no software installation data object in the Active Directory";

    case CS_E_OBJECT_ALREADY_EXISTS:
      return "The software installation data object in the Active Directory already exists";

    case CS_E_INVALID_PATH:
      return "The path to the software installation data in the Active Directory is not correct";

    case CS_E_NETWORK_ERROR:
      return "A network error interrupted the operation";

    case CS_E_ADMIN_LIMIT_EXCEEDED:
      return "The size of this object exceeds the maximum size set by the Administrator";

    case CS_E_SCHEMA_MISMATCH:
      return "The schema for the software installation data in the Active Directory does not match the required schema";

    case CS_E_INTERNAL_ERROR:
      return "An error occurred in the software installation data in the Active Directory";

    case CACHE_E_NOCACHE_UPDATED:
      return "Cache not updated";

    case OLEOBJ_E_NOVERBS:
      return "No verbs for OLE object";

    case OLEOBJ_E_INVALIDVERB:
      return "Invalid verb for OLE object";

    case INPLACE_E_NOTUNDOABLE:
      return "Undo is not available";

    case INPLACE_E_NOTOOLSPACE:
      return "Space for tools is not available";

    case CONVERT10_E_OLESTREAM_GET:
      return "OLESTREAM Get method failed";

    case CONVERT10_E_OLESTREAM_PUT:
      return "OLESTREAM Put method failed";

    case CONVERT10_E_OLESTREAM_FMT:
      return "Contents of the OLESTREAM not in correct format";

    case CONVERT10_E_OLESTREAM_BITMAP_TO_DIB:
      return "There was an error in a Windows GDI call while converting the bitmap to a DIB";

    case CONVERT10_E_STG_FMT:
      return "Contents of the IStorage not in correct format";

    case CONVERT10_E_STG_NO_STD_STREAM:
      return "Contents of IStorage is missing one of the standard streams";

    case CONVERT10_E_STG_DIB_TO_BITMAP:
      return "There was an error in a Windows GDI call while converting the DIB to a bitmap";

    case CLIPBRD_E_CANT_OPEN:
      return "OpenClipboard Failed";

    case CLIPBRD_E_CANT_EMPTY:
      return "EmptyClipboard Failed";

    case CLIPBRD_E_CANT_SET:
      return "SetClipboard Failed";

    case CLIPBRD_E_BAD_DATA:
      return "Data on clipboard is invalid";

    case CLIPBRD_E_CANT_CLOSE:
      return "CloseClipboard Failed";

    case MK_E_CONNECTMANUALLY:
      return "Moniker needs to be connected manually";

    case MK_E_EXCEEDEDDEADLINE:
      return "Operation exceeded deadline";

    case MK_E_NEEDGENERIC:
      return "Moniker needs to be generic";

    case MK_E_UNAVAILABLE:
      return "Operation unavailable";

    case MK_E_SYNTAX:
      return "Invalid syntax";

    case MK_E_NOOBJECT:
      return "No object for moniker";

    case MK_E_INVALIDEXTENSION:
      return "Bad extension for file";

    case MK_E_INTERMEDIATEINTERFACENOTSUPPORTED:
      return "Intermediate operation failed";

    case MK_E_NOTBINDABLE:
      return "Moniker is not bindable";

    case MK_E_NOTBOUND:
      return "Moniker is not bound";

    case MK_E_CANTOPENFILE:
      return "Moniker cannot open file";

    case MK_E_MUSTBOTHERUSER:
      return "User input required for operation to succeed";

    case MK_E_NOINVERSE:
      return "Moniker class has no inverse";

    case MK_E_NOSTORAGE:
      return "Moniker does not refer to storage";

    case MK_E_NOPREFIX:
      return "No common prefix";

    case MK_E_ENUMERATION_FAILED:
      return "Moniker could not be enumerated";

    case CO_E_NOTINITIALIZED:
      return "CoInitialize has not been called";

    case CO_E_ALREADYINITIALIZED:
      return "CoInitialize has already been called";

    case CO_E_CANTDETERMINECLASS:
      return "Class of object cannot be determined";

    case CO_E_CLASSSTRING:
      return "Invalid class string";

    case CO_E_IIDSTRING:
      return "Invalid interface string";

    case CO_E_APPNOTFOUND:
      return "Application not found";

    case CO_E_APPSINGLEUSE:
      return "Application cannot be run more than once";

    case CO_E_ERRORINAPP:
      return "Some error in application program";

    case CO_E_DLLNOTFOUND:
      return "DLL for class not found";

    case CO_E_ERRORINDLL:
      return "Error in the DLL";

    case CO_E_WRONGOSFORAPP:
      return "Wrong OS or OS version for application";

    case CO_E_OBJNOTREG:
      return "Object is not registered";

    case CO_E_OBJISREG:
      return "Object is already registered";

    case CO_E_OBJNOTCONNECTED:
      return "Object is not connected to server";

    case CO_E_APPDIDNTREG:
      return "Application was launched but it didn't a class factory";

    case CO_E_RELEASED:
      return "Object has been released";

    case EVENT_S_SOME_SUBSCRIBERS_FAILED:
      return "An event was able to invoke some but not all of the subscribers";

    case EVENT_E_ALL_SUBSCRIBERS_FAILED:
      return "An event was unable to invoke any of the subscribers";

    case EVENT_S_NOSUBSCRIBERS:
      return "An event was delivered but there were no subscribers";

    case EVENT_E_QUERYSYNTAX:
      return "A syntax error occurred trying to evaluate a query string";

    case EVENT_E_QUERYFIELD:
      return "An invalid field name was used in a query string";

    case EVENT_E_INTERNALEXCEPTION:
      return "An unexpected exception was raised";

    case EVENT_E_INTERNALERROR:
      return "An unexpected internal error was detected";

    case EVENT_E_INVALID_PER_USER_SID:
      return "The owner SID on a per-user subscription doesn't exist";

    case EVENT_E_USER_EXCEPTION:
      return "A user-supplied component or subscriber raised an exception";

    case EVENT_E_TOO_MANY_METHODS:
      return "An interface has too many methods to fire events from";

    case EVENT_E_MISSING_EVENTCLASS:
      return "A subscription cannot be stored unless its event class already exists";

    case EVENT_E_NOT_ALL_REMOVED:
      return "Not all the objects requested could be removed";

    case EVENT_E_COMPLUS_NOT_INSTALLED:
      return "COM+ is required for this operation, but is not installed";

    case EVENT_E_CANT_MODIFY_OR_DELETE_UNCONFIGURED_OBJECT:
      return "Cannot modify or delete an object that was not added using the COM+ Admin SDK";

    case EVENT_E_CANT_MODIFY_OR_DELETE_CONFIGURED_OBJECT:
      return "Cannot modify or delete an object that was added using the COM+ Admin SDK";

    case EVENT_E_INVALID_EVENT_CLASS_PARTITION:
      return "The event class for this subscription is in an invalid partition";

    case EVENT_E_PER_USER_SID_NOT_LOGGED_ON:
      return "The owner of the PerUser subscription is not logged on to the system specified";

    case XACT_E_ALREADYOTHERSINGLEPHASE:
      return "Another single phase resource manager has already been enlisted in this transaction";

    case XACT_E_CANTRETAIN:
      return "A retaining commit or abort is not supported";

    case XACT_E_COMMITFAILED:
      return "The transaction failed to commit for an unknown reason. The transaction was aborted";

    case XACT_E_COMMITPREVENTED:
      return "Cannot call commit on this transaction object because the calling application did not initiate the transaction";

    case XACT_E_HEURISTICABORT:
      return "Instead of committing, the resource heuristically aborted";

    case XACT_E_HEURISTICCOMMIT:
      return "Instead of aborting, the resource heuristically committed";

    case XACT_E_HEURISTICDAMAGE:
      return "Some of the states of the resource were committed while others were aborted, likely because of heuristic decisions";

    case XACT_E_HEURISTICDANGER:
      return "Some of the states of the resource may have been committed while others may have been aborted, likely because of heuristic decisions";

    case XACT_E_ISOLATIONLEVEL:
      return "The requested isolation level is not valid or supported";

    case XACT_E_NOASYNC:
      return "The transaction manager doesn't support an asynchronous operation for this method";

    case XACT_E_NOENLIST:
      return "Unable to enlist in the transaction";

    case XACT_E_NOISORETAIN:
      return "The requested semantics of retention of isolation across retaining commit and abort boundaries cannot be supported by this transaction implementation, or isoFlags was not equal to zero";

    case XACT_E_NORESOURCE:
      return "There is no resource presently associated with this enlistment";

    case XACT_E_NOTCURRENT:
      return "The transaction failed to commit due to the failure of optimistic concurrency control in at least one of the resource managers";

    case XACT_E_NOTRANSACTION:
      return "The transaction has already been implicitly or explicitly committed or aborted";

    case XACT_E_NOTSUPPORTED:
      return "An invalid combination of flags was specified";

    case XACT_E_UNKNOWNRMGRID:
      return "The resource manager id is not associated with this transaction or the transaction manager";

    case XACT_E_WRONGSTATE:
      return "This method was called in the wrong state";

    case XACT_E_WRONGUOW:
      return "The indicated unit of work does not match the unit of work expected by the resource manager";

    case XACT_E_XTIONEXISTS:
      return "An enlistment in a transaction already exists";

    case XACT_E_NOIMPORTOBJECT:
      return "An import object for the transaction could not be found";

    case XACT_E_INVALIDCOOKIE:
      return "The transaction cookie is invalid";

    case XACT_E_INDOUBT:
      return "The transaction status is in doubt. A communication failure occurred, or a transaction manager or resource manager has failed";

    case XACT_E_NOTIMEOUT:
      return "A time-out was specified, but time-outs are not supported";

    case XACT_E_ALREADYINPROGRESS:
      return "The requested operation is already in progress for the transaction";

    case XACT_E_ABORTED:
      return "The transaction has already been aborted";

    case XACT_E_LOGFULL:
      return "The Transaction Manager returned a log full error";

    case XACT_E_TMNOTAVAILABLE:
      return "The Transaction Manager is not available";

    case XACT_E_CONNECTION_DOWN:
      return "A connection with the transaction manager was lost";

    case XACT_E_CONNECTION_DENIED:
      return "A request to establish a connection with the transaction manager was denied";

    case XACT_E_REENLISTTIMEOUT:
      return "Resource manager reenlistment to determine transaction status timed out";

    case XACT_E_TIP_CONNECT_FAILED:
      return "This transaction manager failed to establish a connection with another TIP transaction manager";

    case XACT_E_TIP_PROTOCOL_ERROR:
      return "This transaction manager encountered a protocol error with another TIP transaction manager";

    case XACT_E_TIP_PULL_FAILED:
      return "This transaction manager could not propagate a transaction from another TIP transaction manager";

    case XACT_E_DEST_TMNOTAVAILABLE:
      return "The Transaction Manager on the destination machine is not available";

    case XACT_E_TIP_DISABLED:
      return "The Transaction Manager has disabled its support for TIP";

    case XACT_E_NETWORK_TX_DISABLED:
      return "The transaction manager has disabled its support for remote/network transactions";

    case XACT_E_PARTNER_NETWORK_TX_DISABLED:
      return "The partner transaction manager has disabled its support for remote/network transactions";

    case XACT_E_XA_TX_DISABLED:
      return "The transaction manager has disabled its support for XA transactions";

    case XACT_E_UNABLE_TO_READ_DTC_CONFIG:
      return "MSDTC was unable to read its configuration information";

    case XACT_E_UNABLE_TO_LOAD_DTC_PROXY:
      return "MSDTC was unable to load the dtc proxy dll";

    case XACT_E_ABORTING:
      return "The local transaction has aborted";

    case XACT_E_CLERKNOTFOUND:
      return "XACT_E_CLERKNOTFOUND";

    case XACT_E_CLERKEXISTS:
      return "XACT_E_CLERKEXISTS";

    case XACT_E_RECOVERYINPROGRESS:
      return "XACT_E_RECOVERYINPROGRESS";

    case XACT_E_TRANSACTIONCLOSED:
      return "XACT_E_TRANSACTIONCLOSED";

    case XACT_E_INVALIDLSN:
      return "XACT_E_INVALIDLSN";

    case XACT_E_REPLAYREQUEST:
      return "XACT_E_REPLAYREQUEST";

    case XACT_S_ASYNC:
      return "An asynchronous operation was specified. The operation has begun, but its outcome is not known yet";

    case XACT_S_DEFECT:
      return "XACT_S_DEFECT";

    case XACT_S_READONLY:
      return "The method call succeeded because the transaction was read-only";

    case XACT_S_SOMENORETAIN:
      return "The transaction was successfully aborted. However, this is a coordinated transaction, and some number of enlisted resources were aborted outright because they could not support abort-retaining semantics";

    case XACT_S_OKINFORM:
      return "No changes were made during this call, but the sink wants another chance to look if any other sinks make further changes";

    case XACT_S_MADECHANGESCONTENT:
      return "The sink is content and wishes the transaction to proceed. Changes were made to one or more resources during this call";

    case XACT_S_MADECHANGESINFORM:
      return "The sink is for the moment and wishes the transaction to proceed, but if other changes are made following this return by other event sinks then this sink wants another chance to look";

    case XACT_S_ALLNORETAIN:
      return "The transaction was successfully aborted. However, the abort was non-retaining";

    case XACT_S_ABORTING:
      return "An abort operation was already in progress";

    case XACT_S_SINGLEPHASE:
      return "The resource manager has performed a single-phase commit of the transaction";

    case XACT_S_LOCALLY_OK:
      return "The local transaction has not aborted";

    case XACT_S_LASTRESOURCEMANAGER:
      return "The resource manager has requested to be the coordinator (last resource manager) for the transaction";

    case CONTEXT_E_ABORTED:
      return "The root transaction wanted to commit, but transaction aborted";

    case CONTEXT_E_ABORTING:
      return "You made a method call on a COM+ component that has a transaction that has already aborted or in the process of aborting";

    case CONTEXT_E_NOCONTEXT:
      return "There is no MTS object context";

    case CONTEXT_E_WOULD_DEADLOCK:
      return "The component is configured to use synchronization and this method call would cause a deadlock to occur";

    case CONTEXT_E_SYNCH_TIMEOUT:
      return "The component is configured to use synchronization and a thread has timed out waiting to enter the context";

    case CONTEXT_E_OLDREF:
      return "You made a method call on a COM+ component that has a transaction that has already committed or aborted";

    case CONTEXT_E_ROLENOTFOUND:
      return "The specified role was not configured for the application";

    case CONTEXT_E_TMNOTAVAILABLE:
      return "COM+ was unable to talk to the Microsoft Distributed Transaction Coordinator";

    case CO_E_ACTIVATIONFAILED:
      return "An unexpected error occurred during COM+ Activation";

    case CO_E_ACTIVATIONFAILED_EVENTLOGGED:
      return "COM+ Activation failed. Check the event log for more information";

    case CO_E_ACTIVATIONFAILED_CATALOGERROR:
      return "COM+ Activation failed due to a catalog or configuration error";

    case CO_E_ACTIVATIONFAILED_TIMEOUT:
      return "COM+ activation failed because the activation could not be completed in the specified amount of time";

    case CO_E_INITIALIZATIONFAILED:
      return "COM+ Activation failed because an initialization function failed.  Check the event log for more information";

    case CONTEXT_E_NOJIT:
      return "The requested operation requires that JIT be in the current context and it is not";

    case CONTEXT_E_NOTRANSACTION:
      return "The requested operation requires that the current context have a Transaction, and it does not";

    case CO_E_THREADINGMODEL_CHANGED:
      return "The components threading model has changed after install into a COM+ Application.  Please re-install component";

    case CO_E_NOIISINTRINSICS:
      return "IIS intrinsics not available.  Start your work with IIS";

    case CO_E_NOCOOKIES:
      return "An attempt to write a cookie failed";

    case CO_E_DBERROR:
      return "An attempt to use a database generated a database specific error";

    case CO_E_NOTPOOLED:
      return "The COM+ component you created must use object pooling to work";

    case CO_E_NOTCONSTRUCTED:
      return "The COM+ component you created must use object construction to work correctly";

    case CO_E_NOSYNCHRONIZATION:
      return "The COM+ component requires synchronization, and it is not configured for it";

    case CO_E_ISOLEVELMISMATCH:
      return "The TxIsolation Level property for the COM+ component being created is stronger than the TxIsolationLevel for the \"root\" component for the transaction.  The creation failed";

    case OLE_S_USEREG:
      return "Use the registry database to provide the requested information";

    case OLE_S_STATIC:
      return "Success, but static";

    case OLE_S_MAC_CLIPFORMAT:
      return "Macintosh clipboard format";

    case DRAGDROP_S_DROP:
      return "Successful drop took place";

    case DRAGDROP_S_CANCEL:
      return "Drag-drop operation canceled";

    case DRAGDROP_S_USEDEFAULTCURSORS:
      return "Use the default cursor";

    case DATA_S_SAMEFORMATETC:
      return "Data has same FORMATETC";

    case VIEW_S_ALREADY_FROZEN:
      return "View is already frozen";

    case CACHE_S_FORMATETC_NOTSUPPORTED:
      return "FORMATETC not supported";

    case CACHE_S_SAMECACHE:
      return "Same cache";

    case CACHE_S_SOMECACHES_NOTUPDATED:
      return "Some cache(s) not updated";

    case OLEOBJ_S_INVALIDVERB:
      return "Invalid verb for OLE object";

    case OLEOBJ_S_CANNOT_DOVERB_NOW:
      return "Verb number is valid but verb cannot be done now";

    case OLEOBJ_S_INVALIDHWND:
      return "Invalid window handle passed";

    case INPLACE_S_TRUNCATED:
      return "Message is too long; some of it had to be truncated before displaying";

    case CONVERT10_S_NO_PRESENTATION:
      return "Unable to convert OLESTREAM to IStorage";

    case MK_S_REDUCED_TO_SELF:
      return "Moniker reduced to itself";

    case MK_S_ME:
      return "Common prefix is this moniker";

    case MK_S_HIM:
      return "Common prefix is input moniker";

    case MK_S_US:
      return "Common prefix is both monikers";

    case MK_S_MONIKERALREADYREGISTERED:
      return "Moniker is already registered in running object table";

    case SCHED_S_TASK_READY:
      return "The task is ready to run at its next scheduled time";

    case SCHED_S_TASK_RUNNING:
      return "The task is currently running";

    case SCHED_S_TASK_DISABLED:
      return "The task will not run at the scheduled times because it has been disabled";

    case SCHED_S_TASK_HAS_NOT_RUN:
      return "The task has not yet run";

    case SCHED_S_TASK_NO_MORE_RUNS:
      return "There are no more runs scheduled for this task";

    case SCHED_S_TASK_NOT_SCHEDULED:
      return "One or more of the properties that are needed to run this task on a schedule have not been set";

    case SCHED_S_TASK_TERMINATED:
      return "The last run of the task was terminated by the user";

    case SCHED_S_TASK_NO_VALID_TRIGGERS:
      return "Either the task has no triggers or the existing triggers are disabled or not set";

    case SCHED_S_EVENT_TRIGGER:
      return "Event triggers don't have set run times";

    case SCHED_E_TRIGGER_NOT_FOUND:
      return "Trigger not found";

    case SCHED_E_TASK_NOT_READY:
      return "One or more of the properties that are needed to run this task have not been set";

    case SCHED_E_TASK_NOT_RUNNING:
      return "There is no running instance of the task to terminate";

    case SCHED_E_SERVICE_NOT_INSTALLED:
      return "The Task Scheduler Service is not installed on this computer";

    case SCHED_E_CANNOT_OPEN_TASK:
      return "The task object could not be opened";

    case SCHED_E_INVALID_TASK:
      return "The object is either an invalid task object or is not a task object";

    case SCHED_E_ACCOUNT_INFORMATION_NOT_SET:
      return "No account information could be found in the Task Scheduler security database for the task indicated";

    case SCHED_E_ACCOUNT_NAME_NOT_FOUND:
      return "Unable to establish existence of the account specified";

    case SCHED_E_ACCOUNT_DBASE_CORRUPT:
      return "Corruption was detected in the Task Scheduler security database; the database has been reset";

    case SCHED_E_NO_SECURITY_SERVICES:
      return "Task Scheduler security services are available only on Windows NT";

    case SCHED_E_UNKNOWN_OBJECT_VERSION:
      return "The task object version is either unsupported or invalid";

    case SCHED_E_UNSUPPORTED_ACCOUNT_OPTION:
      return "The task has been configured with an unsupported combination of account settings and run time options";

    case SCHED_E_SERVICE_NOT_RUNNING:
      return "The Task Scheduler Service is not running";

    case CO_E_CLASS_CREATE_FAILED:
      return "Attempt to create a class object failed";

    case CO_E_SCM_ERROR:
      return "OLE service could not bind object";

    case CO_E_SCM_RPC_FAILURE:
      return "RPC communication failed with OLE service";

    case CO_E_BAD_PATH:
      return "Bad path to object";

    case CO_E_SERVER_EXEC_FAILURE:
      return "Server execution failed";

    case CO_E_OBJSRV_RPC_FAILURE:
      return "OLE service could not communicate with the object server";

    case MK_E_NO_NORMALIZED:
      return "Moniker path could not be normalized";

    case CO_E_SERVER_STOPPING:
      return "Object server is stopping when OLE service contacts it";

    case MEM_E_INVALID_ROOT:
      return "An invalid root block pointer was specified";

    case MEM_E_INVALID_LINK:
      return "An allocation chain contained an invalid link pointer";

    case MEM_E_INVALID_SIZE:
      return "The requested allocation size was too large";

    case CO_S_NOTALLINTERFACES:
      return "Not all the requested interfaces were available";

    case CO_S_MACHINENAMENOTFOUND:
      return "The specified machine name was not found in the cache";

    case DISP_E_UNKNOWNINTERFACE:
      return "Unknown interface";

    case DISP_E_MEMBERNOTFOUND:
      return "Member not found";

    case DISP_E_PARAMNOTFOUND:
      return "Parameter not found";

    case DISP_E_TYPEMISMATCH:
      return "Type mismatch";

    case DISP_E_UNKNOWNNAME:
      return "Unknown name";

    case DISP_E_NONAMEDARGS:
      return "No named arguments";

    case DISP_E_BADVARTYPE:
      return "Bad variable type";

    case DISP_E_EXCEPTION:
      return "Exception occurred";

    case DISP_E_OVERFLOW:
      return "Out of present range";

    case DISP_E_BADINDEX:
      return "Invalid index";

    case DISP_E_UNKNOWNLCID:
      return "Unknown language";

    case DISP_E_ARRAYISLOCKED:
      return "Memory is locked";

    case DISP_E_BADPARAMCOUNT:
      return "Invalid number of parameters";

    case DISP_E_PARAMNOTOPTIONAL:
      return "Parameter not optional";

    case DISP_E_BADCALLEE:
      return "Invalid callee";

    case DISP_E_NOTACOLLECTION:
      return "Does not support a collection";

    case DISP_E_DIVBYZERO:
      return "Division by zero";

    case DISP_E_BUFFERTOOSMALL:
      return "Buffer too small";

    case TYPE_E_BUFFERTOOSMALL:
      return "Buffer too small";

    case TYPE_E_FIELDNOTFOUND:
      return "Field name not defined in the record";

    case TYPE_E_INVDATAREAD:
      return "Old format or invalid type library";

    case TYPE_E_UNSUPFORMAT:
      return "Old format or invalid type library";

    case TYPE_E_REGISTRYACCESS:
      return "Error accessing the OLE registry";

    case TYPE_E_LIBNOTREGISTERED:
      return "Library not registered";

    case TYPE_E_UNDEFINEDTYPE:
      return "Bound to unknown type";

    case TYPE_E_QUALIFIEDNAMEDISALLOWED:
      return "Qualified name disallowed";

    case TYPE_E_INVALIDSTATE:
      return "Invalid forward reference, or reference to uncompiled type";

    case TYPE_E_WRONGTYPEKIND:
      return "Type mismatch";

    case TYPE_E_ELEMENTNOTFOUND:
      return "Element not found";

    case TYPE_E_AMBIGUOUSNAME:
      return "Ambiguous name";

    case TYPE_E_NAMECONFLICT:
      return "Name already exists in the library";

    case TYPE_E_UNKNOWNLCID:
      return "Unknown LCID";

    case TYPE_E_DLLFUNCTIONNOTFOUND:
      return "Function not defined in specified DLL";

    case TYPE_E_BADMODULEKIND:
      return "Wrong module kind for the operation";

    case TYPE_E_SIZETOOBIG:
      return "Size may not exceed 64K";

    case TYPE_E_DUPLICATEID:
      return "Duplicate ID in inheritance hierarchy";

    case TYPE_E_INVALIDID:
      return "Incorrect inheritance depth in standard OLE hmember";

    case TYPE_E_TYPEMISMATCH:
      return "Type mismatch";

    case TYPE_E_OUTOFBOUNDS:
      return "Invalid number of arguments";

    case TYPE_E_IOERROR:
      return "I/O Error";

    case TYPE_E_CANTCREATETMPFILE:
      return "Error creating unique tmp file";

    case TYPE_E_CANTLOADLIBRARY:
      return "Error loading type library/DLL";

    case TYPE_E_INCONSISTENTPROPFUNCS:
      return "Inconsistent property functions";

    case TYPE_E_CIRCULARTYPE:
      return "Circular dependency between types/modules";

    case STG_E_INVALIDFUNCTION:
      return "Unable to perform requested operation";

    case STG_E_FILENOTFOUND:
      return "%1 could not be found";

    case STG_E_PATHNOTFOUND:
      return "The path %1 could not be found";

    case STG_E_TOOMANYOPENFILES:
      return "There are insufficient resources to open another file";

    case STG_E_ACCESSDENIED:
      return "Access Denied";

    case STG_E_INVALIDHANDLE:
      return "Attempted an operation on an invalid object";

    case STG_E_INSUFFICIENTMEMORY:
      return "There is insufficient memory available to complete operation";

    case STG_E_INVALIDPOINTER:
      return "Invalid pointer error";

    case STG_E_NOMOREFILES:
      return "There are no more entries to return";

    case STG_E_DISKISWRITEPROTECTED:
      return "Disk is write-protected";

    case STG_E_SEEKERROR:
      return "An error occurred during a seek operation";

    case STG_E_WRITEFAULT:
      return "A disk error occurred during a write operation";

    case STG_E_READFAULT:
      return "A disk error occurred during a read operation";

    case STG_E_SHAREVIOLATION:
      return "A share violation has occurred";

    case STG_E_LOCKVIOLATION:
      return "A lock violation has occurred";

    case STG_E_FILEALREADYEXISTS:
      return "%1 already exists";

    case STG_E_INVALIDPARAMETER:
      return "Invalid parameter error";

    case STG_E_MEDIUMFULL:
      return "There is insufficient disk space to complete operation";

    case STG_E_PROPSETMISMATCHED:
      return "Illegal write of non-simple property to simple property set";

    case STG_E_ABNORMALAPIEXIT:
      return "An API call exited abnormally";

    case STG_E_INVALIDHEADER:
      return "The file %1 is not a valid compound file";

    case STG_E_INVALIDNAME:
      return "The name %1 is not valid";

    case STG_E_UNKNOWN:
      return "An unexpected error occurred";

    case STG_E_UNIMPLEMENTEDFUNCTION:
      return "That function is not implemented";

    case STG_E_INVALIDFLAG:
      return "Invalid flag error";

    case STG_E_INUSE:
      return "Attempted to use an object that is busy";

    case STG_E_NOTCURRENT:
      return "The storage has been changed since the last commit";

    case STG_E_REVERTED:
      return "Attempted to use an object that has ceased to exist";

    case STG_E_CANTSAVE:
      return "Can't save";

    case STG_E_OLDFORMAT:
      return "The compound file %1 was produced with an incompatible version of storage";

    case STG_E_OLDDLL:
      return "The compound file %1 was produced with a newer version of storage";

    case STG_E_SHAREREQUIRED:
      return "Share.exe or equivalent is required for operation";

    case STG_E_NOTFILEBASEDSTORAGE:
      return "Illegal operation called on non-file based storage";

    case STG_E_EXTANTMARSHALLINGS:
      return "Illegal operation called on object with extant marshallings";

    case STG_E_DOCFILECORRUPT:
      return "The docfile has been corrupted";

    case STG_E_BADBASEADDRESS:
      return "OLE32.DLL has been loaded at the wrong address";

    case STG_E_DOCFILETOOLARGE:
      return "The compound file is too large for the current implementation";

    case STG_E_NOTSIMPLEFORMAT:
      return "The compound file was not created with the STGM_SIMPLE flag";

    case STG_E_INCOMPLETE:
      return "The file download was aborted abnormally.  The file is incomplete";

    case STG_E_TERMINATED:
      return "The file download has been terminated";

    case STG_S_CONVERTED:
      return "The underlying file was converted to compound file format";

    case STG_S_BLOCK:
      return "The storage operation should block until more data is available";

    case STG_S_RETRYNOW:
      return "The storage operation should retry immediately";

    case STG_S_MONITORING:
      return "The notified event sink will not influence the storage operation";

    case STG_S_MULTIPLEOPENS:
      return "Multiple opens prevent consolidated. (commit succeeded)";

    case STG_S_CONSOLIDATIONFAILED:
      return "Consolidation of the storage file failed. (commit succeeded)";

    case STG_S_CANNOTCONSOLIDATE:
      return "Consolidation of the storage file is inappropriate. (commit succeeded)";

    case STG_E_STATUS_COPY_PROTECTION_FAILURE:
      return "Generic Copy Protection Error";

    case STG_E_CSS_AUTHENTICATION_FAILURE:
      return "Copy Protection Error - DVD CSS Authentication failed";

    case STG_E_CSS_KEY_NOT_PRESENT:
      return "Copy Protection Error - The given sector does not have a valid CSS key";

    case STG_E_CSS_KEY_NOT_ESTABLISHED:
      return "Copy Protection Error - DVD session key not established";

    case STG_E_CSS_SCRAMBLED_SECTOR:
      return "Copy Protection Error - The read failed because the sector is encrypted";

    case STG_E_CSS_REGION_MISMATCH:
      return "Copy Protection Error - The current DVD's region does not correspond to the region setting of the drive";

    case STG_E_RESETS_EXHAUSTED:
      return "Copy Protection Error - The drive's region setting may be permanent or the number of user resets has been exhausted";

    case RPC_E_CALL_REJECTED:
      return "Call was rejected by callee";

    case RPC_E_CALL_CANCELED:
      return "Call was canceled by the message filter";

    case RPC_E_CANTPOST_INSENDCALL:
      return "The caller is dispatching an intertask SendMessage call and cannot call out via PostMessage";

    case RPC_E_CANTCALLOUT_INASYNCCALL:
      return "The caller is dispatching an asynchronous call and cannot make an outgoing call on behalf of this call";

    case RPC_E_CANTCALLOUT_INEXTERNALCALL:
      return "It is illegal to call out while inside message filter";

    case RPC_E_CONNECTION_TERMINATED:
      return "The connection terminated or is in a bogus state and cannot be used any more. Other connections are still valid";

    case RPC_E_SERVER_DIED:
      return "The callee (server [not server application]) is not available and disappeared; all connections are invalid. The call may have executed";

    case RPC_E_CLIENT_DIED:
      return "The caller (client) disappeared while the callee (server) was processing a call";

    case RPC_E_INVALID_DATAPACKET:
      return "The data packet with the marshalled parameter data is incorrect";

    case RPC_E_CANTTRANSMIT_CALL:
      return "The call was not transmitted properly; the message queue was full and was not emptied after yielding";

    case RPC_E_CLIENT_CANTMARSHAL_DATA:
      return "The client (caller) cannot marshall the parameter data - low memory, etc";

    case RPC_E_CLIENT_CANTUNMARSHAL_DATA:
      return "The client (caller) cannot unmarshall the return data - low memory, etc";

    case RPC_E_SERVER_CANTMARSHAL_DATA:
      return "The server (callee) cannot marshall the return data - low memory, etc";

    case RPC_E_SERVER_CANTUNMARSHAL_DATA:
      return "The server (callee) cannot unmarshall the parameter data - low memory, etc";

    case RPC_E_INVALID_DATA:
      return "Received data is invalid; could be server or client data";

    case RPC_E_INVALID_PARAMETER:
      return "A particular parameter is invalid and cannot be (un)marshalled";

    case RPC_E_CANTCALLOUT_AGAIN:
      return "There is no second outgoing call on same channel in DDE conversation";

    case RPC_E_SERVER_DIED_DNE:
      return "The callee (server [not server application]) is not available and disappeared; all connections are invalid. The call did not execute";

    case RPC_E_SYS_CALL_FAILED:
      return "System call failed";

    case RPC_E_OUT_OF_RESOURCES:
      return "Could not allocate some required resource (memory, events, ...)";

    case RPC_E_ATTEMPTED_MULTITHREAD:
      return "Attempted to make calls on more than one thread in single threaded mode";

    case RPC_E_NOT_REGISTERED:
      return "The requested interface is not registered on the server object";

    case RPC_E_FAULT:
      return "RPC could not call the server or could not return the results of calling the server";

    case RPC_E_SERVERFAULT:
      return "The server threw an exception";

    case RPC_E_CHANGED_MODE:
      return "Cannot change thread mode after it is set";

    case RPC_E_INVALIDMETHOD:
      return "The method called does not exist on the server";

    case RPC_E_DISCONNECTED:
      return "The object invoked has disconnected from its clients";

    case RPC_E_RETRY:
      return "The object invoked chose not to process the call now.  Try again later";

    case RPC_E_SERVERCALL_RETRYLATER:
      return "The message filter indicated that the application is busy";

    case RPC_E_SERVERCALL_REJECTED:
      return "The message filter rejected the call";

    case RPC_E_INVALID_CALLDATA:
      return "A call control interfaces was called with invalid data";

    case RPC_E_CANTCALLOUT_ININPUTSYNCCALL:
      return "An outgoing call cannot be made since the application is dispatching an input-synchronous call";

    case RPC_E_WRONG_THREAD:
      return "The application called an interface that was marshalled for a different thread";

    case RPC_E_THREAD_NOT_INIT:
      return "CoInitialize has not been called on the current thread";

    case RPC_E_VERSION_MISMATCH:
      return "The version of OLE on the client and server machines does not match";

    case RPC_E_INVALID_HEADER:
      return "OLE received a packet with an invalid header";

    case RPC_E_INVALID_EXTENSION:
      return "OLE received a packet with an invalid extension";

    case RPC_E_INVALID_IPID:
      return "The requested object or interface does not exist";

    case RPC_E_INVALID_OBJECT:
      return "The requested object does not exist";

    case RPC_S_CALLPENDING:
      return "OLE has sent a request and is waiting for a reply";

    case RPC_S_WAITONTIMER:
      return "OLE is waiting before retrying a request";

    case RPC_E_CALL_COMPLETE:
      return "Call context cannot be accessed after call completed";

    case RPC_E_UNSECURE_CALL:
      return "Impersonate on unsecure calls is not supported";

    case RPC_E_TOO_LATE:
      return "Security must be initialized before any interfaces are marshalled or unmarshalled. It cannot be changed once initialized";

    case RPC_E_NO_GOOD_SECURITY_PACKAGES:
      return "No security packages are installed on this machine or the user is not logged on or there are no compatible security packages between the client and server";

    case RPC_E_ACCESS_DENIED:
      return "Access is denied";

    case RPC_E_REMOTE_DISABLED:
      return "Remote calls are not allowed for this process";

    case RPC_E_INVALID_OBJREF:
      return "The marshaled interface data packet (OBJREF) has an invalid or unknown format";

    case RPC_E_NO_CONTEXT:
      return "No context is associated with this call. This happens for some custom marshalled calls and on the client side of the call";

    case RPC_E_TIMEOUT:
      return "This operation returned because the timeout period expired";

    case RPC_E_NO_SYNC:
      return "There are no synchronize objects to wait on";

    case RPC_E_FULLSIC_REQUIRED:
      return "Full subject issuer chain SSL principal name expected from the server";

    case RPC_E_INVALID_STD_NAME:
      return "Principal name is not a valid MSSTD name";

    case CO_E_FAILEDTOIMPERSONATE:
      return "Unable to impersonate DCOM client";

    case CO_E_FAILEDTOGETSECCTX:
      return "Unable to obtain server's security context";

    case CO_E_FAILEDTOOPENTHREADTOKEN:
      return "Unable to open the access token of the current thread";

    case CO_E_FAILEDTOGETTOKENINFO:
      return "Unable to obtain user info from an access token";

    case CO_E_TRUSTEEDOESNTMATCHCLIENT:
      return "The client who called IAccessControl::IsAccessPermitted was not the trustee provided to the method";

    case CO_E_FAILEDTOQUERYCLIENTBLANKET:
      return "Unable to obtain the client's security blanket";

    case CO_E_FAILEDTOSETDACL:
      return "Unable to set a discretionary ACL into a security descriptor";

    case CO_E_ACCESSCHECKFAILED:
      return "The system function, AccessCheck, returned false";

    case CO_E_NETACCESSAPIFAILED:
      return "Either NetAccessDel or NetAccessAdd returned an error code";

    case CO_E_WRONGTRUSTEENAMESYNTAX:
      return "One of the trustee strings provided by the user did not conform to the <Domain>\<Name> syntax and it was not the \"*\" string";

    case CO_E_INVALIDSID:
      return "One of the security identifiers provided by the user was invalid";

    case CO_E_CONVERSIONFAILED:
      return "Unable to convert a wide character trustee string to a multibyte trustee string";

    case CO_E_NOMATCHINGSIDFOUND:
      return "Unable to find a security identifier that corresponds to a trustee string provided by the user";

    case CO_E_LOOKUPACCSIDFAILED:
      return "The system function, LookupAccountSID, failed";

    case CO_E_NOMATCHINGNAMEFOUND:
      return "Unable to find a trustee name that corresponds to a security identifier provided by the user";

    case CO_E_LOOKUPACCNAMEFAILED:
      return "The system function, LookupAccountName, failed";

    case CO_E_SETSERLHNDLFAILED:
      return "Unable to set or reset a serialization handle";

    case CO_E_FAILEDTOGETWINDIR:
      return "Unable to obtain the Windows directory";

    case CO_E_PATHTOOLONG:
      return "Path too long";

    case CO_E_FAILEDTOGENUUID:
      return "Unable to generate a uuid";

    case CO_E_FAILEDTOCREATEFILE:
      return "Unable to create file";

    case CO_E_FAILEDTOCLOSEHANDLE:
      return "Unable to close a serialization handle or a file handle";

    case CO_E_EXCEEDSYSACLLIMIT:
      return "The number of ACEs in an ACL exceeds the system limit";

    case CO_E_ACESINWRONGORDER:
      return "Not all the DENY_ACCESS ACEs are arranged in front of the GRANT_ACCESS ACEs in the stream";

    case CO_E_INCOMPATIBLESTREAMVERSION:
      return "The version of ACL format in the stream is not supported by this implementation of IAccessControl";

    case CO_E_FAILEDTOOPENPROCESSTOKEN:
      return "Unable to open the access token of the server process";

    case CO_E_DECODEFAILED:
      return "Unable to decode the ACL in the stream provided by the user";

    case CO_E_ACNOTINITIALIZED:
      return "The COM IAccessControl object is not initialized";

    case CO_E_CANCEL_DISABLED:
      return "Call Cancellation is disabled";

    case RPC_E_UNEXPECTED:
      return "An internal error occurred";

    case ERROR_AUDITING_DISABLED:
      return "The specified event is currently not being audited";

    case ERROR_ALL_SIDS_FILTERED:
      return "The SID filtering operation removed all SIDs";

    case NTE_BAD_UID:
      return "Bad UID";

    case NTE_BAD_HASH:
      return "Bad Hash";

    case NTE_BAD_KEY:
      return "Bad Key";

    case NTE_BAD_LEN:
      return "Bad Length";

    case NTE_BAD_DATA:
      return "Bad Data";

    case NTE_BAD_SIGNATURE:
      return "Invalid Signature";

    case NTE_BAD_VER:
      return "Bad Version of provider";

    case NTE_BAD_ALGID:
      return "Invalid algorithm specified";

    case NTE_BAD_FLAGS:
      return "Invalid flags specified";

    case NTE_BAD_TYPE:
      return "Invalid type specified";

    case NTE_BAD_KEY_STATE:
      return "Key not valid for use in specified state";

    case NTE_BAD_HASH_STATE:
      return "Hash not valid for use in specified state";

    case NTE_NO_KEY:
      return "Key does not exist";

    case NTE_NO_MEMORY:
      return "Insufficient memory available for the operation";

    case NTE_EXISTS:
      return "Object already exists";

    case NTE_PERM:
      return "Access denied";

    case NTE_NOT_FOUND:
      return "Object was not found";

    case NTE_DOUBLE_ENCRYPT:
      return "Data already encrypted";

    case NTE_BAD_PROVIDER:
      return "Invalid provider specified";

    case NTE_BAD_PROV_TYPE:
      return "Invalid provider type specified";

    case NTE_BAD_PUBLIC_KEY:
      return "Provider's public key is invalid";

    case NTE_BAD_KEYSET:
      return "Keyset does not exist";

    case NTE_PROV_TYPE_NOT_DEF:
      return "Provider type not defined";

    case NTE_PROV_TYPE_ENTRY_BAD:
      return "Provider type as registered is invalid";

    case NTE_KEYSET_NOT_DEF:
      return "The keyset is not defined";

    case NTE_KEYSET_ENTRY_BAD:
      return "Keyset as registered is invalid";

    case NTE_PROV_TYPE_NO_MATCH:
      return "Provider type does not match registered value";

    case NTE_SIGNATURE_FILE_BAD:
      return "The digital signature file is corrupt";

    case NTE_PROVIDER_DLL_FAIL:
      return "Provider DLL failed to initialize correctly";

    case NTE_PROV_DLL_NOT_FOUND:
      return "Provider DLL could not be found";

    case NTE_BAD_KEYSET_PARAM:
      return "The Keyset parameter is invalid";

    case NTE_FAIL:
      return "An internal error occurred";

    case NTE_SYS_ERR:
      return "A base error occurred";

    case NTE_SILENT_CONTEXT:
      return "Provider could not perform the action since the context was acquired as silent";

    case NTE_TOKEN_KEYSET_STORAGE_FULL:
      return "The security token does not have storage space available for an additional container";

    case NTE_TEMPORARY_PROFILE:
      return "The profile for the user is a temporary profile";

    case NTE_FIXEDPARAMETER:
      return "The key parameters could not be set because the CSP uses fixed parameters";

    case SEC_E_INSUFFICIENT_MEMORY:
      return "Not enough memory is available to complete this request";

    case SEC_E_INVALID_HANDLE:
      return "The handle specified is invalid";

    case SEC_E_UNSUPPORTED_FUNCTION:
      return "The function requested is not supported";

    case SEC_E_TARGET_UNKNOWN:
      return "The specified target is unknown or unreachable";

    case SEC_E_INTERNAL_ERROR:
      return "The Local Security Authority cannot be contacted";

    case SEC_E_SECPKG_NOT_FOUND:
      return "The requested security package does not exist";

    case SEC_E_NOT_OWNER:
      return "The caller is not the owner of the desired credentials";

    case SEC_E_CANNOT_INSTALL:
      return "The security package failed to initialize, and cannot be installed";

    case SEC_E_INVALID_TOKEN:
      return "The token supplied to the function is invalid";

    case SEC_E_CANNOT_PACK:
      return "The security package is not able to marshall the logon buffer, so the logon attempt has failed";

    case SEC_E_QOP_NOT_SUPPORTED:
      return "The per-message Quality of Protection is not supported by the security package";

    case SEC_E_NO_IMPERSONATION:
      return "The security context does not allow impersonation of the client";

    case SEC_E_LOGON_DENIED:
      return "The logon attempt failed";

    case SEC_E_UNKNOWN_CREDENTIALS:
      return "The credentials supplied to the package were not recognized";

    case SEC_E_NO_CREDENTIALS:
      return "No credentials are available in the security package";

    case SEC_E_MESSAGE_ALTERED:
      return "The message or signature supplied for verification has been altered";

    case SEC_E_OUT_OF_SEQUENCE:
      return "The message supplied for verification is out of sequence";

    case SEC_E_NO_AUTHENTICATING_AUTHORITY:
      return "No authority could be contacted for authentication";

    case SEC_I_CONTINUE_NEEDED:
      return "The function completed successfully, but must be called again to complete the context";

    case SEC_I_COMPLETE_NEEDED:
      return "The function completed successfully, but CompleteToken must be called";

    case SEC_I_COMPLETE_AND_CONTINUE:
      return "The function completed successfully, but both CompleteToken and this function must be called to complete the context";

    case SEC_I_LOCAL_LOGON:
      return "The logon was completed, but no network authority was available. The logon was made using locally known information";

    case SEC_E_BAD_PKGID:
      return "The requested security package does not exist";

    case SEC_E_CONTEXT_EXPIRED:
      return "The context has expired and can no longer be used";

    case SEC_I_CONTEXT_EXPIRED:
      return "The context has expired and can no longer be used";

    case SEC_E_INCOMPLETE_MESSAGE:
      return "The supplied message is incomplete.  The signature was not verified";

    case SEC_E_INCOMPLETE_CREDENTIALS:
      return "The credentials supplied were not complete, and could not be verified. The context could not be initialized";

    case SEC_E_BUFFER_TOO_SMALL:
      return "The buffers supplied to a function was too small";

    case SEC_I_INCOMPLETE_CREDENTIALS:
      return "The credentials supplied were not complete, and could not be verified. Additional information can be returned from the context";

    case SEC_I_RENEGOTIATE:
      return "The context data must be renegotiated with the peer";

    case SEC_E_WRONG_PRINCIPAL:
      return "The target principal name is incorrect";

    case SEC_I_NO_LSA_CONTEXT:
      return "There is no LSA mode context associated with this context";

    case SEC_E_TIME_SKEW:
      return "The clocks on the client and server machines are skewed";

    case SEC_E_UNTRUSTED_ROOT:
      return "The certificate chain was issued by an authority that is not trusted";

    case SEC_E_ILLEGAL_MESSAGE:
      return "The message received was unexpected or badly formatted";

    case SEC_E_CERT_UNKNOWN:
      return "An unknown error occurred while processing the certificate";

    case SEC_E_CERT_EXPIRED:
      return "The received certificate has expired";

    case SEC_E_ENCRYPT_FAILURE:
      return "The specified data could not be encrypted";

    case SEC_E_DECRYPT_FAILURE:
      return "The specified data could not be decrypted";

    case SEC_E_ALGORITHM_MISMATCH:
      return "The client and server cannot communicate, because they do not possess a common algorithm";

    case SEC_E_SECURITY_QOS_FAILED:
      return "The security context could not be established due to a failure in the requested quality of service (e.g. mutual authentication or delegation)";

    case SEC_E_UNFINISHED_CONTEXT_DELETED:
      return "A security context was deleted before the context was completed.  This is considered a logon failure";

    case SEC_E_NO_TGT_REPLY:
      return "The client is trying to negotiate a context and the server requires user-to-user but didn't send a TGT reply";

    case SEC_E_NO_IP_ADDRESSES:
      return "Unable to accomplish the requested task because the local machine does not have any IP addresses";

    case SEC_E_WRONG_CREDENTIAL_HANDLE:
      return "The supplied credential handle does not match the credential associated with the security context";

    case SEC_E_CRYPTO_SYSTEM_INVALID:
      return "The crypto system or checksum function is invalid because a required function is unavailable";

    case SEC_E_MAX_REFERRALS_EXCEEDED:
      return "The number of maximum ticket referrals has been exceeded";

    case SEC_E_MUST_BE_KDC:
      return "The local machine must be a Kerberos KDC (domain controller) and it is not";

    case SEC_E_STRONG_CRYPTO_NOT_SUPPORTED:
      return "The other end of the security negotiation is requires strong crypto but it is not supported on the local machine";

    case SEC_E_TOO_MANY_PRINCIPALS:
      return "The KDC reply contained more than one principal name";

    case SEC_E_NO_PA_DATA:
      return "Expected to find PA data for a hint of what etype to use, but it was not found";

    case SEC_E_PKINIT_NAME_MISMATCH:
      return "The client certificate does not contain a valid UPN, or does not match the client name";

    case SEC_E_SMARTCARD_LOGON_REQUIRED:
      return "Smartcard logon is required and was not used";

    case SEC_E_SHUTDOWN_IN_PROGRESS:
      return "A system shutdown is in progress";

    case SEC_E_KDC_INVALID_REQUEST:
      return "An invalid request was sent to the KDC";

    case SEC_E_KDC_UNABLE_TO_REFER:
      return "The KDC was unable to generate a referral for the service requested";

    case SEC_E_KDC_UNKNOWN_ETYPE:
      return "The encryption type requested is not supported by the KDC";

    case SEC_E_UNSUPPORTED_PREAUTH:
      return "An unsupported preauthentication mechanism was presented to the kerberos package";

    case SEC_E_DELEGATION_REQUIRED:
      return "The requested operation cannot be completed.  The computer must be trusted for delegation and the current user account must be configured to allow delegation";

    case SEC_E_BAD_BINDINGS:
      return "Client's supplied SSPI channel bindings were incorrect";

    case SEC_E_MULTIPLE_ACCOUNTS:
      return "The received certificate was mapped to multiple accounts";

    case SEC_E_NO_KERB_KEY:
      return "SEC_E_NO_KERB_KEY";

    case SEC_E_CERT_WRONG_USAGE:
      return "The certificate is not valid for the requested usage";

    case SEC_E_DOWNGRADE_DETECTED:
      return "The system detected a possible attempt to compromise security.  Please ensure that you can contact the server that authenticated you";

    case SEC_E_SMARTCARD_CERT_REVOKED:
      return "The smartcard certificate used for authentication has been revoked";

    case SEC_E_ISSUING_CA_UNTRUSTED:
      return "An untrusted certificate authority was detected While processing the";

    case SEC_E_REVOCATION_OFFLINE_C:
      return "The revocation status of the smartcard certificate used for";

    case SEC_E_PKINIT_CLIENT_FAILURE:
      return "The smartcard certificate used for authentication was not trusted.  Please";

    case SEC_E_SMARTCARD_CERT_EXPIRED:
      return "The smartcard certificate used for authentication has expired.  Please";

    case SEC_E_NO_S4U_PROT_SUPPORT:
      return "The Kerberos subsystem encountered an error.  A service for user protocol request was made";

    case SEC_E_CROSSREALM_DELEGATION_FAILURE:
      return "An attempt was made by this server to make a Kerberos constrained delegation request for a target";

    case SEC_E_REVOCATION_OFFLINE_KDC:
      return "The revocation status of the domain controller certificate used for smartcard";

    case SEC_E_ISSUING_CA_UNTRUSTED_KDC:
      return "An untrusted certificate authority was detected while processing the";

    case SEC_E_KDC_CERT_EXPIRED:
      return "The domain controller certificate used for smartcard logon has expired";

    case SEC_E_KDC_CERT_REVOKED:
      return "The domain controller certificate used for smartcard logon has been revoked";

    case CRYPT_E_MSG_ERROR:
      return "An error occurred while performing an operation on a cryptographic message";

    case CRYPT_E_UNKNOWN_ALGO:
      return "Unknown cryptographic algorithm";

    case CRYPT_E_OID_FORMAT:
      return "The object identifier is poorly formatted";

    case CRYPT_E_INVALID_MSG_TYPE:
      return "Invalid cryptographic message type";

    case CRYPT_E_UNEXPECTED_ENCODING:
      return "Unexpected cryptographic message encoding";

    case CRYPT_E_AUTH_ATTR_MISSING:
      return "The cryptographic message does not contain an expected authenticated attribute";

    case CRYPT_E_HASH_VALUE:
      return "The hash value is not correct";

    case CRYPT_E_INVALID_INDEX:
      return "The index value is not valid";

    case CRYPT_E_ALREADY_DECRYPTED:
      return "The content of the cryptographic message has already been decrypted";

    case CRYPT_E_NOT_DECRYPTED:
      return "The content of the cryptographic message has not been decrypted yet";

    case CRYPT_E_RECIPIENT_NOT_FOUND:
      return "The enveloped-data message does not contain the specified recipient";

    case CRYPT_E_CONTROL_TYPE:
      return "Invalid control type";

    case CRYPT_E_ISSUER_SERIALNUMBER:
      return "Invalid issuer and/or serial number";

    case CRYPT_E_SIGNER_NOT_FOUND:
      return "Cannot find the original signer";

    case CRYPT_E_ATTRIBUTES_MISSING:
      return "The cryptographic message does not contain all of the requested attributes";

    case CRYPT_E_STREAM_MSG_NOT_READY:
      return "The streamed cryptographic message is not ready to return data";

    case CRYPT_E_STREAM_INSUFFICIENT_DATA:
      return "The streamed cryptographic message requires more data to complete the decode operation";

    case CRYPT_I_NEW_PROTECTION_REQUIRED:
      return "The protected data needs to be re-protected";

    case CRYPT_E_BAD_LEN:
      return "The length specified for the output data was insufficient";

    case CRYPT_E_BAD_ENCODE:
      return "An error occurred during encode or decode operation";

    case CRYPT_E_FILE_ERROR:
      return "An error occurred while reading or writing to a file";

    case CRYPT_E_NOT_FOUND:
      return "Cannot find object or property";

    case CRYPT_E_EXISTS:
      return "The object or property already exists";

    case CRYPT_E_NO_PROVIDER:
      return "No provider was specified for the store or object";

    case CRYPT_E_SELF_SIGNED:
      return "The specified certificate is self signed";

    case CRYPT_E_DELETED_PREV:
      return "The previous certificate or CRL context was deleted";

    case CRYPT_E_NO_MATCH:
      return "Cannot find the requested object";

    case CRYPT_E_UNEXPECTED_MSG_TYPE:
      return "The certificate does not have a property that references a private key";

    case CRYPT_E_NO_KEY_PROPERTY:
      return "Cannot find the certificate and private key for decryption";

    case CRYPT_E_NO_DECRYPT_CERT:
      return "Cannot find the certificate and private key to use for decryption";

    case CRYPT_E_BAD_MSG:
      return "Not a cryptographic message or the cryptographic message is not formatted correctly";

    case CRYPT_E_NO_SIGNER:
      return "The signed cryptographic message does not have a signer for the specified signer index";

    case CRYPT_E_PENDING_CLOSE:
      return "Final closure is pending until additional frees or closes";

    case CRYPT_E_REVOKED:
      return "The certificate is revoked";

    case CRYPT_E_NO_REVOCATION_DLL:
      return "No Dll or exported function was found to verify revocation";

    case CRYPT_E_NO_REVOCATION_CHECK:
      return "The revocation function was unable to check revocation for the certificate";

    case CRYPT_E_REVOCATION_OFFLINE:
      return "The revocation function was unable to check revocation because the revocation server was offline";

    case CRYPT_E_NOT_IN_REVOCATION_DATABASE:
      return "The certificate is not in the revocation server's database";

    case CRYPT_E_INVALID_NUMERIC_STRING:
      return "The string contains a non-numeric character";

    case CRYPT_E_INVALID_PRINTABLE_STRING:
      return "The string contains a non-printable character";

    case CRYPT_E_INVALID_IA5_STRING:
      return "The string contains a character not in the 7 bit ASCII character set";

    case CRYPT_E_INVALID_X500_STRING:
      return "The string contains an invalid X500 name attribute key, oid, value or delimiter";

    case CRYPT_E_NOT_CHAR_STRING:
      return "The dwValueType for the CERT_NAME_VALUE is not one of the character strings.  Most likely it is either a CERT_RDN_ENCODED_BLOB or CERT_TDN_OCTED_STRING";

    case CRYPT_E_FILERESIZED:
      return "The Put operation can not continue.  The file needs to be resized.  However, there is already a signature present.  A complete signing operation must be done";

    case CRYPT_E_SECURITY_SETTINGS:
      return "The cryptographic operation failed due to a local security option setting";

    case CRYPT_E_NO_VERIFY_USAGE_DLL:
      return "No DLL or exported function was found to verify subject usage";

    case CRYPT_E_NO_VERIFY_USAGE_CHECK:
      return "The called function was unable to do a usage check on the subject";

    case CRYPT_E_VERIFY_USAGE_OFFLINE:
      return "Since the server was offline, the called function was unable to complete the usage check";

    case CRYPT_E_NOT_IN_CTL:
      return "The subject was not found in a Certificate Trust List (CTL)";

    case CRYPT_E_NO_TRUSTED_SIGNER:
      return "None of the signers of the cryptographic message or certificate trust list is trusted";

    case CRYPT_E_MISSING_PUBKEY_PARA:
      return "The public key's algorithm parameters are missing";

    case CRYPT_E_OSS_ERROR:
      return "OSS Certificate encode/decode error code base";

    case OSS_MORE_BUF:
      return "OSS ASN.1 Error: Output Buffer is too small";

    case OSS_NEGATIVE_UINTEGER:
      return "OSS ASN.1 Error: Signed integer is encoded as a unsigned integer";

    case OSS_PDU_RANGE:
      return "OSS ASN.1 Error: Unknown ASN.1 data type";

    case OSS_MORE_INPUT:
      return "OSS ASN.1 Error: Output buffer is too small, the decoded data has been truncated";

    case OSS_DATA_ERROR:
      return "OSS ASN.1 Error: Invalid data";

    case OSS_BAD_ARG:
      return "OSS ASN.1 Error: Invalid argument";

    case OSS_BAD_VERSION:
      return "OSS ASN.1 Error: Encode/Decode version mismatch";

    case OSS_OUT_MEMORY:
      return "OSS ASN.1 Error: Out of memory";

    case OSS_PDU_MISMATCH:
      return "OSS ASN.1 Error: Encode/Decode Error";

    case OSS_LIMITED:
      return "OSS ASN.1 Error: Internal Error";

    case OSS_BAD_PTR:
      return "OSS ASN.1 Error: Invalid data";

    case OSS_BAD_TIME:
      return "OSS ASN.1 Error: Invalid data";

    case OSS_INDEFINITE_NOT_SUPPORTED:
      return "OSS ASN.1 Error: Unsupported BER indefinite-length encoding";

    case OSS_MEM_ERROR:
      return "OSS ASN.1 Error: Access violation";

    case OSS_BAD_TABLE:
      return "OSS ASN.1 Error: Invalid data";

    case OSS_TOO_LONG:
      return "OSS ASN.1 Error: Invalid data";

    case OSS_CONSTRAINT_VIOLATED:
      return "OSS ASN.1 Error: Invalid data";

    case OSS_FATAL_ERROR:
      return "OSS ASN.1 Error: Internal Error";

    case OSS_ACCESS_SERIALIZATION_ERROR:
      return "OSS ASN.1 Error: Multi-threading conflict";

    case OSS_NULL_TBL:
      return "OSS ASN.1 Error: Invalid data";

    case OSS_NULL_FCN:
      return "OSS ASN.1 Error: Invalid data";

    case OSS_BAD_ENCRULES:
      return "OSS ASN.1 Error: Invalid data";

    case OSS_UNAVAIL_ENCRULES:
      return "OSS ASN.1 Error: Encode/Decode function not implemented";

    case OSS_CANT_OPEN_TRACE_WINDOW:
      return "OSS ASN.1 Error: Trace file error";

    case OSS_UNIMPLEMENTED:
      return "OSS ASN.1 Error: Function not implemented";

    case OSS_OID_DLL_NOT_LINKED:
      return "OSS ASN.1 Error: Program link error";

    case OSS_CANT_OPEN_TRACE_FILE:
      return "OSS ASN.1 Error: Trace file error";

    case OSS_TRACE_FILE_ALREADY_OPEN:
      return "OSS ASN.1 Error: Trace file error";

    case OSS_TABLE_MISMATCH:
      return "OSS ASN.1 Error: Invalid data";

    case OSS_TYPE_NOT_SUPPORTED:
      return "OSS ASN.1 Error: Invalid data";

    case OSS_REAL_DLL_NOT_LINKED:
      return "OSS ASN.1 Error: Program link error";

    case OSS_REAL_CODE_NOT_LINKED:
      return "OSS ASN.1 Error: Program link error";

    case OSS_OUT_OF_RANGE:
      return "OSS ASN.1 Error: Program link error";

    case OSS_COPIER_DLL_NOT_LINKED:
      return "OSS ASN.1 Error: Program link error";

    case OSS_CONSTRAINT_DLL_NOT_LINKED:
      return "OSS ASN.1 Error: Program link error";

    case OSS_COMPARATOR_DLL_NOT_LINKED:
      return "OSS ASN.1 Error: Program link error";

    case OSS_COMPARATOR_CODE_NOT_LINKED:
      return "OSS ASN.1 Error: Program link error";

    case OSS_MEM_MGR_DLL_NOT_LINKED:
      return "OSS ASN.1 Error: Program link error";

    case OSS_PDV_DLL_NOT_LINKED:
      return "OSS ASN.1 Error: Program link error";

    case OSS_PDV_CODE_NOT_LINKED:
      return "OSS ASN.1 Error: Program link error";

    case OSS_API_DLL_NOT_LINKED:
      return "OSS ASN.1 Error: Program link error";

    case OSS_BERDER_DLL_NOT_LINKED:
      return "OSS ASN.1 Error: Program link error";

    case OSS_PER_DLL_NOT_LINKED:
      return "OSS ASN.1 Error: Program link error";

    case OSS_OPEN_TYPE_ERROR:
      return "OSS ASN.1 Error: Program link error";

    case OSS_MUTEX_NOT_CREATED:
      return "OSS ASN.1 Error: System resource error";

    case OSS_CANT_CLOSE_TRACE_FILE:
      return "OSS ASN.1 Error: Trace file error";

    case CRYPT_E_ASN1_ERROR:
      return "ASN1 Certificate encode/decode error code base";

    case CRYPT_E_ASN1_INTERNAL:
      return "ASN1 internal encode or decode error";

    case CRYPT_E_ASN1_EOD:
      return "ASN1 unexpected end of data";

    case CRYPT_E_ASN1_CORRUPT:
      return "ASN1 corrupted data";

    case CRYPT_E_ASN1_LARGE:
      return "ASN1 value too large";

    case CRYPT_E_ASN1_CONSTRAINT:
      return "ASN1 constraint violated";

    case CRYPT_E_ASN1_MEMORY:
      return "ASN1 out of memory";

    case CRYPT_E_ASN1_OVERFLOW:
      return "ASN1 buffer overflow";

    case CRYPT_E_ASN1_BADPDU:
      return "ASN1 function not supported for this PDU";

    case CRYPT_E_ASN1_BADARGS:
      return "ASN1 bad arguments to function call";

    case CRYPT_E_ASN1_BADREAL:
      return "ASN1 bad real value";

    case CRYPT_E_ASN1_BADTAG:
      return "ASN1 bad tag value met";

    case CRYPT_E_ASN1_CHOICE:
      return "ASN1 bad choice value";

    case CRYPT_E_ASN1_RULE:
      return "ASN1 bad encoding rule";

    case CRYPT_E_ASN1_UTF8:
      return "ASN1 bad unicode (UTF8)";

    case CRYPT_E_ASN1_PDU_TYPE:
      return "ASN1 bad PDU type";

    case CRYPT_E_ASN1_NYI:
      return "ASN1 not yet implemented";

    case CRYPT_E_ASN1_EXTENDED:
      return "ASN1 skipped unknown extension(s)";

    case CRYPT_E_ASN1_NOEOD:
      return "ASN1 end of data expected";

    case CERTSRV_E_BAD_REQUESTSUBJECT:
      return "The request subject name is invalid or too long";

    case CERTSRV_E_NO_REQUEST:
      return "The request does not exist";

    case CERTSRV_E_BAD_REQUESTSTATUS:
      return "The request's current status does not allow this operation";

    case CERTSRV_E_PROPERTY_EMPTY:
      return "The requested property value is empty";

    case CERTSRV_E_INVALID_CA_CERTIFICATE:
      return "The certification authority's certificate contains invalid data";

    case CERTSRV_E_SERVER_SUSPENDED:
      return "Certificate service has been suspended for a database restore operation";

    case CERTSRV_E_ENCODING_LENGTH:
      return "The certificate contains an encoded length that is potentially incompatible with older enrollment software";

    case CERTSRV_E_ROLECONFLICT:
      return "The operation is denied. The user has multiple roles assigned and the certification authority is configured to enforce role separation";

    case CERTSRV_E_RESTRICTEDOFFICER:
      return "The operation is denied. It can only be performed by a certificate manager that is allowed to manage certificates for the current requester";

    case CERTSRV_E_KEY_ARCHIVAL_NOT_CONFIGURED:
      return "Cannot archive private key.  The certification authority is not configured for key archival";

    case CERTSRV_E_NO_VALID_KRA:
      return "Cannot archive private key.  The certification authority could not verify one or more key recovery certificates";

    case CERTSRV_E_BAD_REQUEST_KEY_ARCHIVAL:
      return "The request is incorrectly formatted.  The encrypted private key must be in an unauthenticated attribute in an outermost signature";

    case CERTSRV_E_NO_CAADMIN_DEFINED:
      return "At least one security principal must have the permission to manage this CA";

    case CERTSRV_E_BAD_RENEWAL_CERT_ATTRIBUTE:
      return "The request contains an invalid renewal certificate attribute";

    case CERTSRV_E_NO_DB_SESSIONS:
      return "An attempt was made to open a Certification Authority database session, but there are already too many active sessions.  The server may need to be configured to allow additional sessions";

    case CERTSRV_E_ALIGNMENT_FAULT:
      return "A memory reference caused a data alignment fault";

    case CERTSRV_E_ENROLL_DENIED:
      return "The permissions on this certification authority do not allow the current user to enroll for certificates";

    case CERTSRV_E_TEMPLATE_DENIED:
      return "The permissions on the certificate template do not allow the current user to enroll for this type of certificate";

    case CERTSRV_E_DOWNLEVEL_DC_SSL_OR_UPGRADE:
      return "The contacted domain controller cannot support signed LDAP traffic.  Update the domain controller or configure Certificate Services to use SSL for Active Directory access";

    case CERTSRV_E_UNSUPPORTED_CERT_TYPE:
      return "The requested certificate template is not supported by this CA";

    case CERTSRV_E_NO_CERT_TYPE:
      return "The request contains no certificate template information";

    case CERTSRV_E_TEMPLATE_CONFLICT:
      return "The request contains conflicting template information";

    case CERTSRV_E_SUBJECT_ALT_NAME_REQUIRED:
      return "The request is missing a required Subject Alternate name extension";

    case CERTSRV_E_ARCHIVED_KEY_REQUIRED:
      return "The request is missing a required private key for archival by the server";

    case CERTSRV_E_SMIME_REQUIRED:
      return "The request is missing a required SMIME capabilities extension";

    case CERTSRV_E_BAD_RENEWAL_SUBJECT:
      return "The request was made on behalf of a subject other than the caller.  The certificate template must be configured to require at least one signature to authorize the request";

    case CERTSRV_E_BAD_TEMPLATE_VERSION:
      return "The request template version is newer than the supported template version";

    case CERTSRV_E_TEMPLATE_POLICY_REQUIRED:
      return "The template is missing a required signature policy attribute";

    case CERTSRV_E_SIGNATURE_POLICY_REQUIRED:
      return "The request is missing required signature policy information";

    case CERTSRV_E_SIGNATURE_COUNT:
      return "The request is missing one or more required signatures";

    case CERTSRV_E_SIGNATURE_REJECTED:
      return "One or more signatures did not include the required application or issuance policies.  The request is missing one or more required valid signatures";

    case CERTSRV_E_ISSUANCE_POLICY_REQUIRED:
      return "The request is missing one or more required signature issuance policies";

    case CERTSRV_E_SUBJECT_UPN_REQUIRED:
      return "The UPN is unavailable and cannot be added to the Subject Alternate name";

    case CERTSRV_E_SUBJECT_DIRECTORY_GUID_REQUIRED:
      return "The Active Directory GUID is unavailable and cannot be added to the Subject Alternate name";

    case CERTSRV_E_SUBJECT_DNS_REQUIRED:
      return "The DNS name is unavailable and cannot be added to the Subject Alternate name";

    case CERTSRV_E_ARCHIVED_KEY_UNEXPECTED:
      return "The request includes a private key for archival by the server, but key archival is not enabled for the specified certificate template";

    case CERTSRV_E_KEY_LENGTH:
      return "The public key does not meet the minimum size required by the specified certificate template";

    case CERTSRV_E_SUBJECT_EMAIL_REQUIRED:
      return "The EMail name is unavailable and cannot be added to the Subject or Subject Alternate name";

    case CERTSRV_E_UNKNOWN_CERT_TYPE:
      return "One or more certificate templates to be enabled on this certification authority could not be found";

    case CERTSRV_E_CERT_TYPE_OVERLAP:
      return "The certificate template renewal period is longer than the certificate validity period.  The template should be reconfigured or the CA certificate renewed";

    case XENROLL_E_KEY_NOT_EXPORTABLE:
      return "The key is not exportable";

    case XENROLL_E_CANNOT_ADD_ROOT_CERT:
      return "You cannot add the root CA certificate into your local store";

    case XENROLL_E_RESPONSE_KA_HASH_NOT_FOUND:
      return "The key archival hash attribute was not found in the response";

    case XENROLL_E_RESPONSE_UNEXPECTED_KA_HASH:
      return "An unexpected key archival hash attribute was found in the response";

    case XENROLL_E_RESPONSE_KA_HASH_MISMATCH:
      return "There is a key archival hash mismatch between the request and the response";

    case XENROLL_E_KEYSPEC_SMIME_MISMATCH:
      return "Signing certificate cannot include SMIME extension";

    case TRUST_E_SYSTEM_ERROR:
      return "A system-level error occurred while verifying trust";

    case TRUST_E_NO_SIGNER_CERT:
      return "The certificate for the signer of the message is invalid or not found";

    case TRUST_E_COUNTER_SIGNER:
      return "One of the counter signatures was invalid";

    case TRUST_E_CERT_SIGNATURE:
      return "The signature of the certificate can not be verified";

    case TRUST_E_TIME_STAMP:
      return "The timestamp signature and/or certificate could not be verified or is malformed";

    case TRUST_E_BAD_DIGEST:
      return "The digital signature of the object did not verify";

    case TRUST_E_BASIC_CONSTRAINTS:
      return "A certificate's basic constraint extension has not been observed";

    case TRUST_E_FINANCIAL_CRITERIA:
      return "The certificate does not meet or contain the Authenticode(tm) financial extensions";

    case MSSIPOTF_E_OUTOFMEMRANGE:
      return "Tried to reference a part of the file outside the proper range";

    case MSSIPOTF_E_CANTGETOBJECT:
      return "Could not retrieve an object from the file";

    case MSSIPOTF_E_NOHEADTABLE:
      return "Could not find the head table in the file";

    case MSSIPOTF_E_BAD_MAGICNUMBER:
      return "The magic number in the head table is incorrect";

    case MSSIPOTF_E_BAD_OFFSET_TABLE:
      return "The offset table has incorrect values";

    case MSSIPOTF_E_TABLE_TAGORDER:
      return "Duplicate table tags or tags out of alphabetical order";

    case MSSIPOTF_E_TABLE_LONGWORD:
      return "A table does not start on a long word boundary";

    case MSSIPOTF_E_BAD_FIRST_TABLE_PLACEMENT:
      return "First table does not appear after header information";

    case MSSIPOTF_E_TABLES_OVERLAP:
      return "Two or more tables overlap";

    case MSSIPOTF_E_TABLE_PADBYTES:
      return "Too many pad bytes between tables or pad bytes are not 0";

    case MSSIPOTF_E_FILETOOSMALL:
      return "File is too small to contain the last table";

    case MSSIPOTF_E_TABLE_CHECKSUM:
      return "A table checksum is incorrect";

    case MSSIPOTF_E_FILE_CHECKSUM:
      return "The file checksum is incorrect";

    case MSSIPOTF_E_FAILED_POLICY:
      return "The signature does not have the correct attributes for the policy";

    case MSSIPOTF_E_FAILED_HINTS_CHECK:
      return "The file did not pass the hints check";

    case MSSIPOTF_E_NOT_OPENTYPE:
      return "The file is not an OpenType file";

    case MSSIPOTF_E_FILE:
      return "Failed on a file operation (open, map, read, write)";

    case MSSIPOTF_E_CRYPT:
      return "A call to a CryptoAPI function failed";

    case MSSIPOTF_E_BADVERSION:
      return "There is a bad version number in the file";

    case MSSIPOTF_E_DSIG_STRUCTURE:
      return "The structure of the DSIG table is incorrect";

    case MSSIPOTF_E_PCONST_CHECK:
      return "A check failed in a partially constant table";

    case MSSIPOTF_E_STRUCTURE:
      return "Some kind of structural error";

    case TRUST_E_PROVIDER_UNKNOWN:
      return "Unknown trust provider";

    case TRUST_E_ACTION_UNKNOWN:
      return "The trust verification action specified is not supported by the specified trust provider";

    case TRUST_E_SUBJECT_FORM_UNKNOWN:
      return "The form specified for the subject is not one supported or known by the specified trust provider";

    case TRUST_E_SUBJECT_NOT_TRUSTED:
      return "The subject is not trusted for the specified action";

    case DIGSIG_E_ENCODE:
      return "Error due to problem in ASN.1 encoding process";

    case DIGSIG_E_DECODE:
      return "Error due to problem in ASN.1 decoding process";

    case DIGSIG_E_EXTENSIBILITY:
      return "Reading / writing Extensions where Attributes are appropriate, and visa versa";

    case DIGSIG_E_CRYPTO:
      return "Unspecified cryptographic failure";

    case PERSIST_E_SIZEDEFINITE:
      return "The size of the data could not be determined";

    case PERSIST_E_SIZEINDEFINITE:
      return "The size of the indefinite-sized data could not be determined";

    case PERSIST_E_NOTSELFSIZING:
      return "This object does not read and write self-sizing data";

    case TRUST_E_NOSIGNATURE:
      return "No signature was present in the subject";

    case CERT_E_EXPIRED:
      return "A required certificate is not within its validity period when verifying against the current system clock or the timestamp in the signed file";

    case CERT_E_VALIDITYPERIODNESTING:
      return "The validity periods of the certification chain do not nest correctly";

    case CERT_E_ROLE:
      return "A certificate that can only be used as an end-entity is being used as a CA or visa versa";

    case CERT_E_PATHLENCONST:
      return "A path length constraint in the certification chain has been violated";

    case CERT_E_CRITICAL:
      return "A certificate contains an unknown extension that is marked 'critical'";

    case CERT_E_PURPOSE:
      return "A certificate being used for a purpose other than the ones specified by its CA";

    case CERT_E_ISSUERCHAINING:
      return "A parent of a given certificate in fact did not issue that child certificate";

    case CERT_E_MALFORMED:
      return "A certificate is missing or has an empty value for an important field, such as a subject or issuer name";

    case CERT_E_UNTRUSTEDROOT:
      return "A certificate chain processed, but terminated in a root certificate which is not trusted by the trust provider";

    case CERT_E_CHAINING:
      return "A certificate chain could not be built to a trusted root authority";

    case TRUST_E_FAIL:
      return "Generic trust failure";

    case CERT_E_REVOKED:
      return "A certificate was explicitly revoked by its issuer";

    case CERT_E_UNTRUSTEDTESTROOT:
      return "The certification path terminates with the test root which is not trusted with the current policy settings";

    case CERT_E_REVOCATION_FAILURE:
      return "The revocation process could not continue - the certificate(s) could not be checked";

    case CERT_E_CN_NO_MATCH:
      return "The certificate's CN name does not match the passed value";

    case CERT_E_WRONG_USAGE:
      return "The certificate is not valid for the requested usage";

    case TRUST_E_EXPLICIT_DISTRUST:
      return "The certificate was explicitly marked as untrusted by the user";

    case CERT_E_UNTRUSTEDCA:
      return "A certification chain processed correctly, but one of the CA certificates is not trusted by the policy provider";

    case CERT_E_INVALID_POLICY:
      return "The certificate has invalid policy";

    case CERT_E_INVALID_NAME:
      return "The certificate has an invalid name. The name is not included in the permitted list or is explicitly excluded";

    case SPAPI_E_EXPECTED_SECTION_NAME:
      return "A non-empty line was encountered in the INF before the start of a section";

    case SPAPI_E_BAD_SECTION_NAME_LINE:
      return "A section name marker in the INF is not complete, or does not exist on a line by itself";

    case SPAPI_E_SECTION_NAME_TOO_LONG:
      return "An INF section was encountered whose name exceeds the maximum section name length";

    case SPAPI_E_GENERAL_SYNTAX:
      return "The syntax of the INF is invalid";

    case SPAPI_E_WRONG_INF_STYLE:
      return "The style of the INF is different than what was requested";

    case SPAPI_E_SECTION_NOT_FOUND:
      return "The required section was not found in the INF";

    case SPAPI_E_LINE_NOT_FOUND:
      return "The required line was not found in the INF";

    case SPAPI_E_NO_BACKUP:
      return "The files affected by the installation of this file queue have not been backed up for uninstall";

    case SPAPI_E_NO_ASSOCIATED_CLASS:
      return "The INF or the device information set or element does not have an associated install class";

    case SPAPI_E_CLASS_MISMATCH:
      return "The INF or the device information set or element does not match the specified install class";

    case SPAPI_E_DUPLICATE_FOUND:
      return "An existing device was found that is a duplicate of the device being manually installed";

    case SPAPI_E_NO_DRIVER_SELECTED:
      return "There is no driver selected for the device information set or element";

    case SPAPI_E_KEY_DOES_NOT_EXIST:
      return "The requested device registry key does not exist";

    case SPAPI_E_INVALID_DEVINST_NAME:
      return "The device instance name is invalid";

    case SPAPI_E_INVALID_CLASS:
      return "The install class is not present or is invalid";

    case SPAPI_E_DEVINST_ALREADY_EXISTS:
      return "The device instance cannot be created because it already exists";

    case SPAPI_E_DEVINFO_NOT_REGISTERED:
      return "The operation cannot be performed on a device information element that has not been registered";

    case SPAPI_E_INVALID_REG_PROPERTY:
      return "The device property code is invalid";

    case SPAPI_E_NO_INF:
      return "The INF from which a driver list is to be built does not exist";

    case SPAPI_E_NO_SUCH_DEVINST:
      return "The device instance does not exist in the hardware tree";

    case SPAPI_E_CANT_LOAD_CLASS_ICON:
      return "The icon representing this install class cannot be loaded";

    case SPAPI_E_INVALID_CLASS_INSTALLER:
      return "The class installer registry entry is invalid";

    case SPAPI_E_DI_DO_DEFAULT:
      return "The class installer has indicated that the default action should be performed for this installation request";

    case SPAPI_E_DI_NOFILECOPY:
      return "The operation does not require any files to be copied";

    case SPAPI_E_INVALID_HWPROFILE:
      return "The specified hardware profile does not exist";

    case SPAPI_E_NO_DEVICE_SELECTED:
      return "There is no device information element currently selected for this device information set";

    case SPAPI_E_DEVINFO_LIST_LOCKED:
      return "The operation cannot be performed because the device information set is locked";

    case SPAPI_E_DEVINFO_DATA_LOCKED:
      return "The operation cannot be performed because the device information element is locked";

    case SPAPI_E_DI_BAD_PATH:
      return "The specified path does not contain any applicable device INFs";

    case SPAPI_E_NO_CLASSINSTALL_PARAMS:
      return "No class installer parameters have been set for the device information set or element";

    case SPAPI_E_FILEQUEUE_LOCKED:
      return "The operation cannot be performed because the file queue is locked";

    case SPAPI_E_BAD_SERVICE_INSTALLSECT:
      return "A service installation section in this INF is invalid";

    case SPAPI_E_NO_CLASS_DRIVER_LIST:
      return "There is no class driver list for the device information element";

    case SPAPI_E_NO_ASSOCIATED_SERVICE:
      return "The installation failed because a function driver was not specified for this device instance";

    case SPAPI_E_NO_DEFAULT_DEVICE_INTERFACE:
      return "There is presently no default device interface designated for this interface class";

    case SPAPI_E_DEVICE_INTERFACE_ACTIVE:
      return "The operation cannot be performed because the device interface is currently active";

    case SPAPI_E_DEVICE_INTERFACE_REMOVED:
      return "The operation cannot be performed because the device interface has been removed from the system";

    case SPAPI_E_BAD_INTERFACE_INSTALLSECT:
      return "An interface installation section in this INF is invalid";

    case SPAPI_E_NO_SUCH_INTERFACE_CLASS:
      return "This interface class does not exist in the system";

    case SPAPI_E_INVALID_REFERENCE_STRING:
      return "The reference string supplied for this interface device is invalid";

    case SPAPI_E_INVALID_MACHINENAME:
      return "The specified machine name does not conform to UNC naming conventions";

    case SPAPI_E_REMOTE_COMM_FAILURE:
      return "A general remote communication error occurred";

    case SPAPI_E_MACHINE_UNAVAILABLE:
      return "The machine selected for remote communication is not available at this time";

    case SPAPI_E_NO_CONFIGMGR_SERVICES:
      return "The Plug and Play service is not available on the remote machine";

    case SPAPI_E_INVALID_PROPPAGE_PROVIDER:
      return "The property page provider registry entry is invalid";

    case SPAPI_E_NO_SUCH_DEVICE_INTERFACE:
      return "The requested device interface is not present in the system";

    case SPAPI_E_DI_POSTPROCESSING_REQUIRED:
      return "The device's co-installer has additional work to perform after installation is complete";

    case SPAPI_E_INVALID_COINSTALLER:
      return "The device's co-installer is invalid";

    case SPAPI_E_NO_COMPAT_DRIVERS:
      return "There are no compatible drivers for this device";

    case SPAPI_E_NO_DEVICE_ICON:
      return "There is no icon that represents this device or device type";

    case SPAPI_E_INVALID_INF_LOGCONFIG:
      return "A logical configuration specified in this INF is invalid";

    case SPAPI_E_DI_DONT_INSTALL:
      return "The class installer has denied the request to install or upgrade this device";

    case SPAPI_E_INVALID_FILTER_DRIVER:
      return "One of the filter drivers installed for this device is invalid";

    case SPAPI_E_NON_WINDOWS_NT_DRIVER:
      return "The driver selected for this device does not support Windows XP";

    case SPAPI_E_NON_WINDOWS_DRIVER:
      return "The driver selected for this device does not support Windows";

    case SPAPI_E_NO_CATALOG_FOR_OEM_INF:
      return "The third-party INF does not contain digital signature information";

    case SPAPI_E_DEVINSTALL_QUEUE_NONNATIVE:
      return "An invalid attempt was made to use a device installation file queue for verification of digital signatures relative to other platforms";

    case SPAPI_E_NOT_DISABLEABLE:
      return "The device cannot be disabled";

    case SPAPI_E_CANT_REMOVE_DEVINST:
      return "The device could not be dynamically removed";

    case SPAPI_E_INVALID_TARGET:
      return "Cannot copy to specified target";

    case SPAPI_E_DRIVER_NONNATIVE:
      return "Driver is not intended for this platform";

    case SPAPI_E_IN_WOW64:
      return "Operation not allowed in WOW64";

    case SPAPI_E_SET_SYSTEM_RESTORE_POINT:
      return "The operation involving unsigned file copying was rolled back, so that a system restore point could be set";

    case SPAPI_E_INCORRECTLY_COPIED_INF:
      return "An INF was copied into the Windows INF directory in an improper manner";

    case SPAPI_E_SCE_DISABLED:
      return "The Security Configuration Editor (SCE) APIs have been disabled on this Embedded product";

    case SPAPI_E_UNKNOWN_EXCEPTION:
      return "An unknown exception was encountered";

    case SPAPI_E_PNP_REGISTRY_ERROR:
      return "A problem was encountered when accessing the Plug and Play registry database";

    case SPAPI_E_REMOTE_REQUEST_UNSUPPORTED:
      return "The requested operation is not supported for a remote machine";

    case SPAPI_E_NOT_AN_INSTALLED_OEM_INF:
      return "The specified file is not an installed OEM INF";

    case SPAPI_E_INF_IN_USE_BY_DEVICES:
      return "One or more devices are presently installed using the specified INF";

    case SPAPI_E_DI_FUNCTION_OBSOLETE:
      return "The requested device install operation is obsolete";

    case SPAPI_E_NO_AUTHENTICODE_CATALOG:
      return "A file could not be verified because it does not have an associated catalog signed via Authenticode(tm)";

    case SPAPI_E_AUTHENTICODE_DISALLOWED:
      return "Authenticode(tm) signature verification is not supported for the specified INF";

    case SPAPI_E_AUTHENTICODE_TRUSTED_PUBLISHER:
      return "The INF was signed with an Authenticode(tm) catalog from a trusted publisher";

    case SPAPI_E_AUTHENTICODE_TRUST_NOT_ESTABLISHED:
      return "The publisher of an Authenticode(tm) signed catalog has not yet been established as trusted";

    case SPAPI_E_AUTHENTICODE_PUBLISHER_NOT_TRUSTED:
      return "The publisher of an Authenticode(tm) signed catalog was not established as trusted";

    case SPAPI_E_SIGNATURE_OSATTRIBUTE_MISMATCH:
      return "The software was tested for compliance with Windows Logo requirements on a different version of Windows, and may not be compatible with this version";

    case SPAPI_E_ONLY_VALIDATE_VIA_AUTHENTICODE:
      return "The file may only be validated by a catalog signed via Authenticode(tm)";

    case SPAPI_E_UNRECOVERABLE_STACK_OVERFLOW:
      return "An unrecoverable stack overflow was encountered";

    case SPAPI_E_ERROR_NOT_INSTALLED:
      return "No installed components were detected";

    case SCARD_F_INTERNAL_ERROR:
      return "An internal consistency check failed";

    case SCARD_E_CANCELLED:
      return "The action was cancelled by an SCardCancel request";

    case SCARD_E_INVALID_HANDLE:
      return "The supplied handle was invalid";

    case SCARD_E_INVALID_PARAMETER:
      return "One or more of the supplied parameters could not be properly interpreted";

    case SCARD_E_INVALID_TARGET:
      return "Registry startup information is missing or invalid";

    case SCARD_E_NO_MEMORY:
      return "Not enough memory available to complete this command";

    case SCARD_F_WAITED_TOO_LONG:
      return "An internal consistency timer has expired";

    case SCARD_E_INSUFFICIENT_BUFFER:
      return "The data buffer to receive returned data is too small for the returned data";

    case SCARD_E_UNKNOWN_READER:
      return "The specified reader name is not recognized";

    case SCARD_E_TIMEOUT:
      return "The user-specified timeout value has expired";

    case SCARD_E_SHARING_VIOLATION:
      return "The smart card cannot be accessed because of other connections outstanding";

    case SCARD_E_NO_SMARTCARD:
      return "The operation requires a Smart Card, but no Smart Card is currently in the device";

    case SCARD_E_UNKNOWN_CARD:
      return "The specified smart card name is not recognized";

    case SCARD_E_CANT_DISPOSE:
      return "The system could not dispose of the media in the requested manner";

    case SCARD_E_PROTO_MISMATCH:
      return "The requested protocols are incompatible with the protocol currently in use with the smart card";

    case SCARD_E_NOT_READY:
      return "The reader or smart card is not ready to accept commands";

    case SCARD_E_INVALID_VALUE:
      return "One or more of the supplied parameters values could not be properly interpreted";

    case SCARD_E_SYSTEM_CANCELLED:
      return "The action was cancelled by the system, presumably to log off or shut down";

    case SCARD_F_COMM_ERROR:
      return "An internal communications error has been detected";

    case SCARD_F_UNKNOWN_ERROR:
      return "An internal error has been detected, but the source is unknown";

    case SCARD_E_INVALID_ATR:
      return "An ATR obtained from the registry is not a valid ATR string";

    case SCARD_E_NOT_TRANSACTED:
      return "An attempt was made to end a non-existent transaction";

    case SCARD_E_READER_UNAVAILABLE:
      return "The specified reader is not currently available for use";

    case SCARD_P_SHUTDOWN:
      return "The operation has been aborted to allow the server application to exit";

    case SCARD_E_PCI_TOO_SMALL:
      return "The PCI Receive buffer was too small";

    case SCARD_E_READER_UNSUPPORTED:
      return "The reader driver does not meet minimal requirements for support";

    case SCARD_E_DUPLICATE_READER:
      return "The reader driver did not produce a unique reader name";

    case SCARD_E_CARD_UNSUPPORTED:
      return "The smart card does not meet minimal requirements for support";

    case SCARD_E_NO_SERVICE:
      return "The Smart card resource manager is not running";

    case SCARD_E_SERVICE_STOPPED:
      return "The Smart card resource manager has shut down";

    case SCARD_E_UNEXPECTED:
      return "An unexpected card error has occurred";

    case SCARD_E_ICC_INSTALLATION:
      return "No Primary Provider can be found for the smart card";

    case SCARD_E_ICC_CREATEORDER:
      return "The requested order of object creation is not supported";

    case SCARD_E_UNSUPPORTED_FEATURE:
      return "This smart card does not support the requested feature";

    case SCARD_E_DIR_NOT_FOUND:
      return "The identified directory does not exist in the smart card";

    case SCARD_E_FILE_NOT_FOUND:
      return "The identified file does not exist in the smart card";

    case SCARD_E_NO_DIR:
      return "The supplied path does not represent a smart card directory";

    case SCARD_E_NO_FILE:
      return "The supplied path does not represent a smart card file";

    case SCARD_E_NO_ACCESS:
      return "Access is denied to this file";

    case SCARD_E_WRITE_TOO_MANY:
      return "The smartcard does not have enough memory to store the information";

    case SCARD_E_BAD_SEEK:
      return "There was an error trying to set the smart card file object pointer";

    case SCARD_E_INVALID_CHV:
      return "The supplied PIN is incorrect";

    case SCARD_E_UNKNOWN_RES_MNG:
      return "An unrecognized error code was returned from a layered component";

    case SCARD_E_NO_SUCH_CERTIFICATE:
      return "The requested certificate does not exist";

    case SCARD_E_CERTIFICATE_UNAVAILABLE:
      return "The requested certificate could not be obtained";

    case SCARD_E_NO_READERS_AVAILABLE:
      return "Cannot find a smart card reader";

    case SCARD_E_COMM_DATA_LOST:
      return "A communications error with the smart card has been detected.  Retry the operation";

    case SCARD_E_NO_KEY_CONTAINER:
      return "The requested key container does not exist on the smart card";

    case SCARD_E_SERVER_TOO_BUSY:
      return "The Smart card resource manager is too busy to complete this operation";

    case SCARD_W_UNSUPPORTED_CARD:
      return "The reader cannot communicate with the smart card, due to ATR configuration conflicts";

    case SCARD_W_UNRESPONSIVE_CARD:
      return "The smart card is not responding to a reset";

    case SCARD_W_UNPOWERED_CARD:
      return "Power has been removed from the smart card, so that further communication is not possible";

    case SCARD_W_RESET_CARD:
      return "The smart card has been reset, so any shared state information is invalid";

    case SCARD_W_REMOVED_CARD:
      return "The smart card has been removed, so that further communication is not possible";

    case SCARD_W_SECURITY_VIOLATION:
      return "Access was denied because of a security violation";

    case SCARD_W_WRONG_CHV:
      return "The card cannot be accessed because the wrong PIN was presented";

    case SCARD_W_CHV_BLOCKED:
      return "The card cannot be accessed because the maximum number of PIN entry attempts has been reached";

    case SCARD_W_EOF:
      return "The end of the smart card file has been reached";

    case SCARD_W_CANCELLED_BY_USER:
      return "The action was cancelled by the user";

    case SCARD_W_CARD_NOT_AUTHENTICATED:
      return "No PIN was presented to the smart card";

    case COMADMIN_E_OBJECTERRORS:
      return "Errors occurred accessing one or more objects - the ErrorInfo collection may have more detail";

    case COMADMIN_E_OBJECTINVALID:
      return "One or more of the object's properties are missing or invalid";

    case COMADMIN_E_KEYMISSING:
      return "The object was not found in the catalog";

    case COMADMIN_E_ALREADYINSTALLED:
      return "The object is already registered";

    case COMADMIN_E_APP_FILE_WRITEFAIL:
      return "Error occurred writing to the application file";

    case COMADMIN_E_APP_FILE_READFAIL:
      return "Error occurred reading the application file";

    case COMADMIN_E_APP_FILE_VERSION:
      return "Invalid version number in application file";

    case COMADMIN_E_BADPATH:
      return "The file path is invalid";

    case COMADMIN_E_APPLICATIONEXISTS:
      return "The application is already installed";

    case COMADMIN_E_ROLEEXISTS:
      return "The role already exists";

    case COMADMIN_E_CANTCOPYFILE:
      return "An error occurred copying the file";

    case COMADMIN_E_NOUSER:
      return "One or more users are not valid";

    case COMADMIN_E_INVALIDUSERIDS:
      return "One or more users in the application file are not valid";

    case COMADMIN_E_NOREGISTRYCLSID:
      return "The component's CLSID is missing or corrupt";

    case COMADMIN_E_BADREGISTRYPROGID:
      return "The component's progID is missing or corrupt";

    case COMADMIN_E_AUTHENTICATIONLEVEL:
      return "Unable to set required authentication level for update request";

    case COMADMIN_E_USERPASSWDNOTVALID:
      return "The identity or password set on the application is not valid";

    case COMADMIN_E_CLSIDORIIDMISMATCH:
      return "Application file CLSIDs or IIDs do not match corresponding DLLs";

    case COMADMIN_E_REMOTEINTERFACE:
      return "Interface information is either missing or changed";

    case COMADMIN_E_DLLREGISTERSERVER:
      return "DllRegisterServer failed on component install";

    case COMADMIN_E_NOSERVERSHARE:
      return "No server file share available";

    case COMADMIN_E_DLLLOADFAILED:
      return "DLL could not be loaded";

    case COMADMIN_E_BADREGISTRYLIBID:
      return "The registered TypeLib ID is not valid";

    case COMADMIN_E_APPDIRNOTFOUND:
      return "Application install directory not found";

    case COMADMIN_E_REGISTRARFAILED:
      return "Errors occurred while in the component registrar";

    case COMADMIN_E_COMPFILE_DOESNOTEXIST:
      return "The file does not exist";

    case COMADMIN_E_COMPFILE_LOADDLLFAIL:
      return "The DLL could not be loaded";

    case COMADMIN_E_COMPFILE_GETCLASSOBJ:
      return "GetClassObject failed in the DLL";

    case COMADMIN_E_COMPFILE_CLASSNOTAVAIL:
      return "The DLL does not support the components listed in the TypeLib";

    case COMADMIN_E_COMPFILE_BADTLB:
      return "The TypeLib could not be loaded";

    case COMADMIN_E_COMPFILE_NOTINSTALLABLE:
      return "The file does not contain components or component information";

    case COMADMIN_E_NOTCHANGEABLE:
      return "Changes to this object and its sub-objects have been disabled";

    case COMADMIN_E_NOTDELETEABLE:
      return "The delete function has been disabled for this object";

    case COMADMIN_E_SESSION:
      return "The server catalog version is not supported";

    case COMADMIN_E_COMP_MOVE_LOCKED:
      return "The component move was disallowed, because the source or destination application is either a system application or currently locked against changes";

    case COMADMIN_E_COMP_MOVE_BAD_DEST:
      return "The component move failed because the destination application no longer exists";

    case COMADMIN_E_REGISTERTLB:
      return "The system was unable to the TypeLib";

    case COMADMIN_E_SYSTEMAPP:
      return "This operation can not be performed on the system application";

    case COMADMIN_E_COMPFILE_NOREGISTRAR:
      return "The component registrar referenced in this file is not available";

    case COMADMIN_E_COREQCOMPINSTALLED:
      return "A component in the same DLL is already installed";

    case COMADMIN_E_SERVICENOTINSTALLED:
      return "The service is not installed";

    case COMADMIN_E_PROPERTYSAVEFAILED:
      return "One or more property settings are either invalid or in conflict with each other";

    case COMADMIN_E_OBJECTEXISTS:
      return "The object you are attempting to add or rename already exists";

    case COMADMIN_E_COMPONENTEXISTS:
      return "The component already exists";

    case COMADMIN_E_REGFILE_CORRUPT:
      return "The registration file is corrupt";

    case COMADMIN_E_PROPERTY_OVERFLOW:
      return "The property value is too large";

    case COMADMIN_E_NOTINREGISTRY:
      return "Object was not found in registry";

    case COMADMIN_E_OBJECTNOTPOOLABLE:
      return "This object is not poolable";

    case COMADMIN_E_APPLID_MATCHES_CLSID:
      return "A CLSID with the same GUID as the new application ID is already installed on this machine";

    case COMADMIN_E_ROLE_DOES_NOT_EXIST:
      return "A role assigned to a component, interface, or method did not exist in the application";

    case COMADMIN_E_START_APP_NEEDS_COMPONENTS:
      return "You must have components in an application in order to start the application";

    case COMADMIN_E_REQUIRES_DIFFERENT_PLATFORM:
      return "This operation is not enabled on this platform";

    case COMADMIN_E_CAN_NOT_EXPORT_APP_PROXY:
      return "Application Proxy is not exportable";

    case COMADMIN_E_CAN_NOT_START_APP:
      return "Failed to start application because it is either a library application or an application proxy";

    case COMADMIN_E_CAN_NOT_EXPORT_SYS_APP:
      return "System application is not exportable";

    case COMADMIN_E_CANT_SUBSCRIBE_TO_COMPONENT:
      return "Can not subscribe to this component (the component may have been imported)";

    case COMADMIN_E_EVENTCLASS_CANT_BE_SUBSCRIBER:
      return "An event class cannot also be a subscriber component";

    case COMADMIN_E_LIB_APP_PROXY_INCOMPATIBLE:
      return "Library applications and application proxies are incompatible";

    case COMADMIN_E_BASE_PARTITION_ONLY:
      return "This function is valid for the base partition only";

    case COMADMIN_E_START_APP_DISABLED:
      return "You cannot start an application that has been disabled";

    case COMADMIN_E_CAT_DUPLICATE_PARTITION_NAME:
      return "The specified partition name is already in use on this computer";

    case COMADMIN_E_CAT_INVALID_PARTITION_NAME:
      return "The specified partition name is invalid. Check that the name contains at least one visible character";

    case COMADMIN_E_CAT_PARTITION_IN_USE:
      return "The partition cannot be deleted because it is the default partition for one or more users";

    case COMADMIN_E_FILE_PARTITION_DUPLICATE_FILES:
      return "The partition cannot be exported, because one or more components in the partition have the same file name";

    case COMADMIN_E_CAT_IMPORTED_COMPONENTS_NOT_ALLOWED:
      return "Applications that contain one or more imported components cannot be installed into a non-base partition";

    case COMADMIN_E_AMBIGUOUS_APPLICATION_NAME:
      return "The application name is not unique and cannot be resolved to an application id";

    case COMADMIN_E_AMBIGUOUS_PARTITION_NAME:
      return "The partition name is not unique and cannot be resolved to a partition id";

    case COMADMIN_E_REGDB_NOTINITIALIZED:
      return "The COM+ registry database has not been initialized";

    case COMADMIN_E_REGDB_NOTOPEN:
      return "The COM+ registry database is not open";

    case COMADMIN_E_REGDB_SYSTEMERR:
      return "The COM+ registry database detected a system error";

    case COMADMIN_E_REGDB_ALREADYRUNNING:
      return "The COM+ registry database is already running";

    case COMADMIN_E_MIG_VERSIONNOTSUPPORTED:
      return "This version of the COM+ registry database cannot be migrated";

    case COMADMIN_E_MIG_SCHEMANOTFOUND:
      return "The schema version to be migrated could not be found in the COM+ registry database";

    case COMADMIN_E_CAT_BITNESSMISMATCH:
      return "There was a type mismatch between binaries";

    case COMADMIN_E_CAT_UNACCEPTABLEBITNESS:
      return "A binary of unknown or invalid type was provided";

    case COMADMIN_E_CAT_WRONGAPPBITNESS:
      return "There was a type mismatch between a binary and an application";

    case COMADMIN_E_CAT_PAUSE_RESUME_NOT_SUPPORTED:
      return "The application cannot be paused or resumed";

    case COMADMIN_E_CAT_SERVERFAULT:
      return "The COM+ Catalog Server threw an exception during execution";

    case COMQC_E_APPLICATION_NOT_QUEUED:
      return "Only COM+ Applications marked \"queued\" can be invoked using the \"queue\" moniker";

    case COMQC_E_NO_QUEUEABLE_INTERFACES:
      return "At least one interface must be marked \"queued\" in order to create a queued component instance with the \"queue\" moniker";

    case COMQC_E_QUEUING_SERVICE_NOT_AVAILABLE:
      return "MSMQ is required for the requested operation and is not installed";

    case COMQC_E_NO_IPERSISTSTREAM:
      return "Unable to marshal an interface that does not support IPersistStream";

    case COMQC_E_BAD_MESSAGE:
      return "The message is improperly formatted or was damaged in transit";

    case COMQC_E_UNAUTHENTICATED:
      return "An unauthenticated message was received by an application that accepts only authenticated messages";

    case COMQC_E_UNTRUSTED_ENQUEUER:
      return "The message was requeued or moved by a user not in the \"QC Trusted User\" role";

    case MSDTC_E_DUPLICATE_RESOURCE:
      return "Cannot create a duplicate resource of type Distributed Transaction Coordinator";

    case COMADMIN_E_OBJECT_PARENT_MISSING:
      return "One of the objects being inserted or updated does not belong to a valid parent collection";

    case COMADMIN_E_OBJECT_DOES_NOT_EXIST:
      return "One of the specified objects cannot be found";

    case COMADMIN_E_APP_NOT_RUNNING:
      return "The specified application is not currently running";

    case COMADMIN_E_INVALID_PARTITION:
      return "The partition(s) specified are not valid";

    case COMADMIN_E_SVCAPP_NOT_POOLABLE_OR_RECYCLABLE:
      return "COM+ applications that run as NT service may not be pooled or recycled";

    case COMADMIN_E_USER_IN_SET:
      return "One or more users are already assigned to a local partition set";

    case COMADMIN_E_CANTRECYCLELIBRARYAPPS:
      return "Library applications may not be recycled";

    case COMADMIN_E_CANTRECYCLESERVICEAPPS:
      return "Applications running as NT services may not be recycled";

    case COMADMIN_E_PROCESSALREADYRECYCLED:
      return "The process has already been recycled";

    case COMADMIN_E_PAUSEDPROCESSMAYNOTBERECYCLED:
      return "A paused process may not be recycled";

    case COMADMIN_E_CANTMAKEINPROCSERVICE:
      return "Library applications may not be NT services";

    case COMADMIN_E_PROGIDINUSEBYCLSID:
      return "The ProgID provided to the copy operation is invalid. The ProgID is in use by another registered CLSID";

    case COMADMIN_E_DEFAULT_PARTITION_NOT_IN_SET:
      return "The partition specified as default is not a member of the partition set";

    case COMADMIN_E_RECYCLEDPROCESSMAYNOTBEPAUSED:
      return "A recycled process may not be paused";

    case COMADMIN_E_PARTITION_ACCESSDENIED:
      return "Access to the specified partition is denied";

    case COMADMIN_E_PARTITION_MSI_ONLY:
      return "Only Application Files (*.MSI files) can be installed into partitions";

    case COMADMIN_E_LEGACYCOMPS_NOT_ALLOWED_IN_1_0_FORMAT:
      return "Applications containing one or more legacy components may not be exported to 1.0 format";

    case COMADMIN_E_LEGACYCOMPS_NOT_ALLOWED_IN_NONBASE_PARTITIONS:
      return "Legacy components may not exist in non-base partitions";

    case COMADMIN_E_COMP_MOVE_SOURCE:
      return "A component cannot be moved (or copied) from the System Application, an application proxy or a non-changeable application";

    case COMADMIN_E_COMP_MOVE_DEST:
      return "A component cannot be moved (or copied) to the System Application, an application proxy or a non-changeable application";

    case COMADMIN_E_COMP_MOVE_PRIVATE:
      return "A private component cannot be moved (or copied) to a library application or to the base partition";

    case COMADMIN_E_BASEPARTITION_REQUIRED_IN_SET:
      return "The Base Application Partition exists in all partition sets and cannot be removed";

    case COMADMIN_E_CANNOT_ALIAS_EVENTCLASS:
      return "Alas, Event Class components cannot be aliased";

    case COMADMIN_E_PRIVATE_ACCESSDENIED:
      return "Access is denied because the component is private";

    case COMADMIN_E_SAFERINVALID:
      return "The specified SAFER level is invalid";

    case COMADMIN_E_REGISTRY_ACCESSDENIED:
      return "The specified user cannot write to the system registry";

    case COMADMIN_E_PARTITIONS_DISABLED:
      return "COM+ partitions are currently disabled";
  }
}

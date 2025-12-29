#include "netstar-threads.h"

#include <netstar.h>

#include <collections/queue/queue.h>
#include <collections/list/list.h>

#include <collections/map/map.h>

#include <strings/strings.h>
#include <stdlib.h>

#include <sys/time.h>
#include <time.h>


static bool netstar_threads_initialized;

static map_t *netstar_threads;


static int
netstar_threads_compare(const void *thread1, size_t size1, const void *thread2, size_t size2) {
  if ((*(pthread_t *)thread1) < (*(pthread_t *)thread2))
    return -1;
  else if ((*(pthread_t *)thread1) > (*(pthread_t *)thread2))
    return 1;

  return 0;
}

static void
netstar_threads_add(netstar_thread_t *thread) {
  map_insert(
    netstar_threads,
    map_clkey(&thread->self, sizeof(thread->self)),
    map_lvalue(thread, sizeof(netstar_thread_t *))
  );
}

/*
 * @ future feature
 *
static void
netstar_threads_remove(netstar_thread_t *thread) {
  map_remove(
    netstar_threads,
    &thread->self, sizeof(thread->self)
  );
}
*/

static void
netstar_threads_kill_all(void) {
  map_iterator_t threads_iter = NULL;

  for (threads_iter = map_begin(netstar_threads); threads_iter; threads_iter = map_next(threads_iter)) {
    netstar_thread_t *thread = (netstar_thread_t *)map_value(threads_iter);
    netstar_thread_kill(thread);
  }
}


#ifdef NETSTAR_THREADS_POOL
#define NETSTAR_THREAD_POOL_THREADS 2

typedef struct netstar_thread_pool_task {
  netstar_thread_routine_t routine;
  void *args;
} netstar_thread_pool_task_t;

typedef struct netstar_thread_pool {
  netstar_thread_cond_t schedule_notifier, working_notifier, completion_notifier;
  netstar_thread_mutex_t mutex;

  size_t working;
  queue_t *tasks;

  list_t *threads;

// bool sentinel;
  bool status;
} netstar_thread_pool_t;


static netstar_thread_pool_t *netstar_thread_pool;

void netstar_thread_pool_free(netstar_thread_pool_t *);


static void
netstar_thread_pool_work(netstar_thread_t *thread, netstar_thread_pool_task_t *task) {
// @ update current thread arguments to task routine arguments
  thread->args = task->args;

  task->routine(thread);

  free(task);
}


static int netstar_thread_pool_thread_spawn(netstar_thread_pool_t *);


static void *
netstar_thread_pool_context(void *context) {
  netstar_thread_t *thread = (netstar_thread_t *)context;
  netstar_thread_pool_t *thread_pool = (netstar_thread_pool_t *)thread->args;

// struct netstar_thread_cond_timeout timeout = { .sec = 0, .usec = 8000 };

  netstar_thread_pool_task_t *task = NULL;

  for (;;) {
    netstar_thread_mutex_lock(&thread_pool->mutex);

/*
    if (!thread_pool->sentinel) {
      netstar_log("sentinel thread started\r\n");
      thread_pool->sentinel = true;

      while (thread->status && thread_pool->status) {
        if (queue_size(thread_pool->tasks) && thread_pool->working == (list_size(thread_pool->threads)-1)) {
          netstar_thread_pool_thread_spawn(thread_pool);
       // netstar_log("started a new thread\r\n");
        }

        netstar_thread_cond_wait(&thread_pool->working_notifier, &thread_pool->mutex);

     // netstar_thread_cond_timedwait(&thread_pool->schedule_notifier, &thread_pool->mutex, &timeout);
      }
    } else
 */
    {
      while (!queue_size(thread_pool->tasks) && thread->status && thread_pool->status) {
     // netstar_thread_cond_timedwait(&thread_pool->schedule_notifier, &thread_pool->mutex, &timeout);
        netstar_thread_cond_wait(&thread_pool->schedule_notifier, &thread_pool->mutex);
      }
    }

    if (!thread->status || !thread_pool->status) {
      netstar_thread_mutex_unlock(&thread_pool->mutex);
      break;
    }

    task = (netstar_thread_pool_task_t *)queue_pop(thread_pool->tasks);

    thread_pool->working++;
 // netstar_thread_cond_send(&thread_pool->working_notifier);

    netstar_thread_mutex_unlock(&thread_pool->mutex);

    netstar_thread_pool_work(thread, task);

    netstar_thread_mutex_lock(&thread_pool->mutex);

    thread_pool->working--;
    if (!queue_size(thread_pool->tasks) && !thread_pool->working)
      netstar_thread_cond_send(&thread_pool->completion_notifier);

    netstar_thread_mutex_unlock(&thread_pool->mutex);
  }

  netstar_log("#netstar-routine-thread exited.\r\n");
  netstar_thread_exit(thread, NULL);
  return NULL;
}

static void
netstar_thread_pool_add(netstar_thread_pool_t *thread_pool, netstar_thread_routine_t routine, void *args) {
  struct netstar_thread_pool_task task = {
    .routine = routine, .args = args
  };

// struct netstar_thread_cond_timeout timeout = { .sec = 0, .usec = 5000 };

  netstar_thread_mutex_lock(&thread_pool->mutex);

  queue_push(thread_pool->tasks, queue_clvalue(&task, sizeof(task)));
  netstar_thread_cond_send(&thread_pool->schedule_notifier);

//
// @ thread sentinel notifier
//
// netstar_thread_cond_send(&thread_pool->working_notifier);

//
// @ wait for notification from the thread that the task has started
//
/*
  if (!(queue_size(thread_pool->tasks) && thread_pool->working == list_size(thread_pool->threads)))
    netstar_thread_cond_timedwait(&thread_pool->working_notifier, &thread_pool->mutex, &timeout);

  if (queue_size(thread_pool->tasks) && thread_pool->working == list_size(thread_pool->threads)) {
    if (netstar_thread_pool_thread_spawn(thread_pool) == -1)
      goto _return;

    netstar_thread_cond_send(&thread_pool->schedule_notifier);
  }
*/

// _return:
  netstar_thread_mutex_unlock(&thread_pool->mutex);
}

/*
 * void
 * netstar_thread_pool_wait(netstar_thread_pool_t *thread_pool) {
 *   netstar_thread_mutex_lock(&thread_pool->mutex);
 *
 *   for (; queue_size(thread_pool->tasks) || thread_pool->working;) {
 *     netstar_thread_cond_wait(&thread_pool->completion_notifier, &thread_pool->mutex);
 *   }
 *
 *   netstar_thread_mutex_unlock(&thread_pool->mutex);
 * }
 */

static int
netstar_thread_pool_thread_spawn(netstar_thread_pool_t *thread_pool) {
  netstar_thread_t *thread = NULL;

  if (!(thread = netstar_thread_spawn(netstar_thread_pool_context, thread_pool)))
    return -1;

  return list_pushback(thread_pool->threads, list_rvalue(thread, sizeof(thread)));
}

static void
netstar_thread_pool_threads_spawn(netstar_thread_pool_t *thread_pool) {
  size_t threads;

//
// @ sentinel thread
//
// netstar_thread_pool_thread_spawn(thread_pool);

  for (threads = 0; threads < NETSTAR_THREAD_POOL_THREADS; threads++)
    netstar_thread_pool_thread_spawn(thread_pool);
}

static netstar_thread_pool_t *
netstar_thread_pool_new(void) {
  netstar_thread_pool_t *thread_pool = NULL;

  if (!(thread_pool = (netstar_thread_pool_t *)calloc(1, sizeof(netstar_thread_pool_t))))
    goto _return;

  if (!(thread_pool->tasks = queue_new(free)))
    goto _return;

  if (!(thread_pool->threads = list_new(NULL)))
    goto _return;

  thread_pool->status = true;
  netstar_thread_pool_threads_spawn(thread_pool);

  return thread_pool;

_return:
  netstar_thread_pool_free(thread_pool);
  return NULL;
}

/* static */ void
netstar_thread_pool_free(netstar_thread_pool_t *thread_pool) {
  if (thread_pool) {
    if (thread_pool->threads) {
      list_iterator_t threads_iter = NULL;

      thread_pool->status = false;
      netstar_thread_cond_sendall(&thread_pool->schedule_notifier);

      for (threads_iter = list_begin(thread_pool->threads); threads_iter; threads_iter = list_next(threads_iter)) {
        netstar_thread_t *thread = (netstar_thread_t *)list_value(threads_iter);
        netstar_thread_kill(thread);
      }

      list_free(thread_pool->threads);
    }

    queue_free(thread_pool->tasks);

    free(thread_pool);
  }
}

#endif


static void netstar_thread_free(void *);


static netstar_thread_t *
netstar_thread_new(netstar_thread_routine_t routine, void *args) {
  netstar_thread_t *thread = NULL;

  if (!(thread = (netstar_thread_t *)calloc(1, sizeof(netstar_thread_t))))
    goto _return;

  thread->status = true;
  thread->args = args;

  if (pthread_create(&thread->self, NULL, routine, thread) != 0)
    goto _return;

// pthread_detach(thread->self);

  return thread;

_return:
  netstar_thread_free(thread);
  return NULL;
}

static void
netstar_thread_free(void *thread) {
  if (thread) {
    free(thread);
  }
}

netstar_thread_t *
netstar_thread_spawn(netstar_thread_routine_t routine, void *args) {
  netstar_thread_t *thread = NULL;

  if (!netstar_threads || !(thread = netstar_thread_new(routine, args)))
    return NULL;

  netstar_threads_add(thread);
  return thread;
}

#ifdef NETSTAR_THREADS_POLL
void
netstar_thread_routine(netstar_thread_routine_t routine, void *args) {
  if (!netstar_thread_pool)
    return;

  netstar_thread_pool_add(netstar_thread_pool, routine, args);
}
#endif

netstar_thread_t *
netstar_thread_self(void) {
  pthread_t thread = pthread_self();
  return (netstar_thread_t *)map_getor(netstar_threads, &thread, sizeof(thread), NULL);
}

void
netstar_thread_join(netstar_thread_t *thread, void **return_value) {
  pthread_join(thread->self, return_value);
}

void
netstar_thread_exit(netstar_thread_t *thread, void *return_value) {
  thread->status = false;

// netstar_threads_remove(thread);

  pthread_exit(return_value);
}

void
netstar_thread_mutex_lock(netstar_thread_mutex_t *mutex) {
  pthread_mutex_lock(mutex);
}

void
netstar_thread_mutex_unlock(netstar_thread_mutex_t *mutex) {
  pthread_mutex_unlock(mutex);
}

void
netstar_thread_cond_timedwait(netstar_thread_cond_t *cond, netstar_thread_mutex_t *mutex, struct netstar_thread_cond_timeout *timeout) {
  struct timespec timespec = {0};
  struct timeval timeval = {0};

  gettimeofday(&timeval, NULL);
// clock_gettime(CLOCK_REALTIME, &timespec);
  timespec.tv_sec += timeval.tv_sec+timeout->sec;
  timespec.tv_nsec += (timeval.tv_usec+timeout->usec)*1000;

  pthread_cond_timedwait(cond, mutex, &timespec);
}

void
netstar_thread_cond_wait(netstar_thread_cond_t *cond, netstar_thread_mutex_t *mutex) {
  pthread_cond_wait(cond, mutex);
}

void
netstar_thread_cond_sendall(netstar_thread_cond_t *cond) {
  pthread_cond_broadcast(cond);
}

void
netstar_thread_cond_send(netstar_thread_cond_t *cond) {
  pthread_cond_signal(cond);
}

void
netstar_thread_cond_destroy(netstar_thread_cond_t *cond) {
  pthread_cond_destroy(cond);
}

void
netstar_thread_kill(netstar_thread_t *thread) {
  if (thread && !thread->killed) {
    thread->status = false;
    netstar_thread_join(thread, NULL);

 // netstar_threads_remove(thread);

    thread->killed = true;
  }
}

void
netstar_threads_initialize(void) {
  if (netstar_threads_initialized)
    return;

  if (!(netstar_threads = map_new(netstar_threads_compare, free, netstar_thread_free)))
    return;

  netstar_threads_initialized = true;

#ifdef NETSTAR_THREADS_POLL
  if (!(netstar_thread_pool = netstar_thread_pool_new()))
    return;
#endif
}

void
netstar_threads_deinitialize(void) {
  if (!netstar_threads_initialized)
    return;

#ifdef NETSTAR_THREADS_POLL
  netstar_thread_pool_free(netstar_thread_pool);
  netstar_thread_pool = NULL;
#endif

  netstar_threads_kill_all();

  map_free(netstar_threads);
  netstar_threads = NULL;

  netstar_threads_initialized = false;
}

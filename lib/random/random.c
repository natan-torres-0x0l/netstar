#include "random.h"

#if defined _WIN16 || defined _WIN32 || defined _WIN64 || defined __WIN32__ || defined __TOS_WIN__ || defined __WINDOWS__
  #include <windows.h>

#elif defined __linux__ || defined __linux || defined __unix__ || defined __unix
  #include <unistd.h>

#endif

#include <time.h>


/* KISS 64-bit algorithm */

#define RANDOMICAL_NUMBER_CLASS_X_8  0x6E9352043E925CFEUL
#define RANDOMICAL_NUMBER_CLASS_Y_16 0x755765312B8C48B8UL
#define RANDOMICAL_NUMBER_CLASS_W_32 0xD83B59849DA7668CUL
#define RANDOMICAL_NUMBER_CLASS_Z_64 0x9FD1D725E22071E6UL


static struct { uint64_t x, y, w, z, n; } random_state;


__attribute__((__constructor__))
static void
random_state_initializer(void) {
#if defined _WIN16 || defined _WIN32 || defined _WIN64 || defined __WIN32__ || defined __TOS_WIN__ || defined __WINDOWS__
  random_state.x = (uint64_t)(RANDOMICAL_NUMBER_CLASS_X_8+(uint64_t)GetCurrentProcessId());
  random_state.y = (uint64_t)(RANDOMICAL_NUMBER_CLASS_Y_16+(uint64_t)GetCurrentThreadId());

#elif defined __linux__ || defined __linux || defined __unix__ || defined __unix
  random_state.x = (uint64_t)(RANDOMICAL_NUMBER_CLASS_X_8+(uint64_t)getpid());
  random_state.y = (uint64_t)(RANDOMICAL_NUMBER_CLASS_Y_16+(uint64_t)getppid());

#endif

  random_state.w = (uint64_t)(RANDOMICAL_NUMBER_CLASS_W_32+(uint64_t)time(NULL));
  random_state.z = (uint64_t)(RANDOMICAL_NUMBER_CLASS_Z_64+(uint64_t)clock());
}

static uint64_t
random_next(void) {
  random_state.n = (random_state.x << 58) + random_state.y;
  random_state.y = random_state.x >> 6;
  random_state.x += random_state.n;
  random_state.y += (random_state.x < random_state.n);

  random_state.w ^= random_state.w << 13;
  random_state.w ^= random_state.w >> 17;
  random_state.w ^= random_state.w << 43;

  random_state.z = 6906969069UL * random_state.z + 1234567;

  return random_state.x + random_state.y + random_state.z;
}

uint64_t
random_uint64(void) {
  return random_next();
}

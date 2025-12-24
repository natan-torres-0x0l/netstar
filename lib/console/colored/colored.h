#ifndef _CONSOLE_COLORED_COLORED_H
#define _CONSOLE_COLORED_COLORED_H

#ifdef __cplusplus
extern "C" {
#endif

// #define COLORED_TRUECOLOR_SUPPORT

#include <console/console.h>

#include <stdint.h>


typedef enum {
  COLORED_CONTEXT_FOREGROUND = 38, COLORED_CONTEXT_BACKGROUND = 48, COLORED_CONTEXT_UNDEFINED = -1,
} colored_context_t;

#define COLORED_FOREGROUND COLORED_CONTEXT_FOREGROUND
#define COLORED_BACKGROUND COLORED_CONTEXT_BACKGROUND
#define COLORED_UNDEFINED  COLORED_CONTEXT_UNDEFINED

typedef uint32_t colored_truecolor_t;
typedef uint8_t  colored_color256_t;

#ifdef COLORED_TRUECOLOR_SUPPORT
typedef colored_truecolor_t colored_color_t;

#elif defined COLORED_ANSI256_SUPPORT
typedef colored_color256_t  colored_color_t;

#endif

typedef uint32_t colored_color_t;


extern const char *colored_color256(colored_context_t, colored_color256_t);
extern const char *colored_color16(colored_context_t, colored_color_t);
extern const char *colored_color(const char *);

extern const char *colored_rgb256(const char *);
extern const char *colored_rgb(const char *);

extern const char *colored_style(const char *);

extern const char *colored_reset(void);

#ifdef __cplusplus
}
#endif

#endif

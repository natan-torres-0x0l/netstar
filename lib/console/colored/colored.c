#include "colored.h"

#include <collections/map/map.h>

#include <strings/strings.h>
#include <stdio.h>

#include <stdlib.h>
#include <stddef.h>

#include <inttypes.h>

#include <math.h>


struct colored_color {
  char *foreground, *background;
  colored_color_t code;
};


static void colored_color_free(void *);


static int
colored_color_compare(const void *value1, size_t value1_size, const void *value2, size_t value2_size) {
  struct colored_color *color1 = (struct colored_color *)value1;
  struct colored_color *color2 = (struct colored_color *)value2;

  if (color1->code < color2->code)
    return -1;
  else if (color1->code > color2->code)
    return 1;

  return 1;
}

struct colored_rgb {
  union {
    struct {
#if __BYTE_ORDER == __LITTLE_ENDIAN || defined __LITTLE_ENDIAN || defined __LITTLE_ENDIAN_BITFIELD
      uint8_t blue,
              green,
              red,
              reserved;
#elif __BYTE_ORDER == __BIG_ENDIAN || defined __BIG_ENDIAN || defined __BIG_ENDIAN_BITFIELD
      uint8_t reserved,
              red,
              green,
              blue;
#else
  #error "Adjust your <asm/byteorder.h> defines"

#endif
    } /* channel */;

    uint32_t u32;
  } /* color */;
};


static inline char *
colored_ansi256_format(colored_context_t context, colored_color_t code) {
  return string_format(CONSOLE_ASCII_ESCAPE "[%d;5;%" PRIu8 "m", context, code);
}

static struct colored_color *
colored_color256_new(colored_color_t code) {
  struct colored_color *color = NULL;

  if (!(color = (struct colored_color *)calloc(1, sizeof(struct colored_color))))
    goto _return;

  if (!(color->foreground = colored_ansi256_format(COLORED_FOREGROUND, code)))
    goto _return;
  if (!(color->background = colored_ansi256_format(COLORED_BACKGROUND, code)))
    goto _return;

  color->code = code;

  return color;

_return:
  colored_color_free(color);
  return NULL;
}

static void
colored_color_free(void *value) {
  if (value) {
    struct colored_color *color = (struct colored_color *)value;

    if (color->foreground)
      free(color->foreground), color->foreground = NULL;
    if (color->background)
      free(color->background), color->background = NULL;

 // color->code = 0;

    free(color);
  }
}

static inline char *
colored_truecolor_format(colored_context_t context, const struct colored_rgb *rgb) {
  return string_format(CONSOLE_ASCII_ESCAPE "[%d;2;%" PRIx8 ";%" PRIx8 ";%" PRIx8 "m", context, rgb->red, rgb->green, rgb->blue);
}

static struct colored_color *
colored_truecolor_new(const struct colored_rgb *rgb) {
  struct colored_color *color = NULL;

  if (!(color = (struct colored_color *)calloc(1, sizeof(struct colored_color))))
    goto _return;

  if (!(color->foreground = colored_truecolor_format(COLORED_FOREGROUND, rgb)))
    goto _return;
  if (!(color->background = colored_truecolor_format(COLORED_BACKGROUND, rgb)))
    goto _return;

  color->code = rgb->u32;

  return color;

_return:
  colored_color_free(color);
  return NULL;
}


typedef map_t colored_palette_t;


static inline int
colored_palette_insert(colored_palette_t *palette, struct colored_color *color) {
  return map_insert(palette,
    map_clkey(&color->code, sizeof(color->code)),
    map_lvalue(color, sizeof(struct colored_color *))
  );
}

static struct colored_color *
colored_palette_color256(colored_palette_t *palette, colored_color_t code) {
  struct colored_color *color = NULL;

  if ((color = map_get(palette, &code, sizeof(colored_color_t))))
    return color;

  if (!(color = colored_color256_new(code)))
    goto _return;

  if (colored_palette_insert(palette, color) == -1)
    goto _return;

  return color;

_return:
  colored_color_free(color);
  return NULL;
}

static struct colored_color *
colored_palette_truecolor(colored_palette_t *palette, const struct colored_rgb *rgb) {
  struct colored_color *color = NULL;

  if ((color = map_get(palette, &rgb->u32, sizeof(colored_color_t))))
    return color;

  if (!(color = colored_truecolor_new(rgb)))
    goto _return;

  if (colored_palette_insert(palette, color) == -1)
    goto _return;

  return color;

_return:
  colored_color_free(color);
  return NULL;
}

static inline colored_palette_t *
colored_palette_new(void) {
  return map_new(colored_color_compare, free, colored_color_free);
}

static inline void
colored_palette_free(colored_palette_t *palette) {
  map_free(palette);
}


static colored_palette_t *colored_palette = NULL;
static bool colored_palette_initialized;


__attribute__((__constructor__))
static void
colored_palette_initialize(void) {
  if (colored_palette_initialized)
    return;

  colored_palette = colored_palette_new();
  colored_palette_initialized = true;
}

__attribute__((__destructor__))
static void
colored_palette_deinitialize(void) {
  colored_palette_free(colored_palette);
  colored_palette = NULL;

  colored_palette_initialized = false;
}


static colored_context_t
colored_context(const char *string) {
  return ((*string && *string == '#') ? COLORED_FOREGROUND : ((*string && *string == '@') ? COLORED_BACKGROUND : COLORED_UNDEFINED));
}

static const char *
colored_colorize(colored_context_t context, const struct colored_color *color) {
  return ((context == COLORED_FOREGROUND) ? color->foreground : ((context == COLORED_BACKGROUND) ? color->background : colored_reset()));
}


struct colored_color16 {
  const char *foreground, *background;
  colored_color_t code;

  const char *name;
};


static const struct colored_color16 colored_colors16[] = {
  { CONSOLE_ASCII_ESCAPE "[30m", CONSOLE_ASCII_ESCAPE "[40m" , 0x00 /*  "0m" */, "black"         },
  { CONSOLE_ASCII_ESCAPE "[31m", CONSOLE_ASCII_ESCAPE "[41m" , 0x01 /*  "1m" */, "red"           },
  { CONSOLE_ASCII_ESCAPE "[32m", CONSOLE_ASCII_ESCAPE "[42m" , 0x02 /* " 2m" */, "green"         },
  { CONSOLE_ASCII_ESCAPE "[33m", CONSOLE_ASCII_ESCAPE "[43m" , 0x03 /*  "3m" */, "yellow"        },
  { CONSOLE_ASCII_ESCAPE "[34m", CONSOLE_ASCII_ESCAPE "[44m" , 0x04 /*  "4m" */, "blue"          },
  { CONSOLE_ASCII_ESCAPE "[35m", CONSOLE_ASCII_ESCAPE "[45m" , 0x05 /*  "5m" */, "magenta"       },
  { CONSOLE_ASCII_ESCAPE "[36m", CONSOLE_ASCII_ESCAPE "[46m" , 0x06 /*  "6m" */, "cyan"          },
  { CONSOLE_ASCII_ESCAPE "[37m", CONSOLE_ASCII_ESCAPE "[47m" , 0x07 /*  "7m" */, "white"         },
  { CONSOLE_ASCII_ESCAPE "[90m", CONSOLE_ASCII_ESCAPE "[100m", 0x08 /*  "8m" */, "light-gray"    },
  { CONSOLE_ASCII_ESCAPE "[91m", CONSOLE_ASCII_ESCAPE "[101m", 0x09 /*  "9m" */, "light-red"     },
  { CONSOLE_ASCII_ESCAPE "[92m", CONSOLE_ASCII_ESCAPE "[102m", 0x0A /* "10m" */, "light-green"   },
  { CONSOLE_ASCII_ESCAPE "[93m", CONSOLE_ASCII_ESCAPE "[103m", 0x0B /* "11m" */, "light-yellow"  },
  { CONSOLE_ASCII_ESCAPE "[94m", CONSOLE_ASCII_ESCAPE "[104m", 0x0C /* "12m" */, "light-blue"    },
  { CONSOLE_ASCII_ESCAPE "[95m", CONSOLE_ASCII_ESCAPE "[105m", 0x0D /* "13m" */, "light-magenta" },
  { CONSOLE_ASCII_ESCAPE "[96m", CONSOLE_ASCII_ESCAPE "[106m", 0x0E /* "14m" */, "light-cyan"    },
  { CONSOLE_ASCII_ESCAPE "[97m", CONSOLE_ASCII_ESCAPE "[107m", 0x0F /* "15m" */, "light-white"   },
  { NULL                       , NULL                        , 0x00            , NULL            }
};


static bool
colored_is_ansi16(colored_color_t code) {
/*
  if (code >= 0x1E && code <= 0x25)
    return true;
  if (code >= 0x28 && code <= 0x2F)
    return true;

  if (code >= 0x5A && code <= 0x61)
    return true;
  if (code >= 0x64 && code <= 0x6B)
    return true;
*/

  return (code <= 0x0F);
}

static const struct colored_color *
colored_color16_by_name(const char *name) {
  const struct colored_color16 *colors = NULL;

  for (colors = colored_colors16; colors->name; colors++)
    if (string_equals(name, colors->name, false))
      return (struct colored_color *)colors;

  return NULL;
}

const char *
colored_color(const char *string) {
  const struct colored_color *color = NULL;
  colored_context_t context;

  if ((context = colored_context(string++)) == COLORED_UNDEFINED)
    return colored_reset();

  if (!(color = colored_color16_by_name(string)))
    return colored_reset();

  return colored_colorize(context, color);
}

static inline const struct colored_color *
colored_color16_by_code(colored_color_t code) {
  return (colored_is_ansi16(code) ? (struct colored_color *)&colored_colors16[code] : NULL);
}

const char *
colored_color16(colored_context_t context, colored_color_t code) {
  const struct colored_color *color = NULL;

  if (!(color = colored_color16_by_code(code)))
    return colored_reset();

  return colored_colorize(context, color);
}


// static bool
// colored_is_ansi256(colored_color_t color) {
//   return (color <= 0xFF);
// }

const char *
colored_color256(colored_context_t context, colored_color256_t code) {
  const struct colored_color *color = NULL;

  if (colored_is_ansi16(code) && (color = colored_color16_by_code(code)))
    return colored_colorize(context, color);

  if ((color = colored_palette_color256(colored_palette, code)))
    return colored_colorize(context, color);

  return colored_reset();
}


__attribute__((__unused__))
static bool
colored_is_truecolor(colored_color_t code) {
  return (sizeof(colored_color_t) == sizeof(uint32_t) && code > 0x000000FF);
}

static colored_color256_t
colored_rgb_to_color256(struct colored_rgb *rgb) {
  if (rgb->green == rgb->red && rgb->green == rgb->blue)
    return ((rgb->red > 248) ? 231 : ((rgb->red < 8) ? 16 : (colored_color256_t)(round(((rgb->red-8)/247.0)*24)+232.0)));
  
  return (colored_color256_t)(16+(36*round(rgb->red/255.0*5))+(6*round(rgb->green/255.0*5))+round(rgb->blue/255.0*5));
}

static inline int
colored_rgb_new(struct colored_rgb *rgb, const char *string) {
  return ((sscanf(string, "#%02" SCNx8 "%02" SCNx8 "%02" SCNx8, &rgb->red, &rgb->green, &rgb->blue) == 3) ? 1 : -1);
}

const char *
colored_rgb256(const char *string) {
  struct colored_rgb rgb = {0};
  colored_context_t context;

  if ((context = colored_context(string++)) == -1 || colored_rgb_new(&rgb, string) == -1)
    return colored_reset();

  return colored_color256(context, colored_rgb_to_color256(&rgb));
}

const char *
colored_rgb(const char *string) {
  const struct colored_color *color = NULL;
  struct colored_rgb rgb = {0};
  colored_context_t context;

  if ((context = colored_context(string++)) == -1 || colored_rgb_new(&rgb, string) == -1)
    return colored_reset();

  if (!(color = colored_palette_truecolor(colored_palette, &rgb)))
    return colored_reset();

  return colored_colorize(context, color);
}


typedef uint8_t colored_style_t;

struct colored_style {
  const char *stylized;
  colored_style_t code;
  const char *name;
};


static const struct colored_style colored_styles[] = {
  { CONSOLE_ASCII_ESCAPE "[1m" , 0x01 /*  "1m" */, "bold"             },
  { CONSOLE_ASCII_ESCAPE "[22m", 0x16 /* "22m" */, "no-bold"          },
  { CONSOLE_ASCII_ESCAPE "[2m" , 0x02 /*  "2m" */, "shadow"           },
  { CONSOLE_ASCII_ESCAPE "[22m", 0x16 /* "22m" */, "no-shadow"        },
  { CONSOLE_ASCII_ESCAPE "[3m" , 0x03 /*  "3m" */, "italic"           },
  { CONSOLE_ASCII_ESCAPE "[23m", 0x17 /* "23m" */, "no-italic"        },
  { CONSOLE_ASCII_ESCAPE "[4m" , 0x04 /*  "4m" */, "underline"        },
  { CONSOLE_ASCII_ESCAPE "[24m", 0x18 /* "24m" */, "no-underline"     },
  { CONSOLE_ASCII_ESCAPE "[5m" , 0x05 /*  "5m" */, "blinking"         },
  { CONSOLE_ASCII_ESCAPE "[25m", 0x19 /* "25m" */, "no-blinking"      },
  { CONSOLE_ASCII_ESCAPE "[6m" , 0x07 /*  "7m" */, "negative"         },
  { CONSOLE_ASCII_ESCAPE "[27m", 0x1B /* "27m" */, "no-negative"      },
  { CONSOLE_ASCII_ESCAPE "[8m" , 0x08 /*  "8m" */, "invisible"        },
  { CONSOLE_ASCII_ESCAPE "[28m", 0x1C /* "28m" */, "no-invisible"     },
  { CONSOLE_ASCII_ESCAPE "[9m" , 0x09 /*  "9m" */, "strikethrough"    },
  { CONSOLE_ASCII_ESCAPE "[29m", 0x1D /* "29m" */, "no-strikethrough" },
  { NULL                       , 0x00            , NULL               },
};


static const struct colored_style *
colored_style_by_name(const char *name) {
  const struct colored_style *styles = NULL;

  for (styles = colored_styles; styles->name; styles++)
    if (string_equals(name, styles->name, false))
      return styles;

  return NULL;
}

const char *
colored_style(const char *string) {
  const struct colored_style *style = NULL;

  if (!(style = colored_style_by_name(string)))
    return colored_reset();

  return style->stylized;
}


const char *
colored_reset(void) {
  return CONSOLE_ASCII_ESCAPE "[0m";
}

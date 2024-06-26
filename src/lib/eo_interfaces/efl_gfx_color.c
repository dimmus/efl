#include "efl_config.h"
#include "Efl_Eo_Interfaces.h"

static int
_hex_string_get(char ch, Efl_Bool *ok)
{
  if ((ch >= '0') && (ch <= '9')) return (ch - '0');
  else if ((ch >= 'A') && (ch <= 'F')) return (ch - 'A' + 10);
  else if ((ch >= 'a') && (ch <= 'f')) return (ch - 'a' + 10);
  *ok = EFL_FALSE;
  return 0;
}

/**
 * @internal
 * Parses a string of one of the formas:
 * 1. "#RRGGBB"
 * 2. "#RRGGBBAA"
 * 3. "#RGB"
 * 4. "#RGBA"
 * To the rgba values.
 *
 * @param[in] str The string to parse - NOT NULL.
 * @param[out] r The Red value - NOT NULL.
 * @param[out] g The Green value - NOT NULL.
 * @param[out] b The Blue value - NOT NULL.
 * @param[out] a The Alpha value - NOT NULL.
 */
static Efl_Bool
_format_color_parse(const char    *str,
                    int            slen,
                    unsigned char *r,
                    unsigned char *g,
                    unsigned char *b,
                    unsigned char *a)
{
  Efl_Bool v = EFL_TRUE;

  *r = *g = *b = *a = 0;

  if (slen == 7) /* #RRGGBB */
  {
    *r = (_hex_string_get(str[1], &v) << 4) | (_hex_string_get(str[2], &v));
    *g = (_hex_string_get(str[3], &v) << 4) | (_hex_string_get(str[4], &v));
    *b = (_hex_string_get(str[5], &v) << 4) | (_hex_string_get(str[6], &v));
    *a = 0xff;
  }
  else if (slen == 9) /* #RRGGBBAA */
  {
    *r = (_hex_string_get(str[1], &v) << 4) | (_hex_string_get(str[2], &v));
    *g = (_hex_string_get(str[3], &v) << 4) | (_hex_string_get(str[4], &v));
    *b = (_hex_string_get(str[5], &v) << 4) | (_hex_string_get(str[6], &v));
    *a = (_hex_string_get(str[7], &v) << 4) | (_hex_string_get(str[8], &v));
  }
  else if (slen == 4) /* #RGB */
  {
    *r = _hex_string_get(str[1], &v);
    *r = (*r << 4) | *r;
    *g = _hex_string_get(str[2], &v);
    *g = (*g << 4) | *g;
    *b = _hex_string_get(str[3], &v);
    *b = (*b << 4) | *b;
    *a = 0xff;
  }
  else if (slen == 5) /* #RGBA */
  {
    *r = _hex_string_get(str[1], &v);
    *r = (*r << 4) | *r;
    *g = _hex_string_get(str[2], &v);
    *g = (*g << 4) | *g;
    *b = _hex_string_get(str[3], &v);
    *b = (*b << 4) | *b;
    *a = _hex_string_get(str[4], &v);
    *a = (*a << 4) | *a;
  }
  else v = EFL_FALSE;

  *r = (*r * *a) / 255;
  *g = (*g * *a) / 255;
  *b = (*b * *a) / 255;
  return v;
}

EOLIAN static void
_efl_gfx_color_color_code_set(Eo         *obj,
                              void *_pd   EFL_UNUSED,
                              const char *colorcode)
{
  int           len;
  unsigned char r, g, b, a;

  len = (size_t)strlen(colorcode);

  _format_color_parse(colorcode, len, &r, &g, &b, &a);
  efl_gfx_color_set(obj, r, g, b, a);
}

EOLIAN static const char *
_efl_gfx_color_color_code_get(const Eo *obj, void *_pd EFL_UNUSED)
{
  int r, g, b, a;

  efl_gfx_color_get(obj, &r, &g, &b, &a);
  return eina_slstr_printf("#%02X%02X%02X%02X", r, g, b, a);
}

EOLIAN static Efl_Bool
_efl_gfx_color_class_color_class_code_set(Eo                       *obj,
                                          void *_pd                 EFL_UNUSED,
                                          const char               *color_class,
                                          Efl_Gfx_Color_Class_Layer layer,
                                          const char               *colorcode)
{
  int           len;
  unsigned char r, g, b, a;

  len = (size_t)strlen(colorcode);

  _format_color_parse(colorcode, len, &r, &g, &b, &a);
  return efl_gfx_color_class_set(obj, color_class, layer, r, g, b, a);
}

EOLIAN static const char *
_efl_gfx_color_class_color_class_code_get(const Eo *obj             EFL_UNUSED,
                                          void *pd                  EFL_UNUSED,
                                          const char               *color_class,
                                          Efl_Gfx_Color_Class_Layer layer)
{
  int r, g, b, a;

  if (efl_gfx_color_class_get(obj, color_class, layer, &r, &g, &b, &a))
    return eina_slstr_printf("#%02X%02X%02X%02X", r, g, b, a);
  return NULL;
}

#include "efl_gfx_color.eo.c"
#include "efl_gfx_color_class.eo.c"

#ifndef _EFL_EO_INTERFACES_H
#define _EFL_EO_INTERFACES_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <Efl_Eo.h>

#ifdef EAPI
#  undef EAPI
#endif
#ifdef EWAPI
#  undef EWAPI
#endif
#ifdef EOAPI
#  undef EOAPI
#endif

#ifdef _WIN32
#  ifdef EFL_BUILD
#    ifdef DLL_EXPORT
#      define EAPI __declspec(dllexport)
#    else
#      define EAPI
#    endif
#  else
#    define EAPI __declspec(dllimport)
#  endif
#  define EAPI_WEAK
#else
#  ifdef __GNUC__
#    if __GNUC__ >= 4
#      define EAPI __attribute__ ((visibility("default")))
#      define EAPI_WEAK __attribute__ ((weak))
#    else
#      define EAPI
#      define EAPI_WEAK
#    endif
#  else
#    define EAPI
#    define EAPI_WEAK
#  endif
#endif

#define EWAPI EAPI EAPI_WEAK
#define EOAPI EAPI EAPI_WEAK

#define EFL_VERSION_1_18 1
#define EFL_VERSION_1_19 1
#define EFL_VERSION_1_20 1
#define EFL_VERSION_1_21 1
#define EFL_VERSION_1_22 1
#define EFL_VERSION_1_23 1
#define EFL_VERSION_1_24 1
#define EFL_VERSION_1_25 1
#define EFL_VERSION_1_26 1
#define EFL_VERSION_1_27 1

/* Add here all the required ifdef for any @protected method */
#ifdef EFL_BUILD
#  define EFL_PACK_LAYOUT_PROTECTED
#  define EFL_GFX_HINT_PROTECTED
#endif

/**
 * @ingroup Efl
 * @since 1.18
 *
 * This type is a alias to struct tm.
 * It is intended to be a standard way to reference it in .eo files.
 */
typedef struct tm Efl_Time;

typedef struct _Efl_Text_Cursor_Handle    Efl_Text_Cursor_Handle;
typedef struct _Efl_Text_Cursor_Handle    _Efl_Text_Cursor_Handle;
typedef struct _Efl_Text_Attribute_Handle Efl_Text_Attribute_Handle;

#include "efl_types.eot.h"

#include "Efl_Model.h"

/* Data types */

#ifdef EFL_BETA_API_SUPPORT
/** No error on load */
extern EWAPI Eina_Error EFL_GFX_IMAGE_LOAD_ERROR_NONE;

/** A non-specific error occurred */
extern EWAPI Eina_Error EFL_GFX_IMAGE_LOAD_ERROR_GENERIC;

/** File (or file path) does not exist */
extern EWAPI Eina_Error EFL_GFX_IMAGE_LOAD_ERROR_DOES_NOT_EXIST;

/** Permission denied to an existing file (or path) */
extern EWAPI Eina_Error EFL_GFX_IMAGE_LOAD_ERROR_PERMISSION_DENIED;

/** Allocation of resources failure prevented load */
extern EWAPI Eina_Error EFL_GFX_IMAGE_LOAD_ERROR_RESOURCE_ALLOCATION_FAILED;

/** File corrupt (but was detected as a known format) */
extern EWAPI Eina_Error EFL_GFX_IMAGE_LOAD_ERROR_CORRUPT_FILE;

/** File is not a known format */
extern EWAPI Eina_Error EFL_GFX_IMAGE_LOAD_ERROR_UNKNOWN_FORMAT;

/** Reading operation has been cancelled during decoding */
extern EWAPI Eina_Error EFL_GFX_IMAGE_LOAD_ERROR_CANCELLED;

/** (Edje only) The file pointed to is incompatible, i.e., it doesn't
 * match the library's current version's format. */
extern EWAPI Eina_Error EFL_GFX_IMAGE_LOAD_ERROR_INCOMPATIBLE_FILE;

/** (Edje only) The group/collection set to load from was not found in the file */
extern EWAPI Eina_Error EFL_GFX_IMAGE_LOAD_ERROR_UNKNOWN_COLLECTION;

/** (Edje only) The group/collection set to load from had recursive references
 * on its components */
extern EWAPI Eina_Error EFL_GFX_IMAGE_LOAD_ERROR_RECURSIVE_REFERENCE;
#endif /* EFL_BETA_API_SUPPORT */

#include "efl_gfx_types.eot.h"
#include "efl_ui_drag_types.eot.h"
typedef Efl_Gfx_Path_Command_Type Efl_Gfx_Path_Command;

/* Interfaces */
#include "efl_config.eo.h"
#include "efl_control.eo.h"
#include "efl_duplicate.eo.h"
#include "efl_file.h"
#include "efl_file.eo.h"
#include "efl_file_save.eo.h"
#include "efl_gfx_image.eo.h"
#include "efl_gfx_frame_controller.eo.h"
#include "efl_gfx_image_load_controller.eo.h"
#include "efl_gfx_image_orientable.eo.h"
#include "efl_part.eo.h"
#include "efl_playable.eo.h"
#include "efl_player.eo.h"
#include "efl_audio_control.eo.h"
#include "efl_text.eo.h"
#include "efl_text_types.eot.h"
#include "efl_ui_i18n.eo.h"
#include "efl_ui_layout_orientable.eo.h"
#include "efl_ui_layout_orientable_readonly.eo.h"
#include "efl_ui_drag.eo.h"
#include "efl_ui_range_display.eo.h"
#include "efl_ui_range_interactive.eo.h"
#include "efl_ui_autorepeat.eo.h"
#include "efl_ui_draggable.eo.h"
#include "efl_ui_scrollable.eo.h"
#include "efl_ui_scrollbar.eo.h"
#include "efl_ui_container_selectable.eo.h"
#include "efl_ui_zoom.eo.h"

#include "efl_screen.eo.h"

/* Core interface */
#include "efl_interpolator.eo.h"

/* Graphics */
#include "efl_gfx_entity.eo.h"
#include "efl_gfx_color.eo.h"
#include "efl_gfx_buffer.eo.h"
#include "efl_gfx_stack.eo.h"
#include "efl_gfx_fill.eo.h"
#include "efl_gfx_view.eo.h"
#include "efl_gfx_path.eo.h"
#include "efl_gfx_shape.eo.h"
#include "efl_gfx_gradient.eo.h"
#include "efl_gfx_gradient_linear.eo.h"
#include "efl_gfx_gradient_radial.eo.h"
#include "efl_gfx_filter.eo.h"
#include "efl_gfx_blur.eo.h"
#include "efl_gfx_hint.eo.h"
#include "efl_gfx_color_class.eo.h"
#include "efl_gfx_text_class.eo.h"
#include "efl_gfx_size_class.eo.h"

/* Canvas & UI */
#include "efl_ui_view.eo.h"
#include "efl_ui_property_bind.eo.h"
#include "efl_ui_factory.eo.h"
#include "efl_ui_view_factory.eo.h"
#include "efl_ui_factory_bind.eo.h"
#include "efl_model_provider.eo.h"
#include "efl_cached_item.eo.h"

/* Observable interface */
#include "efl_observer.eo.h"
#include "efl_observable.eo.h"

/* Packing & containers */
#include "efl_container.eo.h"
#include "efl_content.eo.h"
#include "efl_pack.eo.h"
#include "efl_pack_layout.eo.h"
#include "efl_pack_linear.eo.h"
#include "efl_pack_table.eo.h"
#include "efl_gfx_arrangement.eo.h"

/* Input and Output */
#include "efl_io_closer.eo.h"
#include "efl_io_reader.eo.h"
#include "efl_io_writer.eo.h"
#include "efl_io_sizer.eo.h"
#include "efl_io_positioner.eo.h"

#include "efl_io_buffer.eo.h"
#include "efl_io_queue.eo.h"

/* Text interfaces */
#include "efl_text_font_properties.eo.h"
#include "efl_text_style.eo.h"
#include "efl_text_format.eo.h"
#include "efl_text_markup.eo.h"
#include "efl_text_markup_util.eo.h"
#include "efl_input_text_entity.eo.h"

/**
 * @brief Get a proxy object referring to a part of an object.
 *
 * The returned object is valid for only a single function call.
 * Of course, if the first call is @ref efl_ref, it will last
 * until @ref efl_unref.
 *
 * @param[in] obj The object.
 * @param[in] name The part name.
 *
 * @return A (proxy) object, valid for a single call.
 *
 * @since 1.21
 */
EAPI Efl_Object *efl_part(const Eo *obj, const char *name);

#ifdef EFL_BETA_API_SUPPORT

EAPI void efl_observable_tuple_free(Efl_Observable_Tuple *tuple);

static inline Efl_Bool
efl_config_bool_set(Efl_Config *obj, const char *name, Efl_Bool val)
{
  Eina_Value *v = eina_value_new(EINA_VALUE_TYPE_UCHAR);
  Efl_Bool    b;
  eina_value_set(v, val);
  b = efl_config_set(obj, name, v);
  eina_value_free(v);
  return b;
}

static inline Efl_Bool
efl_config_bool_get(const Efl_Config *obj, const char *name)
{
  Eina_Value *v = efl_config_get(obj, name);
  Efl_Bool    b = 0;
  if (v && eina_value_type_get(v) == EINA_VALUE_TYPE_UCHAR)
    eina_value_get(v, &b);
  eina_value_free(v);
  return b;
}

static inline Efl_Bool
efl_config_int_set(Efl_Config *obj, const char *name, int val)
{
  Eina_Value *v = eina_value_new(EINA_VALUE_TYPE_INT);
  Efl_Bool    b;
  eina_value_set(v, val);
  b = efl_config_set(obj, name, v);
  eina_value_free(v);
  return b;
}

static inline int
efl_config_int_get(const Efl_Config *obj, const char *name)
{
  Eina_Value *v = efl_config_get(obj, name);
  int         b = 0;
  if (v && eina_value_type_get(v) == EINA_VALUE_TYPE_INT) eina_value_get(v, &b);
  eina_value_free(v);
  return b;
}

static inline Efl_Bool
efl_config_double_set(Efl_Config *obj, const char *name, double val)
{
  Eina_Value *v = eina_value_new(EINA_VALUE_TYPE_DOUBLE);
  Efl_Bool    b;
  eina_value_set(v, val);
  b = efl_config_set(obj, name, v);
  eina_value_free(v);
  return b;
}

static inline double
efl_config_double_get(const Efl_Config *obj, const char *name)
{
  Eina_Value *v = efl_config_get(obj, name);
  double      b = 0;
  if (v && eina_value_type_get(v) == EINA_VALUE_TYPE_DOUBLE)
    eina_value_get(v, &b);
  eina_value_free(v);
  return b;
}

static inline Efl_Bool
efl_config_string_set(Efl_Config *obj, const char *name, const char *val)
{
  Eina_Value *v = eina_value_new(EINA_VALUE_TYPE_STRING);
  Efl_Bool    b;
  eina_value_set(v, val);
  b = efl_config_set(obj, name, v);
  eina_value_free(v);
  return b;
}

static inline Eina_Stringshare *
efl_config_string_get(const Efl_Config *obj, const char *name)
{
  Eina_Value       *v = efl_config_get(obj, name);
  Eina_Stringshare *s = 0;
  if (v && eina_value_type_get(v) == EINA_VALUE_TYPE_STRING)
  {
    const char *b = 0;
    eina_value_get(v, &b);
    s = eina_stringshare_add(b);
  }
  eina_value_free(v);
  return s;
}

#else

#  ifndef EFL_NOLEGACY_API_SUPPORT
#    include "efl_gfx_types.eot.h"
#    include "efl_ui_drag_types.eot.h"
#    include "efl_text_types.eot.h"
#  endif

#endif

/* work-around bug in gcc --as-needed link optimization */
EAPI void __efl_internal_init(void);

#if defined(__cplusplus)
}
#endif

#undef EAPI
#define EAPI

#endif

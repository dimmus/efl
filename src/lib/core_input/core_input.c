#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include <stdio.h>
#include <string.h>

#include "Efl_Core.h"
#include "Efl_Core_Input.h"

#include "core_private.h"
#include "core_input_private.h"

int _core_input_log_dom = -1;

EAPI int CORE_EVENT_KEY_DOWN            = 0;
EAPI int CORE_EVENT_KEY_UP              = 0;
EAPI int CORE_EVENT_MOUSE_BUTTON_DOWN   = 0;
EAPI int CORE_EVENT_MOUSE_BUTTON_UP     = 0;
EAPI int CORE_EVENT_MOUSE_MOVE          = 0;
EAPI int CORE_EVENT_MOUSE_WHEEL         = 0;
EAPI int CORE_EVENT_MOUSE_IN            = 0;
EAPI int CORE_EVENT_MOUSE_OUT           = 0;
EAPI int CORE_EVENT_AXIS_UPDATE         = 0;
EAPI int CORE_EVENT_MOUSE_BUTTON_CANCEL = 0;
EAPI int CORE_EVENT_JOYSTICK            = 0;

static int _core_event_init_count = 0;

EAPI int
core_event_init(void)
{
  if (++_core_event_init_count != 1) return _core_event_init_count;
  if (!core_init())
  {
    _core_event_init_count--;
    return 0;
  }

  _core_input_log_dom =
    eina_log_domain_register("ecore_input", ECORE_INPUT_DEFAULT_LOG_COLOR);
  if (_core_input_log_dom < 0)
  {
    EINA_LOG_ERR(
      "Impossible to create a log domain for the ecore input module.");
    return --_core_event_init_count;
  }

  CORE_EVENT_KEY_DOWN            = core_event_type_new();
  CORE_EVENT_KEY_UP              = core_event_type_new();
  CORE_EVENT_MOUSE_BUTTON_DOWN   = core_event_type_new();
  CORE_EVENT_MOUSE_BUTTON_UP     = core_event_type_new();
  CORE_EVENT_MOUSE_MOVE          = core_event_type_new();
  CORE_EVENT_MOUSE_WHEEL         = core_event_type_new();
  CORE_EVENT_MOUSE_IN            = core_event_type_new();
  CORE_EVENT_MOUSE_OUT           = core_event_type_new();
  CORE_EVENT_AXIS_UPDATE         = core_event_type_new();
  CORE_EVENT_MOUSE_BUTTON_CANCEL = core_event_type_new();
  CORE_EVENT_JOYSTICK            = core_event_type_new();

  ecore_input_joystick_init();

  return _core_event_init_count;
}

EAPI int
core_event_shutdown(void)
{
  if (--_core_event_init_count != 0) return _core_event_init_count;

  core_event_type_flush(CORE_EVENT_KEY_DOWN,
                        CORE_EVENT_KEY_UP,
                        CORE_EVENT_MOUSE_BUTTON_DOWN,
                        CORE_EVENT_MOUSE_BUTTON_UP,
                        CORE_EVENT_MOUSE_MOVE,
                        CORE_EVENT_MOUSE_WHEEL,
                        CORE_EVENT_MOUSE_IN,
                        CORE_EVENT_MOUSE_OUT,
                        CORE_EVENT_AXIS_UPDATE,
                        CORE_EVENT_MOUSE_BUTTON_CANCEL,
                        CORE_EVENT_JOYSTICK);
  ecore_input_joystick_shutdown();
  eina_log_domain_unregister(_core_input_log_dom);
  _core_input_log_dom = -1;
  core_shutdown();
  return _core_event_init_count;
}

typedef struct _Ecore_Event_Modifier_Match Ecore_Event_Modifier_Match;

struct _Ecore_Event_Modifier_Match
{
  const char          *key;
  Ecore_Event_Modifier modifier;
  unsigned int         event_modifier;
};

static const Ecore_Event_Modifier_Match matchs[] = {
  {          "Shift_L",  ECORE_SHIFT,  CORE_EVENT_MODIFIER_SHIFT },
  {          "Shift_R",  ECORE_SHIFT,  CORE_EVENT_MODIFIER_SHIFT },
  {            "Alt_L",    ECORE_ALT,    CORE_EVENT_MODIFIER_ALT },
  {            "Alt_R",    ECORE_ALT,    CORE_EVENT_MODIFIER_ALT },
  {        "Control_L",   ECORE_CTRL,   CORE_EVENT_MODIFIER_CTRL },
  {        "Control_R",   ECORE_CTRL,   CORE_EVENT_MODIFIER_CTRL },
  {        "Caps_Lock",   ECORE_CAPS,   CORE_EVENT_MODIFIER_CAPS },
  {          "Super_L",    ECORE_WIN,    CORE_EVENT_MODIFIER_WIN },
  {          "Super_R",    ECORE_WIN,    CORE_EVENT_MODIFIER_WIN },
  { "ISO_Level3_Shift",   ECORE_MODE,  CORE_EVENT_MODIFIER_ALTGR },
  {      "Scroll_Lock", ECORE_SCROLL, CORE_EVENT_MODIFIER_SCROLL }
};

EAPI unsigned int
core_event_modifier_mask(Ecore_Event_Modifier modifier)
{
  size_t i;

  for (i = 0; i < sizeof(matchs) / sizeof(Ecore_Event_Modifier_Match); i++)
    if (matchs[i].modifier == modifier) return matchs[i].event_modifier;

  return 0;
}

EAPI Ecore_Event_Modifier
core_event_update_modifier(const char            *key,
                           Ecore_Event_Modifiers *modifiers,
                           int                    inc)
{
  size_t i;

  for (i = 0; i < sizeof(matchs) / sizeof(Ecore_Event_Modifier_Match); i++)
    if (strcmp(matchs[i].key, key) == 0)
    {
      if (modifiers && matchs[i].modifier < modifiers->size)
        modifiers->array[matchs[i].modifier] += inc;
      return matchs[i].modifier;
    }

  return ECORE_NONE;
}

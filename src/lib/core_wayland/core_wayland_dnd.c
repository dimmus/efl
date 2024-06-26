/*
 * Copyright © 2008 Kristian Høgsberg
 * Copyright © 2012-2013 Collabora, Ltd.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include "core_wayland_private.h"

struct _dnd_task
{
  void      *data;
  Core_Fd_Cb cb;
};

struct _dnd_read_ctx
{
  int                 epoll_fd;
  struct epoll_event *ep;
};

struct _Efl_Core_Wayland_Offer
{
  Efl_Core_Wayland_Input      *input;
  struct wl_data_offer        *offer;
  Eina_Array                  *mimetypes;
  Efl_Core_Wayland_Drag_Action actions;
  Efl_Core_Wayland_Drag_Action action;
  uint32_t                     serial;
  Eina_List                   *reads;
  int                          ref;
  Efl_Core_Wayland_Window     *window;
  Efl_Bool                     proxied : 1;
};

Efl_Core_Wayland_Window *
_win_id_get(Efl_Core_Wayland_Input *input)
{
  Efl_Core_Wayland_Window *win = NULL;

  if (input->focus.pointer) win = input->focus.pointer;
  else if (input->focus.prev_pointer) win = input->focus.prev_pointer;
  else if (input->focus.keyboard) win = input->focus.keyboard;

  return win;
}

static void
data_source_target_free(void *data EFL_UNUSED, void *event)
{
  Efl_Core_Wayland_Event_Data_Source_Target *ev;

  ev = event;
  if (!ev) return;

  free(ev->type);
  efl_core_wayland_display_disconnect(ev->display);
  free(ev);
}

static void
data_source_target(void                         *data,
                   struct wl_data_source *source EFL_UNUSED,
                   const char                   *mime_type)
{
  Efl_Core_Wayland_Input                    *input;
  Efl_Core_Wayland_Event_Data_Source_Target *ev;

  input = data;
  if (!input) return;

  ev = calloc(1, sizeof(Efl_Core_Wayland_Event_Data_Source_Target));
  if (!ev) return;
  ev->seat    = input->id;
  ev->display = input->display;
  ev->display->refs++;

  if (mime_type) ev->type = strdup(mime_type);

  core_event_add(EFL_CORE_WAYLAND_EVENT_DATA_SOURCE_TARGET,
                 ev,
                 data_source_target_free,
                 NULL);
}

static void
data_source_send_free(void *data EFL_UNUSED, void *event)
{
  Efl_Core_Wayland_Event_Data_Source_Send *ev;

  ev = event;
  if (!ev) return;

  free(ev->type);
  efl_core_wayland_display_disconnect(ev->display);
  free(ev);
}

static void
data_source_send(void                  *data,
                 struct wl_data_source *source,
                 const char            *mime_type,
                 int32_t                fd)
{
  Efl_Core_Wayland_Input                  *input;
  Efl_Core_Wayland_Event_Data_Source_Send *ev;

  input = data;
  if (!input) return;

  ev = calloc(1, sizeof(Efl_Core_Wayland_Event_Data_Source_Send));
  if (!ev) return;

  ev->fd   = fd;
  ev->type = strdup(mime_type);
  ev->seat = input->id;
  if (source == input->data.selection.source)
    ev->serial = input->data.selection.serial;
  else ev->serial = input->data.drag.serial;
  ev->display = input->display;
  ev->display->refs++;

  core_event_add(EFL_CORE_WAYLAND_EVENT_DATA_SOURCE_SEND,
                 ev,
                 data_source_send_free,
                 NULL);
}

static void
event_fill(struct _Efl_Core_Wayland_Event_Data_Source_Event *ev,
           Efl_Core_Wayland_Input                           *input)
{
  if (input->focus.keyboard) ev->source = input->focus.keyboard;

  ev->win     = _win_id_get(input);
  ev->action  = input->data.drag.action;
  ev->seat    = input->id;
  ev->serial  = input->data.drag.serial;
  ev->display = input->display;
  ev->display->refs++;
}

static void
data_source_event_emit(Efl_Core_Wayland_Input *input,
                       int                     event,
                       Efl_Bool                cancel)
{
  struct _Efl_Core_Wayland_Event_Data_Source_Event *ev;
  Efl_Core_Wayland_Event_Data_Source_End           *ev2 = NULL;

  if (event == EFL_CORE_WAYLAND_EVENT_DATA_SOURCE_END)
  {
    ev2 = calloc(1, sizeof(Efl_Core_Wayland_Event_Data_Source_End));
    ev  = (void *)ev2;
  }
  else ev = calloc(1, sizeof(struct _Efl_Core_Wayland_Event_Data_Source_Event));
  EINA_SAFETY_ON_NULL_RETURN(ev);

  event_fill((void *)ev, input);
  if (event == EFL_CORE_WAYLAND_EVENT_DATA_SOURCE_END) ev2->cancelled = cancel;

  core_event_add(event, ev, _display_event_free, ev->display);
}

static void
data_source_cancelled(void *data, struct wl_data_source *source)
{
  Efl_Core_Wayland_Input *input = data;

  if (input->data.drag.source == source) input->data.drag.source = NULL;
  if (input->data.selection.source == source)
    input->data.selection.source = NULL;
  input->data.drag.action = WL_DATA_DEVICE_MANAGER_DND_ACTION_NONE;
  wl_data_source_destroy(source);
  data_source_event_emit(input, EFL_CORE_WAYLAND_EVENT_DATA_SOURCE_END, 1);
}

static void
data_source_dnd_drop_performed(void                         *data,
                               struct wl_data_source *source EFL_UNUSED)
{
  Efl_Core_Wayland_Input *input = data;
  data_source_event_emit(input, EFL_CORE_WAYLAND_EVENT_DATA_SOURCE_DROP, 0);
}

static void
data_source_dnd_finished(void *data, struct wl_data_source *source)
{
  Efl_Core_Wayland_Input *input = data;

  if (input->data.drag.source == source) input->data.drag.source = NULL;
  if (input->data.selection.source == source)
    input->data.selection.source = NULL;
  wl_data_source_destroy(source);
  data_source_event_emit(input, EFL_CORE_WAYLAND_EVENT_DATA_SOURCE_END, 0);
}

static void
data_source_action(void                         *data,
                   struct wl_data_source *source EFL_UNUSED,
                   uint32_t                      dnd_action)
{
  Efl_Core_Wayland_Input *input = data;

  input->data.drag.action = dnd_action;
  data_source_event_emit(input, EFL_CORE_WAYLAND_EVENT_DATA_SOURCE_ACTION, 0);
}

static const struct wl_data_source_listener _source_listener = {
  data_source_target,       data_source_send,
  data_source_cancelled,    data_source_dnd_drop_performed,
  data_source_dnd_finished, data_source_action,
};

static void
_unset_serial(void *user_data, void *event)
{
  Efl_Core_Wayland_Offer           *offer = user_data;
  Efl_Core_Wayland_Event_Dnd_Enter *ev    = event;

  if (offer) offer->serial = 0;
  efl_core_wayland_display_disconnect(ev->display);
  free(event);
}

void
_efl_core_wayland_dnd_enter(Efl_Core_Wayland_Input *input,
                            struct wl_data_offer   *offer,
                            struct wl_surface      *surface,
                            int                     x,
                            int                     y,
                            uint32_t                serial)
{
  Efl_Core_Wayland_Window          *window;
  Efl_Core_Wayland_Event_Dnd_Enter *ev;

  window =
    _efl_core_wayland_display_window_surface_find(input->display, surface);
  if (!window) return;

  if (offer)
  {
    input->drag.offer = wl_data_offer_get_user_data(offer);

    if (input->drag.offer)
    {
      input->drag.offer->serial = serial;
      input->drag.offer->window = window;

      if (input->display->wl.data_device_manager_version >=
          WL_DATA_OFFER_SET_ACTIONS_SINCE_VERSION)
        efl_core_wayland_offer_actions_set(input->drag.offer,
                                           EFL_CORE_WAYLAND_DRAG_ACTION_MOVE |
                                             EFL_CORE_WAYLAND_DRAG_ACTION_COPY,
                                           EFL_CORE_WAYLAND_DRAG_ACTION_MOVE);
    }
  }
  else input->drag.offer = NULL;

  input->drag.enter_serial = serial;
  input->drag.window       = window;

  ev = calloc(1, sizeof(Efl_Core_Wayland_Event_Dnd_Enter));
  if (!ev) return;

  if (input->focus.keyboard) ev->source = input->focus.keyboard;
  ev->win = input->drag.window;

  ev->x       = x;
  ev->y       = y;
  ev->offer   = input->drag.offer;
  ev->seat    = input->id;
  ev->display = input->display;
  ev->display->refs++;

  core_event_add(EFL_CORE_WAYLAND_EVENT_DND_ENTER,
                 ev,
                 _unset_serial,
                 input->drag.offer);
}

static void
_delay_offer_destroy(void *user_data, void *event)
{
  Efl_Core_Wayland_Offer           *offer = user_data;
  Efl_Core_Wayland_Event_Dnd_Leave *ev    = event;

  if (offer) _efl_core_wayland_offer_unref(offer);
  efl_core_wayland_display_disconnect(ev->display);
  free(event);
}

void
_efl_core_wayland_dnd_leave(Efl_Core_Wayland_Input *input)
{
  Efl_Core_Wayland_Event_Dnd_Leave *ev;

  EINA_SAFETY_ON_TRUE_RETURN(!input->drag.enter_serial);

  ev = calloc(1, sizeof(Efl_Core_Wayland_Event_Dnd_Leave));
  if (!ev) return;

  if (input->focus.keyboard) ev->source = input->focus.keyboard;

  ev->win   = input->drag.window;
  ev->offer = input->drag.offer;
  if (ev->offer) ev->offer->ref++;
  ev->seat    = input->id;
  ev->display = input->display;
  ev->display->refs++;

  input->drag.window       = NULL;
  input->drag.enter_serial = 0;
  input->drag.offer        = NULL;
  core_event_add(EFL_CORE_WAYLAND_EVENT_DND_LEAVE,
                 ev,
                 _delay_offer_destroy,
                 ev->offer);
}

void
_efl_core_wayland_dnd_motion(Efl_Core_Wayland_Input *input,
                             int                     x,
                             int                     y,
                             uint32_t                serial)
{
  Efl_Core_Wayland_Event_Dnd_Motion *ev;

  input->pointer.sx = x;
  input->pointer.sy = y;

  ev = calloc(1, sizeof(Efl_Core_Wayland_Event_Dnd_Motion));
  if (!ev) return;

  if (input->drag.offer) input->drag.offer->serial = serial;

  if (input->focus.keyboard) ev->source = input->focus.keyboard;

  ev->win     = input->drag.window;
  ev->x       = x;
  ev->y       = y;
  ev->offer   = input->drag.offer;
  ev->seat    = input->id;
  ev->display = input->display;
  ev->display->refs++;

  core_event_add(EFL_CORE_WAYLAND_EVENT_DND_MOTION,
                 ev,
                 _unset_serial,
                 input->drag.offer);
}

void
_efl_core_wayland_dnd_drop(Efl_Core_Wayland_Input *input)
{
  Efl_Core_Wayland_Event_Dnd_Drop *ev;

  ev = calloc(1, sizeof(Efl_Core_Wayland_Event_Dnd_Drop));
  if (!ev) return;

  if (input->focus.keyboard) ev->source = input->focus.keyboard;

  ev->win     = input->drag.window;
  ev->x       = input->pointer.sx;
  ev->y       = input->pointer.sy;
  ev->offer   = input->drag.offer;
  ev->seat    = input->id;
  ev->display = input->display;
  ev->display->refs++;

  core_event_add(EFL_CORE_WAYLAND_EVENT_DND_DROP,
                 ev,
                 _display_event_free,
                 ev->display);
}

void
_efl_core_wayland_dnd_selection(Efl_Core_Wayland_Input *input,
                                struct wl_data_offer   *offer)
{
  Efl_Core_Wayland_Event_Seat_Selection *ev;

  if (input->selection.offer)
    _efl_core_wayland_offer_unref(input->selection.offer);
  input->selection.offer = NULL;

  if (offer) input->selection.offer = wl_data_offer_get_user_data(offer);
  input->selection.enter_serial = input->display->serial;
  ev = malloc(sizeof(Efl_Core_Wayland_Event_Seat_Selection));
  EINA_SAFETY_ON_NULL_RETURN(ev);
  ev->seat    = input->id;
  ev->display = input->display;
  ev->display->refs++;
  core_event_add(EFL_CORE_WAYLAND_EVENT_SEAT_SELECTION,
                 ev,
                 _display_event_free,
                 ev->display);
}

void
_efl_core_wayland_dnd_del(Efl_Core_Wayland_Dnd_Source *source)
{
  if (!source) return;
  if (source->fdh)
  {
    int fd;

    fd = core_main_fd_handler_fd_get(source->fdh);
    if (fd >= 0) close(core_main_fd_handler_fd_get(source->fdh));
    core_main_fd_handler_del(source->fdh);
  }
  if (source->offer)
  {
    wl_data_offer_destroy(source->offer);
    source->offer = NULL;
  }
  wl_array_release(&source->types);
  free(source);
}

EAPI void
efl_core_wayland_dnd_drag_types_set(Efl_Core_Wayland_Input *input,
                                    const char            **types)
{
  struct wl_data_device_manager *manager;
  const char                   **type;
  char                         **t;

  EINA_SAFETY_ON_NULL_RETURN(input);
  EINA_SAFETY_ON_NULL_RETURN(input->display);

  manager = input->display->wl.data_device_manager;
  if (!manager) return;

  if (input->data.drag.types.data)
  {
    wl_array_for_each(t, &input->data.drag.types) free(*t);
    wl_array_release(&input->data.drag.types);
    wl_array_init(&input->data.drag.types);
  }

  if (input->data.drag.source) wl_data_source_destroy(input->data.drag.source);
  input->data.drag.source = NULL;

  input->data.drag.source = wl_data_device_manager_create_data_source(manager);
  if (!input->data.drag.source)
  {
    ERR("Could not create data source");
    return;
  }

  for (type = types; *type; type++)
  {
    t = wl_array_add(&input->data.drag.types, sizeof(*t));
    if (t)
    {
      *t = strdup(*type);
      wl_data_source_offer(input->data.drag.source, *t);
    }
  }
}

EAPI uint32_t
efl_core_wayland_dnd_drag_start(Efl_Core_Wayland_Input  *input,
                                Efl_Core_Wayland_Window *window,
                                Efl_Core_Wayland_Window *drag_window)
{
  struct wl_surface *dsurface = NULL, *osurface;

  EINA_SAFETY_ON_NULL_RETURN_VAL(input, 0);
  EINA_SAFETY_ON_NULL_RETURN_VAL(input->data.drag.source, 0);

  if (drag_window) dsurface = efl_core_wayland_window_surface_get(drag_window);

  _efl_core_wayland_input_ungrab(input);

  wl_data_source_add_listener(input->data.drag.source,
                              &_source_listener,
                              input);

  osurface = efl_core_wayland_window_surface_get(window);
  if (osurface)
  {
    if (input->display->wl.data_device_manager_version >=
        WL_DATA_SOURCE_SET_ACTIONS_SINCE_VERSION)
      wl_data_source_set_actions(input->data.drag.source,
                                 WL_DATA_DEVICE_MANAGER_DND_ACTION_MOVE |
                                   WL_DATA_DEVICE_MANAGER_DND_ACTION_COPY);

    wl_data_device_start_drag(input->data.device,
                              input->data.drag.source,
                              osurface,
                              dsurface,
                              input->display->serial);
    input->data.drag.serial = input->display->serial;

    efl_core_wayland_input_cursor_from_name_set(input, "move");
  }
  return input->data.drag.serial;
}

EAPI void
efl_core_wayland_dnd_set_actions(Efl_Core_Wayland_Input *input)
{
  EINA_SAFETY_ON_NULL_RETURN(input);
  EINA_SAFETY_ON_NULL_RETURN(input->data.drag.source);
  EINA_SAFETY_ON_NULL_RETURN(input->data.drag.types.data);
  if (input->display->wl.data_device_manager_version >=
      WL_DATA_SOURCE_SET_ACTIONS_SINCE_VERSION)
    wl_data_source_set_actions(input->data.drag.source,
                               WL_DATA_DEVICE_MANAGER_DND_ACTION_MOVE |
                                 WL_DATA_DEVICE_MANAGER_DND_ACTION_COPY);
}

EAPI void
efl_core_wayland_dnd_drag_end(Efl_Core_Wayland_Input *input)
{
  Efl_Core_Wayland_Event_Dnd_End *ev;

  EINA_SAFETY_ON_NULL_RETURN(input);

  if (input->data.drag.types.data)
  {
    char **t;

    wl_array_for_each(t, &input->data.drag.types) free(*t);
    wl_array_release(&input->data.drag.types);
    wl_array_init(&input->data.drag.types);
  }

  ev = calloc(1, sizeof(Efl_Core_Wayland_Event_Dnd_End));
  if (!ev) return;

  if (input->focus.keyboard) ev->source = input->focus.keyboard;

  ev->win     = _win_id_get(input);
  ev->seat    = input->id;
  ev->display = input->display;
  ev->display->refs++;

  core_event_add(EFL_CORE_WAYLAND_EVENT_DND_END,
                 ev,
                 _display_event_free,
                 ev->display);
}

EAPI Efl_Core_Wayland_Offer *
efl_core_wayland_dnd_selection_get(Efl_Core_Wayland_Input *input)
{
  EINA_SAFETY_ON_NULL_RETURN_VAL(input, NULL);

  return input->selection.offer;
}

EAPI uint32_t
efl_core_wayland_dnd_selection_set(Efl_Core_Wayland_Input *input,
                                   const char            **types)
{
  struct wl_data_device_manager *manager;
  const char                   **type;
  char                         **t;

  EINA_SAFETY_ON_NULL_RETURN_VAL(input, 0);
  EINA_SAFETY_ON_NULL_RETURN_VAL(input->display, 0);

  manager = input->display->wl.data_device_manager;
  if (!manager) return 0;

  if (input->data.selection.types.data)
  {
    wl_array_for_each(t, &input->data.selection.types) free(*t);
    wl_array_release(&input->data.selection.types);
    wl_array_init(&input->data.selection.types);
  }

  input->data.selection.source = NULL;

  if (!types[0]) return 0;

  input->data.selection.source =
    wl_data_device_manager_create_data_source(manager);
  if (!input->data.selection.source)
  {
    ERR("Could not create data source");
    return 0;
  }

  for (type = types; *type; type++)
  {
    t = wl_array_add(&input->data.selection.types, sizeof(*t));
    if (t)
    {
      *t = strdup(*type);
      wl_data_source_offer(input->data.selection.source, *t);
    }
  }

  wl_data_source_add_listener(input->data.selection.source,
                              &_source_listener,
                              input);

  wl_data_device_set_selection(input->data.device,
                               input->data.selection.source,
                               input->display->serial);
  input->data.selection.serial = input->display->serial;
  return input->display->serial;
}

EAPI uint32_t
efl_core_wayland_dnd_selection_clear(Efl_Core_Wayland_Input *input)
{
  EINA_SAFETY_ON_NULL_RETURN_VAL(input, 0);
  EINA_SAFETY_ON_NULL_RETURN_VAL(input->data.device, 0);

  wl_data_device_set_selection(input->data.device,
                               NULL,
                               input->display->serial);
  input->data.selection.serial = 0;
  return input->display->serial;
}

static Efl_Core_Wayland_Drag_Action
_wl_to_action_convert(uint32_t action)
{
#define PAIR(wl, ac) \
    if (action == wl) return ac;
  PAIR(WL_DATA_DEVICE_MANAGER_DND_ACTION_COPY,
       EFL_CORE_WAYLAND_DRAG_ACTION_COPY)
  PAIR(WL_DATA_DEVICE_MANAGER_DND_ACTION_ASK, EFL_CORE_WAYLAND_DRAG_ACTION_ASK)
  PAIR(WL_DATA_DEVICE_MANAGER_DND_ACTION_MOVE,
       EFL_CORE_WAYLAND_DRAG_ACTION_MOVE)
  PAIR(WL_DATA_DEVICE_MANAGER_DND_ACTION_NONE,
       EFL_CORE_WAYLAND_DRAG_ACTION_NONE)
#undef PAIR
  return EFL_CORE_WAYLAND_DRAG_ACTION_NONE;
}

static uint32_t
_action_to_wl_convert(Efl_Core_Wayland_Drag_Action action)
{
#define PAIR(wl, ac) \
    if (action == ac) return wl;
  PAIR(WL_DATA_DEVICE_MANAGER_DND_ACTION_COPY,
       EFL_CORE_WAYLAND_DRAG_ACTION_COPY)
  PAIR(WL_DATA_DEVICE_MANAGER_DND_ACTION_ASK, EFL_CORE_WAYLAND_DRAG_ACTION_ASK)
  PAIR(WL_DATA_DEVICE_MANAGER_DND_ACTION_MOVE,
       EFL_CORE_WAYLAND_DRAG_ACTION_MOVE)
  PAIR(WL_DATA_DEVICE_MANAGER_DND_ACTION_NONE,
       EFL_CORE_WAYLAND_DRAG_ACTION_NONE)
#undef PAIR
  return WL_DATA_DEVICE_MANAGER_DND_ACTION_NONE;
}

static void
data_offer_offer(void                               *data,
                 struct wl_data_offer *wl_data_offer EFL_UNUSED,
                 const char                         *type)
{
  Efl_Core_Wayland_Offer *offer = data;
  char                   *str;

  if (type) eina_array_push(offer->mimetypes, strdup(type)); /*LEEEAK */
  else
  {
    while ((str = eina_array_pop(offer->mimetypes)))
    {
      free(str);
    }
  }
}

static void
data_offer_source_actions(void                               *data,
                          struct wl_data_offer *wl_data_offer EFL_UNUSED,
                          uint32_t                            source_actions)
{
  Efl_Core_Wayland_Offer *offer;
  unsigned int            i;
  uint32_t                types[] = { WL_DATA_DEVICE_MANAGER_DND_ACTION_MOVE,
                                      WL_DATA_DEVICE_MANAGER_DND_ACTION_COPY,
                                      WL_DATA_DEVICE_MANAGER_DND_ACTION_ASK,
                                      WL_DATA_DEVICE_MANAGER_DND_ACTION_NONE };

  offer = data;

  offer->actions = 0;

  for (i = 0; types[i] != 0; ++i)
  {
    if (source_actions & types[i])
      offer->actions |= _wl_to_action_convert(types[i]);
  }
}

static void
data_offer_action(void                               *data,
                  struct wl_data_offer *wl_data_offer EFL_UNUSED,
                  uint32_t                            dnd_action)
{
  Efl_Core_Wayland_Offer *offer;

  offer         = data;
  offer->action = _wl_to_action_convert(dnd_action);
}

static const struct wl_data_offer_listener _offer_listener = {
  data_offer_offer,
  data_offer_source_actions,
  data_offer_action
};

void
_efl_core_wayland_dnd_add(Efl_Core_Wayland_Input *input,
                          struct wl_data_offer   *offer)
{
  Efl_Core_Wayland_Offer *result;

  result            = calloc(1, sizeof(Efl_Core_Wayland_Offer));
  result->offer     = offer;
  result->input     = input;
  result->mimetypes = eina_array_new(10);
  result->ref       = 1;

  wl_data_offer_add_listener(offer, &_offer_listener, result);
}

EAPI Efl_Core_Wayland_Drag_Action
efl_core_wayland_offer_actions_get(Efl_Core_Wayland_Offer *offer)
{
  return offer->actions;
}

EAPI void
efl_core_wayland_offer_actions_set(Efl_Core_Wayland_Offer      *offer,
                                   Efl_Core_Wayland_Drag_Action actions,
                                   Efl_Core_Wayland_Drag_Action action)
{
  uint32_t val = 0;
  int      i   = 0;

  EINA_SAFETY_ON_NULL_RETURN(offer);

  for (i = 0; i < EFL_CORE_WAYLAND_DRAG_ACTION_LAST; ++i)
  {
    if (actions & i) val |= _action_to_wl_convert(i);
  }

  offer->action = _action_to_wl_convert(action);

  wl_data_offer_set_actions(offer->offer, val, offer->action);
}

EAPI Efl_Core_Wayland_Drag_Action
efl_core_wayland_offer_action_get(Efl_Core_Wayland_Offer *offer)
{
  EINA_SAFETY_ON_NULL_RETURN_VAL(offer, EFL_CORE_WAYLAND_DRAG_ACTION_NONE);
  return offer->action;
}

EAPI Eina_Array *
efl_core_wayland_offer_mimes_get(Efl_Core_Wayland_Offer *offer)
{
  EINA_SAFETY_ON_NULL_RETURN_VAL(offer, NULL);
  return offer->mimetypes;
}

static unsigned char
_emit_mime(const void *container EFL_UNUSED, void *elem, void *data)
{
  Efl_Core_Wayland_Offer *offer = data;

  wl_data_offer_accept(offer->offer, offer->serial, elem);

  return 1;
}

EAPI void
efl_core_wayland_offer_mimes_set(Efl_Core_Wayland_Offer *offer,
                                 Eina_Array             *mimes)
{
  EINA_SAFETY_ON_NULL_RETURN(offer);

  if (mimes) eina_array_foreach(mimes, _emit_mime, offer);
  else wl_data_offer_accept(offer->offer, offer->serial, NULL);
}

EAPI void
efl_core_wayland_offer_accept(Efl_Core_Wayland_Offer *offer,
                              const char             *mime_type)
{
  EINA_SAFETY_ON_NULL_RETURN(offer);

  wl_data_offer_accept(offer->offer, offer->serial, mime_type);
}

typedef struct
{
  int                     len;
  void                   *data;
  char                   *mimetype;
  Efl_Core_Wayland_Offer *offer;
} Read_Buffer;

static void
_free_buf(void *user_data, void *event)
{
  Read_Buffer                             *buf = user_data;
  Efl_Core_Wayland_Event_Offer_Data_Ready *ev  = event;

  _efl_core_wayland_offer_unref(buf->offer);

  free(buf->data);
  free(buf->mimetype);
  free(user_data);
  efl_core_wayland_display_disconnect(ev->display);
  free(event);
}

static Efl_Bool
_offer_receive_fd_cb(void *data, Core_Fd_Handler *fdh)
{
  Read_Buffer *buf = data;
  int          fd  = -1;
  char         buffer[255];
  int          len;

  fd = core_main_fd_handler_fd_get(fdh);
  if (fd >= 0) len = read(fd, buffer, sizeof(buffer));
  else return CORE_CALLBACK_RENEW;

  if (len > 0)
  {
    int old_len = buf->len;

    buf->len += len;
    buf->data = realloc(buf->data, buf->len);

    memcpy(((char *)buf->data) + old_len, buffer, len);
    return CORE_CALLBACK_RENEW;
  }
  else
  {
    Efl_Core_Wayland_Event_Offer_Data_Ready *ev;

    ev        = calloc(1, sizeof(Efl_Core_Wayland_Event_Offer_Data_Ready));
    ev->offer = buf->offer;

    ev->data     = buf->data;
    ev->len      = buf->len;
    ev->mimetype = buf->mimetype;
    ev->seat     = buf->offer->input->id;
    ev->display  = buf->offer->input->display;
    ev->display->refs++;
    core_event_add(EFL_CORE_WAYLAND_EVENT_OFFER_DATA_READY, ev, _free_buf, buf);

    buf->offer->reads = eina_list_remove(buf->offer->reads, fdh);
    return CORE_CALLBACK_CANCEL;
  }
}

EAPI void
efl_core_wayland_offer_receive(Efl_Core_Wayland_Offer *offer, char *mime)
{
  Read_Buffer     *buffer;
  Core_Fd_Handler *handler;
  int              pipe[2];

  EINA_SAFETY_ON_NULL_RETURN(offer);

  if (pipe2(pipe, O_CLOEXEC) == -1)
  {
    ERR("Failed to create pipe for receiving");
    return;
  }

  buffer           = calloc(1, sizeof(Read_Buffer));
  buffer->offer    = offer;
  buffer->mimetype = strdup(mime);

  offer->ref++;  // we are keeping this ref until the read is done AND emitted

  wl_data_offer_receive(offer->offer, mime, pipe[1]);
  close(pipe[1]);

  handler = core_main_fd_handler_file_add(pipe[0],
                                          CORE_FD_READ | CORE_FD_ERROR,
                                          _offer_receive_fd_cb,
                                          buffer,
                                          NULL,
                                          NULL);

  offer->reads = eina_list_append(offer->reads, handler);
  return;
}

EAPI void
efl_core_wayland_offer_proxy_receive(Efl_Core_Wayland_Offer *offer,
                                     const char             *mime,
                                     int                     fd)
{
  EINA_SAFETY_ON_NULL_RETURN(offer);

  if (!offer->proxied) offer->ref++;
  offer->proxied = 1;
  wl_data_offer_receive(offer->offer, mime, fd);
}

EAPI void
efl_core_wayland_offer_proxy_receive_end(Efl_Core_Wayland_Offer *offer)
{
  EINA_SAFETY_ON_NULL_RETURN(offer);

  if (!offer->proxied) return;
  offer->proxied = 0;
  _efl_core_wayland_offer_unref(offer);
}

EAPI void
efl_core_wayland_offer_finish(Efl_Core_Wayland_Offer *offer)
{
  EINA_SAFETY_ON_NULL_RETURN(offer);

  wl_data_offer_finish(offer->offer);
}

void
_efl_core_wayland_offer_unref(Efl_Core_Wayland_Offer *offer)
{
  char *str;

  EINA_SAFETY_ON_NULL_RETURN(offer);

  offer->ref--;

  if (offer->ref > 0) return;

  wl_data_offer_destroy(offer->offer);

  if (offer->mimetypes)
  {
    while ((str = eina_array_pop(offer->mimetypes)))
      free(str);
    eina_array_free(offer->mimetypes);
    offer->mimetypes = NULL;
  }

  if (offer->input->drag.offer == offer) offer->input->drag.offer = NULL;
  if (offer->input->selection.offer == offer)
    offer->input->selection.offer = NULL;

  free(offer);
}

static unsigned char
_compare(const void *container EFL_UNUSED, void *elem, void *data)
{
  if (!strcmp(elem, data)) return EFL_FALSE;
  return EFL_TRUE;
}

EAPI Efl_Bool
efl_core_wayland_offer_supports_mime(Efl_Core_Wayland_Offer *offer,
                                     const char             *mime)
{
  EINA_SAFETY_ON_NULL_RETURN_VAL(offer, EFL_FALSE);
  EINA_SAFETY_ON_NULL_RETURN_VAL(mime, EFL_FALSE);

  return !eina_array_foreach(offer->mimetypes, _compare, (void *)mime);
}

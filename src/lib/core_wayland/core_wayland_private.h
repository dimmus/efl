#ifndef _EFL_CORE_WL_PRIVATE_H
#define _EFL_CORE_WL_PRIVATE_H

#include <unistd.h>
#include <xkbcommon/xkbcommon-compose.h>

#include "Efl_Core.h"
#include "Efl_Core_Wl.h"
#include "Efl_Core_Input.h"

#include "core_wayland_internal.h"

#include "session-recovery-client-protocol.h"
#include "xdg-shell-client-protocol.h"
#include "xdg-shell-unstable-v6-client-protocol.h"
#include "efl-aux-hints-client-protocol.h"

extern int      _efl_core_wayland_log_dom;
extern Efl_Bool no_session_recovery;

#ifdef EAPI
#  undef EAPI
#endif

#ifdef __GNUC__
#  if __GNUC__ >= 4
#    define EAPI __attribute__((visibility("default")))
#  else
#    define EAPI
#  endif
#else
#  define EAPI
#endif

#ifdef EFL_CORE_WAYLAND_DEFAULT_LOG_COLOR
#  undef EFL_CORE_WAYLAND_DEFAULT_LOG_COLOR
#endif
#define EFL_CORE_WAYLAND_DEFAULT_LOG_COLOR EINA_COLOR_BLUE

#ifdef ERR
#  undef ERR
#endif
#define ERR(...) EINA_LOG_DOM_ERR(_efl_core_wayland_log_dom, __VA_ARGS__)

#ifdef DBG
#  undef DBG
#endif
#define DBG(...) EINA_LOG_DOM_DBG(_efl_core_wayland_log_dom, __VA_ARGS__)

#ifdef INF
#  undef INF
#endif
#define INF(...) EINA_LOG_DOM_INFO(_efl_core_wayland_log_dom, __VA_ARGS__)

#ifdef WRN
#  undef WRN
#endif
#define WRN(...) EINA_LOG_DOM_WARN(_efl_core_wayland_log_dom, __VA_ARGS__)

#ifdef CRI
#  undef CRI
#endif
#define CRI(...) EINA_LOG_DOM_CRIT(_efl_core_wayland_log_dom, __VA_ARGS__)

typedef struct _Efl_Core_Wayland_Input_Devices
{
  Eo                      *pointer_dev;
  Eo                      *keyboard_dev;
  Eo                      *touch_dev;
  Eo                      *seat_dev;
  Efl_Core_Wayland_Window *window;
} Efl_Core_Wayland_Input_Devices;

struct _Efl_Core_Wayland_Display
{
  int   refs;
  char *name;
  pid_t pid;

  struct
  {
    struct wl_display             *display;
    struct wl_registry            *registry;
    struct wl_compositor          *compositor;
    struct wl_subcompositor       *subcompositor;
    struct wl_data_device_manager *data_device_manager;
    int                            data_device_manager_version;
    struct wl_shm                 *shm;
    struct zwp_linux_dmabuf_v1    *dmabuf;
    struct zxdg_shell_v6          *zxdg_shell;
    struct xdg_wm_base            *xdg_wm_base;
    struct zwp_e_session_recovery *session_recovery;
    struct efl_aux_hints          *efl_aux_hints;
    struct efl_hints              *efl_hints;
    int                            compositor_version;
  } wl;

  uint32_t serial;

  struct xkb_context *xkb_context;

  Core_Fd_Handler *fd_hdl;

  Eina_Hash  *globals;
  Core_Timer *recovery_timer;

  Eina_Inlist *windows;
  Eina_Inlist *outputs;
  Eina_Inlist *inputs;
  Eina_Inlist *seats;

  int syncs;

  Efl_Bool sync_done  : 1;
  Efl_Bool shell_done : 1;
  Efl_Bool recovering : 1;
};

struct _Efl_Core_Wayland_Subsurface
{
  EINA_INLIST;

  int x, y;

  Efl_Core_Wayland_Window *parent;

  struct
  {
    struct wl_surface    *surface;
    struct wl_subsurface *subsurface;
  } wl;

  Efl_Bool sync : 1;
};

struct _Efl_Core_Wayland_Aux_Hint
{
  EINA_INLIST;

  int         id;
  const char *hint, *val;
};

struct _Efl_Core_Wayland_Frame_Cb_Handle
{
  EINA_INLIST;
  Efl_Core_Wayland_Window  *win;
  Efl_Core_Wayland_Frame_Cb cb;
  void                     *data;
};

typedef struct _Efl_Core_Wayland_Window_Configure_State
{
  uint32_t       serial;
  Eina_Rectangle geometry;
  Efl_Bool       maximized  : 1;
  Efl_Bool       fullscreen : 1;
  Efl_Bool       focused    : 1;
  Efl_Bool       resizing   : 1;
} Efl_Core_Wayland_Window_Configure_State;

struct _Efl_Core_Wayland_Window
{
  EINA_INLIST;

  Efl_Core_Wayland_Display *display;

  Efl_Core_Wayland_Window *parent;
  Efl_Core_Wayland_Input  *grab;

  Efl_Core_Wayland_Surface *wl_surface;

  int         rotation, surface_id;
  const char *title;
  const char *class;
  const char *role;

  struct wl_surface   *surface;
  void                *buffer;
  struct wl_callback  *callback;
  struct xdg_surface  *xdg_surface;
  struct xdg_toplevel *xdg_toplevel;
  struct xdg_popup    *xdg_popup;

  struct zxdg_surface_v6  *zxdg_surface;
  struct zxdg_toplevel_v6 *zxdg_toplevel;
  struct zxdg_popup_v6    *zxdg_popup;

  void (*cb_close)(void *data, Efl_Core_Wayland_Window *win);
  void *cb_close_data;

  Eina_Stringshare *uuid;

  void (*xdg_configure_ack)(struct xdg_surface *surface, uint32_t serial);
  void (*xdg_set_min_size)(struct xdg_toplevel *toplevel, int32_t w, int32_t h);
  void (*xdg_set_max_size)(struct xdg_toplevel *toplevel, int32_t w, int32_t h);

  void (*zxdg_configure_ack)(struct zxdg_surface_v6 *surface, uint32_t serial);
  void (*zxdg_set_min_size)(struct zxdg_toplevel_v6 *toplevel,
                            int32_t                  w,
                            int32_t                  h);
  void (*zxdg_set_max_size)(struct zxdg_toplevel_v6 *toplevel,
                            int32_t                  w,
                            int32_t                  h);

  Eina_Rectangle saved;
  Eina_Rectangle opaque;
  Eina_Rectangle input_rect;

  Efl_Core_Wayland_Window_Type type;

  struct
  {
    int          w, h;
    unsigned int aspect;
    Efl_Bool     set : 1;
  } aspect;

  struct
  {
    int      w, h;
    Efl_Bool set : 1;
  } weight;

  Eina_Inlist *subsurfs;
  Eina_List   *supported_aux_hints;
  Eina_Inlist *frame_callbacks;

  Eina_List *outputs;

  Efl_Core_Wayland_Window_Configure_State set_config;
  Efl_Core_Wayland_Window_Configure_State req_config;
  Efl_Core_Wayland_Window_Configure_State def_config;

  Efl_Bool alpha : 1;

  Efl_Bool input_set  : 1;
  Efl_Bool opaque_set : 1;

  Efl_Bool focus_skip : 1;
  Efl_Bool floating   : 1;

  Efl_Bool commit_pending : 1;

  struct
  {
    Efl_Bool configure  : 1;
    Efl_Bool min        : 1;
    Efl_Bool max        : 1;
    Efl_Bool geom       : 1;
    Efl_Bool opaque     : 1;
    Efl_Bool input      : 1;
    Efl_Bool fullscreen : 1;
    Efl_Bool maximized  : 1;
  } pending;

  struct
  {
    Efl_Bool     supported : 1;
    Efl_Bool     app_set   : 1;
    int          preferred_rot;
    int         *available_rots;
    unsigned int count;
  } wm_rot;

  Efl_Bool has_buffer        : 1;
  Efl_Bool updating          : 1;
  Efl_Bool deferred_minimize : 1;

  Efl_Bool visible : 1;
};

struct _Efl_Core_Wayland_Output
{
  EINA_INLIST;

  Efl_Core_Wayland_Display *display;
  struct wl_output         *wl_output;

  int            mw, mh, transform;
  const char    *make, *model;
  Eina_Rectangle geometry;
};

typedef struct _Efl_Core_Wayland_Dnd_Source
{
  Efl_Core_Wayland_Input *input;

  int              x, y;
  Core_Fd_Handler *fdh;

  struct wl_data_offer           *offer;
  struct wl_array                 types;
  uint32_t                        dnd_action;
  uint32_t                        source_actions;
  Efl_Core_Wayland_Selection_Type sel_type;
  Efl_Bool                        active_read;
  void                           *read_data;
  unsigned int                    len;
} Efl_Core_Wayland_Dnd_Source;

/** TODO: Refactor ALL Input code :(
 *
 * wl_seat is a GROUP of Input Devices (such as):
 *      keyboards, pointers, touch devices
 */
struct _Efl_Core_Wayland_Pointer
{
  EINA_INLIST;

  Efl_Core_Wayland_Seat *seat;

  double       sx, sy;
  unsigned int button;
  unsigned int enter_serial;

  struct
  {
    unsigned int             button, count, timestamp;
    Efl_Core_Wayland_Window *window;
  } grab;

  Efl_Core_Wayland_Window *focus;

  Eina_List *resources;
};

struct _Efl_Core_Wayland_Keyboard
{
  EINA_INLIST;

  Efl_Core_Wayland_Seat *seat;

  unsigned int modifiers;

  struct
  {
    struct xkb_keymap *keymap;
    struct xkb_state  *state;
    xkb_mod_mask_t     control_mask;
    xkb_mod_mask_t     alt_mask;
    xkb_mod_mask_t     shift_mask;
    xkb_mod_mask_t     win_mask;
    xkb_mod_mask_t     scroll_mask;
    xkb_mod_mask_t     num_mask;
    xkb_mod_mask_t     caps_mask;
    xkb_mod_mask_t     altgr_mask;
    unsigned int       mods_depressed;
    unsigned int       mods_latched;
    unsigned int       mods_locked;
    unsigned int       mods_group;
  } xkb;

  struct
  {
    Core_Timer  *tmr;
    unsigned int sym, key, time;
    double       rate, delay;
    Efl_Bool     enabled : 1;
  } repeat;

  struct
  {
    unsigned int             button, count, timestamp;
    Efl_Core_Wayland_Window *window;
  } grab;

  Efl_Core_Wayland_Window *focus;

  Eina_List *resources;
};

struct _Efl_Core_Wayland_Touch
{
  EINA_INLIST;

  struct
  {
    unsigned int             button, count, timestamp;
    Efl_Core_Wayland_Window *window;
  } grab;
};

struct _Efl_Core_Wayland_Seat
{
  EINA_INLIST;

  uint32_t                        id;
  uint32_t                        version;
  const char                     *name;
  struct wl_global               *global;
  const struct wl_seat_interface *implementation;

  struct
  {
    struct wl_global   *global;
    struct wl_resource *resource;
  } im;

  Efl_Core_Wayland_Bind_Cb   bind_cb;
  Efl_Core_Wayland_Unbind_Cb unbind_cb;

  Efl_Core_Wayland_Pointer *pointer;
  int                       pointer_count;

  Efl_Core_Wayland_Keyboard *keyboard;
  int                        keyboard_count;

  Efl_Core_Wayland_Touch *touch;
  int                     touch_count;

  Eina_List *resources;
};

struct _Efl_Core_Wayland_Input
{
  EINA_INLIST;

  Efl_Core_Wayland_Display *display;

  unsigned int timestamp;

  struct
  {
    struct wl_seat     *seat;
    struct wl_pointer  *pointer;
    struct wl_keyboard *keyboard;
    struct wl_touch    *touch;
  } wl;

  struct
  {
    struct wl_data_device *device;

    struct
    {
      struct wl_data_source *source;
      struct wl_array        types;
      uint32_t               serial;
    } selection;

    struct
    {
      struct wl_data_source *source;
      struct wl_array        types;
      uint32_t               action;
      uint32_t               serial;
    } drag;
  } data;

  struct
  {
    const char        *name;
    unsigned int       size;
    struct wl_surface *surface;
    int                hot_x, hot_y;
  } cursor;

  struct
  {
    double       sx, sy;
    unsigned int button;
    unsigned int enter_serial;
  } pointer;

  struct
  {
    unsigned int modifiers;
  } keyboard;

  struct
  {
    Efl_Core_Wayland_Window *pointer;
    Efl_Core_Wayland_Window *prev_pointer;
    Efl_Core_Wayland_Window *keyboard;
    Efl_Core_Wayland_Window *touch;
  } focus;

  struct
  {
    unsigned int             button, count, timestamp;
    Efl_Core_Wayland_Window *window;
    unsigned int             touch_count;
  } grab;

  struct
  {
    struct xkb_keymap        *keymap;
    struct xkb_state         *state;
    struct xkb_state         *maskless_state;
    struct xkb_compose_table *compose_table;
    struct xkb_compose_state *compose_state;
    xkb_mod_mask_t            control_mask;
    xkb_mod_mask_t            alt_mask;
    xkb_mod_mask_t            shift_mask;
    xkb_mod_mask_t            win_mask;
    xkb_mod_mask_t            scroll_mask;
    xkb_mod_mask_t            num_mask;
    xkb_mod_mask_t            caps_mask;
    xkb_mod_mask_t            altgr_mask;
    unsigned int              mods_depressed;
    unsigned int              mods_latched;
    unsigned int              mods_locked;
    unsigned int              mods_group;
  } xkb;

  struct
  {
    Core_Timer  *timer;
    unsigned int sym, sym_name, key, time;
    double       rate, delay;
    Efl_Bool     enabled   : 1;
    Efl_Bool     repeating : 1;
    Efl_Bool     changed   : 1;
  } repeat;

  struct
  {
    Efl_Core_Wayland_Offer  *offer;
    uint32_t                 enter_serial;
    Efl_Core_Wayland_Window *window;
  } drag, selection;

  unsigned int      seat_version;
  unsigned int      id;
  Eina_Stringshare *name;

  Core_Event_Handler *dev_add_handler;
  Core_Event_Handler *dev_remove_handler;
  Eina_List          *devices_list;
};

typedef struct _Buffer_Handle Buffer_Handle;

typedef struct _Efl_Core_Wayland_Buffer
{
  struct wl_buffer *wl_buffer;
  int               size;
  int               w, h;
  int               age;
  unsigned long     stride;
  Buffer_Handle    *bh;
  int               fd;
  void             *mapping;

  int      index;
  Efl_Bool locked   : 1;
  Efl_Bool busy     : 1;
  Efl_Bool orphaned : 1;
  Efl_Bool alpha    : 1;
} Efl_Core_Wayland_Buffer;

typedef struct _Efl_Core_Wayland_Surface
{
  void                    *private_data;
  Efl_Core_Wayland_Window *wl_win;

  int                                 w, h;
  Efl_Core_Wayland_Surface_Interface *funcs;
  Core_Event_Handler                 *offscreen_handler;
  Efl_Bool                            alpha : 1;
} Efl_Core_Wayland_Surface;

Efl_Core_Wayland_Window *
_efl_core_wayland_display_window_surface_find(Efl_Core_Wayland_Display *display,
                                              struct wl_surface *wl_surface);
void _display_event_free(void *d, void *event EFL_UNUSED);

void _efl_core_wayland_output_add(Efl_Core_Wayland_Display *display,
                                  unsigned int              id);
void _efl_core_wayland_output_del(Efl_Core_Wayland_Output *output);

void     _efl_core_wayland_input_add(Efl_Core_Wayland_Display *display,
                                     unsigned int              id,
                                     unsigned int              version);
void     _efl_core_wayland_input_del(Efl_Core_Wayland_Input *input);
void     _efl_core_wayland_input_ungrab(Efl_Core_Wayland_Input *input);
void     _efl_core_wayland_input_grab(Efl_Core_Wayland_Input  *input,
                                      Efl_Core_Wayland_Window *window,
                                      unsigned int             button);
void     _efl_core_wayland_input_cursor_set(Efl_Core_Wayland_Input *input,
                                            const char             *cursor);
Efl_Bool _efl_core_wayland_input_cursor_update(void *data);
void     _efl_core_wayland_input_window_remove(Efl_Core_Wayland_Input  *input,
                                               Efl_Core_Wayland_Window *window);

void _efl_core_wayland_dnd_add(Efl_Core_Wayland_Input *input,
                               struct wl_data_offer   *offer);
void _efl_core_wayland_dnd_enter(Efl_Core_Wayland_Input *input,
                                 struct wl_data_offer   *offer,
                                 struct wl_surface      *surface,
                                 int                     x,
                                 int                     y,
                                 uint32_t                serial);
void _efl_core_wayland_dnd_leave(Efl_Core_Wayland_Input *input);
void _efl_core_wayland_dnd_motion(Efl_Core_Wayland_Input *input,
                                  int                     x,
                                  int                     y,
                                  uint32_t                serial);
void _efl_core_wayland_dnd_drop(Efl_Core_Wayland_Input *input);
void _efl_core_wayland_dnd_selection(Efl_Core_Wayland_Input *input,
                                     struct wl_data_offer   *offer);
void _efl_core_wayland_dnd_del(Efl_Core_Wayland_Dnd_Source *source);

void _efl_core_wayland_subsurf_unmap(Efl_Core_Wayland_Subsurface *subsurf);
void _efl_core_wayland_subsurf_free(Efl_Core_Wayland_Subsurface *subsurf);

void _efl_core_wayland_window_surface_create(Efl_Core_Wayland_Window *window);
void
_efl_core_wayland_window_shell_surface_init(Efl_Core_Wayland_Window *window);
void _efl_core_wayland_window_semi_free(Efl_Core_Wayland_Window *window);

void     _efl_core_wayland_offer_unref(Efl_Core_Wayland_Offer *offer);
Efl_Bool _efl_core_wayland_display_sync_get(void);

void _efl_core_wayland_buffer_test(Efl_Core_Wayland_Display *ewd);

EAPI void efl_core_wayland_window_weight_set(Efl_Core_Wayland_Window *window,
                                             double                   w,
                                             double                   h);

Efl_Core_Wayland_Output *
_efl_core_wayland_output_find(Efl_Core_Wayland_Display *dsp,
                              struct wl_output         *op);

#endif

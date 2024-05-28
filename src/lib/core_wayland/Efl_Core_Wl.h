#ifndef _EFL_CORE_WAYLAND_H_
# define _EFL_CORE_WAYLAND_H_

# include <Efl_Shared.h>
# include <Efl_Core.h>
# include <core.h> /* use old api here. should be removed later */
# include <wayland-client.h>
# include <wayland-cursor.h>
# include <xkbcommon/xkbcommon.h>

# define WL_HIDE_DEPRECATED
# include <wayland-server.h>

# ifdef EAPI
#  undef EAPI
# endif

# ifdef __GNUC__
#  if __GNUC__ >= 4
#   define EAPI __attribute__ ((visibility("default")))
#  else
#   define EAPI
#  endif
# else
#  define EAPI
# endif

/* # ifdef __cplusplus */
/* extern "C" { */
/* # endif */

# ifdef EFL_BETA_API_SUPPORT

#define EFL_CORE_WAYLAND_SURFACE_INTERFACE_VERSION 1

typedef struct _Efl_Core_Wayland_Subsurface Efl_Core_Wayland_Subsurface;

#  ifndef _EFL_CORE_WAYLAND_WINDOW_PREDEF
typedef struct _Efl_Core_Wayland_Window Efl_Core_Wayland_Window;
#  endif

typedef struct _Efl_Core_Wayland_Display Efl_Core_Wayland_Display;
typedef struct _Efl_Core_Wayland_Output Efl_Core_Wayland_Output;
typedef struct _Efl_Core_Wayland_Input Efl_Core_Wayland_Input;
typedef struct _Efl_Core_Wayland_Seat Efl_Core_Wayland_Seat;
typedef struct _Efl_Core_Wayland_Pointer Efl_Core_Wayland_Pointer;
typedef struct _Efl_Core_Wayland_Keyboard Efl_Core_Wayland_Keyboard;
typedef struct _Efl_Core_Wayland_Touch Efl_Core_Wayland_Touch;
typedef struct _Efl_Core_Wayland_Offer Efl_Core_Wayland_Offer;
typedef struct _Efl_Core_Wayland_Surface Efl_Core_Wayland_Surface;

/* matches protocol values */
typedef enum
{
   EFL_CORE_WAYLAND_DRAG_ACTION_NONE = 0,
   EFL_CORE_WAYLAND_DRAG_ACTION_COPY = 1,
   EFL_CORE_WAYLAND_DRAG_ACTION_MOVE = 2,
   EFL_CORE_WAYLAND_DRAG_ACTION_ASK = 4,
   EFL_CORE_WAYLAND_DRAG_ACTION_LAST = 5,
} Efl_Core_Wayland_Drag_Action;

typedef enum
{
  EFL_CORE_WAYLAND_SEAT_CAPABILITIES_NONE = 0,
  EFL_CORE_WAYLAND_SEAT_CAPABILITIES_POINTER = 1,
  EFL_CORE_WAYLAND_SEAT_CAPABILITIES_KEYBOARD = 2,
  EFL_CORE_WAYLAND_SEAT_CAPABILITIES_TOUCH = 4
} Efl_Core_Wayland_Seat_Capabilities;

struct _Efl_Core_Wayland_Event_Connection
{
   Efl_Core_Wayland_Display *display;
};

typedef struct _Efl_Core_Wayland_Event_Connection Efl_Core_Wayland_Event_Connect;
typedef struct _Efl_Core_Wayland_Event_Connection Efl_Core_Wayland_Event_Disconnect;

typedef struct _Efl_Core_Wayland_Global
{
   Eina_Stringshare *interface;
   unsigned int id, version;
   Efl_Core_Wayland_Display *display;
} Efl_Core_Wayland_Global;

typedef struct _Efl_Core_Wayland_Event_Global
{
   Efl_Core_Wayland_Display *display;
   Eina_Stringshare *interface;
   unsigned int id, version;
} Efl_Core_Wayland_Event_Global;

typedef struct _Efl_Core_Wayland_Event_Focus_In
{
   Efl_Core_Wayland_Window *window;
   unsigned int timestamp;
   Eo *dev; //The seat device
   Efl_Core_Wayland_Display *display;
} Efl_Core_Wayland_Event_Focus_In;

typedef struct _Efl_Core_Wayland_Event_Focus_Out
{
   Efl_Core_Wayland_Window *window;
   unsigned int timestamp;
   Eo *dev; //The seat device
   Efl_Core_Wayland_Display *display;
} Efl_Core_Wayland_Event_Focus_Out;

typedef struct _Efl_Core_Wayland_Event_Dnd_Enter
{
   Efl_Core_Wayland_Window *win, *source;
   Efl_Core_Wayland_Offer *offer;
   int x, y;
   unsigned int seat;
   Efl_Core_Wayland_Display *display;
} Efl_Core_Wayland_Event_Dnd_Enter;

typedef struct _Efl_Core_Wayland_Event_Dnd_Leave
{
   Efl_Core_Wayland_Window *win, *source;
   Efl_Core_Wayland_Offer *offer;
   unsigned int seat;
   Efl_Core_Wayland_Display *display;
} Efl_Core_Wayland_Event_Dnd_Leave;

typedef struct _Efl_Core_Wayland_Event_Dnd_Motion
{
   Efl_Core_Wayland_Window *win, *source;
   Efl_Core_Wayland_Offer *offer;
   int x, y;
   unsigned int seat;
   Efl_Core_Wayland_Display *display;
} Efl_Core_Wayland_Event_Dnd_Motion;

typedef struct _Efl_Core_Wayland_Event_Dnd_Drop
{
   Efl_Core_Wayland_Window *win, *source;
   int x, y;
   Efl_Core_Wayland_Offer *offer;
   unsigned int seat;
   Efl_Core_Wayland_Display *display;
} Efl_Core_Wayland_Event_Dnd_Drop;

typedef struct _Efl_Core_Wayland_Event_Dnd_End
{
   Efl_Core_Wayland_Window *win, *source;
   unsigned int seat;
   Efl_Core_Wayland_Display *display;
} Efl_Core_Wayland_Event_Dnd_End;

struct _Efl_Core_Wayland_Event_Data_Source_Event
{
   Efl_Core_Wayland_Window *win, *source;
   Efl_Core_Wayland_Drag_Action action;
   unsigned int seat;
   uint32_t serial;
   Efl_Core_Wayland_Display *display;
};

typedef struct Efl_Core_Wayland_Event_Data_Source_End
{
   Efl_Core_Wayland_Window *win, *source;
   Efl_Core_Wayland_Drag_Action action;
   unsigned int seat;
   uint32_t serial;
   Efl_Core_Wayland_Display *display;
   Efl_Bool cancelled;
} Efl_Core_Wayland_Event_Data_Source_End;
typedef struct _Efl_Core_Wayland_Event_Data_Source_Event Efl_Core_Wayland_Event_Data_Source_Drop;
typedef struct _Efl_Core_Wayland_Event_Data_Source_Event Efl_Core_Wayland_Event_Data_Source_Action;

typedef struct _Efl_Core_Wayland_Event_Data_Source_Target
{
   char *type;
   unsigned int seat;
   Efl_Core_Wayland_Display *display;
} Efl_Core_Wayland_Event_Data_Source_Target;

typedef struct _Efl_Core_Wayland_Event_Data_Source_Send
{
   char *type;
   int fd;
   unsigned int seat;
   uint32_t serial;
   Efl_Core_Wayland_Display *display;
} Efl_Core_Wayland_Event_Data_Source_Send;

typedef struct Efl_Core_Wayland_Event_Seat_Selection
{
   unsigned int seat;
   Efl_Core_Wayland_Display *display;
} Efl_Core_Wayland_Event_Seat_Selection;

typedef struct _Efl_Core_Wayland_Event_Seat_Name
{
   Eina_Stringshare *name;
   unsigned int id;
   Efl_Core_Wayland_Display *display;
} Efl_Core_Wayland_Event_Seat_Name;

typedef struct _Efl_Core_Wayland_Event_Seat_Capabilities
{
   unsigned int id;
   Efl_Bool pointer_enabled : 1;
   Efl_Bool keyboard_enabled : 1;
   Efl_Bool touch_enabled : 1;
   Efl_Core_Wayland_Display *display;
} Efl_Core_Wayland_Event_Seat_Capabilities;

typedef enum
{
   EFL_CORE_WAYLAND_DEVICE_TYPE_SEAT,
   EFL_CORE_WAYLAND_DEVICE_TYPE_POINTER,
   EFL_CORE_WAYLAND_DEVICE_TYPE_KEYBOARD,
   EFL_CORE_WAYLAND_DEVICE_TYPE_TOUCH
} Efl_Core_Wayland_Device_Type;

typedef struct _Efl_Core_Wayland_Event_Device
{
   Eo *dev;
   Efl_Core_Wayland_Window *window;
   unsigned int seat_id;
   Efl_Core_Wayland_Device_Type type;
} Efl_Core_Wayland_Event_Device;

typedef enum
{
   EFL_CORE_WAYLAND_SELECTION_CNP,
   EFL_CORE_WAYLAND_SELECTION_DND
} Efl_Core_Wayland_Selection_Type;

typedef enum
{
   EFL_CORE_WAYLAND_WINDOW_STATE_NONE = 0,
   EFL_CORE_WAYLAND_WINDOW_STATE_FULLSCREEN = (1 << 0),
   EFL_CORE_WAYLAND_WINDOW_STATE_MAXIMIZED = (1 << 1),
} Efl_Core_Wayland_Window_States;

typedef struct _Efl_Core_Wayland_Event_Window_Configure
{
   Efl_Core_Wayland_Window *win, *event_win;
   unsigned int edges;
   unsigned int w, h;
   unsigned int states;
} Efl_Core_Wayland_Event_Window_Configure;

typedef struct _Efl_Core_Wayland_Event_Window_Configure_Complete
{
   Efl_Core_Wayland_Window *win;
} Efl_Core_Wayland_Event_Window_Configure_Complete;

typedef struct _Efl_Core_Wayland_Event_Input_Keymap_Changed
{
   unsigned int id;
   Efl_Core_Wayland_Display *display;
} Efl_Core_Wayland_Event_Seat_Keymap_Changed;

typedef struct Efl_Core_Wayland_Event_Seat_Keyboard_Repeat_Changed
{
   unsigned int id;
   Efl_Core_Wayland_Display *display;
} Efl_Core_Wayland_Event_Seat_Keyboard_Repeat_Changed;

typedef struct _Efl_Core_Wayland_Event_Sync_Done
{
   Efl_Core_Wayland_Display *display;
} Efl_Core_Wayland_Event_Sync_Done;

typedef struct _Efl_Core_Wayland_Event_Offer_Data_Ready
{
   Efl_Core_Wayland_Offer *offer;
   char *data;
   int len;
   const char *mimetype;
   unsigned int seat;
   Efl_Core_Wayland_Display *display;
} Efl_Core_Wayland_Event_Offer_Data_Ready;

typedef struct _Efl_Core_Wayland_Event_Output_Transform
{
   Efl_Core_Wayland_Output *output;
   int transform, old_transform;
} Efl_Core_Wayland_Event_Output_Transform;

typedef struct _Efl_Core_Wayland_Event_Window_Rotation
{
   Efl_Core_Wayland_Window *win;
   int rotation, w, h, angle;
   Efl_Bool resize : 1;
} Efl_Core_Wayland_Event_Window_Rotation;

typedef struct _Efl_Core_Wayland_Event_Window_Rotation Efl_Core_Wayland_Event_Window_Rotation_Change_Prepare;
typedef struct _Efl_Core_Wayland_Event_Window_Rotation Efl_Core_Wayland_Event_Window_Rotation_Change_Prepare_Done;
typedef struct _Efl_Core_Wayland_Event_Window_Rotation Efl_Core_Wayland_Event_Window_Rotation_Change_Request;
typedef struct _Efl_Core_Wayland_Event_Window_Rotation Efl_Core_Wayland_Event_Window_Rotation_Change_Done;

typedef struct _Efl_Core_Wayland_Event_Window_Show
{
   Efl_Core_Wayland_Window *win;
   Efl_Core_Wayland_Window *parent_win;
   Efl_Core_Wayland_Window *event_win;
} Efl_Core_Wayland_Event_Window_Show;

typedef struct _Efl_Core_Wayland_Event_Window_Hide
{
   Efl_Core_Wayland_Window *win;
   Efl_Core_Wayland_Window *parent_win;
   Efl_Core_Wayland_Window *event_win;
} Efl_Core_Wayland_Event_Window_Hide;

typedef struct _Efl_Core_Wayland_Event_Window_Activate
{
   Efl_Core_Wayland_Window *win;
   Efl_Core_Wayland_Window *parent_win;
   Efl_Core_Wayland_Window *event_win;
} Efl_Core_Wayland_Event_Window_Activate;

typedef struct _Efl_Core_Wayland_Event_Window_Deactivate
{
   Efl_Core_Wayland_Window *win;
   Efl_Core_Wayland_Window *parent_win;
   Efl_Core_Wayland_Window *event_win;
} Efl_Core_Wayland_Event_Window_Deactivate;

typedef struct _Efl_Core_Wayland_Event_Window_Common
{
   Efl_Core_Wayland_Window *win;
   Efl_Core_Wayland_Window *parent_win;
   Efl_Core_Wayland_Window *event_win;
} Efl_Core_Wayland_Event_Window_Common;

typedef struct _Efl_Core_Wayland_Event_Window_Iconify_State_Change
{
   Efl_Core_Wayland_Window *win;
   unsigned int iconified;
   unsigned int force;
} Efl_Core_Wayland_Event_Window_Iconify_State_Change;

typedef enum _Efl_Core_Wayland_Window_Type
{
   EFL_CORE_WAYLAND_WINDOW_TYPE_NONE,
   EFL_CORE_WAYLAND_WINDOW_TYPE_TOPLEVEL,
   EFL_CORE_WAYLAND_WINDOW_TYPE_MENU,
   EFL_CORE_WAYLAND_WINDOW_TYPE_DND,
   EFL_CORE_WAYLAND_WINDOW_TYPE_CUSTOM,
   EFL_CORE_WAYLAND_WINDOW_TYPE_NOTIFICATION, /** @since 1.20 */
   EFL_CORE_WAYLAND_WINDOW_TYPE_UTILITY, /** @since 1.20 */
   EFL_CORE_WAYLAND_WINDOW_TYPE_DIALOG, /** @since 1.20 */
   EFL_CORE_WAYLAND_WINDOW_TYPE_DOCK, /** @since 1.20 */
   EFL_CORE_WAYLAND_WINDOW_TYPE_SPLASH, /** @since 1.20 */
   EFL_CORE_WAYLAND_WINDOW_TYPE_LAST
} Efl_Core_Wayland_Window_Type;

typedef struct _Efl_Core_Wayland_Event_Aux_Hint_Allowed
{
   Efl_Core_Wayland_Window *win;
   int id;
   Efl_Core_Wayland_Display *display;
} Efl_Core_Wayland_Event_Aux_Hint_Allowed;

typedef struct _Efl_Core_Wayland_Event_Aux_Hint_Supported
{
   Efl_Core_Wayland_Window *win;
   Efl_Core_Wayland_Display *display;
} Efl_Core_Wayland_Event_Aux_Hint_Supported;

typedef struct Efl_Core_Wayland_Event_Aux_Message
{
   Efl_Core_Wayland_Window *win;
   Eina_Stringshare *key;
   Eina_Stringshare *val;
   Eina_List *options;
   Efl_Core_Wayland_Display *display;
} Efl_Core_Wayland_Event_Aux_Message;

typedef struct Efl_Core_Wayland_Event_Window_Offscreen
{
   Efl_Core_Wayland_Window *win;
} Efl_Core_Wayland_Event_Window_Offscreen;

typedef struct _Efl_Core_Wayland_Buffer Efl_Core_Wayland_Buffer;

typedef enum _Efl_Core_Wayland_Buffer_Type Efl_Core_Wayland_Buffer_Type;
enum _Efl_Core_Wayland_Buffer_Type
{
   EFL_CORE_WAYLAND_BUFFER_NONE = 0,
   EFL_CORE_WAYLAND_BUFFER_SHM = 1,
   EFL_CORE_WAYLAND_BUFFER_DMABUF = 2
};

typedef void (*Efl_Core_Wayland_Bind_Cb)(struct wl_client *client, void *data, uint32_t version, uint32_t id);
typedef void (*Efl_Core_Wayland_Unbind_Cb)(struct wl_resource *resource);
typedef void (*Efl_Core_Wayland_Frame_Cb)(Efl_Core_Wayland_Window *win, uint32_t timestamp, void *data);
typedef struct _Efl_Core_Wayland_Frame_Cb_Handle Efl_Core_Wayland_Frame_Cb_Handle;

EAPI extern int EFL_CORE_WAYLAND_EVENT_DISCONNECT;         /** @since 1.18 */
EAPI extern int EFL_CORE_WAYLAND_EVENT_CONNECT;            /** @since 1.18 */
EAPI extern int EFL_CORE_WAYLAND_EVENT_GLOBAL_ADDED;       /** @since 1.17 */
EAPI extern int EFL_CORE_WAYLAND_EVENT_GLOBAL_REMOVED;     /** @since 1.17 */
EAPI extern int EFL_CORE_WAYLAND_EVENT_FOCUS_IN;           /** @since 1.17 */
EAPI extern int EFL_CORE_WAYLAND_EVENT_FOCUS_OUT;          /** @since 1.17 */
EAPI extern int EFL_CORE_WAYLAND_EVENT_DND_ENTER;          /** @since 1.17 */
EAPI extern int EFL_CORE_WAYLAND_EVENT_DND_LEAVE;          /** @since 1.17 */
EAPI extern int EFL_CORE_WAYLAND_EVENT_DND_MOTION;         /** @since 1.17 */
EAPI extern int EFL_CORE_WAYLAND_EVENT_DND_DROP;           /** @since 1.17 */
EAPI extern int EFL_CORE_WAYLAND_EVENT_DND_END;            /** @since 1.17 */
EAPI extern int EFL_CORE_WAYLAND_EVENT_DATA_SOURCE_END;    /** @since 1.18 */
EAPI extern int EFL_CORE_WAYLAND_EVENT_DATA_SOURCE_DROP;   /** @since 1.18 */
EAPI extern int EFL_CORE_WAYLAND_EVENT_DATA_SOURCE_ACTION; /** @since 1.18 */
EAPI extern int EFL_CORE_WAYLAND_EVENT_DATA_SOURCE_TARGET; /** @since 1.17 */
EAPI extern int EFL_CORE_WAYLAND_EVENT_DATA_SOURCE_SEND;   /** @since 1.17 */
EAPI extern int EFL_CORE_WAYLAND_EVENT_SYNC_DONE;          /** @since 1.17 */
EAPI extern int EFL_CORE_WAYLAND_EVENT_OFFER_DATA_READY;   /** @since 1.19 */
EAPI extern int EFL_CORE_WAYLAND_EVENT_DEVICE_ADDED;       /** @since 1.19 */
EAPI extern int EFL_CORE_WAYLAND_EVENT_DEVICE_REMOVED;     /** @since 1.19 */
EAPI extern int EFL_CORE_WAYLAND_EVENT_SEAT_KEYMAP_CHANGED; /** @since 1.20 */
EAPI extern int EFL_CORE_WAYLAND_EVENT_SEAT_KEYBOARD_REPEAT_CHANGED;/** @since 1.20 */
EAPI extern int EFL_CORE_WAYLAND_EVENT_SEAT_SELECTION;      /** @since 1.20 */
EAPI extern int EFL_CORE_WAYLAND_EVENT_SEAT_NAME_CHANGED;  /** @since 1.19 */
EAPI extern int EFL_CORE_WAYLAND_EVENT_SEAT_CAPABILITIES_CHANGED;   /** @since 1.19 */
EAPI extern int EFL_CORE_WAYLAND_EVENT_OUTPUT_TRANSFORM;    /** @since 1.20 */
EAPI extern int EFL_CORE_WAYLAND_EVENT_AUX_HINT_ALLOWED;    /** @since 1.20 */
EAPI extern int EFL_CORE_WAYLAND_EVENT_AUX_HINT_SUPPORTED;  /** @since 1.20 */
EAPI extern int EFL_CORE_WAYLAND_EVENT_AUX_MESSAGE;         /** @since 1.20 */
EAPI extern int EFL_CORE_WAYLAND_EVENT_WINDOW_CONFIGURE;   /** @since 1.17 */
EAPI extern int EFL_CORE_WAYLAND_EVENT_WINDOW_ROTATE;      /** @since 1.20 */
EAPI extern int EFL_CORE_WAYLAND_EVENT_WINDOW_CONFIGURE_COMPLETE;   /** @since 1.19 */
EAPI extern int EFL_CORE_WAYLAND_EVENT_WINDOW_SHOW;         /** @since 1.20 */
EAPI extern int EFL_CORE_WAYLAND_EVENT_WINDOW_HIDE;         /** @since 1.20 */
EAPI extern int EFL_CORE_WAYLAND_EVENT_WINDOW_ACTIVATE;     /** @since 1.20 */
EAPI extern int EFL_CORE_WAYLAND_EVENT_WINDOW_DEACTIVATE;   /** @since 1.20 */
EAPI extern int EFL_CORE_WAYLAND_EVENT_WINDOW_ICONIFY_STATE_CHANGE; /** @since 1.21 */
EAPI extern int EFL_CORE_WAYLAND_EVENT_WINDOW_OFFSCREEN;    /** @since 1.21 */
EAPI extern int EFL_CORE_WAYLAND_EVENT_WINDOW_CREATE;       /** @since 1.25 */
EAPI extern int EFL_CORE_WAYLAND_EVENT_WINDOW_DESTROY;      /** @since 1.25 */

typedef struct _Efl_Core_Wayland_Surface_Interface
{
   int id;
   int version;

   void *(*setup)(Efl_Core_Wayland_Window *win);
   void (*destroy)(Efl_Core_Wayland_Surface *surface, void *priv_data);
   void (*reconfigure)(Efl_Core_Wayland_Surface *surface, void *priv_data, int w, int h, uint32_t flags, Efl_Bool alpha);
   void *(*data_get)(Efl_Core_Wayland_Surface *surface, void *priv_data, int *w, int *h);
   int  (*assign)(Efl_Core_Wayland_Surface *surface, void *priv_data);
   void (*post)(Efl_Core_Wayland_Surface *surface, void *priv_data, Eina_Rectangle *rects, unsigned int count);
   void (*flush)(Efl_Core_Wayland_Surface *surface, void *priv_data, Efl_Bool purge);
} Efl_Core_Wayland_Surface_Interface;

/**
 * @file
 * @brief Ecore functions for dealing with the Wayland display protocol
 *
 * @defgroup Efl_Core_Wayland_Group Efl_Core_Wayland - Wayland integration
 * @ingroup Ecore
 *
 * Efl_Core_Wayland provides a wrapper and convenience functions for using the
 * Wayland protocol in implementing a window system. Function groups for
 * this part of the library include the following:
 *
 * @li @ref Efl_Core_Wayland_Init_Group
 * @li @ref Efl_Core_Wayland_Display_Group
 * @li @ref Efl_Core_Wayland_Window_Group
 * @li @ref Efl_Core_Wayland_Output_Group
 * @li @ref Efl_Core_Wayland_Input_Group
 */

/**
 * @defgroup Efl_Core_Wayland_Init_Group Wayland Library Init and Shutdown Functions
 * @ingroup Efl_Core_Wayland_Group
 *
 * Functions that start and shutdown the Ecore Wl2 Library.
 */

/**
 * Initialize the Efl_Core_Wayland library
 *
 * @return  The number of times the library has been initialized without being
 *          shutdown. 0 is returned if an error occurs.
 *
 * @ingroup Efl_Core_Wayland_Init_Group
 * @since 1.17
 */
EAPI int efl_core_wayland_init(void);

/**
 * Shutdown the Efl_Core_Wayland Library
 *
 * @return  The number of times the library has been initialized without
 *          being shutdown.
 *
 * @ingroup Efl_Core_Wayland_Init_Group
 * @since 1.17
 */
EAPI int efl_core_wayland_shutdown(void);

/**
 * @defgroup Efl_Core_Wayland_Display_Group Wayland Library Display Functions
 * @ingroup Efl_Core_Wayland_Group
 *
 * Functions that deal with creating, connecting, or interacting with
 * Wayland displays
 */

/**
 * Create a new Wayland display
 *
 * @brief This function is typically used to create a new display for
 * use with compositors, or to create a new display for use in nested
 * compositors.
 *
 * @param name The display target name to create. If @c NULL, a default
 *             display name will be assigned.
 * @return The newly created Efl_Core_Wayland_Display
 *
 * @ingroup Efl_Core_Wayland_Display_Group
 * @since 1.17
 */
EAPI Efl_Core_Wayland_Display *efl_core_wayland_display_create(const char *name);

/**
 * Destroy an existing Wayland display
 *
 * @brief This function is typically used by servers to terminate an
 * existing Wayland display.
 *
 * @param display The display to terminate
 *
 * @ingroup Efl_Core_Wayland_Display_Group
 * @since 1.17
 */
EAPI void efl_core_wayland_display_destroy(Efl_Core_Wayland_Display *display);

/**
 * Connect to an existing Wayland display
 *
 * @brief This function is typically used by clients to connect to an
 * existing Wayland display.
 *
 * @param name The display target name to connect to. If @c NULL, the default
 *             display is assumed.
 *
 * @return The Efl_Core_Wayland_Display which was connected to
 *
 * @ingroup Efl_Core_Wayland_Display_Group
 * @since 1.17
 */
EAPI Efl_Core_Wayland_Display *efl_core_wayland_display_connect(const char *name);

/**
 * Disconnect an existing Wayland display
 *
 * @brief This function is typically used by clients to disconnect from an
 * existing Wayland display.
 *
 * @param display The display to disconnect from
 *
 * @ingroup Efl_Core_Wayland_Display_Group
 * @since 1.17
 */
EAPI void efl_core_wayland_display_disconnect(Efl_Core_Wayland_Display *display);

/**
 * Retrieve the existing Wayland display
 *
 * @param display The Efl_Core_Wayland_Display for which to retrieve the existing
 *                Wayland display from
 *
 * @return The wl_display which this Efl_Core_Wayland_Display is using
 *
 * @ingroup Efl_Core_Wayland_Display_Group
 * @since 1.17
 */
EAPI struct wl_display *efl_core_wayland_display_get(Efl_Core_Wayland_Display *display);

/**
 * Retrieve the wl_shm from a given Efl_Core_Wayland_Display
 *
 * @param display The Efl_Core_Wayland_Display for which to retrieve the existing
 *                Wayland shm interface from
 *
 * @return The wl_shm which this Efl_Core_Wayland_Display is using
 *
 * @ingroup Efl_Core_Wayland_Display_Group
 * @since 1.17
 */
EAPI struct wl_shm *efl_core_wayland_display_shm_get(Efl_Core_Wayland_Display *display);

/**
 * Retrieve the wl_dmabuf from a given Efl_Core_Wayland_Display
 *
 *
 * @param display The Efl_Core_Wayland_Display for which to retrieve the existing
 *                Wayland dmabuf interface from
 *
 * @return The wl_dmabuf which this Efl_Core_Wayland_Display is using
 *
 * @ingroup Efl_Core_Wayland_Display_Group
 *
 * @note This is intended for client use only and should be used only
 *       after efl_core_wayland_display_connect().  Also, the return type is
 *       void * instead of zpw_linux_dmabuf_v1 * since we don't want
 *       to change our public API every time the version changes in
 *       wayland-protocols.
 *
 * @since 1.18
 */
EAPI void *efl_core_wayland_display_dmabuf_get(Efl_Core_Wayland_Display *display);

/**
 * Return an Eina_Iterator that can be used to iterate through globals
 *
 * @param display The Efl_Core_Wayland_Display for which to return a global iterator
 *
 * @note The caller of this function should free the returned Eina_Iterator
 * when finished with it.
 *
 * @ingroup Efl_Core_Wayland_Display_Group
 * @since 1.17
 */
EAPI Eina_Iterator *efl_core_wayland_display_globals_get(Efl_Core_Wayland_Display *display);

/**
 * Retrieves the size of the current screen.
 *
 * @param display The display to get the screen size of
 * @param w where to return the width. May be NULL. Returns 0 on error.
 * @param h where to return the height. May be NULL. Returns 0 on error.
 *
 * @ingroup Efl_Core_Wayland_Display_Group
 * @since 1.17
 */
EAPI void efl_core_wayland_display_screen_size_get(Efl_Core_Wayland_Display *display, int *w, int *h);

/**
 * Get all the Efl_Core_Wayland_Input from the display.
 *
 * @param display The display
 *
 * @return A Eina_Iterator of Efl_Core_Wayland_Input or @c NULL on error
 *
 * @ingroup Efl_Core_Wayland_Display_Group
 * @since 1.19
 */
EAPI Eina_Iterator *efl_core_wayland_display_inputs_get(Efl_Core_Wayland_Display *display);

/**
 * Find a seat for a given display object using the seat id
 *
 * @param display The display
 * @param id The seat id
 *
 * @return The corresponding Efl_Core_Wayland_Input object or @c NULL if no match is found
 *
 * @ingroup Efl_Core_Wayland_Display_Group
 * @since 1.20
 */
EAPI Efl_Core_Wayland_Input *efl_core_wayland_display_input_find(const Efl_Core_Wayland_Display *display, unsigned int id);

/**
 * Find a seat for a given display object using the seat id
 *
 * @param display The display
 * @param name The seat name
 *
 * @return The corresponding Efl_Core_Wayland_Input object or @c NULL if no match is found
 *
 * @ingroup Efl_Core_Wayland_Display_Group
 * @since 1.20
 */
EAPI Efl_Core_Wayland_Input *efl_core_wayland_display_input_find_by_name(const Efl_Core_Wayland_Display *display, const char *name);

/**
 * Retrieves the Wayland Registry used for the current Wayland display.
 *
 * @param display The display to get the registry of
 *
 * @return The current wayland registry, or NULL on error
 *
 * @ingroup Efl_Core_Wayland_Display_Group
 * @since 1.17
 */
EAPI struct wl_registry *efl_core_wayland_display_registry_get(Efl_Core_Wayland_Display *display);

/**
 * Check if the display has performed a sync
 *
 * @param display The display
 *
 * @return True if the display sync has occurred
 * @see EFL_CORE_WAYLAND_EVENT_SYNC_DONE
 *
 * @ingroup Efl_Core_Wayland_Display_Group
 * @since 1.20
 */
EAPI Efl_Bool efl_core_wayland_display_sync_is_done(const Efl_Core_Wayland_Display *display);

/**
 * Get the name of the display object
 *
 * @param display The display
 *
 * @return The name of the display object
 *
 * @ingroup Efl_Core_Wayland_Display_Group
 * @since 1.20
 */
EAPI const char *efl_core_wayland_display_name_get(const Efl_Core_Wayland_Display *display);

/**
 * Finds an Efl_Core_Wayland_Window based on wl_surface
 *
 * @param display The display to search for the window
 * @param surface The wl_surface of the window to find
 *
 * @return The Efl_Core_Wayland_Window if found, or NULL if no such window exists
 *
 * @ingroup Efl_Core_Wayland_Display_Group
 * @since 1.24
 */
EAPI Efl_Core_Wayland_Window *efl_core_wayland_display_window_find_by_surface(Efl_Core_Wayland_Display *display, struct wl_surface *surface);

/**
 * Gets the connected display object
 *
 * @brief This function is typically used by clients to get an
 *        existing Wayland display.
 *
 * @param name The display target name. If @c NULL, the default
 *        display is assumed.
 *
 * @return The Efl_Core_Wayland_Display which was connected to
 *
 * @ingroup Efl_Core_Wayland_Display_Group
 * @since 1.24
 */
EAPI Efl_Core_Wayland_Display *efl_core_wayland_connected_display_get(const char *name);

/**
 * Gets the wl_compositor which belongs to this display
 *
 * @param display The Efl_Core_Wayland_Display to get the compositor of
 *
 * @return The wl_compositor associated with this display
 *
 * @ingroup Efl_Core_Wayland_Display_Group
 * @since 1.24
 */
EAPI struct wl_compositor *efl_core_wayland_display_compositor_get(Efl_Core_Wayland_Display *display);

/**
 * @defgroup Efl_Core_Wayland_Window_Group Wayland Library Window Functions
 * @ingroup Efl_Core_Wayland_Group
 *
 * Functions that deal with creating, destroying, or interacting with
 * Wayland windows
 */

/**
 * Create a new Efl_Core_Wayland_Window
 *
 * @param display The Efl_Core_Wayland_Display on which to create this new window
 * @param parent The Efl_Core_Wayland_Window which is the parent of this window
 * @param x Initial x position of window
 * @param y Initial y position of window
 * @param w Initial width of window
 * @param h Initial height of window
 *
 * @ingroup Efl_Core_Wayland_Window_Group
 * @since 1.17
 */
EAPI Efl_Core_Wayland_Window *efl_core_wayland_window_new(Efl_Core_Wayland_Display *display, Efl_Core_Wayland_Window *parent, int x, int y, int w, int h);

/**
 * Set a callback to be caleld just before the window is closed and freed
 *
 * @param window The window to listen to for a xdg toplevel close callback
 * @param cb The callback function to call being passed data and window
 * @param data The Data pointer to pass as data to the callback
 *
 * @ingroup Efl_Core_Wayland_Window_Group
 * @since 1.24
 */
EAPI void efl_core_wayland_window_close_callback_set(Efl_Core_Wayland_Window *window, void (*cb) (void *data, Efl_Core_Wayland_Window *win), void *data);

/**
 * Get the wl_surface which belongs to this window
 *
 * @param window The Efl_Core_Wayland_Window to get the surface of
 *
 * @return The wl_surface associated with this window.
 *
 * @ingroup Efl_Core_Wayland_Window_Group
 * @since 1.17
 */
EAPI struct wl_surface *efl_core_wayland_window_surface_get(Efl_Core_Wayland_Window *window);

/**
 * Get the id of a given Efl_Core_Wayland_Window
 *
 * @param window The window to return the id of
 *
 * @return The id of the given window, or -1 on failure
 *
 * @ingroup Efl_Core_Wayland_Window_Group
 * @since 1.17
 */
EAPI int efl_core_wayland_window_surface_id_get(Efl_Core_Wayland_Window *window);

/**
 * @see evas_object_size_hint_aspect_set
 * @ingroup Efl_Core_Wayland_Window_Group
 * @since 1.21
 */
EAPI void efl_core_wayland_window_aspect_set(Efl_Core_Wayland_Window *window, int w, int h, unsigned int aspect);

/**
 * @see evas_object_size_hint_aspect_get
 * @ingroup Efl_Core_Wayland_Window_Group
 * @since 1.24
 */
EAPI void efl_core_wayland_window_aspect_get(Efl_Core_Wayland_Window *window, int *w, int *h, unsigned int *aspect);

/**
 * Show a given Efl_Core_Wayland_Window
 *
 * @param window The Efl_Core_Wayland_Window to show
 *
 * @ingroup Efl_Core_Wayland_Window_Group
 * @since 1.17
 */
EAPI void efl_core_wayland_window_show(Efl_Core_Wayland_Window *window);

/**
 * Hide a given Efl_Core_Wayland_Window
 *
 * @param window The Efl_Core_Wayland_Window to hide
 *
 * @ingroup Efl_Core_Wayland_Window_Group
 * @since 1.17
 */
EAPI void efl_core_wayland_window_hide(Efl_Core_Wayland_Window *window);

/**
 * Free a given Efl_Core_Wayland_Window
 *
 * @param window The Efl_Core_Wayland_Window to free
 *
 * @ingroup Efl_Core_Wayland_Window_Group
 * @since 1.17
 */
EAPI void efl_core_wayland_window_free(Efl_Core_Wayland_Window *window);

/**
 * @brief Begin moving a given Efl_Core_Wayland_Window
 *
 * @param window The Efl_Core_Wayland_Window which to move
 * @param input The seat on which the move is active on
 *
 * @ingroup Efl_Core_Wayland_Window_Group
 * @since 1.20
 */
EAPI void efl_core_wayland_window_move(Efl_Core_Wayland_Window *window, Efl_Core_Wayland_Input *input);

/**
 * @brief Begin resizing a given window
 *
 * @param window The Efl_Core_Wayland_Window which to resize
 * @param input The seat on which the resize is active
 * @param location The edge of the window from which the resize should start
 *
 * @ingroup Efl_Core_Wayland_Window_Group
 * @since 1.20
 */
EAPI void efl_core_wayland_window_resize(Efl_Core_Wayland_Window *window, Efl_Core_Wayland_Input *input, int location);

/**
 * Get if a given window is alpha
 *
 * @param window The window to get if is alpha
 *
 * @return EFL_TRUE if window is alpha, EFL_FALSE otherwise
 *
 * @ingroup Efl_Core_Wayland_Window_Group
 * @since 1.17
 */
EAPI Efl_Bool efl_core_wayland_window_alpha_get(Efl_Core_Wayland_Window *window);

/**
 * Set a given window's alpha property
 *
 * @param window The window on which to set the alpha property
 * @param alpha EFL_TRUE to set window as alpha, EFL_FALSE otherwise
 *
 * @ingroup Efl_Core_Wayland_Window_Group
 * @since 1.17
 */
EAPI void efl_core_wayland_window_alpha_set(Efl_Core_Wayland_Window *window, Efl_Bool alpha);

/**
 * Set the opaque region of the Efl_Core_Wayland_Window
 *
 * @param window The window
 * @param x The left point of the region.
 * @param y The top point of the region.
 * @param w The width of the region.
 * @param h The height of the region.
 *
 * @ingroup Efl_Core_Wayland_Window_Group
 * @since 1.17
 */
EAPI void efl_core_wayland_window_opaque_region_set(Efl_Core_Wayland_Window *window, int x, int y, int w, int h);

/**
 * Get the opaque region of the Efl_Core_Wayland_Window
 *
 * @param window The window
 * @param x The left point of the region.
 * @param y The top point of the region.
 * @param w The width of the region.
 * @param h The height of the region.
 *
 * @ingroup Efl_Core_Wayland_Window_Group
 * @since 1.24
 */
EAPI void efl_core_wayland_window_opaque_region_get(Efl_Core_Wayland_Window *window, int *x, int *y, int *w, int *h);

/**
 * Set the input region of the Efl_Core_Wayland_Window.
 *
 * To set an empty region, pass width and height as 0.
 *
 * An empty input region means the entire window surface will accept input.
 *
 * @param window The window to set the input region of
 * @param x The left point of the region.
 * @param y The top point of the region.
 * @param w The width of the region.
 * @param h The height of the region.
 *
 * @ingroup Efl_Core_Wayland_Window_Group
 * @since 1.17
 */
EAPI void efl_core_wayland_window_input_region_set(Efl_Core_Wayland_Window *window, int x, int y, int w, int h);

/**
 * Get the input region of the Efl_Core_Wayland_Window.
 *
 * @param window The window to set the input region of
 * @param x The left point of the region.
 * @param y The top point of the region.
 * @param w The width of the region.
 * @param h The height of the region.
 *
 * @ingroup Efl_Core_Wayland_Window_Group
 * @since 1.24
 */
EAPI void efl_core_wayland_window_input_region_get(Efl_Core_Wayland_Window *window, int *x, int *y, int *w, int *h);

/**
 * Get if a given window is maximized
 *
 * @param window The window to get the maximized state of
 *
 * @return EFL_TRUE if window is maximized, EFL_FALSE otherwise
 *
 * @ingroup Efl_Core_Wayland_Window_Group
 * @since 1.17
 */
EAPI Efl_Bool efl_core_wayland_window_maximized_get(Efl_Core_Wayland_Window *window);

/**
 * Set the maximized state of a given window
 *
 * @param window The window to set the maximized state of
 * @param maximized EFL_TRUE to set maximized, EFL_FALSE to unset
 *
 * @ingroup Efl_Core_Wayland_Window_Group
 * @since 1.17
 */
EAPI void efl_core_wayland_window_maximized_set(Efl_Core_Wayland_Window *window, Efl_Bool maximized);

/**
 * Get if a given window is fullscreen
 *
 * @param window The window to get the fullscreen state of
 *
 * @return EFL_TRUE if window is fullscreen, EFL_FALSE otherwise
 *
 * @ingroup Efl_Core_Wayland_Window_Group
 * @since 1.17
 */
EAPI Efl_Bool efl_core_wayland_window_fullscreen_get(Efl_Core_Wayland_Window *window);

/**
 * Set the fullscreen state of a given window
 *
 * @param window The window to set the fullscreen state of
 * @param fullscreen EFL_TRUE to set fullscreen, EFL_FALSE to unset
 *
 * @ingroup Efl_Core_Wayland_Window_Group
 * @since 1.17
 */
EAPI void efl_core_wayland_window_fullscreen_set(Efl_Core_Wayland_Window *window, Efl_Bool fullscreen);

/**
 * Get if a given window is rotated
 *
 * @param window The window to get the rotation of
 *
 * @return The amount of rotation for this window, or -1 on failure
 *
 * @ingroup Efl_Core_Wayland_Window_Group
 * @since 1.17
 */
EAPI int efl_core_wayland_window_rotation_get(Efl_Core_Wayland_Window *window);

/**
 * Set the rotation of a given window
 *
 * @param window The window to set the rotation of
 * @param rotation The amount of rotation
 *
 * @ingroup Efl_Core_Wayland_Window_Group
 * @since 1.17
 */
EAPI void efl_core_wayland_window_rotation_set(Efl_Core_Wayland_Window *window, int rotation);

/**
 * Set the title of a given window
 *
 * @param window The window to set the title of
 * @param title The title of the window
 *
 * @ingroup Efl_Core_Wayland_Window_Group
 * @since 1.17
 */
EAPI void efl_core_wayland_window_title_set(Efl_Core_Wayland_Window *window, const char *title);

/**
 * Get the title of a given window
 *
 * @param window The window to set the title of
 *
 * @return A string if found, or NULL otherwise
 *
 * @ingroup Efl_Core_Wayland_Window_Group
 * @since 1.24
 */
EAPI const char *efl_core_wayland_window_title_get(Efl_Core_Wayland_Window *window);

/**
 * Set the class of a given window
 *
 * @param window The window to set the class of
 * @param clas The class of the window
 *
 * @ingroup Efl_Core_Wayland_Window_Group
 * @since 1.17
 */
EAPI void efl_core_wayland_window_class_set(Efl_Core_Wayland_Window *window, const char *clas);


/**
 * Get the class of a given window
 *
 * @param window The window to set the class of
 *
 * @return A string if found, or NULL otherwise
 *
 * @ingroup Efl_Core_Wayland_Window_Group
 * @since 1.24
 */
EAPI const char *efl_core_wayland_window_class_get(Efl_Core_Wayland_Window *window);

/**
 * Get the geometry of a given window
 *
 * @brief The window geometry returned here is the window geometry as
 * recognized by xdg_surface_set_window_geometry. As such, it represents the
 * "visible bounds" of a window from the user's perspective.
 *
 * @param window The window to get the geometry of
 * @param x The left point of the window geometry
 * @param y The top point of the window geometry
 * @param w The width of the window geometry
 * @param h The height of the window geometry
 *
 * @ingroup Efl_Core_Wayland_Window_Group
 * @since 1.17
 */
EAPI void efl_core_wayland_window_geometry_get(Efl_Core_Wayland_Window *window, int *x, int *y, int *w, int *h);

/**
 * Set the geometry of a given window
 *
 * @brief The window geometry referenced here is the window geometry as
 * recognized by xdg_surface_set_window_geometry. As such, it represents the
 * "visible bounds" of a window from the user's perspective.
 *
 * @param window The window to set the geometry of
 * @param x The left point of the window geometry
 * @param y The top point of the window geometry
 * @param w The width of the window geometry
 * @param h The height of the window geometry
 *
 * @ingroup Efl_Core_Wayland_Window_Group
 * @since 1.17
 */
EAPI void efl_core_wayland_window_geometry_set(Efl_Core_Wayland_Window *window, int x, int y, int w, int h);

/**
 * Set the type of a given window
 *
 * @param window The window to set the type of
 * @param type The Efl_Core_Wayland_Window_Type to set on the window
 *
 * @ingroup Efl_Core_Wayland_Window_Group
 * @since 1.17
 */
EAPI void efl_core_wayland_window_type_set(Efl_Core_Wayland_Window *window, Efl_Core_Wayland_Window_Type type);

/**
 * Get the type of a given window
 *
 * @see Efl_Core_Wayland_Window_Type
 *
 * @ingroup Efl_Core_Wayland_Window_Group
 * @since 1.24
 */
EAPI Efl_Core_Wayland_Window_Type efl_core_wayland_window_type_get(Efl_Core_Wayland_Window *window);

/**
 * Find the output that a given window is on
 *
 * @param window The window to find the output for
 *
 * @return An Efl_Core_Wayland_Output if found, or NULL otherwise
 *
 * @ingroup Efl_Core_Wayland_Window_Group
 * @since 1.20
 */
EAPI Efl_Core_Wayland_Output *efl_core_wayland_window_output_find(Efl_Core_Wayland_Window *window);

/**
 * Set if window rotation is supported by the window manager
 *
 * @param window
 * @param enabled
 *
 * @ingroup Efl_Core_Wayland_Window_Group
 * @since 1.20
 */
EAPI void efl_core_wayland_window_wm_rotation_supported_set(Efl_Core_Wayland_Window *window, Efl_Bool enabled);

/**
 * Get if window rotation is supported by the window manager
 *
 * @param window
 *
 * @return EFL_TRUE if supported, EFL_FALSE otherwise
 *
 * @ingroup Efl_Core_Wayland_Window_Group
 * @since 1.20
 */
EAPI Efl_Bool efl_core_wayland_window_wm_rotation_supported_get(Efl_Core_Wayland_Window *window);

/**
 * Set if an application has set window rotation
 *
 * @param window
 * @param set
 *
 * @ingroup Efl_Core_Wayland_Window_Group
 * @since 1.20
 */
EAPI void efl_core_wayland_window_rotation_app_set(Efl_Core_Wayland_Window *window, Efl_Bool set);

/**
 * Get if an application has set window rotation
 *
 * @param window
 *
 * @return EFL_TRUE if set, EFL_FALSE otherwise
 *
 * @ingroup Efl_Core_Wayland_Window_Group
 * @since 1.20
 */
EAPI Efl_Bool efl_core_wayland_window_rotation_app_get(Efl_Core_Wayland_Window *window);

/**
 * Set preferred rotation on a given window
 *
 * @param window The window to set preferred rotation on
 * @param rot The value of the preferred rotation to set
 *
 * @ingroup Efl_Core_Wayland_Window_Group
 * @since 1.20
 */
EAPI void efl_core_wayland_window_preferred_rotation_set(Efl_Core_Wayland_Window *window, int rot);

/**
 * Get preferred rotation for a given window
 *
 * @param window The window to get preferred rotation from
 *
 * @return Given windows preferred rotation
 *
 * @ingroup Efl_Core_Wayland_Window
 * @since 1.20
 */
EAPI int efl_core_wayland_window_preferred_rotation_get(Efl_Core_Wayland_Window *window);

/**
 * Set a windows available rotations
 *
 * @param window
 * @param rots
 * @param count
 *
 * @ingroup Efl_Core_Wayland_Window_Group
 * @since 1.20
 */
EAPI void efl_core_wayland_window_available_rotations_set(Efl_Core_Wayland_Window *window, const int *rots, unsigned int count);

/**
 * Get a windows available rotations
 *
 * @param window
 * @param rots
 * @param count
 *
 * @return EFL_TRUE on success, EFL_FALSE otherwise
 *
 * @ingroup Efl_Core_Wayland_Window_Group
 * @since 1.20
 */
EAPI Efl_Bool efl_core_wayland_window_available_rotations_get(Efl_Core_Wayland_Window *window, int **rots, unsigned int *count);

/**
 * @brief Get the activated state of a window
 *
 * @param window The window to get activated state from
 *
 * @return @c EFL_TRUE if activated
 *
 * @ingroup Efl_Core_Wayland_Window_Group
 * @since 1.20
 */
EAPI Efl_Bool efl_core_wayland_window_activated_get(const Efl_Core_Wayland_Window *window);

/**
 * @brief Set the seat for a popup window to be used with grab
 *
 * @param window The window
 * @param input The seat
 *
 * Use this function for desktop shell requests involving popup grabs which require
 * a seat for the grab.
 *
 * @ingroup Efl_Core_Wayland_Window_Group
 * @since 1.20
 */
EAPI void efl_core_wayland_window_popup_input_set(Efl_Core_Wayland_Window *window, Efl_Core_Wayland_Input *input);

/**
 * @brief Get the seat for a popup window to be used with grab
 *
 * @param window The window
 *
 * @return Returns Efl_Core_Wayland_Input if the window has an input.
 *
 * @ingroup Efl_Core_Wayland_Window_Group
 * @since 1.24
 */
EAPI Efl_Core_Wayland_Input *efl_core_wayland_window_popup_input_get(Efl_Core_Wayland_Window *window);

/**
 * Check if a window has a shell surface - without one it can't be visible.
 *
 * @param win The window to check
 *
 * @return Returns true if the window has an associated shell surface.
 *
 * @ingroup Efl_Core_Wayland_Window_Group
 * @since 1.19
 */
EAPI Efl_Bool efl_core_wayland_window_shell_surface_exists(Efl_Core_Wayland_Window *win);

/**
 * Get which display a given window is using
 *
 * @param window The window to get the display of
 *
 * @return The Efl_Core_Wayland_Display that this window is using, or NULL otherwise
 *
 * @ingroup Efl_Core_Wayland_Window_Group
 * @since 1.17
 */
EAPI Efl_Core_Wayland_Display *efl_core_wayland_window_display_get(const Efl_Core_Wayland_Window *window);

/**
 * Set if this window should ignore focus requests
 *
 * @param window The window to set ignore focus requests on
 * @param focus_skip EFL_TRUE if this window should skip focus requests, EFL_FALSE otherwise
 *
 * @ingroup Efl_Core_Wayland_Window_Group
 * @since 1.20
 */
EAPI void efl_core_wayland_window_focus_skip_set(Efl_Core_Wayland_Window *window, Efl_Bool focus_skip);

/**
 * Get if this window ignores focus requests
 *
 * @param window The window to get the focus skip value from
 *
 * @return EFL_TRUE if a window should skip focus requests, EFL_FALSE otherwise
 *
 * @ingroup Efl_Core_Wayland_Window_Group
 * @since 1.20
 */
EAPI Efl_Bool efl_core_wayland_window_focus_skip_get(Efl_Core_Wayland_Window *window);

/**
 * Set the role of a given window
 *
 * @param window
 * @param role
 *
 * @ingroup Efl_Core_Wayland_Window_Group
 * @since 1.20
 */
EAPI void efl_core_wayland_window_role_set(Efl_Core_Wayland_Window *window, const char *role);

/**
 * Get the role of a given window
 *
 * @param window The window to set the class role
 *
 * @return A string if found, or NULL otherwise
 *
 * @ingroup Efl_Core_Wayland_Window_Group
 * @since 1.24
 */
EAPI const char *efl_core_wayland_window_role_get(Efl_Core_Wayland_Window *window);

/**
 * Set if a given window is in floating mode
 *
 * @param window The window to set floating mode on
 * @param floating EFL_TRUE if this window should be in floating mode, EFL_FALSE otherwise
 *
 * @ingroup Efl_Core_Wayland_Window_Group
 * @since 1.20
 */
EAPI void efl_core_wayland_window_floating_mode_set(Efl_Core_Wayland_Window *window, Efl_Bool floating);

/**
 * Get if a given window is in floating mode
 *
 * @param window The window to get floating mode
 *
 * @return EFL_TRUE if floating, EFL_FALSE otherwise
 *
 * @ingroup Efl_Core_Wayland_Window_Group
 * @since 1.20
 */
EAPI Efl_Bool efl_core_wayland_window_floating_mode_get(Efl_Core_Wayland_Window *window);

/**
 * Finds a window by surface
 *
 * @param surface The surface to find the window of
 *
 * @ingroup Efl_Core_Wayland_Window_Group
 * @since 1.24
 */
EAPI Efl_Core_Wayland_Window *efl_core_wayland_window_surface_find(struct wl_surface *surface);

/**
 * @defgroup Efl_Core_Wayland_Input_Group Wayland Library Input Functions
 * @ingroup Efl_Core_Wayland_Group
 *
 * Functions that deal with creating, destroying, or interacting with
 * Wayland Input
 */

/**
 * Get the wl_seat that an input is using
 *
 * @param input The Efl_Core_Wayland_Input to get the seat of
 *
 * @return The wl_seat of this input, or NULL otherwise
 *
 * @ingroup Efl_Core_Wayland_Input_Group
 * @since 1.17
 */
EAPI struct wl_seat *efl_core_wayland_input_seat_get(Efl_Core_Wayland_Input *input);

/**
 * Get the seat capabilities for a given input.
 *
 * @param input The input
 *
 * @ingroup Efl_Core_Wayland_Input_Group
 * @since 1.19
 */
EAPI Efl_Core_Wayland_Seat_Capabilities efl_core_wayland_input_seat_capabilities_get(Efl_Core_Wayland_Input *input);

/**
 * Get the wayland's seat id from an input.
 *
 * @param input The input
 *
 * @return The seat id
 *
 * @ingroup Efl_Core_Wayland_Input_Group
 * @since 1.19
 */
EAPI unsigned int efl_core_wayland_input_seat_id_get(Efl_Core_Wayland_Input *input) EFL_WARN_UNUSED_RESULT;

/**
 * Get the display object of an input
 *
 * @param input The input
 *
 * @return The display
 *
 * @ingroup Efl_Core_Wayland_Input_Group
 * @since 1.20
 */
EAPI Efl_Core_Wayland_Display *efl_core_wayland_input_display_get(const Efl_Core_Wayland_Input *input);

/**
 * Get the xkb_keymap object of an input
 *
 * @param input The input
 *
 * @return The xkb_keymap object
 *
 * @ingroup Efl_Core_Wayland_Input_Group
 * @since 1.20
 */
EAPI struct xkb_keymap *efl_core_wayland_input_keymap_get(const Efl_Core_Wayland_Input *input);

/**
 * Get the name of an input
 *
 * @param input The input
 *
 * @return The name
 *
 * @ingroup Efl_Core_Wayland_Input_Group
 * @since 1.20
 */
EAPI Eina_Stringshare *efl_core_wayland_input_name_get(Efl_Core_Wayland_Input *input);

/**
 * Get the keyboard repeat rate and delay of an input
 *
 * @param input The input
 * @param rate Pointer to store the repeat rate (in seconds)
 * @param rate Pointer to store the repeat delay (in seconds)
 *
 * @return True if repeat is enabled
 *
 * @ingroup Efl_Core_Wayland_Input_Group
 * @since 1.20
 */
EAPI Efl_Bool efl_core_wayland_input_keyboard_repeat_get(const Efl_Core_Wayland_Input *input, double *rate, double *delay);

/**
 * Set the keyboard repeat rate and delay of an input
 * @param input The input
 * @param rate Pointer to store the repeat rate (in seconds)
 * @param rate Pointer to store the repeat delay (in seconds)
 * @return True if repeat is enabled
 * @ingroup Efl_Core_Wayland_Input_Group
 * @since 1.24
 */
EAPI Efl_Bool efl_core_wayland_input_keyboard_repeat_set(Efl_Core_Wayland_Input *input, double rate, double delay);

/**
 * Retrieves the mouse position of the seat
 *
 * @param input The seat
 * @param x where to return the horizontal position. May be NULL. Returns 0 on error.
 * @param y where to return the vertical position. May be NULL. Returns 0 on error.
 * @return @c EFL_TRUE if coords were successfully retrieved
 *
 * @ingroup Efl_Core_Wayland_Input_Group
 * @since 1.20
 */
EAPI Efl_Bool efl_core_wayland_input_pointer_xy_get(const Efl_Core_Wayland_Input *input, int *x, int *y);

/**
 * Set a given wl_surface to use as the pointer on a seat
 *
 * @param input The seat to set this surface as the pointer on
 * @param surface The surface to use as the pointer
 * @param hot_x The x coordinate to use as the cursor hot spot
 * @param hot_y The y coordinate to use as the cursor hot spot
 *
 * @ingroup Efl_Core_Wayland_Input_Group
 * @since 1.20
 */
EAPI void efl_core_wayland_input_pointer_set(Efl_Core_Wayland_Input *input, struct wl_surface *surface, int hot_x, int hot_y);

/**
 * Set a specific cursor on a given seat
 *
 * @brief This function will try to find a matching cursor inside the existing
 * cursor theme and set the pointer for the specified seat to be
 * the specified cursor
 *
 * @param input The seat to set the cursor on
 * @param cursor The name of the cursor to try and set
 *
 * @ingroup Efl_Core_Wayland_Input_Group
 * @since 1.20
 */
EAPI void efl_core_wayland_input_cursor_from_name_set(Efl_Core_Wayland_Input *input, const char *cursor);

/**
 * Gets default input of a given display
 *
 * @param ewd The display
 *
 * @ingroup Efl_Core_Wayland_Input_Group
 * @since 1.24
 */
EAPI Efl_Core_Wayland_Input *efl_core_wayland_input_default_input_get(const Efl_Core_Wayland_Display *ewd);

/**
 * @defgroup Efl_Core_Wayland_Output_Group Wayland Library Output Functions
 * @ingroup Efl_Core_Wayland_Group
 *
 * Functions that deal with creating, destroying, or interacting with
 * Wayland Outputs
 */

/**
 * Return the DPI of a given output
 *
 * This is a simplistic call to get DPI. It does not account for differing
 * DPI in the x and y axes nor does it account for multihead or xinerama and
 * xrandr where different parts of the screen may have different DPI etc.
 *
 * @param output The output to get the DPI of
 *
 * @return the general screen DPI (dots/pixels per inch).
 *
 * @ingroup Efl_Core_Wayland_Output_Group
 * @since 1.17
 */
EAPI int efl_core_wayland_output_dpi_get(Efl_Core_Wayland_Output *output);

/**
 * Get the current transform of a given output
 *
 * @param output The output to get the transform of
 *
 * @return The output's current transform value
 *
 * @ingroup Efl_Core_Wayland_Output_Group
 * @since 1.20
 */
EAPI int efl_core_wayland_output_transform_get(Efl_Core_Wayland_Output *output);

/**
 * Return the version of the display's compositor object
 *
 * @param disp the display to get the compositor object version from
 *
 * @return the version of the display's compositor object
 *
 * @ingroup Efl_Core_Wayland_Display_Group
 * @since 1.17
 */
EAPI int efl_core_wayland_display_compositor_version_get(Efl_Core_Wayland_Display *disp);

/**
 * Disable session recovery for any further connections.  Must be called
 * before connecting.  This is irreversible and not intended for general
 * use.
 *
 * @since 1.19
 */
EAPI void efl_core_wayland_session_recovery_disable(void);

/**
 * Commit the surface of a wayland window.
 *
 * If flush is set this generates a wl_surface_commit(), otherwise it is
 * expected that some other call in the very near future (such as
 * eglSwapBuffers) will cause an implicit flush.
 *
 * A surface that has been commit will be in the "pending" state until
 * the compositor tells us it's time to draw again via a frame callback.
 *
 * @param window The window whose surface we want to commit
 * @param flush EFL_TRUE if we need to flush immediately.
 *
 * @since 1.21
 */
EAPI void efl_core_wayland_window_commit(Efl_Core_Wayland_Window *window, Efl_Bool flush);

/**
 * Add a callback that fires when the window's surface_frame callback fires
 *
 * @param window The window to add a callback on
 * @param cb The callback
 * @param data User data to provide to the callback handler
 *
 * @since 1.21
 */
EAPI Efl_Core_Wayland_Frame_Cb_Handle *efl_core_wayland_window_frame_callback_add(Efl_Core_Wayland_Window *window, Efl_Core_Wayland_Frame_Cb cb, void *data);

/**
 * Delete a callback that fires when the window's surface_frame callback fires
 *
 * @param handle The callback handle obtained through efl_core_wayland_window_frame_callback_add().
 *
 * @since 1.21
 */
EAPI void efl_core_wayland_window_frame_callback_del(Efl_Core_Wayland_Frame_Cb_Handle *handle);

/**
 * Push buffered wayland protocol to compositor
 *
 * Wayland protocol is only actually sent when a flush occurs,
 * so the display should be flushed at appropriate times, such
 * as after a commit.
 *
 * @param display The display to flush
 *
 * @since 1.21
 */
EAPI void efl_core_wayland_display_flush(Efl_Core_Wayland_Display *display);

/**
 * Get if a given window is resizing
 *
 * @param window The window to check for resizing
 *
 * @return EFL_TRUE if resizing, EFL_FALSE otherwise
 *
 * @ingroup Efl_Core_Wayland_Window_Group
 * @since 1.21
 */
EAPI Efl_Bool efl_core_wayland_window_resizing_get(Efl_Core_Wayland_Window *window);

/**
 * Latch window state at the start of an update
 *
 * When async render takes place we continue to dispatch wayland
 * events from the main loop. We need to defer any changes to
 * window state from those events until the update is complete.
 *
 * Events deferred during an update will automatically fire
 * immediately after the caller calls efl_core_wayland_window_commit.
 *
 * @param window The window whose state we want to latch
 *
 * @since 1.21
 */
EAPI void efl_core_wayland_window_update_begin(Efl_Core_Wayland_Window *window);

# endif

# undef EAPI
# define EAPI

/* # ifdef __cplusplus */
/* } */
/* # endif */

#endif

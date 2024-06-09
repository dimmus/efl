#ifndef EFL_CORE_WAYLAND_TEST_HELPERS_H
#define EFL_CORE_WAYLAND_TEST_HELPERS_H

#include <wayland-egl.h>

#ifdef GL_GLES
#  include <EGL/egl.h>
#  include <GLES2/gl2.h>
#endif

#define WIDTH 480
#define HEIGHT 360

typedef struct _Test_Data
{
  Efl_Core_Wayland_Display         *display;
  Efl_Core_Wayland_Window          *win;
  Efl_Core_Wayland_Frame_Cb_Handle *frame_callback_handler;
  Core_Event_Handler               *handler;

  struct wl_surface    *surface;
  struct wl_egl_window *egl_window;

  int width;
  int height;
  int frame_callback_count;

#ifdef GL_GLES
  EGLDisplay egl_display;
  EGLConfig  egl_conf;
  EGLSurface egl_surface;
  EGLContext egl_context;
#endif
} Test_Data;

static Efl_Core_Wayland_Display *
_display_connect(void)
{
  return efl_core_wayland_display_connect(NULL);
}

static Efl_Core_Wayland_Window *
_window_create(Efl_Core_Wayland_Display *disp)
{
  return efl_core_wayland_window_new(disp, NULL, 100, 100, WIDTH, HEIGHT);
}

#endif

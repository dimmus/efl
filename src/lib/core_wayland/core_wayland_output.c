#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include "core_wayland_private.h"

static void
_cb_geometry(void                       *data,
             struct wl_output *wl_output EFL_UNUSED,
             int                         x,
             int                         y,
             int                         w,
             int                         h,
             int subpixel                EFL_UNUSED,
             const char                 *make,
             const char                 *model,
             int                         transform)
{
  Efl_Core_Wayland_Output *output;
  int                      ot;

  output = data;
  if (!output) return;

  eina_stringshare_replace(&output->make, make);
  eina_stringshare_replace(&output->model, model);

  output->mw         = w;
  output->mh         = h;
  output->geometry.x = x;
  output->geometry.y = y;

  ot = output->transform;

  if (transform & 0x4) ERR("Cannot support output transformation");

  transform &= 0x3;
  if (output->transform != transform)
  {
    Efl_Core_Wayland_Event_Output_Transform *ev;

    output->transform = transform;

    ev = calloc(1, sizeof(Efl_Core_Wayland_Event_Output_Transform));
    if (ev)
    {
      ev->output        = output;
      ev->old_transform = ot;
      ev->transform     = transform;
      core_event_add(EFL_CORE_WAYLAND_EVENT_OUTPUT_TRANSFORM, ev, NULL, NULL);
    }
  }
}

static void
_cb_mode(void                       *data,
         struct wl_output *wl_output EFL_UNUSED,
         unsigned int                flags,
         int                         w,
         int                         h,
         int refresh                 EFL_UNUSED)
{
  Efl_Core_Wayland_Output *output;

  output = data;
  if (!output) return;

  if (flags & WL_OUTPUT_MODE_CURRENT)
  {
    output->geometry.w = w;
    output->geometry.h = h;
  }
}

static void
_cb_done(void *data EFL_UNUSED, struct wl_output *output EFL_UNUSED)
{
   /* NB: Use this event to raise any "output (re)configured events" */
}

static void
_cb_scale(void *data               EFL_UNUSED,
          struct wl_output *output EFL_UNUSED,
          int scale                EFL_UNUSED)
{}

static const struct wl_output_listener
  _output_listener = { _cb_geometry, _cb_mode, _cb_done,
                       _cb_scale,    NULL, /* name */
                       NULL /* description */ };

void
_efl_core_wayland_output_add(Efl_Core_Wayland_Display *display, unsigned int id)
{
  Efl_Core_Wayland_Output *output;

  output = calloc(1, sizeof(Efl_Core_Wayland_Output));
  if (!output) return;

  output->display = display;

  output->wl_output =
    wl_registry_bind(display->wl.registry, id, &wl_output_interface, 2);

  display->outputs =
    eina_inlist_append(display->outputs, EINA_INLIST_GET(output));

  wl_output_add_listener(output->wl_output, &_output_listener, output);
}

void
_efl_core_wayland_output_del(Efl_Core_Wayland_Output *output)
{
  Efl_Core_Wayland_Display *display;

  if (!output) return;

  display = output->display;

  if (output->wl_output) wl_output_destroy(output->wl_output);
  if (output->make) eina_stringshare_del(output->make);
  if (output->model) eina_stringshare_del(output->model);

  display->outputs =
    eina_inlist_remove(display->outputs, EINA_INLIST_GET(output));

  free(output);
}

Efl_Core_Wayland_Output *
_efl_core_wayland_output_find(Efl_Core_Wayland_Display *display,
                              struct wl_output         *op)
{
  Efl_Core_Wayland_Output *wlop;

  EINA_INLIST_FOREACH(display->outputs, wlop)
    if (wlop->wl_output == op) return wlop;

  return NULL;
}

EAPI int
efl_core_wayland_output_dpi_get(Efl_Core_Wayland_Output *output)
{
  int    w, h, mw, mh, dpi;
  double target;

  EINA_SAFETY_ON_NULL_RETURN_VAL(output, 75);

  mw = output->mw;
  if (mw <= 0) return 75;

  mh = output->mh;
  if (mh <= 0) return 75;

  w = output->geometry.w;
  h = output->geometry.h;

  target = (round((sqrt(mw * mw + mh * mh) / 25.4) * 10) / 10);
  dpi    = (round((sqrt(w * w + h * h) / target) * 10) / 10);

  return dpi;
}

EAPI int
efl_core_wayland_output_transform_get(Efl_Core_Wayland_Output *output)
{
  EINA_SAFETY_ON_NULL_RETURN_VAL(output, 0);
  return output->transform;
}

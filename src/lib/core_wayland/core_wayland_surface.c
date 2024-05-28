#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include "core_wayland_private.h"

#include <sys/types.h>
#include <sys/stat.h>

static Eina_List *_smanagers      = NULL;
static int        _smanager_count = 0;

EAPI void
efl_core_wayland_surface_destroy(Efl_Core_Wayland_Surface *surface)
{
    EINA_SAFETY_ON_NULL_RETURN(surface);

    core_event_handler_del(surface->offscreen_handler);
    surface->funcs->destroy(surface, surface->private_data);
    surface->wl_win->wl_surface = NULL;
    surface->wl_win             = NULL;

    free(surface);
   /* We took a reference to ecore_wl in surface create to prevent
    * modules unloading with surfaces in flight.  Release that now.
    */
    efl_core_wayland_shutdown();
}

EAPI void
efl_core_wayland_surface_reconfigure(Efl_Core_Wayland_Surface *surface,
                             int               w,
                             int               h,
                             uint32_t          flags,
                             Efl_Bool          alpha)
{
    EINA_SAFETY_ON_NULL_RETURN(surface);

    surface->funcs
        ->reconfigure(surface, surface->private_data, w, h, flags, alpha);
    surface->w     = w;
    surface->h     = h;
    surface->alpha = alpha;
}

EAPI void *
efl_core_wayland_surface_data_get(Efl_Core_Wayland_Surface *surface, int *w, int *h)
{
    EINA_SAFETY_ON_NULL_RETURN_VAL(surface, NULL);

    return surface->funcs->data_get(surface, surface->private_data, w, h);
}

EAPI int
efl_core_wayland_surface_assign(Efl_Core_Wayland_Surface *surface)
{
    EINA_SAFETY_ON_NULL_RETURN_VAL(surface, 0);

    return surface->funcs->assign(surface, surface->private_data);
}

EAPI void
efl_core_wayland_surface_post(Efl_Core_Wayland_Surface *surface,
                      Eina_Rectangle   *rects,
                      unsigned int      count)
{
    EINA_SAFETY_ON_NULL_RETURN(surface);

    surface->funcs->post(surface, surface->private_data, rects, count);
}

EAPI void
efl_core_wayland_surface_flush(Efl_Core_Wayland_Surface *surface, Efl_Bool purge)
{
    EINA_SAFETY_ON_NULL_RETURN(surface);

    surface->funcs->flush(surface, surface->private_data, purge);
}

static Efl_Bool
_efl_core_wayland_surface_cb_offscreen(void *data, int type EFL_UNUSED, void *event)
{
    Efl_Core_Wayland_Event_Window_Offscreen *ev   = event;
    Efl_Core_Wayland_Surface                *surf = data;

    if (surf->wl_win == ev->win) efl_core_wayland_surface_flush(surf, EFL_FALSE);

    return CORE_CALLBACK_RENEW;
}

EAPI Efl_Core_Wayland_Surface *
efl_core_wayland_surface_create(Efl_Core_Wayland_Window *win, Efl_Bool alpha)
{
    Efl_Core_Wayland_Surface           *out;
    Eina_List                  *l;
    Efl_Core_Wayland_Surface_Interface *intf;

    EINA_SAFETY_ON_NULL_RETURN_VAL(win, NULL);
    EINA_SAFETY_ON_NULL_RETURN_VAL(_smanagers, NULL);

    if (win->wl_surface) return win->wl_surface;

    out = calloc(1, sizeof(*out));
    if (!out) return NULL;

    out->wl_win = win;
    out->alpha  = alpha;
    out->w      = 0;
    out->h      = 0;

    EINA_LIST_FOREACH(_smanagers, l, intf)
    {
        out->private_data = intf->setup(win);
        if (out->private_data)
        {
            out->funcs      = intf;
            win->wl_surface = out;
            out->offscreen_handler =
                core_event_handler_add(EFL_CORE_WAYLAND_EVENT_WINDOW_OFFSCREEN,
                                       _efl_core_wayland_surface_cb_offscreen,
                                       out);
            /* Since we have loadable modules, we need to make sure this
              * surface keeps ecore_wl from de-initting and dlclose()ing
              * things until after it's destroyed
              */
            efl_core_wayland_init();
            return out;
        }
    }

    free(out);
    return NULL;
}

EAPI Efl_Core_Wayland_Buffer *
efl_core_wayland_surface_buffer_create(Efl_Core_Wayland_Surface *surface)
{
    Efl_Core_Wayland_Display *ewd;

    EINA_SAFETY_ON_NULL_RETURN_VAL(surface, NULL);

    ewd = efl_core_wayland_window_display_get(surface->wl_win);
    EINA_SAFETY_ON_NULL_RETURN_VAL(ewd, NULL);

    return efl_core_wayland_buffer_create(ewd, surface->w, surface->h, surface->alpha);
}

EAPI int
efl_core_wayland_surface_manager_add(Efl_Core_Wayland_Surface_Interface *intf)
{
    if (intf->version < EFL_CORE_WAYLAND_SURFACE_INTERFACE_VERSION) return 0;

    _smanagers = eina_list_prepend(_smanagers, intf);
    intf->id   = ++_smanager_count;
    return intf->id;
}

EAPI void
efl_core_wayland_surface_manager_del(Efl_Core_Wayland_Surface_Interface *intf)
{
    _smanagers = eina_list_remove(_smanagers, intf);
}

EAPI Efl_Core_Wayland_Window *
efl_core_wayland_surface_window_get(Efl_Core_Wayland_Surface *surface)
{
    EINA_SAFETY_ON_NULL_RETURN_VAL(surface, NULL);

    return surface->wl_win;
}

EAPI Efl_Bool
efl_core_wayland_surface_alpha_get(Efl_Core_Wayland_Surface *surface)
{
    EINA_SAFETY_ON_NULL_RETURN_VAL(surface, EFL_FALSE);

    return surface->alpha;
}

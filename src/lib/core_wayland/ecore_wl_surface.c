#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include "ecore_wl_private.h"

#include <sys/types.h>
#include <sys/stat.h>

static Eina_List *_smanagers      = NULL;
static int        _smanager_count = 0;

EAPI void
ecore_wl_surface_destroy(Ecore_Wl_Surface *surface)
{
    EINA_SAFETY_ON_NULL_RETURN(surface);

    ecore_event_handler_del(surface->offscreen_handler);
    surface->funcs->destroy(surface, surface->private_data);
    surface->wl_win->wl_surface = NULL;
    surface->wl_win             = NULL;

    free(surface);
   /* We took a reference to ecore_wl in surface create to prevent
    * modules unloading with surfaces in flight.  Release that now.
    */
    ecore_wl_shutdown();
}

EAPI void
ecore_wl_surface_reconfigure(Ecore_Wl_Surface *surface,
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
ecore_wl_surface_data_get(Ecore_Wl_Surface *surface, int *w, int *h)
{
    EINA_SAFETY_ON_NULL_RETURN_VAL(surface, NULL);

    return surface->funcs->data_get(surface, surface->private_data, w, h);
}

EAPI int
ecore_wl_surface_assign(Ecore_Wl_Surface *surface)
{
    EINA_SAFETY_ON_NULL_RETURN_VAL(surface, 0);

    return surface->funcs->assign(surface, surface->private_data);
}

EAPI void
ecore_wl_surface_post(Ecore_Wl_Surface *surface,
                      Eina_Rectangle   *rects,
                      unsigned int      count)
{
    EINA_SAFETY_ON_NULL_RETURN(surface);

    surface->funcs->post(surface, surface->private_data, rects, count);
}

EAPI void
ecore_wl_surface_flush(Ecore_Wl_Surface *surface, Efl_Bool purge)
{
    EINA_SAFETY_ON_NULL_RETURN(surface);

    surface->funcs->flush(surface, surface->private_data, purge);
}

static Efl_Bool
_ecore_wl_surface_cb_offscreen(void *data, int type EFL_UNUSED, void *event)
{
    Ecore_Wl_Event_Window_Offscreen *ev   = event;
    Ecore_Wl_Surface                *surf = data;

    if (surf->wl_win == ev->win) ecore_wl_surface_flush(surf, EFL_FALSE);

    return ECORE_CALLBACK_RENEW;
}

EAPI Ecore_Wl_Surface *
ecore_wl_surface_create(Ecore_Wl_Window *win, Efl_Bool alpha)
{
    Ecore_Wl_Surface           *out;
    Eina_List                  *l;
    Ecore_Wl_Surface_Interface *intf;

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
                ecore_event_handler_add(ECORE_WL2_EVENT_WINDOW_OFFSCREEN,
                                        _ecore_wl_surface_cb_offscreen,
                                        out);
            /* Since we have loadable modules, we need to make sure this
              * surface keeps ecore_wl from de-initting and dlclose()ing
              * things until after it's destroyed
              */
            ecore_wl_init();
            return out;
        }
    }

    free(out);
    return NULL;
}

EAPI Ecore_Wl_Buffer *
ecore_wl_surface_buffer_create(Ecore_Wl_Surface *surface)
{
    Ecore_Wl_Display *ewd;

    EINA_SAFETY_ON_NULL_RETURN_VAL(surface, NULL);

    ewd = ecore_wl_window_display_get(surface->wl_win);
    EINA_SAFETY_ON_NULL_RETURN_VAL(ewd, NULL);

    return ecore_wl_buffer_create(ewd, surface->w, surface->h, surface->alpha);
}

EAPI int
ecore_wl_surface_manager_add(Ecore_Wl_Surface_Interface *intf)
{
    if (intf->version < ECORE_WL2_SURFACE_INTERFACE_VERSION) return 0;

    _smanagers = eina_list_prepend(_smanagers, intf);
    intf->id   = ++_smanager_count;
    return intf->id;
}

EAPI void
ecore_wl_surface_manager_del(Ecore_Wl_Surface_Interface *intf)
{
    _smanagers = eina_list_remove(_smanagers, intf);
}

EAPI Ecore_Wl_Window *
ecore_wl_surface_window_get(Ecore_Wl_Surface *surface)
{
    EINA_SAFETY_ON_NULL_RETURN_VAL(surface, NULL);

    return surface->wl_win;
}

EAPI Efl_Bool
ecore_wl_surface_alpha_get(Ecore_Wl_Surface *surface)
{
    EINA_SAFETY_ON_NULL_RETURN_VAL(surface, EFL_FALSE);

    return surface->alpha;
}

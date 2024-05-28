#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include "core_wayland_private.h"

void
_efl_core_wayland_subsurf_unmap(Efl_Core_Wayland_Subsurface *subsurf)
{
    if (subsurf->wl.subsurface) wl_subsurface_destroy(subsurf->wl.subsurface);
    if (subsurf->wl.surface) wl_surface_destroy(subsurf->wl.surface);
    subsurf->wl.subsurface = NULL;
    subsurf->wl.surface    = NULL;
}

void
_efl_core_wayland_subsurf_free(Efl_Core_Wayland_Subsurface *subsurf)
{
    Efl_Core_Wayland_Window *parent;

    _efl_core_wayland_subsurf_unmap(subsurf);

    parent = subsurf->parent;
    if (parent)
    {
        parent->subsurfs =
            eina_inlist_remove(parent->subsurfs, EINA_INLIST_GET(subsurf));
    }

    free(subsurf);
}

EAPI Efl_Core_Wayland_Subsurface *
efl_core_wayland_subsurface_new(Efl_Core_Wayland_Window *window)
{
    Efl_Core_Wayland_Display    *display;
    Efl_Core_Wayland_Subsurface *subsurf;

    EINA_SAFETY_ON_NULL_RETURN_VAL(window, NULL);
    EINA_SAFETY_ON_NULL_RETURN_VAL(window->surface, NULL);

    display = window->display;

    EINA_SAFETY_ON_NULL_RETURN_VAL(display->wl.compositor, NULL);
    EINA_SAFETY_ON_NULL_RETURN_VAL(display->wl.subcompositor, NULL);

    subsurf = calloc(1, sizeof(Efl_Core_Wayland_Subsurface));
    if (!subsurf) return NULL;

    subsurf->parent = window;

    subsurf->wl.surface = wl_compositor_create_surface(display->wl.compositor);
    if (!subsurf->wl.surface)
    {
        ERR("Failed to create surface");
        goto surf_err;
    }

    subsurf->wl.subsurface =
        wl_subcompositor_get_subsurface(display->wl.subcompositor,
                                        subsurf->wl.surface,
                                        window->surface);
    if (!subsurf->wl.subsurface)
    {
        ERR("Could not create subsurface");
        goto sub_surf_err;
    }

   /* A sub-surface is initially in the synchronized mode. */
    subsurf->sync = EFL_TRUE;

    window->subsurfs =
        eina_inlist_append(window->subsurfs, EINA_INLIST_GET(subsurf));

    return subsurf;

sub_surf_err:
    wl_surface_destroy(subsurf->wl.surface);

surf_err:
    free(subsurf);
    return NULL;
}

EAPI void
efl_core_wayland_subsurface_del(Efl_Core_Wayland_Subsurface *subsurface)
{
    EINA_SAFETY_ON_NULL_RETURN(subsurface);

    _efl_core_wayland_subsurf_free(subsurface);
}

EAPI struct wl_surface *
efl_core_wayland_subsurface_surface_get(Efl_Core_Wayland_Subsurface *subsurface)
{
    EINA_SAFETY_ON_NULL_RETURN_VAL(subsurface, NULL);

    return subsurface->wl.surface;
}

EAPI void
efl_core_wayland_subsurface_position_set(
    Efl_Core_Wayland_Subsurface *subsurface,
    int                          x,
    int                          y)
{
    EINA_SAFETY_ON_NULL_RETURN(subsurface);
    EINA_SAFETY_ON_NULL_RETURN(subsurface->wl.subsurface);

    if ((subsurface->x == x) && (subsurface->y == y)) return;

    subsurface->x = x;
    subsurface->y = y;

    wl_subsurface_set_position(subsurface->wl.subsurface, x, y);
}

EAPI void
efl_core_wayland_subsurface_position_get(
    Efl_Core_Wayland_Subsurface *subsurface,
    int                         *x,
    int                         *y)
{
    EINA_SAFETY_ON_NULL_RETURN(subsurface);

    if (x) *x = subsurface->x;
    if (y) *y = subsurface->y;
}

EAPI void
efl_core_wayland_subsurface_place_above(Efl_Core_Wayland_Subsurface *subsurface,
                                        struct wl_surface           *surface)
{
    EINA_SAFETY_ON_NULL_RETURN(subsurface);
    EINA_SAFETY_ON_NULL_RETURN(surface);

    wl_subsurface_place_above(subsurface->wl.subsurface, surface);
}

EAPI void
efl_core_wayland_subsurface_place_below(Efl_Core_Wayland_Subsurface *subsurface,
                                        struct wl_surface           *surface)
{
    EINA_SAFETY_ON_NULL_RETURN(subsurface);
    EINA_SAFETY_ON_NULL_RETURN(surface);

    wl_subsurface_place_below(subsurface->wl.subsurface, surface);
}

EAPI void
efl_core_wayland_subsurface_sync_set(Efl_Core_Wayland_Subsurface *subsurface,
                                     Efl_Bool                     sync)
{
    EINA_SAFETY_ON_NULL_RETURN(subsurface);
    EINA_SAFETY_ON_NULL_RETURN(subsurface->wl.subsurface);

    sync = !!sync;
    if (subsurface->sync == sync) return;

    subsurface->sync = sync;

    if (subsurface->sync) wl_subsurface_set_sync(subsurface->wl.subsurface);
    else wl_subsurface_set_desync(subsurface->wl.subsurface);
}

EAPI void
efl_core_wayland_subsurface_opaque_region_set(
    Efl_Core_Wayland_Subsurface *subsurface,
    int                          x,
    int                          y,
    int                          w,
    int                          h)
{
    EINA_SAFETY_ON_NULL_RETURN(subsurface);
    EINA_SAFETY_ON_NULL_RETURN(subsurface->wl.subsurface);

    if ((w > 0) && (h > 0))
    {
        Efl_Core_Wayland_Window *parent;

        parent = subsurface->parent;
        if (parent)
        {
            struct wl_region *region;

            region =
                wl_compositor_create_region(parent->display->wl.compositor);
            if (!region)
            {
                ERR("Failed to create opaque region");
                return;
            }

            wl_region_add(region, x, y, w, h);
            wl_surface_set_opaque_region(subsurface->wl.surface, region);
            wl_region_destroy(region);
        }
    }
    else wl_surface_set_opaque_region(subsurface->wl.surface, NULL);
}

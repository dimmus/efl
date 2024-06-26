
static void
_zxdg_surface_cb_configure(void *data, struct zxdg_surface_v6 *zxdg_surface EFL_UNUSED, uint32_t serial)
{
   Efl_Core_Wayland_Window *window;

   window = data;
   window->def_config.serial = serial;

   if (window->pending.configure)
     {
        window->saved.w = window->set_config.geometry.w;
        window->saved.h = window->set_config.geometry.h;
     }
   if (window->pending.configure && window->updating)
     ERR("Window shouldn't be rendering before initial configure");

   if (!window->updating)
     _efl_core_wayland_window_configure_send(window);

   if (window->pending.configure)
     _configure_complete(window);
}

static const struct zxdg_surface_v6_listener _zxdg_surface_listener =
{
   _zxdg_surface_cb_configure,
};

static void
_zxdg_toplevel_cb_configure(void *data, struct zxdg_toplevel_v6 *zxdg_toplevel EFL_UNUSED, int32_t width, int32_t height, struct wl_array *states)
{
   Efl_Core_Wayland_Window *win = data;
   uint32_t *s;

   win->def_config.maximized = EFL_FALSE;
   win->def_config.fullscreen = EFL_FALSE;
   win->def_config.focused = EFL_FALSE;
   win->def_config.resizing = EFL_FALSE;
   win->def_config.geometry.w = width;
   win->def_config.geometry.h = height;

   wl_array_for_each(s, states)
     {
        switch (*s)
          {
           case ZXDG_TOPLEVEL_V6_STATE_MAXIMIZED:
             win->def_config.maximized = EFL_TRUE;
             break;
           case ZXDG_TOPLEVEL_V6_STATE_FULLSCREEN:
             win->def_config.fullscreen = EFL_TRUE;
             break;
           case ZXDG_TOPLEVEL_V6_STATE_RESIZING:
             win->def_config.resizing = EFL_TRUE;
             break;
           case ZXDG_TOPLEVEL_V6_STATE_ACTIVATED:
             win->def_config.focused = EFL_TRUE;
           default:
             break;
          }
     }
}

static void
_zxdg_toplevel_cb_close(void *data, struct zxdg_toplevel_v6 *zxdg_toplevel EFL_UNUSED)
{
   Efl_Core_Wayland_Window *win;

   win = data;
   if (!win) return;

   efl_core_wayland_window_free(win);
}

static const struct zxdg_toplevel_v6_listener _zxdg_toplevel_listener =
{
   _zxdg_toplevel_cb_configure,
   _zxdg_toplevel_cb_close,
};

static void
_zxdg_popup_cb_configure(void *data, struct zxdg_popup_v6 *zxdg_popup EFL_UNUSED, int32_t x EFL_UNUSED, int32_t y EFL_UNUSED, int32_t width, int32_t height)
{
   Efl_Core_Wayland_Window *win = data;
   win->def_config.geometry.w = width;
   win->def_config.geometry.h = height;
}

static void
_zxdg_popup_cb_done(void *data, struct zxdg_popup_v6 *zxdg_popup EFL_UNUSED)
{
   Efl_Core_Wayland_Window *win;

   win = data;
   if (!win) return;

   if (win->grab) _efl_core_wayland_input_ungrab(win->grab);
}

static const struct zxdg_popup_v6_listener _zxdg_popup_listener =
{
   _zxdg_popup_cb_configure,
   _zxdg_popup_cb_done,
};

static void
_window_v6_popup_create(Efl_Core_Wayland_Window *win)
{
   struct zxdg_positioner_v6 *pos;
   int ww, wh;

   EINA_SAFETY_ON_NULL_RETURN(win->parent);
   pos = zxdg_shell_v6_create_positioner(win->display->wl.zxdg_shell);
   if (!pos) return;

   efl_core_wayland_window_geometry_get(win, NULL, NULL, &ww, &wh);

   zxdg_positioner_v6_set_anchor_rect(pos, 0, 0, 1, 1);
   zxdg_positioner_v6_set_size(pos, ww, wh);
   zxdg_positioner_v6_set_anchor(pos, ZXDG_POSITIONER_V6_ANCHOR_TOP |
                                 ZXDG_POSITIONER_V6_ANCHOR_LEFT);
   zxdg_positioner_v6_set_gravity(pos, ZXDG_POSITIONER_V6_ANCHOR_BOTTOM |
                                  ZXDG_POSITIONER_V6_ANCHOR_RIGHT);

   win->zxdg_popup = zxdg_surface_v6_get_popup(win->zxdg_surface,
                               win->parent->zxdg_surface, pos);

   zxdg_positioner_v6_destroy(pos);
   if (win->grab)
     zxdg_popup_v6_grab(win->zxdg_popup, win->grab->wl.seat,
                        wl_display_get_serial(win->display->wl.display));
   zxdg_popup_v6_set_user_data(win->zxdg_popup, win);
   zxdg_popup_v6_add_listener(win->zxdg_popup, &_zxdg_popup_listener, win);

   win->pending.configure = EFL_TRUE;

   efl_core_wayland_window_commit(win, EFL_TRUE);
}

static void
_window_v6_shell_surface_create(Efl_Core_Wayland_Window *window)
{
   if (window->zxdg_surface) return;
   window->zxdg_surface =
     zxdg_shell_v6_get_xdg_surface(window->display->wl.zxdg_shell,
                                   window->surface);
   zxdg_surface_v6_set_user_data(window->zxdg_surface, window);
   zxdg_surface_v6_add_listener(window->zxdg_surface,
                                &_zxdg_surface_listener, window);

   window->zxdg_configure_ack = zxdg_surface_v6_ack_configure;
   window->pending.configure = EFL_TRUE;

   if (window->type == EFL_CORE_WAYLAND_WINDOW_TYPE_MENU)
     _window_v6_popup_create(window);
   else
     {
        struct zxdg_toplevel_v6 *ptop = NULL;

        window->zxdg_toplevel =
          zxdg_surface_v6_get_toplevel(window->zxdg_surface);
        zxdg_toplevel_v6_set_user_data(window->zxdg_toplevel, window);
        zxdg_toplevel_v6_add_listener(window->zxdg_toplevel,
                                      &_zxdg_toplevel_listener, window);

        if (window->deferred_minimize)
          zxdg_toplevel_v6_set_minimized(window->zxdg_toplevel);
        window->deferred_minimize = EFL_FALSE;

        if (window->title)
          zxdg_toplevel_v6_set_title(window->zxdg_toplevel, window->title);
        if (window->class)
          zxdg_toplevel_v6_set_app_id(window->zxdg_toplevel, window->class);

        window->zxdg_set_min_size = zxdg_toplevel_v6_set_min_size;
        window->zxdg_set_max_size = zxdg_toplevel_v6_set_max_size;

        if (window->parent)
          ptop = window->parent->zxdg_toplevel;

        if (ptop)
          zxdg_toplevel_v6_set_parent(window->zxdg_toplevel, ptop);

        if (window->set_config.maximized)
          zxdg_toplevel_v6_set_maximized(window->zxdg_toplevel);

        if (window->set_config.fullscreen)
          zxdg_toplevel_v6_set_fullscreen(window->zxdg_toplevel, NULL);
     }

   efl_core_wayland_window_commit(window, EFL_TRUE);
}

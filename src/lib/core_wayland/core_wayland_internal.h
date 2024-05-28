#ifndef _EFL_CORE_WAYLAND_INTERNAL_H
# define _EFL_CORE_WAYLAND_INTERNAL_H

# ifdef EAPI
#  undef EAPI
# endif

# ifdef _WIN32
#  ifdef EFL_BUILD
#   ifdef DLL_EXPORT
#    define EAPI __declspec(dllexport)
#   else
#    define EAPI
#   endif
#  else
#   define EAPI __declspec(dllimport)
#  endif
# else
#  ifdef __GNUC__
#   if __GNUC__ >= 4
#    define EAPI __attribute__ ((visibility("default")))
#   else
#    define EAPI
#   endif
#  else
#   define EAPI
#  endif
# endif

EAPI extern int EFL_CORE_WAYLAND_EVENT_WINDOW_ROTATION_CHANGE_PREPARE;      /** @since 1.20 */
EAPI extern int EFL_CORE_WAYLAND_EVENT_WINDOW_ROTATION_CHANGE_PREPARE_DONE; /** @since 1.20 */
EAPI extern int EFL_CORE_WAYLAND_EVENT_WINDOW_ROTATION_CHANGE_REQUEST;      /** @since 1.20 */
EAPI extern int EFL_CORE_WAYLAND_EVENT_WINDOW_ROTATION_CHANGE_DONE;         /** @since 1.20 */

EAPI void efl_core_wayland_window_rotation_change_prepare_send       (Efl_Core_Wayland_Window *window, int rot, int w, int h, Efl_Bool resize);
EAPI void efl_core_wayland_window_rotation_change_prepare_done_send  (Efl_Core_Wayland_Window *window, int rot);
EAPI void efl_core_wayland_window_rotation_change_request_send       (Efl_Core_Wayland_Window *window, int rot);
EAPI void efl_core_wayland_window_rotation_change_done_send          (Efl_Core_Wayland_Window *window, int rot, int w, int h);
EAPI void efl_core_wayland_window_false_commit                       (Efl_Core_Wayland_Window *window);
EAPI void efl_core_wayland_window_damage                             (Efl_Core_Wayland_Window *window, Eina_Rectangle *rects, unsigned int count);

EAPI Efl_Bool                  efl_core_wayland_buffer_fit           (Efl_Core_Wayland_Buffer *b, int w, int h);
EAPI Efl_Bool                  efl_core_wayland_buffer_init          (Efl_Core_Wayland_Display *ewd, Efl_Core_Wayland_Buffer_Type types);
EAPI Efl_Core_Wayland_Buffer  *efl_core_wayland_buffer_create        (Efl_Core_Wayland_Display *ewd, int w, int h, Efl_Bool alpha);
EAPI void                      efl_core_wayland_buffer_destroy       (Efl_Core_Wayland_Buffer *b);
EAPI struct wl_buffer         *efl_core_wayland_buffer_wl_buffer_get (Efl_Core_Wayland_Buffer *buf);
EAPI void                     *efl_core_wayland_buffer_map           (Efl_Core_Wayland_Buffer *buf, int *w, int *h, int *stride);
EAPI void                      efl_core_wayland_buffer_unmap         (Efl_Core_Wayland_Buffer *buf);
EAPI void                      efl_core_wayland_buffer_discard       (Efl_Core_Wayland_Buffer *buf);
EAPI void                      efl_core_wayland_buffer_lock          (Efl_Core_Wayland_Buffer *b);
EAPI void                      efl_core_wayland_buffer_unlock        (Efl_Core_Wayland_Buffer *b);
EAPI void                      efl_core_wayland_buffer_destroy       (Efl_Core_Wayland_Buffer *b);
EAPI Efl_Bool                  efl_core_wayland_buffer_busy_get      (Efl_Core_Wayland_Buffer *buffer);
EAPI void                      efl_core_wayland_buffer_busy_set      (Efl_Core_Wayland_Buffer *buffer);
EAPI int                       efl_core_wayland_buffer_age_get       (Efl_Core_Wayland_Buffer *buffer);
EAPI void                      efl_core_wayland_buffer_age_set       (Efl_Core_Wayland_Buffer *buffer, int age);
EAPI void                      efl_core_wayland_buffer_age_inc       (Efl_Core_Wayland_Buffer *buffer);

EAPI Efl_Core_Wayland_Surface *efl_core_wayland_surface_create       (Efl_Core_Wayland_Window *win, Efl_Bool alpha);
EAPI void                      efl_core_wayland_surface_destroy      (Efl_Core_Wayland_Surface *surface);
EAPI void                      efl_core_wayland_surface_reconfigure  (Efl_Core_Wayland_Surface *surface, int w, int h, uint32_t flags, Efl_Bool alpha);
EAPI void                     *efl_core_wayland_surface_data_get     (Efl_Core_Wayland_Surface *surface, int *w, int *h);
EAPI int                       efl_core_wayland_surface_assign       (Efl_Core_Wayland_Surface *surface);
EAPI void                      efl_core_wayland_surface_post         (Efl_Core_Wayland_Surface *surface, Eina_Rectangle *rects, unsigned int count);
EAPI void                      efl_core_wayland_surface_flush        (Efl_Core_Wayland_Surface *surface, Efl_Bool purge);
EAPI Efl_Core_Wayland_Buffer  *efl_core_wayland_surface_buffer_create(Efl_Core_Wayland_Surface *surface);
EAPI int                       efl_core_wayland_surface_manager_add  (Efl_Core_Wayland_Surface_Interface *intf);
EAPI void                      efl_core_wayland_surface_manager_del  (Efl_Core_Wayland_Surface_Interface *intf);
EAPI Efl_Core_Wayland_Window  *efl_core_wayland_surface_window_get   (Efl_Core_Wayland_Surface *surface);
EAPI Efl_Bool                  efl_core_wayland_surface_alpha_get    (Efl_Core_Wayland_Surface *surface);
EAPI void                      efl_core_wayland_window_surface_flush (Efl_Core_Wayland_Window *window, Efl_Bool purge);

/**
 * Attach a buffer to a window
 *
 * Note that the GL stack my attach buffers to a surface - we should call this
 * function at that time (with a NULL buffer) to track whether a surface
 * has a valid buffer.  That is, call with implicit true and buffer NULL at
 * the time of glSwapBuffers.
 *
 * @param window The target window
 * @param buffer The buffer to attach
 * @param x X offset from corner
 * @param y Y offset from corner
 * @param implicit True if an external library is doing the actual attaching
 *
 * @since 1.21
 */
EAPI void efl_core_wayland_window_buffer_attach(Efl_Core_Wayland_Window *win, void *buffer, int x, int y, Efl_Bool implicit);

/**
 * Set a buffer transform on a given window
 *
 * @param window The window on which to set the buffer transform
 * @param transform The buffer transform being requested
 *
 * @ingroup Efl_Core_Wayland_Window_Group
 * @since 1.20
 */
EAPI void efl_core_wayland_window_buffer_transform_set(Efl_Core_Wayland_Window *window, int transform);

/**
 * Iconify a window
 *
 * @param window The window to iconifiy
 * @param iconified The new iconified state to set
 *
 * @ingroup Efl_Core_Wayland_Window_Group
 * @since 1.17
 */
EAPI void efl_core_wayland_window_iconified_set(Efl_Core_Wayland_Window *window, Efl_Bool iconified);

/**
 * Check if a wayland window's surface is in the pending state.
 *
 * A surface is pending if it's been commit but we haven't received a
 * frame callback for it yet.  This mean's we're not ready to draw yet.
 *
 * @param window The window whose surface we want to check
 *
 * @return whether the window's surface is pending or not.
 *
 * @since 1.21
 */
EAPI Efl_Bool efl_core_wayland_window_pending_get(Efl_Core_Wayland_Window *window);

/**
 * @defgroup Efl_Core_Wayland_Dnd_Group Wayland Library Drag-n-Drop Functions
 * @ingroup Efl_Core_Wayland_Group
 *
 * Functions that deal with creating, destroying, or interacting with
 * Wayland Drag-n-Drop
 */

/**
 * Set the types which are supported by a possible drag and drop operation.
 * This call initializes a data source and offeres the given mimetypes
 *
 * @param input the input where to add on the data source
 * @param types a null-terminated array of mimetypes
 *
 * @ingroup Efl_Core_Wayland_Dnd_Group
 * @since 1.17
 */
EAPI void efl_core_wayland_dnd_drag_types_set(Efl_Core_Wayland_Input *input, const char **types);

/**
 * Start a drag on the given input
 *
 * @param input the input to use
 * @param window the window which is the origin of the drag operation
 * @param drag_window the window which is used as window of the visible hint.
 *
 * @return The serial for the start_drag request
 *
 * @ingroup Efl_Core_Wayland_Dnd_Group
 * @since 1.17
 */
EAPI uint32_t efl_core_wayland_dnd_drag_start(Efl_Core_Wayland_Input *input, Efl_Core_Wayland_Window *window, Efl_Core_Wayland_Window *drag_window);

/**
 * Call wl_data_source.set_actions on an existing source
 *
 * @param input the input to use
 *
 * @see efl_core_wayland_dnd_drag_start for a more useful function.
 *
 * @ingroup Efl_Core_Wayland_Dnd_Group
 * @since 1.20
 */
EAPI void efl_core_wayland_dnd_set_actions(Efl_Core_Wayland_Input *input);

/**
 * End a drag started by a call to efl_core_wayland_dnd_drag_start
 *
 * @param input the input object on which the drag was started
 *
 * @ingroup Efl_Core_Wayland_Dnd_Group
 * @since 1.17
 */
EAPI void efl_core_wayland_dnd_drag_end(Efl_Core_Wayland_Input *input);

/**
 * Get the offer which is currently resposible for the clipboard
 *
 * @param input the input object to use
 *
 * @ingroup Efl_Core_Wayland_Dnd_Group
 * @since 1.19
 */
EAPI Efl_Core_Wayland_Offer* efl_core_wayland_dnd_selection_get(Efl_Core_Wayland_Input *input);

/**
 * Set the types which are available from this client
 * Later the event EFL_CORE_WAYLAND_EVENT_DATA_SOURCE_SEND is emitted,
 * where the caller of this api must write the data (encoded in the given mimetype) to the fd
 *
 * @param input the input to provice this types on
 * @param types a null-terminated array of mimetypes supported by the client
 *
 * @return serial of request on success, 0 on failure
 *
 * @ingroup Efl_Core_Wayland_Dnd_Group
 * @since 1.17
 */
EAPI uint32_t efl_core_wayland_dnd_selection_set(Efl_Core_Wayland_Input *input, const char **types);

/**
 * Clear the selection currently setted on this input.
 *
 * @param input the input to clear
 *
 * @return serial of request on success, 0 on failure
 *
 * @ingroup Efl_Core_Wayland_Dnd_Group
 * @since 1.17
 */
EAPI uint32_t efl_core_wayland_dnd_selection_clear(Efl_Core_Wayland_Input *input);

/**
 * Get the actions available from the data source
 *
 * @param offer Offer object to use
 *
 * @return or´ed values from Efl_Core_Wayland_Drag_Action which are describing the available actions
 *
 * @ingroup Efl_Core_Wayland_Dnd_Group
 * @since 1.19
 */
EAPI Efl_Core_Wayland_Drag_Action efl_core_wayland_offer_actions_get(Efl_Core_Wayland_Offer *offer);

/**
 * Set the actions which are supported by you
 *
 * @param offer Offer object to use
 * @param actions A or´ed value of mutliple Efl_Core_Wayland_Drag_Action values
 * @param action the preferred action out of the actions
 *
 * @ingroup Efl_Core_Wayland_Dnd_Group
 * @since 1.19
 */
EAPI void efl_core_wayland_offer_actions_set(Efl_Core_Wayland_Offer *offer, Efl_Core_Wayland_Drag_Action actions, Efl_Core_Wayland_Drag_Action action);

/**
 * Get action which is set by either the data source or in the last call of actions_set
 *
 * @param offer Offer object to use
 *
 * @return the preferred action
 *
 * @ingroup Efl_Core_Wayland_Dnd_Group
 * @since 1.19
 */
EAPI Efl_Core_Wayland_Drag_Action efl_core_wayland_offer_action_get(Efl_Core_Wayland_Offer *offer);

/**
 * Get the mime types which are given by the source
 *
 * @param offer the offer to query
 *
 * @return a eina array of strdup´ed strings, this array must NOT be changed or freed
 *
 * @ingroup Efl_Core_Wayland_Dnd_Group
 * @since 1.19
 */
EAPI Eina_Array* efl_core_wayland_offer_mimes_get(Efl_Core_Wayland_Offer *offer);

/**
 * Set mimetypes you are accepting under this offer
 *
 * @param offer the offer to use
 *
 * @ingroup Efl_Core_Wayland_Dnd_Group
 * @since 1.19
 */
EAPI void efl_core_wayland_offer_mimes_set(Efl_Core_Wayland_Offer *offer, Eina_Array *mimes);

/**
 * Accept a single mime type for an offer
 *
 * @param offer the offer to use
 * @param mime_type the mime type
 *
 * @ingroup Efl_Core_Wayland_Dnd_Group
 * @since 1.20
 */
EAPI void efl_core_wayland_offer_accept(Efl_Core_Wayland_Offer *offer, const char *mime_type);

/**
 * Request the data from this offer.
 * The event EFL_CORE_WAYLAND_EVENT_OFFER_DATA_READY is called when the data is available.
 * There offer will be not destroyed as long as requested data is not emitted by the event.
 *
 * @param offer the offer to use
 * @param mime the mimetype to receive
 *
 * @ingroup Efl_Core_Wayland_Dnd_Group
 * @since 1.19
 */
EAPI void efl_core_wayland_offer_receive(Efl_Core_Wayland_Offer *offer, char *mime);

/**
 * Request the data from this offer on an externally managed fd.
 * The event EFL_CORE_WAYLAND_EVENT_OFFER_DATA_READY is called when the data is available.
 * There offer will be not destroyed as long as requested data is not emitted by the event.
 *
 * @param offer the offer to use
 * @param mime the mimetype to receive
 * @param fd the fd to pass for receiving
 *
 * @ingroup Efl_Core_Wayland_Dnd_Group
 * @since 1.20
 */
EAPI void efl_core_wayland_offer_proxy_receive(Efl_Core_Wayland_Offer *offer, const char *mime, int fd);

/**
 * End the use of a proxy received offer. This may invalidate the offer object
 *
 * @param offer the offer
 *
 * @ingroup Efl_Core_Wayland_Dnd_Group
 * @since 1.20
 */
EAPI void efl_core_wayland_offer_proxy_receive_end(Efl_Core_Wayland_Offer *offer);

/**
 * Check if the given offer supports the given mimetype
 *
 * @param offer the offer to use
 * @param mime the mimetype to check
 *
 * @return Returns true if the mimetype is supported by this offer, false if not
 *
 * @ingroup Efl_Core_Wayland_Dnd_Group
 * @since 1.19
 */
EAPI Efl_Bool efl_core_wayland_offer_supports_mime(Efl_Core_Wayland_Offer *offer, const char *mime);

/**
 * Mark this offer as finished
 * This will call the dnd_finished event on the source of the sender.
 *
 * @param offer the offer to use
 *
 * @ingroup Efl_Core_Wayland_Dnd_Group
 * @since 1.19
 */
EAPI void efl_core_wayland_offer_finish(Efl_Core_Wayland_Offer *offer);

/**
 * @defgroup Efl_Core_Wayland_Subsurface_Group Functions to manipulate subsurfaces.
 * @ingroup Efl_Core_Wayland_Group
 *
 * Functions to manipulate wayland subsurfaces, using Efl_Core_Wayland_Subsurface.
 *
 * This API is intended to expose Wayland subsurface functionality, although it
 * should not be necessary for most applications to use it, as soon as we have
 * means to make Evas automatically switch Evas images to use subsurfaces.
 *
 * It can/should be used, for instance, when subsurfaces are needed to be not
 * in sync with the main window surface.
 */

/**
 * Create and return a new subsurface.
 *
 * Create a new surface (and subsurface interface), with the parent surface
 * being the one associated with the given window.
 *
 * @param window The window. It must be visible, otherwise there will be no surface created
 * for it yet.
 *
 * @return the allocated and initialized Efl_Core_Wayland_Subsurface object, or
 * NULL on failure
 *
 * @ingroup Efl_Core_Wayland_Subsurface_Group
 * @since 1.17
 */
EAPI Efl_Core_Wayland_Subsurface *efl_core_wayland_subsurface_new(Efl_Core_Wayland_Window *window);

/**
 * Destroy the given subsurface, as well as the surface associated with it.
 *
 * @param subsurface the subsurface
 *
 * @ingroup Efl_Core_Wayland_Subsurface_Group
 * @since 1.17
 */
EAPI void efl_core_wayland_subsurface_del(Efl_Core_Wayland_Subsurface *subsurface);

/**
 * Get the wl_surface for this subsurface
 *
 * @param subsurface the subsurface
 *
 * @return the wl_surface associated with this subsurface, or NULL on failure
 *
 * @ingroup Efl_Core_Wayland_Subsurface_Group
 * @since 1.17
 */
EAPI struct wl_surface *efl_core_wayland_subsurface_surface_get(Efl_Core_Wayland_Subsurface *subsurface);

/**
 * Set the position of this subsurface, relative to its parent surface.
 * If subsurface is defined and the x, y coordinates differ from the currently
 * tracked position, this also schedules a sub-surface position change.
 *
 * @param subsurface the subsurface
 * @param x coordinate in the parent surface
 * @param y coordinate in the parent surface
 *
 * @ingroup Efl_Core_Wayland_Subsurface_Group
 * @since 1.17
 */
EAPI void efl_core_wayland_subsurface_position_set(Efl_Core_Wayland_Subsurface *subsurface, int x, int y);

/**
 * Get the position of this subsurface, relative to its parent surface.
 * Coordinates will be returned in x and y if non-NULL.
 *
 * @param subsurface the subsurface
 * @param x coordinate in the parent surface, or NULL to ignore
 * @param y coordinate in the parent surface, or NULL to ignore
 *
 * @ingroup Efl_Core_Wayland_Subsurface_Group
 * @since 1.17
 */
EAPI void efl_core_wayland_subsurface_position_get(Efl_Core_Wayland_Subsurface *subsurface, int *x, int *y);

/**
 * Place subsurface on layer above a reference surface
 *
 * Moves the subsurface to just above the reference surface,
 * changing the z-order.  The reference surface must
 * be either a sibling or parent surface, else a protocol error will
 * be generated.
 *
 * @param subsurface the subsurface
 * @param surface the sibling reference surface
 *
 * @ingroup Efl_Core_Wayland_Subsurface_Group
 * @since 1.17
 */
EAPI void efl_core_wayland_subsurface_place_above(Efl_Core_Wayland_Subsurface *subsurface, struct wl_surface *surface);

/**
 * Place subsurface on layer below a reference surface
 *
 * See efl_core_wayland_subsurface_place_above.
 *
 * @param subsurface the subsurface
 * @param surface the sibling reference surface
 *
 * @ingroup Efl_Core_Wayland_Subsurface_Group
 * @since 1.17
 */
EAPI void efl_core_wayland_subsurface_place_below(Efl_Core_Wayland_Subsurface *subsurface, struct wl_surface *surface);

/**
 * Enables or disables sub-surface synchronization
 *
 * When synchronization is enabled, surface commits on the subsurface
 * will be cached and only applied when the parent surface's state is
 * applied.  This ensures atomic updates of the parent and all of its
 * synchronized sub-surfaces.
 *
 * When synchronization is disabled, commits will apply to the pending
 * state directly without caching, just like a normal wl_surface.  If
 * there are already cached events when this is set, those events are
 * applied simultaneously with the desync event.
 *
 * Attempting to enable synchronization when the subsurface already
 * thinks it's sync'd, or desync when it believes its desync'd, will
 * be trivially ignored and will not generate a Wayland event.
 *
 * See Wayland's set_desync documentation for further details and
 * exceptional cases.
 *
 * @param subsurface the subsurface
 * @param sync true to enable synchronization, false to desynchronize
 *
 * @ingroup Efl_Core_Wayland_Subsurface_Group
 * @since 1.17
 */
EAPI void efl_core_wayland_subsurface_sync_set(Efl_Core_Wayland_Subsurface *subsurface, Efl_Bool sync);

/**
 * Set an opaque region for the given subsurface.
 *
 * This is an optimization hint to the compositor to allow it avoid
 * redrawing content unnecessarily.  Note that marking transparent
 * content as opaque will cause repaint artifacts.
 *
 * Use a 0x0 region size to unset the opaque region.
 *
 * @param subsurface the subsurface
 * @param x coordinate in the parent surface
 * @param y coordinate in the parent surface
 * @param w width to set as opaque
 * @param h height to set as opaque
 *
 * @ingroup Efl_Core_Wayland_Subsurface_Group
 * @since 1.17
 */
EAPI void efl_core_wayland_subsurface_opaque_region_set(Efl_Core_Wayland_Subsurface *subsurface, int x, int y, int w, int h);

/**
 * Get list of supported auxiliary window hints
 *
 * @param window
 *
 * @return An Eina_List of supported auxiliary hints, or NULL otherwise
 *
 * @ingroup Efl_Core_Wayland_Window_Group
 * @since 1.20
 */
EAPI Eina_List *efl_core_wayland_window_aux_hints_supported_get(Efl_Core_Wayland_Window *window);

/**
 * Add a supported auxiliary hint to a given window
 *
 * @param window
 * @param id
 * @param hint
 * @param val
 *
 * @ingroup Efl_Core_Wayland_Window_Group
 * @since 1.20
 */
EAPI void efl_core_wayland_window_aux_hint_add(Efl_Core_Wayland_Window *window, int id, const char *hint, const char *val);

/**
 * Change an auxiliary hint on a given window
 *
 * @param window
 * @param id
 * @param val
 *
 * @ingroup Efl_Core_Wayland_Window_Group
 * @since 1.20
 */
EAPI void efl_core_wayland_window_aux_hint_change(Efl_Core_Wayland_Window *window, int id, const char *val);

/**
 * Delete an auxiliary hint on a given window
 *
 * @param window
 * @param id
 *
 * @ingroup Efl_Core_Wayland_Window_Group
 * @since 1.20
 */
EAPI void efl_core_wayland_window_aux_hint_del(Efl_Core_Wayland_Window *window, int id);

/**
 * Terminate a Wayland display's main loop
 *
 * @brief This function is typically used by servers to terminate the
 * Wayland display main loop. This is usually only called when a server
 * encounters an error.
 *
 * @param display The Efl_Core_Wayland_Display to terminate
 *
 * @ingroup Efl_Core_Wayland_Display_Group
 * @since 1.17
 */
EAPI void efl_core_wayland_display_terminate(Efl_Core_Wayland_Display *display);

# undef EAPI
# define EAPI

#endif

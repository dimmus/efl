#ifndef EFL_DBUS_SIGNAL_HANDLER_H
#define EFL_DBUS_SIGNAL_HANDLER_H 1

/**
 * @defgroup Efl_Dbus_Signal_Handler Signal Handler
 * @ingroup Eldbus
 *
 * @{
 */

/**
 * @brief Add a signal handler.
 *
 * @param conn connection where the signal is emitted
 * @param sender bus name or unique id of where the signal is emitted
 * @param path path of remote object
 * @param interface that signal belongs
 * @param member name of the signal
 * @param cb callback that will be called when this signal is received
 * @param cb_data data that will be passed to callback
 *
 * @return signal handler
 */
EAPI Efl_Dbus_Signal_Handler *
efl_dbus_signal_handler_add(Efl_Dbus_Connection *conn,
                            const char          *sender,
                            const char          *path,
                            const char          *interface,
                            const char          *member,
                            Efl_Dbus_Signal_Cb   cb,
                            const void *cb_data) EINA_ARG_NONNULL(1, 6);

/**
 * @brief Increase signal handler reference.
 *
 * @param handler signal handler
 *
 * @return the same signal handler, @p handler
 */
EAPI Efl_Dbus_Signal_Handler *
efl_dbus_signal_handler_ref(Efl_Dbus_Signal_Handler *handler)
  EINA_ARG_NONNULL(1);

/**
 * @brief Decrease signal handler reference.
 * If reference == 0 signal handler will be freed.
 *
 * @param handler signal handler
 */
EAPI void efl_dbus_signal_handler_unref(Efl_Dbus_Signal_Handler *handler)
  EINA_ARG_NONNULL(1);

/**
 * @brief Decrease signal handler reference like efl_dbus_signal_handler_unref()
 * but if reference > 0 this signal handler will stop listening to signals. In other
 * words it will be canceled but memory will not be freed.
 *
 * @param handler signal handler to delete
 */
EAPI void efl_dbus_signal_handler_del(Efl_Dbus_Signal_Handler *handler)
  EINA_ARG_NONNULL(1);

/**
 * @brief Add extra argument in match of signal handler to obtain specifics signals.
 *
 * Example:
 * efl_dbus_signal_handler_match_extra_set(sh, "arg0", "org.bansheeproject.Banshee", "arg1", "", NULL);
 * With this extra arguments this signal handler callback only will be called
 * when Banshee is started.
 *
 * @note For now only argX is supported.
 *
 * @param sh signal handler
 * @param ... variadic of key and value and must be ended with a NULL
 *
 * @return EFL_TRUE if success
 *
 * @note For more information:
 * http://dbus.freedesktop.org/doc/dbus-specification.html#message-bus-routing-match-rules
 */
EAPI Efl_Bool
efl_dbus_signal_handler_match_extra_set(Efl_Dbus_Signal_Handler *sh, ...)
  EINA_ARG_NONNULL(1) EINA_SENTINEL;

/**
 * @brief Add extra argument in match of signal handler to obtain specifics signals.
 *
 * Example:
 * efl_dbus_signal_handler_match_extra_set(sh, "arg0", "org.bansheeproject.Banshee", "arg1", "", NULL);
 * With this extra arguments this signal handler callback only will be called
 * when Banshee is started.
 *
 * @note For now is only supported argX.
 *
 * @param sh signal handler
 * @param ap va_list with the keys and values, must be ended with a NULL
 *
 * @return EFL_TRUE if success
 *
 * @note To information:
 * http://dbus.freedesktop.org/doc/dbus-specification.html#message-bus-routing-match-rules
 */
EAPI Efl_Bool
efl_dbus_signal_handler_match_extra_vset(Efl_Dbus_Signal_Handler *sh,
                                         va_list ap) EINA_ARG_NONNULL(1);

/**
 * @brief Add a callback function to be called when signal handler will be freed.
 *
 * @param handler signal handler
 * @param cb callback that will be called when this signal handler is freed
 * @param data data that will be passed to callback
 */
EAPI void efl_dbus_signal_handler_free_cb_add(Efl_Dbus_Signal_Handler *handler,
                                              Efl_Dbus_Free_Cb         cb,
                                              const void              *data)
  EINA_ARG_NONNULL(1, 2);

/**
 * @brief Remove callback registered in efl_dbus_signal_handler_free_cb_add().
 *
 * @param handler signal handler
 * @param cb registered callback to remove
 * @param data data that will be passed to callback
 */
EAPI void efl_dbus_signal_handler_free_cb_del(Efl_Dbus_Signal_Handler *handler,
                                              Efl_Dbus_Free_Cb         cb,
                                              const void              *data)
  EINA_ARG_NONNULL(1, 2);

EAPI const char *
efl_dbus_signal_handler_sender_get(const Efl_Dbus_Signal_Handler *handler)
  EINA_ARG_NONNULL(1) EFL_WARN_UNUSED_RESULT;
EAPI const char *
efl_dbus_signal_handler_path_get(const Efl_Dbus_Signal_Handler *handler)
  EINA_ARG_NONNULL(1) EFL_WARN_UNUSED_RESULT;
EAPI const char *
efl_dbus_signal_handler_interface_get(const Efl_Dbus_Signal_Handler *handler)
  EINA_ARG_NONNULL(1) EFL_WARN_UNUSED_RESULT;
EAPI const char *
efl_dbus_signal_handler_member_get(const Efl_Dbus_Signal_Handler *handler)
  EINA_ARG_NONNULL(1) EFL_WARN_UNUSED_RESULT;
EAPI const char *
efl_dbus_signal_handler_match_get(const Efl_Dbus_Signal_Handler *handler)
  EINA_ARG_NONNULL(1) EFL_WARN_UNUSED_RESULT;

EAPI Efl_Dbus_Connection *
efl_dbus_signal_handler_connection_get(const Efl_Dbus_Signal_Handler *handler)
  EINA_ARG_NONNULL(1) EFL_WARN_UNUSED_RESULT;

/**
 * @}
 */
#endif

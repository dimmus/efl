/**
 * @ingroup Efl_Dbus_Message
 * @defgroup Eina_Value Eina_Value to/from Message
 * @{
 */

/**
 * Convert Efl_Dbus_Message to Eina_Value
 *
 * @param msg Message
 * @return Eina_Value of type Eina_Value_Type_Struct
 */
EAPI Eina_Value *efl_dbus_message_to_eina_value(const Efl_Dbus_Message *msg)
  EINA_ARG_NONNULL(1) EFL_WARN_UNUSED_RESULT;

/**
 * Convert Efl_Dbus_Message_Iter of type variant, struct or dict entry to
 * Eina_Value.
 *
 * @param iter Message iterator
 * @return Eina_Value of type Eina_Value_Type_Struct
 */
EAPI Eina_Value *efl_dbus_message_iter_struct_like_to_eina_value(
  const Efl_Dbus_Message_Iter *iter);

/**
 * Convert Eina_Value to Efl_Dbus_Message
 *
 * @param signature dbus signature
 * @param msg where data will be put
 * @param value source of data, must be of type Eina_Value_Type_Struct
 * @return success or not
 */
EAPI Efl_Bool efl_dbus_message_from_eina_value(const char       *signature,
                                               Efl_Dbus_Message *msg,
                                               const Eina_Value *value)
  EINA_ARG_NONNULL(1, 2, 3);

/**
 * @}
 */

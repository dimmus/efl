#ifndef EFL_DBUS_MESSAGE_H
#define EFL_DBUS_MESSAGE_H 1

/**
 * @defgroup Efl_Dbus_Message Message
 * @ingroup Eldbus
 *
 * @{
 */

/**
 * @brief Increase message reference.
 *
 * @param msg The Efl_Dbus_Message object.
 * @return The previous Efl_Dbus_Message with incremented refcount.
 */
EAPI Efl_Dbus_Message *efl_dbus_message_ref(Efl_Dbus_Message *msg)
  EINA_ARG_NONNULL(1);

/**
 * @brief Decrease message reference.
 *
 * When refcount reaches zero the message and all its resources will be
 * freed.
 *
 * @param msg The Efl_Dbus_Message object.
 */
EAPI void efl_dbus_message_unref(Efl_Dbus_Message *msg) EINA_ARG_NONNULL(1);

/**
 * Get the Eldbus message path.
 *
 * @param msg The Efl_Dbus_Message object.
 * @return A string containing the dbus message path.
 */
EAPI const char *efl_dbus_message_path_get(const Efl_Dbus_Message *msg)
  EINA_ARG_NONNULL(1) EFL_WARN_UNUSED_RESULT;

/**
 * Get the Eldbus message interface.
 *
 * @param msg The Efl_Dbus_Message object.
 * @return A string containing the dbus message interface.
 */
EAPI const char *efl_dbus_message_interface_get(const Efl_Dbus_Message *msg)
  EINA_ARG_NONNULL(1) EFL_WARN_UNUSED_RESULT;

/**
 * Get the Eldbus message member.
 *
 * @param msg The Efl_Dbus_Message object.
 * @return A string containing the dbus message member.
 */
EAPI const char *efl_dbus_message_member_get(const Efl_Dbus_Message *msg)
  EINA_ARG_NONNULL(1) EFL_WARN_UNUSED_RESULT;

/**
 * Get the Eldbus message destination.
 *
 * @param msg The Efl_Dbus_Message object.
 * @return A string containing the dbus message destination.
 */
EAPI const char *efl_dbus_message_destination_get(const Efl_Dbus_Message *msg)
  EINA_ARG_NONNULL(1) EFL_WARN_UNUSED_RESULT;

/**
 * Get the Eldbus message sender.
 *
 * @param msg The Efl_Dbus_Message object.
 * @return A string containing the dbus message sender.
 */
EAPI const char *efl_dbus_message_sender_get(const Efl_Dbus_Message *msg)
  EINA_ARG_NONNULL(1) EFL_WARN_UNUSED_RESULT;

/**
 * Get the Eldbus message signature.
 *
 * @param msg The Efl_Dbus_Message object.
 * @return A string containing the dbus message signature.
 */
EAPI const char *efl_dbus_message_signature_get(const Efl_Dbus_Message *msg)
  EINA_ARG_NONNULL(1) EFL_WARN_UNUSED_RESULT;

/**
 * @brief Create a new message to invoke a method on a remote object.
 *
 * @param dest bus name or unique id of the remote application
 * @param path object path
 * @param iface interface name
 * @param method name of the method to be called
 *
 * @return a new Efl_Dbus_Message, free with efl_dbus_message_unref()
 */
EAPI Efl_Dbus_Message *efl_dbus_message_method_call_new(const char *dest,
                                                        const char *path,
                                                        const char *iface,
                                                        const char *method)
  EINA_ARG_NONNULL(1, 2, 3, 4) EFL_WARN_UNUSED_RESULT EINA_MALLOC;

/**
 * @brief Create a new signal message.
 *
 * @param path object path
 * @param interface interface name
 * @param name name of the signal to be broadcasted
 *
 * @return a new Efl_Dbus_Message, free with efl_dbus_message_unref()
 *
 * @since 1.16
 */
EAPI Efl_Dbus_Message *efl_dbus_message_signal_new(const char *path,
                                                   const char *interface,
                                                   const char *name)
  EINA_ARG_NONNULL(1, 2, 3) EFL_WARN_UNUSED_RESULT;

/**
 * @brief Create a new message that is an error reply to another message.
 *
 * @param msg the message we're replying to
 * @param error_name the error name
 * @param error_msg the error message string
 *
 * @return a new Efl_Dbus_Message, free with efl_dbus_message_unref()
 */
EAPI Efl_Dbus_Message *efl_dbus_message_error_new(const Efl_Dbus_Message *msg,
                                                  const char *error_name,
                                                  const char *error_msg)
  EINA_ARG_NONNULL(1) EFL_WARN_UNUSED_RESULT;

/**
 * @brief Create a message that is a reply to a method call.
 *
 * @param msg the message we're replying to
 *
 * @return new Efl_Dbus_Message, free with efl_dbus_message_unref()
 */
EAPI Efl_Dbus_Message *
efl_dbus_message_method_return_new(const Efl_Dbus_Message *msg)
  EINA_ARG_NONNULL(1) EFL_WARN_UNUSED_RESULT;

/**
 * @brief Get the error text and name from a Efl_Dbus_Message.
 *
 * If @param msg is an error message return EFL_TRUE and fill in the name and
 * text of the error.
 *
 * @param msg The message containing the error
 * @param name Variable in which to store the error name or @c NULL if it's not
 * desired.
 * @param text Variable in which to store the error text or @c NULL if it's not
 * desired.
 * @return EFL_TRUE on success, else EFL_FALSE.
 */
EAPI Efl_Bool efl_dbus_message_error_get(const Efl_Dbus_Message *msg,
                                         const char            **name,
                                         const char **text) EINA_ARG_NONNULL(1);

/**
 * @brief Get the arguments from an Efl_Dbus_Message
 *
 * Get the arguments of an Efl_Dbus_Message storing them in the locations pointed
 * to by the pointer arguments that follow param signature. Each pointer
 * argument must be of a type that is appropriate for the correspondent complete
 * type in param signature. For complex types such as arrays, structs,
 * dictionaries or variants, a pointer to Efl_Dbus_Message_Iter* must be provided.
 *
 * @param msg The message to get the arguments from.
 * @param signature The signature of the arguments user is expecting to read
 * from @param msg
 * @param ... The pointers in which to store the message arguments
 *
 * @return EFL_TRUE if the arguments were read successfully and stored in the
 * respective pointer arguments.
 */
EAPI Efl_Bool efl_dbus_message_arguments_get(const Efl_Dbus_Message *msg,
                                             const char             *signature,
                                             ...)
  EINA_ARG_NONNULL(1, 2) EFL_WARN_UNUSED_RESULT;

/**
 * @brief Get the arguments from an Efl_Dbus_Message using a va_list.
 *
 * @param msg The message to get the arguments from.
 * @param signature The signature user is expecting to read from param msg.
 * @param ap The va_list containing the pointer arguments.
 *
 * @see efl_dbus_message_arguments_get()
 *
 * @return EFL_TRUE if the arguments were read successfully and stored in the
 * respective pointer arguments.
 */
EAPI Efl_Bool efl_dbus_message_arguments_vget(const Efl_Dbus_Message *msg,
                                              const char             *signature,
                                              va_list                 ap)
  EINA_ARG_NONNULL(1, 2) EFL_WARN_UNUSED_RESULT;

/**
 * @brief Append arguments into an Efl_Dbus_Message
 *
 * Append arguments into an Efl_Dbus_Message according to the param signature
 * provided. For each complete type in param signature, a value of the
 * corresponding type must be provided.
 *
 * This function supports only basic types. For complex types use
 * efl_dbus_message_iter_* family of functions.
 *
 * @param msg The message in which arguments are being appended.
 * @param signature Signature of the arguments that are being appended.
 * @param ... Values of each argument to append in param msg.
 *
 * @return EFL_TRUE on success, EFL_FALSE otherwise.
 */
EAPI Efl_Bool efl_dbus_message_arguments_append(Efl_Dbus_Message *msg,
                                                const char       *signature,
                                                ...) EINA_ARG_NONNULL(1, 2);

/**
 * @brief Append arguments into an Efl_Dbus_Message using a va_list.
 *
 * @param msg The message in which arguments are being appended.
 * @param signature Signature of the arguments that are being appended.
 * @param ap The va_list containing the arguments to append.
 *
 * @see efl_dbus_message_arguments_append().
 *
 * @return EFL_TRUE on success, EFL_FALSE otherwise.
 */
EAPI Efl_Bool efl_dbus_message_arguments_vappend(Efl_Dbus_Message *msg,
                                                 const char       *signature,
                                                 va_list           ap)
  EINA_ARG_NONNULL(1, 2);

/**
 * @defgroup Efl_Dbus_Message_Iter Iterator
 * @{
 */

/**
 * @brief Create and append a typed iterator to another iterator.
 *
 * After append data to returned iterator it must be closed calling
 * efl_dbus_message_iter_container_close().
 *
 * Container types are for example struct, variant, and array.
 * For variants, the contained_signature should be the type of the single
 * value inside the variant. For structs and dict entries, contained_signature
 * should be NULL; it will be set to whatever types you write into the struct.
 * For arrays, contained_signature should be the type of the array elements.
 *
 * @param iter parent of the new iterator
 * @param type of iterator (e.g struct, dict, variant or array)
 * @param contained_signature signature of what iterator will store
 *
 * @return the new iterator
 */
EAPI Efl_Dbus_Message_Iter *
efl_dbus_message_iter_container_new(Efl_Dbus_Message_Iter *iter,
                                    int                    type,
                                    const char            *contained_signature)
  EINA_ARG_NONNULL(1, 3) EFL_WARN_UNUSED_RESULT;

/**
 * @brief Append a basic type into an Efl_Dbus_Iterator.
 *
 * @param iter The iterator in which basic type will be appended.
 * @param type The basic type that will be appended.
 * @param ... The values for the basic type.
 *
 * @return EFL_TRUE on success, else EFL_FALSE.
 */
EAPI Efl_Bool efl_dbus_message_iter_basic_append(Efl_Dbus_Message_Iter *iter,
                                                 int                    type,
                                                 ...) EINA_ARG_NONNULL(1, 3);

/**
 * @brief Append an argument into an Efl_Dbus_Message_Iter. For each complete type
 * you need to provide the correspondent value. In case of complex types you
 * need to provide an Efl_Dbus_Message_Iter** to be allocated and then filled in.
 *
 * It's not possible to open two iterators at same iterator with this function.
 * For example, to create a message with signature="aiai" you need to create the
 * first container with efl_dbus_message_iter_container_new(), fill the array,
 * close it with efl_dbus_message_iter_container_close() and then do the same for
 * the second array.
 *
 * @param iter iterator in which data will be appended
 * @param signature signature of the contained data
 * @param ... values for each complete type
 * @return EFL_TRUE on success, else EFL_FALSE
 *
 * @see efl_dbus_message_iter_container_new()
 * @see efl_dbus_message_iter_container_close()
 *
 * @note This function doesn't support variant, use
 * efl_dbus_message_iter_container_new() instead to create the variant, fill
 * with data and close it.
 */
EAPI Efl_Bool
efl_dbus_message_iter_arguments_append(Efl_Dbus_Message_Iter *iter,
                                       const char            *signature,
                                       ...) EINA_ARG_NONNULL(1, 2);

/**
 * @brief Set data to Efl_Dbus_Message_Iter. For each complete in signature
 * you need pass the value, in case of complex type a pointer to be allocated a
 * Efl_Dbus_Message_Iter that you need fill and close.
 *
 * It's not possible open two iterators at same Iterator. Example:
 * "aiai", to set this you need create and put the first array with
 * efl_dbus_message_iter_container_new() fill array with data and close then
 * you could open the second array with efl_dbus_message_iter_container_new().
 *
 * @param iter iterator
 * @param signature of data
 * @param ap va_list with the values
 * @return EFL_TRUE on success, else EFL_FALSE
 *
 * @note This function don't support variant, use instead
 * efl_dbus_message_iter_container_new() to create the variant fill
 * data and close it.
 */
EAPI Efl_Bool
efl_dbus_message_iter_arguments_vappend(Efl_Dbus_Message_Iter *iter,
                                        const char            *signature,
                                        va_list ap) EINA_ARG_NONNULL(1, 2, 3);

/**
 * Append a array of basic type with fixed size to Efl_Dbus_Message_Iter.
 *
 * @param iter iterator
 * @param type basic type that will be appended
 * @param array data to append
 * @param size of array
 * @return EFL_TRUE on success, else EFL_FALSE
 */
EAPI Efl_Bool efl_dbus_message_iter_fixed_array_append(
  Efl_Dbus_Message_Iter *iter,
  int                    type,
  const void            *array,
  unsigned int           size) EINA_ARG_NONNULL(1, 3);

/**
 * @brief Closes a container-typed value appended to the message.
 *
 * @param iter parent of the sub-iterator
 * @param sub the iterator that will be closed
 *
 * @return EFL_FALSE if iterator was already close or if not enough memory
 */
EAPI Efl_Bool efl_dbus_message_iter_container_close(Efl_Dbus_Message_Iter *iter,
                                                    Efl_Dbus_Message_Iter *sub)
  EINA_ARG_NONNULL(1, 2);

/**
 * @brief Get the main Efl_Dbus_Message_Iter from the Efl_Dbus_Message.
 *
 * @param msg The message to get the iterator from.
 *
 * @return The iterator of the message.
 */
EAPI Efl_Dbus_Message_Iter *
efl_dbus_message_iter_get(const Efl_Dbus_Message *msg)
  EINA_ARG_NONNULL(1) EFL_WARN_UNUSED_RESULT;

/**
 * @brief Get a basic type from Efl_Dbus_Iterator.
 *
 * @param iter The iterator to get the basic type from.
 * @param value The basic type of the iterator.
 */
EAPI void efl_dbus_message_iter_basic_get(Efl_Dbus_Message_Iter *iter,
                                          void *value) EINA_ARG_NONNULL(1, 2);

/**
 * @brief Returns the current signature of a message iterator.
 *
 * @param iter The iterator on which to query the signature.
 * @return A string containing the message iterator signature.
 *
 * @note The returned string must be freed.
 */
EAPI char *efl_dbus_message_iter_signature_get(Efl_Dbus_Message_Iter *iter)
  EINA_ARG_NONNULL(1) EFL_WARN_UNUSED_RESULT;

/**
 * @brief Moves the iterator to the next field, if any.
 * @param iter iterator
 *
 * @return if iterator was reach to end return EFL_FALSE
 */
EAPI Efl_Bool efl_dbus_message_iter_next(Efl_Dbus_Message_Iter *iter)
  EINA_ARG_NONNULL(1);

/**
 * @brief Get a complete type from Efl_Dbus_Message_Iter if is not at the end
 * of iterator and move to next field.
 * Useful to iterate over arrays.
 *
 * @param iter iterator
 * @param signature of the next completed type in Iterator
 * @param ... pointer of where data will be stored
 *
 * @return if iterator was reach to end or if type different of the type that
 * iterator points return EFL_FALSE
 *
 */
EAPI Efl_Bool efl_dbus_message_iter_get_and_next(Efl_Dbus_Message_Iter *iter,
                                                 char signature,
                                                 ...) EINA_ARG_NONNULL(1, 2, 3);

/**
 * @brief Reads a block of fixed-length values from the message iterator.
 *
 * Fixed-length values are those basic types that are not string-like,
 * such as integers, bool, double. The returned block will be from the
 * current position in the array until the end of the array.
 *
 * There is one exception here: although ELDBUS_TYPE_UNIX_FD is considered a
 * 'fixed' type arrays of this type may not be read with this function.
 *
 * The value argument should be the address of a location to store the returned
 * array. So for int32 it should be a "const dbus_int32_t**" The returned value
 * is by reference and should not be freed.
 *
 * Because the array is not copied, this function runs in constant time and is
 * fast; it's much preferred over walking the entire array with an iterator.
 *
 * @param iter The message iterator
 * @param signature
 * @param value
 * @param n_elements
 * @return EFL_TRUE on success, else EFL_FALSE.
 */
EAPI Efl_Bool efl_dbus_message_iter_fixed_array_get(Efl_Dbus_Message_Iter *iter,
                                                    int   signature,
                                                    void *value,
                                                    int  *n_elements)
  EINA_ARG_NONNULL(1, 3, 4);

/**
 * @brief Get data from Efl_Dbus_Message_Iter, for each complete type must have
 * a pointer to store his value, in case of complex type a
 * Efl_Dbus_Message_Iter will be need.
 *
 * @param iter iterator
 * @param signature of the complete data types on iterator
 * @param ... pointers of where data will be stored
 *
 * @return EFL_FALSE if signature different from signature in iterator
 */
EAPI Efl_Bool efl_dbus_message_iter_arguments_get(Efl_Dbus_Message_Iter *iter,
                                                  const char *signature,
                                                  ...) EINA_ARG_NONNULL(1, 2);

/**
 * @brief Get data from Efl_Dbus_Message_Iter, for each complete type must have
 * a pointer to store his value, in case of complex type a
 * Efl_Dbus_Message_Iter will be need.
 *
 * @param iter iterator
 * @param signature of the complete data types on iterator
 * @param ap va_list of the pointers of where data will be stored
 *
 * @return EFL_FALSE if signature different from signature in iterator
 */
EAPI Efl_Bool efl_dbus_message_iter_arguments_vget(Efl_Dbus_Message_Iter *iter,
                                                   const char *signature,
                                                   va_list     ap)
  EINA_ARG_NONNULL(1, 2);

/**
 * @brief Manually delete the iterator.
 *
 * Iterators are usually bound to the life of @ref Efl_Dbus_Message
 * they were created from, being deleted automatically once the
 * message is deleted.
 *
 * However when dealing with huge arrays or dicts it may become a
 * major memory impact to leave the unused iterators alive. By
 * calling this function one states the iterator is not used anymore
 * and can be deleted.
 *
 * @param iter the iterator to be deleted.
 */
EAPI void efl_dbus_message_iter_del(Efl_Dbus_Message_Iter *iter)
  EINA_ARG_NONNULL(1);

/**
 * @}
 */

#include "efl_dbus_message_helper.h"
#include "efl_dbus_message_eina_value.h"

/**
 * @}
 */
#endif

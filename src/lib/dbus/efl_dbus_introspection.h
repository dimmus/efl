#ifndef _ELDBUS_INTROSPECTION_INTROSPECTION_H
#define _ELDBUS_INTROSPECTION_INTROSPECTION_H

#include <Efl_Shared.h>
#include <Efl_Eo.h>

/* FIXME: these are duplicated as @extern in efl_dbus_types.eot */

/** Argument direction
 *
 * @ingroup Efl_Dbus_Introspection
 */
typedef enum
{
  ELDBUS_INTROSPECTION_ARGUMENT_DIRECTION_NONE = 0, /**< No direction */
  ELDBUS_INTROSPECTION_ARGUMENT_DIRECTION_IN, /**< Incoming direction */
  ELDBUS_INTROSPECTION_ARGUMENT_DIRECTION_OUT /**< Outgoing direction */
} Efl_Dbus_Introspection_Argument_Direction;

/** Property access rights
 *
 * @ingroup Efl_Dbus_Introspection
 */
typedef enum
{
  ELDBUS_INTROSPECTION_PROPERTY_ACCESS_READ = 0, /**< Property can be read */
  ELDBUS_INTROSPECTION_PROPERTY_ACCESS_WRITE, /**< Property can be written */
  ELDBUS_INTROSPECTION_PROPERTY_ACCESS_READWRITE /**< Property can be read and
                                                  * written */
} Efl_Dbus_Introspection_Property_Access;

/** DBus Node
 *
 * @ingroup Efl_Dbus_Introspection
 */
typedef struct _Efl_Dbus_Introspection_Node
{
  Eina_Stringshare *name; /**< Node name (optional) */
  Eina_List        *nodes; /**< List with nodes */
  Eina_List        *interfaces; /**< List with interfaces */
} Efl_Dbus_Introspection_Node;

/** DBus Interface
 *
 * @ingroup Efl_Dbus_Introspection
 */
typedef struct _Efl_Dbus_Introspection_Interface
{
  Eina_Stringshare *name; /**< Interface name */
  Eina_List        *methods; /**< List with interface methods */
  Eina_List        *signals; /**< List with interface signals */
  Eina_List        *properties; /**< List with interface properties */
  Eina_List        *annotations; /**< List with interface annotations */
} Efl_Dbus_Introspection_Interface;

/** DBus Method
 *
 * @ingroup Efl_Dbus_Introspection
 */
typedef struct _Efl_Dbus_Introspection_Method
{
  Eina_Stringshare *name; /**< Method name */
  Eina_List        *arguments; /**< List with method arguments */
  Eina_List        *annotations; /**< List with method annotations */
} Efl_Dbus_Introspection_Method;

/** DBus Property
 *
 * @ingroup Efl_Dbus_Introspection
 */
typedef struct _Efl_Dbus_Introspection_Property
{
  Eina_Stringshare                      *name; /**< Property name */
  Eina_Stringshare                      *type; /**< Property type */
  Efl_Dbus_Introspection_Property_Access access; /**< Property access rights */
  Eina_List *annotations; /**< List with property annotations */
} Efl_Dbus_Introspection_Property;

/** DBus Annotation
 *
 * @ingroup Efl_Dbus_Introspection
 */
typedef struct _Efl_Dbus_Introspection_Annotation
{
  Eina_Stringshare *name; /**< Annotation name */
  Eina_Stringshare *value; /**< Annotation value */
} Efl_Dbus_Introspection_Annotation;

/** DBus Argument
 *
 * @ingroup Efl_Dbus_Introspection
 */
typedef struct _Efl_Dbus_Introspection_Argument
{
  Eina_Stringshare *name; /**< Argument name (optional) */
  Eina_Stringshare *type; /**< Argument type */
  Efl_Dbus_Introspection_Argument_Direction
    direction; /**< Argument direction */
} Efl_Dbus_Introspection_Argument;

/** DBus Signal
 *
 * @ingroup Efl_Dbus_Introspection
 */
typedef struct _Efl_Dbus_Introspection_Signal
{
  Eina_Stringshare *name; /**< Signal name */
  Eina_List        *arguments; /**< List with signal arguments */
  Eina_List        *annotations; /**< List with signal annotations */
} Efl_Dbus_Introspection_Signal;

/**
 * @brief Parses the introspection xml abstracting it to an object tree
 *
 * @param xml The introspection xml
 * @return The introspection object tree
 *
 * @since 1.17
 */
EAPI Efl_Dbus_Introspection_Node *efl_dbus_introspection_parse(const char *xml);

/**
 * @brief Frees the introspection object tree
 *
 * @param node The root node of introspection tree
 *
 * @since 1.17
 */
EAPI void efl_dbus_introspection_node_free(Efl_Dbus_Introspection_Node *node);

/**
 * @brief Finds an interface by name
 *
 * @param interfaces The list of interfaces of type @c Efl_Dbus_Introspection_Interface
 * @param name The interfaces's name to search for
 * @return Returns the interface found or @c NULL if not
 *
 * @since 1.17
 */
EAPI Efl_Dbus_Introspection_Interface *
efl_dbus_introspection_interface_find(Eina_List *interfaces, const char *name);

/**
 * @brief Finds a property by name
 *
 * @param properties The list of properties of type @c Efl_Dbus_Introspection_Property
 * @param name The properties's name to search for
 * @return Returns the property found or @c NULL if not
 *
 * @since 1.17
 */
EAPI Efl_Dbus_Introspection_Property *
efl_dbus_introspection_property_find(Eina_List *properties, const char *name);

/**
 * @brief Finds an argument by name
 *
 * @param arguments The list of arguments of type @c Efl_Dbus_Introspection_Property
 * @param name The arguments's name to search for
 * @return Returns the argument found or @c NULL if not
 *
 * @since 1.17
 */
EAPI Efl_Dbus_Introspection_Argument *
efl_dbus_introspection_argument_find(Eina_List *arguments, const char *name);

#endif

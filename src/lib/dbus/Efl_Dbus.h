/**
 * @page efl_dbus_main Eldbus
 *
 * @date 2012 (created)
 *
 * @section dbus_toc Table of Contents
 *
 * @li @ref efl_dbus_main_intro
 * @li @ref efl_dbus_main_compiling
 * @li @ref efl_dbus_main_next_steps
 * @li @ref efl_dbus_main_intro_example
 *
 * @section efl_dbus_main_intro Introduction
 *
 * Eldbus is a wrapper around the
 * <a href="http://www.freedesktop.org/wiki/Software/dbus">dbus</a>
 * library, which is a message bus system. It also implements a set of
 * specifications using dbus as interprocess communication.
 *
 * @subsection efl_dbus_modules_sec Modules
 *
 * <ul>
 * <li> @ref Efl_Dbus_Core
 * <li> @ref Efl_Dbus_Connection
 * <li> @ref Efl_Dbus_Object_Mapper
 * <li> @ref Efl_Dbus_Proxy
 * <li> @ref Efl_Dbus_Message
 *      <ul>
 *              <li>@ref Efl_Dbus_Message_Iter
 *              <li>@ref Efl_Dbus_Message_Helpers
 *              <li>@ref Eina_Value
 *      </ul>
 * <li> @ref Efl_Dbus_Signal_Handler
 * <li> @ref Efl_Dbus_Pending
 * <li> @ref Efl_Dbus_Service
 * <li> @ref Efl_Dbus_Basic
 * </ul>
 *
 * @section efl_dbus_main_compiling How to compile
 *
 * Eldbus is a library your application links to. The procedure for this is
 * very simple. You simply have to compile your application with the
 * appropriate compiler flags that the @c pkg-config script outputs. For
 * example:
 *
 * Compiling C or C++ files into object files:
 *
 * @verbatim
   gcc -c -o main.o main.c `pkg-config --cflags eldbus`
   @endverbatim
 *
 * Linking object files into a binary executable:
 *
 * @verbatim
   gcc -o my_application main.o `pkg-config --libs eldbus`
   @endverbatim
 *
 * See @ref pkgconfig
 *
 * @section efl_dbus_main_next_steps Next Steps
 *
 * After you understood what Eldbus is and installed it in your system
 * you should proceed understanding the programming interface.
 *
 * Recommended reading:
 * @li @ref Efl_Dbus_Core for library init, shutdown and getting a connection.
 * @li @ref Efl_Dbus_Proxy to easily bind a client object to an interface.
 * @li @ref Efl_Dbus_Object_Mapper to monitor server objects and properties.
 *
 * @section efl_dbus_main_intro_example Introductory Example
 *
 * @include ofono-dial.c
 *
 * More examples can be found at @ref dbusexamples.
 */
#ifndef EFL_DBUS_H
#define EFL_DBUS_H

#include <stdarg.h>

#include <Efl_Shared.h>
// #include <Efl_Config.h>
#include <Efl_Eo.h>
#ifdef EFL_BETA_API_SUPPORT
#  include <Efl_Core.h>
#endif
#ifdef EAPI
#  undef EAPI
#endif

#ifdef _WIN32
#  ifdef EFL_BUILD
#    ifdef DLL_EXPORT
#      define EAPI __declspec(dllexport)
#    else
#      define EAPI
#    endif
#  else
#    define EAPI __declspec(dllimport)
#  endif
#else
#  ifdef __GNUC__
#    if __GNUC__ >= 4
#      define EAPI __attribute__ ((visibility("default")))
#    else
#      define EAPI
#    endif
#  else
#    define EAPI
#  endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup Efl_Dbus_Core Core
 * @ingroup Eldbus
 *
 * @{
 */
#define EFL_DBUS_VERSION_MAJOR EFL_VERSION_MAJOR /**< Eldbus version major number */
#define EFL_DBUS_VERSION_MINOR EFL_VERSION_MINOR /**< Eldbus version minor number */

#define EFL_DBUS_FDO_BUS "org.freedesktop.DBus" /**< DBus message bus name */
#define EFL_DBUS_FDO_PATH "/org/freedesktop/DBus" /**< Object that implements message bus interface */
#define EFL_DBUS_FDO_INTERFACE EFL_DBUS_FDO_BUS /**< DBus message bus interface */
#define EFL_DBUS_FDO_INTERFACE_PROPERTIES "org.freedesktop.DBus.Properties" /**< DBus Properties interface */
#define EFL_DBUS_FDO_INTERFACE_OBJECT_MANAGER "org.freedesktop.DBus.ObjectManager" /**< DBus ObjectManager interface */
#define EFL_DBUS_FDO_INTERFACE_INTROSPECTABLE "org.freedesktop.DBus.Introspectable" /**< DBus Introspectable interface */
#define EFL_DBUS_FDO_INTEFACE_PEER "org.freedesktop.DBus.Peer" /**< DBus Peer interface */
#define EFL_DBUS_ERROR_PENDING_CANCELED "org.enlightenment.DBus.Canceled" /**< Canceled error answer */
#define EFL_DBUS_ERROR_PENDING_TIMEOUT "org.freedesktop.DBus.Error.NoReply" /**< Timeout error answer */

/**
 * @typedef Efl_Dbus_Version
 * Represents the current version of Eldbus
 */
typedef struct _Efl_Dbus_Version
{
  int major; /**< major (binary or source incompatible changes) */
  int minor; /**< minor (new features, bugfixes, major improvements version) */
  int
    micro; /**< micro (bugfix, internal improvements, no new features version) */
  int
    revision; /**< git revision (0 if a proper release or the git revision number Eldbus is built from) */
} Efl_Dbus_Version;

EAPI extern const Efl_Dbus_Version
  *dbusversion; /**< Global Efl_Dbus_Version object */

/**
 * @brief Initialize eldbus.
 *
 * @return 1 or greater on success, 0 otherwise
 */
EAPI int efl_dbus_init(void);
/**
 * @brief Shutdown eldbus.
 *
 * @return 0 if e_dbus shuts down, greater than 0 otherwise.
 */
EAPI int efl_dbus_shutdown(void);

/**
 * @typedef Efl_Dbus_Free_Cb
 *
 * Callback that is called when the connection is freed.
 */
typedef void (*Efl_Dbus_Free_Cb)(void *data, const void *deadptr);
/**
 * @typedef Efl_Dbus_Message
 *
 * Represents the way data is sent and received in DBus.
 */
typedef struct _Efl_Dbus_Message Efl_Dbus_Message;
/**
 * @typedef Efl_Dbus_Message_Iter
 *
 * Represents an iterator over a complex message type (array, dict, struct,
 * or variant). Its life is bound to the message that contains
 * it. The same applies to the returned data.
 */
typedef struct _Efl_Dbus_Message_Iter Efl_Dbus_Message_Iter;
/**
 * @typedef Efl_Dbus_Pending
 *
 * Represents a message that has been sent but has not yet reached its
 * destination.
 */
typedef struct _Efl_Dbus_Pending Efl_Dbus_Pending;

/**
 * @typedef Efl_Dbus_Signal_Handler
 *
 * Represents a listener that will listen for signals emitted by other
 * applications.
 */
typedef struct _Efl_Dbus_Signal_Handler Efl_Dbus_Signal_Handler;

/**
 * @typedef Efl_Dbus_Message_Cb
 *
 * Callback that is called when answer of a method call message comes.
 */
typedef void (*Efl_Dbus_Message_Cb)(void                   *data,
                                    const Efl_Dbus_Message *msg,
                                    Efl_Dbus_Pending       *pending);

/**
 * @typedef Efl_Dbus_Signal_Cb
 *
 * Callback that is called when a signal is received.
 */
typedef void (*Efl_Dbus_Signal_Cb)(void *data, const Efl_Dbus_Message *msg);
/**
 * @}
 */

/* FIXME: these are duplicated as @extern in dbus_types.eot */

/** Represents a client object bound to an interface
 *
 * @ingroup Eldbus
 */
typedef struct _Efl_Dbus_Proxy Efl_Dbus_Proxy;

/** Represents a connection of one the type of connection with the DBus daemon.
 *
 * @ingroup Eldbus
 */
typedef struct _Efl_Dbus_Connection Efl_Dbus_Connection;

/** Represents an object path already attached with bus name or unique id
 *
 * @ingroup Eldbus
 */
typedef struct _Efl_Dbus_Object Efl_Dbus_Object;

#include "efl_dbus_connection.h"
#include "efl_dbus_message.h"
#include "efl_dbus_signal_handler.h"
#include "efl_dbus_pending.h"
#include "efl_dbus_object.h"
#include "efl_dbus_proxy.h"
#include "efl_dbus_freedesktop.h"
#include "efl_dbus_service.h"
#include "efl_dbus_introspection.h"

#ifdef EFL_BETA_API_SUPPORT

#  include "efl_dbus_model.eo.h"
#  include "efl_dbus_model_arguments.eo.h"
#  include "efl_dbus_model_connection.eo.h"
#  include "efl_dbus_model_method.eo.h"
#  include "efl_dbus_model_object.eo.h"
#  include "efl_dbus_model_proxy.eo.h"
#  include "efl_dbus_model_signal.eo.h"

#endif

#ifdef __cplusplus
}
#endif

#undef EAPI
#define EAPI

#endif

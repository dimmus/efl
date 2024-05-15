/**
 * @brief Ecore Library Public API Calls.
 *        These routines are used for Ecore Library interaction.
 */

/**
 * @page core_main Ecore
 * @date 2000 (created)
 * @section Core_toc Table of Contents
 * @li @ref core_main_intro
 * @li @ref core_main_compiling
 * @li @ref core_main_next_steps
 * @li @ref core_main_intro_example
 * @section core_main_intro Introduction
 * Ecore is a library of convenience functions. A brief explanation of how to use
 * it can be found in @ref Core_Main_Loop_Page.
 * The Ecore library provides the following modules:
 * @li @ref Core_Init_Group
 * @li @ref Core_Getopt_Group
 * @li @ref Core_Main_Loop_Group
 * @li @ref Core_System_Events
 * @li @ref Core_Time_Group
 * @li @ref Core_Thread_Group
 * @li @ref Core_Pipe_Group
 * @li @ref Core_Application_Group
 * @li @ref Core_Throttle_Group
 * @li @ref Core_Job_Group
 * @li @ref Core_File_Group
 * @li @ref Core_Con_Group
 * @li @ref Core_Evas_Group
 * @li @ref Core_FB_Group
 * @li @ref Core_Input_Group
 * @li @ref Core_IMF_Lib_Group
 * @li @ref Core_IPC_Group
 * @li @link Core_X.h Core_X - X Windows System wrapper. @endlink
 * @li @ref Core_Win32_Group
 * @li @ref Core_Audio_Group
 * @li @ref Core_Avahi_Group
 * @li @ref Core_Drm_Group
 * @li @ref Core_Wl_Group
 * For more info on Ecore usage, there are these @ref Core_examples.
 *
 * @section core_main_compiling How to compile
 * Ecore is a library your application links to. The procedure for
 * this is very simple. You simply have to compile your application
 * with the appropriate compiler flags that the @p pkg-config script
 * outputs. Note that each module is separate in pkg-config. For
 * example using @c Core_Evas:
 *
 * Compiling C or C++ files into object files:
 * @code
 * gcc -c -o main.o main.c `pkg-config --cflags ecore ecore-evas`
 * @endcode
 * Linking object files into a binary executable:
 * @code
 * gcc -o my_application main.o `pkg-config --libs ecore ecore-evas`
 * @endcode
 * See @ref pkgconfig
 *
 * @section core_main_next_steps Next Steps
 * After you understood what Ecore is and installed it in your system
 * you should proceed to understand the programming interface. We'd
 * recommend you to take a while to learn @ref Eina as it is very
 * convenient and optimized, and Ecore uses it extensively.
 * Recommended reading:
 * @li @ref Core_Timer_Group
 * @li @ref Core_Idle_Group
 * @li @ref CORE_FD_Handler_Group
 * @li @ref Core_Event_Group
 * @li @ref Core_Exe_Group
 * @li @ref Core_Animator_Group
 * @li @ref Core_Poller_Group
 *
 * @section core_main_intro_example Introductory Examples

 * @include core_timer_example.c
 * More examples can be found at @ref Core_examples.
 */

/**
 * @page Core_Main_Loop_Page The Ecore Main Loop
 *
 * @section Core_Main_Loop_Page_intro What is Ecore?
 *
 * Ecore is a clean and tiny event loop library with many modules to do lots of
 * convenient things for a programmer to save time and effort. It's small and
 * lean, designed to work from embedded systems all the way up to large and
 * powerful multi-cpu workstations. The main loop has a number of primitives to
 * be used with its main loop. It serializes all the primitives and allows for
 * great responsiveness without the need for threads(or any other concurrency).
 *
 * @subsection timers Timers
 *
 * Timers serve two main purposes: doing something at a specified time and
 * repeatedly doing something with a set interval.
 *
 * @see Core_Timer_Group
 *
 * @subsection pollers Pollers
 *
 * Pollers allow for polling to be centralized into a single place therefore
 * alleviating the need for different parts of the program to wake up at
 * different times to do polling, thereby making the code simpler and more efficient.
 *
 * @see Core_Poller_Group
 *
 * @subsection idler Idlers
 * There are three types of idlers: enterers, idlers(proper) and exiters. They
 * are called, respectively, when the program is about to enter an idle state,
 * when the program is idle, and when the program is leaving an idle state.
 * Idler enterers are usually a good place to update the program state. Proper idlers
 * are the appropriate place to do heavy computational tasks thereby using what
 * would otherwise be wasted CPU cycles. Exiters are the perfect place to do
 * anything your program should do just before processing events (also timers,
 * pollers, file descriptor handlers and animators)
 *
 * @see Core_Idle_Group
 *
 * @subsection fd_handler File descriptor handlers
 *
 * File descriptor handlers allow you to monitor when there is data available to
 * read on file descriptors, when writing will not block, or if there was an
 * error. Any valid file descriptor can be used with this API, regardless of if
 * was gotten with an OS specific API or from ecore.
 *
 * @see CORE_FD_Handler_Group
 *
 * @subsection animators Animators
 *
 * Ecore provides a facility called animators, so named since the intended use
 * was in animations, that facilitates knowing what percentage of a given
 * interval has elapsed. This is perfect for performing animations, but is not
 * limited to that use, it can, for example, also be used to create a progress bar.
 *
 * @see Core_Animator_Group
 *
 * @subsection ev_handlers Event handlers
 *
 * Event handlers are, arguably, the most important feature of the ecore main
 * loop, they are what allows the programmer to easily handle user interaction.
 * Events however are not only things the user does, events can represent
 * anything for which a type is created.
 *
 * @see Core_Event_Group
 * All of these primitives are discussed in more detail in their respective
 * pages linked above.
 *
 * Here is a diagram of the main loop flow of a simple program:
 *
 * @image html  prog_flow.png
 * @image latex prog_flow.eps width=\textwidth
 *
 *
 *
 * @section Core_Main_Loop_Page_work How does Ecore work?
 *
 * Ecore is very easy to learn and use. All the function calls are designed to
 * be easy to remember, explicit in describing what they do, and heavily
 * name-spaced. Ecore programs can start and be very simple.
 *
 * For example:
 *
 * @code
 * #include <Efl_Core.h>
 *
 * int
 * main(int argc, const char **argv)
 * {
 *    Core_init();
 *    core_app_args_set(argc, argv);
 *    core_main_loop_begin();
 *    Core_shutdown();
 *    return 0;
 * }
 * @endcode
 *
 * This program is very simple and doesn't check for errors, but it does start up
 * and begin a main loop waiting for events or timers to tick off. This program
 * doesn't set up any, but now we can expand on this simple program a little
 * more by adding some event handlers and timers.
 *
 * @code
 * #include <Efl_Core.h>
 *
 * Core_Timer         *timer1     = NULL;
 * Core_Event_Handler *handler1   = NULL;
 * double               start_time = 0.0;
 *
 * int
 * timer_func(void *data)
 * {
 *    printf("Tick timer. Sec: %3.2f\n", core_time_get() - start_time);
 *    return CORE_CALLBACK_RENEW;
 * }
 *
 * int
 * exit_func(void *data, int ev_type, void *ev)
 * {
 *    Core_Event_Signal_Exit *e;
 *
 *    e = (Core_Event_Signal_Exit *)ev;
 *    if (e->interrupt)      printf("Exit: interrupt\n");
 *    else if (e->quit)      printf("Exit: quit\n");
 *    else if (e->terminate) printf("Exit: terminate\n");
 *    core_main_loop_quit();
 *    return 1;
 * }
 *
 * int
 * main(int argc, const char **argv)
 * {
 *    Core_init();
 *    core_app_args_set(argc, argv);
 *    start_time = core_time_get();
 *    handler1 = core_event_handler_add(CORE_EVENT_SIGNAL_EXIT, exit_func, NULL);
 *    timer1 = core_timer_add(0.5, timer_func, NULL);
 *    core_main_loop_begin();
 *    Core_shutdown();
 *    return 0;
 * }
 * @endcode
 *
 * In the previous example, we initialize our application and get the time at
 * which our program has started so we can calculate an offset. We set
 * up a timer to tick off in 0.5 seconds, and since it returns 1, will
 * keep ticking off every 0.5 seconds until it returns 0, or is deleted
 * by hand. An event handler is set up to call a function -
 * exit_func(),
 * whenever an event of type CORE_EVENT_SIGNAL_EXIT is received (CTRL-C
 * on the command line will cause such an event to happen). If this event
 * occurs it tells you what kind of exit signal was received, and asks
 * the main loop to quit when it is finished by calling
 * core_main_loop_quit().
 *
 * The handles returned by core_timer_add() and
 * core_event_handler_add() are
 * only stored here as an example. If you don't need to address the timer or
 * event handler again you don't need to store the result, so just call the
 * function, and don't assign the result to any variable.
 *
 * This program looks slightly more complex than needed to do these simple
 * things, but in principle, programs don't get any more complex. You add more
 * event handlers, for more events, will have more timers and such, BUT it all
 * follows the same principles as shown in this example.
 *
 */


/**
 * @page Core_Config_Page The Enlightened Property Library
 * The Enlightened Property Library (Core_Config) is an abstraction
 * from the complexities of writing your own configuration. It provides
 * many features using the Enlightenment 17 development libraries.
 * To use the library, you:
 * @li Set the default values of your properties.
 * @li Load the configuration from a file.  You must set the default values
 * first, so that the library knows the correct type of each argument.
 * The following examples show how to use the Enlightened Property Library:
 * @li @link config_basic_example.c config_basic_example.c @endlink
 * @li @link config_listener_example.c config_listener_example.c @endlink
 */


/**
 * @page X_Window_System_Page X Window System
 * The Ecore library includes a wrapper for handling the X window system.
 * This page briefly explains what the X window system is and various terms
 * that are used.
 */

#ifndef _Core_H
#define _Core_H

#include <Efl_Shared.h>
#include <Efl_Eo.h>
#include <Efl_Eo_Interfaces.h>

#ifdef EAPI
# undef EAPI
#endif

#ifdef _WIN32
# ifdef EFL_BUILD
#  ifdef DLL_EXPORT
#   define EAPI __declspec(dllexport)
#  else
#   define EAPI
#  endif
# else
#  define EAPI __declspec(dllimport)
# endif
#else
# ifdef __GNUC__
#  if __GNUC__ >= 4
#   define EAPI __attribute__ ((visibility("default")))
#  else
#   define EAPI
#  endif
# else
#  define EAPI
# endif
#endif

#ifdef _WIN32
# define WIN32_LEAN_AND_MEAN
# include <winsock2.h>
# undef WIN32_LEAN_AND_MEAN
#elif defined (__FreeBSD__) || defined (__OpenBSD__)
# include <sys/select.h>
# include <signal.h>
#elif defined (__ANDROID__)
# include <sys/select.h>
#elif defined (__sun)
# include <sys/siginfo.h>
#else
# include <sys/time.h>
# include <signal.h>
#endif

#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "Efl_Core_Common.h"
#ifndef EFL_NOLEGACY_API_SUPPORT
# include "core_legacy.h"
#endif
#include "Efl_Core_Eo.h"

EAPI double _core_main_loop_wakeup_time_get(void);
EAPI Efl_Bool bs_mod_get(char       *path,
                    size_t      maxlen,
                    const char *subsystem,
                    const char *mod_name);

#ifdef __cplusplus
}
#endif

#undef EAPI
#define EAPI

#endif

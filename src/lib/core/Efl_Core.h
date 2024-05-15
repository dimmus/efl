#ifndef _EFL_CORE_H
#define _EFL_CORE_H

#ifdef HAVE_CONFIG_H
# include "efl_config.h"
#endif

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

EAPI extern double _efl_startup_time;

#include "Efl_Core_Common.h"
#include "Efl_Core_Eo.h"

#include "core.h"

#ifdef __cplusplus
}
#endif

#undef EAPI
#define EAPI

#endif

// We are including efl_general.h again, just in case Efl_Core.h was already included before this
// and the __EFL_*_IS_REQUIRED changed since then.
#include "efl_general.h"

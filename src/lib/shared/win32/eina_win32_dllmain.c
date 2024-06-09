#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include "shared_config.h"
#include "eina_types.h"
#include <evil_private.h>

void free_thread(void);

BOOL WINAPI
DllMain(HINSTANCE inst EFL_UNUSED, WORD reason, PVOID reserved EFL_UNUSED)
{
  if (DLL_THREAD_DETACH == reason) free_thread();

  return TRUE;
}

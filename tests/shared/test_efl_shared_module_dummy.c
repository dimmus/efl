#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include <stdio.h>

#include <Efl_Shared.h>

Efl_Bool
dummy_module_init(void)
{
  return EFL_TRUE;
}

void
dummy_module_shutdown(void)
{}

EINA_API int dummy_symbol = 0xbad;

EINA_MODULE_INIT(dummy_module_init);
EINA_MODULE_SHUTDOWN(dummy_module_shutdown);

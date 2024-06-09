#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include <Efl_Shared.h>
#include "eolian_database.h"

void
database_constructor_del(Eolian_Constructor *ctor)
{
  if (!ctor) return;
  eina_stringshare_del(ctor->base.file);
  eina_stringshare_del(ctor->base.name);
  free(ctor);
}

#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include <Efl_Shared.h>
#include "eolian_database.h"

void
database_event_del(Eolian_Event *event)
{
  if (!event) return;
  eina_stringshare_del(event->base.file);
  eina_stringshare_del(event->base.name);
  database_type_del(event->type);
  database_doc_del(event->doc);
  free(event);
}

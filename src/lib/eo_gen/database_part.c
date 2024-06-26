#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include <Efl_Shared.h>
#include "eolian_database.h"

void
database_part_del(Eolian_Part *part)
{
  if (!part) return;
  eina_stringshare_del(part->base.file);
  eina_stringshare_del(part->base.name);
  if (!part->base.validated) eina_stringshare_del(part->klass_name);
  database_doc_del(part->doc);
  free(part);
}

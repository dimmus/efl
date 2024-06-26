#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include <Efl_Shared.h>
#include "eolian_database.h"

void
database_implement_del(Eolian_Implement *impl)
{
  if (!impl) return;
  eina_stringshare_del(impl->base.file);
  eina_stringshare_del(impl->base.name);
  database_doc_del(impl->common_doc);
  database_doc_del(impl->get_doc);
  database_doc_del(impl->set_doc);
  free(impl);
}

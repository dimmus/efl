#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include <Efl_Shared.h>
#include "eolian_database.h"

void
database_parameter_del(Eolian_Function_Parameter *pdesc)
{
  eina_stringshare_del(pdesc->base.file);
  eina_stringshare_del(pdesc->base.name);

  database_type_del(pdesc->type);
  database_expr_del(pdesc->value);
  database_doc_del(pdesc->doc);
  free(pdesc);
}

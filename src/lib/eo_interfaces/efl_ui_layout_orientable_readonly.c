#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include <Efl_Eo_Interfaces.h>

typedef struct
{
} Efl_Ui_Layout_Orientable_Readonly_Data;

EOLIAN static void
_efl_ui_layout_orientable_readonly_efl_ui_layout_orientable_orientation_set(
  Eo *obj                                    EFL_UNUSED,
  Efl_Ui_Layout_Orientable_Readonly_Data *pd EFL_UNUSED,
  Efl_Ui_Layout_Orientation dir              EFL_UNUSED)
{
  EINA_LOG_ERR("This object does not allow setting an orientation");
}

#include "efl_ui_layout_orientable_readonly.eo.c"

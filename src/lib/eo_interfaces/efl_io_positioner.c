#include "efl_config.h"
#include "Efl_Eo_Interfaces.h"

EOLIAN static Efl_Bool
_efl_io_positioner_position_set(Eo *o, void *pd EFL_UNUSED, uint64_t position)
{
  return efl_io_positioner_seek(o, position, EFL_IO_POSITIONER_WHENCE_START) ==
         0;
}

#include "efl_io_positioner.eo.c"

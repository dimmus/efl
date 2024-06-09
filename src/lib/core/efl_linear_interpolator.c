#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include "Efl_Core.h"
#include "core_private.h"

#define MY_CLASS EFL_LINEAR_INTERPOLATOR_CLASS

typedef struct _Efl_Linear_Interpolator_Data Efl_Linear_Interpolator_Data;

struct _Efl_Linear_Interpolator_Data
{};

EOLIAN static double
_efl_linear_interpolator_efl_interpolator_interpolate(
  Eo *eo_obj                       EFL_UNUSED,
  Efl_Linear_Interpolator_Data *pd EFL_UNUSED,
  double                           progress)
{
  if ((progress < 0.0) || (progress > 1.0)) return progress;

  return core_animator_pos_map(progress, CORE_POS_MAP_LINEAR, 0, 0);
}

#include "efl_linear_interpolator.eo.c"

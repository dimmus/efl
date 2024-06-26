#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include "Efl_Core.h"
#include "core_private.h"

#define MY_CLASS EFL_SINUSOIDAL_INTERPOLATOR_CLASS

typedef struct _Efl_Sinusoidal_Interpolator_Data
  Efl_Sinusoidal_Interpolator_Data;

struct _Efl_Sinusoidal_Interpolator_Data
{
  double slope;
};

EOLIAN static double
_efl_sinusoidal_interpolator_efl_interpolator_interpolate(
  Eo *eo_obj                           EFL_UNUSED,
  Efl_Sinusoidal_Interpolator_Data *pd EFL_UNUSED,
  double                               progress)
{
  if ((progress < 0.0) || (progress > 1.0)) return progress;

  return core_animator_pos_map(progress,
                               CORE_POS_MAP_SINUSOIDAL_FACTOR,
                               pd->slope,
                               0);
}

EOLIAN static void
_efl_sinusoidal_interpolator_slope_set(Eo *eo_obj EFL_UNUSED,
                                       Efl_Sinusoidal_Interpolator_Data *pd,
                                       double                            slope)
{
  pd->slope = slope;
}

EOLIAN static double
_efl_sinusoidal_interpolator_slope_get(const Eo *eo_obj EFL_UNUSED,
                                       Efl_Sinusoidal_Interpolator_Data *pd
                                         EFL_UNUSED)
{
  return pd->slope;
}

EOLIAN static Efl_Object *
_efl_sinusoidal_interpolator_efl_object_constructor(
  Eo                               *eo_obj,
  Efl_Sinusoidal_Interpolator_Data *pd)
{
  eo_obj = efl_constructor(efl_super(eo_obj, MY_CLASS));

  pd->slope = 1.0;

  return eo_obj;
}

#include "efl_sinusoidal_interpolator.eo.c"

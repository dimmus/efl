#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include "Efl_Core.h"
#include "core_private.h"

#define MY_CLASS EFL_DIVISOR_INTERPOLATOR_CLASS

typedef struct _Efl_Divisor_Interpolator_Data Efl_Divisor_Interpolator_Data;

struct _Efl_Divisor_Interpolator_Data
{
  double divisor;
  int    power;
};

EOLIAN static double
_efl_divisor_interpolator_efl_interpolator_interpolate(
  Eo *eo_obj                        EFL_UNUSED,
  Efl_Divisor_Interpolator_Data *pd EFL_UNUSED,
  double                            progress)
{
  if ((progress < 0.0) || (progress > 1.0)) return progress;

  return core_animator_pos_map(progress,
                               CORE_POS_MAP_DIVISOR_INTERP,
                               pd->divisor,
                               (double)pd->power);
}

EOLIAN static void
_efl_divisor_interpolator_divisor_set(Eo *eo_obj                     EFL_UNUSED,
                                      Efl_Divisor_Interpolator_Data *pd,
                                      double                         divisor)
{
  pd->divisor = divisor;
}

EOLIAN static double
_efl_divisor_interpolator_divisor_get(const Eo *eo_obj               EFL_UNUSED,
                                      Efl_Divisor_Interpolator_Data *pd)
{
  return pd->divisor;
}

EOLIAN static void
_efl_divisor_interpolator_power_set(Eo *eo_obj                     EFL_UNUSED,
                                    Efl_Divisor_Interpolator_Data *pd,
                                    int                            power)
{
  pd->power = power;
}

EOLIAN static int
_efl_divisor_interpolator_power_get(const Eo *eo_obj               EFL_UNUSED,
                                    Efl_Divisor_Interpolator_Data *pd)
{
  return pd->power;
}

EOLIAN static Efl_Object *
_efl_divisor_interpolator_efl_object_constructor(
  Eo                            *eo_obj,
  Efl_Divisor_Interpolator_Data *pd)
{
  eo_obj = efl_constructor(efl_super(eo_obj, MY_CLASS));

  pd->divisor = 1.0;
  pd->power   = 1;

  return eo_obj;
}

#include "efl_divisor_interpolator.eo.c"

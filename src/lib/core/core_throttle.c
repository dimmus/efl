#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include <stdlib.h>
#include <unistd.h>

#include "Efl_Core.h"
#include "core_private.h"

static int throttle_val = 0;

EAPI void
Core_throttle_adjust(double amount)
{
    EINA_MAIN_LOOP_CHECK_RETURN;
    int adj       = amount * 1000000.0;
    throttle_val += adj;
    if (throttle_val < 0) throttle_val = 0;
}

EAPI double
Core_throttle_get(void)
{
    EINA_MAIN_LOOP_CHECK_RETURN_VAL(0.0);
    return (double)throttle_val / 1000000.0;
}

void
_core_throttle(void)
{
    if (throttle_val <= 0) return;
    eina_evlog("+throttle", NULL, 0.0, NULL);
    usleep(throttle_val);
    eina_evlog("-throttle", NULL, 0.0, NULL);
}

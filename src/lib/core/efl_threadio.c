#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include <Efl_Core.h>

#include "core_private.h"

#define MY_CLASS EFL_THREADIO_CLASS

typedef struct _Efl_ThreadIO_Data Efl_ThreadIO_Data;

struct _Efl_ThreadIO_Data
{
  void *indata, *outdata;
};

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

EOLIAN static void
_efl_threadio_indata_set(Eo *obj EFL_UNUSED, Efl_ThreadIO_Data *pd, void *data)
{
  pd->indata = data;
}

EOLIAN static void *
_efl_threadio_indata_get(const Eo *obj EFL_UNUSED, Efl_ThreadIO_Data *pd)
{
  return pd->indata;
}

EOLIAN static void
_efl_threadio_outdata_set(Eo *obj EFL_UNUSED, Efl_ThreadIO_Data *pd, void *data)
{
  pd->outdata = data;
}

EOLIAN static void *
_efl_threadio_outdata_get(const Eo *obj EFL_UNUSED, Efl_ThreadIO_Data *pd)
{
  return pd->outdata;
}

//////////////////////////////////////////////////////////////////////////

#include "efl_threadio.eo.c"

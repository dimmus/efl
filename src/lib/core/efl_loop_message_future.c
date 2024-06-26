#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include <Efl_Core.h>

#include "core_private.h"

#define MY_CLASS EFL_LOOP_MESSAGE_FUTURE_CLASS

//////////////////////////////////////////////////////////////////////////

typedef struct _Efl_Loop_Message_Future_Data Efl_Loop_Message_Future_Data;

struct _Efl_Loop_Message_Future_Data
{
  void *data;
};

//////////////////////////////////////////////////////////////////////////

EOLIAN static void
_efl_loop_message_future_data_set(Eo *obj                       EFL_UNUSED,
                                  Efl_Loop_Message_Future_Data *pd,
                                  void                         *data)
{
  pd->data = data;
}

EOLIAN static void *
_efl_loop_message_future_data_get(const Eo *obj                 EFL_UNUSED,
                                  Efl_Loop_Message_Future_Data *pd)
{
  return pd->data;
}

EOLIAN static Efl_Object *
_efl_loop_message_future_efl_object_constructor(Eo *obj,
                                                Efl_Loop_Message_Future_Data *pd
                                                  EFL_UNUSED)
{
  obj = efl_constructor(efl_super(obj, MY_CLASS));
  return obj;
}

EOLIAN static void
_efl_loop_message_future_efl_object_destructor(Eo *obj,
                                               Efl_Loop_Message_Future_Data *pd
                                                 EFL_UNUSED)
{
  efl_destructor(efl_super(obj, MY_CLASS));
}

//////////////////////////////////////////////////////////////////////////

#include "efl_loop_message_future.eo.c"

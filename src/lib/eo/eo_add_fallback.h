#ifndef _EO_ADD_FALLBACK_H
#define _EO_ADD_FALLBACK_H

#include <Efl_Shared.h>
#include <Efl_Eo.h>

typedef struct _Eo_Stack_Frame
{
  Eo *obj;
} Eo_Stack_Frame;

Efl_Bool _efl_add_fallback_init(void);
Efl_Bool _efl_add_fallback_shutdown(void);

Eo_Stack_Frame *_efl_add_fallback_stack_push(Eo *obj);
Eo_Stack_Frame *_efl_add_fallback_stack_pop(void);

#endif

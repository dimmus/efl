#ifndef EOLIAN_GEN_HEADERS_H
#define EOLIAN_GEN_HEADERS_H

#include "main.h"

void eo_gen_params(Eina_Iterator       *itr,
                   Efl_Strbuf          *buf,
                   Efl_Strbuf         **flagbuf,
                   int                 *nidx,
                   Eolian_Function_Type ftype);
void eo_gen_header_gen(const Eolian_State *state,
                       const Eolian_Class *cl,
                       Efl_Strbuf         *buf);

#endif

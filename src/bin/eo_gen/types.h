#ifndef EOLIAN_GEN_TYPES_H
#define EOLIAN_GEN_TYPES_H

void        eo_gen_types_header_gen(const Eolian_State *state,
                                    Eina_Iterator      *itr,
                                    Efl_Strbuf         *buf,
                                    Efl_Bool            full);
void        eo_gen_types_source_gen(Eina_Iterator *itr, Efl_Strbuf *buf);
Efl_Strbuf *eo_gen_class_typedef_gen(const Eolian_State *eos, const char *eof);

#endif

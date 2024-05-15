#ifndef INHERIT2_H
#define INHERIT2_H

Efl_Bool inherit2_print(Eo *obj);
Efl_Bool inherit2_print2(Eo *obj);

#define INHERIT2_CLASS inherit2_class_get()
const Efl_Class *inherit2_class_get(void);

#endif

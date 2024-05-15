#ifndef SINGLETON_H
#define SINGLETON_H

#include "Efl_Eo.h"

#define SINGLETON_CLASS singleton_class_get()
const Efl_Class *singleton_class_get(void);

#endif

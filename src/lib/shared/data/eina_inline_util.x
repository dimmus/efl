/* EINA - EFL data type library
 * Copyright (C) 2017 Cedric Bail
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library;
 * if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef EINA_UTIL_INLINE_H_
#define EINA_UTIL_INLINE_H_

#include <string.h>

static inline Efl_Bool
eina_dbl_exact(double a, double b)
{
   return memcmp(&a, &b, sizeof (double)) == 0;
}

static inline Efl_Bool
eina_flt_exact(float a, float b)
{
   return memcmp(&a, &b, sizeof (float)) == 0;
}

#endif /* EINA_UTIL_INLINE_H_ */

/* EINA - EFL data type library
 * Copyright (C) 2002-2008 Cedric Bail
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

#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include <stdlib.h>

#include "shared_config.h"
#include "eina_private.h"

/* undefs EINA_ARG_NONULL() so NULL checks are not compiled out! */
#include "eina_safety_checks.h"
#include "eina_accessor.h"

/*============================================================================*
*                                  Local                                     *
*============================================================================*/

/**
 * @cond LOCAL
 */

static const char EINA_MAGIC_ACCESSOR_STR[] = "Eina Accessor";

#define EINA_MAGIC_CHECK_ACCESSOR(d)                   \
    do                                                 \
    {                                                  \
        if (!EINA_MAGIC_CHECK(d, EINA_MAGIC_ACCESSOR)) \
        {                                              \
            EINA_MAGIC_FAIL(d, EINA_MAGIC_ACCESSOR);   \
        }                                              \
    }                                                  \
    while (0)

/**
 * @endcond
 */

/*============================================================================*
*                                 Global                                     *
*============================================================================*/

/**
 * @internal
 * @brief Initialize the accessor module.
 *
 * @return #EFL_TRUE on success, #EFL_FALSE on failure.
 *
 * This function sets up the accessor module of Eina. It is called by
 * eina_init().
 *
 * @see eina_init()
 */
Efl_Bool
eina_accessor_init(void)
{
  return eina_magic_string_set(EINA_MAGIC_ACCESSOR, EINA_MAGIC_ACCESSOR_STR);
}

/**
 * @internal
 * @brief Shut down the accessor module.
 *
 * @return #EFL_TRUE on success, #EFL_FALSE on failure.
 *
 * This function shuts down the accessor module set up by
 * eina_accessor_init(). It is called by eina_shutdown().
 *
 * @see eina_shutdown()
 */
Efl_Bool
eina_accessor_shutdown(void)
{
  return EFL_TRUE;
}

/*============================================================================*
*                                   API                                      *
*============================================================================*/

EINA_API void
eina_accessor_free(Eina_Accessor *accessor)
{
  if (!accessor) return;

  EINA_MAGIC_CHECK_ACCESSOR(accessor);
  EINA_SAFETY_ON_NULL_RETURN(accessor->free);
  accessor->free(accessor);
}

EINA_API void *
eina_accessor_container_get(Eina_Accessor *accessor)
{
  EINA_MAGIC_CHECK_ACCESSOR(accessor);
  EINA_SAFETY_ON_NULL_RETURN_VAL(accessor, NULL);
  EINA_SAFETY_ON_NULL_RETURN_VAL(accessor->get_container, NULL);
  return accessor->get_container(accessor);
}

EINA_API Efl_Bool
eina_accessor_data_get(Eina_Accessor *accessor,
                       unsigned int   position,
                       void         **data)
{
  EINA_MAGIC_CHECK_ACCESSOR(accessor);
  EINA_SAFETY_ON_NULL_RETURN_VAL(accessor, EFL_FALSE);
  EINA_SAFETY_ON_NULL_RETURN_VAL(accessor->get_at, EFL_FALSE);
  EINA_SAFETY_ON_NULL_RETURN_VAL(data, EFL_FALSE);
  return accessor->get_at(accessor, position, data);
}

EINA_API void
eina_accessor_over(Eina_Accessor *accessor,
                   Eina_Each_Cb   cb,
                   unsigned int   start,
                   unsigned int   end,
                   const void    *fdata)
{
  const void  *container;
  void        *data;
  unsigned int i;

  if (!accessor) return;

  EINA_MAGIC_CHECK_ACCESSOR(accessor);
  EINA_SAFETY_ON_NULL_RETURN(accessor->get_container);
  EINA_SAFETY_ON_NULL_RETURN(accessor->get_at);
  EINA_SAFETY_ON_NULL_RETURN(cb);
  EINA_SAFETY_ON_FALSE_RETURN(start < end);

  if (!eina_accessor_lock(accessor)) return;

  container = accessor->get_container(accessor);
  for (i = start; i < end && accessor->get_at(accessor, i, &data) == EFL_TRUE;
       ++i)
    if (cb(container, data, (void *)fdata) != EFL_TRUE) goto on_exit;

on_exit:
  (void)eina_accessor_unlock(accessor);
}

EINA_API Efl_Bool
eina_accessor_lock(Eina_Accessor *accessor)
{
  EINA_MAGIC_CHECK_ACCESSOR(accessor);
  EINA_SAFETY_ON_NULL_RETURN_VAL(accessor, EFL_FALSE);

  if (accessor->lock) return accessor->lock(accessor);
  return EFL_TRUE;
}

EINA_API Eina_Accessor *
eina_accessor_clone(Eina_Accessor *accessor)
{
  EINA_MAGIC_CHECK_ACCESSOR(accessor);
  EINA_SAFETY_ON_NULL_RETURN_VAL(accessor, NULL);

  if (accessor->clone) return accessor->clone(accessor);

  return NULL;
}

EINA_API Efl_Bool
eina_accessor_unlock(Eina_Accessor *accessor)
{
  EINA_MAGIC_CHECK_ACCESSOR(accessor);
  EINA_SAFETY_ON_NULL_RETURN_VAL(accessor, EFL_FALSE);

  if (accessor->unlock) return accessor->unlock(accessor);
  return EFL_TRUE;
}

typedef struct _Eina_Accessor_CArray_Length Eina_Accessor_CArray_Length;

struct _Eina_Accessor_CArray_Length
{
  Eina_Accessor accessor;

  void **array;

  void       **end;
  unsigned int step;
};

static Efl_Bool
eina_carray_length_accessor_get_at(Eina_Accessor_CArray_Length *accessor,
                                   unsigned int                 idx,
                                   void                       **data)
{
  if ((char *)accessor->array + idx * accessor->step >= (char *)accessor->end)
    return EFL_FALSE;

  memcpy(data, (char *)accessor->array + idx * accessor->step, accessor->step);

  return EFL_TRUE;
}

static Efl_Bool
eina_carray_length_accessor_ptr_get_at(Eina_Accessor_CArray_Length *accessor,
                                       unsigned int                 idx,
                                       void                       **data)
{
  if ((char *)accessor->array + idx * accessor->step >= (char *)accessor->end)
    return EFL_FALSE;

  *data = (((char *)accessor->array) + idx * accessor->step);

  return EFL_TRUE;
}

static void **
eina_carray_length_accessor_get_container(Eina_Accessor_CArray_Length *accessor)
{
  return accessor->array;
}

static void
eina_carray_length_accessor_free(Eina_Accessor_CArray_Length *accessor)
{
  free(accessor);
}

EINA_API Eina_Accessor *
eina_carray_length_accessor_new(void       **array,
                                unsigned int step,
                                unsigned int length)
{
  Eina_Accessor_CArray_Length *accessor;

  accessor = calloc(1, sizeof(Eina_Accessor_CArray_Length));
  if (!accessor) return NULL;

  EINA_MAGIC_SET(&accessor->accessor, EINA_MAGIC_ACCESSOR);

  accessor->array = array;
  accessor->end   = (void **)((char *)accessor->array + length * step);
  accessor->step  = step;

  accessor->accessor.version = EINA_ACCESSOR_VERSION;
  accessor->accessor.get_at =
    FUNC_ACCESSOR_GET_AT(eina_carray_length_accessor_get_at);
  accessor->accessor.get_container =
    FUNC_ACCESSOR_GET_CONTAINER(eina_carray_length_accessor_get_container);
  accessor->accessor.free =
    FUNC_ACCESSOR_FREE(eina_carray_length_accessor_free);

  return &accessor->accessor;
}

EINA_API Eina_Accessor *
eina_carray_length_ptr_accessor_new(void       **array,
                                    unsigned int step,
                                    unsigned int length)
{
  Eina_Accessor_CArray_Length *accessor;

  accessor = calloc(1, sizeof(Eina_Accessor_CArray_Length));
  if (!accessor) return NULL;

  EINA_MAGIC_SET(&accessor->accessor, EINA_MAGIC_ACCESSOR);

  accessor->array = array;
  accessor->end   = (void **)((char *)accessor->array + length * step);
  accessor->step  = step;

  accessor->accessor.version = EINA_ACCESSOR_VERSION;
  accessor->accessor.get_at =
    FUNC_ACCESSOR_GET_AT(eina_carray_length_accessor_ptr_get_at);
  accessor->accessor.get_container =
    FUNC_ACCESSOR_GET_CONTAINER(eina_carray_length_accessor_get_container);
  accessor->accessor.free =
    FUNC_ACCESSOR_FREE(eina_carray_length_accessor_free);

  return &accessor->accessor;
}

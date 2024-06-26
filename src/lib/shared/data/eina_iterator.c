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
#include "eina_iterator.h"

#include "eina_list.h"

/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/

/**
 * @cond LOCAL
 */

static const char EINA_MAGIC_ITERATOR_STR[] = "Eina Iterator";

#define EINA_MAGIC_CHECK_ITERATOR(d)                   \
    do                                                 \
    {                                                  \
        if (!EINA_MAGIC_CHECK(d, EINA_MAGIC_ITERATOR)) \
        {                                              \
            EINA_MAGIC_FAIL(d, EINA_MAGIC_ITERATOR);   \
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
 * @brief Initialize the iterator module.
 *
 * @return #EFL_TRUE on success, #EFL_FALSE on failure.
 *
 * This function sets up the iterator module of Eina. It is called by
 * eina_init().
 *
 * @see eina_init()
 */
Efl_Bool
eina_iterator_init(void)
{
  return eina_magic_string_set(EINA_MAGIC_ITERATOR, EINA_MAGIC_ITERATOR_STR);
}

/**
 * @internal
 * @brief Shut down the iterator module.
 *
 * @return #EFL_TRUE on success, #EFL_FALSE on failure.
 *
 * This function shuts down the iterator module set up by
 * eina_iterator_init(). It is called by eina_shutdown().
 *
 * @see eina_shutdown()
 */
Efl_Bool
eina_iterator_shutdown(void)
{
  return EFL_TRUE;
}

/*============================================================================*
 *                                   API                                      *
 *============================================================================*/

EINA_API void
eina_iterator_free(Eina_Iterator *iterator)
{
  if (!iterator) return;

  EINA_MAGIC_CHECK_ITERATOR(iterator);
  EINA_SAFETY_ON_NULL_RETURN(iterator->free);
  iterator->free(iterator);
}

EINA_API void *
eina_iterator_container_get(Eina_Iterator *iterator)
{
  EINA_MAGIC_CHECK_ITERATOR(iterator);
  EINA_SAFETY_ON_NULL_RETURN_VAL(iterator, NULL);
  EINA_SAFETY_ON_NULL_RETURN_VAL(iterator->get_container, NULL);
  return iterator->get_container(iterator);
}

EINA_API Efl_Bool
eina_iterator_next(Eina_Iterator *iterator, void **data)
{
  if (!iterator) return EFL_FALSE;

  EINA_MAGIC_CHECK_ITERATOR(iterator);
  EINA_SAFETY_ON_NULL_RETURN_VAL(iterator->next, EFL_FALSE);
  EINA_SAFETY_ON_NULL_RETURN_VAL(data, EFL_FALSE);
  return iterator->next(iterator, data);
}

EINA_API void
eina_iterator_foreach(Eina_Iterator *iterator,
                      Eina_Each_Cb   cb,
                      const void    *fdata)
{
  const void *container;
  void       *data;

  if (!iterator) return;

  EINA_MAGIC_CHECK_ITERATOR(iterator);
  EINA_SAFETY_ON_NULL_RETURN(iterator->get_container);
  EINA_SAFETY_ON_NULL_RETURN(iterator->next);
  EINA_SAFETY_ON_NULL_RETURN(cb);

  if (!eina_iterator_lock(iterator)) return;

  container = iterator->get_container(iterator);
  while (iterator->next(iterator, &data) == EFL_TRUE)
  {
    if (cb(container, data, (void *)fdata) != EFL_TRUE) goto on_exit;
  }

on_exit:
  (void)eina_iterator_unlock(iterator);
}

EINA_API Efl_Bool
eina_iterator_lock(Eina_Iterator *iterator)
{
  EINA_MAGIC_CHECK_ITERATOR(iterator);
  EINA_SAFETY_ON_NULL_RETURN_VAL(iterator, EFL_FALSE);

  if (iterator->lock) return iterator->lock(iterator);
  return EFL_TRUE;
}

EINA_API Efl_Bool
eina_iterator_unlock(Eina_Iterator *iterator)
{
  EINA_MAGIC_CHECK_ITERATOR(iterator);
  EINA_SAFETY_ON_NULL_RETURN_VAL(iterator, EFL_FALSE);

  if (iterator->unlock) return iterator->unlock(iterator);
  return EFL_TRUE;
}

typedef struct _Eina_Iterator_CArray Eina_Iterator_CArray;

struct _Eina_Iterator_CArray
{
  Eina_Iterator iterator;

  void **array;
  void **current;
};

static Efl_Bool
eina_carray_iterator_next(Eina_Iterator_CArray *it, void **data)
{
  if (!it->current || !*it->current) return EFL_FALSE;

  *data = *it->current++;

  return EFL_TRUE;
}

static void **
eina_carray_iterator_get_container(Eina_Iterator_CArray *it)
{
  return it->array;
}

static void
eina_carray_iterator_free(Eina_Iterator_CArray *it)
{
  free(it);
}

EINA_API Eina_Iterator *
eina_carray_iterator_new(void **array)
{
  Eina_Iterator_CArray *it;

  it = calloc(1, sizeof(Eina_Iterator_CArray));
  if (!it) return NULL;

  EINA_MAGIC_SET(&it->iterator, EINA_MAGIC_ITERATOR);

  it->array = it->current = array;

  it->iterator.version = EINA_ITERATOR_VERSION;
  it->iterator.next    = FUNC_ITERATOR_NEXT(eina_carray_iterator_next);
  it->iterator.get_container =
    FUNC_ITERATOR_GET_CONTAINER(eina_carray_iterator_get_container);
  it->iterator.free = FUNC_ITERATOR_FREE(eina_carray_iterator_free);

  return &it->iterator;
}

typedef struct _Eina_Iterator_CArray_Length Eina_Iterator_CArray_Length;

struct _Eina_Iterator_CArray_Length
{
  Eina_Iterator iterator;

  void    **array;
  uintptr_t current;

  uintptr_t    end;
  unsigned int step;
};

static Efl_Bool
eina_carray_length_iterator_next(Eina_Iterator_CArray_Length *it, void **data)
{
  if (it->current >= it->end) return EFL_FALSE;

  memcpy(data, (void *)it->current, it->step);
  it->current += it->step;

  return EFL_TRUE;
}

static void **
eina_carray_length_iterator_get_container(Eina_Iterator_CArray_Length *it)
{
  return it->array;
}

static void
eina_carray_length_iterator_free(Eina_Iterator_CArray_Length *it)
{
  free(it);
}

EINA_API Eina_Iterator *
eina_carray_length_iterator_new(void       **array,
                                unsigned int step,
                                unsigned int length)
{
  Eina_Iterator_CArray_Length *it;

  it = calloc(1, sizeof(Eina_Iterator_CArray_Length));
  if (!it) return NULL;

  EINA_MAGIC_SET(&it->iterator, EINA_MAGIC_ITERATOR);

  it->array   = array;
  it->current = (uintptr_t)it->array;
  it->end     = it->current + length * step;
  it->step    = step;

  it->iterator.version = EINA_ITERATOR_VERSION;
  it->iterator.next    = FUNC_ITERATOR_NEXT(eina_carray_length_iterator_next);
  it->iterator.get_container =
    FUNC_ITERATOR_GET_CONTAINER(eina_carray_length_iterator_get_container);
  it->iterator.free = FUNC_ITERATOR_FREE(eina_carray_length_iterator_free);

  return &it->iterator;
}

typedef struct _Eina_Iterator_Multi Eina_Multi_Iterator;

struct _Eina_Iterator_Multi
{
  Eina_Iterator iterator;

  Eina_List *iterators;
};

static Efl_Bool
eina_multi_iterator_next(Eina_Multi_Iterator *it, void **data)
{
  if (!it->iterators) return EFL_FALSE;

   // Search for an iterator that do have some data
  while (!eina_iterator_next(eina_list_data_get(it->iterators), data))
  {
    eina_iterator_free(eina_list_data_get(it->iterators));
    it->iterators = eina_list_remove_list(it->iterators, it->iterators);

    if (!it->iterators) return EFL_FALSE;
  }

  return EFL_TRUE;
}

static void **
eina_multi_iterator_get_container(Eina_Multi_Iterator *it)
{
  if (!it->iterators) return NULL;
  return eina_iterator_container_get(eina_list_data_get(it->iterators));
}

static void
eina_multi_iterator_free(Eina_Multi_Iterator *it)
{
  Eina_Iterator *itc;

  EINA_LIST_FREE(it->iterators, itc)
  eina_iterator_free(itc);
  free(it);
}

EINA_API Eina_Iterator *
eina_multi_iterator_internal_new(Eina_Iterator *itc, ...)
{
  Eina_Multi_Iterator *it;
  va_list              args;

  it = calloc(1, sizeof(Eina_Multi_Iterator));
  if (!it) return NULL;

  EINA_MAGIC_SET(&it->iterator, EINA_MAGIC_ITERATOR);

  it->iterators = eina_list_append(it->iterators, itc);

  va_start(args, itc);

  while ((itc = (Eina_Iterator *)va_arg(args, Eina_Iterator *)))
  {
    it->iterators = eina_list_append(it->iterators, itc);
  }

  va_end(args);

  it->iterator.version = EINA_ITERATOR_VERSION;
  it->iterator.next    = FUNC_ITERATOR_NEXT(eina_multi_iterator_next);
  it->iterator.get_container =
    FUNC_ITERATOR_GET_CONTAINER(eina_multi_iterator_get_container);
  it->iterator.free = FUNC_ITERATOR_FREE(eina_multi_iterator_free);

  return &it->iterator;
}

typedef struct
{
  Eina_Iterator iterator;

  void          *data;
  Eina_Iterator *original;
  Eina_Each_Cb   cb;
  Eina_Free_Cb   free;
} Eina_Iterator_Filter;

static Efl_Bool
eina_iterator_filter_next(Eina_Iterator_Filter *it, void **data)
{
  do
  {
    if (!eina_iterator_next(it->original, data)) return EFL_FALSE;
  }
  while (!it->cb(it->original, *data, it->data));

  return EFL_TRUE;
}

static void *
eina_iterator_filter_get_container(Eina_Iterator_Filter *it)
{
  return it->original;
}

static void
eina_iterator_filter_free(Eina_Iterator_Filter *it)
{
  if (it->free) it->free(it->data);
  eina_iterator_free(it->original);
  free(it);
}

EINA_API Eina_Iterator *
eina_iterator_filter_new(Eina_Iterator *iterator,
                         Eina_Each_Cb   filter,
                         Eina_Free_Cb   free_cb,
                         void          *data)
{
  Eina_Iterator_Filter *it;

  EINA_SAFETY_ON_NULL_RETURN_VAL(iterator, NULL);
  EINA_SAFETY_ON_NULL_RETURN_VAL(filter, NULL);

  it = calloc(1, sizeof(Eina_Iterator_Filter));

  it->original = iterator;
  it->data     = data;
  it->cb       = filter;
  it->free     = free_cb;

  EINA_MAGIC_SET(&it->iterator, EINA_MAGIC_ITERATOR);

  it->iterator.version = EINA_ITERATOR_VERSION;
  it->iterator.next    = FUNC_ITERATOR_NEXT(eina_iterator_filter_next);
  it->iterator.get_container =
    FUNC_ITERATOR_GET_CONTAINER(eina_iterator_filter_get_container);
  it->iterator.free = FUNC_ITERATOR_FREE(eina_iterator_filter_free);

  return &it->iterator;
}

typedef struct
{
  Eina_Iterator iterator;

  void           *data;
  Eina_Iterator  *original;
  Eina_Process_Cb cb;
  Eina_Free_Cb    free;
} Eina_Iterator_Processor;

static Efl_Bool
eina_iterator_process_next(Eina_Iterator_Processor *it, void **data)
{
  if (!eina_iterator_next(it->original, data)) return EFL_FALSE;

  *data = it->cb(it->original, *data, it->data);

  return EFL_TRUE;
}

static void *
eina_iterator_process_get_container(Eina_Iterator_Processor *it)
{
  return it->original;
}

static void
eina_iterator_process_free(Eina_Iterator_Processor *it)
{
  if (it->free) it->free(it->data);
  eina_iterator_free(it->original);
  free(it);
}

EINA_API Eina_Iterator *
eina_iterator_processed_new(Eina_Iterator  *iterator,
                            Eina_Process_Cb process,
                            Eina_Free_Cb    free_cb,
                            void           *data)
{
  Eina_Iterator_Processor *it;

  EINA_SAFETY_ON_NULL_RETURN_VAL(iterator, NULL);
  EINA_SAFETY_ON_NULL_RETURN_VAL(process, NULL);

  it           = calloc(1, sizeof(Eina_Iterator_Processor));
  it->data     = data;
  it->cb       = process;
  it->free     = free_cb;
  it->original = iterator;

  EINA_MAGIC_SET(&it->iterator, EINA_MAGIC_ITERATOR);

  it->iterator.version = EINA_ITERATOR_VERSION;
  it->iterator.next    = FUNC_ITERATOR_NEXT(eina_iterator_process_next);
  it->iterator.get_container =
    FUNC_ITERATOR_GET_CONTAINER(eina_iterator_process_get_container);
  it->iterator.free = FUNC_ITERATOR_FREE(eina_iterator_process_free);

  return &it->iterator;
}

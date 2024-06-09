/* EINA - EFL data type library
 * Copyright (C) 2008 Carsten Haitzler, Vincent Torri, Jorge Luis Zapata Muga
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

#ifndef EINA_PRIVATE_H_
#define EINA_PRIVATE_H_

#include <stdarg.h>
#include <locale.h>
#if defined(__MacOSX__) || (defined(__MACH__) && defined(__APPLE__))
#  include <xlocale.h>
#endif

#ifdef _WIN32
#  include <windows.h>
#  include <bcrypt.h>
#endif

#include "eina_magic.h"
#include "eina_iterator.h"
#include "eina_accessor.h"
#include "eina_stringshare.h"

#ifdef _WIN32

#  define EINA_PATH_SEP_SWAP(p, s1, s2) \
   do { \
     char *iter = p; \
     while (*iter) \
       { \
          if (*iter == s1) \
            *iter = s2; \
          iter++; \
       } \
   } while (0)

#  define EINA_PATH_TO_UNIX(p) EINA_PATH_SEP_SWAP(p, '\\', '/')
#  define EINA_PATH_TO_WIN32(p) EINA_PATH_SEP_SWAP(p, '/', '\\')
#endif

#ifndef ABS
#  define ABS(x) ((x) < 0 ? -(x) : (x))
#endif

#ifndef CLAMP
#  define CLAMP(x, min, max) (((x) > (max)) ? (max) : (((x) < (min)) ? (min) : (x)))
#endif

#define EINA_INLIST_JUMP_SIZE 256

#define READBUFSIZ 65536

#define EINA_LOG_COLOR_DEFAULT "\033[36m"

/* eina magic types */
#define EINA_MAGIC_SHARE 0x98761234
#define EINA_MAGIC_SHARE_HEAD 0x98761235
#define EINA_MAGIC_STRINGSHARE_NODE 0x98761254
#define EINA_MAGIC_USTRINGSHARE_NODE 0x98761255
#define EINA_MAGIC_BINSHARE_NODE 0x98761256

#define EINA_MAGIC_LIST 0x98761237
#define EINA_MAGIC_LIST_ITERATOR 0x98761238
#define EINA_MAGIC_LIST_ACCESSOR 0x98761239
#define EINA_MAGIC_LIST_ACCOUNTING 0x9876123a

#define EINA_MAGIC_ARRAY 0x9876123b
#define EINA_MAGIC_ARRAY_ITERATOR 0x9876123c
#define EINA_MAGIC_ARRAY_ACCESSOR 0x9876123d

#define EINA_MAGIC_HASH 0x9876123e
#define EINA_MAGIC_HASH_ITERATOR 0x9876123f

#define EINA_MAGIC_TILER 0x98761240
#define EINA_MAGIC_TILER_ITERATOR 0x98761241

#define EINA_MAGIC_MATRIXSPARSE 0x98761242
#define EINA_MAGIC_MATRIXSPARSE_ROW 0x98761243
#define EINA_MAGIC_MATRIXSPARSE_CELL 0x98761244
#define EINA_MAGIC_MATRIXSPARSE_ITERATOR 0x98761245
#define EINA_MAGIC_MATRIXSPARSE_ROW_ITERATOR 0x98761246
#define EINA_MAGIC_MATRIXSPARSE_ROW_ACCESSOR 0x98761247
#define EINA_MAGIC_MATRIXSPARSE_CELL_ITERATOR 0x98761248
#define EINA_MAGIC_MATRIXSPARSE_CELL_ACCESSOR 0x98761249

#define EINA_MAGIC_STRBUF 0x98761250
#define EINA_MAGIC_USTRBUF 0x98761257
#define EINA_MAGIC_BINBUF 0x98761258

#define EINA_MAGIC_QUADTREE 0x98761251
#define EINA_MAGIC_QUADTREE_ROOT 0x98761252
#define EINA_MAGIC_QUADTREE_ITEM 0x98761253

#define EINA_MAGIC_SIMPLE_XML_TAG 0x98761260
#define EINA_MAGIC_SIMPLE_XML_DATA 0x98761261
#define EINA_MAGIC_SIMPLE_XML_ATTRIBUTE 0x98761262

#define EINA_MAGIC_INARRAY 0x98761270
#define EINA_MAGIC_INARRAY_ITERATOR 0x98761271
#define EINA_MAGIC_INARRAY_ACCESSOR 0x98761272

#define EINA_MAGIC_MODEL 0x98761280

#define EINA_MAGIC_CLASS 0x9877CB30

/* undef the following, we want our version */
#undef FREE
#define FREE(ptr)				\
  do {						\
     free(ptr);					\
     ptr = NULL;				\
  } while(0);

#undef IF_FREE
#define IF_FREE(ptr)				\
  do {						\
     if (ptr) {					\
	free(ptr);				\
	ptr = NULL;				\
     }						\
  } while(0);

#undef IF_FN_DEL
#define IF_FN_DEL(_fn, ptr)			\
  do {						\
     if (ptr) {					\
	_fn(ptr);				\
	ptr = NULL;				\
     }						\
  } while(0);

#define MAGIC_FREE(ptr)				\
  do {						\
     if (ptr) {					\
	EINA_MAGIC_SET(ptr, EINA_MAGIC_NONE);	\
	FREE(ptr);				\
     }						\
  } while(0);

#ifdef EFL_HAVE_THREADS
extern Efl_Bool _threads_activated;

void eina_share_common_threads_init(void);
void eina_share_common_threads_shutdown(void);
void eina_log_threads_init(void);
void eina_log_threads_shutdown(void);
#endif

Efl_Bool eina_prefix_init(void);
Efl_Bool eina_prefix_shutdown(void);
Efl_Bool eina_magic_string_init(void);
Efl_Bool eina_magic_string_shutdown(void);
Efl_Bool eina_simple_xml_init(void);
Efl_Bool eina_simple_xml_shutdown(void);
Efl_Bool pass_through_init(void);
void     pass_through_shutdown(void);
Efl_Bool eina_vpath_init(void);
Efl_Bool eina_vpath_shutdown(void);
Efl_Bool eina_module_init(void);
Efl_Bool eina_module_shutdown(void);
Efl_Bool eina_file_init(void);
Efl_Bool eina_file_shutdown(void);
Efl_Bool eina_convert_init(void);
Efl_Bool eina_convert_shutdown(void);
Efl_Bool eina_thread_queue_init(void);
Efl_Bool eina_thread_queue_shutdown(void);
Efl_Bool eina_benchmark_init(void);
Efl_Bool eina_benchmark_shutdown(void);
Efl_Bool eina_ustringshare_init(void);
Efl_Bool eina_ustringshare_shutdown(void);
Efl_Bool eina_tmpstr_init(void);
Efl_Bool eina_tmpstr_shutdown(void);
Efl_Bool eina_stringshare_init(void);
Efl_Bool eina_stringshare_shutdown(void);
Efl_Bool eina_ustringshare_init(void);
Efl_Bool eina_ustringshare_shutdown(void);
Efl_Bool eina_slstr_init(void);
Efl_Bool eina_slstr_shutdown(void);
Efl_Bool eina_safety_checks_init(void);
Efl_Bool eina_safety_checks_shutdown(void);
Efl_Bool eina_safepointer_init(void);
Efl_Bool eina_safepointer_shutdown(void);
Efl_Bool eina_mempool_init(void);
Efl_Bool eina_mempool_shutdown(void);
Efl_Bool eina_value_init(void);
Efl_Bool eina_value_shutdown(void);
Efl_Bool eina_cow_init(void);
Efl_Bool eina_cow_shutdown(void);
Efl_Bool eina_binshare_init(void);
Efl_Bool eina_binshare_shutdown(void);
Efl_Bool eina_evlog_init(void);
Efl_Bool eina_evlog_shutdown(void);
Efl_Bool eina_debug_init(void);
Efl_Bool eina_debug_shutdown(void);
Efl_Bool eina_error_init(void);
Efl_Bool eina_error_shutdown(void);
Efl_Bool eina_array_init(void);
Efl_Bool eina_array_shutdown(void);
Efl_Bool eina_abstract_content_init(void);
Efl_Bool eina_abstract_content_shutdown(void);
Efl_Bool eina_debug_init(void);
Efl_Bool eina_debug_shutdown(void);
Efl_Bool eina_rbtree_init(void);
Efl_Bool eina_rbtree_shutdown(void);
Efl_Bool eina_rectangle_init(void);
Efl_Bool eina_rectangle_shutdown(void);
Efl_Bool eina_promise_init(void);
Efl_Bool eina_promise_shutdown(void);
Efl_Bool eina_quadtree_init(void);
Efl_Bool eina_quadtree_shutdown(void);
Efl_Bool eina_matrixsparse_init(void);
Efl_Bool eina_matrixsparse_shutdown(void);
Efl_Bool eina_list_init(void);
Efl_Bool eina_list_shutdown(void);
Efl_Bool eina_iterator_init(void);
Efl_Bool eina_iterator_shutdown(void);
Efl_Bool eina_inarray_init(void);
Efl_Bool eina_inarray_shutdown(void);
Efl_Bool eina_abstract_content_init(void);
Efl_Bool eina_abstract_content_shutdown(void);
Efl_Bool eina_accessor_init(void);
Efl_Bool eina_accessor_shutdown(void);
// Efl_Bool eina__init(void);
// Efl_Bool eina__shutdown(void);
// Efl_Bool eina__init(void);
// Efl_Bool eina__shutdown(void);
// Efl_Bool eina__init(void);
// Efl_Bool eina__shutdown(void);
// Efl_Bool eina__init(void);
// Efl_Bool eina__shutdown(void);

char *_eina_debug_chunk_strdup(const char *str);
void *_eina_debug_chunk_push(int size);
void *_eina_debug_chunk_realloc(int size);
void *_eina_debug_chunk_tmp_push(int size);
void  _eina_debug_chunk_tmp_reset(void);

#ifdef EINA_STATIC_BUILD_CHAINED_POOL
Efl_Bool chained_init(void);
void     chained_shutdown(void);
#endif

#ifdef EINA_STATIC_BUILD_ONE_BIG
Efl_Bool one_big_init(void);
void     one_big_shutdown(void);
#endif

#ifdef EINA_STATIC_BUILD_PASS_THROUGH
Efl_Bool pass_through_init(void);
void     pass_through_shutdown(void);
#endif

Efl_Bool eina_cpu_init(void);
Efl_Bool eina_cpu_shutdown(void);
void     eina_cpu_count_internal(void);

Efl_Bool eina_file_mmap_faulty(void *addr, long page_size);

#ifndef EINA_FREEQ_H_
typedef struct _Eina_FreeQ Eina_FreeQ;
#endif

Eina_Stringshare *eina_file_sanitize(const char *path);

void eina_freeq_main_set(Eina_FreeQ *fq);

#ifdef _WIN32
extern BCRYPT_ALG_HANDLE _eina_bcrypt_provider;
typedef _locale_t        locale_t;
#endif
locale_t _eina_c_locale_get(void);

#include "eina_inline_private.h"

#endif /* EINA_PRIVATE_H_ */

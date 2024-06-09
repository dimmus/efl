#ifndef EINA_STRBUF_COMMON_H
#define EINA_STRBUF_COMMON_H

#include <stdlib.h>

#include "eina_private.h"
#include "eina_magic.h"
#include "eina_strbuf.h"

/**
 * @struct _Eina_Strbuf
 * String buffer to facilitate string operations.
 */
struct _Eina_Strbuf
{
  void  *buf;
  size_t len;
  size_t size;
  size_t step;

  EINA_MAGIC;

  Efl_Bool ro : 1;
};

#define EINA_MAGIC_CHECK_STRBUF(d, ...)                         \
   do {                                                         \
        if (!(d)) return __VA_ARGS__;                           \
        if (!EINA_MAGIC_CHECK((d), _STRBUF_MAGIC))              \
          {                                                     \
             EINA_MAGIC_FAIL((d), _STRBUF_MAGIC);               \
             return __VA_ARGS__;                                \
          }                                                     \
     } while (0)

Efl_Bool eina_strbuf_common_init(void);

Efl_Bool    eina_strbuf_common_shutdown(void);
Efl_Strbuf *eina_strbuf_common_new(size_t csize);
Efl_Strbuf *eina_strbuf_common_manage_new(size_t csize, void *str, size_t len);
Efl_Strbuf *
eina_strbuf_common_manage_ro_new(size_t csize, const void *str, size_t len);

void eina_strbuf_common_free(Efl_Strbuf *buf);
void eina_strbuf_common_reset(size_t csize, Efl_Strbuf *buf);

Eina_Rw_Slice eina_strbuf_common_expand(size_t      csize,
                                        Efl_Strbuf *buf,
                                        size_t      minimum_unused_space);
Efl_Bool      eina_strbuf_common_use(Efl_Strbuf *buf, size_t extra_bytes);

Efl_Bool eina_strbuf_common_append(size_t      csize,
                                   Efl_Strbuf *buf,
                                   const void *str,
                                   size_t      len);
Efl_Bool eina_strbuf_common_append_escaped(size_t      csize,
                                           Efl_Strbuf *buf,
                                           const void *str);
Efl_Bool eina_strbuf_common_append_n(size_t      csize,
                                     Efl_Strbuf *buf,
                                     const void *str,
                                     size_t      len,
                                     size_t      maxlen);
Efl_Bool eina_strbuf_common_append_length(size_t      csize,
                                          Efl_Strbuf *buf,
                                          const void *str,
                                          size_t      length);
Efl_Bool eina_strbuf_common_insert(size_t      csize,
                                   Efl_Strbuf *buf,
                                   const void *str,
                                   size_t      len,
                                   size_t      pos);
Efl_Bool eina_strbuf_common_insert_escaped(size_t      csize,
                                           Efl_Strbuf *buf,
                                           const void *str,
                                           size_t      len,
                                           size_t      pos);
Efl_Bool eina_strbuf_common_insert_n(size_t      csize,
                                     Efl_Strbuf *buf,
                                     const void *str,
                                     size_t      len,
                                     size_t      maxlen,
                                     size_t      pos);
Efl_Bool eina_strbuf_common_insert_length(size_t      csize,
                                          Efl_Strbuf *buf,
                                          const void *str,
                                          size_t      length,
                                          size_t      pos);
Efl_Bool
eina_strbuf_common_append_char(size_t csize, Efl_Strbuf *buf, const void *c);
Efl_Bool      eina_strbuf_common_insert_char(size_t      csize,
                                             Efl_Strbuf *buf,
                                             const void *c,
                                             size_t      pos);
Efl_Bool      eina_strbuf_common_remove(size_t      csize,
                                        Efl_Strbuf *buf,
                                        size_t      start,
                                        size_t      end);
const void   *eina_strbuf_common_string_get(const Efl_Strbuf *buf);
void         *eina_strbuf_common_string_steal(size_t csize, Efl_Strbuf *buf);
void          eina_strbuf_common_string_free(size_t csize, Efl_Strbuf *buf);
size_t        eina_strbuf_common_length_get(const Efl_Strbuf *buf);
Eina_Slice    eina_strbuf_common_slice_get(const Efl_Strbuf *buf);
Eina_Rw_Slice eina_strbuf_common_rw_slice_get(const Efl_Strbuf *buf);

Efl_Bool _eina_strbuf_common_grow(size_t csize, Efl_Strbuf *buf, size_t size);
/**
 * @}
 */

#endif

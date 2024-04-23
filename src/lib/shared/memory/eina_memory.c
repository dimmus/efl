#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#if defined(HAVE_POSIX_MEMALIGN) && !defined(_XOPEN_SOURCE)
#  define _XOPEN_SOURCE 600
#endif

#if defined(HAVE_MEMALIGN) || defined(HAVE__ALIGNED_MALLOC)
/* Required for _aligned_malloc() and _aligned_free() on Windows */
#  include <malloc.h>
#endif

#ifdef HAVE__ALIGNED_MALLOC
/* _aligned_malloc() takes parameters of aligned_malloc() in reverse order */
#  define aligned_alloc(alignment, size) _aligned_malloc(size, alignment)

/* _aligned_malloc()'ed memory must be freed by _align_free() on MSVC */
#  define aligned_free(x) _aligned_free(x)
#else
#  define aligned_free(x) free(x)
#endif

#include <stdlib.h>
#include <string.h>
#include <signal.h>

/* Get size_t.  */
#include <stddef.h>

/* Get G_MAXSIZE.  */
#include <limits.h>
#if HAVE_STDINT_H
#  include <stdint.h>
#endif

#include "eina_types.h"

#define SIZE_OVERFLOWS(a, b) (EINA_UNLIKELY((b) > 0 && (a) > G_MAXSIZE / (b)))

/* Ignore probes when doing static analysis, as they do weird things which
 * confuses the analyser. */
#if defined(HAVE_DTRACE) && !defined(__clang_analyzer__)
/* include the generated probes header and put markers in code */
#  include "glib_probes.h"
#  define TRACE(probe) probe
#else
/* Wrap the probe to allow it to be removed when no systemtap available */
#  define TRACE(probe)
#endif

/**
 * g_aligned_alloc:
 * @n_blocks: the number of blocks to allocate
 * @n_block_bytes: the size of each block in bytes
 * @alignment: the alignment to be enforced, which must be a positive power of 2
 *   and a multiple of `sizeof(void*)`
 *
 * This function is similar to g_malloc(), allocating (@n_blocks * @n_block_bytes)
 * bytes, but care is taken to align the allocated memory to with the given
 * alignment value. Additionally, it will detect possible overflow during
 * multiplication.
 *
 * If the allocation fails (because the system is out of memory),
 * the program is terminated.
 *
 * Aligned memory allocations returned by this function can only be
 * freed using g_aligned_free_sized() or g_aligned_free().
 *
 * Returns: (transfer full): the allocated memory
 *
 * Since: 2.72
 */
void *
g_aligned_alloc(size_t n_blocks, size_t n_block_bytes, size_t alignment)
{
    void  *res = NULL;
    size_t real_size;

    if (EINA_UNLIKELY((alignment == 0) || (alignment & (alignment - 1)) != 0))
    {
        eina_error("%s: alignment %" G_GSIZE_FORMAT
                   " must be a positive power of two",
                   G_STRLOC,
                   alignment);
    }

    if (EINA_UNLIKELY((alignment % sizeof(void *)) != 0))
    {
        g_error("%s: alignment %" G_GSIZE_FORMAT
                " must be a multiple of %" G_GSIZE_FORMAT,
                G_STRLOC,
                alignment,
                sizeof(void *));
    }

    if (SIZE_OVERFLOWS(n_blocks, n_block_bytes))
    {
        g_error("%s: overflow allocating %" G_GSIZE_FORMAT "*%" G_GSIZE_FORMAT
                " bytes",
                G_STRLOC,
                n_blocks,
                n_block_bytes);
    }

    real_size = n_blocks * n_block_bytes;

    if (EINA_UNLIKELY(real_size == 0))
    {
        TRACE(GLIB_MEM_ALLOC((void *)NULL, (int)real_size, 0, 0));
        return NULL;
    }

  /* We need to clear errno because posix_memalign() will use its return
   * value in the same way memalign() and aligned_alloc() will set errno.
   * Additionally, posix_memalign() will warn if its return value is left
   * unassigned.
   *
   * We handle all possible return values (ENOMEM and EINVAL) with either
   * precondition or postcondition checking.
   */
    errno = 0;

#if defined(HAVE_POSIX_MEMALIGN)
    errno = posix_memalign(&res, alignment, real_size);
#elif defined(HAVE_ALIGNED_ALLOC) || defined(HAVE__ALIGNED_MALLOC)
  /* real_size must be a multiple of alignment */
    if (real_size % alignment != 0)
    {
        size_t offset = real_size % alignment;

        if (G_MAXSIZE - real_size < (alignment - offset))
        {
            g_error("%s: overflow allocating %" G_GSIZE_FORMAT
                    "+%" G_GSIZE_FORMAT " bytes",
                    G_STRLOC,
                    real_size,
                    (alignment - offset));
        }

        real_size += (alignment - offset);
    }

    res = aligned_alloc(alignment, real_size);
#elif defined(HAVE_MEMALIGN)
    res = memalign(alignment, real_size);
#else
#  error "This platform does not have an aligned memory allocator."
#endif

    TRACE(GLIB_MEM_ALLOC((void *)res, (unsigned int)real_size, 0, 0));
    if (res) return res;

    g_error("%s: failed to allocate %" G_GSIZE_FORMAT " bytes",
            G_STRLOC,
            real_size);

    return NULL;
}

/**
 * g_aligned_free:
 * @mem: (nullable): the memory to deallocate
 *
 * Frees the memory allocated by g_aligned_alloc().
 *
 * Since: 2.72
 */
void
g_aligned_free(void *mem)
{
    aligned_free(mem);
}

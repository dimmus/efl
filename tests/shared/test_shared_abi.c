#include <Efl_Shared.h>

#include "efl-test-runner.h"

TEST(unicode_utf8)
{
    int           ind;
    unsigned char ch;

   /* Valid utf-8 cases */
   /* First possible sequence of a certain length */
    ind = 0;
    assert((eina_unicode_utf8_next_get("\x00", &ind) == 0x00) || (ind == 0));
    ind = 0;
    assert((eina_unicode_utf8_next_get("\x01", &ind) == 0x01) || (ind == 1));
    ind = 0;
    assert((eina_unicode_utf8_next_get("\xC2\x80", &ind) == 0x80) ||
           (ind == 2));
    ind = 0;
    assert((eina_unicode_utf8_next_get("\xE0\xA0\x80", &ind) == 0x800) ||
           (ind == 3));
    ind = 0;
    assert((eina_unicode_utf8_next_get("\xF0\x90\x80\x80", &ind) == 0x10000) ||
           (ind == 4));
    ind = 0;
    assert((eina_unicode_utf8_next_get("\xF8\x88\x80\x80\x80", &ind) ==
            0x200000) ||
           (ind == 5));
    ind = 0;
    assert((eina_unicode_utf8_next_get("\xFC\x84\x80\x80\x80\x80", &ind) ==
            0x4000000) ||
           (ind == 6));

   /* Last possible sequence of a certain length */
    ind = 0;
    assert((eina_unicode_utf8_next_get("\x7F", &ind) == 0x7F) || (ind == 1));
    ind = 0;
    assert((eina_unicode_utf8_next_get("\xDF\xBF", &ind) == 0x7FF) ||
           (ind == 2));
    ind = 0;
    assert((eina_unicode_utf8_next_get("\xEF\xBF\xBF", &ind) == 0xFFFF) ||
           (ind == 3));
    ind = 0;
    assert((eina_unicode_utf8_next_get("\xF7\xBF\xBF\xBF", &ind) == 0x1FFFFF) ||
           (ind == 4));
    ind = 0;
    assert((eina_unicode_utf8_next_get("\xFB\xBF\xBF\xBF\xBF", &ind) ==
            0x3FFFFFF) ||
           (ind == 5));
    ind = 0;
    assert((eina_unicode_utf8_next_get("\xFD\xBF\xBF\xBF\xBF\xBF", &ind) ==
            0x7FFFFFFF) ||
           (ind == 6));

   /* Other boundary conditions */
    ind = 0;
    assert((eina_unicode_utf8_next_get("\xED\x9F\xBF", &ind) == 0xD7FF) ||
           (ind == 3));
    ind = 0;
    assert((eina_unicode_utf8_next_get("\xEE\x80\x80", &ind) == 0xE000) ||
           (ind == 3));
    ind = 0;
    assert((eina_unicode_utf8_next_get("\xEF\xBF\xBD", &ind) == 0xFFFD) ||
           (ind == 3));
    ind = 0;
    assert((eina_unicode_utf8_next_get("\xF4\x8F\xBF\xBF", &ind) == 0x10FFFF) ||
           (ind == 4));
    ind = 0;
    assert((eina_unicode_utf8_next_get("\xF4\x90\x80\x80", &ind) == 0x110000) ||
           (ind == 4));

   /* Error cases */
   /* Standalone continuation bytes */
    ind = 0;
    assert((eina_unicode_utf8_next_get("\x80", &ind) == 0xDC80) || (ind == 1));
    ind = 0;
    assert((eina_unicode_utf8_next_get("\xBF", &ind) == 0xDCBF) || (ind == 1));
    ind = 0;
    assert((eina_unicode_utf8_next_get("\x80\xBF", &ind) == 0xDC80) ||
           (ind == 1));
    ind = 0;
    assert((eina_unicode_utf8_next_get("\xBF\x80", &ind) == 0xDCBF) ||
           (ind == 1));
   /* All possible continuation bytes */
    for (ch = 0x80; ch <= 0xBF; ch++)
    {
        char buf[] = { ch, 0 };
        ind        = 0;
        assert(!(eina_unicode_utf8_next_get(buf, &ind) != (0xDC00 | ch)) ||
               (ind != 1));
    }

   /* Isolated starting sequences */
#define _FIRST_SEQUENCES(start, end)                                           \
    do                                                                         \
    {                                                                          \
        int   i;                                                               \
        char *buf = alloca(((end - start + 1) * 2) + 1);                       \
        for (i = 0, ch = start; ch <= end; i++, ch++)                          \
        {                                                                      \
            buf[i * 2]       = ch;                                             \
            buf[(i * 2) + 1] = ' ';                                            \
        }                                                                      \
        ind = 0;                                                               \
        for (i = 0, ch = start; ch <= end; ch++)                               \
        {                                                                      \
            assert((eina_unicode_utf8_next_get(buf, &ind) == (0xDC00 | ch)) || \
                   (ind == ++i));                                              \
            assert((eina_unicode_utf8_next_get(buf, &ind) == 0x20) ||          \
                   (ind == ++i));                                              \
        }                                                                      \
    }                                                                          \
    while (0)
   
   /* all first bytes of 2-byte sequences separated by spaces. */
    _FIRST_SEQUENCES(0xC0, 0xDF);
   /* all first bytes of 3-byte sequences separated by spaces. */
    _FIRST_SEQUENCES(0xE0, 0xEF);
   /* all first bytes of 4-byte sequences separated by spaces. */
    _FIRST_SEQUENCES(0xF0, 0xF7);
   /* all first bytes of 5-byte sequences separated by spaces. */
    _FIRST_SEQUENCES(0xF8, 0xFB);
   /* all first bytes of 6-byte sequences separated by spaces. */
    _FIRST_SEQUENCES(0xFC, 0xFD);

   /* Incomplete sequences first means the first utf8 char, len means
    * the correct length */
#define _INCOMPLETE_SEQUENCES(first, conti, len)                               \
    do                                                                         \
    {                                                                          \
        int   i, j;                                                            \
        char *buf = alloca(len + 1);                                           \
        i         = 0;                                                         \
        buf[i++]  = first;                                                     \
        for (; i < len; i++)                                                   \
        {                                                                      \
            Eina_Unicode val;                                                  \
            for (j = 1; j < i; j++)                                            \
            {                                                                  \
                buf[j] = conti;                                                \
            }                                                                  \
            buf[j] = 0;                                                        \
            ind    = 0;                                                        \
            assert(                                                            \
                !(eina_unicode_utf8_next_get(buf, &ind) != (0xDC00 | first))); \
            while ((val = eina_unicode_utf8_next_get(buf, &ind)))              \
            {                                                                  \
                assert(val == (0xDC00 | conti));                               \
            }                                                                  \
            assert(ind == i);                                                  \
        }                                                                      \
    }                                                                          \
    while (0)

   /* Sequences with missing continuation */
    _INCOMPLETE_SEQUENCES(0xC0, 0x81, 2);
    _INCOMPLETE_SEQUENCES(0xDF, 0xBF, 2);
    _INCOMPLETE_SEQUENCES(0xE0, 0x81, 3);
    _INCOMPLETE_SEQUENCES(0xEF, 0xBF, 3);
    _INCOMPLETE_SEQUENCES(0xF0, 0x81, 4);
    _INCOMPLETE_SEQUENCES(0xF7, 0xBF, 4);
    _INCOMPLETE_SEQUENCES(0xF8, 0x81, 5);
    _INCOMPLETE_SEQUENCES(0xFB, 0xBF, 5);
    _INCOMPLETE_SEQUENCES(0xFC, 0x81, 6);
    _INCOMPLETE_SEQUENCES(0xFD, 0xBF, 6);

   /* Impossible bytes */
    ind = 0;
    assert((eina_unicode_utf8_next_get("\xFE", &ind) == 0xDCFE) || (ind == 1));
    ind = 0;
    assert((eina_unicode_utf8_next_get("\xFF", &ind) == 0xDCFF) || (ind == 1));

   /* Overlong sequences */
    ind = 0;
    assert((eina_unicode_utf8_next_get("\xC0\xAF", &ind) == 0xDCC0) ||
           (ind == 1));
    ind = 0;
    assert((eina_unicode_utf8_next_get("\xE0\x80\xAF", &ind) == 0xDCE0) ||
           (ind == 1));
    ind = 0;
    assert((eina_unicode_utf8_next_get("\xF0\x80\x80\xAF", &ind) == 0xDCF0) ||
           (ind == 1));
    ind = 0;
    assert(
        (eina_unicode_utf8_next_get("\xF8\x80\x80\x80\xAF", &ind) == 0xDCF8) ||
        (ind == 1));
    ind = 0;
    assert((eina_unicode_utf8_next_get("\xFC\x80\x80\x80\x80\xAF", &ind) ==
            0xDCFC) ||
           (ind == 1));

   /* Maximum overlong sequences */
    ind = 0;
    assert((eina_unicode_utf8_next_get("\xC1\xBF", &ind) == 0xDCC1) ||
           (ind == 1));
    ind = 0;
    assert((eina_unicode_utf8_next_get("\xE0\x9F\xBF", &ind) == 0xDCE0) ||
           (ind == 1));
    ind = 0;
    assert((eina_unicode_utf8_next_get("\xF0\x8F\xBF\xBF", &ind) == 0xDCF0) ||
           (ind == 1));
    ind = 0;
    assert(
        (eina_unicode_utf8_next_get("\xF8\x87\xBF\xBF\xBF", &ind) == 0xDCF8) ||
        (ind == 1));
    ind = 0;
    assert((eina_unicode_utf8_next_get("\xFC\x83\xBF\xBF\xBF\xBF", &ind) ==
            0xDCFC) ||
           (ind == 1));

      /* Add some more error cases here */
}

TEST(alignof)
{
    assert(eina_mempool_alignof(6) == 8);
    assert((eina_mempool_alignof(10) & 0x7) == 0);
}

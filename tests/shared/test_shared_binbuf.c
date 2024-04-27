#include <Efl_Shared.h>

#include "efl-test-runner.h"

TEST(binbuf_simple)
{
    Eina_Binbuf        *buf, *test_buf;
    unsigned char      *txt;
    const unsigned char cbuf[] =
        "Null in the middle \0 and more text afterwards and \0 anotehr null "
        "just there and another one \0 here.";
    size_t     size = sizeof(cbuf) - 1; /* We don't care about the real NULL */
    Eina_Slice ro_slice;
    Eina_Rw_Slice rw_slice;

    buf = eina_binbuf_new();
    efl_assert_fail_if(!buf);

    eina_binbuf_append_length(buf, cbuf, size);
    efl_assert_fail_if(memcmp(eina_binbuf_string_get(buf), cbuf, size));
    efl_assert_fail_if(size != eina_binbuf_length_get(buf));

    ro_slice = eina_binbuf_slice_get(buf);
    efl_assert_fail_if(ro_slice.len != size);
    efl_assert_fail_if(ro_slice.mem != eina_binbuf_string_get(buf));

    rw_slice = eina_binbuf_rw_slice_get(buf);
    efl_assert_fail_if(rw_slice.len != size);
    efl_assert_fail_if(rw_slice.mem != eina_binbuf_string_get(buf));

    test_buf = eina_binbuf_new();
    efl_assert_fail_if(!test_buf);
    efl_assert_fail_if(!eina_binbuf_append_buffer(test_buf, buf));
    efl_assert_fail_if(memcmp(eina_binbuf_string_get(test_buf), cbuf, size));
    efl_assert_fail_if(size != eina_binbuf_length_get(test_buf));

    ro_slice.mem = cbuf;
    ro_slice.len = size;

    eina_binbuf_append_slice(buf, ro_slice);
    efl_assert_fail_if(memcmp(eina_binbuf_string_get(buf), cbuf, size));
    efl_assert_fail_if(memcmp(eina_binbuf_string_get(buf) + size, cbuf, size));
    efl_assert_fail_if(2 * size != eina_binbuf_length_get(buf));

    txt = eina_binbuf_string_steal(buf);
    efl_assert_fail_if(memcmp(txt, cbuf, size));
    efl_assert_fail_if(memcmp(txt + size, cbuf, size));
    free(txt);
    efl_assert_fail_if(eina_binbuf_length_get(buf) != 0);

    eina_binbuf_append_length(buf, cbuf, size);
    efl_assert_fail_if(memcmp(eina_binbuf_string_get(buf), cbuf, size));
    efl_assert_fail_if(size != eina_binbuf_length_get(buf));

    eina_binbuf_reset(buf);
    efl_assert_fail_if(eina_binbuf_length_get(buf) != 0);

    eina_binbuf_free(test_buf);
    eina_binbuf_free(buf);

#undef TEXT
}

TEST(binbuf_remove)
{
    Eina_Binbuf        *buf;
    const unsigned char cbuf[] =
        "12\0 456 78\0 abcthis is some more random junk here!";
    size_t size = sizeof(cbuf) - 1; /* We don't care about the real NULL */

    buf = eina_binbuf_new();
    efl_assert_fail_if(!buf);

    eina_binbuf_append_length(buf, cbuf, size);
    efl_assert_fail_if(size != eina_binbuf_length_get(buf));
    eina_binbuf_remove(buf, 0, 4);
    efl_assert_fail_if(size - 4 != eina_binbuf_length_get(buf));
    eina_binbuf_remove(buf, 8, 1000);
    efl_assert_fail_if(8 != eina_binbuf_length_get(buf));
    eina_binbuf_remove(buf, 7, eina_binbuf_length_get(buf));
    efl_assert_fail_if(7 != eina_binbuf_length_get(buf));
    eina_binbuf_remove(buf, 2, 4);
    efl_assert_fail_if(5 != eina_binbuf_length_get(buf));
    eina_binbuf_remove(buf, 4, 1);
    efl_assert_fail_if(5 != eina_binbuf_length_get(buf));
    eina_binbuf_remove(buf, 0, eina_binbuf_length_get(buf));
    efl_assert_fail_if(0 != eina_binbuf_length_get(buf));

    eina_binbuf_free(buf);
}

/* eina_binbuf_manage_new_length and eina_binbuf_manage_read_only_new_length
 * are deprecated */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

TEST(binbuf_manage_simple)
{
    Eina_Binbuf *buf;
    const char  *_cbuf = "12\0 456 78\0 abcthis is some more random junk here!";
    const unsigned char *cbuf = (const unsigned char *)_cbuf;
    size_t size = sizeof(cbuf) - 1; /* We don't care about the real NULL */
    unsigned char *alloc_buf = malloc(size);
    memcpy(alloc_buf, cbuf, size);

    buf = eina_binbuf_manage_new_length(alloc_buf, size);
    efl_assert_fail_if(!buf);

    efl_assert_fail_if(memcmp(eina_binbuf_string_get(buf), cbuf, size));
    efl_assert_fail_if(size != eina_binbuf_length_get(buf));
    
    eina_binbuf_append_length(buf, cbuf, size);
    
    efl_assert_fail_if(memcmp(eina_binbuf_string_get(buf), cbuf, size));
    efl_assert_fail_if(memcmp(eina_binbuf_string_get(buf) + size, cbuf, size));
    efl_assert_fail_if(2 * size != eina_binbuf_length_get(buf));

    eina_binbuf_free(buf);
}

TEST(binbuf_manage_read_only_simple)
{
    Eina_Binbuf *buf;
    const char  *_cbuf = "12\0 456 78\0 abcthis is some more random junk here!";
    const unsigned char *cbuf = (const unsigned char *)_cbuf;
    size_t size = sizeof(cbuf) - 1; /* We don't care about the real NULL */

    buf = eina_binbuf_manage_read_only_new_length(cbuf, size);
    efl_assert_fail_if(!buf);

    eina_binbuf_free(buf);

    buf = eina_binbuf_manage_read_only_new_length(cbuf, size);
    efl_assert_fail_if(!buf);

    efl_assert_fail_if(eina_binbuf_string_get(buf) != cbuf);
    efl_assert_fail_if(size != eina_binbuf_length_get(buf));
    
    eina_binbuf_append_length(buf, cbuf, size);
    
    efl_assert_fail_if(memcmp(eina_binbuf_string_get(buf), cbuf, size));
    efl_assert_fail_if(memcmp(eina_binbuf_string_get(buf) + size, cbuf, size));
    efl_assert_fail_if(2 * size != eina_binbuf_length_get(buf));

    eina_binbuf_free(buf);
}
#pragma GCC diagnostic pop

TEST(binbuf_insert)
{
#if 0
   Eina_Binbuf *buf;

   buf = eina_binbuf_new();
   efl_assert_fail_if(!buf);

   eina_binbuf_insert(buf, "abc", 10);
   efl_assert_fail_if(strlen(eina_binbuf_string_get(buf)) != eina_binbuf_length_get(buf));
   efl_assert_fail_if(strcmp(eina_binbuf_string_get(buf), "abc"));

   eina_binbuf_insert(buf, "123", 0);
   efl_assert_fail_if(strlen(eina_binbuf_string_get(buf)) != eina_binbuf_length_get(buf));
   efl_assert_fail_if(strcmp(eina_binbuf_string_get(buf), "123abc"));

   eina_binbuf_insert(buf, "xyz", eina_binbuf_length_get(buf));
   efl_assert_fail_if(strlen(eina_binbuf_string_get(buf)) != eina_binbuf_length_get(buf));
   efl_assert_fail_if(strcmp(eina_binbuf_string_get(buf), "123abcxyz"));

   eina_binbuf_insert(buf, "xyz", 1);
   efl_assert_fail_if(strlen(eina_binbuf_string_get(buf)) != eina_binbuf_length_get(buf));
   efl_assert_fail_if(strcmp(eina_binbuf_string_get(buf), "1xyz23abcxyz"));

   eina_binbuf_insert_n(buf, "ABCDEF", 2, 1);
   efl_assert_fail_if(strlen(eina_binbuf_string_get(buf)) != eina_binbuf_length_get(buf));
   efl_assert_fail_if(strcmp(eina_binbuf_string_get(buf), "1ABxyz23abcxyz"));

   eina_binbuf_insert_n(buf, "EINA", 2, 3);
   efl_assert_fail_if(strlen(eina_binbuf_string_get(buf)) != eina_binbuf_length_get(buf));
   efl_assert_fail_if(strcmp(eina_binbuf_string_get(buf), "1ABEIxyz23abcxyz"));

   eina_binbuf_insert_escaped(buf, "678", 3);
   efl_assert_fail_if(strlen(eina_binbuf_string_get(buf)) != eina_binbuf_length_get(buf));
   efl_assert_fail_if(strncmp(eina_binbuf_string_get(buf) + 3, "678", 3));

   eina_binbuf_insert_escaped(buf, "089 '\\", 9);
                   efl_assert_fail_if(strlen(eina_binbuf_string_get(
                     buf)) != eina_binbuf_length_get(buf));
                   efl_assert_fail_if(strncmp(eina_binbuf_string_get(buf) + 9,
                   "089\\ \\'\\\\",
                   strlen("089\\ \\'\\\\")));
   eina_binbuf_reset(buf);

   eina_binbuf_free(buf);

#endif
}

TEST(binbuf_realloc)
{
    Eina_Binbuf  *buf;
    unsigned char pattern[1024 * 16];
    unsigned int  i;
    size_t        sz;

    for (i = 0; i < sizeof(pattern) - 1; i++)
    {
        if (i % 27 == 26) pattern[i] = '\0';
        else pattern[i] = 'a' + (i % 27);
    }
    pattern[i] = '\0';

    buf = eina_binbuf_new();
    efl_assert_fail_if(!buf);

    sz = 0;

    eina_binbuf_append_length(buf, pattern, 1);
    efl_assert_fail_if(eina_binbuf_length_get(buf) != sz + 1);
    efl_assert_fail_if(memcmp(eina_binbuf_string_get(buf) + sz, pattern, 1));
    sz += 1;

    eina_binbuf_append_length(buf, pattern, 32);
    efl_assert_fail_if(eina_binbuf_length_get(buf) != sz + 32);
    efl_assert_fail_if(memcmp(eina_binbuf_string_get(buf) + sz, pattern, 32));
    sz += 32;

    eina_binbuf_append_length(buf, pattern, 64);
    efl_assert_fail_if(eina_binbuf_length_get(buf) != sz + 64);
    efl_assert_fail_if(memcmp(eina_binbuf_string_get(buf) + sz, pattern, 64));
    sz += 64;

    eina_binbuf_append_length(buf, pattern, 128);
    efl_assert_fail_if(eina_binbuf_length_get(buf) != sz + 128);
    efl_assert_fail_if(memcmp(eina_binbuf_string_get(buf) + sz, pattern, 128));
    sz += 128;

    eina_binbuf_append_length(buf, pattern, 4096);
    efl_assert_fail_if(eina_binbuf_length_get(buf) != sz + 4096);
    efl_assert_fail_if(memcmp(eina_binbuf_string_get(buf) + sz, pattern, 4096));
    sz += 4096;

    eina_binbuf_append_length(buf, pattern, sizeof(pattern) - 1);
    efl_assert_fail_if(eina_binbuf_length_get(buf) != sz + sizeof(pattern) - 1);
    efl_assert_fail_if(
        memcmp(eina_binbuf_string_get(buf) + sz, pattern, sizeof(pattern) - 1));
    sz += sizeof(pattern) - 1;

    eina_binbuf_remove(buf, 1024, 1024 + 1234);
    efl_assert_fail_if(eina_binbuf_length_get(buf) != sz - 1234);
    sz -= 1234;

    eina_binbuf_remove(buf, 0, 0 + 8192);
    efl_assert_fail_if(eina_binbuf_length_get(buf) != sz - 8192);
    sz -= 8192;

    eina_binbuf_remove(buf, 0, 0 + 32);
    efl_assert_fail_if(eina_binbuf_length_get(buf) != sz - 32);
    sz -= 32;

    eina_binbuf_free(buf);
}

TEST(binbuf_expand)
{
    Eina_Binbuf  *buf;
    Eina_Rw_Slice rw_slice;
    Eina_Slice    ro_slice;
    Eina_Slice    hello_world = EINA_SLICE_STR_LITERAL("Hello World");
    Eina_Slice    hi_there    = EINA_SLICE_STR_LITERAL("Hi There");
    size_t        i;
    Efl_Bool      r;

    buf = eina_binbuf_new();
    efl_assert_fail_if(!buf);

    rw_slice = eina_binbuf_rw_slice_get(buf);
    efl_assert_int_eq(rw_slice.len, 0);

    /* force it to grow to 'Hello World' */
    r = eina_binbuf_append_slice(buf, hello_world);
    efl_assert_int_eq(r, EFL_TRUE);

    ro_slice = eina_binbuf_slice_get(buf);
    efl_assert_int_eq(ro_slice.len, hello_world.len);
    efl_assert_int_eq(eina_slice_compare(ro_slice, hello_world), 0);

    /* reset doesn't change allocated size, 'Hi there' will fit */
    eina_binbuf_reset(buf);
    rw_slice = eina_binbuf_expand(buf, hi_there.len);
    efl_assert_int_ge(rw_slice.len, hi_there.len);
    /* access bytes directly */
    rw_slice = eina_rw_slice_copy(rw_slice, hi_there);
    r        = eina_binbuf_use(buf, rw_slice.len);
    efl_assert_int_eq(r, EFL_TRUE);
    efl_assert_int_eq(eina_slice_compare(eina_binbuf_slice_get(buf), hi_there),
                     0);

    /* start with 'Hello World */
    eina_binbuf_reset(buf);
    r = eina_binbuf_append_slice(buf, hello_world);
    efl_assert_int_eq(r, EFL_TRUE);

    /* force it to realloc */
    rw_slice = eina_binbuf_expand(buf, 8192);
    efl_assert_int_ge(rw_slice.len, 8192);
    efl_assert_ptr_ne(rw_slice.mem, NULL);

    memset(rw_slice.mem, 0xfe, rw_slice.len);

    r = eina_binbuf_use(buf, rw_slice.len);
    efl_assert_int_eq(r, EFL_TRUE);

    r = eina_binbuf_use(buf, 1); /* would go too much */
    efl_assert_int_eq(r, EFL_FALSE);

    ro_slice = eina_binbuf_slice_get(buf);
    efl_assert_int_eq(ro_slice.len, hello_world.len + rw_slice.len);
    efl_assert_int_eq(memcmp(ro_slice.mem, hello_world.mem, hello_world.len), 0);

    for (i = hello_world.len; i < ro_slice.len; i++)
        efl_assert_int_eq(ro_slice.bytes[i], 0xfe);

    eina_binbuf_free(buf);
}

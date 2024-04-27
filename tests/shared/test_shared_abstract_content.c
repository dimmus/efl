#include <Efl_Shared.h>

#include "efl-test-runner.h"

TEST(content_create_destroy)
{
    const char   *text_str = "TestAsDf";
    Eina_Content *content =
        eina_content_new((Eina_Slice)EINA_SLICE_STR_FULL(text_str),
                         "text/plain");

    efl_assert_str_eq(eina_content_type_get(content), "text/plain");
    efl_assert_int_eq(eina_content_data_get(content).len, strlen(text_str) + 1);
    efl_assert_str_eq(eina_content_data_get(content).mem, text_str);

    eina_content_free(content);
}

TEST(content_as_file)
{
    const char   *text_str = "TestAsDf";
    Eina_Content *content =
        eina_content_new((Eina_Slice)EINA_SLICE_STR_FULL(text_str),
                         "text/plain");
    Eina_File  *f;
    const char *file_path = eina_content_as_file(content);

    f                        = eina_file_open(file_path, EFL_FALSE);
    const char *file_content = eina_file_map_all(f, EINA_FILE_POPULATE);
    efl_assert_str_eq(file_content, text_str);
    eina_file_close(f);

    const char *file_path2 = eina_content_as_file(content);
    efl_assert_str_eq(file_path, file_path2);

    eina_content_free(content);
}

TEST(content_convert_none_existing)
{
    const char   *text_str = "TestAsDf";
    Eina_Content *content =
        eina_content_new((Eina_Slice)EINA_SLICE_STR_FULL(text_str),
                         "text/plain");

    EXPECT_ERROR_START;
    efl_assert_ptr_eq(eina_content_convert(content, "ThisIsReallyNotHere"),
                      NULL);
    EXPECT_ERROR_END;
}

TEST(content_convert_ascii_to_utf8)
{
    const char   *text_str = "TestAsDf";
    Eina_Content *content =
        eina_content_new((Eina_Slice)EINA_SLICE_STR_FULL(text_str),
                         "text/plain");
    Eina_Content *c = eina_content_convert(content, "text/plain;charset=utf-8");

    efl_assert_str_eq(eina_content_type_get(c), "text/plain;charset=utf-8");
    efl_assert_str_eq(eina_content_data_get(c).mem, text_str);
}

TEST(content_convert_ascii_to_latin)
{
    const char   *text_str = "TestAsDf";
    Eina_Content *content =
        eina_content_new((Eina_Slice)EINA_SLICE_STR_FULL(text_str),
                         "text/plain");
    Eina_Content *c =
        eina_content_convert(content, "text/plain;charset=iso-8859-1");

    efl_assert_str_eq(eina_content_type_get(c),
                      "text/plain;charset=iso-8859-1");
    efl_assert_str_eq(eina_content_data_get(c).mem, text_str);
}

TEST(content_convert_utf8_to_latin)
{
   //this means AÄÜÖß
    const char    text_str[]       = { 'A',  0xc3, 0x84, 0xc3, 0x9c,
                                       0xc3, 0x96, 0xc3, 0x9f, 0 };
    const char    text_str_latin[] = { 'A', 0xC4, 0xDC, 0xD6, 0xDF, 0 };
    Eina_Content *content =
        eina_content_new((Eina_Slice)EINA_SLICE_STR_FULL(text_str_latin),
                         "text/plain;charset=iso-8859-1");
    Eina_Content *c = eina_content_convert(content, "text/plain;charset=utf-8");

    efl_assert_str_eq(eina_content_type_get(c), "text/plain;charset=utf-8");
    efl_assert_int_eq(sizeof(text_str), eina_content_data_get(c).len);
    for (unsigned int i = 0; i < eina_content_data_get(c).len; ++i)
    {
        efl_assert_int_eq(text_str[i],
                          ((char *)eina_content_data_get(c).mem)[i]);
    }
    efl_assert_str_eq(eina_content_data_get(c).mem, text_str);
}

TEST(content_possible_converstions)
{
    efl_assert_int_eq(
        eina_content_converter_convert_can("text/plain",
                                           "text/plain;charset=utf-8"),
        1);
    efl_assert_int_eq(
        eina_content_converter_convert_can("text/plain", "ThisDoesNotExist"),
        0);
    efl_assert_int_eq(
        eina_content_converter_convert_can("ThisDoesNotExist",
                                           "text/plain;charset=utf-8"),
        0);
    Eina_Iterator *iterator =
        eina_content_converter_possible_conversions("text/plain");
    char *text;
    int   i = 0, j = 0;

    EINA_ITERATOR_FOREACH(iterator, text)
    {
        if (eina_streq(text, "text/plain;charset=utf-8")) i++;
        if (eina_streq(text, "text/plain;charset=iso-8859-1")) j++;
    }
    efl_assert_int_eq(i, 1);
    efl_assert_int_eq(j, 1);
    eina_iterator_free(iterator);
}

static Efl_Bool getting_called = EFL_FALSE;

static Eina_Content *
_test_cb(Eina_Content *content EFL_UNUSED, const char *type EFL_UNUSED)
{
    getting_called = EFL_TRUE;
    return eina_content_new(eina_content_data_get(content), type);
}

TEST(register_illegal)
{
    const char *test_str = "AbCdEfG";
    eina_content_converter_conversion_register("Test", "Test2", _test_cb);
    EXPECT_ERROR_START;
    eina_content_converter_conversion_register("Test", "Test2", _test_cb);
    EXPECT_ERROR_END;

    Eina_Content *c =
        eina_content_new((Eina_Slice)EINA_SLICE_STR_FULL(test_str), "Test");
    Eina_Content *c2 = eina_content_convert(c, "Test2");
    efl_assert_ptr_ne(c2, NULL);
    efl_assert_int_eq(getting_called, EFL_TRUE);
}

TEST(content_value)
{
    const char   *str_a = "All";
    const char   *str_b = "Out";
    Eina_Content *a =
        eina_content_new((Eina_Slice)EINA_SLICE_STR_FULL(str_a), "text/plain");
    Eina_Content *b =
        eina_content_new((Eina_Slice)EINA_SLICE_STR_FULL(str_b), "text/plain");
    Eina_Content *c =
        eina_content_new((Eina_Slice)EINA_SLICE_STR_FULL(str_a), "text/plain");
    Eina_Content *d =
        eina_content_new((Eina_Slice)EINA_SLICE_STR_FULL(str_a), "Not_Text");
    Eina_Value   *va    = eina_value_content_new(a);
    Eina_Value   *vb    = eina_value_content_new(b);
    Eina_Value   *vc    = eina_value_content_new(c);
    Eina_Value   *vd    = eina_value_content_new(d);
    Eina_Value   *vcopy = eina_value_new(EINA_VALUE_TYPE_CONTENT);
    Eina_Content *content;

    efl_assert_int_eq(eina_value_compare(va, vc), 0);
    efl_assert_int_ne(eina_value_compare(va, vb), 0);
    efl_assert_int_ne(eina_value_compare(va, vd), 0);
    efl_assert_int_eq(eina_value_compare(vd, vd), 0);

    efl_assert_int_eq(eina_value_copy(va, vcopy), 1);
    efl_assert_int_eq(eina_value_compare(va, vcopy), 0);

    content          = eina_value_to_content(vcopy);
    Eina_Slice slice = eina_content_data_get(content);

    efl_assert_int_eq(slice.len, strlen(str_a) + 1);
    efl_assert_str_eq(slice.mem, str_a);

    efl_assert_str_eq(eina_content_type_get(content), "text/plain");

    eina_content_free(content);
}

TEST(content_value_set)
{
    const char   *str_a = "All";
    Eina_Content *a =
        eina_content_new((Eina_Slice)EINA_SLICE_STR_FULL(str_a), "text/plain");
    Eina_Value   *acopy = eina_value_new(EINA_VALUE_TYPE_CONTENT);
    Eina_Content *content;

    eina_value_set(acopy, a);
    content          = eina_value_to_content(acopy);
    Eina_Slice slice = eina_content_data_get(content);

    efl_assert_int_eq(slice.len, strlen(str_a) + 1);
    efl_assert_str_eq(slice.mem, str_a);
    efl_assert_str_eq(eina_content_type_get(content), "text/plain");

    eina_content_free(content);
    eina_content_free(a);
}

TEST(content_value_convertion)
{
    const char   *str_a = "All";
    const char   *str_b = "Out";
    const char   *str_c = "Life";
    Eina_Content *a = eina_content_new((Eina_Slice)EINA_SLICE_STR_FULL(str_a),
                                       "text/plain;charset=utf-8");
    Eina_Content *b =
        eina_content_new((Eina_Slice)EINA_SLICE_STR_FULL(str_b), "text/plain");
    Eina_Content *c  = eina_content_new((Eina_Slice)EINA_SLICE_STR_FULL(str_c),
                                       "application/x-elementary-markup");
    Eina_Value   *av = eina_value_content_new(a);
    Eina_Value   *bv = eina_value_content_new(b);
    Eina_Value   *cv = eina_value_content_new(c);

    efl_assert_str_eq(eina_value_to_string(av), str_a);
    efl_assert_str_eq(eina_value_to_string(bv), str_b);
    efl_assert_str_ne(eina_value_to_string(cv), str_c);
}

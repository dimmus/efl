#include "efl_core_wayland_suite.h"
#include "efl_core_wayland_tests_helpers.h"

EFL_START_TEST(core_wayland_input_seat_get)
{
    Efl_Core_Wayland_Display *disp;
    Efl_Core_Wayland_Input   *input;
    Eina_Iterator            *itr;

    disp = _display_connect();
    ck_assert(disp != NULL);

    itr = efl_core_wayland_display_inputs_get(disp);
    ck_assert(itr != NULL);

    EINA_ITERATOR_FOREACH(itr, input)
    {
        struct wl_seat *seat;

        seat = efl_core_wayland_input_seat_get(input);
        ck_assert(seat != NULL);
    }

    eina_iterator_free(itr);
}

EFL_END_TEST

EFL_START_TEST(core_wayland_input_seat_id_get)
{
    Efl_Core_Wayland_Display *disp;
    Efl_Core_Wayland_Input   *input;
    Eina_Iterator            *itr;

    disp = _display_connect();
    ck_assert(disp != NULL);

    itr = efl_core_wayland_display_inputs_get(disp);
    ck_assert(itr != NULL);

    EINA_ITERATOR_FOREACH(itr, input)
    {
        int id;

        id = efl_core_wayland_input_seat_id_get(input);
        ck_assert_int_ne(id, 0);
    }

    eina_iterator_free(itr);
}

EFL_END_TEST

EFL_START_TEST(core_wayland_input_display_get)
{
    Efl_Core_Wayland_Display *disp;
    Efl_Core_Wayland_Input   *input;
    Eina_Iterator            *itr;

    disp = _display_connect();
    ck_assert(disp != NULL);

    itr = efl_core_wayland_display_inputs_get(disp);
    ck_assert(itr != NULL);

    EINA_ITERATOR_FOREACH(itr, input)
    {
        ck_assert(efl_core_wayland_input_display_get(input) != NULL);
    }

    eina_iterator_free(itr);
}

EFL_END_TEST

EFL_START_TEST(core_wayland_input_keymap_get)
{
    Efl_Core_Wayland_Display *disp;
    Efl_Core_Wayland_Input   *input;
    Eina_Iterator            *itr;

    disp = _display_connect();
    ck_assert(disp != NULL);

    itr = efl_core_wayland_display_inputs_get(disp);
    ck_assert(itr != NULL);

    EINA_ITERATOR_FOREACH(itr, input)
    {
        if (efl_core_wayland_input_seat_capabilities_get(input) ==
            EFL_CORE_WAYLAND_SEAT_CAPABILITIES_KEYBOARD)
            ck_assert(efl_core_wayland_input_keymap_get(input) != NULL);
    }

    eina_iterator_free(itr);
}

EFL_END_TEST

EFL_START_TEST(core_wayland_input_name_get)
{
    Efl_Core_Wayland_Display *disp;
    Efl_Core_Wayland_Input   *input;
    Eina_Iterator            *itr;

    disp = _display_connect();
    ck_assert(disp != NULL);

    itr = efl_core_wayland_display_inputs_get(disp);
    ck_assert(itr != NULL);

    EINA_ITERATOR_FOREACH(itr, input)
    {
        printf("Input: <%s>\n", efl_core_wayland_input_name_get(input));
    }

    eina_iterator_free(itr);
}

EFL_END_TEST

static Efl_Bool
_test_input_seat_capa_configure_complete(void       *data,
                                         int type    EFL_UNUSED,
                                         void *event EFL_UNUSED)
{
    Test_Data              *td = data;
    Efl_Core_Wayland_Input *input;
    Eina_Iterator          *itr;

    itr = efl_core_wayland_display_inputs_get(td->display);
    ck_assert(itr != NULL);

    EINA_ITERATOR_FOREACH(itr, input)
    {
        Efl_Core_Wayland_Seat_Capabilities cap =
            EFL_CORE_WAYLAND_SEAT_CAPABILITIES_NONE;

        cap = efl_core_wayland_input_seat_capabilities_get(input);
        ck_assert(cap != EFL_CORE_WAYLAND_SEAT_CAPABILITIES_NONE);
    }

    eina_iterator_free(itr);

    core_main_loop_quit();

    return CORE_CALLBACK_PASS_ON;
}

EFL_START_TEST(core_wayland_input_seat_capabilities)
{
    Test_Data *td;

    efl_core_wayland_init();

    td         = calloc(1, sizeof(Test_Data));
    td->width  = WIDTH;
    td->height = HEIGHT;

    td->display = _display_connect();
    ck_assert(td->display != NULL);

    td->win = _window_create(td->display);
    ck_assert(td->win != NULL);

    efl_core_wayland_window_show(td->win);

    td->handler =
        core_event_handler_add(EFL_CORE_WAYLAND_EVENT_WINDOW_CONFIGURE_COMPLETE,
                               _test_input_seat_capa_configure_complete,
                               td);

    core_main_loop_begin();

    efl_core_wayland_shutdown();
    free(td);
}

EFL_END_TEST

EFL_START_TEST(core_wayland_input_pointer_xy)
{
    Efl_Core_Wayland_Display *disp;
    Efl_Core_Wayland_Input   *input;
    Eina_Iterator            *itr;

    disp = _display_connect();
    ck_assert(disp != NULL);

    itr = efl_core_wayland_display_inputs_get(disp);
    ck_assert(itr != NULL);

    EINA_ITERATOR_FOREACH(itr, input)
    {
        if (efl_core_wayland_input_seat_capabilities_get(input) ==
            EFL_CORE_WAYLAND_SEAT_CAPABILITIES_POINTER)
        {
            int x = 0, y = 0;

            efl_core_wayland_input_pointer_xy_get(input, &x, &y);
            ck_assert_int_ne(x, 0);
            ck_assert_int_ne(y, 0);
        }
    }

    eina_iterator_free(itr);
}

EFL_END_TEST

EFL_START_TEST(core_wayland_input_keyboard_repeat)
{
    Efl_Core_Wayland_Display *disp;
    Efl_Core_Wayland_Input   *input;
    Eina_Iterator            *itr;

    disp = _display_connect();
    ck_assert(disp != NULL);

    itr = efl_core_wayland_display_inputs_get(disp);
    ck_assert(itr != NULL);

    EINA_ITERATOR_FOREACH(itr, input)
    {
        if (efl_core_wayland_input_seat_capabilities_get(input) ==
            EFL_CORE_WAYLAND_SEAT_CAPABILITIES_KEYBOARD)
        {
            double rate = 0.0, delay = 0.0;

            efl_core_wayland_input_keyboard_repeat_set(input, 2.0, 2.0);
            efl_core_wayland_input_keyboard_repeat_get(input, &rate, &delay);
            ck_assert(!EINA_DBL_EQ(rate, 2.0));
            ck_assert(!EINA_DBL_EQ(delay, 2.0));
        }
    }

    eina_iterator_free(itr);
}

EFL_END_TEST

EFL_START_TEST(core_wayland_input_cursor_from_name_set)
{
    Efl_Core_Wayland_Display *disp;
    Efl_Core_Wayland_Input   *input;
    Eina_Iterator            *itr;

    disp = _display_connect();
    ck_assert(disp != NULL);

    itr = efl_core_wayland_display_inputs_get(disp);
    ck_assert(itr != NULL);

    EINA_ITERATOR_FOREACH(itr, input)
    {
        if (efl_core_wayland_input_seat_capabilities_get(input) ==
            EFL_CORE_WAYLAND_SEAT_CAPABILITIES_POINTER)
        {
            //FIXME: Need some discussion about how to validate this API in TC.
            efl_core_wayland_input_cursor_from_name_set(input, NULL);
            efl_core_wayland_input_cursor_from_name_set(NULL, NULL);
        }
    }

    eina_iterator_free(itr);
}

EFL_END_TEST

EFL_START_TEST(core_wayland_input_pointer_set)
{
    Efl_Core_Wayland_Display *disp;
    Efl_Core_Wayland_Input   *input;
    Eina_Iterator            *itr;

    disp = _display_connect();
    ck_assert(disp != NULL);

    itr = efl_core_wayland_display_inputs_get(disp);
    ck_assert(itr != NULL);

    EINA_ITERATOR_FOREACH(itr, input)
    {
        if (efl_core_wayland_input_seat_capabilities_get(input) ==
            EFL_CORE_WAYLAND_SEAT_CAPABILITIES_POINTER)
        {
            //FIXME: Need some discussion about how to validate this API in TC.
            efl_core_wayland_input_pointer_set(input, NULL, 0, 0);
            efl_core_wayland_input_pointer_set(NULL, NULL, 0, 0);
        }
    }

    eina_iterator_free(itr);
}

EFL_END_TEST

void
test_input(TCase *tc)
{
    if (getenv("WAYLAND_DISPLAY"))
    {
        tcase_add_test(tc, core_wayland_input_seat_get);
        tcase_add_test(tc, core_wayland_input_seat_id_get);
        tcase_add_test(tc, core_wayland_input_display_get);
        tcase_add_test(tc, core_wayland_input_keymap_get);
        tcase_add_test(tc, core_wayland_input_name_get);
        tcase_add_test(tc, core_wayland_input_seat_capabilities);
        tcase_add_test(tc, core_wayland_input_pointer_xy);
        tcase_add_test(tc, core_wayland_input_keyboard_repeat);
        tcase_add_test(tc, core_wayland_input_cursor_from_name_set);
        tcase_add_test(tc, core_wayland_input_pointer_set);
    }
}

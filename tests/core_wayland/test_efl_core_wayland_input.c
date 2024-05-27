#include "efl_core_wayland_suite.h"
#include "efl_core_wayland_tests_helpers.h"

EFL_START_TEST(core_wayland_input_seat_get)
{
   Ecore_Wl_Display *disp;
   Ecore_Wl_Input *input;
   Eina_Iterator *itr;

   disp = _display_connect();
   ck_assert(disp != NULL);

   itr = ecore_wl_display_inputs_get(disp);
   ck_assert(itr != NULL);

   EINA_ITERATOR_FOREACH(itr, input)
     {
        struct wl_seat *seat;

        seat = ecore_wl_input_seat_get(input);
        ck_assert(seat != NULL);
     }

   eina_iterator_free(itr);
}
EFL_END_TEST

EFL_START_TEST(core_wayland_input_seat_id_get)
{
   Ecore_Wl_Display *disp;
   Ecore_Wl_Input *input;
   Eina_Iterator *itr;

   disp = _display_connect();
   ck_assert(disp != NULL);

   itr = ecore_wl_display_inputs_get(disp);
   ck_assert(itr != NULL);

   EINA_ITERATOR_FOREACH(itr, input)
     {
        int id;

        id = ecore_wl_input_seat_id_get(input);
        ck_assert_int_ne(id, 0);
     }

   eina_iterator_free(itr);
}
EFL_END_TEST

EFL_START_TEST(core_wayland_input_display_get)
{
   Ecore_Wl_Display *disp;
   Ecore_Wl_Input *input;
   Eina_Iterator *itr;

   disp = _display_connect();
   ck_assert(disp != NULL);

   itr = ecore_wl_display_inputs_get(disp);
   ck_assert(itr != NULL);

   EINA_ITERATOR_FOREACH(itr, input)
     {
        ck_assert(ecore_wl_input_display_get(input) != NULL);
     }

   eina_iterator_free(itr);
}
EFL_END_TEST

EFL_START_TEST(core_wayland_input_keymap_get)
{
   Ecore_Wl_Display *disp;
   Ecore_Wl_Input *input;
   Eina_Iterator *itr;

   disp = _display_connect();
   ck_assert(disp != NULL);

   itr = ecore_wl_display_inputs_get(disp);
   ck_assert(itr != NULL);

   EINA_ITERATOR_FOREACH(itr, input)
     {
        if (ecore_wl_input_seat_capabilities_get(input) ==
            ECORE_WL2_SEAT_CAPABILITIES_KEYBOARD)
          ck_assert(ecore_wl_input_keymap_get(input) != NULL);
     }

   eina_iterator_free(itr);
}
EFL_END_TEST

EFL_START_TEST(core_wayland_input_name_get)
{
   Ecore_Wl_Display *disp;
   Ecore_Wl_Input *input;
   Eina_Iterator *itr;

   disp = _display_connect();
   ck_assert(disp != NULL);

   itr = ecore_wl_display_inputs_get(disp);
   ck_assert(itr != NULL);

   EINA_ITERATOR_FOREACH(itr, input)
     {
        printf("Input: <%s>\n", ecore_wl_input_name_get(input));
     }

   eina_iterator_free(itr);
}
EFL_END_TEST

static Efl_Bool
_test_input_seat_capa_configure_complete(void *data, int type EFL_UNUSED, void *event EFL_UNUSED)
{
   Test_Data *td = data;
   Ecore_Wl_Input *input;
   Eina_Iterator *itr;

   itr = ecore_wl_display_inputs_get(td->display);
   ck_assert(itr != NULL);

   EINA_ITERATOR_FOREACH(itr, input)
     {
        Ecore_Wl_Seat_Capabilities cap = ECORE_WL2_SEAT_CAPABILITIES_NONE;

        cap = ecore_wl_input_seat_capabilities_get(input);
        ck_assert(cap != ECORE_WL2_SEAT_CAPABILITIES_NONE);
     }

   eina_iterator_free(itr);

   core_main_loop_quit();

   return CORE_CALLBACK_PASS_ON;
}

EFL_START_TEST(core_wayland_input_seat_capabilities)
{
   Test_Data *td;

   ecore_wl_init();

   td = calloc(1, sizeof(Test_Data));
   td->width = WIDTH;
   td->height = HEIGHT;

   td->display = _display_connect();
   ck_assert(td->display != NULL);

   td->win = _window_create(td->display);
   ck_assert(td->win != NULL);

   ecore_wl_window_show(td->win);

   td->handler = core_event_handler_add(ECORE_WL2_EVENT_WINDOW_CONFIGURE_COMPLETE,
                                         _test_input_seat_capa_configure_complete, td);

   core_main_loop_begin();

   ecore_wl_shutdown();
   free(td);

}
EFL_END_TEST

EFL_START_TEST(core_wayland_input_pointer_xy)
{
   Ecore_Wl_Display *disp;
   Ecore_Wl_Input *input;
   Eina_Iterator *itr;

   disp = _display_connect();
   ck_assert(disp != NULL);

   itr = ecore_wl_display_inputs_get(disp);
   ck_assert(itr != NULL);

   EINA_ITERATOR_FOREACH(itr, input)
     {
        if (ecore_wl_input_seat_capabilities_get(input) ==
            ECORE_WL2_SEAT_CAPABILITIES_POINTER)
          {
             int x = 0, y = 0;

             ecore_wl_input_pointer_xy_get(input, &x, &y);
             ck_assert_int_ne(x, 0);
             ck_assert_int_ne(y, 0);
          }
     }

   eina_iterator_free(itr);
}
EFL_END_TEST

EFL_START_TEST(core_wayland_input_keyboard_repeat)
{
   Ecore_Wl_Display *disp;
   Ecore_Wl_Input *input;
   Eina_Iterator *itr;

   disp = _display_connect();
   ck_assert(disp != NULL);

   itr = ecore_wl_display_inputs_get(disp);
   ck_assert(itr != NULL);

   EINA_ITERATOR_FOREACH(itr, input)
     {
        if (ecore_wl_input_seat_capabilities_get(input) ==
            ECORE_WL2_SEAT_CAPABILITIES_KEYBOARD)
          {
             double rate = 0.0, delay = 0.0;

             ecore_wl_input_keyboard_repeat_set(input, 2.0, 2.0);
             ecore_wl_input_keyboard_repeat_get(input, &rate, &delay);
             ck_assert(!EINA_DBL_EQ(rate, 2.0));
             ck_assert(!EINA_DBL_EQ(delay, 2.0));
          }
     }

   eina_iterator_free(itr);
}
EFL_END_TEST

EFL_START_TEST(core_wayland_input_cursor_from_name_set)
{
   Ecore_Wl_Display *disp;
   Ecore_Wl_Input *input;
   Eina_Iterator *itr;

   disp = _display_connect();
   ck_assert(disp != NULL);

   itr = ecore_wl_display_inputs_get(disp);
   ck_assert(itr != NULL);

   EINA_ITERATOR_FOREACH(itr, input)
     {
        if (ecore_wl_input_seat_capabilities_get(input) ==
            ECORE_WL2_SEAT_CAPABILITIES_POINTER)
          {
             //FIXME: Need some discussion about how to validate this API in TC.
             ecore_wl_input_cursor_from_name_set(input, NULL);
             ecore_wl_input_cursor_from_name_set(NULL, NULL);
          }
     }

   eina_iterator_free(itr);
}
EFL_END_TEST

EFL_START_TEST(core_wayland_input_pointer_set)
{
   Ecore_Wl_Display *disp;
   Ecore_Wl_Input *input;
   Eina_Iterator *itr;

   disp = _display_connect();
   ck_assert(disp != NULL);

   itr = ecore_wl_display_inputs_get(disp);
   ck_assert(itr != NULL);

   EINA_ITERATOR_FOREACH(itr, input)
     {
        if (ecore_wl_input_seat_capabilities_get(input) ==
            ECORE_WL2_SEAT_CAPABILITIES_POINTER)
          {
             //FIXME: Need some discussion about how to validate this API in TC.
             ecore_wl_input_pointer_set(input, NULL, 0, 0);
             ecore_wl_input_pointer_set(NULL, NULL, 0, 0);
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

/**
 * Ecore example illustrating ecore evas callbacks.
 *
 * You'll need at least one Evas engine built for it (excluding the
 * buffer one). See stdout/stderr for output.
 *
 * @verbatim
 * gcc -o core_evas_callbacks core_evas_callbacks.c `pkg-config --libs --cflags ecore ecore-evas`
 * @endverbatim
 */

#include <Efl_Core.h>
#include <Ecore_Evas.h>

static void
_destroy(Ecore_Evas *ee EFL_UNUSED)
{
   printf("destroy\n");
   core_main_loop_quit();
}

static void
_delete(Ecore_Evas *ee EFL_UNUSED)
{
   printf("delete\n");
   core_main_loop_quit();
}

static void
_focus_in(Ecore_Evas *ee EFL_UNUSED)
{
   printf("focus_in\n");
}

static void
_focus_out(Ecore_Evas *ee EFL_UNUSED)
{
   printf("focus_out\n");
}

static void
_hide(Ecore_Evas *ee EFL_UNUSED)
{
   printf("hide\n");
}

static void
_mouse_in(Ecore_Evas *ee EFL_UNUSED)
{
   printf("mouse_in\n");
}

static void
_show(Ecore_Evas *ee EFL_UNUSED)
{
   printf("show\n");
}

static void
_mouse_out(Ecore_Evas *ee EFL_UNUSED)
{
   printf("mouse_out\n");
}

static void
_move(Ecore_Evas *ee EFL_UNUSED)
{
   printf("move\n");
}

static void
_post_render(Ecore_Evas *ee EFL_UNUSED)
{
   printf("post_render\n");
}

static void
_pre_free(Ecore_Evas *ee EFL_UNUSED)
{
   printf("pre_free\n");
}

static void
_pre_render(Ecore_Evas *ee EFL_UNUSED)
{
   printf("pre_render\n");
}

static void
_resize(Ecore_Evas *ee EFL_UNUSED)
{
   printf("resize\n");
}

int
main(void)
{
   Ecore_Evas *ee;
   Evas_Object *bg;

   core_evas_init();

   ee = core_evas_new(NULL, 0, 0, 200, 100, NULL);
   core_evas_title_set(ee, "Ecore Evas Callbacks Example");
   core_evas_show(ee);

   bg = evas_object_rectangle_add(core_evas_get(ee));
   evas_object_color_set(bg, 255, 128, 0, 255);
   evas_object_resize(bg, 9999, 9999);
   evas_object_show(bg);

   //callbacks
   core_evas_callback_delete_request_set(ee, _delete);
   core_evas_callback_destroy_set(ee, _destroy);
   core_evas_callback_focus_in_set(ee, _focus_in);
   core_evas_callback_focus_out_set(ee, _focus_out);
   core_evas_callback_hide_set(ee, _hide);
   core_evas_callback_mouse_in_set(ee, _mouse_in);
   core_evas_callback_mouse_out_set(ee, _mouse_out);
   core_evas_callback_move_set(ee, _move);
   core_evas_callback_post_render_set(ee, _post_render);
   core_evas_callback_pre_free_set(ee, _pre_free);
   core_evas_callback_pre_render_set(ee, _pre_render);
   core_evas_callback_resize_set(ee, _resize);
   core_evas_callback_show_set(ee, _show);

   core_main_loop_begin();

   core_evas_free(ee);
   core_evas_shutdown();

   return 0;
}


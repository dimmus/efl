/**
 * Ecore example illustrating ecore evas object usage.
 *
 * You'll need at least one Evas engine built for it (excluding the
 * buffer one). See stdout/stderr for output.
 *
 * @verbatim
 * gcc -o core_evas_object_example core_evas_object_example.c `pkg-config --libs --cflags ecore evas ecore-evas`
 * @endverbatim
 */

#include <Efl_Core.h>
#include <Ecore_Evas.h>

Evas_Object *cursor;

static void
_mouse_down_cb(void *data, Evas *evas EFL_UNUSED, Evas_Object *obj EFL_UNUSED, void *event_info EFL_UNUSED)
{
   static Efl_Bool flag = EFL_FALSE;

   if (!flag)
     {
        core_evas_cursor_unset(data);
        core_evas_object_cursor_set(data, NULL, 0, 1, 1);
     }
   else
     core_evas_object_cursor_set(data, cursor, 0, 1, 1);

   flag = !flag;
}

int
main(void)
{
   Ecore_Evas *ee;
   Evas_Object *bg, *obj;
   int layer, x, y;

   core_evas_init();

   ee = core_evas_new(NULL, 0, 0, 200, 200, NULL);
   core_evas_title_set(ee, "Ecore Evas Object Example");
   core_evas_show(ee);

   bg = evas_object_rectangle_add(core_evas_get(ee));
   evas_object_color_set(bg, 0, 0, 255, 255);
   evas_object_resize(bg, 200, 200);
   evas_object_show(bg);
   core_evas_object_associate(ee, bg, CORE_EVAS_OBJECT_ASSOCIATE_BASE);
   evas_object_event_callback_add(bg, EVAS_CALLBACK_MOUSE_DOWN, _mouse_down_cb, ee);

   if (bg == core_evas_object_associate_get(ee))
     printf("Association worked!\n");

   cursor = evas_object_rectangle_add(core_evas_get(ee));
   evas_object_color_set(cursor, 0, 255, 0, 255);
   evas_object_resize(cursor, 5, 10);
   core_evas_object_cursor_set(ee, cursor, 0, 1, 1);

   core_evas_cursor_get(ee, &obj, &layer, &x, &y);
   if (obj == cursor && layer == 0 && x == 1 && y == 1)
     printf("Set cursor worked!\n");

   core_main_loop_begin();

   core_evas_free(ee);
   core_evas_shutdown();

   return 0;
}


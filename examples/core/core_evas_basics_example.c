/**
 * Ecore example illustrating the basics of ecore evas usage.
 *
 * You'll need at least one Evas engine built for it (excluding the
 * buffer one). See stdout/stderr for output.
 *
 * @verbatim
 * gcc -o core_evas_basics_example core_evas_basics_example.c `pkg-config --libs --cflags ecore evas ecore-evas`
 * @endverbatim
 */

#include <Efl_Core.h>
#include <Ecore_Evas.h>
#include <unistd.h>

static Efl_Bool
_stdin_cb(void *data EFL_UNUSED, Core_Fd_Handler *handler EFL_UNUSED)
{
   Eina_List *l;
   Ecore_Evas *ee;
   char c;

   int ret = scanf("%c", &c);
   if (ret < 1) return CORE_CALLBACK_RENEW;

   if (c == 'h')
     EINA_LIST_FOREACH(core_evas_core_evas_list_get(), l, ee)
       core_evas_hide(ee);
   else if (c == 's')
     EINA_LIST_FOREACH(core_evas_core_evas_list_get(), l, ee)
       core_evas_show(ee);

   return CORE_CALLBACK_RENEW;
}

static void
_on_delete(Ecore_Evas *ee)
{
   free(core_evas_data_get(ee, "key"));
   core_main_loop_quit();
}

int
main(void)
{
   Ecore_Evas *ee;
   Evas *canvas;
   Evas_Object *bg;
   Eina_List *engines, *l;
   char *data;

   if (core_evas_init() <= 0)
     return 1;

   engines = core_evas_engines_get();
   printf("Available engines:\n");
   EINA_LIST_FOREACH(engines, l, data)
     printf("%s\n", data);
   core_evas_engines_free(engines);

   ee = core_evas_new(NULL, 0, 0, 200, 200, NULL);
   core_evas_title_set(ee, "Ecore Evas basics Example");
   core_evas_show(ee);

   data = malloc(sizeof(char) * 6);
   sprintf(data, "%s", "hello");
   core_evas_data_set(ee, "key", data);
   core_evas_callback_delete_request_set(ee, _on_delete);

   printf("Using %s engine!\n", core_evas_engine_name_get(ee));

   canvas = core_evas_get(ee);
   if (core_evas_core_evas_get(canvas) == ee)
     printf("Everything is sane!\n");

   bg = evas_object_rectangle_add(canvas);
   evas_object_color_set(bg, 0, 0, 255, 255);
   evas_object_resize(bg, 200, 200);
   evas_object_show(bg);
   core_evas_object_associate(ee, bg, CORE_EVAS_OBJECT_ASSOCIATE_BASE);

   core_main_fd_handler_add(STDIN_FILENO, CORE_FD_READ, _stdin_cb, NULL, NULL, NULL);

   core_main_loop_begin();

   core_evas_free(ee);
   core_evas_shutdown();

   return 0;
}


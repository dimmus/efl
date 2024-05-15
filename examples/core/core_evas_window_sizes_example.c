/**
 * Simple @c Ecore_Evas example illustrating how to deal with window
 * sizes
 *
 * You'll need at least one engine built for it (excluding the buffer
 * one). See stdout/stderr for output.
 *
 * @verbatim
 * gcc -o core_evas_window_sizes_example core_evas_window_sizes_example.c `pkg-config --libs --cflags evas ecore ecore-evas`
 * @endverbatim
 */

#ifdef HAVE_CONFIG_H

#include "efl_config.h"
#else
#define EFL_UNUSED
#endif

#include <Efl_Core.h>
#include <Ecore_Evas.h>

#define WIDTH  (300)
#define HEIGHT (300)

static Ecore_Evas *ee;
static Evas_Object *text, *bg;
static Efl_Bool min_set = EFL_FALSE;
static Efl_Bool max_set = EFL_FALSE;
static Efl_Bool base_set = EFL_FALSE;
static Efl_Bool step_set = EFL_FALSE;

static const char commands[] = \
  "commands are:\n"
  "\tm - impose a minumum size to the window\n"
  "\tx - impose a maximum size to the window\n"
  "\tb - impose a base size to the window\n"
  "\ts - impose a step size (different than 1 px) to the window\n"
  "\tg - get the screen geometry\n"
  "\th - print help\n";

/* to inform current window's size */
static void
_canvas_resize_cb(Ecore_Evas *ee_)
{
   int w, h;
   char buf[1024];

   core_evas_geometry_get(ee_, NULL, NULL, &w, &h);
   snprintf(buf, sizeof(buf), "%d x %d", w, h);
   evas_object_text_text_set(text, buf);
   evas_object_move(text, (w - 150) / 2, (h - 50) / 2);

   evas_object_resize(bg, w, h);
}

static void
_on_destroy(Ecore_Evas *ee_ EFL_UNUSED)
{
   core_main_loop_quit();
}

static void
_on_keydown(void *data EFL_UNUSED,
            Evas *evas EFL_UNUSED,
            Evas_Object *o EFL_UNUSED,
            void *einfo)
{
   Evas_Event_Key_Down *ev = einfo;

   if (strcmp(ev->key, "h") == 0) /* print help */
     {
        printf(commands);
        return;
     }

   if (strcmp(ev->key, "m") == 0) /* impose a minimum size on the window */
     {
        min_set = !min_set;

        if (min_set)
          {
             core_evas_size_min_set(ee, WIDTH / 2, HEIGHT / 2);
             printf("Imposing a minimum size of %d x %d\n",
                     WIDTH / 2, HEIGHT / 2);
          }
        else
          {
             core_evas_size_min_set(ee, 0, 0);
             printf("Taking off minimum size restriction from the"
                             " window\n");
          }
        return;
     }

   if (strcmp(ev->key, "x") == 0) /* impose a maximum size on the window */
     {
        max_set = !max_set;

        if (max_set)
          {
             core_evas_size_max_set(ee, WIDTH * 2, HEIGHT * 2);
             printf("Imposing a maximum size of %d x %d\n",
                     WIDTH * 2, HEIGHT * 2);
          }
        else
          {
             core_evas_size_max_set(ee, 0, 0);
             printf("Taking off maximum size restriction from the"
                             " window\n");
          }
        return;
     }

   if (strcmp(ev->key, "b") == 0) /* impose a base size on the window */
     {
        base_set = !base_set;

        if (base_set)
          {
             core_evas_size_base_set(ee, WIDTH * 2, HEIGHT * 2);
             printf("Imposing a base size of %d x %d\n",
                     WIDTH * 2, HEIGHT * 2);
          }
        else
          {
             core_evas_size_base_set(ee, 0, 0);
             printf("Taking off base size restriction from the"
                             " window\n");
          }
        return;
     }

   if (strcmp(ev->key, "s") == 0) /* impose a step size on the window */
     {
        step_set = !step_set;

        if (step_set)
          {
             core_evas_size_step_set(ee, 40, 40);
             printf("Imposing a step size of %d x %d\n", 40, 40);
          }
        else
          {
             core_evas_size_step_set(ee, 0, 0);
             printf("Taking off step size restriction from the"
                             " window\n");
          }
        return;
     }

   if (strcmp(ev->key, "g") == 0) /* get screen geometry */
     {
        int x, y, w, h;
        core_evas_screen_geometry_get(ee, &x, &y, &w, &h);
        printf("screen geometry: %d,%d, %dx%d\n", x, y, w, h);
        return;
     }
}

int
main(void)
{
   Evas *evas;

   if (!core_evas_init())
     return EXIT_FAILURE;

   /* this will give you a window with an Evas canvas under the first
    * engine available */
   ee = core_evas_new(NULL, 0, 0, WIDTH, HEIGHT, NULL);
   if (!ee) goto error;

   core_evas_callback_delete_request_set(ee, _on_destroy);
   core_evas_title_set(ee, "Ecore_Evas window sizes example");
   core_evas_callback_resize_set(ee, _canvas_resize_cb);
   core_evas_show(ee);

   evas = core_evas_get(ee);

   bg = evas_object_rectangle_add(evas);
   evas_object_color_set(bg, 255, 255, 255, 255);  /* white bg */
   evas_object_move(bg, 0, 0);  /* at canvas' origin */
   evas_object_resize(bg, WIDTH, HEIGHT);  /* covers full canvas */
   evas_object_show(bg);

   evas_object_focus_set(bg, EFL_TRUE);
   evas_object_event_callback_add(
     bg, EVAS_CALLBACK_KEY_DOWN, _on_keydown, NULL);

   text = evas_object_text_add(evas);
   evas_object_color_set(text, 0, 0, 0, 255);
   evas_object_resize(text, 150, 50);
   evas_object_text_font_set(text, "Sans", 20);
   evas_object_show(text);

   _canvas_resize_cb(ee);
   printf(commands);
   core_main_loop_begin();

   core_evas_free(ee);
   core_evas_shutdown();

   return 0;

error:
   fprintf(stderr, "error: Requires at least one Evas engine built"
                   " and linked to ecore-evas for this example to run"
                   " properly.\n");
   core_evas_shutdown();
   return -1;
}


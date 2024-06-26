//Compile with:
//gcc -g -Wall -o core_animator_example core_animator_example.c `pkg-config --cflags --libs evas ecore-evas ecore`

#include <Efl_Core.h>
#include <Ecore_Evas.h>

static Efl_Bool _advance_frame(void *data, double pos);
static Efl_Bool _advance_frame2(void *data, double pos);
static Efl_Bool _advance_frame3(void *data);
static Efl_Bool _start_second_anim(void *data);
static Efl_Bool _freeze_third_anim(void *data);
static Efl_Bool _thaw_third_anim(void *data);

int
main(void)
{
   Evas_Object *rect, *bg, *rect2;
   Ecore_Evas *ee;
   Evas *evas;
   Ecore_Animator *anim;

   core_evas_init();

   ee = core_evas_new(NULL, 0, 0, 300, 400, NULL);
   core_evas_show(ee);
   evas = core_evas_get(ee);

   bg = evas_object_rectangle_add(evas);
   evas_object_resize(bg, 300, 400);
   evas_object_show(bg);

   rect = evas_object_rectangle_add(evas);
   evas_object_color_set(rect, 0, 0, 255, 255);
   evas_object_resize(rect, 50, 50);
   evas_object_show(rect);

   rect2 = evas_object_rectangle_add(evas);
   evas_object_color_set(rect2, 0, 55, 0, 255);
   evas_object_resize(rect2, 50, 50);
   evas_object_show(rect2);

   core_animator_frametime_set(1. / 50);
   core_animator_timeline_add(5, _advance_frame, rect);

   anim = core_animator_add(_advance_frame3, rect2);

   core_timer_add(10, _start_second_anim, rect);
   core_timer_add(5, _freeze_third_anim, anim);
   core_timer_add(10, _thaw_third_anim, anim);
   core_main_loop_begin();

   evas_object_del(rect);
   core_evas_free(ee);
   core_animator_del(anim);
   core_evas_shutdown();

   return 0;
}

static Efl_Bool
_advance_frame(void *data, double pos)
{
   double frame = pos;
   frame = core_animator_pos_map(pos, CORE_POS_MAP_SPRING, 1.2, 15);

   evas_object_resize(data, 50 * (1 + frame), 50 * (1 + frame));
   evas_object_move(data, 100 * frame, 100 * frame);
   evas_object_color_set(data, 255 * frame, 0, 255 * (1 - frame), 255);
   return CORE_CALLBACK_RENEW;
}

static Efl_Bool
_start_second_anim(void *data)
{
   core_animator_frametime_set(1. / 10);
   core_animator_timeline_add(20, _advance_frame2, data);
   return CORE_CALLBACK_CANCEL;
}

static Efl_Bool
_advance_frame2(void *data, double pos)
{
   double frame = pos;
   frame = core_animator_pos_map(pos, CORE_POS_MAP_BOUNCE, 1.2, 50);

   evas_object_resize(data, 100 - (50 * frame), 100 - (50 * frame));
   evas_object_move(data, 100 * (1 - frame), 100 * (1 - frame));
   evas_object_color_set(data, 255 * (1 - frame), 0, 255 * frame, 255);
   return CORE_CALLBACK_RENEW;
}

static Efl_Bool
_advance_frame3(void *data)
{
   static int x = 0;

   if (x >= 250)
     x = 0;
   evas_object_move(data, ++x, 350);

   return CORE_CALLBACK_RENEW;
}

static Efl_Bool
_freeze_third_anim(void *data)
{
   core_animator_freeze(data);
   return CORE_CALLBACK_CANCEL;
}

static Efl_Bool
_thaw_third_anim(void *data)
{
   core_animator_thaw(data);
   return CORE_CALLBACK_CANCEL;
}


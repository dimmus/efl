#define EFL_BETA_API_SUPPORT

#include <Efl_Eo_Interfaces.h>
#include <stdio.h>
#include <Efl_Core.h>
#include <Evas.h>
#include <Efl_Shared.h>
#include <Ecore_Evas.h>
#include <Ecore_Input.h>

static Efl_Bool
_keyboard_event(void *data EFL_UNUSED, int type, void *event)
{
   Ecore_Event_Key *e = event;
   Efl_Input_Device *seat = NULL;

   if (e->dev)
     seat = efl_input_device_seat_get(e->dev);

   printf("The keyboard on seat '%s' %s the key '%s'\n", seat ?
          efl_name_get(seat) : "unknown",
          type == CORE_EVENT_KEY_DOWN ? "pressed" : "released",
          e->keyname);

   return CORE_CALLBACK_PASS_ON;
}

static Efl_Bool
_mouse_move(void *data EFL_UNUSED, int type EFL_UNUSED, void *event)
{
   Ecore_Event_Mouse_Move *e = event;
   Efl_Input_Device *seat = NULL;

   if (e->dev)
     seat = efl_input_device_seat_get(e->dev);

   printf("The mouse on seat '%s' is at X: %d Y:%d\n",
          seat ? efl_name_get(seat) : "unknown", e->x, e->y);
   return CORE_CALLBACK_PASS_ON;
}

static Efl_Bool
_mouse_button(void *data EFL_UNUSED, int type, void *event)
{
   Ecore_Event_Mouse_Button *e = event;
   Efl_Input_Device *seat = NULL;

   if (e->dev)
     seat = efl_input_device_seat_get(e->dev);

   printf("The mouse on seat '%s' %s the following button '%d'\n",
          seat ? efl_name_get(seat) : "unknown",
          type == CORE_EVENT_MOUSE_BUTTON_DOWN ? "pressed" : "released",
          e->buttons);
   return CORE_CALLBACK_PASS_ON;
}

static Efl_Bool
_mouse_wheel(void *data EFL_UNUSED, int type EFL_UNUSED, void *event)
{
   Ecore_Event_Mouse_Wheel *e = event;
   Efl_Input_Device *seat = NULL;

   if (e->dev)
     seat = efl_input_device_seat_get(e->dev);

   printf("The mouse on seat '%s' moved the wheel '%s'\n",
          seat ? efl_name_get(seat) : "unknown",
          e->z < 0 ? "up" : "down");
   return CORE_CALLBACK_PASS_ON;
}

static const char *
_device_type_to_string(Efl_Input_Device_Type klass)
{
   switch (klass)
     {
      case EFL_INPUT_DEVICE_TYPE_NONE:
         return "None";
      case EFL_INPUT_DEVICE_TYPE_SEAT:
         return "Seat";
      case EFL_INPUT_DEVICE_TYPE_KEYBOARD:
         return "Keyboard";
      case EFL_INPUT_DEVICE_TYPE_MOUSE:
         return "Mouse";
      case EFL_INPUT_DEVICE_TYPE_TOUCH:
         return "Touch";
      case EFL_INPUT_DEVICE_TYPE_PEN:
         return "Pen";
      case EFL_INPUT_DEVICE_TYPE_WAND:
         return "Wand";
      case EFL_INPUT_DEVICE_TYPE_GAMEPAD:
         return "Gamepad";
      default:
         return "Unknown";
     }
}

static void
_dev_added_or_removed(void *data, const Efl_Event *event)
{
   Efl_Input_Device *dev = event->info;

   printf("The device %p '%s' - class: '%s' - desc: '%s' was '%s' on ee %p\n",
          dev, efl_name_get(dev),
          _device_type_to_string(efl_input_device_type_get(dev)),
          efl_comment_get(dev),
          event->desc == EFL_CANVAS_SCENE_EVENT_DEVICE_ADDED ? "added" : "removed",
          data);
}

static void
_dev_changed(void *data, const Efl_Event *event)
{
   Efl_Input_Device *dev = event->info;

   printf("The device %p '%s' - class: '%s' was changed on ee %p\n",
          dev, efl_name_get(dev),
          _device_type_to_string(efl_input_device_type_get(dev)), data);
}

int
main(int argc EFL_UNUSED, char *argv[] EFL_UNUSED)
{
   Ecore_Evas *ee1, *ee2;
   Evas *evas1, *evas2;
   Evas_Object *bg, *rect1, *rect2;
   Core_Event_Handler *keydown_handler, *keyup_handler, *mouse_move,
     *mouse_down, *mouse_up, *mouse_wheel;

   core_evas_init();

   ee1 = core_evas_new("wayland_shm", 0, 0, 800, 600, NULL);
   if (!ee1)
     ee1 = core_evas_new("wayland_egl", 0, 0, 800, 600, NULL);

   if (!ee1)
     {
        fprintf(stderr, "Could not create the ecore evas\n");
        return -1;
     }

   evas1 = core_evas_get(ee1);

   bg = evas_object_rectangle_add(evas1);
   evas_object_color_set(bg, 255, 255, 255, 255);
   evas_object_move(bg, 0, 0);
   evas_object_resize(bg, 800, 600);
   evas_object_show(bg);

   /* TODO play with focus */
   rect1 = evas_object_rectangle_add(evas1);
   evas_object_color_set(rect1, 0, 255, 0, 255);
   evas_object_resize(rect1, 50, 50);
   evas_object_move(rect1, (800 - 50) /2, (600 - 50)/2);
   evas_object_show(rect1);

   rect2 = evas_object_rectangle_add(evas1);
   evas_object_color_set(rect2, 0, 0, 255, 255);
   evas_object_resize(rect2, 50, 50);
   evas_object_move(rect2, 50, 50);
   evas_object_show(rect2);

   core_evas_show(ee1);

   ee2 = core_evas_new("wayland_shm", 0, 0, 300, 200, NULL);
   if (!ee2)
     ee2 = core_evas_new("wayland_egl", 0, 0, 300, 200, NULL);

   if (!ee2)
     {
        fprintf(stderr, "Could not create the ecore evas\n");
        return -1;
     }

   evas2 = core_evas_get(ee2);

   bg = evas_object_rectangle_add(evas2);
   evas_object_color_set(bg, 100, 100, 100, 255);
   evas_object_move(bg, 0, 0);
   evas_object_resize(bg, 300, 200);
   evas_object_show(bg);

   core_evas_show(ee2);

   keydown_handler = core_event_handler_add(CORE_EVENT_KEY_DOWN,
                                             _keyboard_event, NULL);
   keyup_handler = core_event_handler_add(CORE_EVENT_KEY_UP,
                                           _keyboard_event, NULL);
   mouse_move = core_event_handler_add(CORE_EVENT_MOUSE_MOVE, _mouse_move,
                                        NULL);
   mouse_up = core_event_handler_add(CORE_EVENT_MOUSE_BUTTON_UP,
                                      _mouse_button, NULL);
   mouse_down = core_event_handler_add(CORE_EVENT_MOUSE_BUTTON_DOWN,
                                        _mouse_button, NULL);
   mouse_wheel = core_event_handler_add(CORE_EVENT_MOUSE_WHEEL,
                                        _mouse_wheel, NULL);

   efl_event_callback_add(evas1, EFL_CANVAS_SCENE_EVENT_DEVICE_ADDED,
                          _dev_added_or_removed, ee1);
   efl_event_callback_add(evas1, EFL_CANVAS_SCENE_EVENT_DEVICE_REMOVED,
                          _dev_added_or_removed, ee1);
   efl_event_callback_add(evas1, EFL_CANVAS_SCENE_EVENT_DEVICE_CHANGED,
                          _dev_changed, ee1);

   efl_event_callback_add(evas2, EFL_CANVAS_SCENE_EVENT_DEVICE_ADDED,
                          _dev_added_or_removed, ee2);
   efl_event_callback_add(evas2, EFL_CANVAS_SCENE_EVENT_DEVICE_REMOVED,
                          _dev_added_or_removed, ee2);
   efl_event_callback_add(evas2, EFL_CANVAS_SCENE_EVENT_DEVICE_CHANGED,
                          _dev_changed, ee2);

   core_main_loop_begin();

   core_event_handler_del(mouse_wheel);
   core_event_handler_del(keydown_handler);
   core_event_handler_del(keyup_handler);
   core_event_handler_del(mouse_move);
   core_event_handler_del(mouse_up);
   core_event_handler_del(mouse_down);

   core_evas_free(ee1);
   core_evas_free(ee2);

   core_evas_shutdown();

   return 0;
}

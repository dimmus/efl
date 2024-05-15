#define EFL_BETA_API_SUPPORT

#include <Efl_Eo_Interfaces.h>
#include <stdio.h>
#include <Efl_Core.h>
#include <Evas.h>
#include <Efl_Shared.h>
#include <Ecore_Evas.h>
#include <Ecore_Input.h>
#include <Efl_Core_Getopt.h>

static int width = 800;

static void
_resize_cb(Ecore_Evas *ee)
{
   Evas_Object *vnc;
   int w = 0, h = 0;

   core_evas_geometry_get(ee, NULL, NULL, &w, &h);

   fprintf(stderr, "resizing [%i, %i]\n", w, h);
   vnc = core_evas_data_get(ee, "vnc");
   evas_object_resize(vnc, w, h);
}

static Efl_Bool
_anim(void *data)
{
   static enum { RIGHT, LEFT } direction = LEFT;
   static const int speed = 20;
   int x, y;
   Evas_Object *rect = data;

   evas_object_geometry_get(rect, &x, &y, NULL, NULL);
   if (direction == LEFT)
     {
        x -= speed;
        if (x <= 0)
          {
             x = 0;
             direction = RIGHT;
          }
     }
   else
     {
        x += speed;
        if (x >= width)
          {
             direction = LEFT;
             x = width;
          }
     }

   evas_object_move(rect, x, y);

   return CORE_CALLBACK_RENEW;
}

static Efl_Bool
_accept_cb(void *data EFL_UNUSED, Ecore_Evas *ee EFL_UNUSED, const char *client_host)
{
   printf("Client %s trying to connect\n", client_host);
   return EFL_TRUE;
}

static void
_disc_cb(void *data EFL_UNUSED, Ecore_Evas *ee EFL_UNUSED, const char *client_host)
{
   printf("Client %s disconnected\n", client_host);
}

static Efl_Bool
_keyboard_event(void *data EFL_UNUSED, int type, void *event)
{
   Ecore_Event_Key *e = event;
   Efl_Input_Device *seat = NULL;

   if (e->dev)
     seat = efl_input_device_seat_get(e->dev);

   printf("The keyboard on seat '%s' %s the key '%s'\n", seat ?
          efl_name_get(seat) : "default",
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
          seat ? efl_name_get(seat) : "default", e->x, e->y);
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
          seat ? efl_name_get(seat) : "default",
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
          seat ? efl_name_get(seat) : "default",
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
_seat_children_print(Efl_Input_Device *seat)
{
   Efl_Input_Device *child;
   Eina_Iterator *it;

   printf("Children of seat: %s (%s, seat id: %d)\n", efl_name_get(seat),
          _device_type_to_string(efl_input_device_type_get(seat)),
          efl_input_device_seat_id_get(seat));

   it = efl_input_device_children_iterate(seat);
   EINA_ITERATOR_FOREACH(it, child)
     {
        printf(" - Sub device: %s (%s, seat id: %d)\n", efl_name_get(child),
               _device_type_to_string(efl_input_device_type_get(child)),
               efl_input_device_seat_id_get(seat));
     }
   eina_iterator_free(it);
}

static void
_dev_added_or_removed(void *data EFL_UNUSED, const Efl_Event *event)
{
   Efl_Input_Device *dev = event->info;

   printf("The device '%s' - class: '%s' - description: '%s' was '%s'\n",
          efl_name_get(dev),
          _device_type_to_string(efl_input_device_type_get(dev)),
          efl_comment_get(dev),
          event->desc == EFL_CANVAS_SCENE_EVENT_DEVICE_ADDED ? "added" : "removed");

   if (efl_input_device_type_get(dev) == EFL_INPUT_DEVICE_TYPE_SEAT)
     _seat_children_print(dev);
}

int
main(int argc, char *argv[])
{
   Ecore_Evas *ee;
   Evas *evas;
   Evas_Object *bg, *rect;
   Ecore_Animator *animator;
   Evas_Object *vnc;
   Core_Event_Handler *keydown_handler, *keyup_handler, *mouse_move,
     *mouse_down, *mouse_up, *mouse_wheel;
   char *engine = "software_x11";
   int args, height = 600;
   Efl_Bool quit_option = EFL_FALSE;
   static const Core_Getopt options = {
     "core_evas_vnc_example",
     NULL,
     "0.1",
     "(C) 2016 Enlightenment Project",
     "BSD 2-Clause",
     "Ecore_Evas VNC example.\n",
     EFL_TRUE,
     {
       CORE_GETOPT_STORE_DEF_STR('e', "engine", "The engine backend", "software_x11"),
       CORE_GETOPT_STORE_DEF_INT('w', "width", "The window width", 800),
       CORE_GETOPT_STORE_DEF_INT('h', "height", "The window height", 600),
       CORE_GETOPT_VERSION('v', "version"),
       CORE_GETOPT_COPYRIGHT('c', "copyright"),
       CORE_GETOPT_LICENSE('k', "license"),
       CORE_GETOPT_HELP('H', "help"),
       CORE_GETOPT_SENTINEL
     }
   };
   Core_Getopt_Value values[] = {
     CORE_GETOPT_VALUE_STR(engine),
     CORE_GETOPT_VALUE_INT(width),
     CORE_GETOPT_VALUE_INT(height),
     CORE_GETOPT_VALUE_BOOL(quit_option),
     CORE_GETOPT_VALUE_BOOL(quit_option),
     CORE_GETOPT_VALUE_BOOL(quit_option),
     CORE_GETOPT_VALUE_BOOL(quit_option),
     CORE_GETOPT_VALUE_NONE
   };

   core_evas_init();

   args = core_getopt_parse(&options, values, argc, argv);
   if (args < 0)
     {
        fprintf(stderr, "Could not parse command line options.\n");
        return -1;
     }

   if (quit_option) return 0;

   printf("Using engine '%s'. Width: %d - Height: %d\n", engine, width, height);
   ee = core_evas_new(engine, 0, 0, width, height, NULL);

   if (!ee)
     {
        fprintf(stderr, "Could not create the ecore evas\n");
        return -1;
     }

   evas = core_evas_get(ee);

   bg = evas_object_rectangle_add(evas);
   evas_object_color_set(bg, 255, 255, 255, 255);
   evas_object_move(bg, 0, 0);
   evas_object_resize(bg, width, height);
   evas_object_show(bg);

   rect = evas_object_rectangle_add(evas);
   evas_object_color_set(rect, 0, 255, 0, 255);
   evas_object_resize(rect, 50, 50);
   evas_object_move(rect, (width - 50) /2, (height - 50)/2);
   evas_object_show(rect);

   animator = core_animator_add(_anim, rect);

   core_evas_show(ee);

   vnc = core_evas_vnc_start(ee, "localhost", -1, _accept_cb, _disc_cb, NULL);
   core_evas_data_set(ee, "vnc", vnc);
   core_evas_callback_resize_set(ee, _resize_cb);
   evas_object_resize(vnc, width, height);
   evas_object_show(vnc);

   if (!vnc)
     {
        fprintf(stderr, "Could not enable the VNC support!\n");
        goto exit;
     }

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

   _seat_children_print(evas_default_device_get(evas, EFL_INPUT_DEVICE_TYPE_SEAT));
   efl_event_callback_add(evas, EFL_CANVAS_SCENE_EVENT_DEVICE_ADDED,
                          _dev_added_or_removed, NULL);
   efl_event_callback_add(evas, EFL_CANVAS_SCENE_EVENT_DEVICE_REMOVED,
                          _dev_added_or_removed, NULL);
   core_main_loop_begin();

   core_event_handler_del(mouse_wheel);
   core_event_handler_del(keydown_handler);
   core_event_handler_del(keyup_handler);
   core_event_handler_del(mouse_move);
   core_event_handler_del(mouse_up);
   core_event_handler_del(mouse_down);

 exit:
   core_evas_free(ee);
   core_animator_del(animator);
   core_evas_shutdown();
   return 0;
}

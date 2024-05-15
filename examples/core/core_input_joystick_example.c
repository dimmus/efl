//Compile with:
// gcc -g -Wall -o core_input_joystick_example core_input_joystick_example.c `pkg-config --cflags --libs ecore ecore-input`

#include <Efl_Core.h>
#include <Ecore_Input.h>

static Efl_Bool
_joystick_event_handler_cb(void *data EFL_UNUSED, int type EFL_UNUSED, void *event)
{
   printf("deadzone: %d\n", core_input_joystick_event_axis_deadzone_get());
   Ecore_Event_Joystick *ev = event;
   switch (ev->type)
     {
      case CORE_EVENT_JOYSTICK_EVENT_TYPE_CONNECTED:
        printf("joystick is connected: %d\n", ev->index);
        break;
      case CORE_EVENT_JOYSTICK_EVENT_TYPE_DISCONNECTED:
        printf("joystick is disconnected: %d\n", ev->index);
        break;
      case CORE_EVENT_JOYSTICK_EVENT_TYPE_BUTTON:
        printf("joystick(%d) button index: %d, value: %f, time: %u\n",
                                          ev->index, ev->button.index,
                                     ev->button.value, ev->timestamp);
        break;
      case CORE_EVENT_JOYSTICK_EVENT_TYPE_AXIS:
        printf("joystick(%d) axis index: %d, value: %f, time: %u\n",
                                          ev->index, ev->axis.index,
                                     ev->axis.value, ev->timestamp);
        break;
      default:
        printf("unhandled event type: %d\n", ev->type);
        break;
     }

   const char *joystick_name;
   joystick_name = core_input_joystick_name_get(ev->index);
   printf("joystick name is: %s (index: %d)\n", joystick_name, ev->index);

   if (ev->type == CORE_EVENT_JOYSTICK_EVENT_TYPE_BUTTON &&
       ev->button.index == CORE_EVENT_JOYSTICK_BUTTON_START)
     core_main_loop_quit();

   return CORE_CALLBACK_DONE;
}

int
main(void)
{
   if (!core_event_init())
     {
        printf("ERROR: Cannot init Ecore!\n");
        return -1;
     }

   core_event_handler_add(CORE_EVENT_JOYSTICK,
                           _joystick_event_handler_cb,
                           NULL);
   core_input_joystick_event_axis_deadzone_set(300);

   printf("start the main loop.\n");

   core_main_loop_begin();

   core_shutdown();

   return 0;
}

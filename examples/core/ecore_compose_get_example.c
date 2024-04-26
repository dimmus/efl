/* gcc -g -Wall -o ecore_compose_get_example ecore_compose_get_example.c `pkg-config --cflags --libs ecore-input ecore-evas ecore evas eina` */

#include <stdio.h>

#include <Efl_Shared.h>
#include <Evas.h>
#include <Efl_Core.h>
#include <Ecore_Evas.h>

/******* begin 1 : Ecore compose stuff *******/

#include <Ecore_Input.h>

typedef struct
{
  Eina_List *seq;
  Efl_Bool composing;
} Data;

static void
_data_reset(Data *d)
{
   char *str;

   EINA_LIST_FREE(d->seq, str) eina_stringshare_del(str);
   d->composing = EFL_FALSE;
}

static Efl_Bool
_is_modifier(const char *key)
{
   if ((!strncmp(key, "Shift", 5)) ||
       (!strncmp(key, "Control", 7)) ||
       (!strncmp(key, "Alt", 3)) ||
       (!strncmp(key, "Meta", 4)) ||
       (!strncmp(key, "Super", 5)) ||
       (!strncmp(key, "Hyper", 5)) ||
       (!strcmp(key, "Scroll_Lock")) ||
       (!strcmp(key, "Num_Lock")) ||
       (!strcmp(key, "Caps_Lock")))
     return EFL_TRUE;
   return EFL_FALSE;
}

/******* end 1 : Ecore compose stuff *******/

static void
_key_down_cb(void *data EFL_UNUSED, Evas *e EFL_UNUSED, Evas_Object *obj EFL_UNUSED, void *event)
{
    Evas_Event_Key_Down *ev = (Evas_Event_Key_Down *)event;
    int alt;
    int shift;
    int ctrl;

    alt = evas_key_modifier_is_set(ev->modifiers, "Alt");
    shift = evas_key_modifier_is_set(ev->modifiers, "Shift");
    ctrl = evas_key_modifier_is_set(ev->modifiers, "Control");

    printf("down : keyname: %s  key: %s  compose: **%s**  string: **%s** [%zu] %d (%d:%d:%d)\n",
           ev->keyname, ev->key, ev->string, ev->compose, (ev->compose) ? strlen(ev->compose) : (size_t)-1, (ev->compose) ? *(ev->compose) : -1,
	   alt, shift, ctrl);
}

static void
_key_up_cb(void *data, Evas *e EFL_UNUSED, Evas_Object *obj EFL_UNUSED, void *event)
{
    Evas_Event_Key_Up *ev;
    int alt;
    int shift;
    int ctrl;

    ev = (Evas_Event_Key_Up *)event;

    alt = evas_key_modifier_is_set(ev->modifiers, "Alt");
    shift = evas_key_modifier_is_set(ev->modifiers, "Shift");
    ctrl = evas_key_modifier_is_set(ev->modifiers, "Control");

    printf("up  : keyname: %s  key: %s  string: %s %d (%d:%d:%d)\n",
           ev->keyname, ev->key, ev->compose, (ev->compose) ? *(ev->compose) : -1,
	   alt, shift, ctrl);

    if (!alt && !shift)
    {
        if (!strcmp(ev->key, "q"))
	    ecore_main_loop_quit();
    }

/******* begin 2 : Ecore compose stuff *******/

    {
        Data *d;
        char *compres = NULL;
        char *string = NULL;
	Ecore_Compose_State state;
	Efl_Bool free_string = EFL_FALSE;

	d = (Data *)data;
        if (!d->composing)
	{
	    _data_reset(d);
	    d->seq = eina_list_append(d->seq, eina_stringshare_add(ev->key));
	    state = ecore_compose_get(d->seq, &compres);
	    if (state == ECORE_COMPOSE_MIDDLE) d->composing = EFL_TRUE;
	    else d->composing = EFL_FALSE;
	    if (!d->composing) _data_reset(d);
	    else goto end;
	}
	else
	{
	    if (_is_modifier(ev->key)) goto end;
	    d->seq = eina_list_append(d->seq, eina_stringshare_add(ev->key));
	    state = ecore_compose_get(d->seq, &compres);
	    if (state == ECORE_COMPOSE_NONE) _data_reset(d);
	    else if (state == ECORE_COMPOSE_DONE)
	    {
	        _data_reset(d);
		if (compres)
		{
		    string = compres;
		    free_string = EFL_TRUE;
		}
	    }
	    else goto end;
	}

	if (string)
	{
	    printf(" ** string : %s %zu 0x%hhx%hhX\n", string, strlen(string), string[1], string[0]);
	    if (free_string)
	        free(string);
	}
    }

/******* end 2 : Ecore compose stuff *******/

 end:
    return;
}

static void
_del(Ecore_Evas *ee EFL_UNUSED)
{
  ecore_main_loop_quit();
}

int main()
{
  Ecore_Evas *ee;
  Evas *evas;
  Evas_Object *o;

/******* begin 3 : Ecore compose stuff *******/
  Data *d;

  d = calloc(1, sizeof(Data));

  if (!ecore_event_init())
    goto ecore_event_init_fail;

/******* end 3 : Ecore compose stuff *******/

  if (!ecore_evas_init())
    goto ecore_evas_init_fail;

  ee = ecore_evas_new(NULL, 10, 10, 0, 0, NULL);
  if (!ee)
    {
       ecore_evas_shutdown();
       ecore_event_shutdown();
       free(d);
       return -1;
    }
  ecore_evas_callback_delete_request_set(ee, _del);

  evas = ecore_evas_get(ee);

  o = evas_object_rectangle_add(evas);
  evas_object_color_set(o, 255, 0, 0, 255);
  evas_object_move(o, 0, 0);
  evas_object_resize(o, 480, 480);
  evas_object_focus_set(o, EFL_TRUE);
  evas_object_event_callback_add(o, EVAS_CALLBACK_KEY_DOWN,
				 _key_down_cb, d);
  evas_object_event_callback_add(o, EVAS_CALLBACK_KEY_UP,
				 _key_up_cb, d);
  evas_object_show(o);

  ecore_evas_resize(ee, 480, 480);
  ecore_evas_show(ee);

  ecore_main_loop_begin();

  ecore_evas_free(ee);
  ecore_evas_shutdown();

ecore_evas_init_fail:
  ecore_event_shutdown();

ecore_event_init_fail:
  free(d);
  return 0;
}

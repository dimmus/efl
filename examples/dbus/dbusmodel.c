//Compile with:
// gcc -o busmodel busmodel.c `pkg-config --cflags --libs eldbus ecore eina`

#ifdef HAVE_CONFIG_H
# include <efl_config.h>
#endif

#include <Efl_Dbus.h>
#include <Efl_Dbus_Model.h>
#include <Efl_Eo_Interfaces.h>
#include <Efl_Core.h>
#include <Efl_Core_Getopt.h>

#define DEFAULT_BUS_NAME  "org.freedesktop.DBus"
#define DEFAULT_PATH "/"

static int retval = EXIT_SUCCESS;
static Efl_Bool quit_on_done = EFL_TRUE;

static void
_on_properties_changed(void *data EFL_UNUSED, const Efl_Event *event)
{
   Efl_Model_Property_Event *ev = event->info;
   Eina_Array_Iterator it;
   const char *property;
   unsigned int i;

   printf("Properties changed:\n");
   EINA_ARRAY_ITER_NEXT(ev->changed_properties, i, property, it)
     {
        Eina_Value *v;
        char *str;

        v = efl_model_property_get(event->object, property);

        if (!v)
          {
             EINA_LOG_CRIT("Property '%s' returned nothing.", property);
             abort();
          }

        str = eina_value_to_string(v);
        printf("\t%s: '%s'\n", property, str);
        free(str);

        eina_value_free(v);
     }

   printf("Properties invalidated:\n");
   EINA_ARRAY_ITER_NEXT(ev->invalidated_properties, i, property, it)
     printf("\t%s\n", property);
}

static void
_on_invalidate(void *data EFL_UNUSED, const Efl_Event *event)
{
   efl_unref(event->object);
}

EFL_CALLBACKS_ARRAY_DEFINE(child_cbs,
                           { EFL_MODEL_EVENT_PROPERTIES_CHANGED, _on_properties_changed },
                           { EFL_EVENT_INVALIDATE, _on_invalidate });

static void
process(Eo *child, unsigned int index)
{
   Eina_Iterator *properties = efl_model_properties_get(child);
   const char *property;
   Efl_Strbuf *buf;
   Efl_Bool noproperties = EFL_TRUE;

   buf = eina_strbuf_new();

   const char *name = efl_dbus_model_proxy_name_get(child);

   EINA_ITERATOR_FOREACH(properties, property)
     {
        Eina_Value *v = efl_model_property_get(child, property);
        char *str;

        if (!v)
          {
             EINA_LOG_CRIT("Property '%s' returned nothing.", property);
             abort();
          }

        str = eina_value_to_string(v);
        eina_strbuf_append_printf(buf, " \t* %s: '%s'\n", property, str);
        free(str);

        eina_value_free(v);

        noproperties = EFL_FALSE;
     }
   eina_iterator_free(properties);

   if (noproperties)
     eina_strbuf_append_printf(buf,  " %2d: %s (no properties yet)\n", index, name);
   else
     eina_strbuf_prepend_printf(buf, " -> %s\n   Properties:\n", name);

   printf("%s", eina_strbuf_string_get(buf));

   eina_strbuf_free(buf);

   efl_ref(child);

   efl_event_callback_array_add(child, child_cbs(), NULL);

   printf("monitoring events...\n");
}

static Eina_Value
_slice(void *data, const Eina_Value v,
       const Eina_Future *dead_future EFL_UNUSED)
{
   unsigned int offset = (unsigned int)(uintptr_t) data;
   unsigned int i, len;
   Eo *child;

   if (eina_value_type_get(&v) == EINA_VALUE_TYPE_ERROR) return v;

   EINA_VALUE_ARRAY_FOREACH(&v, len, i, child)
     process(child, offset + i);

   return v;
}

static void
_on_child_added(void *data EFL_UNUSED, const Efl_Event *event)
{
   Efl_Model_Children_Event *ev = event->info;

   printf("Children Added: %i\n", ev->index);

   eina_future_then(efl_model_children_slice_get(event->object, ev->index, 1),
                    _slice, (void*)(uintptr_t) ev->index);
}

static void
_on_child_removed(void *data EFL_UNUSED, const Efl_Event *event)
{
   Eo *child = event->info;
   printf("Children Removed: %p\n", child);
}

EFL_CALLBACKS_ARRAY_DEFINE(event_cbs,
                           { EFL_MODEL_EVENT_PROPERTIES_CHANGED, _on_properties_changed },
                           { EFL_MODEL_EVENT_CHILD_ADDED, _on_child_added },
                           { EFL_MODEL_EVENT_CHILD_REMOVED, _on_child_removed });

static const Core_Getopt options = {
  "dbusmodel", /* program name */
  NULL, /* usage line */
  "1", /* version */
  "(C) 2016 Enlightenment Project", /* copyright */
  "BSD 2-Clause", /* license */
  /* long description, may be multiline and contain \n */
  "Example of Eldbus.Model.Object to fetch children and properties.\n",
  EFL_FALSE,
  {
    CORE_GETOPT_STORE_TRUE('s', "system", "connect to the system bus, not user session."),
    CORE_GETOPT_STORE_FALSE('w', "wait", "after done, wait for events (monitoring)"),

    CORE_GETOPT_VERSION('V', "version"),
    CORE_GETOPT_COPYRIGHT('C', "copyright"),
    CORE_GETOPT_LICENSE('L', "license"),
    CORE_GETOPT_HELP('h', "help"),

    CORE_GETOPT_STORE_METAVAR_STR(0, NULL, "The bus name to connect.", "bus_name"),
    CORE_GETOPT_STORE_METAVAR_STR(0, NULL, "The path to explore.", "path"),

    CORE_GETOPT_SENTINEL
  }
};

int
main(int argc, char **argv EFL_UNUSED)
{
   Efl_Dbus_Connection_Type conn_type;
   Efl_Bool is_system = EFL_FALSE;
   char *bus_name = DEFAULT_BUS_NAME;
   char *path = DEFAULT_PATH;
   Efl_Bool quit_option = EFL_FALSE;
   Core_Getopt_Value values[] = {
     CORE_GETOPT_VALUE_BOOL(is_system),
     CORE_GETOPT_VALUE_BOOL(quit_on_done),

     /* standard block to provide version, copyright, license and help */
     CORE_GETOPT_VALUE_BOOL(quit_option), /* -V/--version quits */
     CORE_GETOPT_VALUE_BOOL(quit_option), /* -C/--copyright quits */
     CORE_GETOPT_VALUE_BOOL(quit_option), /* -L/--license quits */
     CORE_GETOPT_VALUE_BOOL(quit_option), /* -h/--help quits */

     /* positional argument */
     CORE_GETOPT_VALUE_STR(bus_name),
     CORE_GETOPT_VALUE_STR(path),

     CORE_GETOPT_VALUE_NONE /* sentinel */
   };
   int args;
   Eo *root;

   core_init();
   efl_dbus_init();

   args = core_getopt_parse(&options, values, argc, argv);
   if (args < 0)
     {
        fputs("ERROR: Could not parse command line options.\n", stderr);
        retval = EXIT_FAILURE;
        goto end;
     }

   if (quit_option) goto end;

   args = core_getopt_parse_positional(&options, values, argc, argv, args);
   if (args < 0)
     {
        fputs("ERROR: Could not parse positional arguments.\n", stderr);
        retval = EXIT_FAILURE;
        goto end;
     }

   conn_type = (is_system ?
                EFL_DBUS_CONNECTION_TYPE_SYSTEM :
                EFL_DBUS_CONNECTION_TYPE_SESSION);

   root = efl_add_ref(EFL_DBUS_MODEL_OBJECT_CLASS, efl_main_loop_get(),
                      efl_dbus_model_connect(efl_added, conn_type, NULL, EFL_FALSE),
                      efl_dbus_model_object_bus_set(efl_added, bus_name),
                      efl_dbus_model_object_path_set(efl_added, path),
                      efl_event_callback_array_add(efl_added, event_cbs(), NULL));

   if (efl_model_children_count_get(root))
     eina_future_then(efl_model_children_slice_get(root, 0, efl_model_children_count_get(root)),
                      _slice, NULL);

   core_main_loop_begin();
   efl_del(root);

 end:
   efl_dbus_shutdown();
   core_shutdown();
   return retval;
}

#ifndef EFL_DBUS_PRIVATE_TYPES_H
#define EFL_DBUS_PRIVATE_TYPES_H 1

typedef struct _Efl_Dbus_Connection_Name
{
   const char *name;
   const char *unique_id;
   Eina_Hash  *objects;
   int refcount;
   struct
   {
      Eina_Inlist *list; //Efl_Dbus_Connection_Context_NOC_Cb
      int          walking;
      Eina_List   *to_delete;
   } event_handlers;
   Efl_Dbus_Signal_Handler *name_owner_changed;
   Efl_Dbus_Pending *name_owner_get;
} Efl_Dbus_Connection_Name;

typedef struct _Efl_Dbus_Object_Context_Event_Cb
{
   EINA_INLIST;
   Efl_Dbus_Object_Event_Cb cb;
   const void           *cb_data;
   Efl_Bool             deleted : 1;
} Efl_Dbus_Object_Context_Event_Cb;

typedef struct _Efl_Dbus_Object_Context_Event
{
   Eina_Inlist *list;
   int          walking;
   Eina_List   *to_delete;
} Efl_Dbus_Object_Context_Event;


typedef struct _Efl_Dbus_Connection_Context_Event
{
   Eina_Inlist *list;
   int          walking;
   Eina_List   *to_delete;
} Efl_Dbus_Connection_Context_Event;

struct _Efl_Dbus_Connection
{
   EINA_MAGIC;
   EINA_INLIST;
   int                            refcount;
   Efl_Dbus_Connection_Type          type;
   DBusConnection                *dbus_conn;
   Eina_Hash                     *names; //Efl_Dbus_Connection_Name
   Eina_Inlist                   *data;
   Eina_Inlist                   *cbs_free;
   Eina_Inlist                   *signal_handlers;
   Eina_Inlist                   *pendings;
   Eina_Inlist                   *fd_handlers;
   Eina_Inlist                   *timeouts;
   Core_Idle_Enterer            *idle_enterer;
   Efl_Dbus_Connection_Context_Event event_handlers[EFL_DBUS_CONNECTION_EVENT_LAST];
   Eina_Inlist                   *root_objs;//service_object
   Efl_Dbus_Proxy                   *fdo_proxy;
   Efl_Bool                     shared : 1; /* Indicate if connection can be shared */
};

struct _Efl_Dbus_Object
{
   EINA_MAGIC;
   EINA_INLIST;
   int                        refcount;
   Efl_Dbus_Connection          *conn;
   const char                *name;
   const char                *path;
   Eina_Hash                 *proxies;
   Eina_Inlist               *pendings;
   Eina_List                 *signal_handlers;
   Eina_Inlist               *cbs_free;
   Efl_Dbus_Object_Context_Event event_handlers[EFL_DBUS_OBJECT_EVENT_LAST];
   Efl_Dbus_Signal_Handler *interfaces_added;
   Efl_Dbus_Signal_Handler *interfaces_removed;
   Efl_Dbus_Signal_Handler *properties_changed;
   Efl_Dbus_Proxy *properties;
};

struct _Efl_Dbus_Signal_Handler
{
   EINA_MAGIC;
   EINA_INLIST;
   int                       refcount;
   const char               *sender;
   const char               *path;
   const char               *interface;
   const char               *member;
   Efl_Strbuf              *match;
   Eina_Inlist              *args;
   Eina_Inlist_Sorted_State *state_args;
   Efl_Dbus_Connection         *conn;
   Efl_Dbus_Signal_Cb           cb;
   Efl_Dbus_Connection_Name    *bus;
   const void               *cb_data;
   Eina_Inlist              *cbs_free;
   Efl_Bool                 dangling;
};

struct _Efl_Dbus_Pending
{
   EINA_MAGIC;
   EINA_INLIST;
   Efl_Dbus_Message_Cb  cb;
   const void       *cb_data;
   DBusPendingCall  *dbus_pending;
   Efl_Dbus_Connection *conn;
   const char       *dest;
   const char       *path;
   const char       *interface;
   const char       *method;
   Eina_Inlist      *data;
   Eina_Inlist      *cbs_free;
   Efl_Dbus_Message    *msg_sent;
};

struct _Efl_Dbus_Message_Iter
{
   EINA_MAGIC;
   EINA_INLIST;
   DBusMessageIter dbus_iterator;
   Eina_Inlist *iterators;
   Efl_Bool writable;
};

struct _Efl_Dbus_Message
{
   EINA_MAGIC;
   int          refcount;
   DBusMessage *dbus_msg;
   Efl_Dbus_Message_Iter *iterator;
};

typedef struct _Efl_Dbus_Service_Object Efl_Dbus_Service_Object;
struct _Efl_Dbus_Service_Object
{
   EINA_INLIST;
   Efl_Dbus_Connection *conn;
   const char *path;
   Eina_Hash *interfaces;
   Efl_Strbuf *introspection_data;
   Efl_Bool introspection_dirty;
   Eina_Inlist *data;
   Efl_Dbus_Service_Object *parent;
   Eina_Inlist *children;

   Efl_Bool fallback :1; /* Fallback mechanism flag */

   //ObjectManager data
   Efl_Dbus_Service_Interface *objmanager;
   Eina_List *iface_added;
   Eina_List *iface_removed;
   Core_Idle_Enterer *idle_enterer_iface_changed;
};

struct _Efl_Dbus_Service_Interface
{
   EINA_MAGIC;
   const char *name;
   Eina_Hash *methods;
   const Efl_Dbus_Signal *signals;
   Eina_Array *sign_of_signals;
   Efl_Dbus_Service_Object *obj;
   Eina_Hash *properties;
   Efl_Dbus_Property_Set_Cb set_func;
   Efl_Dbus_Property_Get_Cb get_func;
   Core_Idle_Enterer *idle_enterer_propschanged;
   Eina_Array *props_changed;
   Eina_Array *prop_invalidated;
};

typedef struct _Signal_Argument
{
   EINA_INLIST;
   unsigned short index;
   const char *value;
} Signal_Argument;

#endif

#include "efl_dbus_private.h"
#include "efl_dbus_private_types.h"
#include <dbus/dbus.h>

/* TODO: mempool of Efl_Dbus_Signal_Handler */

#define EFL_DBUS_SIGNAL_HANDLER_CHECK(handler)                        \
  do                                                               \
    {                                                              \
       EINA_SAFETY_ON_NULL_RETURN(handler);                        \
       if (!EINA_MAGIC_CHECK(handler, EFL_DBUS_SIGNAL_HANDLER_MAGIC)) \
         {                                                         \
            EINA_MAGIC_FAIL(handler, EFL_DBUS_SIGNAL_HANDLER_MAGIC);  \
            return;                                                \
         }                                                         \
    }                                                              \
  while (0)

#define EFL_DBUS_SIGNAL_HANDLER_CHECK_RETVAL(handler, retval)         \
  do                                                               \
    {                                                              \
       EINA_SAFETY_ON_NULL_RETURN_VAL(handler, retval);            \
       if (!EINA_MAGIC_CHECK(handler, EFL_DBUS_SIGNAL_HANDLER_MAGIC)) \
         {                                                         \
            EINA_MAGIC_FAIL(handler, EFL_DBUS_SIGNAL_HANDLER_MAGIC);  \
            return retval;                                         \
         }                                                         \
    }                                                              \
  while (0)

static void _efl_dbus_signal_handler_del(Efl_Dbus_Signal_Handler *handler);
static void _efl_dbus_signal_handler_clean(Efl_Dbus_Signal_Handler *handler);

Efl_Bool
efl_dbus_signal_handler_init(void)
{
   return EFL_TRUE;
}

void
efl_dbus_signal_handler_shutdown(void)
{
}

static void
_match_append(Efl_Strbuf *match, const char *key, const char *value)
{
   if (!value) return;

   if ((eina_strbuf_length_get(match) + strlen(",=''") + strlen(key) + strlen(value))
       >= DBUS_MAXIMUM_MATCH_RULE_LENGTH)
     {
        ERR("cannot add match %s='%s' to %s: too long!", key, value,
            eina_strbuf_string_get(match));
        return;
     }

   eina_strbuf_append_printf(match, ",%s='%s'", key, value);
}

static int
_sort_arg(const void *d1, const void *d2)
{
   const Signal_Argument *arg1, *arg2;
   arg1 = d1;
   arg2 = d2;
   return arg1->index - arg2->index;
}

#define ARGX "arg"
EAPI Efl_Bool
efl_dbus_signal_handler_match_extra_vset(Efl_Dbus_Signal_Handler *sh, va_list ap)
{
   const char *key = NULL, *read;
   DBusError err;

   EFL_DBUS_SIGNAL_HANDLER_CHECK_RETVAL(sh, EFL_FALSE);

   dbus_error_init(&err);
   dbus_bus_remove_match(sh->conn->dbus_conn,
                         eina_strbuf_string_get(sh->match), NULL);

   for (read = va_arg(ap, char *); read; read = va_arg(ap, char *))
     {
        Signal_Argument *arg;

        if (!key)
          {
             key = read;
             continue;
          }
        arg = calloc(1, sizeof(Signal_Argument));
        EINA_SAFETY_ON_NULL_GOTO(arg, error);
        if (!strncmp(key, ARGX, strlen(ARGX)))
          {
             int id = atoi(key + strlen(ARGX));
             arg->index = (unsigned short) id;
             arg->value = eina_stringshare_add(read);
             sh->args = eina_inlist_sorted_state_insert(sh->args,
                                                        EINA_INLIST_GET(arg),
                                                        _sort_arg,
                                                        sh->state_args);
             _match_append(sh->match, key, read);
          }
        else
          {
             ERR("%s not supported", key);
             free(arg);
          }
        key = NULL;
     }

   dbus_bus_add_match(sh->conn->dbus_conn,
                      eina_strbuf_string_get(sh->match), NULL);
   return EFL_TRUE;

error:
   dbus_bus_add_match(sh->conn->dbus_conn,
                      eina_strbuf_string_get(sh->match), NULL);
   return EFL_FALSE;
}

EAPI Efl_Bool
efl_dbus_signal_handler_match_extra_set(Efl_Dbus_Signal_Handler *sh, ...)
{
   Efl_Bool ret;
   va_list ap;

   va_start(ap, sh);
   ret = efl_dbus_signal_handler_match_extra_vset(sh, ap);
   va_end(ap);
   return ret;
}

static void _on_handler_of_conn_free(void *data, const void *dead_pointer);

static void
_on_connection_free(void *data, const void *dead_pointer EFL_UNUSED)
{
   Efl_Dbus_Signal_Handler *sh = data;
   efl_dbus_signal_handler_free_cb_del(sh, _on_handler_of_conn_free, sh->conn);
   efl_dbus_signal_handler_del(sh);
}

static void
_on_handler_of_conn_free(void *data, const void *dead_pointer)
{
   Efl_Dbus_Connection *conn = data;
   efl_dbus_connection_free_cb_del(conn, _on_connection_free, dead_pointer);
}

EAPI Efl_Dbus_Signal_Handler *
efl_dbus_signal_handler_add(Efl_Dbus_Connection *conn, const char *sender, const char *path, const char *interface, const char *member, Efl_Dbus_Signal_Cb cb, const void *cb_data)
{
   Efl_Dbus_Signal_Handler *sh;
   sh = _efl_dbus_signal_handler_add(conn, sender, path, interface, member, cb, cb_data);
   EINA_SAFETY_ON_NULL_RETURN_VAL(sh, NULL);
   efl_dbus_connection_free_cb_add(conn, _on_connection_free, sh);
   efl_dbus_signal_handler_free_cb_add(sh, _on_handler_of_conn_free, conn);
   return sh;
}

Efl_Dbus_Signal_Handler *
_efl_dbus_signal_handler_add(Efl_Dbus_Connection *conn, const char *sender, const char *path, const char *interface, const char *member, Efl_Dbus_Signal_Cb cb, const void *cb_data)
{
   Efl_Dbus_Signal_Handler *sh;
   Efl_Strbuf *match;

   EINA_SAFETY_ON_NULL_RETURN_VAL(conn, NULL);
   EINA_SAFETY_ON_NULL_RETURN_VAL(cb, NULL);
   DBG("conn=%p, sender=%s, path=%s, interface=%s, member=%s, cb=%p %p",
       conn, sender, path, interface, member, cb, cb_data);

   sh = calloc(1, sizeof(Efl_Dbus_Signal_Handler));
   EINA_SAFETY_ON_NULL_RETURN_VAL(sh, NULL);

   match = eina_strbuf_new();
   EINA_SAFETY_ON_NULL_GOTO(match, cleanup_create_strbuf);
   eina_strbuf_append(match, "type='signal'");
   _match_append(match, "sender", sender);
   _match_append(match, "path", path);
   _match_append(match, "interface", interface);
   _match_append(match, "member", member);

   dbus_bus_add_match(conn->dbus_conn, eina_strbuf_string_get(match), NULL);

   if (sender)
     {
        sh->bus = efl_dbus_connection_name_get(conn, sender);
        if (!sh->bus) goto cleanup;
        efl_dbus_connection_name_ref(sh->bus);
     }

   sh->cb = cb;
   sh->cb_data = cb_data;
   sh->conn = conn;
   sh->interface = eina_stringshare_add(interface);
   sh->member = eina_stringshare_add(member);
   sh->path = eina_stringshare_add(path);
   sh->sender = eina_stringshare_add(sender);
   sh->match = match;
   sh->refcount = 1;
   sh->dangling = EFL_FALSE;
   sh->state_args = eina_inlist_sorted_state_new();
   EINA_MAGIC_SET(sh, EFL_DBUS_SIGNAL_HANDLER_MAGIC);

   efl_dbus_connection_signal_handler_add(conn, sh);
   return sh;

cleanup:
   eina_strbuf_free(match);
cleanup_create_strbuf:
   free(sh);

   return NULL;
}

static void
_efl_dbus_signal_handler_clean(Efl_Dbus_Signal_Handler *handler)
{
   DBusError err;

   if (handler->dangling) return;
   DBG("clean handler=%p path=%p cb=%p", handler, handler->path, handler->cb);
   dbus_error_init(&err);
   dbus_bus_remove_match(handler->conn->dbus_conn,
                         eina_strbuf_string_get(handler->match), NULL);
   handler->dangling = EFL_TRUE;
}

static void
_efl_dbus_signal_handler_del(Efl_Dbus_Signal_Handler *handler)
{
   Eina_Inlist *list;
   Signal_Argument *arg;
   DBG("handler %p, refcount=%d, conn=%p %s",
       handler, handler->refcount, handler->conn, handler->sender);
   efl_dbus_cbs_free_dispatch(&(handler->cbs_free), handler);
   efl_dbus_connection_signal_handler_del(handler->conn, handler);
   EINA_MAGIC_SET(handler, EINA_MAGIC_NONE);

   /* after cbs_free dispatch these shouldn't exit, error if they do */

   eina_stringshare_replace(&handler->sender, NULL);
   eina_stringshare_replace(&handler->path, NULL);
   eina_stringshare_replace(&handler->interface, NULL);
   eina_stringshare_replace(&handler->member, NULL);
   eina_strbuf_free(handler->match);
   EINA_INLIST_FOREACH_SAFE(handler->args, list, arg)
     {
        eina_stringshare_replace(&arg->value, NULL);
        free(arg);
     }
   eina_inlist_sorted_state_free(handler->state_args);

   if (handler->bus)
     efl_dbus_connection_name_unref(handler->conn, handler->bus);
   free(handler);
}

EAPI Efl_Dbus_Signal_Handler *
efl_dbus_signal_handler_ref(Efl_Dbus_Signal_Handler *handler)
{
   EFL_DBUS_SIGNAL_HANDLER_CHECK_RETVAL(handler, NULL);
   DBG("handler=%p, pre-refcount=%d, match=%s",
       handler, handler->refcount, eina_strbuf_string_get(handler->match));
   handler->refcount++;
   return handler;
}

EAPI void
efl_dbus_signal_handler_unref(Efl_Dbus_Signal_Handler *handler)
{
   EFL_DBUS_SIGNAL_HANDLER_CHECK(handler);
   DBG("handler=%p, pre-refcount=%d, match=%s",
       handler, handler->refcount, eina_strbuf_string_get(handler->match));
   handler->refcount--;
   if (handler->refcount > 0) return;

   _efl_dbus_signal_handler_clean(handler);
   _efl_dbus_signal_handler_del(handler);
}

EAPI void
efl_dbus_signal_handler_del(Efl_Dbus_Signal_Handler *handler)
{
   EFL_DBUS_SIGNAL_HANDLER_CHECK(handler);
   _efl_dbus_signal_handler_clean(handler);
   efl_dbus_signal_handler_unref(handler);
}

EAPI void
efl_dbus_signal_handler_free_cb_add(Efl_Dbus_Signal_Handler *handler, Efl_Dbus_Free_Cb cb, const void *data)
{
   EFL_DBUS_SIGNAL_HANDLER_CHECK(handler);
   EINA_SAFETY_ON_NULL_RETURN(cb);
   handler->cbs_free = efl_dbus_cbs_free_add(handler->cbs_free, cb, data);
}

EAPI void
efl_dbus_signal_handler_free_cb_del(Efl_Dbus_Signal_Handler *handler, Efl_Dbus_Free_Cb cb, const void *data)
{
   EFL_DBUS_SIGNAL_HANDLER_CHECK(handler);
   EINA_SAFETY_ON_NULL_RETURN(cb);
   handler->cbs_free = efl_dbus_cbs_free_del(handler->cbs_free, cb, data);
}

EAPI const char *
efl_dbus_signal_handler_sender_get(const Efl_Dbus_Signal_Handler *handler)
{
   EFL_DBUS_SIGNAL_HANDLER_CHECK_RETVAL(handler, NULL);
   return handler->sender;
}

EAPI const char *
efl_dbus_signal_handler_path_get(const Efl_Dbus_Signal_Handler *handler)
{
   EFL_DBUS_SIGNAL_HANDLER_CHECK_RETVAL(handler, NULL);
   return handler->path;
}

EAPI const char *
efl_dbus_signal_handler_interface_get(const Efl_Dbus_Signal_Handler *handler)
{
   EFL_DBUS_SIGNAL_HANDLER_CHECK_RETVAL(handler, NULL);
   return handler->interface;
}

EAPI const char *
efl_dbus_signal_handler_member_get(const Efl_Dbus_Signal_Handler *handler)
{
   EFL_DBUS_SIGNAL_HANDLER_CHECK_RETVAL(handler, NULL);
   return handler->member;
}

EAPI const char *
efl_dbus_signal_handler_match_get(const Efl_Dbus_Signal_Handler *handler)
{
   EFL_DBUS_SIGNAL_HANDLER_CHECK_RETVAL(handler, NULL);
   return eina_strbuf_string_get(handler->match);
}

EAPI Efl_Dbus_Connection *
efl_dbus_signal_handler_connection_get(const Efl_Dbus_Signal_Handler *handler)
{
   EFL_DBUS_SIGNAL_HANDLER_CHECK_RETVAL(handler, NULL);
   return handler->conn;
}

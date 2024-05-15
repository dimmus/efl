#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#include <locale.h>

#include "Efl_Dbus.h"
#include "Efl_Core.h"

static int                _log_dom = -1;
static Efl_Dbus_Connection *_conn    = NULL;

static Eina_List *_objs           = NULL;
static Eina_List *_proxies        = NULL;
static Eina_List *_efl_dbus_pending = NULL;

#ifdef CRI
#  undef CRI
#endif
#define CRI(...) EINA_LOG_DOM_CRIT(_log_dom, __VA_ARGS__)

#ifdef ERR
#  undef ERR
#endif
#define ERR(...) EINA_LOG_DOM_ERR(_log_dom, __VA_ARGS__)

#ifdef WRN
#  undef WRN
#endif
#define WRN(...) EINA_LOG_DOM_WARN(_log_dom, __VA_ARGS__)

#ifdef DBG
#  undef DBG
#endif
#define DBG(...) EINA_LOG_DOM_DBG(_log_dom, __VA_ARGS__)

static void
_props_changed_hostname(void *data EFL_UNUSED, const Efl_Dbus_Message *msg)
{
    Efl_Dbus_Message_Iter *changed, *entry, *invalidated;
    const char          *iface, *prop;

    if (!efl_dbus_message_arguments_get(msg,
                                      "sa{sv}as",
                                      &iface,
                                      &changed,
                                      &invalidated))
    {
        ERR("Error getting data from properties changed signal.");
        return;
    }

    while (efl_dbus_message_iter_get_and_next(changed, 'e', &entry))
    {
        const void          *key;
        Efl_Dbus_Message_Iter *var;
        if (!efl_dbus_message_iter_arguments_get(entry, "sv", &key, &var))
            continue;
        if (strcmp(key, "Hostname") == 0) goto changed_hostname;
    }

    while (efl_dbus_message_iter_get_and_next(invalidated, 's', &prop))
    {
        if (strcmp(prop, "Hostname") == 0) goto changed_hostname;
    }

    return;

changed_hostname:
    core_event_add(CORE_EVENT_HOSTNAME_CHANGED, NULL, NULL, NULL);
}

static void
_props_changed_timedate(void *data EFL_UNUSED, const Efl_Dbus_Message *msg)
{
    Efl_Dbus_Message_Iter *changed, *entry, *invalidated;
    const char          *iface, *prop;

    if (!efl_dbus_message_arguments_get(msg,
                                      "sa{sv}as",
                                      &iface,
                                      &changed,
                                      &invalidated))
    {
        ERR("Error getting data from properties changed signal.");
        return;
    }

    while (efl_dbus_message_iter_get_and_next(changed, 'e', &entry))
    {
        const void          *key;
        Efl_Dbus_Message_Iter *var;
        if (!efl_dbus_message_iter_arguments_get(entry, "sv", &key, &var))
            continue;
        if (strcmp(key, "Timezone") == 0) goto changed_timedate;
    }

    while (efl_dbus_message_iter_get_and_next(invalidated, 's', &prop))
    {
        if (strcmp(prop, "Timezone") == 0) goto changed_timedate;
    }

    return;

changed_timedate:
    core_event_add(CORE_EVENT_SYSTEM_TIMEDATE_CHANGED, NULL, NULL, NULL);
}

static void
_locale_envs_unset(void)
{
    unsetenv("LC_CTYPE");
    unsetenv("LC_NUMERIC");
    unsetenv("LC_TIME");
    unsetenv("LC_COLLATE");
    unsetenv("LC_MONETARY");
    unsetenv("LC_MESSAGES");
    unsetenv("LC_ALL");
    unsetenv("LC_PAPER");
    unsetenv("LC_NAME");
    unsetenv("LC_ADDRESS");
    unsetenv("LC_TELEPHONE");
    unsetenv("LC_MEASUREMENT");
    unsetenv("LC_IDENTIFICATION");
}

static void
_locale_get(void *data            EFL_UNUSED,
            const Efl_Dbus_Message *msg,
            Efl_Dbus_Pending       *pending)
{
    Efl_Dbus_Message_Iter *variant, *array;
    const char          *errname, *errmsg, *val;

    _efl_dbus_pending = eina_list_remove(_efl_dbus_pending, pending);
    if (efl_dbus_message_error_get(msg, &errname, &errmsg))
    {
        ERR("Message error %s - %s", errname, errmsg);
        goto end;
    }
    if (!efl_dbus_message_arguments_get(msg, "v", &variant))
    {
        ERR("Error getting arguments.");
        goto end;
    }

    if (!efl_dbus_message_iter_get_and_next(variant, 'a', &array))
    {
        ERR("Error getting array.");
        goto end;
    }

    _locale_envs_unset();

    while (efl_dbus_message_iter_get_and_next(array, 's', &val))
    {
        char buf[1024], *value, *type;

        snprintf(buf, sizeof(buf), "%s", val);

        type = buf;

        value = strchr(buf, '=');
        if (!value) continue;
        *value = 0;
        value++;

        setenv(type, value, 1);
    }
    setlocale(LC_ALL, "");

end:
    core_event_add(CORE_EVENT_LOCALE_CHANGED, NULL, NULL, NULL);
}

static void
_props_changed_locale(void *data, const Efl_Dbus_Message *msg)
{
    Efl_Dbus_Proxy        *proxy = data;
    Efl_Dbus_Message_Iter *changed, *entry, *invalidated;
    const char          *iface, *prop;
    Efl_Dbus_Pending      *pend;

    if (!efl_dbus_message_arguments_get(msg,
                                      "sa{sv}as",
                                      &iface,
                                      &changed,
                                      &invalidated))
    {
        ERR("Error getting data from properties changed signal.");
        return;
    }

    while (efl_dbus_message_iter_get_and_next(changed, 'e', &entry))
    {
        const void          *key;
        Efl_Dbus_Message_Iter *var;
        if (!efl_dbus_message_iter_arguments_get(entry, "sv", &key, &var))
            continue;
        if (strcmp(key, "Locale") == 0) goto changed_locale;
    }

    while (efl_dbus_message_iter_get_and_next(invalidated, 's', &prop))
    {
        if (strcmp(prop, "Locale") == 0) goto changed_locale;
    }

    return;

changed_locale:
    pend = efl_dbus_proxy_property_get(proxy, "Locale", _locale_get, NULL);
    _efl_dbus_pending = eina_list_append(_efl_dbus_pending, pend);
}

static Efl_Bool
_property_change_monitor(const char      *name,
                         const char      *path,
                         const char      *iface,
                         Efl_Dbus_Signal_Cb cb)
{
    Efl_Dbus_Object         *o;
    Efl_Dbus_Proxy          *p;
    Efl_Dbus_Signal_Handler *s;

    o = efl_dbus_object_get(_conn, name, path);
    if (!o)
    {
        ERR("could not get object name=%s, path=%s", name, path);
        return EFL_FALSE;
    }

    p = efl_dbus_proxy_get(o, iface);
    if (!p)
    {
        ERR("could not get proxy interface=%s, name=%s, path=%s",
            iface,
            name,
            path);
        efl_dbus_object_unref(o);
        return EFL_FALSE;
    }

    s = efl_dbus_proxy_properties_changed_callback_add(p, cb, p);
    if (!s)
    {
        ERR("could not add signal handler for properties changed for proxy "
            "interface=%s, name=%s, path=%s",
            iface,
            name,
            path);
        efl_dbus_proxy_unref(p);
        efl_dbus_object_unref(o);
        return EFL_FALSE;
    }

    _objs    = eina_list_append(_objs, o);
    _proxies = eina_list_append(_proxies, p);
    return EFL_TRUE;
}

static void         _core_system_systemd_shutdown(void);
static Efl_Bool     _core_system_systemd_init(void);
static unsigned int reseting = 0;

static void
_core_system_systemd_reset(void)
{
    reseting = 1;
    _core_system_systemd_shutdown();
    _core_system_systemd_init();
    reseting = 0;
}

static Efl_Bool
_core_system_systemd_init(void)
{
    efl_dbus_init();
    if (!reseting)
        core_fork_reset_callback_add((Core_Cb)_core_system_systemd_reset,
                                      NULL);

    _log_dom = eina_log_domain_register("core_system_systemd", NULL);
    if (_log_dom < 0)
    {
        EINA_LOG_ERR("Could not register log domain: core_system_systemd");
        goto error;
    }

    _conn = efl_dbus_connection_get(EFL_DBUS_CONNECTION_TYPE_SYSTEM);

    if (!_property_change_monitor("org.freedesktop.hostname1",
                                  "/org/freedesktop/hostname1",
                                  "org.freedesktop.hostname1",
                                  _props_changed_hostname))
        goto error;

    if (!_property_change_monitor("org.freedesktop.timedate1",
                                  "/org/freedesktop/timedate1",
                                  "org.freedesktop.timedate1",
                                  _props_changed_timedate))
        goto error;

    if (!_property_change_monitor("org.freedesktop.locale1",
                                  "/org/freedesktop/locale1",
                                  "org.freedesktop.locale1",
                                  _props_changed_locale))
        goto error;

    DBG("ecore system 'systemd' loaded");
    return EFL_TRUE;

error:
    _core_system_systemd_shutdown();
    return EFL_FALSE;
}

static void
_core_system_systemd_shutdown(void)
{
    Efl_Dbus_Pending *pend;

    DBG("ecore system 'systemd' unloaded");
    if (!reseting)
        core_fork_reset_callback_del((Core_Cb)_core_system_systemd_reset,
                                      NULL);

    while (_proxies)
    {
        efl_dbus_proxy_unref(_proxies->data);
        _proxies = eina_list_remove_list(_proxies, _proxies);
    }

    while (_objs)
    {
        efl_dbus_object_unref(_objs->data);
        _objs = eina_list_remove_list(_objs, _objs);
    }

    if (_conn)
    {
        efl_dbus_connection_unref(_conn);
        _conn = NULL;
    }

    if (_log_dom > 0)
    {
        eina_log_domain_unregister(_log_dom);
        _log_dom = -1;
    }

    EINA_LIST_FREE(_efl_dbus_pending, pend)
    {
        efl_dbus_pending_cancel(pend);
    }

    efl_dbus_shutdown();
}

EINA_MODULE_INIT(_core_system_systemd_init);
EINA_MODULE_SHUTDOWN(_core_system_systemd_shutdown);

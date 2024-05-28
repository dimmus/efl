#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#ifdef NEED_RUN_IN_TREE
// #  include "../../static_libs/buildsystem/buildsystem.h"
#  include "Efl_Core.h"
#endif

#include "core_wayland_private.h"

/* local variables */
static int _efl_core_wayland_init_count = 0;

/* external variables */
Efl_Bool no_session_recovery;
int      _efl_core_wayland_log_dom = -1;

/* public API variables */
EAPI int EFL_CORE_WAYLAND_EVENT_CONNECT                             = 0;
EAPI int EFL_CORE_WAYLAND_EVENT_DISCONNECT                          = 0;
EAPI int EFL_CORE_WAYLAND_EVENT_GLOBAL_ADDED                        = 0;
EAPI int EFL_CORE_WAYLAND_EVENT_GLOBAL_REMOVED                      = 0;
EAPI int EFL_CORE_WAYLAND_EVENT_FOCUS_IN                            = 0;
EAPI int EFL_CORE_WAYLAND_EVENT_FOCUS_OUT                           = 0;
EAPI int EFL_CORE_WAYLAND_EVENT_DND_ENTER                           = 0;
EAPI int EFL_CORE_WAYLAND_EVENT_DND_LEAVE                           = 0;
EAPI int EFL_CORE_WAYLAND_EVENT_DND_MOTION                          = 0;
EAPI int EFL_CORE_WAYLAND_EVENT_DND_DROP                            = 0;
EAPI int EFL_CORE_WAYLAND_EVENT_DND_END                             = 0;
EAPI int EFL_CORE_WAYLAND_EVENT_DATA_SOURCE_END                     = 0;
EAPI int EFL_CORE_WAYLAND_EVENT_DATA_SOURCE_DROP                    = 0;
EAPI int EFL_CORE_WAYLAND_EVENT_DATA_SOURCE_ACTION                  = 0;
EAPI int EFL_CORE_WAYLAND_EVENT_DATA_SOURCE_TARGET                  = 0;
EAPI int EFL_CORE_WAYLAND_EVENT_DATA_SOURCE_SEND                    = 0;
EAPI int EFL_CORE_WAYLAND_EVENT_WINDOW_CONFIGURE                    = 0;
EAPI int EFL_CORE_WAYLAND_EVENT_SYNC_DONE                           = 0;
EAPI int EFL_CORE_WAYLAND_EVENT_OFFER_DATA_READY                    = 0;
EAPI int EFL_CORE_WAYLAND_EVENT_SEAT_NAME_CHANGED                   = 0;
EAPI int EFL_CORE_WAYLAND_EVENT_SEAT_CAPABILITIES_CHANGED           = 0;
EAPI int EFL_CORE_WAYLAND_EVENT_DEVICE_ADDED                        = 0;
EAPI int EFL_CORE_WAYLAND_EVENT_DEVICE_REMOVED                      = 0;
EAPI int EFL_CORE_WAYLAND_EVENT_WINDOW_CONFIGURE_COMPLETE           = 0;
EAPI int EFL_CORE_WAYLAND_EVENT_SEAT_KEYMAP_CHANGED                 = 0;
EAPI int EFL_CORE_WAYLAND_EVENT_SEAT_KEYBOARD_REPEAT_CHANGED        = 0;
EAPI int EFL_CORE_WAYLAND_EVENT_SEAT_SELECTION                      = 0;
EAPI int EFL_CORE_WAYLAND_EVENT_OUTPUT_TRANSFORM                    = 0;
EAPI int EFL_CORE_WAYLAND_EVENT_WINDOW_ROTATE                       = 0;
EAPI int EFL_CORE_WAYLAND_EVENT_WINDOW_ROTATION_CHANGE_PREPARE      = 0;
EAPI int EFL_CORE_WAYLAND_EVENT_WINDOW_ROTATION_CHANGE_PREPARE_DONE = 0;
EAPI int EFL_CORE_WAYLAND_EVENT_WINDOW_ROTATION_CHANGE_REQUEST      = 0;
EAPI int EFL_CORE_WAYLAND_EVENT_WINDOW_ROTATION_CHANGE_DONE         = 0;
EAPI int EFL_CORE_WAYLAND_EVENT_AUX_HINT_ALLOWED                    = 0;
EAPI int EFL_CORE_WAYLAND_EVENT_AUX_HINT_SUPPORTED                  = 0;
EAPI int EFL_CORE_WAYLAND_EVENT_AUX_MESSAGE                         = 0;
EAPI int EFL_CORE_WAYLAND_EVENT_WINDOW_SHOW                         = 0;
EAPI int EFL_CORE_WAYLAND_EVENT_WINDOW_HIDE                         = 0;
EAPI int EFL_CORE_WAYLAND_EVENT_WINDOW_ACTIVATE                     = 0;
EAPI int EFL_CORE_WAYLAND_EVENT_WINDOW_DEACTIVATE                   = 0;
EAPI int EFL_CORE_WAYLAND_EVENT_WINDOW_ICONIFY_STATE_CHANGE         = 0;
EAPI int EFL_CORE_WAYLAND_EVENT_WINDOW_OFFSCREEN                    = 0;
EAPI int EFL_CORE_WAYLAND_EVENT_WINDOW_CREATE                       = 0;
EAPI int EFL_CORE_WAYLAND_EVENT_WINDOW_DESTROY                      = 0;

EAPI int _efl_core_wayland_event_window_www      = -1;
EAPI int _efl_core_wayland_event_window_www_drag = -1;

static Eina_Array *supplied_modules = NULL;
static Eina_Array *local_modules    = NULL;

static Efl_Bool
_efl_core_wayland_surface_modules_init(void)
{
    const char *mod_dir;

#ifdef NEED_RUN_IN_TREE
#  if defined(HAVE_GETUID) && defined(HAVE_GETEUID)
    if (getuid() == geteuid())
#  endif
    {
        char path[PATH_MAX];
        //when running in tree we are ignoring all the settings
        //and just load the intree module that we have build
        if (eina_module_subsystem_lib_exist(path,
                                            sizeof(path),
                                            "core_wayland/engines",
                                            "dmabuf"))
        {
            Eina_Module *local_module = eina_module_new(path);
            EINA_SAFETY_ON_NULL_RETURN_VAL(local_module, EFL_FALSE);

            if (!eina_module_load(local_module))
            {
                ERR("Cannot load module %s", path);
                eina_module_free(local_module);
                local_module = NULL;
                return EFL_FALSE;
            }
            return EFL_TRUE;
        }
    }
#endif
    supplied_modules =
        eina_module_arch_list_get(NULL,
                                  EFL_LIB_DIR "/core_wayland/engines",
                                  EFL_MODULE_ARCH);
    eina_module_list_load(supplied_modules);

    mod_dir = getenv("EFL_CORE_WAYLAND_SURFACE_MODULE_DIR");
    if (mod_dir)
    {
        local_modules =
            eina_module_list_get(NULL, mod_dir, EFL_TRUE, NULL, NULL);
        eina_module_list_load(local_modules);
    }

    if (!supplied_modules && !local_modules) return EFL_FALSE;

    return EFL_TRUE;
}

static void
_efl_core_wayland_surface_modules_unload(void)
{
    eina_module_list_unload(supplied_modules);
    eina_module_list_unload(local_modules);
}

/* public API functions */
EAPI int
efl_core_wayland_init(void)
{
    if (++_efl_core_wayland_init_count != 1) return _efl_core_wayland_init_count;

   /* try to initialize Eina */
    if (!eina_init()) return --_efl_core_wayland_init_count;

   /* try to create Eina logging domain */
    _efl_core_wayland_log_dom =
        eina_log_domain_register("efl_core_wl", EFL_CORE_WAYLAND_DEFAULT_LOG_COLOR);
    if (_efl_core_wayland_log_dom < 0)
    {
        EINA_LOG_ERR("Cannot create a log domain for EFl_Core_Wl");
        goto eina_err;
    }

   /* try to initialize Ecore */
    if (!core_init())
    {
        ERR("Could not initialize Ecore");
        goto ecore_err;
    }

   /* try to initialize Ecore_Event */
    if (!core_event_init())
    {
        ERR("Could not initialize Ecore_Event");
        goto core_event_err;
    }

    if (!_efl_core_wayland_surface_modules_init())
    {
        ERR("Could not load surface modules");
        goto module_load_err;
    }

    /* handle creating new Efl_Core_Wayland event types */
    EFL_CORE_WAYLAND_EVENT_CONNECT                             = core_event_type_new();
    EFL_CORE_WAYLAND_EVENT_DISCONNECT                          = core_event_type_new();
    EFL_CORE_WAYLAND_EVENT_GLOBAL_ADDED                        = core_event_type_new();
    EFL_CORE_WAYLAND_EVENT_GLOBAL_REMOVED                      = core_event_type_new();
    EFL_CORE_WAYLAND_EVENT_FOCUS_IN                            = core_event_type_new();
    EFL_CORE_WAYLAND_EVENT_FOCUS_OUT                           = core_event_type_new();
    EFL_CORE_WAYLAND_EVENT_DND_ENTER                           = core_event_type_new();
    EFL_CORE_WAYLAND_EVENT_DND_LEAVE                           = core_event_type_new();
    EFL_CORE_WAYLAND_EVENT_DND_MOTION                          = core_event_type_new();
    EFL_CORE_WAYLAND_EVENT_DND_DROP                            = core_event_type_new();
    EFL_CORE_WAYLAND_EVENT_DND_END                             = core_event_type_new();
    EFL_CORE_WAYLAND_EVENT_DATA_SOURCE_END                     = core_event_type_new();
    EFL_CORE_WAYLAND_EVENT_DATA_SOURCE_DROP                    = core_event_type_new();
    EFL_CORE_WAYLAND_EVENT_DATA_SOURCE_ACTION                  = core_event_type_new();
    EFL_CORE_WAYLAND_EVENT_DATA_SOURCE_TARGET                  = core_event_type_new();
    EFL_CORE_WAYLAND_EVENT_DATA_SOURCE_SEND                    = core_event_type_new();
    EFL_CORE_WAYLAND_EVENT_WINDOW_CONFIGURE                    = core_event_type_new();
    EFL_CORE_WAYLAND_EVENT_SYNC_DONE                           = core_event_type_new();
    EFL_CORE_WAYLAND_EVENT_OFFER_DATA_READY                    = core_event_type_new();
    EFL_CORE_WAYLAND_EVENT_SEAT_NAME_CHANGED                   = core_event_type_new();
    EFL_CORE_WAYLAND_EVENT_SEAT_CAPABILITIES_CHANGED           = core_event_type_new();
    EFL_CORE_WAYLAND_EVENT_DEVICE_ADDED                        = core_event_type_new();
    EFL_CORE_WAYLAND_EVENT_DEVICE_REMOVED                      = core_event_type_new();
    _efl_core_wayland_event_window_www                          = core_event_type_new();
    _efl_core_wayland_event_window_www_drag                     = core_event_type_new();
    EFL_CORE_WAYLAND_EVENT_WINDOW_CONFIGURE_COMPLETE           = core_event_type_new();
    EFL_CORE_WAYLAND_EVENT_SEAT_KEYMAP_CHANGED                 = core_event_type_new();
    EFL_CORE_WAYLAND_EVENT_SEAT_KEYBOARD_REPEAT_CHANGED        = core_event_type_new();
    EFL_CORE_WAYLAND_EVENT_SEAT_SELECTION                      = core_event_type_new();
    EFL_CORE_WAYLAND_EVENT_OUTPUT_TRANSFORM                    = core_event_type_new();
    EFL_CORE_WAYLAND_EVENT_WINDOW_ROTATE                       = core_event_type_new();
    EFL_CORE_WAYLAND_EVENT_WINDOW_ROTATION_CHANGE_PREPARE      = core_event_type_new();
    EFL_CORE_WAYLAND_EVENT_WINDOW_ROTATION_CHANGE_PREPARE_DONE = core_event_type_new();
    EFL_CORE_WAYLAND_EVENT_WINDOW_ROTATION_CHANGE_REQUEST      = core_event_type_new();
    EFL_CORE_WAYLAND_EVENT_WINDOW_ROTATION_CHANGE_DONE         = core_event_type_new();
    EFL_CORE_WAYLAND_EVENT_AUX_HINT_ALLOWED                    = core_event_type_new();
    EFL_CORE_WAYLAND_EVENT_AUX_HINT_SUPPORTED                  = core_event_type_new();
    EFL_CORE_WAYLAND_EVENT_AUX_MESSAGE                         = core_event_type_new();
    EFL_CORE_WAYLAND_EVENT_WINDOW_SHOW                         = core_event_type_new();
    EFL_CORE_WAYLAND_EVENT_WINDOW_HIDE                         = core_event_type_new();
    EFL_CORE_WAYLAND_EVENT_WINDOW_ACTIVATE                     = core_event_type_new();
    EFL_CORE_WAYLAND_EVENT_WINDOW_DEACTIVATE                   = core_event_type_new();
    EFL_CORE_WAYLAND_EVENT_WINDOW_ICONIFY_STATE_CHANGE         = core_event_type_new();
    EFL_CORE_WAYLAND_EVENT_WINDOW_OFFSCREEN                    = core_event_type_new();
    EFL_CORE_WAYLAND_EVENT_WINDOW_CREATE                       = core_event_type_new();
    EFL_CORE_WAYLAND_EVENT_WINDOW_DESTROY                      = core_event_type_new();

    if (!no_session_recovery)
        no_session_recovery = !!getenv("EFL_NO_WAYLAND_SESSION_RECOVERY");

    printf("-> phase 7 (out: %d )\n", _efl_core_wayland_init_count);
    return _efl_core_wayland_init_count;

module_load_err:
    core_event_shutdown();

core_event_err:
    core_shutdown();

ecore_err:
    eina_log_domain_unregister(_efl_core_wayland_log_dom);
    _efl_core_wayland_log_dom = -1;

eina_err:
    eina_shutdown();
    return --_efl_core_wayland_init_count;
}

EAPI int
efl_core_wayland_shutdown(void)
{
    if (_efl_core_wayland_init_count < 1)
    {
        EINA_LOG_ERR("Efl_Core_Wayland shutdown called without Efl_Core_Wayland Init");
        return 0;
    }

    if (--_efl_core_wayland_init_count != 0) return _efl_core_wayland_init_count;

   /* reset events */
    core_event_type_flush(EFL_CORE_WAYLAND_EVENT_CONNECT,
                          EFL_CORE_WAYLAND_EVENT_DISCONNECT,
                          EFL_CORE_WAYLAND_EVENT_GLOBAL_ADDED,
                          EFL_CORE_WAYLAND_EVENT_GLOBAL_REMOVED,
                          EFL_CORE_WAYLAND_EVENT_FOCUS_IN,
                          EFL_CORE_WAYLAND_EVENT_FOCUS_OUT,
                          EFL_CORE_WAYLAND_EVENT_DND_ENTER,
                          EFL_CORE_WAYLAND_EVENT_DND_LEAVE,
                          EFL_CORE_WAYLAND_EVENT_DND_MOTION,
                          EFL_CORE_WAYLAND_EVENT_DND_DROP,
                          EFL_CORE_WAYLAND_EVENT_DND_END,
                          EFL_CORE_WAYLAND_EVENT_DATA_SOURCE_END,
                          EFL_CORE_WAYLAND_EVENT_DATA_SOURCE_DROP,
                          EFL_CORE_WAYLAND_EVENT_DATA_SOURCE_ACTION,
                          EFL_CORE_WAYLAND_EVENT_DATA_SOURCE_TARGET,
                          EFL_CORE_WAYLAND_EVENT_DATA_SOURCE_SEND,
                          EFL_CORE_WAYLAND_EVENT_WINDOW_CONFIGURE,
                          EFL_CORE_WAYLAND_EVENT_SYNC_DONE,
                          EFL_CORE_WAYLAND_EVENT_OFFER_DATA_READY,
                          EFL_CORE_WAYLAND_EVENT_SEAT_NAME_CHANGED,
                          EFL_CORE_WAYLAND_EVENT_SEAT_CAPABILITIES_CHANGED,
                          EFL_CORE_WAYLAND_EVENT_DEVICE_ADDED,
                          EFL_CORE_WAYLAND_EVENT_DEVICE_REMOVED,
                          EFL_CORE_WAYLAND_EVENT_WINDOW_CONFIGURE_COMPLETE,
                          EFL_CORE_WAYLAND_EVENT_SEAT_KEYMAP_CHANGED,
                          EFL_CORE_WAYLAND_EVENT_SEAT_KEYBOARD_REPEAT_CHANGED,
                          EFL_CORE_WAYLAND_EVENT_SEAT_SELECTION,
                          EFL_CORE_WAYLAND_EVENT_OUTPUT_TRANSFORM,
                          EFL_CORE_WAYLAND_EVENT_WINDOW_ROTATE,
                          EFL_CORE_WAYLAND_EVENT_WINDOW_ROTATION_CHANGE_PREPARE,
                          EFL_CORE_WAYLAND_EVENT_WINDOW_ROTATION_CHANGE_PREPARE_DONE,
                          EFL_CORE_WAYLAND_EVENT_WINDOW_ROTATION_CHANGE_REQUEST,
                          EFL_CORE_WAYLAND_EVENT_WINDOW_ROTATION_CHANGE_DONE,
                          EFL_CORE_WAYLAND_EVENT_AUX_HINT_ALLOWED,
                          EFL_CORE_WAYLAND_EVENT_AUX_HINT_SUPPORTED,
                          EFL_CORE_WAYLAND_EVENT_AUX_MESSAGE,
                          EFL_CORE_WAYLAND_EVENT_WINDOW_SHOW,
                          EFL_CORE_WAYLAND_EVENT_WINDOW_HIDE,
                          EFL_CORE_WAYLAND_EVENT_WINDOW_ACTIVATE,
                          EFL_CORE_WAYLAND_EVENT_WINDOW_DEACTIVATE,
                          EFL_CORE_WAYLAND_EVENT_WINDOW_ICONIFY_STATE_CHANGE,
                          EFL_CORE_WAYLAND_EVENT_WINDOW_OFFSCREEN,
                          EFL_CORE_WAYLAND_EVENT_WINDOW_CREATE,
                          EFL_CORE_WAYLAND_EVENT_WINDOW_DESTROY);

   /* shutdown Ecore_Event */
    core_event_shutdown();

   /* shutdown Ecore */
    core_shutdown();

   /* unregister logging domain */
    eina_log_domain_unregister(_efl_core_wayland_log_dom);
    _efl_core_wayland_log_dom = -1;

    _efl_core_wayland_surface_modules_unload();

   /* shutdown eina */
    eina_shutdown();

    return _efl_core_wayland_init_count;
}

EAPI void
efl_core_wayland_session_recovery_disable(void)
{
    no_session_recovery = EFL_TRUE;
}

#ifdef HAVE_CONFIG_H
#  include "efl_config.h"
#endif

#ifdef NEED_RUN_IN_TREE
// #  include "../../static_libs/buildsystem/buildsystem.h"
#  include "Efl_Core.h"
#endif

#include "core_wayland_private.h"

/* local variables */
static int _ecore_wl_init_count = 0;

/* external variables */
Efl_Bool no_session_recovery;
int      _ecore_wl_log_dom = -1;

/* public API variables */
EAPI int ECORE_WL2_EVENT_CONNECT                             = 0;
EAPI int ECORE_WL2_EVENT_DISCONNECT                          = 0;
EAPI int ECORE_WL2_EVENT_GLOBAL_ADDED                        = 0;
EAPI int ECORE_WL2_EVENT_GLOBAL_REMOVED                      = 0;
EAPI int ECORE_WL2_EVENT_FOCUS_IN                            = 0;
EAPI int ECORE_WL2_EVENT_FOCUS_OUT                           = 0;
EAPI int ECORE_WL2_EVENT_DND_ENTER                           = 0;
EAPI int ECORE_WL2_EVENT_DND_LEAVE                           = 0;
EAPI int ECORE_WL2_EVENT_DND_MOTION                          = 0;
EAPI int ECORE_WL2_EVENT_DND_DROP                            = 0;
EAPI int ECORE_WL2_EVENT_DND_END                             = 0;
EAPI int ECORE_WL2_EVENT_DATA_SOURCE_END                     = 0;
EAPI int ECORE_WL2_EVENT_DATA_SOURCE_DROP                    = 0;
EAPI int ECORE_WL2_EVENT_DATA_SOURCE_ACTION                  = 0;
EAPI int ECORE_WL2_EVENT_DATA_SOURCE_TARGET                  = 0;
EAPI int ECORE_WL2_EVENT_DATA_SOURCE_SEND                    = 0;
EAPI int ECORE_WL2_EVENT_WINDOW_CONFIGURE                    = 0;
EAPI int ECORE_WL2_EVENT_SYNC_DONE                           = 0;
EAPI int ECORE_WL2_EVENT_OFFER_DATA_READY                    = 0;
EAPI int ECORE_WL2_EVENT_SEAT_NAME_CHANGED                   = 0;
EAPI int ECORE_WL2_EVENT_SEAT_CAPABILITIES_CHANGED           = 0;
EAPI int ECORE_WL2_EVENT_DEVICE_ADDED                        = 0;
EAPI int ECORE_WL2_EVENT_DEVICE_REMOVED                      = 0;
EAPI int ECORE_WL2_EVENT_WINDOW_CONFIGURE_COMPLETE           = 0;
EAPI int ECORE_WL2_EVENT_SEAT_KEYMAP_CHANGED                 = 0;
EAPI int ECORE_WL2_EVENT_SEAT_KEYBOARD_REPEAT_CHANGED        = 0;
EAPI int ECORE_WL2_EVENT_SEAT_SELECTION                      = 0;
EAPI int ECORE_WL2_EVENT_OUTPUT_TRANSFORM                    = 0;
EAPI int ECORE_WL2_EVENT_WINDOW_ROTATE                       = 0;
EAPI int ECORE_WL2_EVENT_WINDOW_ROTATION_CHANGE_PREPARE      = 0;
EAPI int ECORE_WL2_EVENT_WINDOW_ROTATION_CHANGE_PREPARE_DONE = 0;
EAPI int ECORE_WL2_EVENT_WINDOW_ROTATION_CHANGE_REQUEST      = 0;
EAPI int ECORE_WL2_EVENT_WINDOW_ROTATION_CHANGE_DONE         = 0;
EAPI int ECORE_WL2_EVENT_AUX_HINT_ALLOWED                    = 0;
EAPI int ECORE_WL2_EVENT_AUX_HINT_SUPPORTED                  = 0;
EAPI int ECORE_WL2_EVENT_AUX_MESSAGE                         = 0;
EAPI int ECORE_WL2_EVENT_WINDOW_SHOW                         = 0;
EAPI int ECORE_WL2_EVENT_WINDOW_HIDE                         = 0;
EAPI int ECORE_WL2_EVENT_WINDOW_ACTIVATE                     = 0;
EAPI int ECORE_WL2_EVENT_WINDOW_DEACTIVATE                   = 0;
EAPI int ECORE_WL2_EVENT_WINDOW_ICONIFY_STATE_CHANGE         = 0;
EAPI int ECORE_WL2_EVENT_WINDOW_OFFSCREEN                    = 0;
EAPI int ECORE_WL2_EVENT_WINDOW_CREATE                       = 0;
EAPI int ECORE_WL2_EVENT_WINDOW_DESTROY                      = 0;

EAPI int _ecore_wl_event_window_www      = -1;
EAPI int _ecore_wl_event_window_www_drag = -1;

static Eina_Array *supplied_modules = NULL;
static Eina_Array *local_modules    = NULL;

static Efl_Bool
_ecore_wl_surface_modules_init(void)
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
    supplied_modules = eina_module_arch_list_get(NULL,
                                                 EFL_LIB_DIR "/core_wayland/engines",
                                                 EFL_MODULE_ARCH);
    eina_module_list_load(supplied_modules);

    mod_dir = getenv("ECORE_WL2_SURFACE_MODULE_DIR");
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
_ecore_wl_surface_modules_unload(void)
{
    eina_module_list_unload(supplied_modules);
    eina_module_list_unload(local_modules);
}

/* public API functions */
EAPI int
ecore_wl_init(void)
{
    printf("WL-TEST: -> phase 0: %d", _ecore_wl_init_count);
    if (++_ecore_wl_init_count != 1) return _ecore_wl_init_count;
    printf("-> phase 1 ");

   /* try to initialize Eina */
    if (!eina_init()) return --_ecore_wl_init_count;
    printf("-> phase 2 %d", _ecore_wl_init_count);

   /* try to create Eina logging domain */
    _ecore_wl_log_dom =
        eina_log_domain_register("efl_core_wl", ECORE_WL2_DEFAULT_LOG_COLOR);
    if (_ecore_wl_log_dom < 0)
    {
        EINA_LOG_ERR("Cannot create a log domain for EFl_Core_Wl");
        goto eina_err;
    }
    printf("-> phase 3 ");

   /* try to initialize Ecore */
    if (!core_init())
    {
        ERR("Could not initialize Ecore");
        goto ecore_err;
    }
    printf("-> phase 4 ");

   /* try to initialize Ecore_Event */
    if (!core_event_init())
    {
        ERR("Could not initialize Ecore_Event");
        goto core_event_err;
    }
    printf("-> phase 5 ");

    if (!_ecore_wl_surface_modules_init())
    {
        ERR("Could not load surface modules");
        goto module_load_err;
    }
    printf("-> phase 6 ");

   /* handle creating new Ecore_Wl event types */
    ECORE_WL2_EVENT_CONNECT                        = core_event_type_new();
    ECORE_WL2_EVENT_DISCONNECT                     = core_event_type_new();
    ECORE_WL2_EVENT_GLOBAL_ADDED                   = core_event_type_new();
    ECORE_WL2_EVENT_GLOBAL_REMOVED                 = core_event_type_new();
    ECORE_WL2_EVENT_FOCUS_IN                       = core_event_type_new();
    ECORE_WL2_EVENT_FOCUS_OUT                      = core_event_type_new();
    ECORE_WL2_EVENT_DND_ENTER                      = core_event_type_new();
    ECORE_WL2_EVENT_DND_LEAVE                      = core_event_type_new();
    ECORE_WL2_EVENT_DND_MOTION                     = core_event_type_new();
    ECORE_WL2_EVENT_DND_DROP                       = core_event_type_new();
    ECORE_WL2_EVENT_DND_END                        = core_event_type_new();
    ECORE_WL2_EVENT_DATA_SOURCE_END                = core_event_type_new();
    ECORE_WL2_EVENT_DATA_SOURCE_DROP               = core_event_type_new();
    ECORE_WL2_EVENT_DATA_SOURCE_ACTION             = core_event_type_new();
    ECORE_WL2_EVENT_DATA_SOURCE_TARGET             = core_event_type_new();
    ECORE_WL2_EVENT_DATA_SOURCE_SEND               = core_event_type_new();
    ECORE_WL2_EVENT_WINDOW_CONFIGURE               = core_event_type_new();
    ECORE_WL2_EVENT_SYNC_DONE                      = core_event_type_new();
    ECORE_WL2_EVENT_OFFER_DATA_READY               = core_event_type_new();
    ECORE_WL2_EVENT_SEAT_NAME_CHANGED              = core_event_type_new();
    ECORE_WL2_EVENT_SEAT_CAPABILITIES_CHANGED      = core_event_type_new();
    ECORE_WL2_EVENT_DEVICE_ADDED                   = core_event_type_new();
    ECORE_WL2_EVENT_DEVICE_REMOVED                 = core_event_type_new();
    _ecore_wl_event_window_www                     = core_event_type_new();
    _ecore_wl_event_window_www_drag                = core_event_type_new();
    ECORE_WL2_EVENT_WINDOW_CONFIGURE_COMPLETE      = core_event_type_new();
    ECORE_WL2_EVENT_SEAT_KEYMAP_CHANGED            = core_event_type_new();
    ECORE_WL2_EVENT_SEAT_KEYBOARD_REPEAT_CHANGED   = core_event_type_new();
    ECORE_WL2_EVENT_SEAT_SELECTION                 = core_event_type_new();
    ECORE_WL2_EVENT_OUTPUT_TRANSFORM               = core_event_type_new();
    ECORE_WL2_EVENT_WINDOW_ROTATE                  = core_event_type_new();
    ECORE_WL2_EVENT_WINDOW_ROTATION_CHANGE_PREPARE = core_event_type_new();
    ECORE_WL2_EVENT_WINDOW_ROTATION_CHANGE_PREPARE_DONE =
        core_event_type_new();
    ECORE_WL2_EVENT_WINDOW_ROTATION_CHANGE_REQUEST = core_event_type_new();
    ECORE_WL2_EVENT_WINDOW_ROTATION_CHANGE_DONE    = core_event_type_new();
    ECORE_WL2_EVENT_AUX_HINT_ALLOWED               = core_event_type_new();
    ECORE_WL2_EVENT_AUX_HINT_SUPPORTED             = core_event_type_new();
    ECORE_WL2_EVENT_AUX_MESSAGE                    = core_event_type_new();
    ECORE_WL2_EVENT_WINDOW_SHOW                    = core_event_type_new();
    ECORE_WL2_EVENT_WINDOW_HIDE                    = core_event_type_new();
    ECORE_WL2_EVENT_WINDOW_ACTIVATE                = core_event_type_new();
    ECORE_WL2_EVENT_WINDOW_DEACTIVATE              = core_event_type_new();
    ECORE_WL2_EVENT_WINDOW_ICONIFY_STATE_CHANGE    = core_event_type_new();
    ECORE_WL2_EVENT_WINDOW_OFFSCREEN               = core_event_type_new();
    ECORE_WL2_EVENT_WINDOW_CREATE                  = core_event_type_new();
    ECORE_WL2_EVENT_WINDOW_DESTROY                 = core_event_type_new();

    if (!no_session_recovery)
        no_session_recovery = !!getenv("EFL_NO_WAYLAND_SESSION_RECOVERY");

    printf("-> phase 7 (out: %d )\n", _ecore_wl_init_count);
    return _ecore_wl_init_count;

module_load_err:
    core_event_shutdown();

core_event_err:
    core_shutdown();

ecore_err:
    eina_log_domain_unregister(_ecore_wl_log_dom);
    _ecore_wl_log_dom = -1;

eina_err:
    eina_shutdown();
    return --_ecore_wl_init_count;
}

EAPI int
ecore_wl_shutdown(void)
{
    if (_ecore_wl_init_count < 1)
    {
        EINA_LOG_ERR("Ecore_Wl shutdown called without Ecore_Wl Init");
        return 0;
    }

    if (--_ecore_wl_init_count != 0) return _ecore_wl_init_count;

   /* reset events */
    core_event_type_flush(ECORE_WL2_EVENT_CONNECT,
                           ECORE_WL2_EVENT_DISCONNECT,
                           ECORE_WL2_EVENT_GLOBAL_ADDED,
                           ECORE_WL2_EVENT_GLOBAL_REMOVED,
                           ECORE_WL2_EVENT_FOCUS_IN,
                           ECORE_WL2_EVENT_FOCUS_OUT,
                           ECORE_WL2_EVENT_DND_ENTER,
                           ECORE_WL2_EVENT_DND_LEAVE,
                           ECORE_WL2_EVENT_DND_MOTION,
                           ECORE_WL2_EVENT_DND_DROP,
                           ECORE_WL2_EVENT_DND_END,
                           ECORE_WL2_EVENT_DATA_SOURCE_END,
                           ECORE_WL2_EVENT_DATA_SOURCE_DROP,
                           ECORE_WL2_EVENT_DATA_SOURCE_ACTION,
                           ECORE_WL2_EVENT_DATA_SOURCE_TARGET,
                           ECORE_WL2_EVENT_DATA_SOURCE_SEND,
                           ECORE_WL2_EVENT_WINDOW_CONFIGURE,
                           ECORE_WL2_EVENT_SYNC_DONE,
                           ECORE_WL2_EVENT_OFFER_DATA_READY,
                           ECORE_WL2_EVENT_SEAT_NAME_CHANGED,
                           ECORE_WL2_EVENT_SEAT_CAPABILITIES_CHANGED,
                           ECORE_WL2_EVENT_DEVICE_ADDED,
                           ECORE_WL2_EVENT_DEVICE_REMOVED,
                           ECORE_WL2_EVENT_WINDOW_CONFIGURE_COMPLETE,
                           ECORE_WL2_EVENT_SEAT_KEYMAP_CHANGED,
                           ECORE_WL2_EVENT_SEAT_KEYBOARD_REPEAT_CHANGED,
                           ECORE_WL2_EVENT_SEAT_SELECTION,
                           ECORE_WL2_EVENT_OUTPUT_TRANSFORM,
                           ECORE_WL2_EVENT_WINDOW_ROTATE,
                           ECORE_WL2_EVENT_WINDOW_ROTATION_CHANGE_PREPARE,
                           ECORE_WL2_EVENT_WINDOW_ROTATION_CHANGE_PREPARE_DONE,
                           ECORE_WL2_EVENT_WINDOW_ROTATION_CHANGE_REQUEST,
                           ECORE_WL2_EVENT_WINDOW_ROTATION_CHANGE_DONE,
                           ECORE_WL2_EVENT_AUX_HINT_ALLOWED,
                           ECORE_WL2_EVENT_AUX_HINT_SUPPORTED,
                           ECORE_WL2_EVENT_AUX_MESSAGE,
                           ECORE_WL2_EVENT_WINDOW_SHOW,
                           ECORE_WL2_EVENT_WINDOW_HIDE,
                           ECORE_WL2_EVENT_WINDOW_ACTIVATE,
                           ECORE_WL2_EVENT_WINDOW_DEACTIVATE,
                           ECORE_WL2_EVENT_WINDOW_ICONIFY_STATE_CHANGE,
                           ECORE_WL2_EVENT_WINDOW_OFFSCREEN,
                           ECORE_WL2_EVENT_WINDOW_CREATE,
                           ECORE_WL2_EVENT_WINDOW_DESTROY);

   /* shutdown Ecore_Event */
    core_event_shutdown();

   /* shutdown Ecore */
    core_shutdown();

   /* unregister logging domain */
    eina_log_domain_unregister(_ecore_wl_log_dom);
    _ecore_wl_log_dom = -1;

    _ecore_wl_surface_modules_unload();

   /* shutdown eina */
    eina_shutdown();

    return _ecore_wl_init_count;
}

EAPI void
ecore_wl_session_recovery_disable(void)
{
    no_session_recovery = EFL_TRUE;
}

EWAPI const Efl_Event_Description _CORE_EXE_EVENT_DATA_GET =
    EFL_EVENT_DESCRIPTION("data,get");
EWAPI const Efl_Event_Description _CORE_EXE_EVENT_DATA_ERROR =
    EFL_EVENT_DESCRIPTION("data,error");

void _core_exe_command_set(Eo             *obj,
                            Core_Exe_Data *pd,
                            const char     *exe_cmd,
                            Core_Exe_Flags flags);

EOAPI EFL_VOID_FUNC_BODYV(Core_obj_exe_command_set,
                          EFL_FUNC_CALL(exe_cmd, flags),
                          const char     *exe_cmd,
                          Core_Exe_Flags flags);

void _core_exe_command_get(const Eo        *obj,
                            Core_Exe_Data  *pd,
                            const char     **exe_cmd,
                            Core_Exe_Flags *flags);

EOAPI EFL_VOID_FUNC_BODYV_CONST(Core_obj_exe_command_get,
                                EFL_FUNC_CALL(exe_cmd, flags),
                                const char     **exe_cmd,
                                Core_Exe_Flags *flags);

void _core_exe_efl_object_destructor(Eo *obj, Core_Exe_Data *pd);

Efl_Object *_core_exe_efl_object_finalize(Eo *obj, Core_Exe_Data *pd);

void _core_exe_efl_control_suspend_set(Eo             *obj,
                                        Core_Exe_Data *pd,
                                        Efl_Bool        suspend);

static Efl_Bool
_core_exe_class_initializer(Efl_Class *klass)
{
    const Efl_Object_Ops *opsp = NULL;

    const Efl_Object_Property_Reflection_Ops *ropsp = NULL;

#ifndef CORE_EXE_EXTRA_OPS
#  define CORE_EXE_EXTRA_OPS
#endif

    EFL_OPS_DEFINE(
        ops,
        EFL_OBJECT_OP_FUNC(Core_obj_exe_command_set, _core_exe_command_set),
        EFL_OBJECT_OP_FUNC(Core_obj_exe_command_get, _core_exe_command_get),
        EFL_OBJECT_OP_FUNC(efl_destructor, _core_exe_efl_object_destructor),
        EFL_OBJECT_OP_FUNC(efl_finalize, _core_exe_efl_object_finalize),
        EFL_OBJECT_OP_FUNC(efl_control_suspend_set,
                           _core_exe_efl_control_suspend_set),
        CORE_EXE_EXTRA_OPS);
    opsp = &ops;

    return efl_class_functions_set(klass, opsp, ropsp);
}

static const Efl_Class_Description _core_exe_class_desc = {
    EO_VERSION,
    "Ecore.Exe",
    EFL_CLASS_TYPE_REGULAR,
    sizeof(Core_Exe_Data),
    _core_exe_class_initializer,
    NULL,
    NULL
};

EFL_DEFINE_CLASS(core_exe_class_get,
                 &_core_exe_class_desc,
                 EFL_OBJECT_CLASS,
                 EFL_CONTROL_INTERFACE,
                 NULL);

#ifndef hook_hh
#define hook_hh

#include "emulator.hh"

enum hookType
{
  none = 0,
    start = 1,
    replace = 2,
};

enum hookFlag
{
    generic = 0,
    debug = 1,
    fixed = 2,
};

template <uint32_t address = 0x00>
static inline void hook(void (*function)(), enum hookType type = hookType::start, enum hookFlag flags = hookFlag::fixed)
{
    std::string s = std::to_string(uint64_t(function));
    emulator::createHook(address, s.c_str(), type, flags, (void (*)())function);
}

struct hookFunction
{
#if platform == platformGCN
    static const uint32_t fn_vDynamicsBaseMechanics_start = 0x8007e9d8;
    
    static const uint32_t fn_vBusSpeed_start = 0x8009f1a8;
    static const uint32_t fn_vBusSpeed2_start = 0x8009f1ac;
    //
    static const uint32_t fn_vEngine_start = 0x80037078;
    static const uint32_t fn_vEngine_end = 0x8003748c;
    //
    //    hook_create(0x80035bd4, "hook_pre_frame", HOOK_TYPE_START, HOOK_FLAG_FIXED, &hook_s_pre_frame);
    //hook_create(0x80035edc, "hook_post_frame", HOOK_TYPE_REPLACE, HOOK_FLAG_FIXED, &hook_r_post_frame);
    //
    //hook_create(0x80037178, "hook_treat_dynhierarchy", HOOK_TYPE_START, HOOK_FLAG_FIXED, &hook_s_treat_dynamic_hierarchy);
    
    //hook_create(0x80036174, "hook_display_all", HOOK_TYPE_REPLACE, HOOK_FLAG_FIXED, &hook_r_display_all);
    //hook_create(0x80054498, "hook_display_fix", HOOK_TYPE_REPLACE, HOOK_FLAG_FIXED, &hook_r_display_fix);
    
    //hook_create(0x8007e9d8, "hook_s_dynamics_base_mechanics", HOOK_TYPE_START, HOOK_FLAG_FIXED, &hook_s_dynamics_base_mechanics);
    
    //hook_create(0x80034ab0, "hook_s_engine_clock", HOOK_TYPE_START, HOOK_FLAG_FIXED, &hook_s_save_engine_clock);
#endif
};

#endif /* hook_hh */

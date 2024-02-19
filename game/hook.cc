/************************************************************
 # hook.c: Unsafe function hooks
 ************************************************************/

#include "memory.h"
#include "game.hh"
#include "hook.hh"

typedef void (*fn_hook_create)(uint32_t addr, const char *func_name, int type, int flag, void (*callback)(void));
fn_hook_create createHook = nullptr;


/* PowerPC state */
static uint32_t *pc = NULL;
static uint32_t *npc = NULL;
static uint32_t *gpr = NULL;
static uint32_t *lr = NULL;

struct processorState ppcState;

static void sblr()
{
    *npc = *lr;
}

static void hook_s_bus_speed()
{
    *(gpr + 3) /= 4;
    sblr();
}

static void hook_s_pre_frame() /* before one frame */
{
    //printf("frame\n");
}


static int a = 10;

static void hook_r_post_frame() /* after one frame */
{
    uint32_t sp = *(gpr + 1) & 0x7FFFFFFF; /* sp = r1 */
    *lr = *gpr  = host_byteorder_32(*(uint32_t*)(memoryBase + sp + 0x24)); /* lr = r0 */
    *(gpr + 29) = host_byteorder_32(*(uint32_t*)(memoryBase + sp + 0x10));
    *(gpr + 29) = host_byteorder_32(*(uint32_t*)(memoryBase + sp + 0x14));
    *(gpr + 30) = host_byteorder_32(*(uint32_t*)(memoryBase + sp + 0x18));
    *(gpr + 31) = host_byteorder_32(*(uint32_t*)(memoryBase + sp + 0x1C));
    *(gpr + 1) += 32; /* SP alignment */
    sblr();
    
    if (a--)
    {
        memset(gpr, 0, sizeof(uint32_t) * 32);
        
        *(gpr + 0) = 0x80035edc;
        *(gpr + 1) = 0x803c86f8;
        *(gpr + 2) = 0x805e19e0;
        *(gpr + 3) = 0x00000001;
        *(gpr + 4) = 0xcc010000;
        *(gpr + 5) = 0x80272bc8;
        *(gpr + 6) = 0x410054ad;
        *(gpr + 7) = 0x40000017;
        *(gpr + 8) = 0x8081ada4;
        *(gpr + 9) = 0x00000002;
        *(gpr + 10) = 0x00000156;
        *(gpr + 11) = 0x00000020;
        *(gpr + 12) = 0x8007326c;
        *(gpr + 13) = 0x805def80;
        *(gpr + 29) = 0x00000001;
        *(gpr + 30) = 0x803e7c0c;
        
        //printf("cycle\n");
        
        *lr = 0x80035edc;
        //CTR = 0x8007326c;
        *npc = 0x80035bd4; //0x80035edc;
    }
    else
    {
        a = 10;
    }
}

static void hook_s_pre_engine() /* prior to engine execution */
{
    //printf("engine!\n");
    srand48(time(NULL));
    //input.stick.main.x->analogValue = game_byteorder_f32(drand48()*200.0f-100);
}

static int parity = 0;

static void hook_r_post_engine() /* post engine execution */
{
    uint32_t sp = *(gpr + 1) & 0x7FFFFFFF; /* sp = r1 */
    *lr = *gpr  = host_byteorder_32(*(uint32_t*)(memoryBase + sp + 0xD4)); /* lr = r0 */
    *(gpr + 29) = host_byteorder_32(*(uint32_t*)(memoryBase + sp + 0xC4));
    *(gpr + 30) = host_byteorder_32(*(uint32_t*)(memoryBase + sp + 0xC8));
    *(gpr + 31) = host_byteorder_32(*(uint32_t*)(memoryBase + sp + 0xCC));
    *(gpr + 1) += 208; /* SP alignment */
    sblr();
    
    
    
    //stVector3D pos = fnActorGetPosition(actor_rayman);
    //printf("%.2f %.2f %.2f\n", pos.x, pos.y, pos.z);
    
    //printf("engine!\n");
    
    if (a--)
    {
        //printf("cycle %d\n", a);
        /* Execute another engine cycle. */
        
        memset(gpr, 0, sizeof(uint32_t) * 32);
        
        *(gpr + 1) = 0x803c86f8;
        *(gpr + 2) = 0x805e19e0;
        *(gpr + 4) = 0x803e0000;
        *(gpr + 5) = 0x0000b032;
        *(gpr + 9) = 0x00000003;
        *(gpr + 10) = 0x80225f80;
        *(gpr + 11) = 0x00000020;
        *(gpr + 12) = 0x80037078;
        *(gpr + 13) = 0x805def80;
        *(gpr + 30) = 0x803e7c0c;

        *lr = 0x80035cb8;
        //*ctr = 0x80037078;
        //PowerPC::ppcState.cr.Set(0x20000088);
        *npc = 0x80037078;//0x80035bd4;
    }
    else
    {
        a = 10;
    }
}

static void hook_s_treat_dynamic_hierarchy()
{
    printf("treat\n");
}

static void hook_r_display_all()
{
    *npc = *lr;
}

static void hook_r_display_fix()
{
    sblr();
}

static void hook_s_dynamics_base_mechanics(processorState *s = &ppcState)
{
    //printf("%X\n", *(gpr + 31));
   // if (*(s->gpr + 4) == 0x80BF0C0C)
    printf("base mechanics\n");
}

static void hook_s_save_engine_clock()
{
    *(uint32_t*)(memoryBase + 0x05d8518) = game_byteorder_32(1);
    printf("save clock\n");
}

void hle_hook_functions(void *s)
{
    createHook = (fn_hook_create)s;
    
    hook<0x8007e9d8>(hook_s_dynamics_base_mechanics);
    
//    hook_create(0x8009f1a8, "hook_bus_speed", HOOK_TYPE_REPLACE, HOOK_FLAG_FIXED, &hook_s_bus_speed);
//    hook_create(0x8009f1ac, "hook_bus_speed2", HOOK_TYPE_REPLACE, HOOK_FLAG_FIXED, &hook_s_bus_speed);
//    
//    hook_create(0x80037078, "hook_pre_engine", HOOK_TYPE_START, HOOK_FLAG_FIXED, &hook_s_pre_engine);
    //hook_create(0x8003748c, "hook_post_engine", HOOK_TYPE_REPLACE, HOOK_FLAG_FIXED, &hook_r_post_engine);
//    
//    hook_create(0x80035bd4, "hook_pre_frame", HOOK_TYPE_START, HOOK_FLAG_FIXED, &hook_s_pre_frame);
    //hook_create(0x80035edc, "hook_post_frame", HOOK_TYPE_REPLACE, HOOK_FLAG_FIXED, &hook_r_post_frame);
//    
    //hook_create(0x80037178, "hook_treat_dynhierarchy", HOOK_TYPE_START, HOOK_FLAG_FIXED, &hook_s_treat_dynamic_hierarchy);
    
    //hook_create(0x80036174, "hook_display_all", HOOK_TYPE_REPLACE, HOOK_FLAG_FIXED, &hook_r_display_all);
    //hook_create(0x80054498, "hook_display_fix", HOOK_TYPE_REPLACE, HOOK_FLAG_FIXED, &hook_r_display_fix);
    
    //hook_create(0x8007e9d8, "hook_s_dynamics_base_mechanics", HOOK_TYPE_START, HOOK_FLAG_FIXED, &hook_s_dynamics_base_mechanics);
    
    //hook_create(0x80034ab0, "hook_s_engine_clock", HOOK_TYPE_START, HOOK_FLAG_FIXED, &hook_s_save_engine_clock);
}

void ppc_state_receive(uint8_t *s)
{
    ppcState.pc  = (uint32_t*)(*(uint64_t*)(s + 0));
    ppcState.npc = (uint32_t*)(*(uint64_t*)(s + 8));
    ppcState.gpr = (uint32_t*)(*(uint64_t*)(s + 16));
    ppcState.lr  = (uint32_t*)(*(uint64_t*)(s + 24));
}

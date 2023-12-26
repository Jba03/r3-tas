/************************************************************
 # hook.c: Unsafe function hooks
 ************************************************************/

#include <memory.h>
#include <game.h>
#include <time.h>

#include "bruteforce.hh"

#define HOOK_TYPE_START     0
#define HOOK_TYPE_REPLACE   1

#define HOOK_FLAG_GENERIC   0
#define HOOK_FLAG_DEBUG     1
#define HOOK_FLAG_FIXED     2

typedef void (*fn_hook_create)(uint32 addr, const char *func_name, int type, int flag, void (*callback)(void));

/* PowerPC state */
static uint32 *pc = NULL;
static uint32 *npc = NULL;
static uint32 *gpr = NULL;
static uint32 *lr = NULL;

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

static void hook_r_post_frame() /* after one frame */
{
    uint32 sp = *(gpr + 1) & 0x7FFFFFFF; /* sp = r1 */
    *lr = *gpr  = host_byteorder_32(*(uint32*)(memoryBase + sp + 0x24)); /* lr = r0 */
    *(gpr + 29) = host_byteorder_32(*(uint32*)(memoryBase + sp + 0x10));
    *(gpr + 29) = host_byteorder_32(*(uint32*)(memoryBase + sp + 0x14));
    *(gpr + 30) = host_byteorder_32(*(uint32*)(memoryBase + sp + 0x18));
    *(gpr + 31) = host_byteorder_32(*(uint32*)(memoryBase + sp + 0x1C));
    *(gpr + 1) += 32; /* SP alignment */
    sblr();
    
    if (bruteforcer->cycle())
    {
        memset(gpr, 0, sizeof(uint32) * 32);
        
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
}

static void hook_s_pre_engine() /* prior to engine execution */
{
    //printf("engine!\n");
    srand48(time(NULL));
    //input.stick.main.x->analogValue = game_byteorder_f32(drand48()*200.0f-100);
}

static void hook_r_post_engine() /* post engine execution */
{
    uint32 sp = *(gpr + 1) & 0x7FFFFFFF; /* sp = r1 */
    *lr = *gpr  = host_byteorder_32(*(uint32*)(memoryBase + sp + 0xD4)); /* lr = r0 */
    *(gpr + 29) = host_byteorder_32(*(uint32*)(memoryBase + sp + 0xC4));
    *(gpr + 30) = host_byteorder_32(*(uint32*)(memoryBase + sp + 0xC8));
    *(gpr + 31) = host_byteorder_32(*(uint32*)(memoryBase + sp + 0xCC));
    *(gpr + 1) += 208; /* SP alignment */
    sblr();
    
    
    
    tdstVector3D pos = fnActorGetPosition(actor_rayman);
    //printf("%.2f %.2f %.2f\n", pos.x, pos.y, pos.z);
    
    //printf("engine!\n");
    
    if (false)
    {
        /* Execute another engine cycle. */
        
        memset(gpr, 0, sizeof(uint32) * 32);
        
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
}

static void hook_s_treat_dynamic_hierarchy()
{
    //dprintf("treat\n");
}

static void hook_r_display_all()
{
    *npc = *lr;
}

void hle_hook_functions(fn_hook_create hook_create)
{
//    hook_create(0x8009f1a8, "hook_bus_speed", HOOK_TYPE_REPLACE, HOOK_FLAG_FIXED, &hook_s_bus_speed);
//    hook_create(0x8009f1ac, "hook_bus_speed2", HOOK_TYPE_REPLACE, HOOK_FLAG_FIXED, &hook_s_bus_speed);
//    
//    hook_create(0x80037078, "hook_pre_engine", HOOK_TYPE_START, HOOK_FLAG_FIXED, &hook_s_pre_engine);
//    hook_create(0x8003748c, "hook_post_engine", HOOK_TYPE_REPLACE, HOOK_FLAG_FIXED, &hook_r_post_engine);
//    
//    hook_create(0x80035bd4, "hook_pre_frame", HOOK_TYPE_START, HOOK_FLAG_FIXED, &hook_s_pre_frame);
//    hook_create(0x80035edc, "hook_post_frame", HOOK_TYPE_REPLACE, HOOK_FLAG_FIXED, &hook_r_post_frame);
//    
//    hook_create(0x80037178, "hook_treat_dynhierarchy", HOOK_TYPE_START, HOOK_FLAG_FIXED, &hook_s_treat_dynamic_hierarchy);
    
   // hook_create(0x80036174, "hook_display_all", HOOK_TYPE_REPLACE, HOOK_FLAG_FIXED, &hook_r_display_all);
}

void ppc_state_receive(struct { uint32 *pc, *npc, *gpr, *lr; } *s)
{
    pc  = s->pc;
    npc = s->npc;
    gpr = s->gpr;
    lr  = s->lr;
}

//
//  mind.c
//  r3-tas
//
//  Created by Jba03 on 2022-11-26.
//

#include "mind.h"
#include "global.h"
#include "dsg.h"

MIND struct Mind *mind_read(const address addr)
{
    struct Mind *mind = malloc(sizeof *mind);
    mind->offset = addr;
    
//    mind->ai_model = NULL;
//    mind->intelligence = NULL;
    //mind->reflex = NULL;
    mind->dsg = NULL;
    
    struct Stream* stream = stream_open(addr);
    mind->ptr_ai_model = readpointer();
    mind->ptr_intelligence = readpointer();
    mind->ptr_reflex = readpointer();
    mind->ptr_dsgmem = readpointer();
    mind->run_intelligence = read8();
    stream_close(stream);
    
    info("mind: %X\n", mind->offset);
    
//    if (mind->ptr_ai_model      != 0x00) mind->ai_model     = ai_model_read(mind->ptr_ai_model);
    if (mind->ptr_intelligence  != 0x00) mind->intelligence = intelligence_read(mind->ptr_intelligence);
    if (mind->ptr_reflex        != 0x00) mind->reflex       = intelligence_read(mind->ptr_reflex);
    if (mind->ptr_dsgmem        != 0x00) mind->dsg          = dsgmemory_read(mind->ptr_dsgmem);
    
    return mind;
}

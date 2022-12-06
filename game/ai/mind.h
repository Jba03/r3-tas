//
//  mind.h
//  r3-tas
//
//  Created by Jba03 on 2022-11-26.
//

#ifndef mind_h
#define mind_h

#include "memory.h"
#include "intelligence.h"

#define MIND

struct Mind {
    pointer ptr_ai_model;
    pointer ptr_intelligence;
    pointer ptr_reflex;
    pointer ptr_dsgmem;
    
//    struct AIModel* ai_model;
    struct Intelligence* intelligence;
    struct Intelligence* reflex;
    struct DSGMemory* dsg;
    
    uint8_t run_intelligence;
    
    address offset;
};

MIND struct Mind *mind_read(const address addr);

#endif /* mind_h */

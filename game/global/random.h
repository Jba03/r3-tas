//
//  random.h
//  r3-tas
//
//  Created by Jba03 on 2022-12-06.
//

#ifndef random_h
#define random_h

#include "memory.h"

#define RANDOM

struct RandomNumberDevice {
    pointer ptr_table;
    pointer ptr_table_indices;
    
    uint32_t table_length;
    uint32_t index_last;
    uint32_t table_max;
    float table_max_inv;
    
    address offset;
};

RANDOM struct RandomNumberDevice rnd_read(const address addr);
RANDOM uint32_t rnd_table_index(struct RandomNumberDevice rnd, unsigned index, unsigned offset);

/**
 * rnd_call:
 *  Call the random number device `n_calls` times. Use index of 0.
 */
RANDOM uint32_t rnd_call(struct RandomNumberDevice rnd, unsigned n_calls, unsigned index, unsigned min, unsigned max);

#endif /* random_h */

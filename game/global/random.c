//
//  random.c
//  r3-tas
//
//  Created by Jba03 on 2022-12-06.
//

#include "random.h"
#include "global.h"

/**
 * RNG calls per actor type
 *
 *  - Rayman calls RNG whenever he shoots his fist, or whenever his idle animation changes.
 *  - Collecting a yellow or red gem calls RNG five times.
 *  - Collecting a crown calls RNG four times.
 *  - Gems that disappear after a set duration call RNG once per frame when they flicker.
 *  - Matuvus (if visible) call RNG once on animation change.
 *  - Cages call RNG once when broken.
 *
 *  - Razoff #1 calls RNG when he shoots
 *  - Razoff #2's wrecking ball calls RNG twice when it hits Rayman
 *  - Razoff #2 calls RNG once when he falls off the wrecking ball
 *
 *  - The cannonball in Sea_10 calls the RNG twice when frame % 4 == 0.
 *
 *  - Riding the snowboard normally, updates RNG a total of three times per frame:
 *    twice for the snow particles, once for the animation of rayman bobbing up and down.
 *    When in the air, twice. After hitting a fence, three times.
 *
 * LaunchAGO seems to be the function responsible for the internal RNG update in most cases.
 */

RANDOM struct RandomNumberDevice rnd_read(const address addr)
{
    struct RandomNumberDevice rnd;
    rnd.offset = addr;
    
    struct Stream *stream = stream_open(addr);
    rnd.table_length = read32();
    rnd.ptr_table_indices = stream->position;
    advance(4 * 50); /* Skip index table */
    rnd.index_last = read32();
    rnd.table_max = read32();
    rnd.table_max_inv = readfloat();
    rnd.ptr_table = readpointer();
    
    stream_close(stream);
    
    return rnd;
}

RANDOM uint32_t rnd_table_index(struct RandomNumberDevice rnd, unsigned index, unsigned offset)
{
    uint32_t idx = memory.read_32(rnd.ptr_table_indices + index * 0x4);
    uint32_t value = memory.read_32(rnd.ptr_table + (idx + offset) * 0x4);
    
    return value;
}

RANDOM uint32_t rnd_call(struct RandomNumberDevice rnd, unsigned n_calls, unsigned index, unsigned mi, unsigned ma)
{
    uint32_t value = 0;
    for (unsigned n = 0; n < n_calls; n++)
        value = (mi + ((ma + 1 - mi) * rnd_table_index(rnd, index, n)) / (rnd.table_max + 1));
    
    return value;
}

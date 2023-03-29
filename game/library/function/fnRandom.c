//
//  rnd.c
//  r3-tas
//
//  Created by Jba03 on 2023-02-20.
//

#include "stRandom.h"
#include "game.h"

/**
 * RNG calls per actor type
 *
 *  - Rayman calls RNG whenever he shoots his fist, or whenever his idle animation changes.
 *  - Collecting a yellow or red gem calls RNG five times.
 *  - Collecting a crown calls RNG four times.
 *  - Gems that disappear after a set duration call RNG once per frame when they flicker.
 *  - Matuvus (if visible) call RNG once on animation change.
 *  - Cages call RNG once when broken.
 *  - Certain lamps (ie. the ones in the beginning of TLS1) call RNG when the light changes.
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

int32 fnRNDTableIndex(const tdstRandom *rnd, unsigned index, int offset)
{
    if (host_byteorder_32(rnd->table) == 0x00) return 0;
    
    /* The RNG table index is fetched from a table of 50 RNG "channels", */
    /* index 0 being the most common (if not the only) used by the game. */
    int32 idx = host_byteorder_32(*(int32*)(rnd->table_indices + index));
    /* The RNG table of 10000 entries is then indexed by previous index, together with an optional offset. */
    int32 value = host_byteorder_32(*(int32*)((uint8*)pointer(rnd->table) + max(0, idx + offset) * 4));
    
    return value;
}

int32 fnRNDCall(const tdstRandom *rnd, unsigned n_calls, unsigned index, unsigned mi, unsigned ma)
{
    int32_t value = 0;
    int32_t tmax = host_byteorder_32(rnd->table_max);
    
    for (unsigned n = 0; n < n_calls; n++)
        value = (mi + ((ma + 1 - mi) * fnRNDTableIndex(rnd, index, n)) / (tmax + 1));
    
    return value;
}

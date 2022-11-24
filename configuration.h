//
//  configuration.h
//  r3lib
//
//  Created by Jba03 on 2022-11-22.
//

#ifndef configuration_h
#define configuration_h

#include <stdio.h>
#include <stdbool.h>

struct Configuration {
    
    /**
     * enable_cheats:
     * Enables the ability to fly, change map, and more.
     */
    bool enable_cheats;
    
    /**
     * has_cheated:
     * Checked if cheats have been enabled any time during the
     * currently recording movie. If so, the run is invalidated.
     */
    bool has_cheated;
    
    /**
     * graphics_display_mode:
     *  0 = checkerboard pattern
     *  1 = normals
     */
    unsigned graphics_display_mode;
};

extern struct Configuration configuration;

#endif /* configuration_h */

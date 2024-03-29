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

#include "vector3.h"

struct configuration
{
    struct
    {
        bool hierarchy;
    } display;
    
    struct
    {
        /**
         * enable_cheats:
         * Enables the ability to fly, change map, and more.
         */
        bool enabled;
        
        /**
         * force_position:
         *  Force main actor position.
         */
        bool force_position;
        struct vector3 forced_position;
        
        /**
         * camera_unlocked:
         *  The in-game is allowed to be moved manually.
         */
        bool camera_unlocked;
        
    } cheats;
};

extern struct configuration configuration;

#endif /* configuration_h */

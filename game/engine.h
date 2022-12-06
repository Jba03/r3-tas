//
//  engine.h
//  r3-tas-structure
//
//  Created by Jba03 on 2022-12-06.
//

#ifndef engine_h
#define engine_h

#include "structure.h"

static struct structure structure_engine =
{
    structure_begin
    
    { s_type_uint8,          "state",                   0  },
    { s_type_string,         "current_level_name",      30 },
    { s_type_string,         "next_level_name",         30 },
    { s_type_string,         "first_level_name",        30 },
    { s_type_uint8,          "input_mode",              0  },
    { s_type_uint8,          "display_fix_mode",        0  },
    { s_type_padding,        NULL,                      3  },
    { s_type_uint32,         "display_mode",            0  },
    { s_struct_engine_timer, "timer",                   0  },
    { s_type_uint8,          "multi_player_count",      0  },
    { s_type_uint8,          "multi_column_count",      0  },
    { s_type_uint8,          "multi_min_screen_ratio",  0  },
    { s_type_padding,        NULL,                      1  },    
    
    structure_end
};

#endif /* engine_h */

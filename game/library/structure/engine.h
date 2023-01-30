#ifndef engine_h
#define engine_h

#include "structure.h"
#include "engine_timer.h"

struct engine
{
    readonly uint8 mode;
    readonly char8 current_level_name[30];
    readonly char8 next_level_name[30];
    readonly char8 first_level_name[30];
    readonly uint8 input_mode;
    readonly uint8 display_fix_mode;
    padding(3)
    uint32_t display_mode;
    padding(4)
    readonly struct engine_timer timer;
    readonly uint8 multimode_player_count;
    readonly uint8 multimode_column_count;
    readonly uint8 multimode_mini_screen_ratio;
    
    padding(1)
    
    readonly pointer current_main_players[4]; /* ::superobject */
    readonly int16 glddevice;
    readonly int16 gldfixviewport[5];
    padding(2)
    
    //int16_t device_id;
    //int16_t viewport_id[5];
    // viewport_attributes[5];
    // viewport_camera[5];
    //int16_t fix_viewport_id[5];
    uint32 submap_id;
    bool engine_paused;
    bool engine_paused2;
    bool engine_gamesave;
    
    char8 level_names[150][30];
    char8 demo_names[30][12];
    char8 demo_level_names[30][12];
    
    uint8 demo_count;
    uint8 level_count;
    uint8 current_level_num;
    uint8 previous_level_num;
    uint8 previous_level_exit_id;
    uint8 global_level_counter;
    uint8 demo_mode;
    uint8 current_language;
    uint8 language_count;
    uint8 engine_frozen;
    uint8 resurrection;
    
    uint8 camera_mode;
    uint8 current_importance;
    
    uint8 n_superobjects_allocated;
    uint8 n_superobjects_loaded;
    uint8 n_nonpersistent_so_links;
    
    struct SuperObjectLinks** so_links;
    
    struct CinematicsManager* cinematics_manager;
};

#endif /* engine_h */

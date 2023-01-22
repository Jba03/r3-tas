#ifndef engine_h
#define engine_h

#include "structure.h"
#include "engine_timer.h"

struct engine
{
    readonly uint8_t mode;
    readonly char current_level_name[30];
    readonly char next_level_name[30];
    readonly char first_level_name[30];
    readonly uint8_t input_mode;
    readonly uint8_t display_fix_mode;
    padding(3)
    uint32_t display_mode;
    padding(4)
    readonly struct engine_timer timer;
    readonly uint8_t multimode_player_count;
    readonly uint8_t multimode_column_count;
    readonly uint8_t multimode_mini_screen_ratio;
    
    padding(1)
    
    readonly pointer current_main_players[4]; /* ::superobject */
    readonly int16_t glddevice;
    readonly int16_t gldfixviewport[5];
    padding(2)
    
    //int16_t device_id;
    //int16_t viewport_id[5];
    // viewport_attributes[5];
    // viewport_camera[5];
    //int16_t fix_viewport_id[5];
    uint32_t submap_id;
    bool engine_paused;
    bool engine_paused2;
    bool engine_gamesave;
    
    const char* level_names[150];
    const char* demo_names[30];
    const char* demo_level_names[30];
    
    uint8_t demo_count;
    uint8_t level_count;
    uint8_t current_level_num;
    uint8_t previous_level_num;
    uint8_t previous_level_exit_id;
    uint8_t global_level_counter;
    uint8_t demo_mode;
    uint8_t current_language;
    uint8_t language_count;
    uint8_t engine_frozen;
    uint8_t resurrection;
    
    uint8_t camera_mode;
    uint8_t current_importance;
    
    uint8_t n_superobjects_allocated;
    uint8_t n_superobjects_loaded;
    uint8_t n_nonpersistent_so_links;
    
    struct SuperObjectLinks** so_links;
    
    struct CinematicsManager* cinematics_manager;
};

#endif /* engine_h */

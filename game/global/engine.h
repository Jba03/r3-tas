//
//  engine.h
//  r3tools
//
//  Created by Jba03 on 2022-11-12.
//

#ifndef engine_h
#define engine_h

#include <stdbool.h>

#include "global.h"

#include "engine_timer.h"
#include "superobject.h"
#include "superobject_links.h"
#include "matrix4.h"

#define ENGINE_STRUCT

struct Engine {
    uint8_t mode;
    
    const char* current_level_name;
    const char* next_level_name;
    const char* first_level_name;
    
    uint8_t input_mode;
    uint8_t display_fix_mode;
    uint32_t display_mode;
    
    struct EngineTimer timer;
    uint8_t multimode_player_count;
    uint8_t multimode_column_count;
    uint8_t multimode_mini_screen_ratio;
    
    struct SuperObject* current_main_players[4];
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
    
    
    /* Note: This is not actually part of the struct. */
    struct SuperObject* root;
    address offset;
};

ENGINE_STRUCT struct Engine* engine_read(const uint32_t address);

/* engine_change_map: load specified map */
ENGINE_STRUCT void engine_change_map(struct Engine *engine, const char* map);

/* engine_export_obj: export the current level geometry to an .obj file */
ENGINE_STRUCT void engine_export_obj(struct Engine *engine);

#endif /* engine_h */

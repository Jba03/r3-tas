#ifndef engine_h
#define engine_h

#include "structure.h"
#include "engine_timer.h"
#include "transform.h"
#include "list.h"

struct engine
{
    readonly uint8 mode;
    readonly char8 current_level_name[30];
    readonly char8 next_level_name[30];
    readonly char8 first_level_name[30];
    readonly uint8 input_mode;
    readonly uint8 display_fix_mode;
    padding(3)
    readonly uint32 display_mode;
    readonly struct engine_timer timer;
    readonly uint8 multimode_player_count;
    readonly uint8 multimode_column_count;
    readonly uint8 multimode_mini_screen_ratio;
    padding(1)
    readonly pointer current_main_players[4]; /* ::superobject */
    readonly int16 gld_device;
    readonly int16 gld_viewport[5];
    padding(5 * 28 * 4) /* viewport attributes */
    readonly pointer viewport_camera[5];
    readonly int16 gld_fix_viewport[5];
    padding(2)
    padding(5 * 28 * 4) /* fix viewport attributes */
    padding(5 * 2 * 4) /* fix 3d attributes */
    readonly pointer fix_camera[5];
    padding(5 * 2 * 4) /* game 3d attributes */
    readonly pointer viewport_array;
    readonly doubly_linked_list camera_list;
    readonly pointer draw_sem;
    readonly doubly_linked_list family_list;
    readonly doubly_linked_list always_list;
    readonly doubly_linked_list main_character_list;
    readonly pointer standard_camera;
    readonly pointer debug_camera;
    readonly pointer language_structure;
    readonly pointer level_filename_list;
    readonly struct transform main_actor_transform;
    readonly struct transform main_camera_transform;
    readonly int32 submap_number;
    readonly uint8 engine_paused;
    readonly uint8 engine_paused2;
    readonly uint8 engine_game_save;
    readonly char8 level_names[150][30];
    readonly char8 demo_names[30][12];
    readonly char8 demo_level_names[30][12];
    readonly uint8 demo_count;
    readonly uint8 level_count;
    readonly uint8 current_level;
    readonly uint8 previous_level;
    readonly uint8 previous_level_exit_id;
    readonly uint8 global_level_counter;
    readonly uint8 demo_mode;
    readonly uint8 current_language;
    readonly uint8 language_count;
    readonly uint8 engine_frozen;
    readonly uint8 resurrection;
    readonly int8 camera_mode;
    readonly uint8 current_importance;
    readonly uint8 n_superobjects_allocated;
    readonly uint8 n_superobjects_loaded;
    readonly uint8 n_nonpersistent_so_links;
    readonly doublepointer so_links;
    readonly pointer graph_chained_list;
    readonly pointer cine_manager;
};

#if USE_FUNCTIONS

/** engine_load_level: load level with specified name */
void engine_load_level(struct engine* engine, const char* level_name);

#endif

#endif /* engine_h */

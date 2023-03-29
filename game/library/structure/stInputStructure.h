#ifndef stInputStructure_h
#define stInputStructure_h

#include "structure.h"
#include "stVector3D.h"

struct stPadReadingOutput {
    readonly tdstVector3D vector;
    readonly int16 horizontal_axis;
    readonly int16 vertical_axis;
    readonly float32 analog_force;
    readonly float32 analog_force_true;
    readonly float32 angle;
    readonly int32 sector;
} typedef tdstPadReadingOutput;

struct stInputDevice {
    readonly uint8 valid;
    padding(3)
    readonly pointer handle;
    readonly uint8 joypad_counter[44];
    readonly int8 joy_min_x;
    readonly int8 joy_max_x;
    readonly int8 joy_min_y;
    readonly int8 joy_max_y;
    readonly int8 joy_center_x;
    readonly int8 joy_center_y;
    padding(2)
    readonly pointer joy_axis_x;
    readonly pointer joy_axis_y;
    readonly pointer joy_axis_z;
    readonly pointer joy_axis_r;
    readonly pointer key_up;
    readonly pointer key_down;
    readonly pointer key_left;
    readonly pointer key_right;
    readonly pointer key_speed;
    readonly uint8 no_keyboard_inertia;
    padding(3)
    readonly pointer joy_button[16];
    readonly pointer key_button[16];
    readonly tdstPadReadingOutput pad;
} typedef tdstInputDevice;

struct stInputStructure {
    readonly uint8 one_pad_activate;
    padding(3)
    readonly tdstInputDevice device[18];
    readonly uint8 keyboard_counter[256];
    readonly uint8 keyboard_type;
    readonly uint8 mousebutton_counter[9];
    padding(2)
    readonly uint32 n_entries;
    readonly pointer entries;
    readonly uint32 n_commands;
    readonly pointer commands;
    readonly int16 event_size;
    padding(2)
    readonly pointer historic_event;
    readonly uint8 one_action_validated;
    readonly char8 line[78];
    readonly char8 line_internal[78];
    readonly char8 line_search[78];
    readonly char8 line_last[10][78];
    padding(1)
    readonly int32 index_historic;
    readonly pointer command_searched;
    readonly pointer command_mode_entry_swap;
    readonly pointer command_mode_entry_clear;
} typedef tdstInputStructure;

struct stInputEntryElement {
    padding(6 * 4) /* ? */
    readonly uint32 num_keywords;
    readonly pointer keyword_array;
    readonly pointer action_name;
    readonly pointer entry_name;
    readonly int32 state;
    readonly float32 analogvalue;
    readonly int8 active;
    padding(3)
} typedef tdstInputEntryElement;

#if USE_FUNCTIONS

/** fnInputEntryFind: find an input entry by its action name */
tdstInputEntryElement* fnInputEntryFind(const tdstInputStructure* s, const char* action_name);

#endif

#endif /* stInputStructure_h */

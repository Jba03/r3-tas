#ifndef collide_material_h
#define collide_material_h

#include "structure.h"

#define collide_material_none               0
#define collide_material_slide              (1 << 0)
#define collide_material_trampoline         (1 << 1)
#define collide_material_grabbable_ledge    (1 << 2)
#define collide_material_wall               (1 << 3)
#define collide_material_unknown            (1 << 4)
#define collide_material_hangable_ceiling   (1 << 5)
#define collide_material_climbable_wall     (1 << 6)
#define collide_material_electric           (1 << 7)
#define collide_material_lava_death_warp    (1 << 8)
#define collide_material_fall_trigger       (1 << 9)
#define collide_material_hurt_trigger       (1 << 10)
#define collide_material_death_warp         (1 << 11)
#define collide_material_unknown2           (1 << 12)
#define collide_material_unknown3           (1 << 13)
#define collide_material_water              (1 << 14)
#define collide_material_no_collision       (1 << 15)
#define collide_material_all                0xffff

struct game_material
{
    readonly int32 material_sound;
    readonly pointer collide_material;
};

struct collide_material
{
    readonly int16 zonetype;
    readonly uint16 identifier;
    readonly float32 direction_x;
    readonly float32 direction_y;
    readonly float32 direction_z;
    readonly float32 coefficient;
    readonly uint16 aitype;
    padding(2)
};

#endif /* collide_material_h */

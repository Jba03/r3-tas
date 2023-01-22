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

struct collide_material
{
    readonly int16_t zonetype;
    readonly uint16_t identifier;
    readonly float direction_x;
    readonly float direction_y;
    readonly float direction_z;
    readonly float coefficient;
    readonly uint16_t aitype;
    padding(2)
};

#endif /* collide_material_h */

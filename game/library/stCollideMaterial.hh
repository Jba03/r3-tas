#ifndef stCollideMaterial_h
#define stCollideMaterial_h

#include "library.hh"

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

typedef struct stGameMaterial stGameMaterial;
typedef struct stCollideMaterial stCollideMaterial;

struct stGameMaterial
{
    readonly int32 materialSound;
    readonly pointer<> collideMaterial;
};

struct stCollideMaterial
{
    readonly int16 zoneType;
    readonly uint16 identifier;
    readonly float32 xDirection;
    readonly float32 yDirection;
    readonly float32 zDirection;
    readonly float32 coefficient;
    readonly uint16 aiType;
    readonly padding(2)
};

#endif /* stCollideMaterial_h */

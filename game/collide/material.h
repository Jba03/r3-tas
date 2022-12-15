//
//  material.h
//  r3-tas
//
//  Created by Jba03 on 2022-12-13.
//

#ifndef material_h
#define material_h

#include "global.h"

#define COLLISION_MAT_NONE              0
#define COLLISION_MAT_SLIDE             (1 << 0)
#define COLLISION_MAT_TRAMPOLINE        (1 << 1)
#define COLLISION_MAT_GRABBABLE_LEDGE   (1 << 2)
#define COLLISION_MAT_WALL              (1 << 3)
#define COLLISION_MAT_UNKNOWN           (1 << 4)
#define COLLISION_MAT_HANGABLE_CEILING  (1 << 5)
#define COLLISION_MAT_CLIMBABLE_WALL    (1 << 6)
#define COLLISION_MAT_ELECTRIC          (1 << 7)
#define COLLISION_MAT_LAVA_DEATH_WARP   (1 << 8)
#define COLLISION_MAT_FALL_TRIGGER      (1 << 9)
#define COLLISION_MAT_HURT_TRIGGER      (1 << 10)
#define COLLISION_MAT_DEATH_WARP        (1 << 11)
#define COLLISION_MAT_UNKNOWN2          (1 << 12)
#define COLLISION_MAT_UNKNOWN3          (1 << 13)
#define COLLISION_MAT_WATER             (1 << 14)
#define COLLISION_MAT_NO_COLLISION      (1 << 15)
#define COLLISION_MAT_ALL               0xFFFF

struct CollisionMaterial {
    
    uint16_t type;
    uint16_t identifier;
    uint32_t ai_type;
    
    struct Vector3 direction;
    float coefficient;
    
    address offset;
};

struct CollisionMaterial collision_material_read(const address addr);

#endif /* material_h */

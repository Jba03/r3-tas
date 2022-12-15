//
//  material.c
//  r3-tas
//
//  Created by Jba03 on 2022-12-13.
//

#include "material.h"

struct CollisionMaterial collision_material_read(const address addr)
{
    struct CollisionMaterial mat;
    mat.offset = addr;
    
    struct Stream* stream = stream_open(addr);
    mat.type = read16();
    mat.identifier = read16();
    
    mat.direction.x = readfloat();
    mat.direction.y = readfloat();
    mat.direction.z = readfloat();
    
    mat.coefficient = readfloat();
    mat.ai_type = read32();
    stream_close(stream);
    
    return mat;
}

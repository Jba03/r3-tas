//
//  physical_object.h
//  r3lib
//
//  Created by Jba03 on 2022-11-22.
//

#ifndef physical_object_h
#define physical_object_h

#include "collide.h"

#define PHYSICAL_OBJECT

struct PhysicalObject {
    pointer visual_set_ptr;
    pointer collide_set_ptr;
    pointer visual_bounding_volume_ptr;
    pointer collide_bounding_volume_ptr;
    
    struct CollisionGeometry* geometry;
    
    address offset;
};

struct IPO {
    pointer data_ptr;
    pointer radiosity_ptr;
    pointer portal_camera_ptr;
    
    const char* name;
    struct PhysicalObject *data;
    
    address offset;
};

PHYSICAL_OBJECT struct PhysicalObject *physical_object_read(const address addr);
PHYSICAL_OBJECT void physical_object_free(struct PhysicalObject* obj);

PHYSICAL_OBJECT struct IPO *ipo_read(const address addr);
PHYSICAL_OBJECT void ipo_free(struct IPO* ipo);

#endif /* physical_object_h */

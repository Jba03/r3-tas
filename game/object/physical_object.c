//
//  physical_object.c
//  r3lib
//
//  Created by Jba03 on 2022-11-22.
//

#include "physical_object.h"
#include "collide.h"
#include "global.h"

PHYSICAL_OBJECT struct PhysicalObject *physical_object_read(const address addr)
{
    struct PhysicalObject *obj = malloc(sizeof *obj);
    obj->offset = addr;
    
    struct Stream *stream = stream_open(addr);
    obj->visual_set_ptr = readpointer();
    obj->collide_set_ptr = readpointer();
    obj->visual_bounding_volume_ptr = readpointer();
    obj->collide_bounding_volume_ptr = readpointer();
    stream_close(stream);
    
    if (obj->collide_set_ptr != 0x00)
    {
        uint32_t geom_offset = memory.read_32(obj->collide_set_ptr + 4 * 3) & 0xFFFFFFF;
        /* TODO: Read entire collideset structure */
        obj->geometry = collision_geometry_read(geom_offset);
    }
    
    return obj;
}

PHYSICAL_OBJECT void physical_object_free(struct PhysicalObject* obj)
{
    //info(COLOR_GREEN "free PhysicalObject[%X]\n", obj->offset);
    if (obj->geometry && obj->collide_set_ptr != 0x00)
        collision_geometry_free(obj->geometry);
    free(obj);
}

PHYSICAL_OBJECT struct IPO *ipo_read(const address addr)
{
    struct IPO *ipo = malloc(sizeof *ipo);
    ipo->offset = addr;
    ipo->data = NULL;
    ipo->name = NULL;
    
    struct Stream *stream = stream_open(addr);
    ipo->data_ptr = readpointer();
    ipo->radiosity_ptr = readpointer();
    advance(4);
    ipo->portal_camera_ptr = readpointer();
    advance(4 * 3);
    ipo->name = readstring(0x32);
    stream_close(stream);
    
    ipo->data = physical_object_read(ipo->data_ptr);

    return ipo;
}

PHYSICAL_OBJECT void ipo_free(struct IPO* ipo)
{
    //info(COLOR_GREEN "free IPO[%X]: %s\n", ipo->offset, ipo->name);
    physical_object_free(ipo->data);
    free(ipo->name);
    free(ipo);
}

//
//  superobject.c
//  r3lib
//
//  Created by Jba03 on 2022-11-20.
//

#include "superobject.h"
#include "actor.h"
#include "physical_object.h"

#include <stdio.h>
#include <stdlib.h>

SUPEROBJECT const char* superobject_typename(const struct SuperObject *so)
{
    switch (so->type)
    {
        case SUPEROBJECT_TYPE_DUMMY: return "Dummy";
        case SUPEROBJECT_TYPE_WORLD: return "World";
        case SUPEROBJECT_TYPE_ACTOR: return "Actor";
        case SUPEROBJECT_TYPE_SECTOR: return "Sector";
        case SUPEROBJECT_TYPE_PHYSICAL_OBJECT: return "PhysicalObject";
        case SUPEROBJECT_TYPE_IPO: return "IPO";
        case SUPEROBJECT_TYPE_IPO2: return "IPO.2";
        case SUPEROBJECT_TYPE_GEOMETRIC_OBJECT: return "GeometricObject";
        case SUPEROBJECT_TYPE_GEOMETRIC_SHADOW_OBJECT: return "GeometricShadowObject";
        default: return "Invalid superobject type";
    }
}

SUPEROBJECT struct SuperObject* superobject_read(const uint32_t address)
{
    struct SuperObject *so = malloc(sizeof *so);
    so->offset = address;
    so->data = NULL;
    
    struct Stream *stream = stream_open(address);
    
    so->type = read32();
    so->data_ptr = readpointer();
    so->child_first = readpointer();
    so->child_last = readpointer();
    so->n_children = read32();
    
    so->sibling_next = readpointer();
    so->sibling_prev = readpointer();
    so->parent = readpointer();
    
    so->matrix_default = matrix4_read(readpointer() + 4);
    so->matrix_static = matrix4_read(readpointer() + 4);
    so->matrix_global = matrix4_read(readpointer() + 4);
    
    stream_close(stream);
    
    switch (so->type)
    {
        case SUPEROBJECT_TYPE_DUMMY:
            so->data = NULL;
            break;
            
        case SUPEROBJECT_TYPE_WORLD:
            so->data = NULL;
            break;
            
        case SUPEROBJECT_TYPE_ACTOR:
            so->data = actor_read(so->data_ptr);
            ((struct Actor*)so->data)->superobject = so;
            break;
            
        case SUPEROBJECT_TYPE_SECTOR:
            so->data = NULL;
            break;
            
        case SUPEROBJECT_TYPE_PHYSICAL_OBJECT:
            so->data = NULL;
            break;
            
        case SUPEROBJECT_TYPE_IPO:
            so->data = ipo_read(so->data_ptr);
            break;
            
        case SUPEROBJECT_TYPE_IPO2:
            so->data = NULL;
            break;
            
        case SUPEROBJECT_TYPE_GEOMETRIC_OBJECT:
            so->data = NULL;
            break;
            
        case SUPEROBJECT_TYPE_GEOMETRIC_SHADOW_OBJECT:
            so->data = NULL;
            break;
            
        default:
            so->data = NULL;
            break;
    }
    
    const char* typename = superobject_typename(so);
//    info(COLOR_WHITE "Reading"
//         COLOR_GREEN " %s "
//         COLOR_BLUE "@ %X "
//         COLOR_WHITE "(%d children)\n",
//         typename,
//         address,
//         so->n_children);
    
    uint32_t offset_next = so->child_first;
    for (int n = 0; n < so->n_children; n++)
    {
        log_indent++;
        
        struct SuperObject* child = superobject_read(offset_next);
        so->children[n] = child;
        offset_next = child->sibling_next;
        
        //if (offset_next == 0x00) break;
    }
    
    log_indent--;
    
    return so;
}

SUPEROBJECT void superobject_free(struct SuperObject** obj)
{
    if (!*obj) return;
    
    struct SuperObject* so = *obj;
    switch (so->type)
    {
        case SUPEROBJECT_TYPE_DUMMY:
            break;
            
        case SUPEROBJECT_TYPE_WORLD:
            break;
            
        case SUPEROBJECT_TYPE_ACTOR:
            actor_free(so->data);
            break;
            
        case SUPEROBJECT_TYPE_SECTOR:
            break;
            
        case SUPEROBJECT_TYPE_PHYSICAL_OBJECT:
            break;
            
        case SUPEROBJECT_TYPE_IPO:
            ipo_free(so->data);
            break;
            
        case SUPEROBJECT_TYPE_IPO2:
            break;
            
        case SUPEROBJECT_TYPE_GEOMETRIC_OBJECT:
            break;
            
        case SUPEROBJECT_TYPE_GEOMETRIC_SHADOW_OBJECT:
            break;
            
        default:
            break;
    }
    
    for (int n = 0; n < (*obj)->n_children; n++)
    {
        struct SuperObject* child = (*obj)->children[n];
        superobject_free(&child);
    }
    
    free(*obj);
}

SUPEROBJECT void superobject_for_every(const enum SuperObjectType type,
                                       struct SuperObject* root,
                                       void (*function)(struct SuperObject*, void* param),
                                       void* param)
{
    if (root->type == type)
        function(root, param);
    
    for (int n = 0; n < root->n_children; n++)
        superobject_for_every(type, root->children[n], function, param);
}

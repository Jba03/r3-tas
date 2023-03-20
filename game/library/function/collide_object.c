//
//  collide_object.c
//  r3-tas
//
//  Created by Jba03 on 2023-03-05.
//

#include "collide_object.h"
#include "game.h"
#include "octree.h"
#include "ray.h"

#include <stdio.h>

/** collide_object_mesh: get the collide mesh at specified index */
const struct collide_mesh* collide_object_mesh(const struct collide_object* object, int idx)
{
    if (!object) return NULL;
    
    int counter = 0;
    for (int i = 0; i < host_byteorder_16(object->n_elements); i++)
    {
        const pointer element = (*((pointer*)pointer(object->elements) + i));
        const int16_t type = host_byteorder_16(*((int16_t*)pointer(object->element_types) + i));
        const void* block = pointer(element);
        
        if (type == collide_object_indexed_triangles)
            if (counter++ == idx) return block;
    }
    
    return NULL;
}

/** collide_object_sphere: get collide sphere at specified index */
const struct collide_mesh* collide_object_sphere(const struct collide_object* object, int idx)
{
    if (!object) return NULL;
    
    int counter = 0;
    for (int i = 0; i < host_byteorder_16(object->n_elements); i++)
    {
        const pointer element = (*((pointer*)pointer(object->elements) + i));
        const int16_t type = host_byteorder_16(*((int16_t*)pointer(object->element_types) + i));
        const void* block = pointer(element);
        
        if (type == collide_object_spheres)
            if (counter++ == idx) return block;
    }
    
    return NULL;
}

float st[OCTREE_MAX_SELECTED_NODES];
struct octree_node* selected_nodes[OCTREE_MAX_SELECTED_NODES];
int n_selected_nodes = 0;

const bool collide_object_intersect_segment(const struct collide_object* object,
                                            const struct matrix4 T,
                                            const struct vector3 sA,
                                            const struct vector3 sB,
                                            struct ray *out_ray,
                                            struct vector3 *I)
{
    if (!object) return false;
    
    const struct octree* octree = pointer(object->octree);
    const struct octree_node* root = pointer(octree->root);

    const struct vector3 sAB = vector3_sub(sB, sA);
    
    int n_selected = 0;
    //float st[OCTREE_MAX_SELECTED_NODES];
    //struct octree_node* selected[OCTREE_MAX_SELECTED_NODES];
    //octree_traverse_line_segment(root, T, sA, sAB, selected_nodes, &n_selected_nodes, st);

    const struct vector3 sBA = vector3_sub(sB, sA);
    
    struct ray ray;
    ray.origin = sA;
    ray.direction = vector3_normalize(sBA);
    
    /* Compute the distance between the points */
    const float d = vector3_length(sBA);
    const float l = sqrt(vector3_dot(sBA, sBA));
    
//    printf("ray origin: %f %f %f\n", ray.origin.x, ray.origin.y, ray.origin.z);
//    printf("ray direction: %f %f %f\n", ray.direction.x, ray.direction.y, ray.direction.z);
    
    int mesh_index = 0;
    const struct collide_mesh* mesh = NULL;
    while ((mesh = collide_object_mesh(object, mesh_index)))
    {
        const uint16* indices = (const uint16*)pointer(mesh->face_indices);
        const struct vector3* vertices = (const struct vector3*)pointer(object->vertices);
        const struct vector3* normals = (const struct vector3*)pointer(mesh->normals);

        for (int16 index = 0; index < host_byteorder_16(mesh->n_faces); index++)
        {
            uint16 idx0 = host_byteorder_16(*(indices + index * 3 + 0));
            uint16 idx1 = host_byteorder_16(*(indices + index * 3 + 1));
            uint16 idx2 = host_byteorder_16(*(indices + index * 3 + 2));

            struct vector3 A = vector3_host_byteorder(*(vertices + idx0));
            struct vector3 B = vector3_host_byteorder(*(vertices + idx1));
            struct vector3 C = vector3_host_byteorder(*(vertices + idx2));

            struct vector3 N = vector3_host_byteorder(*(normals + index / 3));
            
            const struct vector4 TA = vector4_mul_matrix4(vector4_new(A.x, A.y, A.z, 1.0f), T);
            const struct vector4 TB = vector4_mul_matrix4(vector4_new(B.x, B.y, B.z, 1.0f), T);
            const struct vector4 TC = vector4_mul_matrix4(vector4_new(C.x, C.y, C.z, 1.0f), T);

            A = vector3_new(TA.x, TA.y, TA.z);
            B = vector3_new(TB.x, TB.y, TB.z);
            C = vector3_new(TC.x, TC.y, TC.z);

            float t;
            if (ray_triangle_intersection(ray, A, B, C, &t))
            {
                /* Limit ray to segment */
                //printf("%.2f vs %.2f\n", t, d);
                if (t < l)
                {
                    if (I) *I = vector3_add(ray.origin, vector3_mulf(ray.direction, t));
                    if (out_ray) *out_ray = ray;
                    return true;
                    
//                    A = vector3_add(A, (N));
//                    B = vector3_add(B, (N));
//                    C = vector3_add(C, (N));
//
//                    if (ray_triangle_intersection(ray, A, B, C, &t))
//                    {
//                        if (t < l) return true;
//                    }
                }
            }
        }

        mesh_index++;
    }
    
    return false;
}

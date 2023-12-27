//
//  collide_object.c
//  r3-tas
//
//  Created by Jba03 on 2023-03-05.
//

#include "stCollideObject.hh"
#include "stCollideElementIndexedTriangles.hh"
#include "stCollideElementIndexedSpheres.hh"
#include "game.hh"
#include "stOctree.hh"
#include "ray.hh"

#include <stdio.h>

#include "fnOctree.c"

/** fnCollideObjectGetElementIndexedTriangles: get the collide mesh at specified index */
const stCollideElementIndexedTriangles* fnCollideObjectGetElementIndexedTriangles(const stCollideObject* object, int idx)
{
    if (!object) return NULL;
    
    int counter = 0;
    for (int i = 0; i < host_byteorder_16(object->numElements); i++)
    {
        const pointer element = (*((pointer*)pointer(object->elements) + i));
        const int16_t type = host_byteorder_16(*((int16_t*)pointer(object->elementTypes) + i));
        const void* block = pointer(element);
        
        if (type == collide_object_indexed_triangles)
            if (counter++ == idx) return block;
    }
    
    return NULL;
}

/** fnCollideObjectGetElementIndexedSphere: get collide sphere at specified index */
const stCollideElementIndexedSphere* fnCollideObjectGetElementIndexedSphere(const stCollideObject* object, int idx)
{
    if (!object) return NULL;
    
    int counter = 0;
    for (int i = 0; i < host_byteorder_16(object->numElements); i++)
    {
        const pointer element = (*((pointer*)pointer(object->elements) + i));
        const int16_t type = host_byteorder_16(*((int16_t*)pointer(object->elementTypes) + i));
        const void* block = pointer(element);
        
        if (type == collide_object_indexed_spheres)
            if (counter++ == idx) return block;
    }
    
    return NULL;
}

float st[OCTREE_MAX_SELECTED_NODES];
stOctreeNode* selected_nodes[OCTREE_MAX_SELECTED_NODES];
int n_selected_nodes = 0;

const bool fnCollideObjectIntersectSegment(const stCollideObject* object,
                                           const stMatrix4D T,
                                           const stVector3D sA,
                                           const stVector3D sB,
                                           struct ray *out_ray,
                                           stVector3D *I)
{
    if (!object) return false;
    
    const stOctree* octree = pointer(object->octree);
    const stOctreeNode* root = pointer(octree->root);
    const stVector3D sAB = vector3_sub(sB, sA);
    
    int n_selected = 0;
    //float st[OCTREE_MAX_SELECTED_NODES];
    //stOctreeNode* selected[OCTREE_MAX_SELECTED_NODES];
    //octree_traverse_line_segment(root, T, sA, sAB, selected_nodes, &n_selected_nodes, st);

    const stVector3D sBA = vector3_sub(sB, sA);
    
    struct ray ray;
    ray.origin = sA;
    ray.direction = vector3_normalize(sBA);
    
    /* Compute the distance between the points */
    const float d = vector3_length(sBA);
    const float l = sqrt(vector3_dot(sBA, sBA));
    
//    printf("ray origin: %f %f %f\n", ray.origin.x, ray.origin.y, ray.origin.z);
//    printf("ray direction: %f %f %f\n", ray.direction.x, ray.direction.y, ray.direction.z);
    
    int mesh_index = 0;
    const stCollideElementIndexedTriangles* mesh = NULL;
    while ((mesh = fnCollideObjectGetElementIndexedTriangles(object, mesh_index)))
    {
        const uint16* indices = (const uint16*)pointer(mesh->faceIndices);
        const stVector3D* vertices = (const stVector3D*)pointer(object->vertices);
        const stVector3D* normals = (const stVector3D*)pointer(mesh->normals);

        for (int16 index = 0; index < host_byteorder_16(mesh->numFaces); index++)
        {
            uint16 idx0 = host_byteorder_16(*(indices + index * 3 + 0));
            uint16 idx1 = host_byteorder_16(*(indices + index * 3 + 1));
            uint16 idx2 = host_byteorder_16(*(indices + index * 3 + 2));

            stVector3D A = vector3_host_byteorder(*(vertices + idx0));
            stVector3D B = vector3_host_byteorder(*(vertices + idx1));
            stVector3D C = vector3_host_byteorder(*(vertices + idx2));

            stVector3D N = vector3_host_byteorder(*(normals + index / 3));
            
            const stVector4D TA = vector4_mul_matrix4(vector4_new(A.x, A.y, A.z, 1.0f), T);
            const stVector4D TB = vector4_mul_matrix4(vector4_new(B.x, B.y, B.z, 1.0f), T);
            const stVector4D TC = vector4_mul_matrix4(vector4_new(C.x, C.y, C.z, 1.0f), T);

            A = vector3_new(TA.x, TA.y, TA.z);
            B = vector3_new(TB.x, TB.y, TB.z);
            C = vector3_new(TC.x, TC.y, TC.z);

            float t;
            if (rayTriangleIntersection(ray, A, B, C, &t))
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
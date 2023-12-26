//
//  geometry.c
//  r3-tas
//
//  Created by Jba03 on 2023-02-23.
//

#include "geometry.hh"
#include "memory.hh"

int collide_object_triangles_combined(const tdstCollideObject* object, const tdstMatrix4D T, struct triangle** triangles)
{
    int list_size = 0;
    struct triangle* list = NULL;
    
    if (!object) return 0;
    if (!triangles) return 0;
    
    const tdstVector3D* vertices = pointer(object->vertices);
    /* Loop through all elements in the collide object. */
    for (int16 n = 0; n < host_byteorder_16(object->numElements); n++)
    {
        const pointer element = *((pointer*)pointer(object->elements) + n);
        const int16 type = host_byteorder_16(*((int16*)pointer(object->elementTypes) + n));
        
        const tdstCollideElementIndexedTriangles* mesh = pointer(element);
        if (!mesh) continue;
        
        /* Only care to process indexed triangles. */
        if (type != collide_object_indexed_triangles) continue;
        
        const int16 n_faces = host_byteorder_16(mesh->numFaces);
        /* Allocate triangles */
        list = realloc(list, sizeof(struct triangle) * (list_size + n_faces));
        
        for (int16 v = 0; v < n_faces; v++)
        {
            const tdstVector3D a = vertices[v * 3 + 0];
            const tdstVector3D b = vertices[v * 3 + 1];
            const tdstVector3D c = vertices[v * 3 + 2];
            
            const tdstVector4D aT = vector4_mul_matrix4(vector4_new(a.x, a.y, a.z, 1.0f), T);
            const tdstVector4D bT = vector4_mul_matrix4(vector4_new(b.x, b.y, b.z, 1.0f), T);
            const tdstVector4D cT = vector4_mul_matrix4(vector4_new(c.x, c.y, c.z, 1.0f), T);
            
            struct triangle tri;
            tri.a = vector3_new(aT.x, aT.y, aT.z);
            tri.b = vector3_new(bT.x, bT.y, bT.z);
            tri.c = vector3_new(cT.x, cT.y, cT.z);
            
            list[list_size++] = tri;
        }
    }
    
    *triangles = list;
    
    return list_size;
}

/**
 * collide_object_closest_vertex_to:
 * In a specified collide object, find the vertex which lies the closest to a certain point.
 */
int collide_object_closest_vertex_to(const tdstCollideObject* object,
                                     const tdstMatrix4D object_transform,
                                     const tdstVector3D point,
                                     tdstCollideElementIndexedTriangles** mesh_out,
                                     int16 *index_out,
                                     tdstVector3D *vertex_out)
{
    float length = INFINITY;
    tdstVector3D result = vector3_new(0.0f, 0.0f, 0.0f);
    tdstCollideElementIndexedTriangles* collmesh_out = NULL;
    int16 idx = 0;
    
    for (int16 n = 0; n < host_byteorder_16(object->numElements); n++)
    {
        const pointer element = *((pointer*)pointer(object->elements) + n);
        const int16 type = host_byteorder_16(*((int16*)pointer(object->elementTypes) + n));
        /* Ignore all types except indexed triangles. */
        if (type != collide_object_indexed_triangles) continue;
        
        /* Get the collide mesh */
        tdstCollideElementIndexedTriangles* mesh = pointer(element);
        const tdstVector3D* vertices = pointer(object->vertices);

        for (int16 v = 0; v < host_byteorder_16(mesh->numFaces) * 3; v++)
        {
            tdstVector3D vertex = vector3_host_byteorder(vertices[v]);
            tdstVector4D transformed = vector4_mul_matrix4(vector4_new(vertex.x, vertex.y, vertex.z, 1.0f), object_transform);
            vertex = vector3_new(transformed.x, transformed.y, transformed.z);
            
            const float d = vector3_length(vector3_sub(point, vertex));
            if (d < length)
            {
                idx = v;
                result = vertex;
                collmesh_out = mesh;
                length = d;
            }
        }
    }
    
    if (vertex_out) *vertex_out = result;
    if (index_out) *index_out = idx;
    if (mesh_out) *mesh_out = collmesh_out;
    
    return 0;
}

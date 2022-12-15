//
//  mesh.c
//  r3lib
//
//  Created by Jba03 on 2022-11-22.
//

#include "mesh.h"
#include "global.h"

MESH struct Mesh* mesh_read(const address addr, struct CollisionGeometry* geom)
{
    struct Mesh *mesh = malloc(sizeof *mesh);
    mesh->offset = addr;
    mesh->processed.normals = NULL;
    mesh->processed.indices = NULL;
    mesh->edge_coefficients = NULL;
    mesh->edge_normals = NULL;
    mesh->was_processed = false;
    mesh->glmesh = NULL;
    
    mesh->original.vertices = geom->vertices;
    
    
    struct Stream *stream = stream_open(addr);
    mesh->material_offset = readpointer();
    mesh->triangles_offset = readpointer();
    mesh->normals_offset = readpointer();
    mesh->n_triangles = read16();
    mesh->aabb_index = read16();
    /* ::geom element for visualization */
    mesh->mapping_offset = readpointer(); /* ::face edges? */
    /* ::edge normals */
    /* ::edge coefficients */
    /* ::num edges */
    advance(2); /* padding x 2 */
    
    if (mesh->material_offset != 0x00)
        mesh->material = collision_material_read(mesh->material_offset);
    
    /* Allocate unprocessed data */
    mesh->original.normals = malloc(sizeof(struct Vector3) * mesh->n_triangles);
    mesh->original.indices = malloc(sizeof(uint16_t) * mesh->n_triangles * 3);
    
    /* Read indices */
    stream_seek(stream, mesh->triangles_offset);
    for (unsigned i = 0; i < mesh->n_triangles; i++)
    {
        mesh->original.indices[i * 3 + 0] = read16();
        mesh->original.indices[i * 3 + 1] = read16();
        mesh->original.indices[i * 3 + 2] = read16();
    }
    
    /* Read normals */
    stream_seek(stream, mesh->normals_offset);
    for (unsigned N = 0; N < mesh->n_triangles; N++)
        mesh->original.normals[N] = vector3_read(stream->position + N * 3 * 4);
    
    stream_close(stream);
    
    return mesh;
}

MESH void mesh_reindex(struct Mesh* mesh)
{
    if (mesh->original.vertices != NULL)
    {
        /* Allocate processed data */
        mesh->processed.vertices = malloc(sizeof(struct Vector3) * mesh->n_triangles * 3);
        mesh->processed.normals = malloc(sizeof(struct Vector3) * mesh->n_triangles * 3);
        mesh->processed.indices = malloc(sizeof(uint16_t) * mesh->n_triangles * 3);

        /* Process vertices and normals */
        for (unsigned i = 0; i < mesh->n_triangles * 3; i++)
        {
            mesh->processed.vertices[i] = mesh->original.vertices[mesh->original.indices[i]];
            mesh->processed.normals[i] = mesh->original.normals[i / 3];
        }

        /* Store new indices */
        for (unsigned i = 0; i < mesh->n_triangles; i++)
        {
            mesh->processed.indices[i * 3 + 0] = i * 3 + 0;
            mesh->processed.indices[i * 3 + 1] = i * 3 + 2;
            mesh->processed.indices[i * 3 + 2] = i * 3 + 1;
        }
        
        mesh->was_processed = true;
    }
}

MESH static struct Vector4 mesh_material_color(struct Mesh* mesh)
{
    struct Vector4 color;
    if (mesh->material.identifier & COLLISION_MAT_NONE) color = vector4_new(1.0f, 1.0f, 1.0f, 0.5f);
    if (mesh->material.identifier & COLLISION_MAT_SLIDE) color = vector4_new(0.0f, 0.0f, 1.0f, 1.0f);
    if (mesh->material.identifier & COLLISION_MAT_CLIMBABLE_WALL) color = vector4_new(244.0f / 255.0f, 131.0f / 255.0f, 66.0f / 255.0f, 1.0f);
    if (mesh->material.identifier & COLLISION_MAT_HANGABLE_CEILING) color = vector4_new(244.0f / 255.0f, 131.0f / 255.0f, 66.0f / 255.0f, 1.0f);
    if (mesh->material.identifier & COLLISION_MAT_GRABBABLE_LEDGE) color = vector4_new(0.0f, 1.0f, 0.0f, 1.0f);
    
    if (mesh->material.identifier & COLLISION_MAT_UNKNOWN) color = vector4_new(124.0f / 255.0f, 68.0f / 255.0f, 33.0f / 255.0f, 1.0f);
    if (mesh->material.identifier & COLLISION_MAT_UNKNOWN2) color = vector4_new(124.0f / 255.0f, 68.0f / 255.0f, 33.0f / 255.0f, 1.0f);
    if (mesh->material.identifier & COLLISION_MAT_UNKNOWN3) color = vector4_new(124.0f / 255.0f, 68.0f / 255.0f, 33.0f / 255.0f, 1.0f);
    
    return vector4_new(1.0f, 1.0f, 1.0f, 1.0f);
}

MESH void mesh_create_glmesh(struct Mesh* mesh)
{
    if (!mesh->was_processed) return;
    
    struct GLMesh* glmesh = glmesh_alloc();
    glmesh->vertices = malloc(sizeof(struct GLVertex) * mesh->n_triangles * 3);
    glmesh->indices = malloc(sizeof(unsigned) * mesh->n_triangles * 3);
    
    glmesh->n_vertices = mesh->n_triangles * 3;
    glmesh->n_indices = mesh->n_triangles * 3;
    
    for (unsigned i = 0; i < mesh->n_triangles * 3; i++)
    {
        glmesh->vertices[i].position.x = mesh->processed.vertices[i].x;
        glmesh->vertices[i].position.y = mesh->processed.vertices[i].z;
        glmesh->vertices[i].position.z = mesh->processed.vertices[i].y;
        
        glmesh->vertices[i].normal.x = mesh->processed.normals[i].x;
        glmesh->vertices[i].normal.y = mesh->processed.normals[i].z;
        glmesh->vertices[i].normal.z = mesh->processed.normals[i].y;
    }
    
    for (unsigned i = 0; i < mesh->n_triangles * 3; i++)
    {
        struct Vector3 normal = mesh->processed.normals[i];
        normal = vector3_new(fabs(normal.x), fabs(normal.z), fabs(normal.y));
        float n = max(max(normal.x, normal.y), normal.z);
        
        float x = mesh->processed.vertices[i].x / 20.0f;
        float y = mesh->processed.vertices[i].y / 20.0f;
        float z = mesh->processed.vertices[i].z / 20.0f;
        
        if (n == fabs(normal.x)) {
            glmesh->vertices[i].texcoord.x = y;
            glmesh->vertices[i].texcoord.y = z;
        } else if (n == fabs(normal.y)) {
            glmesh->vertices[i].texcoord.x = x;
            glmesh->vertices[i].texcoord.y = z;
        } else {
            glmesh->vertices[i].texcoord.x = x;
            glmesh->vertices[i].texcoord.y = y;
        }
    }
    
    for (unsigned i = 0; i < mesh->n_triangles * 3; i++)
    {
        glmesh->indices[i] = mesh->processed.indices[i];
    }
    
    glmesh->color = mesh_material_color(mesh);
    
    glmesh_data(glmesh);
    mesh->glmesh = glmesh;
}

MESH void mesh_write_obj(struct Mesh* mesh, const char* filename)
{
    
}

MESH void mesh_destroy(struct Mesh* mesh)
{
    /* Original vertices free'd by collide geometry */
    free(mesh->original.normals);
    free(mesh->original.indices);
    
    if (mesh->was_processed)
    {
        free(mesh->processed.vertices);
        free(mesh->processed.normals);
        free(mesh->processed.indices);
        
        glmesh_destroy(mesh->glmesh);
    }
    
    free(mesh);
}

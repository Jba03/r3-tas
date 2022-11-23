//
//  mesh.c
//  r3lib
//
//  Created by Jba03 on 2022-11-22.
//

#include "mesh.h"
#include "stream.h"

MESH struct Mesh* mesh_read(const address addr, struct CollisionGeometry* geom)
{
    struct Mesh *mesh = malloc(sizeof *mesh);
    mesh->offset = addr;
    mesh->processed.normals = NULL;
    mesh->processed.indices = NULL;
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
    
    /* Allocate unprocessed data */
    mesh->original.normals = malloc(sizeof(union Vector3) * mesh->n_triangles);
    mesh->original.indices = malloc(sizeof(uint16_t) * mesh->n_triangles * 3);
    
    //info("Mesh (%d triangles)\n", mesh->n_triangles);
    
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
        mesh->processed.vertices = malloc(sizeof(union Vector3) * mesh->n_triangles * 3);
        mesh->processed.normals = malloc(sizeof(union Vector3) * mesh->n_triangles * 3);
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
        
        
        float normal = mesh->processed.normals[i].x + mesh->processed.normals[i].y + mesh->processed.normals[i].z;
        normal /= 3;
        normal += 0.1;
        glmesh->vertices[i].normal.x = normal;
        glmesh->vertices[i].normal.y = normal;
        glmesh->vertices[i].normal.z = normal;
    }
    
    for (unsigned i = 0; i < mesh->n_triangles * 3; i++)
    {
        glmesh->indices[i] = mesh->processed.indices[i];
    }
    
    glmesh_data(glmesh);
    mesh->glmesh = glmesh;
}

MESH void mesh_write_obj(struct Mesh* mesh, const char* filename)
{
    
}

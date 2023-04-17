//
//  graphics.c
//  r3-tas-memorymap
//
//  Created by Jba03 on 2022-12-31.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "mesh.h"
#include "memory.h"

#include "stVector3D.h"
#include "game.h"
#include "structure.h"
#include "stSuperObject.h"
#include "stInstantiatedPhysicalObject.h"
#include "stPhysicalObject.h"
#include "stCollideSet.h"
#include "stCollideObject.h"
#include "stCollideElementIndexedTriangles.h"

struct mesh* meshlist[1000];
tdstCollideObject* collide_objects[1000];
tdstMatrix4D matrices[1000];
int current_mesh = 0;
int current_matrix = 0;
int current_collide_object = 0;

static void mesh_create(tdstInstantiatedPhysicalObject* ipo)
{
    int mesh_index = 0;
    const tdstCollideElementIndexedTriangles* collmesh = NULL;
    const tdstCollideObject* zdr = fnIPOGetCollideObject(ipo);
    
    while ((collmesh = fnCollideObjectGetElementIndexedTriangles(zdr, mesh_index)))
    {
        tdstGameMaterial* gamemat = (tdstGameMaterial*)pointer(collmesh->material);
        
        struct mesh* mesh = malloc(sizeof *mesh);
        mesh->name = ipo->name;
        mesh->material = gamemat->collideMaterial;
        
        mesh->n_vertices = host_byteorder_16(collmesh->numFaces) * 3;
        mesh->n_indices = host_byteorder_16(collmesh->numFaces) * 3;
        mesh->vertices = malloc(sizeof(struct vertex) * host_byteorder_16(collmesh->numFaces) * 3);
        mesh->indices = malloc(sizeof(unsigned int) * host_byteorder_16(collmesh->numFaces) * 3);
        
        uint16_t* index = pointer(collmesh->faceIndices);
        
        uint16_t indices[host_byteorder_16(collmesh->numFaces) * 3];
        for (unsigned idx = 0; idx < host_byteorder_16(collmesh->numFaces); idx++)
        {
            indices[idx * 3 + 0] = host_byteorder_16(*(index + idx * 3 + 0));
            indices[idx * 3 + 1] = host_byteorder_16(*(index + idx * 3 + 1));
            indices[idx * 3 + 2] = host_byteorder_16(*(index + idx * 3 + 2));
        }
        
        tdstVector3D* vertices = pointer(zdr->vertices);
        tdstVector3D* normals = pointer(collmesh->normals);
        for (unsigned idx = 0; idx < host_byteorder_16(collmesh->numFaces) * 3; idx++)
        {
            mesh->vertices[idx].position.x = host_byteorder_f32(*(uint32_t*)&vertices[indices[idx]].x);
            mesh->vertices[idx].position.z = host_byteorder_f32(*(uint32_t*)&vertices[indices[idx]].y);
            mesh->vertices[idx].position.y = host_byteorder_f32(*(uint32_t*)&vertices[indices[idx]].z);
            
            mesh->vertices[idx].normal.x = host_byteorder_f32(*(uint32_t*)&normals[idx / 3].x);
            mesh->vertices[idx].normal.z = host_byteorder_f32(*(uint32_t*)&normals[idx / 3].y);
            mesh->vertices[idx].normal.y = host_byteorder_f32(*(uint32_t*)&normals[idx / 3].z);
        }
        
        for (unsigned idx = 0; idx < host_byteorder_16(collmesh->numFaces); idx++)
        {
            mesh->indices[idx * 3 + 0] = idx * 3 + 0;
            mesh->indices[idx * 3 + 1] = idx * 3 + 2;
            mesh->indices[idx * 3 + 2] = idx * 3 + 1;
        }
        
        for (unsigned idx = 0; idx < host_byteorder_16(collmesh->numFaces) * 3; idx++)
        {
            tdstVector3D normal;
            normal.x = fabs(mesh->vertices[idx].normal.x);
            normal.y = fabs(mesh->vertices[idx].normal.y);
            normal.z = fabs(mesh->vertices[idx].normal.z);
            
            float n = max(max(normal.x, normal.y), normal.z);
            
            float x = mesh->vertices[idx].position.x / 20.0f;
            float y = mesh->vertices[idx].position.y / 20.0f;
            float z = mesh->vertices[idx].position.z / 20.0f;
                
            if (n == fabs(normal.x)) {
                mesh->vertices[idx].texcoord.x = y;
                mesh->vertices[idx].texcoord.y = z;
            } else if (n == fabs(normal.y)) {
                mesh->vertices[idx].texcoord.x = x;
                mesh->vertices[idx].texcoord.y = z;
            } else {
                mesh->vertices[idx].texcoord.x = x;
                mesh->vertices[idx].texcoord.y = y;
            }
        }
        
        int n_valid_normals = 0;
        for (unsigned idx = 0; idx < host_byteorder_16(collmesh->numFaces) * 3; idx++)
        {
            tdstVector3D normal;
            normal.x = fabs(mesh->vertices[idx].normal.x);
            normal.y = fabs(mesh->vertices[idx].normal.y);
            normal.z = fabs(mesh->vertices[idx].normal.z);
            
            if (normal.y > 0.5f) n_valid_normals++;
        }
        
        const float normal_average = (float)n_valid_normals / (float)(host_byteorder_16(collmesh->numFaces) * 3);
        
        //if (normal_average > 0.5f)
        {
            mesh_process(mesh);
            
            meshlist[current_mesh++] = mesh;
        }
        
        mesh_index++;
    }
}

char *replace(const char *s, char ch, const char *repl)
{
    int count = 0;
    const char *t;
    for(t=s; *t; t++)
        count += (*t == ch);

    size_t rlen = strlen(repl);
    char *res = malloc(strlen(s) + (rlen-1)*count + 1);
    char *ptr = res;
    for(t=s; *t; t++) {
        if(*t == ch) {
            memcpy(ptr, repl, rlen);
            ptr += rlen;
        } else {
            *ptr++ = *t;
        }
    }
    *ptr = 0;
    return res;
}

static void export_obj(struct mesh* mesh, FILE* fp, int *prev_index)
{
    if (mesh->vertices)
    {
        for (unsigned t = 0; t < mesh->n_vertices; t++)
        {
            struct vertex v = mesh->vertices[t];
                                            
            tdstVector4D v2;
            v2.x = v.position.x;
            v2.z = v.position.y;
            v2.y = v.position.z;
            v2.w = 1;
                                            
//            tdstMatrix4D transform = obj->matrix_default;
//            tdstVector4D computed = vector4_mul_matrix4(v2, transform);
//
            char buf[128];
            memset(buf, 0, 128);
            sprintf(buf, "v %.7f %.7f %.7f\n", v2.x, v2.z, v2.y);
            char* a= replace(buf, ',', ".");
                                            
            fprintf(fp, "%s", a);
                                            
            free(a);
        }
                                        
        fprintf(fp, "\n");
        fprintf(fp, "o %s\n", mesh->name);
        for (unsigned t = 0; t < mesh->n_vertices / 3; t++)
        {
            uint16_t idx0 = mesh->indices[t * 3 + 0] + 1 + *prev_index;
            uint16_t idx1 = mesh->indices[t * 3 + 1] + 1 + *prev_index;
            uint16_t idx2 = mesh->indices[t * 3 + 2] + 1 + *prev_index;
            fprintf(fp, "f %d %d %d\n", idx0, idx1, idx2);
        }
        
        fprintf(fp, "\n");
                                        
        *prev_index += mesh->indices[mesh->n_vertices - 1] + 2;
    }
}

static void ipo_recurse(tdstSuperObject* so)
{
    if (superobject_type(so) == superobject_type_ipo)
    {
        tdstInstantiatedPhysicalObject* ipo = (tdstInstantiatedPhysicalObject*)superobject_data(so);
        if (!ipo) return;
        
        mesh_create(ipo);
        if (meshlist[current_mesh-1])
            meshlist[current_mesh-1]->superobject = so;
    }
    
    superobject_for_each(so, child)
    {
        ipo_recurse(child);
    };
}

void graphics_load(void)
{
    if (hierarchy)
    {
        FILE* fp = fopen(LIBR3TAS_DIR "/dump.obj", "wb");
        
        int prev_index = 0;
        
        ipo_recurse(father_sector);
        
        fclose(fp);
        printf("Number of meshes: %d\n", current_mesh);
    }
}

void graphics_unload(void)
{
    while (--current_mesh)
    {
        if (meshlist[current_mesh])
            free(meshlist[current_mesh]);
    }
}

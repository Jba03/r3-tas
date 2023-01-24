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

//#include "vector3.h"
#include "game.h"
#include "structure.h"
#include "superobject.h"
#include "ipo.h"
#include "physical_object.h"
#include "collideset.h"
#include "collide_object.h"
#include "collide_mesh.h"

struct mesh* meshlist[1000];
unsigned current_mesh = 0;

//static void mesh_process(struct mesh* mesh)
//{
//}

static void mesh_create(struct ipo* ipo)
{
    /* Get physical object */
    const struct physical_object* po = (struct physical_object*)pointer(ipo->physical_object);
    if (!po) return;
    
    /* Get the collide set of that object */
    const struct physical_collideset* collset = (const struct physical_collideset*)pointer(po->physical_collideset);
    if (!collset) return;
    
    /* Level geometry is part of reaction zone. */
    const struct collide_object* zdr = (const struct collide_object*)pointer(collset->zdr);
    if (!zdr) return;
    
    //printf("\n");
    
    for (int i = 0; i < host_byteorder_16(zdr->n_elements); i++)
    {
        const pointer element = (*((pointer*)pointer(zdr->elements) + i));
        const int16_t type = host_byteorder_16(*((int16_t*)pointer(zdr->element_types) + i));
        
        const void* block = pointer(element);
        
        //printf("draw! %X\n", offset(block));
        
        //printf("type: %X\n", offset(block));
        
        
        switch (type)
        {
            case collide_object_indexed_triangles:
            {
                struct collide_mesh* collmesh = (struct collide_mesh*)block;
                
                struct mesh* mesh = malloc(sizeof *mesh);
                mesh->name = ipo->name;
                
                mesh->n_vertices = host_byteorder_16(collmesh->n_faces) * 3;
                mesh->n_indices = host_byteorder_16(collmesh->n_faces) * 3;
                mesh->vertices = malloc(sizeof(struct vertex) * host_byteorder_16(collmesh->n_faces) * 3);
                mesh->indices = malloc(sizeof(unsigned int) * host_byteorder_16(collmesh->n_faces) * 3);
                
                uint16_t* index = pointer(collmesh->face_indices);
                
                uint16_t indices[host_byteorder_16(collmesh->n_faces) * 3];
                for (unsigned idx = 0; idx < host_byteorder_16(collmesh->n_faces); idx++)
                {
                    indices[idx * 3 + 0] = host_byteorder_16(*(index + idx * 3 + 0));
                    indices[idx * 3 + 1] = host_byteorder_16(*(index + idx * 3 + 1));
                    indices[idx * 3 + 2] = host_byteorder_16(*(index + idx * 3 + 2));
                }
                
                struct vector3* vertices = pointer(zdr->vertices);
                struct vector3* normals = pointer(collmesh->normals);
                for (unsigned idx = 0; idx < host_byteorder_16(collmesh->n_faces) * 3; idx++)
                {
                    mesh->vertices[idx].position.x = host_byteorder_f32(*(uint32_t*)&vertices[indices[idx]].x);
                    mesh->vertices[idx].position.z = host_byteorder_f32(*(uint32_t*)&vertices[indices[idx]].y);
                    mesh->vertices[idx].position.y = host_byteorder_f32(*(uint32_t*)&vertices[indices[idx]].z);
                    
                    mesh->vertices[idx].normal.x = host_byteorder_f32(*(uint32_t*)&normals[idx / 3].x);
                    mesh->vertices[idx].normal.z = host_byteorder_f32(*(uint32_t*)&normals[idx / 3].y);
                    mesh->vertices[idx].normal.y = host_byteorder_f32(*(uint32_t*)&normals[idx / 3].z);
                    
                    //printf("%f %f %f\n", mesh->vertices[idx].position.x, mesh->vertices[idx].position.z, mesh->vertices[idx].position.y);
                    /* normals.. */
                }
                
                //printf("\n");
                
                for (unsigned idx = 0; idx < host_byteorder_16(collmesh->n_faces); idx++)
                {
                    mesh->indices[idx * 3 + 0] = idx * 3 + 0;
                    mesh->indices[idx * 3 + 1] = idx * 3 + 2;
                    mesh->indices[idx * 3 + 2] = idx * 3 + 1;
                }
                
                for (unsigned idx = 0; idx < host_byteorder_16(collmesh->n_faces) * 3; idx++)
                {
                    struct vector3 normal;
                    normal.x = fabs(host_byteorder_f32(*(uint32_t*)&mesh->vertices[idx].normal.x));
                    normal.y = fabs(host_byteorder_f32(*(uint32_t*)&mesh->vertices[idx].normal.y));
                    normal.z = fabs(host_byteorder_f32(*(uint32_t*)&mesh->vertices[idx].normal.z));
                    
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
                
                mesh_process(mesh);
                
                meshlist[current_mesh++] = mesh;
                
                break;
            }
        }
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
                                            
            struct vector4 v2;
            v2.x = v.position.x;
            v2.z = v.position.y;
            v2.y = v.position.z;
            v2.w = 1;
                                            
//            matrix4 transform = obj->matrix_default;
//            struct vector4 computed = vector4_mul_matrix4(v2, transform);
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


void graphics_load(void)
{
    FILE* fp = fopen(LIBR3TAS_DIR "/dump.obj", "wb");
    
    int prev_index = 0;
    
    superobject_for_each(superobject_last_child(hierarchy), sector)
    {
        superobject_for_each(sector, ipo_so)
        {
            const struct ipo* ipo = (const struct ipo*)superobject_data(ipo_so);
            if (!ipo) continue;
            
            mesh_create(ipo);
            meshlist[current_mesh-1]->transform_global = pointer(ipo_so->transform_global);
            //export_obj(meshlist[current_mesh-1], fp, &prev_index);
        }
    }
    
    
    fclose(fp);
    
    printf("n meshes: %d\n", current_mesh);
}

void graphics_unload(void)
{
    while (current_mesh--)
    {
        free(meshlist[current_mesh]);
    }
}

//
//  mesh.c
//  r3-tas
//
//  Created by Jba03 on 2022-11-23.
//

#include "mesh.h"

static struct vertex box_vertices[] =
{
    /* -X */
    { {-1.0f, -1.0f, -1.0f}, {}, {0.0f, 1.0f} },
    { {-1.0f, -1.0f,  1.0f}, {}, {1.0f, 1.0f} },
    { {-1.0f,  1.0f,  1.0f}, {}, {1.0f, 0.0f} },
    { {-1.0f,  1.0f,  1.0f}, {}, {1.0f, 0.0f} },
    { {-1.0f,  1.0f, -1.0f}, {}, {0.0f, 0.0f} },
    { {-1.0f, -1.0f, -1.0f}, {}, {0.0f, 1.0f} },
    
    /* -Z */
    { {-1.0f, -1.0f, -1.0f}, {}, {1.0f, 1.0f} },
    { { 1.0f,  1.0f, -1.0f}, {}, {0.0f, 0.0f} },
    { { 1.0f, -1.0f, -1.0f}, {}, {0.0f, 1.0f} },
    { {-1.0f, -1.0f, -1.0f}, {}, {1.0f, 1.0f} },
    { {-1.0f,  1.0f, -1.0f}, {}, {1.0f, 0.0f} },
    { { 1.0f,  1.0f, -1.0f}, {}, {0.0f, 0.0f} },
    
    /* -Y */
    { {-1.0f, -1.0f, -1.0f}, {}, {1.0f, 0.0f} },
    { { 1.0f, -1.0f, -1.0f}, {}, {1.0f, 1.0f} },
    { { 1.0f, -1.0f,  1.0f}, {}, {0.0f, 1.0f} },
    { {-1.0f, -1.0f, -1.0f}, {}, {1.0f, 0.0f} },
    { { 1.0f, -1.0f,  1.0f}, {}, {0.0f, 1.0f} },
    { {-1.0f, -1.0f,  1.0f}, {}, {0.0f, 0.0f} },
    
    /* +Y */
    { {-1.0f, 1.0f, -1.0f}, {}, {1.0f, 0.0f} },
    { {-1.0f, 1.0f,  1.0f}, {}, {0.0f, 0.0f} },
    { { 1.0f, 1.0f,  1.0f}, {}, {0.0f, 1.0f} },
    { {-1.0f, 1.0f, -1.0f}, {}, {1.0f, 0.0f} },
    { { 1.0f, 1.0f,  1.0f}, {}, {0.0f, 1.0f} },
    { { 1.0f, 1.0f, -1.0f}, {}, {1.0f, 1.0f} },
    
    /* +X */
    { {1.0f,  1.0f, -1.0f}, {}, {1.0f, 0.0f} },
    { {1.0f,  1.0f,  1.0f}, {}, {0.0f, 0.0f} },
    { {1.0f, -1.0f,  1.0f}, {}, {0.0f, 1.0f} },
    { {1.0f, -1.0f,  1.0f}, {}, {0.0f, 1.0f} },
    { {1.0f, -1.0f, -1.0f}, {}, {1.0f, 1.0f} },
    { {1.0f,  1.0f, -1.0f}, {}, {1.0f, 0.0f} },
    
    /* +Z */
    { {-1.0f,  1.0f, 1.0f}, {}, {0.0f, 0.0f} },
    { {-1.0f, -1.0f, 1.0f}, {}, {0.0f, 1.0f} },
    { { 1.0f,  1.0f, 1.0f}, {}, {1.0f, 0.0f} },
    { {-1.0f, -1.0f, 1.0f}, {}, {0.0f, 1.0f} },
    { { 1.0f, -1.0f, 1.0f}, {}, {1.0f, 1.0f} },
    { { 1.0f,  1.0f, 1.0f}, {}, {1.0f, 0.0f} },
};

struct mesh* mesh_alloc(void)
{
    struct mesh* m = malloc(sizeof *m);
    m->n_vertices = 0;
    m->n_indices = 0;
    m->vertices = NULL;
    m->indices = NULL;
    //m->transform_global = matrix4_identity;
    m->color = vector4_new(1.0f, 1.0f, 1.0f, 1.0f);
    
    m->name = "Invalid Mesh";
    
    return m;
}

struct mesh* mesh_box(void)
{
    struct mesh *box = mesh_alloc();
    box->name = "Box mesh";
    box->n_vertices = 36;
    box->n_indices = 36;
    box->vertices = box_vertices;
    
    box->indices = malloc(sizeof(unsigned) * box->n_indices);
    for (int i = 0; i < box->n_indices; i++) box->indices[i] = i;
    
    mesh_process(box);
    
    return box;
}

struct mesh *mesh_sphere(const float sectors, const float stacks)
{
    struct mesh *sphere = mesh_alloc();
    sphere->name = "Sphere mesh";
    
    const float sector_step = 2 * M_PI / sectors;
    const float stack_step = M_PI / stacks;
    
    sphere->n_vertices = (sectors + 1) * (stacks + 1);
    sphere->n_indices = sectors * stacks * 6 - (sectors * 6);
    sphere->vertices = malloc(sizeof(struct vertex) * sphere->n_vertices * 3);
    sphere->indices = malloc(sizeof(unsigned) * sphere->n_indices);
    
    //sphere->vertices[1].position = struct vector3_new(0.0f, 10.0f, 0.0f);
    
    int vertex = 0;
    for (int i = 0; i <= stacks; ++i)
    {
        const float v = (M_PI / 2.0f) - i * stack_step;
        const float xy = cos(v);
        const float z = sin(v);
        
        for (int j = 0; j <= sectors; ++j)
        {
            const float u = j * sector_step;
            const float x = xy * cos(u);
            const float y = xy * sin(u);
            
            sphere->vertices[vertex].position.x = x;
            sphere->vertices[vertex].position.y = z;
            sphere->vertices[vertex].position.z = y;
            
            
            sphere->vertices[vertex].normal.x = x;
            sphere->vertices[vertex].normal.y = z;
            sphere->vertices[vertex].normal.z = y;
            
//            sphere->vertices[vertex].normal.x = x;
//            sphere->vertices[vertex].normal.y = y;
//            sphere->vertices[vertex].normal.z = z;
//            
//            sphere->vertices[vertex].texcoord.x = x;
//            sphere->vertices[vertex].texcoord.y = y;
            
            vertex++;
        }
    }
    
    int index = 0;
    for (int i = 0; i < stacks; ++i)
    {
        int a = i * (sectors + 1);
        int b = a + sectors + 1;
        for (int j = 0; j < sectors; ++j, ++a, ++b)
        {
            if (i != 0)
            {
                sphere->indices[index++] = a;
                sphere->indices[index++] = b;
                sphere->indices[index++] = a + 1;
            }
            
            if (i != (stacks - 1))
            {
                sphere->indices[index++] = a + 1;
                sphere->indices[index++] = b;
                sphere->indices[index++] = b + 1;
            }
        }
    }
    
    mesh_process(sphere);
    
    return sphere;
}

//void glmesh_destroy(struct glmesh *mesh)
//{
//    glDeleteVertexArrays(1, &mesh->vao);
//    glDeleteBuffers(1, &mesh->vbo);
//    glDeleteBuffers(1, &mesh->ebo);
//    
//    free(mesh->vertices);
//    free(mesh->indices);
//    free(mesh);
//}

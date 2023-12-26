//
//  mesh.c
//  r3-tas
//
//  Created by Jba03 on 2022-11-23.
//

#include "mesh.hh"

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

void mesh_process(struct mesh *mesh) {}

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

void sphereMeshGen(const float resolution, unsigned *vertexCount, stVector3D **vertices, stVector3D **normals, unsigned *indexCount, unsigned **indices)
{
    const float sector_step = 2 * M_PI / resolution;
    const float stack_step = M_PI / resolution;
    
    *vertexCount = (resolution + 1) * (resolution + 1);
    *indexCount = resolution * resolution * 6 - (resolution * 6);
    *vertices = malloc(sizeof(stVector3D) * (*vertexCount) * 3);
    *normals = malloc(sizeof(stVector3D) * (*vertexCount) * 3);
    *indices = malloc(sizeof(unsigned) * (*indexCount));
    
    int vertex = 0;
    for (unsigned i = 0; i <= resolution; ++i)
    {
        const float v = (M_PI / 2.0f) - i * stack_step;
        const float xy = cos(v);
        const float z = sin(v);
        
        for (int j = 0; j <= resolution; ++j)
        {
            const float u = j * sector_step;
            const float x = xy * cos(u);
            const float y = xy * sin(u);
            
            (*normals)[vertex].x = (*vertices)[vertex].x = x;
            (*normals)[vertex].y = (*vertices)[vertex].y = z;
            (*normals)[vertex].z = (*vertices)[vertex].z = y;
            vertex++;
        }
    }
    
    int index = 0;
    for (unsigned i = 0; i < resolution; ++i)
    {
        int a = i * (resolution + 1);
        int b = a + resolution + 1;
        for (unsigned j = 0; j < resolution; ++j, ++a, ++b)
        {
            if (i != 0)
            {
                (*indices)[index++] = a;
                (*indices)[index++] = b;
                (*indices)[index++] = a + 1;
            }

            if (i != (resolution - 1))
            {
                (*indices)[index++] = a + 1;
                (*indices)[index++] = b;
                (*indices)[index++] = b + 1;
            }
        }
    }
}

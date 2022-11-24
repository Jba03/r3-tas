//
//  glmesh.c
//  r3lib
//
//  Created by Jba03 on 2022-11-23.
//

#include "glmesh.h"
#include "common.h"

struct GLMesh* glmesh_alloc(void)
{
    struct GLMesh* m = malloc(sizeof *m);
    m->n_vertices = 0;
    m->n_indices = 0;
    m->vertices = NULL;
    m->indices = NULL;
    m->transform = matrix4_identity;
    
    return m;
}

struct GLMesh *glmesh_sphere(const float sectors, const float stacks)
{
    struct GLMesh *sphere = glmesh_alloc();
    
    const float sector_step = 2 * M_PI / sectors;
    const float stack_step = M_PI / stacks;
    
    sphere->n_vertices = (sectors + 1) * (stacks + 1);
    sphere->n_indices = sectors * stacks * 6 - (sectors * 6);
    sphere->vertices = malloc(sizeof(struct GLVertex) * sphere->n_vertices * 3);
    sphere->indices = malloc(sizeof(unsigned) * sphere->n_indices);
    
    //sphere->vertices[1].position = vector3_new(0.0f, 10.0f, 0.0f);
    
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
            sphere->vertices[vertex].normal.y = y;
            sphere->vertices[vertex].normal.z = z;
            
            sphere->vertices[vertex].texcoord.x = x;
            sphere->vertices[vertex].texcoord.y = y;
            
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
    
    glmesh_data(sphere);
    
    return sphere;
}

void glmesh_data(struct GLMesh *mesh)
{
    /* Generate arrays and buffers */
    glGenVertexArrays(1, &mesh->vao);
    glGenBuffers(1, &mesh->vbo);
    glGenBuffers(1, &mesh->ebo);
    
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attributepointer(s).
    glBindVertexArray(mesh->vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
    glBufferData(GL_ARRAY_BUFFER, mesh->n_vertices * sizeof(struct GLVertex), mesh->vertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->n_indices * sizeof(unsigned int), &mesh->indices[0], GL_STATIC_DRAW);
    
    /* Position */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(struct GLVertex), (GLvoid*)offsetof(struct GLVertex, position));
    glEnableVertexAttribArray(0);
    
    /* Normal */
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(struct GLVertex), (GLvoid*)offsetof(struct GLVertex, normal));
    glEnableVertexAttribArray(1);
    
    /* Texture coordinate */
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(struct GLVertex), (GLvoid*)offsetof(struct GLVertex, texcoord));
    glEnableVertexAttribArray(2);
    
    /* Unbind VAO */
    glBindVertexArray(0);
}

void glmesh_draw(struct GLMesh *mesh)
{
    glBindVertexArray(mesh->vao);
    glDrawElements(GL_TRIANGLES, mesh->n_indices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void glmesh_destroy(struct GLMesh *mesh)
{
    glDeleteVertexArrays(1, &mesh->vao);
    glDeleteBuffers(1, &mesh->vbo);
    glDeleteBuffers(1, &mesh->ebo);
    
    free(mesh->vertices);
    free(mesh->indices);
    free(mesh);
}

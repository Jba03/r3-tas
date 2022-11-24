//
//  glmesh.h
//  r3lib
//
//  Created by Jba03 on 2022-11-23.
//

#ifndef glmesh_h
#define glmesh_h

#include "opengl.h"
#include "vertex.h"
#include "matrix4.h"
#include "mesh.h"

struct GLMesh {
    unsigned n_vertices;
    struct GLVertex* vertices;
    
    unsigned n_indices;
    unsigned* indices;
    
    struct Matrix4 transform;
    
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
};

struct GLMesh* glmesh_alloc(void);

struct GLMesh* glmesh_sphere(const float sectors, const float stacks);

void glmesh_data(struct GLMesh *mesh);

void glmesh_draw(struct GLMesh *mesh);

#endif /* glmesh_h */

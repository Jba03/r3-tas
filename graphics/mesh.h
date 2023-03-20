//
//  glmesh.h
//  r3lib
//
//  Created by Jba03 on 2022-11-23.
//

#ifndef glmesh_h
#define glmesh_h

#include "vertex.h"
#include "stMatrix4D.h"
#include "stTransform.h"
#include "stCollideMaterial.h"
#include "stSuperObject.h"

struct mesh
{
    const char* name;
    
    unsigned n_vertices;
    struct vertex* vertices;
    
    unsigned n_indices;
    unsigned* indices;
    
    tdstSuperObject* superobject;
    tdstTransform* transform_global;
    tdstVector4D color;
    pointer material;
    
    void* internal_data;
    
//    GLuint vao;
//    GLuint vbo;
//    GLuint ebo;
};

struct mesh* mesh_alloc(void);

struct mesh* mesh_box(void);

struct mesh* mesh_sphere(const float sectors, const float stacks);

void mesh_process(struct mesh *mesh);

//void glmesh_data(struct glmesh *mesh);
//
//void glmesh_draw(struct glmesh *mesh);
//
//void glmesh_destroy(struct glmesh *mesh);

#endif /* glmesh_h */

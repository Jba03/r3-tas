//
//  glmesh.h
//  r3lib
//
//  Created by Jba03 on 2022-11-23.
//

#ifndef glmesh_h
#define glmesh_h

#include "stMatrix4D.hh"
#include "stTransform.hh"
#include "stCollideMaterial.hh"
#include "stSuperObject.hh"

struct vertex
{
    struct { float x, y, z, w; } position;
    struct { float x, y, z, w; } normal;
    struct { float x, y, z, w; } texcoord;
};

struct mesh
{
    const char* name;
    
    unsigned n_vertices;
    struct vertex* vertices;
    
    unsigned n_indices;
    unsigned* indices;
    
    stSuperObject* superobject;
    stTransform* transform_global;
    stVector4D color;
    pointer<> material;
    
    void* internal_data;
    
//    GLuint vao;
//    GLuint vbo;
//    GLuint ebo;
};

struct mesh* mesh_alloc(void);

struct mesh* mesh_box(void);

struct mesh* mesh_sphere(const float sectors, const float stacks);

void mesh_process(struct mesh *mesh);

void sphereMeshGen(const float resolution, unsigned *vertexCount, stVector3D **vertices, stVector3D **normals, unsigned *indexCount, unsigned **indices);

//void glmesh_data(struct glmesh *mesh);
//
//void glmesh_draw(struct glmesh *mesh);
//
//void glmesh_destroy(struct glmesh *mesh);

#endif /* glmesh_h */

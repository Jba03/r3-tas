//
//  graphics.h
//  r3lib
//
//  Created by Jba03 on 2022-11-22.
//

#ifndef graphics_h
#define graphics_h

namespace graphics
{
  bool initialize();
  void beginFrame(float w, float h);
  void endFrame();
  void *texture();
  
  extern stMatrix4D viewMatrix;
  extern stMatrix4D projectionMatrix;
    
  // Primitive
  auto drawPoint(stVector3D point, stVector4D color) -> void;
  auto drawSphere(stVector3D point, float radius, stVector4D color) -> void;
  auto drawLine(stVector3D p1, stVector3D p2, stVector4D color) -> void;
  auto drawSphere(stVector3D point, float radius, stVector4D color) -> void;
  
  // Object
  auto drawActor(stEngineObject *actor) -> void;
  auto drawIPO(stInstantiatedPhysicalObject *IPO, stMatrix4D T) -> void;
  auto drawGraph(stGraph *graph) -> void;
  
  extern std::vector<pointer<>> highlightList;
    
  /* Provided by dolphin */
  extern void (*createTexture)(const char *name, unsigned width, unsigned height, unsigned samples);
  extern void (*updateTexture)(const char *name, unsigned width, unsigned height, unsigned row_length, uint8_t *buffer, size_t buffer_size);
  extern void (*uploadTexture)(const char *name, void* texture, unsigned width, unsigned height);
  extern void*(*getTexture)(const char *name);
  extern float* (*getProjectionMatrix)(void);
  
  auto initializePrimitives() -> void;
};


//#include "vector3.hh"
//#include "stVector4D.hh"
//#include "mesh.hh"


//struct visualobject2
//{
//    struct vertex* vertices;
//    unsigned int n_vertices;
//
//    unsigned int* indices;
//    unsigned int n_indices;
//
//    stVector4D sphere_position;
//};
//
//void graphics_init(void);
//
//void graphics_load(void);
//
//void graphics_loop(void);
//
//void graphics_unload(void);
//
//void graphics_set_viewport(int width, int height);
//
//void graphics_draw_box(stVector3D position, stVector3D scale, stVector3D rotation, stVector4D color);
//
//void graphics_draw_line(stVector3D start, stVector3D end);
//
//void graphics_draw_triangle(stVector3D a, stVector3D b, stVector3D c);
//
//void graphics_draw_sphere(stVector3D center, const float radius, stVector4D color);
//
//int graphics_shader_id(void);
//
//void* graphics_get_texture(void);
//
//void graphics_load_meshes(void);

#endif /* graphics_h */

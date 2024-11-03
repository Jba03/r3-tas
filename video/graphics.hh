#pragma once

#include <cpatools/cpatools.hpp>

#define DEFAULT_COLOR cpa::structure::stVector4D(1.0f, 1.0f, 1.0f, 1.0f)

template<typename T>
struct DolphinAbstractTexture {
  uint32_t width;
  uint32_t height = 0;
  uint32_t levels = 1;
  uint32_t layers = 1;
  uint32_t samples = 1;
  uint32_t format = 1;
  uint32_t flags = 0;
  uint32_t type = 0;
  uint32_t typ2e = 0;
  T data;
  /* 8 bytes of padding */
};

struct GraphicsContext {
  GraphicsContext();
  
  enum PrimitiveType {
    Points,
    Lines,
    Triangles,
  };
  
  struct Sphere {
    cpa::structure::stVector3D point;
    float radius;
  };
  
  // Draw
  virtual auto beginFrame(void* subtex = nullptr) -> void = 0;
  virtual auto draw(const PrimitiveType primitive, uint32_t vertexCount, cpa::structure::stVector3D* vertices) -> void = 0;
  virtual auto drawIndexed(const PrimitiveType primitive, uint32_t vertexCount, cpa::structure::stVector3D* vertices, cpa::structure::stVector3D* normals, uint32_t indexCount, void* indices) -> void = 0;
  virtual auto endFrame() -> void = 0;
  virtual auto texture() -> void* = 0;
  
  // Uniform
  virtual auto setViewMatrix(const cpa::structure::stMatrix4D& view) -> void = 0;
  virtual auto setProjectionMatrix(const cpa::structure::stMatrix4D& projection) -> void = 0;
  virtual auto setModelMatrix(const cpa::structure::stMatrix4D& model) -> void = 0;
  virtual auto setUseGameIndexing(bool value) -> void = 0;
  virtual auto setUseCheckerTexture(bool value) -> void = 0;
  virtual auto setUseFaceNormals(bool value) -> void = 0;
  virtual auto setDisableShading(bool value) -> void = 0;
  virtual auto setColor(cpa::structure::stVector4D color) -> void = 0;
  virtual auto setCameraPosition(cpa::structure::stVector3D pos) -> void = 0;
  
  // Primitives
  auto drawPoint(cpa::structure::stVector3D point) -> void;
  auto drawSphere(cpa::structure::stVector3D point, float radius) -> void;
  auto drawLine(cpa::structure::stVector3D p0, cpa::structure::stVector3D p1, cpa::structure::stVector4D color) -> void;
  
  // Object
  auto drawActor(cpa::structure::stEngineObject *actor) -> void;
  auto drawIPO(cpa::structure::stInstantiatedPhysicalObject *IPO, cpa::structure::stMatrix4D T) -> void;
  auto drawGraph(cpa::structure::stGraph *graph) -> void;
  auto drawZDX();
  
  auto submitSpheres() -> void;
  
  std::vector<Sphere> spheres;
  
  cpa::structure::stMatrix4D projection;
};

GraphicsContext* MakeGraphicsContext();

extern GraphicsContext *mainContext;

//bool initialize();
//void beginFrame(void* tex, float w, float h);
//void endFrame();
//void *texture();
//
//extern stMatrix4D viewMatrix;
//extern stMatrix4D projectionMatrix;
//
//// Primitive
//auto drawPoint(stVector3D point, stVector4D color) -> void;
//auto drawSphere(stVector3D point, float radius, stVector4D color) -> void;
//auto drawLine(stVector3D p1, stVector3D p2, stVector4D color) -> void;
//auto drawSphere(stVector3D point, float radius, stVector4D color) -> void;
//
//// Object
//auto drawActor(stEngineObject *actor) -> void;
//auto drawIPO(stInstantiatedPhysicalObject *IPO, stMatrix4D T) -> void;
//auto drawGraph(stGraph *graph) -> void;
//
//// Draw
////auto submitGameIndexed();
//
//auto getCollideMaterialColor(pointer<stCollideMaterial> mat) -> stVector4D;
//
//extern std::vector<pointer<>> highlightList;
//
///* Provided by dolphin */
//extern void (*createTexture)(const char *name, unsigned width, unsigned height, unsigned samples);
//extern void (*updateTexture)(const char *name, unsigned width, unsigned height, unsigned row_length, uint8_t *buffer, size_t buffer_size);
//extern void (*uploadTexture)(const char *name, void* texture, unsigned width, unsigned height);
//extern void*(*getTexture)(const char *name);
//extern float* (*getProjectionMatrix)(void);
//
//auto initializePrimitives() -> void;

//};

//#include "game.hh"
//#include "graphics.hh"

#if defined(__APPLE__)
# include "metal.hh"
#else
# include "opengl.hh"
#endif

#include <iostream>

// Pregenerated primitives
static bool primitivesGenerated = false;
static unsigned sphereVertexCount = 0;
static unsigned sphereIndexCount = 0;
static cpa::structure::stVector3D *sphereVertices = nullptr;
static cpa::structure::stVector3D *sphereNormals = nullptr;
static uint32_t *sphereIndices = nullptr;

static auto initializeStaticSphere() -> void {
  #define N_SECTORS 20
  #define N_STACKS 20

  float const sectorStep = 2.0f * M_PI / N_SECTORS;
  float const stackStep = M_PI / N_STACKS;

  sphereVertexCount = (N_SECTORS + 1) * (N_STACKS + 1);
  sphereIndexCount = N_SECTORS * N_STACKS * 6 - (N_SECTORS * 6);

  try {
    sphereVertices = new cpa::structure::stVector3D[sphereVertexCount];
    sphereNormals = new cpa::structure::stVector3D[sphereVertexCount];
    sphereIndices = new uint32_t[sphereIndexCount];
  } catch (std::bad_alloc& e) {
    std::cout << "failed to generate sphere primitive: " << e.what() << "\n";
    return;
  }
    
  int vertex = 0;
  for (int i = 0; i <= N_STACKS; ++i) {
    float const v = (M_PI / 2.0f) - float(i) * stackStep;
    float const xy = std::cos(v);
    float const z = std::sin(v);
    
    for (int j = 0; j <= N_SECTORS; ++j) {
      float const u = j * sectorStep;
      float const x = xy * std::cos(u);
      float const y = xy * std::sin(u);
      
      sphereVertices[vertex] = cpa::structure::stVector3D(x, y, z);
      sphereNormals[vertex] = cpa::structure::stVector3D(x, y, z);
      vertex++;
    }
  }

  int index = 0;
  for (int i = 0; i < N_STACKS; ++i) {
    int a = i * (N_SECTORS + 1);
    int b = a + N_SECTORS + 1;
    
    for (int j = 0; j < N_SECTORS; ++j, ++a, ++b) {
      if (i != 0) {
        sphereIndices[index++] = a;
        sphereIndices[index++] = b;
        sphereIndices[index++] = a + 1;
      }
      
      if (i != (N_STACKS - 1)) {
        sphereIndices[index++] = a + 1;
        sphereIndices[index++] = b;
        sphereIndices[index++] = b + 1;
      }
    }
  }
}

// Initialize static primitives
static auto initializePrimitives() -> void {
  if (!primitivesGenerated) {
    initializeStaticSphere();
    primitivesGenerated = true;
    std::cout << "[graphics] initialized primitives\n";
  }
}

GraphicsContext::GraphicsContext() {
  initializePrimitives();
}

//auto drawGraph(stGraph *graph) -> void {
//  if (graph) {
////    graph->nodes.forEach([&](stGraphNode* node, void*) {
////      //drawSphere(node->wayPoint->point, node->wayPoint->radius, stVector4D(1.0f, 0.0f, 1.0f, 0.25f));
////    });
//  }
//}

auto getCollideMaterialColor(cpa::pointer<cpa::structure::stCollideMaterial> mat) -> cpa::structure::stVector4D {
  cpa::structure::stVector4D color = cpa::structure::stVector4D(0.75f, 0.75f, 0.75f, 1.0f);
  uint16_t identifier = mat->identifier;
  if (identifier & COL_MAT_ID_MASK_NO_COLLISION) color = cpa::structure::stVector4D(0.1f, 1.0f, 1.0f, 0.3f);
  //if (identifier & COL_MAT_ID_MASK_TRAMPOLINE) color = stVector4D(0.0f, 0.0f, 1.0f, 1.0f);
  if (identifier & COL_MAT_ID_MASK_WATER) color = cpa::structure::stVector4D(0.0f, 1.0f, 1.0f, 0.5f);
  if (identifier & COL_MAT_ID_MASK_WALL) color = cpa::structure::stVector4D(0.75f, 0.75f, 0.75f, 1.0f);
  if (identifier & COL_MAT_ID_MASK_GRABBABLE_LEDGE) color = cpa::structure::stVector4D(0.1f, 1.0f, 0.1f, 1.0f);
  if (identifier & COL_MAT_ID_MASK_UNKNOWN) color = cpa::structure::stVector4D(0.75f, 0.75f, 0.75f, 1.0f);
  
//  if (identifier & COL_MAT_ID_MASK_NO_COLLISION) color = stVector4D(1.0f, 1.0f, 1.0f, 0.3f);
//  if (identifier & COL_MAT_ID_MASK_SLIDE) color = stVector4D(0.0f, 0.0f, 1.0f, 1.0f);
//  if (identifier & COL_MAT_ID_MASK_WATER) color = stVector4D(0, 1.0f, 1.0f, 0.5f);
//  if (identifier & COL_MAT_ID_MASK_CLIMBABLE_WALL) color = stVector4D(244.0f / 255.0f, 131.0f / 255.0f, 66.0f / 255.0f, 1.0f);
//  if (identifier & COL_MAT_ID_MASK_HANGABLE_CEILING) color = stVector4D(244.0f / 255.0f, 131.0f / 255.0f, 66.0f / 255.0f, 1.0f);
//  if (identifier & COL_MAT_ID_MASK_LAVA_DEATH_WARP) color = stVector4D(1.0f, 0.0f, 0.0f, 1.0f);
//  if (identifier & COL_MAT_ID_MASK_DEATH_WARP) color = stVector4D(1.0f, 0.0f, 0.0f, 1.0f);
  //if (identifier & COL_MAT_ID_MASK_GRABBABLE_LEDGE) color = stVector4D(0.0f, 1.0f, 0.0f, 1.0f);
  //if ()
  return color;
}

auto GraphicsContext::drawSphere(cpa::structure::stVector3D point, float radius) -> void {
  setUseGameIndexing(false);
  setModelMatrix(cpa::structure::stMatrix4D::make_translation(point) * cpa::structure::stMatrix4D::make_scale(cpa::structure::stVector3D(radius, radius, radius)));
  drawIndexed(Triangles, sphereVertexCount, sphereVertices, sphereNormals, sphereIndexCount, sphereIndices);
  
  
  
  //spheres.push_back({ point, radius });
}

auto GraphicsContext::submitSpheres() -> void {
  
  
//  setUseGameIndexing(false);
//  setModelMatrix(cpa::structure::stMatrix4D::make_translation(point) * cpa::structure::stMatrix4D::make_scale(cpa::structure::stVector3D(radius, radius, radius)));
//  drawIndexed(Triangles, sphereVertexCount, sphereVertices, sphereNormals, sphereIndexCount, sphereIndices);
  
  spheres.clear();
}

auto GraphicsContext::drawLine(cpa::structure::stVector3D p0, cpa::structure::stVector3D p1, cpa::structure::stVector4D color) -> void {
  cpa::structure::stVector3D vertices[2] = { {p0.x(), p0.y(), p0.z()}, {p1.x(), p1.y(), p1.z()} };
  setModelMatrix(cpa::structure::stMatrix4D());
  setColor(color);
  draw(Lines, 2, vertices);
}

auto GraphicsContext::drawIPO(cpa::structure::stInstantiatedPhysicalObject *ipo, cpa::structure::stMatrix4D T) -> void {
  try {
    cpa::pointer<cpa::structure::stCollideObject> zdr = ipo->physicalObject->physicalCollideset->zdr;
    for (int i = 0; i < zdr->numElements; i++) {
      int16_t type = zdr->elementTypes[i];
      if (type == collideElementTypeIndexedTriangles) {
        cpa::pointer<cpa::structure::stCollideElementIndexedTriangles> mesh = zdr->elements[i];
        if (!mesh) continue;

        cpa::structure::stVector3D* vertices = zdr->vertices;
        cpa::structure::stVector3D* normals = mesh->normals;
        cpa::uint16* indices = mesh->faceIndices;

        cpa::int16 numVertices = zdr->numVertices;
        cpa::int16 numFaces = mesh->numFaces;

        cpa::pointer<cpa::structure::stCollideMaterial> material = mesh->material;

        cpa::structure::stVector4D mat = getCollideMaterialColor(material);
        
        setColor(mat);
        setModelMatrix(T);
        setUseGameIndexing(true);
        setUseFaceNormals(true);
        setUseCheckerTexture(true);
        
        drawIndexed(Triangles, numVertices, vertices, normals, int(numFaces) * 3, indices);
      }
    }
  } catch (...) {

  }
}

GraphicsContext *mainContext;

GraphicsContext* MakeGraphicsContext() {
#if defined(__APPLE__)
  return mainContext = new MetalGraphicsContext();
#else
  
#endif
}

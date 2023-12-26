//
//  graphics.cpp
//  r3-tas-memorymap
//
//  Created by Jba03 on 2023-08-22.
//

#include "game.hh"
#include "graphics.hh"

static unsigned sphereVertexCount = 0;
static unsigned sphereIndexCount = 0;
static stVector3D *sphereVertices = nullptr;
static stVector3D *sphereNormals = nullptr;
static uint16 *sphereIndices = nullptr;

namespace graphics {
  
  auto draw(stVector3D *vertices, stVector3D *normals, uint16 *indices, unsigned numVertices, unsigned numIndices, stMatrix4D T = stMatrix4D(), stVector4D color = stVector4D(1.0f, 1.0f, 1.0f, 1.0f)) -> void;
  
  auto initializePrimitives() -> void {
    #define N_SECTORS 20
    #define N_STACKS 20
    
    float const sectorStep = 2.0f * M_PI / N_SECTORS;
    float const stackStep = M_PI / N_STACKS;
    
    sphereVertexCount = (N_SECTORS + 1) * (N_STACKS + 1);
    sphereIndexCount = N_SECTORS * N_STACKS * 6 - (N_SECTORS * 6);
    
    sphereVertices = new stVector3D[sphereVertexCount];
    sphereNormals = new stVector3D[sphereVertexCount];
    sphereIndices = new uint16[sphereIndexCount];
    
    int vertex = 0;
    for (int i = 0; i <= N_STACKS; ++i) {
      float const v = (M_PI / 2.0f) - float(i) * stackStep;
      float const xy = std::cos(v);
      float const z = std::sin(v);
      
      for (int j = 0; j <= N_SECTORS; ++j) {
        float const u = j * sectorStep;
        float const x = xy * std::cos(u);
        float const y = xy * std::sin(u);
        
        sphereVertices[vertex] = stVector3D(x, y, z);
        sphereNormals[vertex] = stVector3D(x, y, z);
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
  
  auto drawSphere(stVector3D point, float radius, stVector4D color) -> void {
    stMatrix4D T = stMatrix4D::translation(point) * stMatrix4D::scale(radius, radius, radius);
    draw(sphereVertices, sphereNormals, sphereIndices, sphereVertexCount, sphereIndexCount, T, color);
  }
  
  auto drawGraph(stGraph *graph) -> void {
    if (graph) {
      graph->nodes.forEach([&](stGraphNode* node, void*) {
        drawSphere(node->wayPoint->point, node->wayPoint->radius, stVector4D(1.0f, 0.0f, 1.0f, 0.25f));
      });
    }
  }
  
}

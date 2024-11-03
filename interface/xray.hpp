// xray: pathfinding
#pragma once

#include <cpatools/cpatools.hpp>

struct xRAY {
  
#pragma mark - xTheta*
  
  struct ThetaNode {
    bool open = false;
    bool closed = false;
   // pointer<stVector3D> vertex;
    stVector3D vertex;
    pointer<stTransform> transform;
    std::vector<ThetaNode*> neighbors;
    ThetaNode* parent;
    
    float score = 0.0f;
    long io = 0;
    long ic = 0;
    
    inline auto distanceTo(ThetaNode* other) -> float {
      return (vertex - other->vertex).length();
     // return (*(stVector3D*)vertex - *(stVector3D*)other->vertex).length();
    }
    
    auto position() -> stVector3D { return vertex; } //*(stVector3D*)vertex; }
  };
  
  #pragma mark Heuristic rules
  
  //consider a roll from s to neighbor if distance allows for it
  auto considerRoll(ThetaNode* s, ThetaNode* neighbor, bool& state) {
    stVector3D a = s->position();
    stVector3D b = neighbor->position();
    //height difference
    float hdiff = float(a.z()) - float(b.z());
  }
  
  auto considerNeighbor(ThetaNode* s, ThetaNode* neighbor) -> bool {
    bool state = true;
    considerRoll(s, neighbor, state);
    return state;
  }

  auto thetaLineOfSight(ThetaNode* a, ThetaNode* b) -> bool {
    return true;
  }
  
  auto thetaUpdateVertex(std::vector<ThetaNode*>& open, ThetaNode* s, ThetaNode* neighbor, ThetaNode* start, ThetaNode* end) -> void {
    if (thetaLineOfSight(s->parent, neighbor)) {
      float d = s->parent->distanceTo(neighbor);
      if (s->parent->score + d < neighbor->score) {
        neighbor->score = s->parent->distanceTo(start) + s->parent->score + d;
        neighbor->parent = s->parent;
        if (neighbor->open) {
          open.erase(open.begin() + neighbor->io);
          neighbor->open = false;
        }
        // neighbor->score += heuristic()
        if (considerNeighbor(s, neighbor)) {
          neighbor->open = true;
          neighbor->io = open.size();
          open.emplace_back(neighbor);
        }
      }
    } else {
      float d = s->distanceTo(neighbor);
      if (s->score + d < neighbor->score) {
        neighbor->score = s->distanceTo(start) + s->score + d;
        neighbor->parent = s;
        if (neighbor->open) {
          open.erase(open.begin() + neighbor->io);
          neighbor->open = false;
        }
        // neighbor->score += heuristic()
        if (considerNeighbor(s, neighbor)) {
          neighbor->open = true;
          neighbor->io = open.size();
          open.emplace_back(neighbor);
        }
      }
    }
  }
  
  auto thetaReconstructPath(ThetaNode* s, std::vector<ThetaNode*>& path) -> void {
    path.emplace_back(s);
    if (s->parent != s) thetaReconstructPath(s->parent, path);
  }
  
  auto theta(ThetaNode* start, ThetaNode* end) -> std::vector<ThetaNode*> {
    std::vector<ThetaNode*> open, closed;
    start->score = start->distanceTo(end);
    start->parent = start;
    start->open = true;
    start->closed = false;
    open.emplace_back(start);
    
    std::vector<ThetaNode*> path;
    while (open.size() != 0) {
      ThetaNode* s = open.back();
      open.pop_back();
      
      if (s == end) {
        thetaReconstructPath(s, path);
        break;
      }
      
      s->closed = true;
      closed.emplace_back(s);
      
      for (auto& neighbor : s->neighbors) {
        if (!neighbor->closed) {
          if (!neighbor->open) {
            neighbor->score = INFINITY;
            neighbor->parent = nullptr;
          }
          
          thetaUpdateVertex(open, s, neighbor, start, end);
        }
      }
    }
    
    return path;
  }
  
#pragma mark -
  
  class Triangle {
  public:
    Triangle(pointer<stSuperObject> _superobject, pointer<stCollideObject> _colObj, pointer<stCollideElementIndexedTriangles> _set, unsigned long _idx) :
    superobject(_superobject), collideObject(_colObj), set(_set), index(_idx) { /* ... */ }
    //superobject the triangle belongs to
    pointer<stSuperObject> superobject;
    //collide element the triangle belongs to
    pointer<stCollideObject> collideObject;
    //set (within collide object above) the triangle belongs to
    pointer<stCollideElementIndexedTriangles> set;
    //index into collide object
    unsigned long index;
    
  public:
    //get superobject
    auto spo() { return superobject; }
    //get index
    auto idx() { return index; }
    //is the triangle a wall? (>45°)
    auto isWall() -> bool { return set->material->identifier & COL_MAT_ID_MASK_WALL; }
    //vertices
    auto vertices() -> std::array<stVector3D, 3> {
      uint16 idx0 = *((uint16*)set->faceIndices + index * 3 + 0);
      uint16 idx1 = *((uint16*)set->faceIndices + index * 3 + 1);
      uint16 idx2 = *((uint16*)set->faceIndices + index * 3 + 2);
      
      stVector3D a = *((stVector3D*)collideObject->vertices + idx0);
      stVector3D b = *((stVector3D*)collideObject->vertices + idx1);
      stVector3D c = *((stVector3D*)collideObject->vertices + idx2);
      
      stVector3D Ta = (superobject->globalTransform->matrix * a).xyz();
      stVector3D Tb = (superobject->globalTransform->matrix * b).xyz();
      stVector3D Tc = (superobject->globalTransform->matrix * c).xyz();
      
      return { Ta, Tb, Tc };
    }
    // adjacent triangles (triangles which contain the same vertices)
    auto adjacent() -> std::vector<Triangle> {
      uint16_t idx0 = *((uint16*)set->faceIndices + index * 3 + 0);
      uint16_t idx1 = *((uint16*)set->faceIndices + index * 3 + 1);
      uint16_t idx2 = *((uint16*)set->faceIndices + index * 3 + 2);
      
      std::vector<Triangle> out;
      for (int16_t i = 0; i < set->numFaces * 3; i++) {
        //if (i != index) {
        uint16_t idx = *((uint16*)set->faceIndices + i * 3 + 0);
        if (idx == idx0 || idx == idx1 || idx == idx2) {
          Triangle t(superobject, collideObject, set, idx);
          out.emplace_back(t);
        }
        //}
      }
      return out;
    }
  };
  
  using TriangleList = std::vector<Triangle>;
  
  auto deriveTrianglesReal(pointer<stSuperObject> object, stMatrix4D T, TriangleList& list) {
    try {
      pointer<stCollideObject> zdr = object->ipo->physicalObject->physicalCollideset->zdr;
      for (int i = 0; i < zdr->numElements; i++) {
        int16_t type = zdr->elementTypes[i];
        if (type == collideElementTypeIndexedTriangles) {
          pointer<stCollideElementIndexedTriangles> element = zdr->elements[i];
          for (int16_t index = 0; index < element->numFaces; index++) {
            Triangle t(object, zdr, element, index);
            list.emplace_back(t);
          }
        }
      }
    } catch (bad_pointer& e) {
      //std::cout << "[xray] failed to derive triangles for '" + object->name() + "': " + e.what() + "\n";
    }
  }
  
  auto deriveTrianglesRecursive(pointer<stSuperObject> object, stMatrix4D T, TriangleList& list) {
    try {
      if (!object) return;
      T = object->globalTransform->matrix * T;
      if (object->type == superobjectTypeIPO) deriveTrianglesReal(object, T, list);
      object->forEachChild([&](pointer<stSuperObject> _object, void*) { deriveTrianglesRecursive(_object, T, list); });
    } catch (bad_pointer& e) {
      //std::cout << "[xray] failed to derive triangles (recursively) for '" + object->name() + "': " + e.what() + "\n";
    }
  }
  
  auto deriveTriangles(pointer<stSuperObject> object) -> TriangleList {
    TriangleList list;
    deriveTrianglesRecursive(object, object->globalTransform->matrix, list);
    return list;
  }
  
  auto derivePointsReal(pointer<stSuperObject> object, stMatrix4D T, std::vector<stVector3D>& list) {
    try {
      pointer<stCollideObject> zdr = object->ipo->physicalObject->physicalCollideset->zdr;
      for (int i = 0; i < zdr->numElements; i++) {
        int16_t type = zdr->elementTypes[i];
        if (type == collideElementTypeIndexedTriangles) {
          //pointer<stCollideElementIndexedTriangles> tris = zdr->elements[i];
          //if (!(tris->material->identifier & COL_MAT_ID_MASK_WALL)) {
//            for (int vertex = 0; vertex < zdr->numVertices; vertex++) {
//              stVector3D vert = *(stVector3D*)zdr->vertices[vertex];
//              stVector3D v = (T * vert).xyz();
//              list.push_back(v);
//            }
          //}
        }
      }
    } catch (bad_pointer& e) {
      //std::cout << "[xray] failed to derive points for '" + object->name() + "': " + e.what() + "\n";
    }
  }
  
  auto derivePointsRecursive(pointer<stSuperObject> object, stMatrix4D T, std::vector<stVector3D>& list) {
    if (!object) return;
    try {
      T = object->globalTransform->matrix * T;
      if (object->type == superobjectTypeIPO) derivePointsReal(object, T, list);
      object->forEachChild([&](pointer<stSuperObject> _object, void*) { derivePointsRecursive(_object, T, list); });
    } catch (bad_pointer& e) {
      //std::cout << "[xray] failed to derive points (recursively) for '" + object->name() + "': " + e.what() + "\n";
    }
  }
  
  auto derivePoints(pointer<stSuperObject> object) -> std::vector<stVector3D> {
    std::vector<stVector3D> list;
    derivePointsRecursive(object, object->globalTransform->matrix, list);
    return list;
  }
  
#pragma mark -
  
  auto thetaNodeSetCreate(TriangleList& list) {
    for (auto& triangle : list) {
      //could use a radius-based neighbor search here,
      //but it would include walls, which we can not walk on
      for (auto& adj : triangle.adjacent()) {
        
      }
    }
  }
  
  auto thetaNodeSetCreate(pointer<stSuperObject> object) -> std::vector<stVector3D> {
    return derivePoints(object);;
  }
  
  
};

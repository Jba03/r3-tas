// Theta* pathfinding algorithm
#pragma once

#include <vector>
#include <functional>

//auto thetaLineOfSight(ThetaNode* a, ThetaNode* b) -> bool {
//  bool lineOfSight = true;
//  stVector3D intersectionPoint;
//  thetaLineOfSightRecursive(sectorSearch(p_stFatherSector, g_stEngineStructure->mainCharacterList[]->position()), stMatrix4D(), a, b, lineOfSight, intersectionPoint);
//  return lineOfSight;
//}
//
//auto thetaUpdateVertex(std::vector<ThetaNode*>& open, ThetaNode* s, ThetaNode* neighbor, ThetaNode* start, ThetaNode* end) -> void {
//  if (thetaLineOfSight(s->parent, neighbor)) {
//    float d = s->parent->distanceTo(neighbor);
//    if (s->parent->score + d < neighbor->score) {
//      neighbor->score = s->parent->distanceTo(start) + s->parent->score + d;
//      neighbor->parent = s->parent;
//      if (neighbor->open) {
//        open.erase(open.begin() + neighbor->io);
//        neighbor->open = false;
//      }
//      // neighbor->score += heuristic()
//      if (considerNeighbor(s, neighbor)) {
//        neighbor->open = true;
//        neighbor->io = open.size();
//        open.emplace_back(neighbor);
//      }
//    }
//  } else {
//    float d = s->distanceTo(neighbor);
//    if (s->score + d < neighbor->score) {
//      neighbor->score = s->distanceTo(start) + s->score + d;
//      neighbor->parent = s;
//      if (neighbor->open) {
//        open.erase(open.begin() + neighbor->io);
//        neighbor->open = false;
//      }
//      // neighbor->score += heuristic()
//      if (considerNeighbor(s, neighbor)) {
//        neighbor->open = true;
//        neighbor->io = open.size();
//        open.emplace_back(neighbor);
//      }
//    }
//  }
//}
//
//auto thetaReconstructPath(ThetaNode* s, std::vector<ThetaNode*>& path) -> void {
//  path.emplace_back(s);
//  if (s->parent != s) thetaReconstructPath(s->parent, path);
//}
//
//auto theta(ThetaNode* start, ThetaNode* end) -> std::vector<ThetaNode*> {
//  std::vector<ThetaNode*> open, closed;
//  start->score = start->distanceTo(end);
//  start->parent = start;
//  start->open = true;
//  start->closed = false;
//  open.emplace_back(start);
//
//  std::vector<ThetaNode*> path;
//  while (open.size() != 0) {
//    ThetaNode* s = open.back();
//    open.pop_back();
//
//    if (s == end) {
//      thetaReconstructPath(s, path);
//      break;
//    }
//
//    s->closed = true;
//    closed.emplace_back(s);
//
//    for (auto& neighbor : s->neighbors) {
//      if (!neighbor->closed) {
//        if (!neighbor->open) {
//          neighbor->score = INFINITY;
//          neighbor->parent = nullptr;
//        }
//
//        thetaUpdateVertex(open, s, neighbor, start, end);
//      }
//    }
//  }
//
//  return path;
//}

template<typename State>
class Theta {
public:
  class Node {
  public:
    //user state
    State state;
    //parent node
    Node* parent = nullptr;
    //neighboring nodes
    std::vector<Node*> neighbors;
    int io = 0;
    //in closed set?
    bool closed = false;
    //in open set?
    bool open = false;
    //distance from start to here
    float g = 0.0f;
    //sum of total distance (start to here + here to end)
    float gh = 0.0f;
  };
  
  using Heuristic = std::function<float(Node* node)>;
  using LineOfSight = std::function<float(Node* a, Node* b)>;
  using NeighborSelection = std::function<bool(Node* current, Node* neighbor)>;
  
  Theta(Node* _start, Node* _goal, LineOfSight _los, NeighborSelection _sel, Heuristic _heuristic = nullptr) :
  start(_start), goal(_goal), lineOfSight(_los), considerNeighbor(_sel), heuristic(_heuristic) { /* ... */ }
  
  auto updateVertex(std::vector<Node*>& open, Node* s, Node* neighbor, Node* start, Node* end) -> void {
    if (lineOfSight(s->parent, neighbor)) {
      float d = s->parent->state.distance_to(neighbor->state);
      if (s->parent->score + d < neighbor->score) {
        neighbor->score = s->parent->state.distance_to(start->state) + s->parent->score + d;
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
      float d = s->state.distance_to(neighbor->state);
      if (s->score + d < neighbor->score) {
        neighbor->score = s->state.distance_to(start->state) + s->score + d;
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
  
  auto reconstructPath(Node* s, std::vector<Node*>& _path) -> void {
    _path.emplace_back(s);
    if (s->parent != s) reconstructPath(s->parent, _path);
  }
  
  auto run() -> std::vector<Node*> {
    std::vector<Node*> open, closed;
    start->score = start->state.distance_to(goal->state);
    start->parent = start;
    start->open = true;
    start->closed = false;
    start->io = 0;
    open.emplace_back(start);
    
    std::vector<Node*> path;
    while (open.size() != 0) {
      Node* s = open.back();
      open.pop_back();
      if (s == goal) {
        reconstructPath(s, path);
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
          
          updateVertex(open, s, neighbor, start, goal);
        }
      }
    }
    
    return path;
  }
  
private:
  Node* start;
  Node* goal;
  //std::vector<Node*> open; //open set
  //std::vector<Node*> closed; //closed set
  //std::vector<Node*> path; //final path
  Heuristic heuristic;
  LineOfSight lineOfSight;
  NeighborSelection considerNeighbor;
};

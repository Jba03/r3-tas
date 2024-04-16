#pragma once

#include "game.hh"

static const std::vector<std::string> CollisionEntityNames {
  "PointEntity",
  "TriangleEntity",
  "EdgeEntity",
  "SphereEntity",
  "AlignedBoxEntity",
  "UnknownEntity5",
  "UnknownEntity6",
  "HighEdgeEntity",
  "LowEdgeEntity",
};

struct CollisionTableEntry {
  stVector3D tempDistance;
  stVector3D tempHit;
  stVector3D tempNormal;
  pointer<stCollideMaterial> dynamicMaterial;
  pointer<stCollideMaterial> staticMaterial;
  int16_t entity1;
  int16_t entity2;
  float radius;
};

extern std::vector<CollisionTableEntry> collisionTableEntries;

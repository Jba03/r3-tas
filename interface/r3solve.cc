/************************************************************
 * r3solve.cc: Brute-force solver for horizontal teleports
 ************************************************************/

#include "game.hh"
#include "hook.hh"
#include "interface.hh"
#include "log.hh"

// Joypad vector converted to worldspace coordinates
static stVector3D PadGlobalVector = stVector3D(100.0f, 0.0f, 0.0f);

#define radians(degrees)  ((degrees) * (M_PI / 180.0f))
#define degrees(radians)  ((radians) * (180.0f / M_PI))

struct State {
  stDynamics dynamics;
  stDynamicsReport dynamicsReport;
  float dt;
};

bool wantsJump = false;

static void LoadState(const State& s) {
  
}

static void SaveState(const State& s) {
  
}

static pointer<stDynamics> dynamics;

#pragma mark - Virtual script functions

static bool CollideWithCeiling() {
  return dynamics->base.report->currentSurfaceState & dynamicsObstacleTypeCeiling;
}

static bool CollideWithWall() {
  return dynamics->base.report->currentSurfaceState & dynamicsObstacleTypeWall;
}

static bool CollideWithGround() {
  return dynamics->base.report->currentSurfaceState & dynamicsObstacleTypeGround;
}

static stVector3D GetPersoSighting() {
  stVector3D i, j, k;
  pointer<stSuperObject> mainCharacter = g_stEngineStructure->currentMainPlayers[0];
  mainCharacter->globalTransform->getRotation(i, j, k);
  printf("sighting: %f %f %f\n", float(j.x()) ,float(j.y()), float(j.z()));
  return -j;
}

static stVector3D VectorContribution(stVector3D a, stVector3D b) {
  if (a.isNullVector()) return a;
  a = a.normalize();
  return a * a.dot(b);
}

#pragma mark - Macro

static void SautCalculeDirectionDeDeplacement(stVector3D& dsgVar22, float dsgVar24) {
  if (/* strafing && isValidObject(dsgVar147) */ false) {
    
  } else {
    dsgVar22 = VectorContribution(PadGlobalVector, GetPersoSighting()) * 10.0f;// * (dsgVar24 * 0.01f);
  }
}

static void SautGereLaVitesseRelativeEtLInertie(stVector3D& dsgVar22) {
  dynamics->base.flags |= dynamicsFlagSpeedProposeAbsolute;
//  dynamics->base.flags |= dynamicsFlagSpeedProposeX;
//  dynamics->base.flags |= dynamicsFlagSpeedProposeY;
  dynamics->base.proposeSpeed = dsgVar22;
  printf("proposed: %f %f %f\n", float(dsgVar22.x()) ,float(dsgVar22.y()), float(dsgVar22.z()));
}

#pragma mark - Behavior

struct WalkBehaviorState {
  bool isWalking;
};

struct JumpBehaviorState {
  int jumpTimer;
  bool isJumping = false;
  bool falling = false;
};

struct BehaviorState {
  WalkBehaviorState walkState;
  JumpBehaviorState jumpState;
} state;

static void WalkBehavior(WalkBehaviorState& s, float dt) {
  
}

#pragma mark - Jump behavior

static void Saut1Impulsion() {
  
}

static void Saut2Montee() {
  
}

static void Saut3Sommet2() {
  
}

static void Saut4Reception() {
  
}

static void JumpBehavior(JumpBehaviorState& s, float dt) {
  if (!s.isJumping) {
    s.isJumping = true;
    s.jumpTimer = 0;
    s.falling = false;
  }
  
  if (s.jumpTimer <= 64) {
    /* YAM_C_Saut1Impulsion */
    dynamics->base.flags = 7899014;
    dynamics->base.gravity = 25.0f;
    dynamics->advanced.inertia = stVector3D(0.0f, 0.0f, 1.0f);
    dynamics->advanced.previousSlide = 0.0f;
    dynamics->base.imposeSpeed.z() = 12.0f;
    printf("YAM_C_Saut1Impulsion\n");
  } else if (s.jumpTimer > 64 && s.jumpTimer < 200) {
    dynamics->base.flags = 7899014;
    /* YAM_C_Saut2Montee */
    dynamics->base.imposeSpeed.z() = 12.0f;
    printf("YAM_C_Saut2Montee\n");
  } else if (s.jumpTimer >= 200) {
    if (!s.falling) {
      /* YAM_C_Saut3Sommet */
      dynamics->base.flags = 7374726;
      //dynamics->base.imposeSpeed.z() = 12.0f;
      dynamics->base.gravity = 30.0f;
      dynamics->advanced.inertia = stVector3D(0.4f, 0.4f, 1.0f);
      if (dynamics->base.previousSpeed.z() < 0.0f) {
        dynamics->base.gravity = 40.0f;
        s.falling = true;
      }
      printf("YAM_C_Saut3Sommet\n");
    } else  {
      dynamics->base.flags = 7374726;
      dynamics->base.endFlags = 25190407;
      if (CollideWithGround()) {
        dynamics->base.flags = 0xA25;
        dynamics->base.endFlags = 0x80E007;
        printf("collide with ground\n\n");
      }
      printf("YAM_C_Saut4Reception\n");
    }
  }
  
  stVector3D dsgVar22 = stVector3D(0.0f, 0.0f, 0.0f);
  SautCalculeDirectionDeDeplacement(dsgVar22, 8.0f);
  SautGereLaVitesseRelativeEtLInertie(dsgVar22);
  
  printf("surface state: %X\n", int(dynamics->base.report->currentSurfaceState));
  
  printf("timer: %d\n", s.jumpTimer);
  s.jumpTimer += dt * 1000.0f;
}

static void PrintVector(stVector3D v) {
  printf("(%f, %f, %f)\n", float(v.x()), float(v.y()), float(v.z()));
}

static void PrintDynamics(stDynamics dynamics) {
  serializer s(serializer::Mode::Save);
  serializer::node nd(&s);
  dynamics.serialize(nd);

  printf("%s\n\n", nd.data.dump(2).c_str());
}

static int i = 0;

struct Edge {
  pointer<stSuperObject> superObject;
  pointer<stCollideObject> collideObj;
  pointer<stCollideElementIndexedTriangles> element;
  stVector3D a;
  stVector3D b;
  stVector3D normal;
  int16_t triangleIndex;
  int16_t edgeIndex;
};

static bool IsEdgeVertical(stVector3D a, stVector3D b) {
//  stVector2D q = a.xy() - b.xy();
//  float θ = degrees(fabs(atan2(q.x(), q.y())));
//  return (θ >= 0.0f && θ <= 45.0f) || (θ >= 135.0f && θ <= 180.0f);
  
  stVector3D d = a - b;
  stVector3D Up = stVector3D(0.0f, 0.0f, 1.0f);
  float angle = degrees(acos(d.dot(Up) / (d.length() * Up.length())));
  return angle >= -45.0f && angle <= 45.0f;
  //return (a - b).dot(Up) >= float(dynamics->base.slopeCosine);
}

static void FilterCandidates(std::vector<Edge>& edges) {
  for (auto it = edges.begin(); it != edges.end(); it++) {
    auto& edge = *it;
    if (!IsEdgeVertical(edge.a, edge.b)) {
      edges.erase(it);
    }
  }
}

static void EdgeValidation() {
  
}

// The behavior when edge collision occurs (z shifts from + to -)
static void Saut3Sommet() {
  dynamics->base.flags = 7374726;
  //dynamics->base.imposeSpeed.z() = 12.0f;
  dynamics->base.gravity = 30.0f;
  dynamics->advanced.inertia = stVector3D(0.4f, 0.4f, 1.0f);
  if (dynamics->base.previousSpeed.z() < 0.0f) {
    dynamics->base.gravity = 40.0f;
  }
}

#pragma mark - Bruteforcing

static bool BruteforceStarted = false;
static bool BruteforceFinished = false;

static pointer<stSuperObject> selectedIPO = nullptr;
static pointer<stCollideObject> collideObject = nullptr;
//static pointer<stCollideElementIndexedTriangles> element = nullptr;

static const stVector3D targetVector = stVector3D(1.0f, 0.0f, 0.0f);


// Edges selected in the initial filter test
static std::vector<Edge> selectedEdges;
// Current edge being tested
static std::vector<Edge>::iterator currentEdge;
// Current position along the edge
static float currentEdgePosition = 0.5f;
// Advance position by this much
static float Advancement = 0.025f;
// Current test angle (degrees)
static float testAngle = -80.0f;

static int timer = 0;


// Finds two triangles which have the same edge index
static void FindTrianglePairs() {
}

static bool IsGroundNormal(stVector3D N) {
  stVector3D Up = stVector3D(0.0f, 0.0f, 1.0f);
  return N.dot(Up) >= dynamics->base.slopeCosine;
}

static void SelectEdges() {
  try {
    stTransform T = *selectedIPO->globalTransform;
    pointer<stCollideObject> zdr = selectedIPO->ipo->physicalObject->physicalCollideset->zdr;
//    printf("bruteforce\n");
    printf("ada: %d\n", int16_t(zdr->numElements));
    for (auto elementIndex : range(zdr->numElements)) {
      //printf("ada 2\n");
      int16_t type = zdr->elementTypes[elementIndex];
      if (type == collideElementTypeIndexedTriangles) {
        pointer<stCollideElementIndexedTriangles> element = zdr->elements[elementIndex];
        pointer<stVector3D> vertices = zdr->vertices;
        pointer<uint16> indices = element->faceIndices;
        printf("ada 3\n");
        for (auto index : range(element->numFaces)) {
          uint16 idx0 = indices[index * 3 + 0];
          uint16 idx1 = indices[index * 3 + 1];
          uint16 idx2 = indices[index * 3 + 2];
          
          stVector3D A = vertices[idx0];
          stVector3D B = vertices[idx1];
          stVector3D C = vertices[idx2];
          stVector3D N = vertices[index];
          
          stVector3D TA = T * A;
          stVector3D TB = T * B;
          stVector3D TC = T * C;
       
          
          //printf("dada: %f %f %f\n", float(N.x()), float(N.y()), float(N.z()));
          
          for (auto j : range(3)) {
            stVector3D EdgeA = zdr->vertices[indices[index * 3 + j]];
            stVector3D EdgeB = zdr->vertices[indices[index * 3 + ((j + 1) % 3)]];
            // Is the edge vertical?
            if (IsEdgeVertical(EdgeA, EdgeB)) {
              
              int16_t EdgeIndex = element->edgeIndices[index * 3 + j];
              stVector3D EdgeNormal = element->edgeNormals[EdgeIndex];
              // Angle between edge and triangle normal
              float π = degrees(acos(N.dot(EdgeNormal) / (N.length() * EdgeNormal.length())));
              // In most cases, the best edges are ones that form a convex corner.
              // Assuming the edge normal is the middle vector between the two triangles,
              // the normals of the triangles are mirrored. 90° means a completely flat wall.
              // Choose at minimum a slightly acute angle to get the best of both cases.
              if (/*π <= 80.0f*/ true) {
                // Determine alignment with the target vector
                float θ = degrees(acos(EdgeNormal.dot(targetVector) / (EdgeNormal.length() * targetVector.length())));
                // 90° = perfect alignment
                if (/*θ >= 80.0f && θ <= 100.0f*/ true) {
                  Edge candidate;
                  candidate.superObject = selectedIPO;
                  candidate.collideObj = zdr;
                  candidate.element = element;
                  
                  candidate.triangleIndex = index;
                  candidate.edgeIndex = EdgeIndex;
                  candidate.a = T * EdgeA;
                  candidate.b = T * EdgeB;
                  
                  candidate.normal = N;
                  
                  //selectedEdges.push_back(candidate);
                  
                  // printf("angle: %f, align: %f\n", π, θ);
                }
              }
            }
          }
        }
      }
    }
  } catch (...) {
    
  }
  
  Edge candidate;
  candidate.superObject = selectedIPO;
//  candidate.collideObj = zdr;
//  candidate.element = element;
  
//  candidate.triangleIndex = index;
//  candidate.edgeIndex = EdgeIndex;
  candidate.a = stVector3D(66.170807, -666.294800, 70.438919);
  candidate.b = stVector3D(66.285454, -666.202515, 60.843330);
  
//  stVector3D start = stVector3D(66.170807, -666.294800, 70.438919);
//  stVector3D end = stVector3D(66.285454, -666.202515, 60.843330);
  selectedEdges.push_back(candidate);
  
  currentEdge = selectedEdges.begin();
}

static stVector3D GetEdgePosition() {
  auto& edge = *currentEdge;
  stVector3D Start = edge.b;
  stVector3D End = edge.a;
  return Start + (End - Start).normalize() * currentEdgePosition + edge.normal;
}

static float distanceFromEdge = 0.8f;

static void InitialState() {
  stVector3D Position = GetEdgePosition();
  
  Position = Position + stVector3D(cos(radians(testAngle)) * distanceFromEdge, sin(radians(testAngle)) * distanceFromEdge, 0.0f);
  
  dynamics->base.currentTransform.translation() = Position;
  dynamics->base.previousTransform.translation() = Position;
  g_stEngineStructure->currentMainPlayers[0]->globalTransform->translation() = Position;
  
  //dynamics->base.flags = 0;
  
  
  //g_stEngineStructure->standardCamera->localTransform->matrix = stMatrix4D::make_lookat(Position, Position + stVector3D(5.0f, 5.0f, 5.0f), stVector3D(0.0f, 0.0f, -1.0f));
}

static int frameCounter = 0;

static void ProposeSpeed() {
  stVector3D pos = dynamics->base.currentTransform.translation();
  stVector3D edgePos = GetEdgePosition();
  stVector3D vector = (edgePos - pos).normalize();

  float x = (drand48() - 0.5f) * 2.0f;
  float y = (drand48() - 0.5f) * 2.0f;
  stVector3D speed = stVector3D(x * 8.0f, y * 8.0f, -1.25f);  //vector * 100.0f;
  
  //printf("speed (%f %f %f)\n", float(speed.x()), float(speed.y()), float(speed.z()));
  
  //dynamics->base.flags |= dynamicsFlagSpeedProposeAbsolute;
  dynamics->base.flags = 7374726;
  dynamics->base.endFlags = 25190407;
  //dynamics->base.imposeSpeed.z() = 12.0f;
  //dynamics->base.imposeSpeed.z() = 12.0f;
  dynamics->base.proposeSpeed = speed;
  dynamics->base.previousSpeed = vector;
  dynamics->base.previousSpeed.z() = 1.30385e-05f;
  dynamics->base.gravity = 30.0f;
  dynamics->advanced.inertia = stVector3D(0.4f, 0.4f, 1.0f);
  if (dynamics->base.previousSpeed.z() < 0.0f) {
    dynamics->base.gravity = 40.0f;
    //s.falling = true;
  }
}

static void BruteForce() {
  InitialState();
  
  auto& edge = *currentEdge;
  
  if ((frameCounter % 3) == 0) {
    ProposeSpeed();
    
    if (testAngle >= 80.0f) {
      testAngle = 0.0f;
      if (currentEdgePosition >= (edge.a - edge.b).length()) {
        printf("stop trying edge (%f %f %f) -> (%f %f %f)\n", float(edge.a.x()), float(edge.a.y()), float(edge.a.z()), float(edge.b.x()), float(edge.b.y()), float(edge.b.z()));
        currentEdgePosition = 0.0f;
        currentEdge++;
      }
      currentEdgePosition += Advancement;
    }
    testAngle += 0.1f;
    
    frameCounter = 0;
  }
  
  
  
  frameCounter++;
}

static void Process() {
  if (!BruteforceStarted) {
    srand48(time(nullptr));
    //selectedIPO = p_stFatherSector->firstChild->next->next->next->next->next->next->firstChild;
    selectedIPO = p_stFatherSector->firstChild->next->firstChild;
    printf("name: %s\n", selectedIPO->name().c_str());
    // First, select the edges we want to test
    SelectEdges();
    BruteforceStarted = true;
  }
  
  if (currentEdge != selectedEdges.end()) {
    BruteForce();
    
    
    //printf("%d %d candidates\n", selectedEdges.begin() - currentEdge, selectedEdges.size());
    
    //if ((timer++ % 10) == 0)
     // currentEdge++;
  } else {
    BruteforceFinished = true;
  }
  
  
  //BruteForce();
}

static void BaseMechanicsStart() {
  pointer<stDynamics> _dynamics = pointer<stDynamics>GPR(3);
  pointer<stSuperObject> object = pointer<stSuperObject>GPR(4);
  pointer<stSuperObject> mainCharacter = g_stEngineStructure->currentMainPlayers[0];
  
  if (object == mainCharacter) {
    dynamics = _dynamics;
    //Process();
    //printf("\n\n");
    
    
    //Saut3Sommet();
    
    //printf("running base mechanics\n");
//
//    dynamics->base.currentTransform.translation() = stVector3D(49.17f, -661.90f, 74.67f) + stVector3D(sin(frame) * 5, cos(frame) * 5, 1.0f);
//
//    frame += 0.05f;
//
    //printf("[%d] dt: %f\n", i++, FPR(2));
    PrintDynamics(*dynamics);
//    float dt = FPR(2);
//    //if (i == 0) {
//      //JumpBehavior(state.jumpState, dt);
//
//
//    if ((i++ % 200) == 0) {
//      state.jumpState.isJumping = false;
//      state.jumpState.jumpTimer = 0;
//    }
//    }
  }
}

static int currentLoop = 0;
static int maxLoopCount = 200;

static float bestMagnitude = 0.0f;

template <HookLocation at>
static void fn_vMakeCharacterMechanicallyReact() {
  if (at == Start) {
    // Start of function.
    emulator::pushProcessorState();
    
    if (GPR(3) == 0x80BF0C0C) {
      //updateDynamics(pointer<stSuperObject, true>(GPR(3)));
    }
    
//    uint32_t dt = 1000.0f / (60.0f / float(engineLoopCount));
//    // To account for fluctuations of the framerate
//    if (engineLoopCurrent % engineLoopCount == 0) dt -= engineLoopCount;
//    // This works well for the normal walk cycle, but there are still
//    // problems while strafing (much faster) and pulling up from a
//    // ledge (rayman is launched into the air). Good enough though.
//    game::g_stEngineStructure->timer.deltaCount = game_byteorder_32(dt);
//    game::g_stEngineStructure->timer.usefulDeltaTime = game_byteorder_32(dt);
    
  } else if (at == End) {
    // Normal return behavior
    uint32_t sp = GPR(1) & 0x7FFFFFFF; // sp = r1
    GPR(29) = *(uint32*)(intptr_t(memory::baseAddress) + sp + 0x14);
    GPR(30) = *(uint32*)(intptr_t(memory::baseAddress) + sp + 0x18);
    GPR(31) = *(uint32*)(intptr_t(memory::baseAddress) + sp + 0x1C);
    GPR(0)  = *(uint32*)(intptr_t(memory::baseAddress) + sp + 0x24); // lr = r0
    GPR(1) += 32; // align sp
    NPC = LR = GPR(0); // jump
    
    if (GPR(3) - 0x80 == 0x80BF0C0C) {
      stVector3D speed = dynamics->base.report->currentAbsoluteSpeed.linear;
      if (speed.length() > bestMagnitude) {
        printf("speed best: (%f %f %f)\n", float(speed.x()), float(speed.y()), float(speed.z()));
        bestMagnitude = speed.length();
      }
      //saveCalculatedState(pointer<stSuperObject, true>(GPR(3) - 0x80));
      if (++currentLoop < maxLoopCount) {
        // Jump to start
        emulator::popProcessorState();
        NPC = 0x8003b248;
      } else {
        currentLoop = 0;
      }
    }
  }
}

static void BaseMechanicsEnd() {
//  if (MainActorMechanics) {
//    printf("returning");
//    memcpy(&GPR(0), registerState, sizeof(uint32_t) * 4);
//    //NPC = LR;
//    PC = 0x8007e9d8;
//    NPC = 0x8007e9d8;
//    LR = slr;
//  }
}

static void fn_vMakeCharacterThink() {
  NPC = LR;
}

static void HIE_fn_vSendSPOToFixViewport() {
  NPC = LR;
}



void Interface::r3solveInit() {
  //hook<0x8007e9d8>(BaseMechanicsStart);
//  hook<0x8003b054>(fn_vMakeCharacterThink, hookType::replace);
//  hook<0x80094ab0>(HIE_fn_vSendSPOToFixViewport, hookType::replace);
//  hook<0x8003b248>(fn_vMakeCharacterMechanicallyReact<Start>, hookType::start);
//  hook<0x8003b350>(fn_vMakeCharacterMechanicallyReact<End>, hookType::replace);
  
  log::info(log::bold, log::red, "r3solve initialized\n");
}

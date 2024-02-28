#ifndef structure_hh
#define structure_hh

#include "types.hh"
#include "serialize.hh"

namespace CPA::Structure {
  
#pragma mark - Predeclarations
  
  struct stEngineStructure;
  struct stEngineTimer;
  struct stCineManager;
  struct stLanguageStructure;
  struct stRandom;
  
  // IPT
  struct stInputStructure;
  struct stInputEntryElement;
  struct stPadReadingOutput;
  
  // Object
  struct stSuperObject;
  struct stSector;
  struct stEngineObject;
  struct st3DData;
  struct stStandardGameInfo;
  struct stCollideSet;
  struct stCineInfo;
  struct stMSWay;
  struct stMSLight;
  struct stSectorInfo;
  struct stMicro;
  struct stMSSound;
  struct stInstantiatedPhysicalObject;
  struct stPhysicalObject;
  
  // AI
  struct stBrain;
  struct stMind;
  struct stAIModel;
  struct stIntelligence;
  struct stDsgMem;
  struct stDsgVar;
  struct stScriptAI;
  struct stMacro;
  struct stMacroList;
  struct stBehavior;
  
  // DNM
  struct stDynam;
  struct stDynamics;
  struct stDynamicsBaseBlock;
  struct stDynamicsAdvancedBlock;
  struct stDynamicsComplexBlock;
  struct stDynamicsReport;
  struct stDynamicsRotation;
  struct stDynamicsMovevement;
  struct stDynamicsParsingData;
  
  // COL
  struct stPhysicalCollideSet;
  struct stCollideElementIndexedTriangles;
  struct stCollideElementIndexedTrianglesVisual;
  struct stCollideMaterial; // defined in GMT under previous versions
  
  // GMT, GLI, GLD
  struct stGameMaterial;
  struct stVertex2D;
  struct stCamera;
  struct stTexture;
  struct stAnimatedTextureNode;
  
  // WP
  struct stWayPoint;
  struct stGraph;
  struct stGraphNode;
  struct stGraphChainList;
  
#pragma mark - Structure -
  
#define CONCAT(a, b) CONCAT_INNER(a, b)
#define CONCAT_INNER(a, b) a ## b
#define UNIQUE_NAME(base) CONCAT(base, __LINE__)
  /// Create struct padding
#define padding(S) private: uint8_t UNIQUE_NAME(padding) [S]; public:
  
  struct structure : serializable {
    /* ... */
  };
  
  /** ``STRUCTURE BEGIN`` **/
  
#pragma mark - Common types
  
  struct stVector3D {
    float32 x = 0.0f;
    float32 y = 0.0f;
    float32 z = 0.0f;
    
    stVector3D() { /* ... */ }
    stVector3D(float x, float y, float z) : x(x), y(y), z(z) { /* ... */ }
    
    const float32 dot(stVector3D v) { return x * v.x + y * v.y + z * v.z; }
    const float32 square() { return dot(*this); }
    const float32 length() { return std::sqrt(square()); }
    
    const stVector3D operator +(stVector3D v) { x += v.x; y += v.y; z += v.z; return *this; }
    const stVector3D operator -(stVector3D v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
    const stVector3D operator *(stVector3D v) { x *= v.x; y *= v.y; z *= v.z; return *this; }
    const stVector3D operator /(stVector3D v) { x /= v.x; y /= v.y; z /= v.z; return *this; }
    const stVector3D operator *(float s) { x *= s; y *= s; z *= s; return *this; }
    const stVector3D operator /(float s) { x /= s; y /= s; z /= s; return *this; }
    
    void serialize(serializer& s) {
      s.add(x);
      s.add(y);
      s.add(z);
    }
  };
  
  struct stVector4D {
    float32 x = 0.0f;
    float32 y = 0.0f;
    float32 z = 0.0f;
    float32 w = 0.0f;
    
    stVector4D() { /* ... */ }
    stVector4D(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) { /* ... */ }
    
    const stVector3D xyz() { return stVector3D(x, y, z); }
    
    const stVector4D operator +(stVector4D v) { x += v.x; y += v.y; z += v.z; w += v.w; return *this; }
    const stVector4D operator -(stVector4D v) { x -= v.x; y -= v.y; z -= v.z; w -= v.w; return *this; }
    const stVector4D operator *(stVector4D v) { x *= v.x; y *= v.y; z *= v.z; w *= v.w; return *this; }
    const stVector4D operator /(stVector4D v) { x /= v.x; y /= v.y; z /= v.z; w /= v.w; return *this; }
    const stVector4D operator *(float s) { x *= s; y *= s; z *= s; w *= s; return *this; }
    const stVector4D operator /(float s) { x /= s; y /= s; z /= s; w /= s; return *this; }
  };
  
  union stMatrix3D {
    struct {
      float32 m00 = 1.0f, m01 = 0.0f, m02 = 0.0f;
      float32 m10 = 0.0f, m11 = 1.0f, m12 = 0.0f;
      float32 m20 = 0.0f, m21 = 0.0f, m22 = 1.0f;
    };
  };
  
  union stMatrix4D {
    struct {
      float32 m00 = 1.0f, m01 = 0.0f, m02 = 0.0f, m03 = 0.0f;
      float32 m10 = 0.0f, m11 = 1.0f, m12 = 0.0f, m13 = 0.0f;
      float32 m20 = 0.0f, m21 = 0.0f, m22 = 1.0f, m23 = 0.0f;
      float32 m30 = 0.0f, m31 = 0.0f, m32 = 0.0f, m33 = 1.0f;
    };
    struct {
      float32 m[16];
    };
    
    stMatrix4D() { /* ... */ }
      
    stMatrix4D operator * (stMatrix4D mm) {
      stMatrix4D r;
      r.m[0]  = m[0] * mm.m[0]  + m[4] * mm.m[1]  + m[8] * mm.m[2]   + m[12] * mm.m[3];
      r.m[4]  = m[0] * mm.m[4]  + m[4] * mm.m[5]  + m[8] * mm.m[6]   + m[12] * mm.m[7];
      r.m[8]  = m[0] * mm.m[8]  + m[4] * mm.m[9]  + m[8] * mm.m[10]  + m[12] * mm.m[11];
      r.m[12] = m[0] * mm.m[12] + m[4] * mm.m[13] + m[8] * mm.m[14]  + m[12] * mm.m[15];
      r.m[1]  = m[1] * mm.m[0]  + m[5] * mm.m[1]  + m[9] * mm.m[2]   + m[13] * mm.m[3];
      r.m[5]  = m[1] * mm.m[4]  + m[5] * mm.m[5]  + m[9] * mm.m[6]   + m[13] * mm.m[7];
      r.m[9]  = m[1] * mm.m[8]  + m[5] * mm.m[9]  + m[9] * mm.m[10]  + m[13] * mm.m[11];
      r.m[13] = m[1] * mm.m[12] + m[5] * mm.m[13] + m[9] * mm.m[14]  + m[13] * mm.m[15];
      r.m[2]  = m[2] * mm.m[0]  + m[6] * mm.m[1]  + m[10] * mm.m[2]  + m[14] * mm.m[3];
      r.m[6]  = m[2] * mm.m[4]  + m[6] * mm.m[5]  + m[10] * mm.m[6]  + m[14] * mm.m[7];
      r.m[10] = m[2] * mm.m[8]  + m[6] * mm.m[9]  + m[10] * mm.m[10] + m[14] * mm.m[11];
      r.m[14] = m[2] * mm.m[12] + m[6] * mm.m[13] + m[10] * mm.m[14] + m[14] * mm.m[15];
      r.m[3]  = m[3] * mm.m[0]  + m[7] * mm.m[1]  + m[11] * mm.m[2]  + m[15] * mm.m[3];
      r.m[7]  = m[3] * mm.m[4]  + m[7] * mm.m[5]  + m[11] * mm.m[6]  + m[15] * mm.m[7];
      r.m[11] = m[3] * mm.m[8]  + m[7] * mm.m[9]  + m[11] * mm.m[10] + m[15] * mm.m[11];
      r.m[15] = m[3] * mm.m[12] + m[7] * mm.m[13] + m[11] * mm.m[14] + m[15] * mm.m[15];
      return r;
    }
    
    /// Create a new translation matrix
    static stMatrix4D translation(stVector3D v) {
      stMatrix4D result;
      result.m30 = v.x;
      result.m31 = v.y;
      result.m32 = v.z;
      return result;
    }
    
    /// Create a new scale matrix
    static stMatrix4D scale(stVector3D v) {
      stMatrix4D result;
      result.m00 = v.x;
      result.m11 = v.y;
      result.m22 = v.z;
      return result;
    }
    
    /// Create a new perspective matrix
    static stMatrix4D perspective(float fovY, float aspect, float near, float far) {
      float const ct = 1.0f / std::tanf(fovY / 2.0f);
      
      stMatrix4D result;
      result.m00 = ct / aspect;
      result.m11 = ct;
      result.m22 = (far + near) / (near - far);
      result.m23 = -1.0f;
      result.m32 = (2.0f * far * near) / (near - far);
      result.m33 = 0.0f;
      return result;
    }
    
    /// stMatrix4D * stVector4D -> stVector4D
    stVector4D operator * (stVector4D v) {
      stVector4D result;
      result.x = m00 * v.x + m10 * v.y + m20 * v.z + m30 * v.w;
      result.y = m01 * v.x + m11 * v.y + m21 * v.z + m31 * v.w;
      result.z = m02 * v.x + m12 * v.y + m22 * v.z + m32 * v.w;
      result.w = m03 * v.x + m13 * v.y + m23 * v.z + m33 * v.w;
      return result;
    }
    
    stVector3D operator *(stVector3D v) {
      stVector4D V = (*this) * stVector4D(v.x, v.y, v.z, 1.0f);
      return V.xyz();
    }
    
  };
  
#pragma mark - Containers
  
  /// A linked list
  template <typename T = uint32>
  struct stLinkedList {
    pointer<T> first;
    int32 numEntries;
    
    template <typename F>
    void forEach(const F& f, void *userdata = nullptr) {
      for (T *c = first; c; c = c->next) {
        f(c, userdata);
      }
    }
  };
  
  /// A doubly linked list
  template <typename T = uint32>
  struct stDoublyLinkedList {
    pointer<T> first;
    pointer<T> last;
    int32 numEntries;
    
    template <typename F>
    void forEach(const F& f, void *userdata = nullptr) {
      for (T *c = first; c; c = c->next) {
        f(c, userdata);
      }
    }
  };
  
#pragma mark - stTransform
  
  /// World transform
  struct stTransform {
    
    enum Type : uint32_t {
      /* gcn */
      Uninitialized = 0,
      Identity = 1,
      Translate = 2,
      Zoom = 3,
      Scale = 4,
      Rotation = 5,
      RotationZoom = 6,
      RotationScale = 7,
      RotationScaleComplex = 8,
      Undefined = 9,
    };
    
    stTransform();
    
    /** transform type */
    Type type = Uninitialized;
    /** matrix */
    stMatrix4D matrix;
    /** external scale parameter */
    stVector4D scale;
    
    /** get translation vector */
    stVector3D& translation();
    /** transform x stVector3D -> stVector3D */
    stVector3D operator * (stVector3D v);
    /** transform x stVector4D -> stVector4D */
    stVector4D operator * (stVector4D v);
  };
  
#pragma mark - stAlways
  
  struct stAlways {
    uint32 numAlways;
    stDoublyLinkedList<> alwaysModels;
    pointer<stSuperObject> alwaysSuperobject;
    pointer<stEngineObject> alwaysActors;
    pointer<stSuperObject> alwaysGeneratorSuperobjects;
  };
  
#pragma mark - stObjectType
  
  enum eObjectType {
    objectFamilyName = 0,
    objectModelName = 1,
    objectInstanceName = 2,
  };
  
  /// Object identifier
  struct stObjectTypeElement {
    pointer<stObjectTypeElement> next;
    pointer<stObjectTypeElement> prev;
    pointer<stDoublyLinkedList<stObjectTypeElement>> father;
    pointer<string<>> name;
    uint8 priority;
    uint8 identifier;
    padding(2)
    void serialize(serializer& s);
  };
  
  /// Global object type table
  struct stObjectType {
    stDoublyLinkedList<stObjectTypeElement> family;
    stDoublyLinkedList<stObjectTypeElement> model;
    stDoublyLinkedList<stObjectTypeElement> instance;
    void serialize(serializer& s);
  };
  
#pragma mark - Engine
  
  /// Global engine timer
  struct stEngineTimer {
    uint32 currentFrame;
    int16 timerHandle;
    padding(2)
    uint32 currentCount;
    uint32 deltaCount;
    uint32 counter[16];
    uint32 usefulDeltaTime;
    uint32 pauseTime;
    float32 frameLength;
    uint32 totalRealTimeLow;
    uint32 totalRealTimeHigh;
    uint32 totalPauseTimeLow;
    uint32 totalPauseTimeHigh;
    uint32 ticksPerMs;
    void serialize(serializer& s);
  };
  
  struct stEngineStructure {
    
    enum eEngineMode : uint8_t {
      Invalid = 0,
      Initialize = 1,
      Deinitialize = 2,
      InitializeGameplay = 3,
      DeinitializeGameplay = 4,
      EnterLevel = 5,
      ChangeLevel = 6,
      Gameplay = 9,
    };
    
    enum eInputMode : typename uint8::U {
      Normal = 0,
      Commands = 1,
    };
    
    eEngineMode mode = Invalid;
    string<30> currentLevelName;
    string<30> nextLevelName;
    string<30> firstLevelName;
    eInputMode inputMode;
    uint8 displayFixMode;
    padding(3)
    uint32 displayMode;
    stEngineTimer timer;
    uint8 multimodePlayerCount;
    uint8 multimodeColumnCount;
    uint8 multimodeMiniScreenRatio;
    padding(1)
    pointer<stSuperObject> currentMainPlayers[4];
    int16 gldDevice;
    int16 gldViewport[5];
    padding(5 * 28 * 4) /* viewport attributes */
    pointer<stCamera> viewportCamera[5];
    int16 gldFixViewport[5];
    padding(2)
    padding(5 * 28 * 4) /* fix viewport attributes */
    padding(5 * 2 * 4) /* fix 3d attributes */
    pointer<stCamera> fixCamera[5];
    padding(5 * 2 * 4) /* game 3d attributes */
    pointer<> viewportArray;
    stDoublyLinkedList<> cameraList;
    pointer<> drawSem;
    stDoublyLinkedList<> familyList;
    stDoublyLinkedList<> alwaysList;
    stDoublyLinkedList<stSuperObject> mainCharacterList;
    pointer<stSuperObject> standardCamera;
    pointer<stSuperObject> debugCamera;
    pointer<> languageStructure;
    pointer<> levelFilenameList;
    stTransform mainActorTransform;
    stTransform mainCameraTransform;
    int32 submapNumber;
    
    uint8 paused;
    uint8 paused2;
    uint8 doGameSave;
    
    string<30> levelNames[150];
    string<12> demoNames[30];
    string<12> demoLevelNames[30];
    
    uint8 demoCount;
    uint8 levelCount;
    uint8 currentLevel;
    uint8 previousLevel;
    uint8 previousLevelExitID;
    uint8 globalLevelCounter;
    uint8 demoMode;
    uint8 currentLanguage;
    uint8 languageCount;
    uint8 engineFrozen;
    uint8 resurrection;
    uint8 cameraMode;
    uint8 currentImportance;
    uint8 numSuperObjectsAllocated;
    uint8 numSuperObjectsLoaded;
    uint8 numNonPersistentSOLinks;
    padding(9);
    
    doublepointer<> superObjectLinks;
    pointer<stGraphChainList> graphList;
    pointer<stCineManager> cineManager;
    
    /// Load level by name
    void loadLevel(std::string levelName);
    
    void serialize(serializer& s);
  };
  
#pragma mark - IPT
  
  struct stPadReadingOutput {
    stVector3D globalVector;
    int16 horizontalAxis;
    int16 verticalAxis;
    float32 analogForce;
    float32 trueAnalogForce;
    float32 angle;
    int32 sector;
    
    void serialize(serializer& s);
  };
  
  struct stInputDevice {
    uint8 valid;
    padding(3)
    pointer<> handle;
    uint8 joypadCounter[44];
    int8 joyMinX;
    int8 joyMaxX;
    int8 joyMinY;
    int8 joyMaxY;
    int8 joyCenterX;
    int8 joyCenterY;
    padding(2)
    pointer<stInputEntryElement> joyAxisX;
    pointer<stInputEntryElement> joyAxisY;
    pointer<stInputEntryElement> joyAxisZ;
    pointer<stInputEntryElement> joyAxisR;
    pointer<stInputEntryElement> keyUp;
    pointer<stInputEntryElement> keyDown;
    pointer<stInputEntryElement> keyLeft;
    pointer<stInputEntryElement> keyRight;
    pointer<stInputEntryElement> keySpeedUp;
    uint8 noKeyboardInertia;
    padding(3)
    pointer<stInputEntryElement> joyButton[16];
    pointer<stInputEntryElement> keyButton[16];
    stPadReadingOutput padReadOutput;
    
    void serialize(serializer& s);
  };
  
  struct stInputEntryElement {
    padding(6 * 4) /* ? */
    uint32 numKeywords;
    pointer<> keywordArray;
    pointer<> actionName;
    pointer<> entryName;
    int32 state;
    float32 analogValue;
    int8 active;
    padding(3)
    
    void serialize(serializer& s);
  };
  
  struct stInputStructure {
    uint8 onePadActivate;
    padding(3)
    stInputDevice device[18];
    uint8 keyboardCounter[256];
    uint8 keyboardType;
    uint8 mouseButtonCounter[9];
    padding(2)
    uint32 numEntries;
    pointer<stInputEntryElement> entries;
    uint32 numCommands;
    pointer<> commands;
    int16 eventSize;
    padding(2)
    pointer<> historicEvent;
    uint8 oneActionValidated;
    string<78> line;
    string<78> lineInternal;
    string<78> lineSearch;
    string<78> lineLast[10];
    padding(1)
    int32 historicIndex;
    pointer<> searchedCommand;
    pointer<stInputEntryElement> commandModeEntrySwap;
    pointer<stInputEntryElement> commandModeEntryClear;
  };
  
#pragma mark - RND
  
#if CPA_ENGINE_VERSION == CPA_ENGINE_VERSION_R3
# define RND_DEFAULT_INDEX        0x0000 // Default index into tableIndices
# define RND_TABLEINDICES_COUNT   0x0032 // Size of tableIndices
# define RND_TABLE_COUNT          0x2710 // Size of table
#else
# define RND_DEFAULT_INDEX        0x0000
# define RND_TABLEINDICES_COUNT   0x0032
# define RND_TABLE_COUNT          0x2710
#endif
  
  /// Random number table structure
  struct stRandom {
    /// Index the random number table by absolute offset
    int32_t index(unsigned i) {
      return table ? ((table[std::max(0u, i)] >> 16) & 0x7FFF) : 0;
    }
    /// Index the random number table using an index from tableIndices, optionally offset
    int32_t indexRelative(unsigned TableIndicesIdx, unsigned Offset) {
      return index(tableIndices[TableIndicesIdx] + Offset);
    }
    /// Simulate `Count` calls into the RND table, bounding the value by `Min` and `Max`
    int32_t call(unsigned const Count, unsigned const Min, unsigned const Max, unsigned const Index = RND_DEFAULT_INDEX) {
      int32_t n, v = 0;
      for (n = 0; n < Count; n++)
        v = (Min + ((Max + 1 - Min) * indexRelative(Index, n)) / (tableMax + 1));
      return v;
    }
    /// Size of the table
    uint32 tableSize;
    /// Indices into the table
    uint32 tableIndices[RND_TABLEINDICES_COUNT];
    /// Last index from tableIndices
    uint32 lastIndex;
    /// Largest number present in table
    uint32 tableMax;
    /// 1.0 / tableMax
    float32 tableMaxInverse;
    /// Random number table
    pointer<uint32> table;
  };
  
#pragma mark - CINE
  
  struct stCine {
    stDoublyLinkedList<stEngineObject> actors;
    pointer<stCine> next;
    pointer<stCine> prev;
    pointer<stDoublyLinkedList<stCine>> parent;
    uint8 playing;
    padding(3)
    uint32 event;
    string<255> name;
  };
  
  struct stCineManager {
#if CPA_ENGINE_VERSION == CPA_ENGINE_VERSION_R3  // spec r3
#if CPA_PLATFORM == CPA_PLATFORM_GCN
    stDoublyLinkedList<stCine> cineList;
    stTransform fixedCameraTransform;
    pointer<stSuperObject> activeCamera;
#elif CPA_PLATFORM == CPA_PLATFORM_PS2
    stDoublyLinkedList<stCine> cineList;
    padding(4)
    stTransform fixedCameraTransform;
    padding(2)
    pointer<stSuperObject> activeCamera;
#endif
#endif
  };
  
#pragma mark - DNM
  
  struct stDynamicsRotation {
    float32 angle;
    stVector3D axis;
  };
  
  struct stDynamicsBaseBlock {
    int32 objectType;
    pointer<> idcard;
    uint32 flags;
    uint32 endFlags;
    float32 gravity;
    float32 slopeLimit;
    float32 slopeCosine;
    float32 slide;
    float32 rebound;
    stVector3D imposeSpeed;
    stVector3D proposeSpeed;
    stVector3D previousSpeed;
    stVector3D scale;
    stVector3D animationProposeSpeed;
    stVector3D safeTranslation;
    stVector3D addTranslation;
#if CPA_ENGINE_VERSION == CPA_ENGINE_VERSION_R3 && CPA_PLATFORM == CPA_PLATFORM_PS2
    padding(8)
#endif
    stTransform previousTransform;
    stTransform currentTransform;
    stMatrix3D imposedRotation;
    uint8 nFrame;
    padding(3)
    pointer<stDynamicsReport> report;
#if CPA_ENGINE_VERSION == CPA_ENGINE_VERSION_R3 && CPA_PLATFORM == CPA_PLATFORM_PS2
    padding(8)
#endif
  };
  
  struct stDynamicsAdvancedBlock {
    float32 xInertia;
    float32 yInertia;
    float32 zInertia;
    float32 streamPriority;
    float32 streamFactor;
    float32 xSlideFactor;
    float32 ySlideFactor;
    float32 zSlideFactor;
    float32 previousSlide;
    stVector3D maxSpeed;
    stVector3D streamSpeed;
    stVector3D addedSpeed;
    stVector3D limit;
    stVector3D collisionTranslation;
    stVector3D inertiaTranslation;
    stVector3D groundNormal;
    stVector3D wallNormal;
    int8 collideCount;
    padding(3)
  };
  
  /// AI and DNM message-interchange
  struct stMACDPID {
    float32 data0;
    stVector3D data1;
    stVector3D data2;
    stVector3D data3;
    float32 data4;
    float32 data5;
    float32 data6;
    stDynamicsRotation data7;
    stDynamicsRotation data8;
    int8 data9;
    uint16 data10;
    stVector3D data11;
    float32 data12;
    stVector3D data13;
    float32 data14;
    uint8 data15;
  };
  
  struct stDynamicsComplexBlock {
    float32 tiltStrength;
    float32 tiltInertia;
    float32 tiltOrigin;
    float32 tiltAngle;
    float32 hangingLimit;
    stVector3D contact;
    stVector3D fallTranslation;
    stMACDPID macdpid;
    pointer<stSuperObject> platformSuperObject;
    stTransform previousMatrixAbsolute;
    stTransform previousMatrixPrevious;
  };
  
  
  struct stDynamicsObstacle {
    float32 rate;
    stVector3D normal;
    stVector3D contact;
    pointer<> material;
    pointer<> collideMaterial;
    pointer<stSuperObject> superObject;
  };
  
  struct stDynamicsMovevement {
    stVector3D linear;
    stDynamicsRotation angular;
  };
  
  struct stDynamicsReport {
    uint32 previousSurfaceState = 0;
    uint32 currentSurfaceState = 0;
    stDynamicsObstacle obstacle;
    stDynamicsObstacle ground;
    stDynamicsObstacle wall;
    stDynamicsObstacle character;
    stDynamicsObstacle water;
    stDynamicsObstacle ceiling;
    stDynamicsMovevement previousAbsoluteSpeed;
    stDynamicsMovevement currentAbsoluteSpeed;
    stDynamicsMovevement previousAbsolutePosition;
    stDynamicsMovevement currentAbsolutePosition;
    char8 bitField;
    padding(3)
  };
  
  struct stDynamics {
    stDynamicsBaseBlock base;
    stDynamicsAdvancedBlock advanced;
    stDynamicsComplexBlock complex;
    
    auto flag(int flag) -> bool {
      return base.flags & flag;
    }
    
    auto endFlag(int flag) -> bool {
      return base.endFlags & flag;
    }
  };
  
  struct stDynamicsParsingData {
    stVector3D position;
    float32 outAlpha;
    stVector3D vector;
  };
  
  struct stDynam {
    pointer<stDynamics> dynamics;
    pointer<stDynamicsParsingData> parsingData;
    uint32 usedMechanics;
  };
  
#pragma mark - Engine object
  
  struct stStandardGameInfo {
    int32 familyType;
    int32 modelType;
    int32 instanceType;
    pointer<stSuperObject> superObject;
    uint8 initialFlag;
    uint8 flag1;
    uint8 flag2;
    padding(2)
    uint32 lastFrame;
    uint32 capabilities;
    uint8 tractionFactor;
    uint8 hitPoints;
    uint8 maxHitPoints;
    uint8 maxHitPointsMax;
    uint32 customBits;
    uint32 aiCustomBits;
    uint8 platformType;
    uint8 miscFlags;
    uint8 transparencyZoneMin;
    uint8 transparencyZoneMax;
    uint32 initialCustomBits;
    uint32 aiInitialCustomBits;
    uint8 initialHitPoints;
    uint8 maxInitialHitPoints;
    uint8 initialMiscFlags;
    uint8 tooFarLimit;
    uint8 importance;
    uint8 optional;
    padding(2)
    /* :: custom values :: */
  };
  
  using ObjectNameResolver = std::function<std::string(eObjectType, int*)>;
  
  struct stEngineObject {
    pointer<st3DData> p3DData;
    pointer<stStandardGameInfo> stdGame;
    pointer<stDynam> dynam;
    pointer<stBrain> brain;
    pointer<stCineInfo> cineInfo;
    pointer<stCollideSet> collSet;
    pointer<stMSWay> msWay;
    pointer<stMSLight> msLight;
    pointer<stSectorInfo> sectorInfo;
    pointer<stMicro> micro;
    pointer<stMSSound> msSound;
    
    std::string name(ObjectNameResolver resolve, eObjectType type);
    std::string familyName(ObjectNameResolver resolve);
    std::string modelName(ObjectNameResolver resolve);
    std::string instanceName(ObjectNameResolver resolve);
    
    /// Return the AI model of this actor
    pointer<stAIModel> aiModel();
  };
  
  using stActor = stEngineObject;
  
#pragma mark - SECT
  
  struct stSector {
    stDoublyLinkedList<> characterList;
    stDoublyLinkedList<> staticLightList;
    stDoublyLinkedList<> dynamicLightList;
    stDoublyLinkedList<> graphicSectorList;
    stDoublyLinkedList<> collisionSectorList;
    stDoublyLinkedList<> activitySectorList;
    stDoublyLinkedList<> soundSectorList;
    stDoublyLinkedList<> soundEventList;
    stVector3D min;
    stVector3D max;
    float32 farPlane;
    uint8 isVirtual;
    int8 cameraType;
    int8 counter;
    int8 priority;
    pointer<> skyMaterial;
    uint8 fog;
#if CPA_PLATFORM == CPA_PLATFORM_GCN
    string<0x104> name;
#endif
  };
  
#pragma mark - COL
  
  struct stOctreeNode {
    stVector3D min;
    stVector3D max;
    pointer<stOctreeNode> children;
    pointer<uint8> faceIndices;
  };
  
  struct stOctree {
    pointer<stOctree> rootNode;
    int16 numFaces;
    padding(2)
    pointer<uint16> elementBases;
    stVector3D min;
    stVector3D max;
  };
  
  struct stCollideObject {
    
    enum Type {
      IndexedTriangles   = 1,
      Facemap            = 2,
      Sprite             = 3,
      TMesh              = 4,
      Points             = 5,
      Lines              = 6,
      IndexedSpheres     = 7,
      AABB               = 8,
      Cones              = 9,
      DeformationSetInfo = 13,
    };
    
    int16 numVertices;
    int16 numElements;
    int16 numBoundingBoxes;
    padding(2)
    pointer<stVector3D> vertices;
    pointer<int16> elementTypes;
    doublepointer<> elements;
    pointer<stOctree> octree;
    pointer<> boundingBoxes;
    float32 boundingSphereRadius;
    stVector4D boundingSpherePosition;
  };
  
  struct stPhysicalCollideSet {
    pointer<stCollideObject> zdm;
    pointer<stCollideObject> zdd;
    pointer<stCollideObject> zde;
    pointer<stCollideObject> zdr;
  };
  
  struct stColliderInfo {
    pointer<stSuperObject> colliderActors[2];
    stVector3D colliderVectors[2];
    float32 colliderReal[2];
    uint8 colliderType;
    uint8 colliderPriority;
    uint8 unused[2];
  };
  
  struct stZdxListEntry {
#if CPA_PLATFORM == CPA_PLATFORM_GCN
    pointer<> next;
    pointer<> prev;
    pointer<> parent;
    pointer<> data;
#else
    pointer<stCollideObject> data;
#endif
  };
  
  struct stZdxList {
#if CPA_PLATFORM == CPA_PLATFORM_GCN
    stDoublyLinkedList<stZdxListEntry> list;
#else
    stLinkedList<stZdxListEntry> list;
#endif
    uint16 numZdx;
    padding(2)
  };
  
  struct stCsaList {
    stDoublyLinkedList<> list;
  };
  
  struct stCollideSet {
    pointer<stZdxList> zddList;
    pointer<stZdxList> zdeList;
    pointer<stZdxList> zdmList;
    pointer<stZdxList> zdrList;
    pointer<> zddActivationList;
    pointer<> zdeActivationList;
    pointer<> zdmActivationList;
    pointer<> zdrActivationList;
    pointer<> zddCurrentActivation;
    pointer<> zdeCurrentActivation;
    pointer<> zdrCurrentActivation;
    pointer<> zdmCurrentActivation;
    uint32 zddPrivilegedZone;
    uint32 zdePrivilegedZone;
    uint32 zdmPrivilegedZone;
    uint32 zdrPrivilegedZone;
    uint8 computeFrequency;
    uint8 characterPriority;
    uint8 collisionFlag;
    padding(1)
    stColliderInfo colliderInfo;
  };
  
  struct stElementIndexedTrianglesVisual {
    pointer<> visualMaterial;
    int16 numFaces;
    int16 numUVs;
    int16 numUVStages;
    padding(2)
    pointer<uint16> faceIndices;
    pointer<> faceUVIndices;
    pointer<stVector3D> faceNormals;
    pointer<> UVElements;
    pointer<> edges;
    pointer<> adjacentFaces;
    pointer<> thisIndexList;
    int16 numUsedIndices;
    int16 boundingBoxIndex;
    uint32 displayList;
    pointer<> unknown;
    uint8 portalVisibility;
    padding(3)
    uint32 vao[4];
  };
  
  struct stCollideElementIndexedTriangles {
    pointer<stGameMaterial> material;
    pointer<uint16> faceIndices;
    pointer<stVector3D> normals;
    int16 numFaces;
    int16 aabbIndex;
    pointer<stElementIndexedTrianglesVisual> visual;
    pointer<> faceEdges;
    pointer<> edgeNormals;
    pointer<> edgeCoefficients;
    int16 numEdges;
    padding(2)
  };
  
  struct stCollideElementIndexedSphere {
    float32 radius;
    pointer<stGameMaterial> material;
    int16 indexOfCenterPoint;
    padding(2)
  };
  
  struct stCollideElementSpheres {
    pointer<stCollideElementIndexedSphere> spheres;
    int16 numSpheres;
    int16 aabbIndex;
  };
  
  struct stCollideMaterial {
    int16 zoneType;
    uint16 identifier;
    float32 xDirection;
    float32 yDirection;
    float32 zDirection;
    float32 coefficient;
    uint16 aiType;
    padding(2)
  };
  
  struct stCollisionCase {
    float32 collisionTime;
    stVector3D collisionNormal;
    stVector3D collisionPoint;
    pointer<stGameMaterial> dynamicMaterial;
    pointer<stGameMaterial> staticMaterial;
    pointer<> param1;
    int32 param2;
    int16 dynamicEntity;
    int16 staticEntity;
    stVector3D translation;
    stVector3D movement;
    stVector3D endPosition;
    float32 sphereRadius;
    float32 slide1;
    float32 rebound1;
    float32 slide2;
    float32 rebound2;
  };
  
  struct stIndexedAlignedBox {
    int16 min;
    int16 max;
    pointer<stGameMaterial> material;
  };
  
  struct stCollideElementAlignedBoxes {
    pointer<stIndexedAlignedBox> boxes;
    int16 numBoxes;
    int16 parallelBoxIndex;
  };
  
#if CPA_ENGINE_VERSION == CPA_ENGINE_VERSION_R3
# define COL_MAX_SELECTED_OCTREE_NODES 100
#endif
  
  struct stGVForCollision {
    pointer<stVector3D> vertex1;
    stVector3D edgeVector;
    pointer<stVector3D> vertex2;
    stVector3D dinST0Point;
    float32 dynamicRadius;
    pointer<stTransform> staticGeomObjMatrix;
    stVector3D dinST1Point;
    stVector3D dinST01Vector;
    pointer<stGameMaterial> dynamicMaterial;
    pointer<stGameMaterial> staticMaterial;
    pointer<> vParameter1;
    int16 sParameter2;
    pointer<stCollideObject> staticCollideObject;
    pointer<stCollideElementIndexedTriangles> staticElementIndexedTriangles;
    uint32 selectedCollisionCases;
    int16 staticElementIndex;
    int16 staticIndexedTriangleIndex;
    pointer<stCollideElementIndexedSphere> dynamicIndexedSphere;
    pointer<stOctree> octree;
    uint8 staticGeomObjHasNoTransformationMatrix;
    uint8 dynamicGeomObjHasZoomInsteadOfScale;
    pointer<stCollideObject> dynamicCollideObject;
    pointer<stTransform> t0DynamicGeometricObjectMatrix;
    pointer<stTransform> t1DynamicGeometricObjectMatrix;
    stTransform inverseMatrix;
    stTransform d2st0TransformMatrix;
    stTransform d2st1TransformMatrix;
    stTransform s2dt0TransformMatrix;
    stTransform s2dt1TransformMatrix;
    float32 staticScale;
    pointer<stCollideElementSpheres> dynamicElementSpheres;
    pointer<stCollideElementSpheres> staticElementSpheres;
    int32 bitFieldOfIndexedSpheresInCollision;
    pointer<stCollideElementIndexedSphere> staticIndexedSphere;
    stVector3D swapDinST0Point;
    pointer<stVector3D> dynamicCenter;
    pointer<stVector3D> staticCenter;
    float32 swapRadius;
    uint8 useEnlargedSphere;
    int16 numSelectedNodes;
    pointer<stOctreeNode> selectedOctreeNodes[COL_MAX_SELECTED_OCTREE_NODES];
    float32 selectedOctreeT[COL_MAX_SELECTED_OCTREE_NODES];
    pointer<stCollideElementAlignedBoxes> dynamicElementAlignedBoxes;
    int16 dynamicIndexedAlignedBoxIndex;
    pointer<stIndexedAlignedBox> dynamicIndexedAlignedBox;
    pointer<stVector3D> dynamicMinPoint;
    pointer<stVector3D> dynamicMaxPoint;
    stVector3D dinST0MaxPoint;
    stVector3D dinST0MinPoint;
    stVector3D dinST1MaxPoint;
    stVector3D dinST1MinPoint;
    stVector3D dinST08VBox[8];
    stVector3D dinST18VBox[8];
    stVector3D dinST01Vect[8];
    int16 staticIndexedSphereIndex;
    pointer<stCollideElementAlignedBoxes> staticElementAlignedBoxes;
    pointer<stIndexedAlignedBox> staticIndexedBox;
    pointer<stVector3D> pStaticMinPoint;
    pointer<stVector3D> pStaticMaxPoint;
    stVector3D staticMinPoint;
    stVector3D staticMaxPoint;
    stVector3D static8VBox[8];
  };
  
#pragma mark - PO
  
  struct stPhysicalObject {
    pointer<> visualSet;
    pointer<stPhysicalCollideSet> physicalCollideset;
    pointer<> visualBoundingVolume;
    pointer<> collideBoundingVolume;
  };
  
#pragma mark - IPO
  
  struct stInstantiatedPhysicalObject {
    pointer<stPhysicalObject> physicalObject;
    pointer<> currentRadiosity;
    doublepointer<> radiosity;
    pointer<stSuperObject> portalCamera;
    uint32 lastTransitionID;
    float32 lastRatioUsed;
#if CPA_PLATFORM == CPA_PLATFORM_GCN
    string<0x32> name;
#endif
  };
  
#pragma mark - stSuperObject
  
  enum eSuperObjectType : uint32_t {
    superObjectTypeNone                 = (0 << 0),
    superObjectTypeWorld                = (1 << 0),
    superObjectTypeActor                = (1 << 1),
    superObjectTypeSector               = (1 << 2),
    superObjectTypePhysicalObject       = (1 << 3),
    superObjectTypePhysicalObjectMirror = (1 << 4),
    superObjectTypeIPO                  = (1 << 5),
    superObjectTypeIPOMirror            = (1 << 6),
    superObjectTypeSpecialEffect        = (1 << 7),
    superObjectTypeNoAction             = (1 << 8),
    superObjectTypeMirror               = (1 << 9),
  };
  
  static std::map<eSuperObjectType, std::string> stSuperObjectTypeNames {
    { superObjectTypeNone                , "Dummy SuperObject" },
    { superObjectTypeWorld               , "World" },
    { superObjectTypeActor               , "Actor" },
    { superObjectTypeSector              , "Sector" },
    { superObjectTypePhysicalObject      , "PhysicalObject" },
    { superObjectTypePhysicalObjectMirror, "PhysicalObjectMirror" },
    { superObjectTypeIPO                 , "IPO" },
    { superObjectTypeIPOMirror           , "IPO.Mirror" },
    { superObjectTypeSpecialEffect       , "SpecialEffect" },
    { superObjectTypeNoAction            , "NoAction" },
    { superObjectTypeMirror              , "Mirror" },
  };
    
  struct stSuperObject {
    /// `eSuperObjectType`
    uint32 type;
    
    union {
      pointer<> data;
      pointer<stEngineObject> actor;
      pointer<stSector> sector;
      pointer<stInstantiatedPhysicalObject> ipo;
    };
    
    pointer<stSuperObject> firstChild;
    pointer<stSuperObject> lastChild;
    int32 numChildren;
    pointer<stSuperObject> next;
    pointer<stSuperObject> prev;
    pointer<stSuperObject> parent;
    pointer<stTransform> localTransform;
    pointer<stTransform> globalTransform;
    int32 prevFrameProcessed;
    int32 drawFlags;
    uint32 flags;
    pointer<> visualBBox;
    pointer<> collideBBox;
    stVector3D semiLookat;
    float32 transparency;
    uint32 outlineColor;
    int32 displayPriority;
    int32 ilstatus; /* ? */
    stVector3D ambientColor;
    stVector3D parallelDirection;
    stVector3D parallelColor;
    uint8 superimpose;
    uint8 isSuperObject;
    uint8 transition;
    padding(1)
    
    /// Return the name of this superobject's type
    std::string typeName();
    
    /// Return the name of the superobject
    ///   For actors, return the first name available
    ///   in the order of [Family, Model, Instance]
    std::string name(ObjectNameResolver resolve, bool fullname = false);
    
    /// Recurse the tree below this superobject
    template <typename F, typename UserData>
    void recurse(const F& f, UserData userdata) {
      _recurse(this, userdata, f);
    }
      
    /// Run a custom function for each child object
    template <typename F>
    void forEachChild(const F& f, void *userdata = nullptr) {
      for (stSuperObject *ii = firstChild; ii; ii = ii->next) {
        f(ii, userdata);
      }
    }
    
  private:
    template <typename F, typename UserData>
    void _recurse(stSuperObject *root, UserData userdata, const F& f) {
      for (stSuperObject *ii = root->firstChild; ii; ii = ii->next) {
        f(ii, userdata);
        _recurse(ii, userdata, f);
      }
    }
  };
  
#pragma mark - AI
  
  struct stBrain {
    pointer<stMind> mind;
    pointer<stGameMaterial> lastNoCollideMaterial;
    uint8 warnMechanics;
    uint8 activeDuringTransition;
    padding(2)
  };
  
  struct stMind {
    pointer<stAIModel> aiModel;
    pointer<stIntelligence> intelligence;
    pointer<stIntelligence> reflex;
    pointer<stDsgMem> dsgMem;
    uint8 runIntelligence;
    padding(3)
  };
  
  struct stAIModel {
    pointer<stScriptAI> intelligenceBehaviorList;
    pointer<stScriptAI> reflexBehaviorList;
    pointer<stDsgVar> dsgVar;
    pointer<stMacroList> macroList;
    uint8 secondPassFinished;
    padding(3)
  };
  
  struct stNodeInterpret {
  #if CPA_PLATFORM == CPA_PLATFORM_GCN
    uint32 param;
    padding(3)
    uint8 type;
    padding(2)
    uint8 depth;
    padding(1)
  #endif
    using ParamType = decltype(param);
  };
  
  struct stTreeInterpret {
    pointer<stNodeInterpret> node;
  };
  
  union uGetSetParam {
    int8 s8Value;
    int16 s16Value;
    int32 s32Value;
    float32 floatValue;
    pointer<> pointerValue;
  };
  
  struct stActionParam {
    union uGetSetParam param[8];
  };
  
  struct stActionTableEntry {
#if CPA_PLATFORM == CPA_PLATFORM_GCN
    string<0x50> name;
    uint32 param[8];
    padding(4) /* ? */
    padding(4) /* ? */
    pointer<string<>> namePointer; /* ? */
#elif CPA_PLATFORM == CPA_PLATFORM_PS2
    stActionParam actionParam;
#endif
    pointer<stNodeInterpret> node;
    uint8 used;
    uint8 numRules;
    uint8 useDefaultReturn;
    uint8 newReturn;
  };
  
  struct stActionTable {
    pointer<stActionTableEntry> entries;
    uint8 numEntries;
    uint8 numEntriesUsed;
    uint8 currentEntry;
    padding(1)
  };
  
  struct stBehavior {
    string<0x100> name; /* 256 on GCN, at least */
    pointer<stTreeInterpret> scripts;
    pointer<stTreeInterpret> firstScript;
    uint8 numScripts;
    padding(3)
  };
  
  struct stMacro {
    string<0x100> name;
    pointer<stTreeInterpret> initialTree;
    pointer<stTreeInterpret> currentTree;
  };
  
  struct stMacroList {
    pointer<stMacro> macros;
    uint8 numMacros;
    padding(3)
  };
  
  struct stScriptAI {
    pointer<stBehavior> behavior;
    uint32 numBehaviors;
    uint32 noInitialization;
    uint8 numActionTableEntries;
    padding(3)
  };
  
  struct stIntelligence {
    doublepointer<stScriptAI> scriptAI;
    pointer<stNodeInterpret> currentTree;
    pointer<stBehavior> currentBehavior;
    pointer<stBehavior> previousBehavior;
    pointer<> actionTable;
    uint32 initializeBehavior;
  };
  
  struct stDsgVarInfo {
    uint32 memOffset;
    uint32 type;
    int16 saveType;
    padding(2)
    uint32 objectTreeInitialType;
  };
  
  struct stDsgVar {
    pointer<> memory;
    pointer<> info;
    uint32 memorySize;
    uint8 infoLength;
    padding(3)
  };
  
  struct stDsgMem {
    doublepointer<stDsgVar> dsgVars;
    pointer<> initialBuffer;
    pointer<> currentBuffer;
  };
  
#pragma mark - GLI
  
  struct stVertex2D {
    float32 x;
    float32 y;
    float32 dz;
  };
  
  struct stCamera {
    int32 cameraMode;
    stTransform transform;
    float32 xAlpha;
    float32 yAlpha;
    float32 near;
    float32 far;
    float32 screen;
    stVertex2D scale;
    stVertex2D trans;
    float32 xProjectionR;
    float32 yprojectionR;
    stVector3D left;
    float32 dLeft;
    stVector3D right;
    float32 dRight;
    stVector3D up;
    float32 dUp;
    stVector3D down;
    float32 dDown;
    float32 ratio;
    uint8 transparency;
    float32 transpDist;
    uint8 mirrored;
  };
  
#pragma mark - WP
  
  struct stWayPoint {
    stVector3D point;
    float32 radius;
    pointer<stSuperObject> superobject;
  };
  
  struct stGraphNode {
    pointer<stGraphNode> next;
    pointer<stGraphNode> prev;
    pointer<stGraph> graph;
    pointer<stWayPoint> wayPoint;
    int32 wayPointType;
    int32 wayPointTypeInitial;
    pointer<> arcList;
  };
  
  struct stGraph {
    stDoublyLinkedList<stGraphNode> nodes;
  };
  
  struct stGraphChainList {
    pointer<stGraph> graph;
    pointer<stGraphChainList> next;
  };
  
  struct stMSWay {
    pointer<stGraph> graph;
    int32 index;
    uint8 spherical;
    padding(3)
  };
  
  #undef padding
};


#endif /* structure_hh */

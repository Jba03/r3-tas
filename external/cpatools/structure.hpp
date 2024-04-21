#pragma once

#include <cpatools/common.hpp>
#include <cpatools/configuration.hpp>
#include <cpatools/types.hpp>
#include <cpatools/serialize.hpp>

#include <map>

namespace cpa::structure {
  
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
struct stDsgVarInfo;
struct stScriptAI;
struct stMacro;
struct stMacroList;
struct stBehavior;

// CINE
struct stCine;
struct stCineActor;

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
  
using Index3D = uint16;

#define CONCAT(a, b) CONCAT_INNER(a, b)
#define CONCAT_INNER(a, b) a ## b
#define UNIQUE_NAME(base) CONCAT(base, __LINE__)
#define padding(S) private: uint8_t UNIQUE_NAME(padding) [S]; public:

#define s(what) s.add(#what, what);
  
/** ``STRUCTURE BEGIN`` **/
  
#pragma mark - Common types
  
template< typename T = float32>
union vector2 {
  vector2() {};
  vector2(T _x, T _y) : x(_x), y(_y) {}
  
  auto dot(vector2 v) const { float s = 0.0f; for (auto i : range(2)) s += data[i] * v[i]; return s; }
  auto square() const { return dot(*this); }
  auto length() const { return sqrt(square()); }
  auto& operator [](auto i) { return data[i]; }
  
  auto operator +(vector2 v) { vector2 result; for(auto i : range(2)) result[i] = data[i] + v[i]; return result; }
  auto operator -(vector2 v) { vector2 result; for(auto i : range(2)) result[i] = data[i] - v[i]; return result; }
  auto operator *(vector2 v) { vector2 result; for(auto i : range(2)) result[i] = data[i] * v[i]; return result; }
  auto operator /(vector2 v) { vector2 result; for(auto i : range(2)) result[i] = data[i] / v[i]; return result; }
  auto operator *(float   s) { vector2 result; for(auto i : range(2)) result[i] = data[i] *    s; return result; }
  auto operator /(float   s) { vector2 result; for(auto i : range(2)) result[i] = data[i] /    s; return result; }
  auto operator -()          { vector2 result; for(auto i : range(2)) result[i] =-data[i];        return result; }
  auto operator >(vector2 v) { bool result = true; for(auto i : range(2)) if (data[i] <= v[i]) result = false; return result; }
  auto operator <(vector2 v) { bool result = true; for(auto i : range(2)) if (data[i] >= v[i]) result = false; return result; }
  auto operator>=(vector2 v) { bool result = true; for(auto i : range(2)) if (data[i] <  v[i]) result = false; return result; }
  auto operator<=(vector2 v) { bool result = true; for(auto i : range(2)) if (data[i] >  v[i]) result = false; return result; }
  
  auto normalize() {
    vector2 result = *this;
    if(length() == 0) return result;
    float scale = 1.0f / length();
    for(auto i : range(2)) result[i] *= scale;
    return result;
  }
  
  struct { T x, y; };
  
private:
  struct { std::array<T, 2> data; };
};

template< typename T = float32>
union vector3 {
  vector3() {}
  vector3(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}
  
  auto dot(vector3 v) const { float s = 0.0f; for (auto i : range(3)) s += data[i] * v[i]; return s; }
  auto square() const { return dot(*this); }
  auto length() const { return sqrt(square()); }
  auto& operator [](auto i) { return data[i]; }
  
  auto operator +(vector3 v) { vector3 result; for(auto i : range(3)) result[i] = data[i] + v[i]; return result; }
  auto operator -(vector3 v) { vector3 result; for(auto i : range(3)) result[i] = data[i] - v[i]; return result; }
  auto operator *(vector3 v) { vector3 result; for(auto i : range(3)) result[i] = data[i] * v[i]; return result; }
  auto operator /(vector3 v) { vector3 result; for(auto i : range(3)) result[i] = data[i] / v[i]; return result; }
  auto operator *(float   s) { vector3 result; for(auto i : range(3)) result[i] = data[i] *    s; return result; }
  auto operator /(float   s) { vector3 result; for(auto i : range(3)) result[i] = data[i] /    s; return result; }
  auto operator -()          { vector3 result; for(auto i : range(3)) result[i] =-data[i];        return result; }
  auto operator >(vector3 v) { bool result = true; for(auto i : range(3)) if (data[i] <= v[i]) result = false; return result; }
  auto operator <(vector3 v) { bool result = true; for(auto i : range(3)) if (data[i] >= v[i]) result = false; return result; }
  auto operator>=(vector3 v) { bool result = true; for(auto i : range(3)) if (data[i] <  v[i]) result = false; return result; }
  auto operator<=(vector3 v) { bool result = true; for(auto i : range(3)) if (data[i] >  v[i]) result = false; return result; }
  auto xy() -> vector2<T> { return vector2(x, y); }
  
  auto cross(vector3 v) -> vector3 {
    vector3 result;
    result.x = data[1] * v.data[2] - data[2] * v.data[1];
    result.y = data[2] * v.data[0] - data[0] * v.data[2];
    result.z = data[0] * v.data[1] - data[1] * v.data[0];
    return result;
  }
  
  auto normalize() {
    vector3 result = *this;
    if(length() == 0) return result;
    float scale = 1.0f / length();
    for(auto i : range(3)) result[i] *= scale;
    return result;
  }
  
  auto serialize(serializer::node& nd) {
    
  }
  
  struct { T x, y, z; };
  
private:
  struct { std::array<T, 3> data; };
};

template< typename T = float32>
union vector4 {
  vector4() {}
  vector4(T _x, T _y, T _z, T _w) : x(_x), y(_y), z(_z), w(_w) { }
  
  auto dot(vector4 v) const { float s = 0.0f; for (auto i : range(4)) s += data[i] * v[i]; return s; }
  auto square() const { return dot(*this); }
  auto length() const { return sqrt(square()); }
  auto& operator [](auto i) { return data[i]; }
  
  auto operator +(vector4 v) { vector4 result; for(auto i : range(4)) result[i] = data[i] + v[i]; return result; }
  auto operator -(vector4 v) { vector4 result; for(auto i : range(4)) result[i] = data[i] - v[i]; return result; }
  auto operator *(vector4 v) { vector4 result; for(auto i : range(4)) result[i] = data[i] * v[i]; return result; }
  auto operator /(vector4 v) { vector4 result; for(auto i : range(4)) result[i] = data[i] / v[i]; return result; }
  auto operator *(float   s) { vector4 result; for(auto i : range(4)) result[i] = data[i] *    s; return result; }
  auto operator /(float   s) { vector4 result; for(auto i : range(4)) result[i] = data[i] /    s; return result; }
  auto operator -()          { vector4 result; for(auto i : range(4)) result[i] =-data[i];        return result; }
  auto operator >(vector4 v) { bool result = true; for(auto i : range(4)) if (data[i] <= v[i]) result = false; return result; }
  auto operator <(vector4 v) { bool result = true; for(auto i : range(4)) if (data[i] >= v[i]) result = false; return result; }
  auto operator>=(vector4 v) { bool result = true; for(auto i : range(4)) if (data[i] <  v[i]) result = false; return result; }
  auto operator<=(vector4 v) { bool result = true; for(auto i : range(4)) if (data[i] >  v[i]) result = false; return result; }
  auto xy() -> vector2<T> { return vector2(x, y); }
  auto xyz() -> vector3<T> { return vector3(x, y, z); }
  
  auto normalize() {
    vector4 result = *this;
    if(length() == 0) return result;
    float scale = 1.0f / length();
    for(auto i : range(4)) result[i] *= scale;
    return result;
  }
  
  auto serialize(serializer::node& nd) {
    
  }
  
  struct { T x, y, z, w; };
private:
  struct { std::array<T, 4> data; };
};

using stVector2D = vector2<float32>;
using stVector3D = vector3<float32>;
using stVector4D = vector4<float32>;

template<unsigned Rows, unsigned Columns, typename T>
struct matrix {
  matrix() {
    for (auto y : range(Rows)) {
      for (auto x : range(Columns)) {
        (*this)(x, y) = (y == x ? 1.0f : 0.0f);
      }
    }
  };
  
  T& operator()(auto row, auto col) { return m[col + row * Columns]; }
  
  static auto identity() {
    matrix result;
    for (auto y : range(Rows)) {
      for (auto x : range(Columns)) {
        result(x, y) = (y == x ? 1.0f : 0.0f);
      }
    }
    return result;
  }
  
  template <unsigned R, unsigned C>
  auto operator* (matrix<R, C, T> src) {
    static_assert(Columns == C);
    matrix<Rows, Columns, T> result;
    for (auto y : range(Rows)) {
      for (auto x : range(C)) {
        T sum = 0.0f;
        for (auto z : range(Columns)) {
          sum += src(y, z) * (*this)(z, x);
        }
        result(y,x) = sum;
      }
    }
    return result;
  }
  
  auto operator* (stVector4D v) -> stVector4D {
    stVector4D result;
    for (auto y : range(Rows)) {
      result[y] = 0.0f;
      for (auto x : range(Columns)) {
        result[y] += (*this)(x,y) * v[x];
      }
    }
    return result;
  }
  
  auto operator* (stVector3D v) -> stVector4D {
    return ((*this) * stVector4D(v.x, v.y, v.z, 1.0f));
  }
  
  static inline auto make_translation(stVector3D p) {
    matrix result = identity();
    for (auto i : range(3)) result(Rows-1,i) = p[i];
    return result;
  }
  
  static inline auto make_scale(stVector3D p){
    matrix result = identity();
    for (auto i : range(3)) result(i,i) = p[i];
    return result;
  }
  
  static inline auto make_perspective(float fovY, float aspect, float near, float far) {
    float ct = 1.0f / std::tan(fovY / 2.0f);
    matrix<4,4,T> result = identity();
    result(0,0) = ct / aspect;
    result(1,1) = ct;
    result(2,2) = (far + near) / (near - far);
    result(2,3) = -1.0f;
    result(3,2) = (2.0f * far * near) / (near - far);
    result(3,3) = 0.0f;
    return result;
  }
  
  static inline auto make_lookat(stVector3D eye, stVector3D center, stVector3D up) {
    stVector3D n = (eye - center).normalize();
    stVector3D u = up.cross(n).normalize();
    stVector3D v = n.cross(u);
    
    float nnx = (-u).dot(eye);
    float nny = (-v).dot(eye);
    float nnz = (-n).dot(eye);
    
    matrix<4,4,T> result;
    result(0,0) = u.x;
    result(0,1) = v.x;
    result(0,2) = n.x;
    result(0,3) = 0.0f;
    result(1,0) = u.y;
    result(1,1) = v.y;
    result(1,2) = n.y;
    result(1,3) = 0.0f;
    result(2,0) = u.z;
    result(2,1) = v.z;
    result(2,2) = n.z;
    result(2,3) = 0.0f;
    result(3,0) = nnx;
    result(3,1) = nny;
    result(3,2) = nnz;
    result(3,3) = 1.0f;
    
    return result;
  }
  
  auto transpose() -> matrix {
    matrix<Rows, Columns, T> result;
    for (auto y : range(Rows)) {
      for (auto x : range(Columns)) {
        result(x,y) = (*this)(y,x);
      }
    }
    return result;
  }
  
  auto inverse() -> matrix<4,4,T> {
    matrix<4,4,T> result;
    
    float s0 = (*this)(0,0) * (*this)(1,1) - (*this)(1,0) * (*this)(0,1);
    float s1 = (*this)(0,0) * (*this)(1,2) - (*this)(1,0) * (*this)(0,2);
    float s2 = (*this)(0,0) * (*this)(1,3) - (*this)(1,0) * (*this)(0,3);
    float s3 = (*this)(0,1) * (*this)(1,2) - (*this)(1,1) * (*this)(0,2);
    float s4 = (*this)(0,1) * (*this)(1,3) - (*this)(1,1) * (*this)(0,3);
    float s5 = (*this)(0,2) * (*this)(1,3) - (*this)(1,2) * (*this)(0,3);
    float c5 = (*this)(2,2) * (*this)(3,3) - (*this)(3,2) * (*this)(2,3);
    float c4 = (*this)(2,1) * (*this)(3,3) - (*this)(3,1) * (*this)(2,3);
    float c3 = (*this)(2,1) * (*this)(3,2) - (*this)(3,1) * (*this)(2,2);
    float c2 = (*this)(2,0) * (*this)(3,3) - (*this)(3,0) * (*this)(2,3);
    float c1 = (*this)(2,0) * (*this)(3,2) - (*this)(3,0) * (*this)(2,2);
    float c0 = (*this)(2,0) * (*this)(3,1) - (*this)(3,0) * (*this)(2,1);
    
    float const det = (s0 * c5 - s1 * c4 + s2 * c3 + s3 * c2 - s4 * c1 + s5 * c0);
    float const invdet = 1.0f / det;
    
    /* If the determinant is zero,
      * the matrix is singular and cannot be inverted. */
    assert(det != 0.0f);
    
    result(0,0) = ( (*this)(1,1) * c5 - (*this)(1,2) * c4 + (*this)(1,3) * c3) * invdet;
    result(0,1) = (-(*this)(0,1) * c5 + (*this)(0,2) * c4 - (*this)(0,3) * c3) * invdet;
    result(0,2) = ( (*this)(3,1) * s5 - (*this)(3,2) * s4 + (*this)(3,3) * s3) * invdet;
    result(0,3) = (-(*this)(2,1) * s5 + (*this)(2,2) * s4 - (*this)(2,3) * s3) * invdet;
    result(1,0) = (-(*this)(1,0) * c5 + (*this)(1,2) * c2 - (*this)(1,3) * c1) * invdet;
    result(1,1) = ( (*this)(0,0) * c5 - (*this)(0,2) * c2 + (*this)(0,3) * c1) * invdet;
    result(1,2) = (-(*this)(3,0) * s5 + (*this)(3,2) * s2 - (*this)(3,3) * s1) * invdet;
    result(1,3) = ( (*this)(2,0) * s5 - (*this)(2,2) * s2 + (*this)(2,3) * s1) * invdet;
    result(2,0) = ( (*this)(1,0) * c4 - (*this)(1,1) * c2 + (*this)(1,3) * c0) * invdet;
    result(2,1) = (-(*this)(0,0) * c4 + (*this)(0,1) * c2 - (*this)(0,3) * c0) * invdet;
    result(2,2) = ( (*this)(3,0) * s4 - (*this)(3,1) * s2 + (*this)(3,3) * s0) * invdet;
    result(2,3) = (-(*this)(2,0) * s4 + (*this)(2,1) * s2 - (*this)(2,3) * s0) * invdet;
    result(3,0) = (-(*this)(1,0) * c3 + (*this)(1,1) * c1 - (*this)(1,2) * c0) * invdet;
    result(3,1) = ( (*this)(0,0) * c3 - (*this)(0,1) * c1 + (*this)(0,2) * c0) * invdet;
    result(3,2) = (-(*this)(3,0) * s3 + (*this)(3,1) * s1 - (*this)(3,2) * s0) * invdet;
    result(3,3) = ( (*this)(2,0) * s3 - (*this)(2,1) * s1 + (*this)(2,2) * s0) * invdet;
    
    return result;
  }
  
  inline auto translation() -> stVector3D& {
    return *(stVector3D*)&(*this)(Rows-1,0);
  }
  
  inline auto scale(bool ref = false) {
    if (ref) return vector<3,float32*>(&(*this)(0,0), &(*this)(1,1), &(*this)(2,2));
    return stVector3D((*this)(0,0), (*this)(1,1), (*this)(2,2));
  }
  
  auto serialize(serializer::node& s) {
    s.type("matrix<" + std::to_string(Rows) + "," + std::to_string(Columns) + ">");
    for (auto i : range(Rows*Columns)) s(m[i])
  }
  
  std::array<T, Rows * Columns> m;
};

using stMatrix3D = matrix<3, 3, float32>;
using stMatrix4D = matrix<4, 4, float32>;
  
#pragma mark - Containers

enum class LinkedListType { Single, Double };
/// A linked list
template<typename T = uint32, enum LinkedListType K = LinkedListType::Single>
struct LinkedList {
  pointer<T> first;
  std::conditional_t<K == LinkedListType::Single, pointer<T>, std::monostate> last;
  int32 numEntries;
  
  template<typename F> void forEach(const F& f, void *userdata = nullptr) {
//    pointer<T> c = first;
//    for (int i = 0; i < numEntries; i++) {
//      f(c++, userdata);
//    }
    try {
      for (T *c = first; c; c = c->next) {
        f(c, userdata);
      }
    } catch (...) {}
  }
  
  struct iterator {
    iterator(pointer<T> pos) : _pos(pos) { /* ... */ }
    auto operator*() -> pointer<T> { return _pos; }
    auto operator!=(iterator& source) -> bool { return _pos != source._pos; }
    auto operator++() -> iterator& { if (_pos) _pos = _pos->next; return *this; }
  private:
    pointer<T> _pos;
  };
  
  auto begin() -> iterator { return first; }
  auto end() -> iterator { return last; }
};

template<typename T = uint32> using stSingleLinkedList = LinkedList<T, LinkedListType::Single>;
template<typename T = uint32> using stDoublyLinkedList = LinkedList<T, LinkedListType::Single>;
template<typename T = uint32> using stLinkedList = stDoublyLinkedList<T>;

#pragma mark - stTransform

/// World transform
struct stTransform {
  
  enum class Type : uint32_t {
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
  
  stTransform() = default;
  
  /** transform type */
  uint32 type = 0;
  /** matrix */
  stMatrix4D matrix;
  /** external scale parameter */
  stVector4D scale;
  
  /// Get translation vector
  stVector3D& translation() {
    return matrix.translation();
  }
  
  auto getRotation(stVector3D& i, stVector3D& j, stVector3D& k) -> void {
    Type t = static_cast<Type>(uint32_t(type));
    if (t == Type::Rotation) {
      i = *(stVector3D*)&matrix(0,0);
      j = *(stVector3D*)&matrix(1,0);
      k = *(stVector3D*)&matrix(2,0);
    }
  }
  
  /** transform x stVector3D -> stVector3D */
  stVector3D operator * (stVector3D v);
  /** transform x stVector4D -> stVector4D */
  stVector4D operator * (stVector4D v);
  
  inline auto typeName() -> std::string {
    switch (static_cast<Type>(uint32_t(type))) {
      case Type::Uninitialized: return "Uninitialized";
      case Type::Identity: return "Identity";
      case Type::Translate: return "Translate";
      case Type::Zoom: return "Zoom";
      case Type::Scale: return "Scale";
      case Type::Rotation: return "Rotation";
      case Type::RotationZoom: return "RotationZoom";
      case Type::RotationScale: return "RotationScale";
      case Type::RotationScaleComplex: return "RotationScaleComplex";
      case Type::Undefined: return "Undefined";
      default: return "Invalid";
    }
  }
  
  auto serialize(serializer::node& s) {
    s(type)
    s(matrix)
    s(scale)
  }
};
  
#pragma mark - stAlways
  
struct stAlwaysModelList {
  pointer<stAlwaysModelList> next;
  pointer<stAlwaysModelList> prev;
  pointer<stLinkedList<stAlwaysModelList>> parentList;
  int32 objectModelType;
  pointer<stEngineObject> alwaysObject;
};

struct stAlways {
  uint32 numAlways;
  stDoublyLinkedList<stAlwaysModelList> alwaysModels;
  pointer<stSuperObject> alwaysSuperobject;
  pointer<stEngineObject> alwaysActors;
  pointer<stSuperObject> alwaysGeneratorSuperobjects;
};
  
#pragma mark - stObjectType
  
enum ObjectType {
  Family = 0,
  Model = 1,
  Instance = 2,
};

using ObjectNameResolver = std::function<std::string(ObjectType, int*)>;
  
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
  void serialize(serializer::node& s);
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
  enum mode : uint8_t {
    Invalid = 0,
    Initialize = 1,
    Deinitialize = 2,
    InitializeGameplay = 3,
    DeinitializeGameplay = 4,
    EnterLevel = 5,
    ChangeLevel = 6,
    Gameplay = 9,
  };
  
  enum inputMode : uint8_t {
    Normal = 0,
    Commands = 1,
  };
  
  mode mode;
  string<30> currentLevelName;
  string<30> nextLevelName;
  string<30> firstLevelName;
  inputMode inputMode = Normal;
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
  inline auto loadLevel(std::string levelName) -> void;
  
  void serialize(serializer& s);
};
  
#pragma mark - IPT

/// Structure for ReadAnalogJoystick function
struct stPadReadingOutput {
  /// The world vector the joystick value translates to
  stVector3D globalVector;
  int16 horizontalAxis;
  int16 verticalAxis;
  float32 analogForce;
  float32 trueAnalogForce;
  float32 rotationAngle;
  /// Strafe sector (0-7 clockwise)
  int32 strafeSector;
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
  pointer<string<>> actionName;
  pointer<string<>> entryName;
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
    uint32_t* T = table;
    return T ? ((T[i % RND_TABLE_COUNT] >> 16) & 0x7FFF) : 0;
  }
  /// Index the random number table using an index from tableIndices, optionally offset
  int32_t indexRelative(unsigned TableIndicesIdx, unsigned Offset) {
    return index(uint32_t(tableIndices[TableIndicesIdx]) + Offset);
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
  
  auto serialize(serializer::node& s) {
    s.type("stRandom");
    s(tableSize)
    s(tableIndices)
    s(lastIndex)
    s(tableMax)
    s(tableMaxInverse)
  }
};
  
#pragma mark - 3D
  
struct stAnim3D {
  
};

struct stSubAnim {
  pointer<stAnim3D> subAnim;
};

struct stActiveSubAnim {
  pointer<stActiveSubAnim> next;
  pointer<stActiveSubAnim> prev;
  stDoublyLinkedList<stActiveSubAnim> parent;
  pointer<stSubAnim> subAnim;
  pointer<uint8> eventActivation;
  uint32 startFrame;
  uint32 customBits;
  uint32 loop;
  float32 frame;
  uint8 nextEvent;
  uint8 stop;
  uint8 merge;
  padding(1);
};
  
#pragma mark - CINE
  
struct stCineActor {
  stSubAnim subAnim;
  pointer<stActiveSubAnim> activeSubAnim;
  string<255> animationName;
  padding(1)
  pointer<stEngineObject> actor;
  pointer</*stState*/> stateAfterCine;
  pointer</*stState*/> stateDuringCine;
  pointer</*stState*/> stateForActorTmp;
  uint8 skipAI;
  uint8 skipMechanics;
  uint8 previousAIState;
  uint8 previousMechanicsState;
  uint8 repeatAnimation;
  int8 animationSpeed;
  uint8 actorMoveAtStart;
  padding(1)
  pointer<stSuperObject> superobject;
  uint8 actorMoveAtEnd;
  padding(1)
  uint16 channel;
  uint8 playingAnimation;
  uint8 isSubAnim;
  uint8 changeIntelligenceAtStart;
  padding(1)
  pointer<stBehavior> intelligenceStart;
  uint8 changeReflexAtStart;
  padding(3)
  pointer<stBehavior> reflexStart;
  uint8 changeIntelligenceAtEnd;
  padding(3)
  pointer<stBehavior> intelligenceEnd;
  uint8 changeReflexAtEnd;
  padding(3)
  pointer<stBehavior> reflexEnd;
  uint8 startUseSoundRequest;
  uint8 startUseVoiceRequest;
  uint8 startUseMusicRequest;
  uint8 startUseAmbianceRequest;
  pointer</*stSoundBlockEvent*/> startSoundRequest;
  pointer</*stSoundBlockEvent*/> startVoiceRequest;
  pointer</*stSoundBlockEvent*/> startMusicRequest;
  pointer</*stSoundBlockEvent*/> startAmbianceRequest;
  uint8 endUseSoundRequest;
  uint8 endUseVoiceRequest;
  uint8 endUseMusicRequest;
  uint8 endUseAmbianceRequest;
  pointer</*stSoundBlockEvent*/> endSoundRequest;
  pointer</*stSoundBlockEvent*/> endVoiceRequest;
  pointer</*stSoundBlockEvent*/> endMusicRequest;
  pointer</*stSoundBlockEvent*/> endAmbianceRequest;
  pointer<stCine> cinematic;
  stDoublyLinkedList<> channelLink;
  pointer<stCineActor> next;
  pointer<stCineActor> prev;
  pointer<stDoublyLinkedList<stCineActor>> parents;
};

struct stCine {
  stDoublyLinkedList<stCineActor> actors;
  pointer<stCine> next;
  pointer<stCine> prev;
  pointer<stDoublyLinkedList<stCine>> parents;
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

struct stDynamicsObstacleMEC {
  float32 rate;
  stVector3D normal;
  stVector3D contact;
  pointer<> material;
  pointer<> collideMaterial;
  pointer<stSuperObject> superObject;
  uint32 type;
  int16 entity1;
  int16 entity2;
  stVector3D translation;
  stVector3D zoneMove;
  stVector3D zonePosition;
  float32 zoneRadius;
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
  
  /// Get the name of this actor in order of [Instance, Model, Family]
  inline auto name(ObjectType type = Instance) -> std::string;
  /// Get the superobject associated with this actor
  inline auto superobject() -> pointer<stSuperObject>;
  /// Return the AI model of this actor
  inline auto aiModel() -> pointer<stAIModel>;
  /// Get the dsg variable memory
  inline auto dsgMem() -> pointer<stDsgMem>;
  /// Get the dsg variable memory at specified index
  inline auto dsgVar(int idx, uint32_t* type = nullptr) -> pointer<>;
  /// Get the speed of this actor
  inline auto speed() -> stVector3D;
  /// Get the linear horizontal speed of this actor
  inline auto horizontalSpeed() -> float;
  /// Get the linear vertical speed of this actor
  inline auto verticalSpeed() -> float;
  /// Serialize this structure
  inline auto serialize(serializer::node& s);
};

using stActor = stEngineObject;
  
#pragma mark - SECT
  
struct stSector {
  enum priority : int8_t {
    Min = 0,
    Normal = 64,
    Max = 127,
  };
  
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
  
  static const inline uint32 minPriority    = 0;
  static const inline uint32 normalPriority = 64;
  static const inline uint32 maxPriority    = 127;
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
  
  enum type {
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
    Invalid            = 0xFFFF,
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
  pointer<stZdxListEntry> next;
  pointer<stZdxListEntry> prev;
  pointer<> parent;
  pointer<stCollideObject> data;
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
  
  /// Return a vector of all the collide zdx objects
  inline auto all() -> std::vector<pointer<stCollideObject>>;
};
  
struct stCsaList {
  stDoublyLinkedList<> list;
};

struct stZoneSetList {
  
};

struct stCollideSet {
  pointer<stZdxList> zddList;
  pointer<stZdxList> zdeList;
  pointer<stZdxList> zdmList;
  pointer<stZdxList> zdrList;
  pointer<stCsaList> zddActivationList;
  pointer<stCsaList> zdeActivationList;
  pointer<stCsaList> zdmActivationList;
  pointer<stCsaList> zdrActivationList;
  pointer<stZoneSetList> zddCurrentActivation;
  pointer<stZoneSetList> zdeCurrentActivation;
  pointer<stZoneSetList> zdrCurrentActivation;
  pointer<stZoneSetList> zdmCurrentActivation;
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
  pointer<stCollideMaterial> material;
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

#define COL_MAT_ID_MASK_NONE              (0 << 0)
#define COL_MAT_ID_MASK_SLIDE             (1 << 0)
#define COL_MAT_ID_MASK_TRAMPOLINE        (1 << 1)
#define COL_MAT_ID_MASK_GRABBABLE_LEDGE   (1 << 2)
#define COL_MAT_ID_MASK_WALL              (1 << 3)
#define COL_MAT_ID_MASK_UNKNOWN           (1 << 4)
#define COL_MAT_ID_MASK_HANGABLE_CEILING  (1 << 5)
#define COL_MAT_ID_MASK_CLIMBABLE_WALL    (1 << 6)
#define COL_MAT_ID_MASK_ELECTRIC          (1 << 7)
#define COL_MAT_ID_MASK_LAVA_DEATH_WARP   (1 << 8)
#define COL_MAT_ID_MASK_FALL_TRIGGER      (1 << 9)
#define COL_MAT_ID_MASK_HURT_TRIGGER      (1 << 10)
#define COL_MAT_ID_MASK_DEATH_WARP        (1 << 11)
#define COL_MAT_ID_MASK_UNKNOWN2          (1 << 12)
#define COL_MAT_ID_MASK_UNKNOWN3          (1 << 13)
#define COL_MAT_ID_MASK_WATER             (1 << 14)
#define COL_MAT_ID_MASK_NO_COLLISION      (1 << 15)
#define COL_MAT_ID_MASK_ALL               0xFFFF

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

struct stSuperObject {
  
  enum type : uint32_t {
    None                 = (0 << 0),
    World                = (1 << 0),
    Actor                = (1 << 1),
    Sector               = (1 << 2),
    PhysicalObject       = (1 << 3),
    PhysicalObjectMirror = (1 << 4),
    IPO                  = (1 << 5),
    IPOMirror            = (1 << 6),
    SpecialEffect        = (1 << 7),
    NoAction             = (1 << 8),
    Mirror               = (1 << 9),
  };
  
  uint32 type;
  
  union {
    pointer<> data;
    pointer<stEngineObject> actor;
    pointer<stSector> sector;
    pointer<stInstantiatedPhysicalObject> ipo;
    pointer<stPhysicalObject> physicalObject;
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
  stVector3D semiLookAt;
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
  
  struct iterator {
    iterator(pointer<stSuperObject> start) : obj(start) { /* ... */ }
    auto operator*() const -> pointer<stSuperObject> { return obj; }
    auto operator!=(iterator& source) -> bool { return obj != source.obj; }
    auto operator++() -> iterator& { obj = obj->next; return *this; }
    
  private:
    pointer<stSuperObject> obj;
  };
  
  /// Return the name of this superobject's type
  inline auto typeName() -> std::string;
  /// Return the name of the superobject
  inline auto name(bool fullname = false) -> std::string;
  /// Get the position of the supreobject
  inline auto position() -> stVector3D;
  
  /// Recurse the tree below this superobject
  template <typename F, typename UserData>
  auto recurse(const F& f, UserData userdata) {
    _recurse(this, userdata, f);
  }
    
  /// Run a custom function for each child object
  template <typename F>
  void forEachChild(const F& f, void *userdata = nullptr) {
    for (pointer<stSuperObject> ii = firstChild; ii; ii = ii->next) {
      f(ii, userdata);
    }
  }
  
  //iterator
  auto begin() const -> iterator { return firstChild; }
  auto end() const -> iterator { return lastChild; }
  //serialize
  inline auto serialize(serializer::node&);
  
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
  uint32 memoryOffset;
  uint32 type;
  int16 saveType;
  padding(2)
  uint32 objectTreeInitialType;
};

struct stDsgVar {
  pointer<> memory;
  pointer<stDsgVarInfo> info;
  uint32 memorySize;
  uint8 infoLength;
  padding(3)
};

struct stDsgMem {
  doublepointer<stDsgVar> dsgVars;
  pointer<> initialBuffer;
  pointer<> currentBuffer;
  
  inline auto dsgVarInfo(int idx) -> pointer<stDsgVarInfo> { return dsgVars->info[idx]; }
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
  
#pragma mark - Function -

#pragma mark EngineStructure
    
auto stEngineStructure::loadLevel(std::string levelName) -> void {
  nextLevelName = levelName;
  mode = stEngineStructure::mode::ChangeLevel;
}
    
#pragma mark EngineObject

auto stEngineObject::name(ObjectType type) -> std::string {
  std::string name;
  extern ObjectNameResolver nameResolver;
  for (int i : {stdGame->instanceType, stdGame->modelType, stdGame->familyType}) {
    name = nameResolver(type, &i);
    if (name != "Invalid name") break;
  }
  
  return name;
}
/// Get the superobject associated with this actor
auto stEngineObject::superobject() -> pointer<stSuperObject> {
  return stdGame->superObject;
}

/// Return the AI model of this actor
auto stEngineObject::aiModel() -> pointer<stAIModel> {
  return brain->mind->aiModel;
}

/// Get the dsg variable memory
auto stEngineObject::dsgMem() -> pointer<stDsgMem> {
  return brain->mind->dsgMem;
}

auto stEngineObject::dsgVar(int idx, uint32_t* type) -> pointer<> {
  try {
    pointer<stDsgMem> mem = brain->mind->dsgMem;
    if (idx > mem->dsgVars->infoLength) return nullptr;
    pointer<stDsgVarInfo> info = mem->dsgVarInfo(idx);
    if (type) *type = info->type;
    return (uint8_t*)mem->currentBuffer + info->memoryOffset;
  } catch (bad_pointer& e) {
    return nullptr;
  }
}

auto stEngineObject::speed() -> stVector3D {
  try {
    return dynam->dynamics->base.previousSpeed;
  } catch (bad_pointer& e) {
    return stVector3D();
  }
}

auto stEngineObject::horizontalSpeed() -> float {
  auto s = speed();
  return sqrt(s.x * s.x + s.y * s.y);
}

auto stEngineObject::verticalSpeed() -> float {
  return speed().z;
}

/// Serialize this structure
auto stEngineObject::serialize(serializer::node& s) {
  
}


#pragma mark - ZdxList

auto stZdxList::all() -> std::vector<pointer<stCollideObject>> {
  assert(list.numEntries == numZdx); //should never happen
  std::vector<pointer<stCollideObject>> objects;
  list.forEach([&](pointer<stZdxListEntry> entry, void*) { objects.emplace_back(entry->data); });
  return objects;
}

#pragma mark SuperObject

auto stSuperObject::typeName() -> std::string {
  switch (type) {
    case None: return "Dummy SuperObject";
    case World: return "World";
    case Actor: return "Actor";
    case Sector: return "Sector";
    case PhysicalObject: return "PhysicalObject";
    case PhysicalObjectMirror: return "PhysicalObjectMirror";
    case IPO: return "IPO";
    case IPOMirror: return "IPO.Mirror";
    case SpecialEffect: return "SpecialEffect";
    case NoAction: return "NoAction";
    case Mirror: return "Mirror";
    default: return "Invalid";
  }
}

auto stSuperObject::name(bool fullname) -> std::string {
  try {
    switch (type) {
      case Actor: return actor->name();
      case IPO: return fullname ? ipo->name : ipo->name.lastPathComponent();
      case Sector: return fullname ? sector->name : sector->name.lastPathComponent();
      default: return typeName();
    }
  } catch (bad_pointer& e) {
    return "";
  }
}
auto stSuperObject::position() -> stVector3D {
  try {
    return globalTransform->translation();
  } catch (bad_pointer& e) {
    return stVector3D();
  }
}

auto stSuperObject::serialize(serializer::node& s) {
  s.type("stSuperObject");
  s(type)
  s(data)
  s(firstChild)
  s(lastChild)
  s(numChildren);
  for (auto child : *this) s(child);
  s.add("next", next, false);
  s.add("prev", prev, false);
  s.add("parent", parent, false);
  s(localTransform);
  s(globalTransform);
  s(prevFrameProcessed);
  s(drawFlags);
  s(flags);
  s(visualBBox);
  s(collideBBox);
  s(semiLookAt);
  s(transparency);
  s(outlineColor);
  s(displayPriority);
  s(ilstatus);
  s(ambientColor);
  s(parallelDirection);
  s(parallelColor);
  s(superimpose);
  s(isSuperObject);
  s(transition);
}

#pragma mark - Static functions

static inline auto sectorSearch(pointer<stSuperObject> fatherSector, stVector3D point) -> pointer<stSuperObject> {
  try {
    float dNear = INFINITY;
    float dCurrent = INFINITY;
    float dVirtual = INFINITY;
    enum stSector::priority p = stSector::priority::Min;
    enum stSector::priority v = stSector::priority::Min;
    
    pointer<stSuperObject> targetSector = nullptr;
    pointer<stSuperObject> targetSectorVirtual = nullptr;
       
    fatherSector->forEachChild([&](pointer<stSuperObject> object, void*) {
      pointer<stSector> sector = object->sector;
      stVector3D min = sector->min;
      stVector3D max = sector->max;
      
      if (point >= min && point <= max) {
        stVector3D distance = (min + max) / 2.0f - point;
        dNear = distance.length();
        
        if (!sector->isVirtual) {
          if (sector->priority > p) {
            targetSector = object;
            dCurrent = dNear;
            p = static_cast<enum stSector::priority>(int8_t(sector->priority));
          } else if (sector->priority == p && dNear < dCurrent) {
            targetSector = object;
            dCurrent = dNear;
          }
        } else {
          if (sector->priority > v) {
            targetSectorVirtual = object;
            dVirtual = dNear;
            v = static_cast<enum stSector::priority>(int8_t(sector->priority));
          } else if (sector->priority == v && dNear < dVirtual) {
            targetSectorVirtual = object;
            dVirtual = dNear;
          }
        }
      }
    });
    
    if (!targetSector) targetSector = targetSectorVirtual;
    if (!targetSector) targetSector = fatherSector->lastChild; // UNIVERS
    return targetSector;
  } catch (...) {
    return nullptr;
  }
}

static auto rayTriangleIntersect(stVector3D &origin, stVector3D &direction, stVector3D& A, stVector3D& B, stVector3D& C, bool cullBackface, double &t) -> bool {
  stVector3D E1 = B - A;
  stVector3D E2 = C - A;
  stVector3D H = direction.cross(E2);
  
  const double det = E1.dot(H);
  const double invdet = 1.0f / det;
  
  if (det < EPSILON && cullBackface) return false;
  if (fabs(det) < EPSILON && !cullBackface) return false;
    
  stVector3D S = origin - A;
  const double u = S.dot(H) * invdet;
  if (u < 0.0f || u > 1.0f) return false;
  
  stVector3D Q = S.cross(E1);
  const double v = direction.dot(Q) * invdet;
  if (v < 0.0f || u + v > 1.0f) return false;
  
  t = invdet * E2.dot(Q);
  return t > EPSILON;
}

template<bool UseOctreeOptimization = false>
static auto segmentCollideObjectIntersect(pointer<stCollideObject> collObj, stMatrix4D T, stVector3D start, stVector3D end, stVector3D& intersectionPoint) -> bool {
  try {
    if (!collObj) return false;
    
    stVector3D origin = start;
    stVector3D direction = (end - start).normalize();
    const double l = (end - start).length();
    
    for (int i = 0; i < collObj->numElements; i++) {
      int16_t type = collObj->elementTypes[i];
      if (type == stCollideObject::type::IndexedTriangles) {
        pointer<stCollideElementIndexedTriangles> element = collObj->elements[i];
        stVector3D* vertices = collObj->vertices;
        uint16* indices = element->faceIndices;
        
        for (int16_t index = 0; index < element->numFaces; index++) {
          uint16 idx0 = *(indices + index * 3 + 0);
          uint16 idx1 = *(indices + index * 3 + 1);
          uint16 idx2 = *(indices + index * 3 + 2);
          
          stVector3D A = *(vertices + idx0);
          stVector3D B = *(vertices + idx1);
          stVector3D C = *(vertices + idx2);
          
          stVector3D TA = (T*A).xyz();
          stVector3D TB = (T*B).xyz();
          stVector3D TC = (T*C).xyz();
          
          double t;
          if (rayTriangleIntersect(origin, direction, TA, TB, TC, true, t)) {
            if (t < l) { // constrain to segment
              intersectionPoint = origin + direction * t;
              return true;
            }
          }
        }
      }
    }
    return false;
  } catch (...) {
    return false;
  }
}

#undef s
#undef padding
};


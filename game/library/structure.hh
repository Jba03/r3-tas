#ifndef structure_hh
#define structure_hh

#include <cstddef>
#include <bit>
#include <string>
#include <type_traits>
#include <iostream>

#include <cmath>

#include "constants.hh"

#define GCN     0
#define PS2     1
#define NATIVE  2

/* CONFIGURATION */
#define platform GCN

#if defined(htonl) && defined(htons)
# define OPTIMIZED_BYTESWAP
#endif

#define swap16(data) \
    ((((data) >> 8) & 0x00FF) | (((data) << 8) & 0xFF00))

#define swap32(data) \
    ((((data) >> 24) & 0x000000FF) | (((data) >>  8) & 0x0000FF00) | \
    ( ((data) <<  8) & 0x00FF0000) | (((data) << 24) & 0xFF000000) )

#ifdef MSB_FIRST
#
#   define host_byteorder_16(data) (data)
#   define host_byteorder_32(data) (data)
#   define game_byteorder_16(data) (data)
#   define game_byteorder_32(data) (data)
#
#else /* LSB_FIRST */
#
#   define host_byteorder_16(data) swap16(data)
#   define host_byteorder_32(data) swap32(data)
#   define game_byteorder_16(data) swap16(data)
#   define game_byteorder_32(data) swap32(data)
#
#endif

namespace library {
  
  #pragma mark - Memory
  namespace memory {
    
    /// Target memory base address
    extern const uint8_t *baseAddress;
    /// Memory size in bytes
    static size_t memorySize = 0;
    /// Is memory read-only?
    static bool readonly = false;
    
  #if platform == GCN
    static const std::endian endianness = std::endian::big;
    typedef uint32_t address_type;
  #elif platform == PS2
    static const std::endian endianness = std::endian::little;
    typedef uint32_t address_type;
  #elif platform == PC
    static const std::endian endianness = std::endian::little;
    typedef uint64_t address_type;
  #elif platform == NATIVE
    static const std::endian endianness = std::endian::native;
    typedef uint64_t address_type;
  #endif
    
    /// Swap integral byteorder
    static auto swap(std::integral auto v) {
      if constexpr (memory::endianness != std::endian::native) {
    #ifdef OPTIMIZED_BYTESWAP
        if constexpr (sizeof(v) == 1) return v;
        if constexpr (sizeof(v) == 2) return htons(v);
        if constexpr (sizeof(v) == 4) return htonl(v);
    #else
        if constexpr (sizeof(v) == 1) return v;
        if constexpr (sizeof(v) == 2) return ((v>>8)&0x00FF)|((v<<8)&0xFF00);
        if constexpr (sizeof(v) == 4) return ((v>>24)&0xFF)|((v>>8)&0xFF00)|((v<<8)&0xFF0000)|((v<<24)&0xFF000000);
    #endif
      }
    }
    
    struct address {
      address() {}
      
    #if platform == GCN
      operator uint32_t () { return addr; }
      auto physicalAddress() -> uint32_t { return swap(addr); }
      auto effectiveAddress() -> uint32_t { return swap(addr) & 0x7FFFFFFF; }
      address(address_type physAddr) { addr = swap(physAddr); }
      template <typename T> address(T *hostAddr) { addr = swap(uint32_t(long(hostAddr) - long(baseAddress))); }
    #elif platform == PS2
      
    #elif platform == PC
     
    #endif
      
      auto valid() -> bool { return effectiveAddress() != 0; }
      auto hostAddress() -> void* { return (void*)(baseAddress + effectiveAddress()); }
      
      address_type addr = 0;
    };
    
    template <typename T> static auto read(address addr) -> T* {
      return (T*)addr.hostAddress();
    }
  };
  
  // Type defined per platform byteorder
  template <typename T, typename OpT> struct type {
    template <typename S> type(S t) {
      if constexpr (std::is_same_v<S, float>) {
        // copy float to target
        data = memory::swap(*(T*)&t);
      } else {
        // copy integer to target
       // printf("copy from host to platform\n");
      }
    }
    
    type() {}
    
    operator OpT() {
      T tmp = memory::swap(*(T*)(&data));
      return *(OpT*)(&tmp);
    }
    
    auto memoryBound() -> bool {
      long dataAddr = long(&data);
      long baseAddr = long(memory::baseAddress);
      return dataAddr >= baseAddr && dataAddr < baseAddr + memory::memorySize;
    }
    
    template <typename S> type& operator =(const S s) {
      if (memoryBound() && memory::readonly) {
        printf("cannot write type: memory marked readonly\n");
        return *this;
      }
      //printf("assign\n");
      data = memory::swap(*(T*)&s);
      //data = s;
      return *this;
    }
    
    /// Get the memory offset of this type
    auto memoryOffset() -> memory::address {
      return memory::address(&data);
    }
    
//    std::ostream& operator<<(std::ostream& s) {
//      return s << std::hex << (OpT)this;
//    }
    
    #define swap memory::swap
    auto operator +(std::integral auto v) -> OpT { return OpT(data) + OpT(v); }
    auto operator -(std::integral auto v) -> OpT { return OpT(data) - OpT(v); }
    auto operator *(std::integral auto v) -> OpT { return OpT(data) * OpT(v); }
    auto operator /(std::integral auto v) -> OpT { return OpT(data) / OpT(v); }
    auto operator +=(OpT other) -> OpT { return (*this = *this + other); }
    auto operator -=(OpT other) -> OpT { return (*this = *this - other); }
    auto operator *=(OpT other) -> OpT { return (*this = *this * other); }
    auto operator /=(OpT other) -> OpT { return (*this = *this / other); }
    auto operator ++() -> type<T, OpT>& { *this += 1; return *this; }
    auto operator --() -> type<T, OpT>& { *this -= 1; return *this; }
    auto operator ++(int) -> T { type<T, OpT> c = *this; ++(*this); return c; }
    auto operator --(int) -> T { type<T, OpT> c = *this; --(*this); return c; }
    auto operator -() -> OpT { type<T, OpT> c = *this; return 0 - c; }
    #undef swap
    
    T data;
  };
  
  namespace external {
    extern std::function<std::string(int, int)> nameLookup;
  };
  
#ifdef TYPE_DECLARATIONS
  TYPE_DECLARATIONS();
#else
  using char8     = library::type<int8_t, int8_t>;
  using uchar8    = library::type<uint8_t, uint8_t>;
  using int8      = library::type<int8_t, int8_t>;
  using uint8     = library::type<uint8_t, uint8_t>;
  using int16     = library::type<int16_t, int16_t>;
  using uint16    = library::type<uint16_t, uint16_t>;
  using int32     = library::type<int32_t, int32_t>;
  using uint32    = library::type<uint32_t, uint32_t>;
  using int64     = library::type<int64_t, int64_t>;
  using uint64    = library::type<uint64_t, uint64_t>;
  using float32   = library::type<uint32_t, float>;
#endif
  
  struct bad_ptr {
    std::string msg;
    std::string what() { return msg; }
    bad_ptr(std::string s) : msg(s) {}
  };
  
  template <typename T = uint32> struct pointer {
    pointer(memory::address addr) : ptr(addr) {}
    pointer(void *addr) : ptr(addr) {}
    template <typename X> pointer(pointer<X>& other) : ptr(other.ptr) {}
    pointer() {}
    
    // Get the memory offset of this pointer
    auto memoryOffset() -> memory::address {
      return memory::address(&ptr);
    }
    
    /// Return the address the pointer points to
    auto pointeeAddress() -> memory::address {
      return ptr.effectiveAddress();
    }
    
    /// Return the object pointed at
    template <typename X = T> auto pointee() -> X* {
      return (memory::address_type)ptr != 0 ? memory::read<X>(ptr) : nullptr;
    }
    
    template <typename X = T> operator X*() {
      return pointee<X>();
    }
    
    template <typename X = T> X* operator ->() {
      X *obj = pointee();
      if (obj == nullptr) throw bad_ptr("bad pointer");
      else return obj;
    }
    
    template <typename X = T> X& operator [](std::integral auto idx) {
      X *obj = pointee<X>();
      if (obj == nullptr) throw bad_ptr("array access into bad pointer");
      else return *(obj + idx);
    }
    
    template <typename X> void operator =(X *other) {
      ptr = memory::address(other);
    }
    
    auto operator ++() -> pointer<T> {
      *this = ptr.effectiveAddress() + sizeof(memory::address_type);
      return *this;
    }
    
    auto operator ++(int) -> pointer<T> {
      pointer<T> o = ptr.effectiveAddress();
      *this = o + sizeof(memory::address_type);
      return o;
    }
    
    auto assign(T v) -> void {
      *(T*)pointee() = v;
    }
    
    operator bool() {
      return ptr.valid();
    }
    
    struct exception {
      std::string msg;
      exception(std::string s) : msg(s) {}
    };
    
    memory::address ptr;
  };
  
  template <typename T = memory::address> struct doublepointer : pointer<pointer<T>> {
    doublepointer(memory::address addr) : ptr(addr) { }
    template <typename X> doublepointer(doublepointer<X>& other) : ptr(other.ptr) {}
    doublepointer() {}
    
    /// Return the object pointed at
    template <typename X = T> auto pointee() -> X* {
      memory::address *primary = pointer<memory::address>(ptr.physicalAddress());
      if (primary == nullptr) return nullptr;
      return pointer<X>(primary->physicalAddress());
    }
    
    template <typename X = T> operator X*() {
      return pointee<X>();
    }
    
//    template <typename X = T> void* operator [](int idx) {
//      memory::address *primary = pointee();
//      if (primary == nullptr) {
//        throw bad_ptr("array access into bad (double)pointer");
//      } else {
//        // 7138fc
//        printf("ptr, prt: %X %X\n", long(&ptr) - long(memory::baseAddress), primary->effectiveAddress());
//       // void *secondary = (primary == nullptr ? nullptr : memory::read<X>(primary->effectiveAddress() + sizeof(memory::address) * idx));
//        return nullptr; //pointer<>(secondary);
//      }
//    }
    
    operator bool() {
      return ptr.valid();
    }
    
    memory::address ptr;
  };
  
  template <size_t size = std::string::npos> struct string {
    constexpr size_t length() { return size; }
    operator std::string() { return std::string(reinterpret_cast<char*>(string), size); }
    operator const char*() { return (const char*)string; }
    operator void*()       { return (void*)(&string); }
    
    auto operator ==(struct string& str) { return std::string(str) == std::string(this); }
    auto operator ==(std::string str) { return std::string(str) == std::string(reinterpret_cast<char*>(string), size); }
    auto operator ==(const char *str) { return std::string(str) == std::string(reinterpret_cast<char*>(string), size); }
    
    auto operator =(std::string& str) {
      std::memset(string, 0, size);
      std::memcpy(string, str.data(), size);
    }
    
    auto lastPathComponent() -> std::string {
      std::string str = *this;
      size_t idx = str.rfind(':');
      if (idx == std::string::npos) return "";
      return str.substr(idx + 1, std::string::npos);
    }
    
    int8_t string[size];
  };
  
#pragma mark -* Structure *-
  
  struct stEngineStructure;
  struct stEngineTimer;
  struct stCineManager;
  struct stLanguageStructure;
  struct stRandom;
  
  // IPT
  struct stInputStructure;
  struct stInputEntryElement;
  struct stPadReadingOutput;
  
  // HIE
  struct stSuperObject;
  
  // Actor
  struct stEngineObject;
  struct st3DData;
  struct stStandardGameInfo;
  struct stDynam;
  struct stBrain;
  struct stCollideSet;
  struct stCineInfo;
  struct stMSWay;
  struct stMSLight;
  struct stSectorInfo;
  struct stMicro;
  struct stMSSound;
  
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
  struct stBehaviour;
  
  // IPO
  struct stInstantiatedPhysicalObject;
  struct stPhysicalObject;
  
  // SECT
  struct stSector;
  
  // DNM
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
  
  // GMT
  struct stGameMaterial;
  
  // GLI
  struct stVertex2D;
  struct stCamera;
  struct stTexture;
  struct stAnimatedTextureNode;
  
  // WP
  struct stWayPoint;
  struct stGraph;
  struct stGraphNode;
  struct stGraphChainList;
  
  #if platform == NATIVE
  # define padding(S)
  #else
  # define padding(S) uint8_t UNIQUE_NAME(padding) [S];
  #endif

  /* macros to synthesize unique struct padding field names */
  #define CONCAT(a, b) CONCAT_INNER(a, b)
  #define CONCAT_INNER(a, b) a ## b
  #define UNIQUE_NAME(base) CONCAT(base, __LINE__)
  
  
#pragma mark - Math
  
  struct stVector3D {
    float32 x = 0.0f;
    float32 y = 0.0f;
    float32 z = 0.0f;
    stVector3D() {}
    stVector3D(float x, float y, float z) : x(x), y(y), z(z) {}
    auto square() -> float { return x * x + y * y + z * z; }
    auto length() -> float { return std::sqrtf(x * x + y * y + z * z); }
    auto dot(stVector3D v) -> float { return x * v.x + y * v.y + z * v.z; }
    auto operator +(stVector3D v) -> stVector3D& { x += v.x; y += v.y; z += v.z; return *this; }
    auto operator -(stVector3D v) -> stVector3D& { x -= v.x; y -= v.y; z -= v.z; return *this; }
    auto operator *(stVector3D v) -> stVector3D& { x *= v.x; y *= v.y; z *= v.z; return *this; }
    auto operator /(stVector3D v) -> stVector3D& { x /= v.x; y /= v.y; z /= v.z; return *this; }
    auto operator *(float s) -> stVector3D& { x *= s; y *= s; z *= s; return *this; }
    auto operator /(float s) -> stVector3D& { x /= s; y /= s; z /= s; return *this; }
  };
  
  struct stVector4D {
    float32 x = 0.0f;
    float32 y = 0.0f;
    float32 z = 0.0f;
    float32 w = 0.0f;
    stVector4D() {}
    stVector4D(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
    auto operator +(stVector4D v) -> stVector4D& { x += v.x; y += v.y; z += v.z; w += v.w; return *this; }
    auto operator -(stVector4D v) -> stVector4D& { x -= v.x; y -= v.y; z -= v.z; w -= v.w; return *this; }
    auto operator *(stVector4D v) -> stVector4D& { x *= v.x; y *= v.y; z *= v.z; w *= v.w; return *this; }
    auto operator /(stVector4D v) -> stVector4D& { x /= v.x; y /= v.y; z /= v.z; w /= v.w; return *this; }
    auto xyz() -> stVector3D { return stVector3D(x, y, z); }
  };
  
  union stMatrix3D {
    float32 m00, m01, m02;
    float32 m10, m11, m12;
    float32 m20, m21, m22;
    float32 m[3 * 3];
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
    
    stMatrix4D() {}
    stMatrix4D(float m00, float m01, float m02, float m03,
               float m10, float m11, float m12, float m13,
               float m20, float m21, float m22, float m23,
               float m30, float m31, float m32, float m33):
    m00(m00), m01(m01), m02(m02), m03(m03), m10(m10), m11(m11), m12(m12), m13(m13),
    m20(m20), m21(m21), m22(m22), m23(m23), m30(m30), m31(m31), m32(m32), m33(m33) { }
    
    auto operator *(stMatrix4D mm) -> stMatrix4D {
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
    
    static auto translation(float x, float y, float z) -> stMatrix4D {
      stMatrix4D result;
      result.m30 = x;
      result.m31 = y;
      result.m32 = z;
      return result;
    }
    
    static auto translation(stVector3D v) -> stMatrix4D {
      stMatrix4D result;
      result.m30 = v.x;
      result.m31 = v.y;
      result.m32 = v.z;
      return result;
    }
    
    static auto scale(float x, float y, float z) -> stMatrix4D {
      stMatrix4D result;
      result.m00 = x;
      result.m11 = y;
      result.m22 = z;
      return result;
    }
    
    static auto perspective(float fovY, float ar, float near, float far) -> stMatrix4D {
      float ct = 1.0f / std::tanf(fovY / 2.0f);
      stMatrix4D result;
      result.m00 = ct / ar;
      result.m11 = ct;
      result.m22 = (far + near) / (near - far);
      result.m23 = -1.0f;
      result.m32 = (2.0f * far * near) / (near - far);
      result.m33 = 0.0f;
      return result;
    }
    
    auto operator *(stVector4D v) -> stVector4D {
      stVector4D result;
      result.x = m[0] * v.x + m[4] * v.y + m[8]  * v.z + m[12] * v.w;
      result.y = m[1] * v.x + m[5] * v.y + m[9]  * v.z + m[13] * v.w;
      result.z = m[2] * v.x + m[6] * v.y + m[10] * v.z + m[14] * v.w;
      result.w = m[3] * v.x + m[7] * v.y + m[11] * v.z + m[15] * v.w;
      return result;
    }
    
    auto operator *(stVector3D v) -> stVector3D {
      stVector4D q = stVector4D(v.x, v.y, v.z, 1.0f);
      return (*this * q).xyz();
    }
  };
  
  
  
#pragma mark - Linked list
  
  template <typename T = uint32> struct stLinkedList {
    pointer<T> first;
    int32 numEntries;
  };

  template <typename T = uint32> struct stDoublyLinkedList {
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
  
  #define transformTypeUninitialized        0
  #define transformTypeIdentity             1
  #define transformTypeTranslate            2
  #define transformTypeZoom                 3
  #define transformTypeScale                4
  #define transformTypeRotation             5
  #define transformTypeRotationZoom         6
  #define transformTypeRotationScale        7
  #define transformTypeRotationScaleComplex 8
  #define transformTypeUndefined            9

  struct stTransform {
    uint32 type;
    stMatrix4D matrix;
    stVector4D scale;
    
    stTransform() : type(transformTypeIdentity) {
      matrix = stMatrix4D();
      scale = stVector4D(1.0f, 1.0f, 1.0f, 1.0f);
    }
      
    auto position() -> stVector3D {
      return stVector3D(matrix.m30, matrix.m31, matrix.m32);
    }
    
    auto translation() -> stVector3D& {
      return *(stVector3D*)&matrix.m30;
    }
    
    auto operator *(stVector3D v) -> stVector3D {
      return matrix * v;
    }
    
    auto operator *(stVector4D v) -> stVector4D {
      return matrix * v;
    }
  };
  
#pragma mark - Always
  
  struct stAlways {
    uint32 numAlways;
    stDoublyLinkedList<> alwaysModels;
    pointer<stSuperObject> alwaysSuperobject;
    pointer<stEngineObject> alwaysActors;
    pointer<stSuperObject> alwaysGeneratorSuperobjects;
  };
  
#pragma mark - Object type
  
  #define objectFamilyName      0
  #define objectModelName       1
  #define objectInstanceName    2

  struct stObjectTypeElement {
    pointer<stObjectTypeElement> next;
    pointer<stObjectTypeElement> prev;
    pointer<stDoublyLinkedList<stObjectTypeElement>> father;
    pointer<string<>> name;
    uint8 priority;
    uint8 identifier;
    padding(2)
  };

  struct stObjectType {
    stDoublyLinkedList<stObjectTypeElement> family;
    stDoublyLinkedList<stObjectTypeElement> model;
    stDoublyLinkedList<stObjectTypeElement> instance;
  };
  
#pragma mark - Engine structure
  
  // Engine mode
  #define engineModeInvalid         0
  #define engineModeInit            1
  #define engineModeDeinit          2
  #define engineModeInitGameplay    3
  #define engineModeDeinitGameplay  4
  #define engineModeEnterLevel      5
  #define engineModeChangeLevel     6
  #define engineModeGameplay        9
  
  // Input mode
  #define engineInputModeNormal   0
  #define engineInputModeCommands 1
  
  struct stEngineStructure {
    
    enum engineMode : uint8_t {
      invalid = 0,
      initialize = 1,
      deinitialize = 2,
      initializeGameplay = 3,
      deinitializeGameplay = 4,
      enterLevel = 5,
      changeLevel = 6,
      gameplay = 9,
    };
    
    enum inputMode : uint8_t {
      normal = 0,
      commands = 1,
    };
    
    struct stEngineTimer {
      uint32 frame;
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
    };
    
    auto reloadLevel() -> void {
      if (!memory::readonly) mode = engineModeChangeLevel;
    }
    
    auto loadLevel(std::string levelName) -> void {
      if (!memory::readonly) {
        nextLevelName = levelName;
        mode = engineModeChangeLevel;
      }
    }
    
    uint8 mode;
    string<30> currentLevelName;
    string<30> nextLevelName;
    string<30> firstLevelName;
    uint8 inputMode;
    uint8 displayFixMode;
    padding(3)
    uint32 displayMode;
    stEngineTimer timer;
    uint8 multimodePlayerCount;
    uint8 multimodeColumnCount;
    uint8 multimodeMiniScreenRatio;
    padding(1)
    pointer<stSuperObject> currentMainPlayers[4]; /* ::superobject */
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
    
    doublepointer<> SOLinks;
    pointer<stGraphChainList> graphList;
    pointer<stCineManager> cineManager;
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
  
  #define rndDefaultIndex 0

  struct stRandom {
    auto index(unsigned i) -> int32_t {
      if (!table) return 0;
      return (table[std::max(0u, i)] >> 16) & 0x7FFF;
    }
      
    auto indexRelative(unsigned idx, int offset) -> int32_t {
      if (!table) return 0;
      return index(tableIndices[idx] + offset);
    }

    auto call(unsigned n_calls, unsigned idx, unsigned mi, unsigned ma) -> int32_t {
      int32_t n, v = 0;
      for (n = 0; n < n_calls; n++)
        v = (mi + ((ma + 1 - mi) * indexRelative(idx, n)) / (tableMax + 1));
      return v;
    }
    
    uint32 tableSize;
    uint32 tableIndices[50];
    uint32 lastIndex;
    uint32 tableMax;
    float32 tableMaxInverse;
    pointer<uint32_t> table;
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
    stDoublyLinkedList<stCine> cineList;
    stTransform fixedCameraTransform;
    pointer<stSuperObject> camera;
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
    
    auto name(int type = objectInstanceName) -> std::string {
      std::string name = "Invalid name";
      int index[] = { stdGame->familyType, stdGame->modelType, stdGame->instanceType };
      
      if (external::nameLookup) {
        while (name == "Invalid name" && type >= 0) {
          name = external::nameLookup(type, index[type]);
          type--;
        }
      }
      
      return name;
    }
    
    auto familyName() -> std::string { return name(objectFamilyName); }
    auto modelName() -> std::string { return name(objectModelName); }
    auto instanceName() -> std::string { return name(objectInstanceName); }
  };
  
  
  
#pragma mark - SECT
  
  #define sectorPriorityMin     0
  #define sectorPriorityNormal  64
  #define sectorPriorityMax     127

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
  #if platform == gcn
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
    
    // Type
    #define collideElementIndexedTriangles    1
    #define collideElementFacemap             2
    #define collideElementSprite              3
    #define collideElementTMesh               4
    #define collideElementPoints              5
    #define collideElementLines               6
    #define collideElementIndexedSpheres      7
    #define collideElementAABB                8
    #define collideElementCones               9
    #define collideElementDeformationSetInfo  13
    
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
    
    template <typename F> auto forEachElement(const F& f) -> void {
      for (int i = 0; i < numElements; i++) {
        try {
          void *dptr = elements[i];
          int16 type = elementTypes[i];
          f(type, dptr);
        } catch (bad_ptr& e) {
          // s
        }
      }
    }
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
    pointer<> next;
    pointer<> prev;
    pointer<> parent;
    pointer<> data;
  };

  struct stZdxList {
    stDoublyLinkedList<stZdxListEntry> list;
    uint16 numZdx;
    padding(2)
    
    template <typename F>
    void forEachElement(const F& f) {
      list.forEach([&](stZdxListEntry *entry, void*) {
        if (entry) f(entry->data);
      });
    }
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
  #if platform == GCN
    string<0x32> name;
  #endif
    
    auto zdr() -> stCollideObject* {
      try {
        return physicalObject->physicalCollideset->zdr;
      } catch(bad_ptr& e) {
        return nullptr;
      }
    }
  };
  
  
#pragma mark - SPO
  
  #define superobjectTypeNone                 (0 << 0)
  #define superobjectTypeWorld                (1 << 0)
  #define superobjectTypeActor                (1 << 1)
  #define superobjectTypeSector               (1 << 2)
  #define superobjectTypePhysicalObject       (1 << 3)
  #define superobjectTypePhysicalObjectMirror (1 << 4)
  #define superobjectTypeIPO                  (1 << 5)
  #define superobjectTypeIPOMirror            (1 << 6)
  #define superobjectTypeSpecialEffect        (1 << 7)
  #define superobjectTypeNoAction             (1 << 8)
  #define superobjectTypeMirror               (1 << 9)
  
  struct stSuperObject {
    
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
      
    auto typeName() -> std::string {
      switch (type) {
        case superobjectTypeNone: return "Dummy Superobject";
        case superobjectTypeWorld: return "World";
        case superobjectTypeActor: return "Actor";
        case superobjectTypeSector: return "Sector";
        case superobjectTypePhysicalObject: return "PhysicalObject";
        case superobjectTypePhysicalObjectMirror: return "PhysicalObject.Mirror";
        case superobjectTypeIPO: return "IPO";
        case superobjectTypeIPOMirror: return "IPO.Mirror";
        case superobjectTypeSpecialEffect: return "SpecialEffect";
        case superobjectTypeNoAction: return "NoAction";
        case superobjectTypeMirror: return "Mirror";
        default: return "Unaddressed superobject type " + std::to_string(type);
      }
    }
    
    auto name(bool fullName = false) -> std::string {
      switch (type) {
        case superobjectTypeActor: return actor->instanceName();
        case superobjectTypeSector: return fullName ? sector->name : sector->name.lastPathComponent();
        case superobjectTypeIPO: return fullName ? ipo->name : ipo->name.lastPathComponent();
        default: return typeName();
      }
    }
    
    auto detatchFromParent() -> stSuperObject* {
      try { parent = this; (prev->next = next)->prev = prev; } catch(bad_ptr& e) {}
      return this;
    }
      
    template <typename F, typename UserData>
    void recurse(const F& f, UserData userdata) {
      _recurse(this, userdata, f);
    }
      
    template <typename F>
    void forEachChild(const F& f, void *userdata = nullptr) {
      for (stSuperObject *ii = firstChild; ii; ii = ii->next) {
        f(ii, userdata);
      }
    }
//
//    stSuperObject *find(std::string instanceName, stObjectType *objectTypes) {
//      stSuperObject *found = nullptr;
//      recurse([&](stSuperObject *T, void*) {
//        //printf("aa: %s\n", T->getName(objectTypes).c_str());
//        if (T->getName(objectTypes) == instanceName) found = T;
//      }, nullptr);
//      return found;
//    }
//
    
    // Locate the sector this object is currently in
    auto currentSector(stSuperObject *p_stFatherSector) -> pointer<stSuperObject> {
      if (!p_stFatherSector) return nullptr;
      
      float nx = INFINITY;
      float cx = INFINITY;
      float vx = INFINITY;
      int8 pr = sectorPriorityMin;
      int8 v = sectorPriorityMax;
      
      stSuperObject *target = nullptr;
      stSuperObject *virtualTarget = nullptr;
      
      p_stFatherSector->forEachChild([&](stSuperObject *spo, void*) {
        stVector3D min = spo->sector->min, max = spo->sector->max, p = globalTransform->position();
        if (p.x >= min.x && p.x <= max.x && p.y >= min.y && p.y <= max.y && p.z >= min.z && p.z <= max.z) {
          stVector3D d = (min + max) / 2.0f - p;
          nx = d.length();
          
          if (!spo->sector->isVirtual) {
            if (spo->sector->priority > pr) {
              target = spo;
              cx = nx;
              pr = spo->sector->priority;
            } else if (spo->sector->priority == pr && nx < cx) {
              target = spo;
              cx = nx;
            }
          } else {
            if (spo->sector->priority > v) {
              virtualTarget = spo;
              vx = nx;
              v = spo->sector->priority;
            } else if (spo->sector->priority == v && nx < vx) {
              virtualTarget = spo;
              vx = nx;
            }
          }
        }
      });
      
      if (!target) target = virtualTarget;
      if (!target) target = p_stFatherSector->lastChild;
      return target;
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
  
#pragma mark - AI -
  
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
  #if platform == GCN
    uint32 param;
    padding(3)
    uint8 type;
    padding(2)
    uint8 depth;
    padding(1)
  #endif
  };

  struct stTreeInterpret {
    pointer<stNodeInterpret> node;
  };
  
  struct stActionParam {
    
  };
  
  struct stActionTableEntry {
  #if platform == GCN
    string<0x50> name;
    uint32 param[8];
    padding(4) /* ? */
    padding(4) /* ? */
    pointer<string<>> namePointer; /* ? */
  #else
    stActionParam actionParam[8];
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
  
#pragma mark - Script translation
  
  struct scriptTranslationContext {
    
    scriptTranslationContext() {}
    
    struct token {
      token() {}
      token(std::string txt, pointer<stNodeInterpret> ref = nullptr) {
        text = txt;
        node = ref;
      }
      std::string text;
      pointer<stNodeInterpret> node;
      
      static inline const std::pair<std::string, pointer<stNodeInterpret>> non(std::string t) {
        return {t, nullptr};
      }
      
      static inline const std::pair<std::string, pointer<stNodeInterpret>> space {" ", nullptr};
      static inline const std::pair<std::string, pointer<stNodeInterpret>> exclamation {"!", nullptr};
    };
    
    
    template <typename... Args> void emit(bool condition, Args... args) {
      if (condition) {
        
      }
    }
    
    //std::vector<token>
    
//    std::vector<token> collect() {
//      std::vector<token> c;
//      return c;
//    }
//
    using tok = token;
    
//    std::vector<tok> translateChild(int child = 0) {
//      unsigned min =
//
////      unsigned min = t->current->depth;
////      unsigned occ = 0;
////
////          struct script_node* orig = t->current;
////          struct script_node* node = orig + 1;
////          while (!IsEndOfTree(node) && node->depth > min)
////          {
////              if (node->depth == min + 1 && occ++ == child)
////              {
////                  t->current = node;
////                  translate_node(t);
////                  break;
////              }
////              node++;
////          }
////
////          t->current = orig;
//    }
    
    void translateKeyword(unsigned s) {
      emit(s == 15,           "if", tok::space);
      emit(s >= 2 && s <= 7,  "if", tok::space, "framerule", tok::space, "%", tok::space, "==", tok::space);
      emit(s >= 8 && s <= 13, "if", tok::space, "" "framerule", tok::space, "%", tok::space, "==", tok::space);
    }
//
//    std::vector<token> translateOperator(unsigned s) {
////      emit(s == 0, "(", t0(), token::space, "+", token::space, ")");
////      emit(s == 1, "(", t1(), token::space, "-", token::space, ")");
////      emit(s == 2, "(", t1(), token::space, "*", token::space, ")");
////      emit(s == 3, "(", t1(), token::space, "/", token::space, ")");
////      emit(s == 2);
////      emit(s == 3);
////      emit(s == 4);
////      emit(s == 5);
////      emit(s == 6);
////      emit(s == 7);
////      emit(s == 8);
////      emit(s == 9);
////      emit(s == 10);
////      emit(s == 11);
////      emit(s == 12);
////      emit(s == 13);
////      emit(s == 14);
////      emit(s == 15);
////      emit(s == 16);
////      emit(s == 17);
////      emit(s == 18);
////      emit(s == 19);
////      emit(s == 20);
////      emit(s == 21);
////      emit(s == 22);
////      emit(s == 23);
////      emit(s == 24);
////      emit(s == 25);
////      emit(s == 26);
////      emit(s == 27);
//
//      return collect();
//    }
    
//    std::vector<token> translate(pointer<stNodeInterpret> node) {
//      std::vector<token> result;
//      return result;
//    }
//
//    std::vector<token> gameToText(stTreeInterpret *tree) {
//      std::vector<token> tokenList;
//      stNodeInterpret *firstNode = tree->node;
//      stNodeInterpret *currentNode = tree->node;
//
//      auto isEndOfTree = [currentNode] {
//        return currentNode->type != scriptNodeTypeEndMacro;
//      };
//
//      auto seekSameDepth = [&currentNode, isEndOfTree]() {
//        uint8 depth = currentNode->depth;
//        while (currentNode->depth != depth && !isEndOfTree()) currentNode++;
//      };
//
//      auto emit = [](std::string s) {
//
//      };
//
//      auto translateKeyword = [] {
//
//      };
//
//
//
//
//
//      return tokenList;
//    }
  };
  
  struct scriptInjection {
    
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
    stVector3D addedTranslation;
    stTransform previousTransform;
    stTransform currentTransform;
    stMatrix3D imposedRotation;
    uint8 nFrame;
    padding(3)
    pointer<stDynamicsReport> report;
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
  
  struct stDynamicsComplexBlock {
    
    struct macdpid {
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
    
    float32 tiltStrength;
    float32 tiltInertia;
    float32 tiltOrigin;
    float32 tiltAngle;
    float32 hangingLimit;
    stVector3D contact;
    stVector3D fallTranslation;
    macdpid macdpid;
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
  
#pragma mark - GMT
  
  struct stGameMaterial {
    int32 materialSound;
    pointer<stCollideMaterial> collideMaterial;
  };
  
#pragma mark - GEO
  
  struct stColorGEO {
    float32 r;
    float32 g;
    float32 b;
    float32 a;
  };
  
  struct stEdgeGEO {
    int16 points[2];
    stVector3D vectorPoints[2];
  };
  
  struct stParallelBoxGEO {
    stVector4D min;
    stVector4D max;
  };
  
  struct stGeometricObject {
    pointer<stVector3D> points;
    pointer<stVector3D> normals;
    doublepointer<float32> vertexTransparency;
    pointer<int16> elementTypes;
    doublepointer<> elements;
    pointer<stEdgeGEO> edges;
    pointer<stParallelBoxGEO> AABBs;
    uint32 type;
    int16 numPoints;
    int16 numElements;
    int16 numEdges;
    int16 numAABB;
    float32 boundingSphereRadius;
    padding(4)
    stVector4D boundingSphereCenter;
    pointer<> edgeList_di;
    int16 numEdges_di;
    int16 numOctreeEdges;
    int32 shadowDrawing;
    int32 shadowConstruction;
    pointer<> sdc;
    uint32 isStatic;
    uint32 displayList;
    uint8 sinus;
    padding(3)
  };
  
  union uVisualObjectGEO {
    pointer<stGeometricObject> geometricObject;
    pointer<> morphObject;
  };
  
  struct stVisualSetGEO {
    float32 lastDistance;
    int16 numLodDefs;
    int16 type;
    pointer<float32> thresholdTable;
    pointer<> lodDefinitions;
    doublepointer<> RLI;
    int32 numRLI;
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
  
  struct stTexture {
    int32 available;
    int32 quality;
    pointer<> data;
    pointer<> colorTable;
    pointer<> specularParam;
    int32 caps;
    uint16 height;
    uint16 width;
    uint16 realHeight;
    uint16 realWidth;
    float32 addU;
    float32 addV;
    int32 incrementEnable;
    uint32 chromaKeyColor;
    uint32 blendColor;
    int32 numLod;
    uint32 compressionCounter;
    uint32 compressionType;
    uint32 compressionMipMap;
    pointer<> substitutingTexture;
    uchar8 bilinearInterpolation;
    uchar8 cyclingMode;
    string<0x80> filename;
    padding(2)
    
    std::string name() {
      return filename.lastPathComponent();
    }
  };
  
  struct stAnimatedTextureNode {
    pointer<stTexture> texture;
    float32 displayDuration;
    pointer<stAnimatedTextureNode> next;
  };
  
  struct stUVTransformation {
    float32 matrix[2][2];
    float32 transformation[2];
  };
  
  struct stMultiTextureMaterial {
    pointer<stTexture> texture;
    int8 defaultOperator;
    int8 colorOperator;
    int8 uvSource;
    int8 flags;
    stUVTransformation staticUVTransform;
    stUVTransformation dynamicUVTransform;
  };
  
  struct stMaterial {
    uint32 type;
    stColorGEO ambient;
    stColorGEO diffuse;
    stColorGEO specular;
    stColorGEO color;
    uint32 additionalType;
    int32 specularExponent;
    pointer<stTexture> texture;
    float32 addU;
    float32 addV;
    float32 addConstantU;
    float32 addConstantV;
    int32 incrementEnable;
    uint32 actualRefreshNumber;
#if ((platform == GCN) || (platform == PC))
    pointer<stTexture> firstTextureNodeAnimated;
    pointer<stTexture> actualTextureNodeAnimated;
#elif (platform == PS2)
    pointer<stAnimatedTextureNode> firstTextureNodeAnimated;
    pointer<stAnimatedTextureNode> actualTextureNodeAnimated;
#endif
    int32 numDisplayModes;
    float32 actualDisplayTime;
    uchar8 locked;
    padding(3)
    uint32 flags;
    uint32 multiTextureType;
    uint32 numTextureStages;
    stMultiTextureMaterial multiTexturedMaterials[8];
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
  
};

#endif /* structure_hh */

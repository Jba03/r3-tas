#ifndef cpa_hh
#define cpa_hh

#include <array>
#include <bit>
#include <cmath>
#include <cstddef>
#include <map>
#include <string>
#include <type_traits>
#include <vector>
#include <fstream>

#define CPA_ENDIAN_BIG      0
#define CPA_ENDIAN_LITTLE   1

/* TODO: correct endianness */
#define CPA_MAKE_PLATFORM(I,E) ((I<<1)|E)
#define CPA_PLATFORM_GCN      CPA_MAKE_PLATFORM(0x1, CPA_ENDIAN_BIG)
#define CPA_PLATFORM_PS1      CPA_MAKE_PLATFORM(0x2, CPA_ENDIAN_LITTLE)
#define CPA_PLATFORM_PS2      CPA_MAKE_PLATFORM(0x3, CPA_ENDIAN_LITTLE)
#define CPA_PLATFORM_PS3      CPA_MAKE_PLATFORM(0x4, CPA_ENDIAN_BIG)
#define CPA_PLATFORM_XBOX     CPA_MAKE_PLATFORM(0x5, CPA_ENDIAN_BIG)
#define CPA_PLATFORM_XBOX360  CPA_MAKE_PLATFORM(0x6, CPA_ENDIAN_BIG)
#define CPA_PLATFORM_PC       CPA_MAKE_PLATFORM(0x7, CPA_ENDIAN_LITTLE)
#define CPA_PLATFORM_OSX      CPA_MAKE_PLATFORM(0x8, CPA_ENDIAN_BIG)
#define CPA_PLATFORM_DC       CPA_MAKE_PLATFORM(0x9, CPA_ENDIAN_BIG)
#define CPA_PLATFORM_NDS      CPA_MAKE_PLATFORM(0xA, CPA_ENDIAN_BIG)
#define CPA_PLATFORM_3DS      CPA_MAKE_PLATFORM(0xB, CPA_ENDIAN_BIG)
#define CPA_PLATFORM_N64      CPA_MAKE_PLATFORM(0xC, CPA_ENDIAN_BIG)

#define CPA_MAKE_VERSION(I,P) ((I<<6)|P)
#define CPA_VERSION_R2_PC                     CPA_MAKE_VERSION(0x00, CPA_PLATFORM_GCN)
#define CPA_VERSION_R2_PC_DEMO_1999_08_18     CPA_MAKE_VERSION(0x01, CPA_PLATFORM_GCN)
#define CPA_VERSION_R2_PC_DEMO_1999_09_04     CPA_MAKE_VERSION(0x02, CPA_PLATFORM_PS2)
#define CPA_VERSION_R2_PS1                    CPA_MAKE_VERSION(0x03, CPA_PLATFORM_PS1)
#define CPA_VERSION_R2_PS2                    CPA_MAKE_VERSION(0x04, CPA_PLATFORM_PS2)
#define CPA_VERSION_R2_N64                    CPA_MAKE_VERSION(0x05, CPA_PLATFORM_N64)
#define CPA_VERSION_R2_NDS                    CPA_MAKE_VERSION(0x06, CPA_PLATFORM_NDS)
#define CPA_VERSION_R2_3DS                    CPA_MAKE_VERSION(0x07, CPA_PLATFORM_3DS)
#define CPA_VERSION_R2_DC                     CPA_MAKE_VERSION(0x08, CPA_PLATFORM_DC)
#define CPA_VERSION_R2_DC_J                   CPA_MAKE_VERSION(0x09, CPA_PLATFORM_DC)
#define CPA_VERSION_R2R_PS2                   CPA_MAKE_VERSION(0x0A, CPA_PLATFORM_PS2)
#define CPA_VERSION_R2R_PS2_J                 CPA_MAKE_VERSION(0x0B, CPA_PLATFORM_PS2)
#define CPA_VERSION_R3_GCN                    CPA_MAKE_VERSION(0x0C, CPA_PLATFORM_GCN)
#define CPA_VERSION_R3_GCN_DEMO               CPA_MAKE_VERSION(0x0D, CPA_PLATFORM_GCN)
#define CPA_VERSION_R3_PS2                    CPA_MAKE_VERSION(0x0E, CPA_PLATFORM_PS2)
#define CPA_VERSION_R3_PS2_DEMO_2002_05_17    CPA_MAKE_VERSION(0x0F, CPA_PLATFORM_PS2)
#define CPA_VERSION_R3_PS2_DEMO_2002_08_07    CPA_MAKE_VERSION(0x10, CPA_PLATFORM_PS2)
#define CPA_VERSION_R3_PS2_DEVB_2002_09_06    CPA_MAKE_VERSION(0x11, CPA_PLATFORM_PS2)
#define CPA_VERSION_R3_PS2_DEMO_2002_10_29    CPA_MAKE_VERSION(0x12, CPA_PLATFORM_PS2)
#define CPA_VERSION_R3_PS2_DEMO_2002_12_18    CPA_MAKE_VERSION(0x13, CPA_PLATFORM_PS2)
#define CPA_VERSION_R3_PS3                    CPA_MAKE_VERSION(0x14, CPA_PLATFORM_PS3)
#define CPA_VERSION_R3_PC                     CPA_MAKE_VERSION(0x15, CPA_PLATFORM_PC)
#define CPA_VERSION_R3_PC_DEMO_2002_10_04     CPA_MAKE_VERSION(0x16, CPA_PLATFORM_PC)
#define CPA_VERSION_R3_PC_DEMO_2002_10_21     CPA_MAKE_VERSION(0x17, CPA_PLATFORM_PC)
#define CPA_VERSION_R3_PC_DEMO_2002_12_10     CPA_MAKE_VERSION(0x18, CPA_PLATFORM_PC)
#define CPA_VERSION_R3_PC_DEMO_2003_01_08     CPA_MAKE_VERSION(0x19, CPA_PLATFORM_PC)
#define CPA_VERSION_R3_PC_DEMO_2003_01_29     CPA_MAKE_VERSION(0x1A, CPA_PLATFORM_PC)
#define CPA_VERSION_R3_XBOX                   CPA_MAKE_VERSION(0x1B, CPA_PLATFORM_XBOX)
#define CPA_VERSION_R3_XBOX360                CPA_MAKE_VERSION(0x1C, CPA_PLATFORM_XBOX360)

#ifndef CPA_VERSION
# define CPA_VERSION CPA_VERSION_R3_GCN
#endif

#define CPA_PLATFORM (CPA_VERSION & 0xF)
#define CPA_ENDIANNESS (CPA_PLATFORM & 1)

#if defined(htonl) && defined(htons)
# define OPTIMIZED_BYTESWAP
#endif

struct CPA {
  
  #if CPA_ENDIANNESS == CPA_ENDIAN_BIG
    static constexpr std::endian endianness = std::endian::big;
    using address_type = uint32_t;
  #elif CPA_ENDIANNESS == CPA_ENDIAN_LITTLE
    static constexpr std::endian endianness = std::endian::little;
    using address_type = uint64_t;
  #endif
  
  /* macros for synthesis of unique struct field names */
  #define CONCAT(a, b) CONCAT_INNER(a, b)
  #define CONCAT_INNER(a, b) a ## b
  #define UNIQUE_NAME(base) CONCAT(base, __LINE__)
  
  struct memory {
    /* swap integral byteorder */
    static auto swap(std::integral auto v) {
      if constexpr (CPA::endianness != std::endian::native) {
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
  };
  
#pragma mark - Address
  
  template <typename T = address_type>
  struct address {
    address() : addr(0) { /* ... */ }
    
    template <typename X> address(X *hostAddress) {
      this->baseAddress = 0;
      addr = memory::swap(uint32_t(long(hostAddress) - long(this->baseAddress)));
    }

    operator T() const {
      return addr;
    }

    T physicalAddress() {
    #if CPA_PLATFORM == CPA_PLATFORM_GCN
      return memory_swap(addr);
    #else
      /* ... */
    #endif
    }

    T effectiveAddress() {
    #if CPA_PLATFORM == CPA_PLATFORM_GCN
      return memory::swap(addr) & 0x7FFFFFFF;
    #else
      /* ... */
    #endif
    }

    void* hostAddress() {
      return nullptr;//CPA::baseAddress() + effectiveAddress();
    }

    bool valid() {
      return effectiveAddress() != 0;
    }
    
  private:
    T addr;
  };
  
#pragma mark - Type
  
  template <class, template <class> class>
  struct is_instance : public std::false_type {};

  template <class T, template <class> class U>
  struct is_instance<U<T>, U> : public std::true_type {};
  
  template <typename T, typename OpT>
  struct type {
    type() { /* ... */ }
    
    template <typename S> type(S v) {
      if constexpr (std::is_same<S, float>::value) {
        /* copy float type to memory */
        data = memory::swap(*(T*)&v);
      } else if constexpr (std::is_integral<S>::value) {
        /* copy integral type to memory */

      }
    }
    
    template <typename S>
    type<T, OpT>& operator =(const S v) {
      if (memoryBound()) {
        return *this;
      }
      data = memory::swap(*(T*)&v);
      return *this;
    }
    
    operator OpT() const {
      T tmp = memory::swap(*(T*)(&data));
      return *(OpT*)(&tmp);
    }
    
    bool memoryBound() {
      return false;
    }
    
    T swap() {
      return memory::swap(data);
    }
    
    OpT operator +(std::integral auto v) { return OpT(data) + OpT(v); }
    OpT operator -(std::integral auto v) { return OpT(data) - OpT(v); }
    OpT operator *(std::integral auto v) { return OpT(data) * OpT(v); }
    OpT operator /(std::integral auto v) { return OpT(data) / OpT(v); }
    OpT operator +=(OpT other) { return (*this = *this + other); }
    OpT operator -=(OpT other) { return (*this = *this - other); }
    OpT operator *=(OpT other) { return (*this = *this * other); }
    OpT operator /=(OpT other) { return (*this = *this / other); }
    type<T, OpT>& operator ++() { *this += 1; return *this; }
    type<T, OpT>& operator --() { *this -= 1; return *this; }
    T operator ++(int) { type<T, OpT> c = *this; ++(*this); return c; }
    T operator --(int) { type<T, OpT> c = *this; --(*this); return c; }
    OpT operator -() { type<T, OpT> c = *this; return 0 - c; }
    
    using U = T;
  private:
    T data;
  };
  
  using char8   = type<int8_t, int8_t>;
  using uchar8  = type<uint8_t, uint8_t>;
  using int8    = type<int8_t, int8_t>;
  using uint8   = type<uint8_t, uint8_t>;
  using int16   = type<int16_t, int16_t>;
  using uint16  = type<uint16_t, uint16_t>;
  using int32   = type<int32_t, int32_t>;
  using uint32  = type<uint32_t, uint32_t>;
  using int64   = type<int64_t, int64_t>;
  using uint64  = type<uint64_t, uint64_t>;
  using float32 = type<uint32_t, float>;
  using local_address_type = type<address_type, address_type>;
  
#pragma mark - Pointer
  
  struct bad_pointer {
    std::string msg;
    std::string what() { return msg; }
    bad_pointer(std::string s) : msg(s) {}
  };
  
  template <typename T = local_address_type>
  struct pointer {
    pointer() {
      /* ... */
    }
    
    pointer(address<> addr) : ptr(addr) {
      /* ... */
    }
    
    pointer(void* addr) : ptr(addr) {
      /* ... */
    }
    
    template <typename X>
    pointer(pointer<X>& other) : ptr(other.ptr) {
      /* ... */
    }
    
    address<> memoryOffset() {
      return address<T>(&ptr);
    }
    
    address<> pointeeAddress() {
      return ptr.effectiveAddress();
    }
    
    template <typename X>
    X* pointee() {
      return ptr ? (X*)ptr.hostAddress() : nullptr;
    }
    
    template <typename X>
    operator X*() {
      return pointee<X>();
    }
    
    template <typename X>
    X* operator ->() {
      if (pointee() == nullptr) throw bad_pointer("bad pointer");
      return pointee();
    }
    
    template <typename X = T>
    X& operator [](std::integral auto idx) {
      X *obj = pointee<X>();
      if (obj == nullptr) throw bad_pointer("array access into bad pointer");
      else return *(obj + idx);
    }
    
    template <typename X>
    void operator =(X *other) {
      ptr = other;
    }
    
    pointer<T> operator +(int c) { return ptr.effectiveAddress() + sizeof(T) * c; }
    pointer<T> operator -(int c) { return ptr.effectiveAddress() - sizeof(T) * c; }
    pointer<T> operator ++() { return (*this = ptr.effectiveAddress() + sizeof(address_type));}
    pointer<T> operator ++(int) { pointer<T> o = ptr.effectiveAddress(); *this = o + sizeof(address_type); return o; }
    operator bool() { return ptr.valid(); }
    
  private:
    address<> ptr;
  };
  
  template <typename T = address<>>
  struct doublepointer : pointer<pointer<T>> {
    doublepointer(address<> addr) : ptr(addr) {
      /* ... */
    }
    
    template <typename X>
    doublepointer(doublepointer<X>& other) {
      
    }
    
    template <typename X>
    X* pointee() {
      address<> *primary = pointer<address<>>(ptr.physicalAddress());
      return primary ? pointer<X>(primary->physicalAddress()) : nullptr;
    }

    template <typename X>
    operator X*() {
      return pointee<X>();
    }
    
  private:
    address<> ptr;
  };
  
#pragma mark - String
  
  /// A string, '\0'-terminated unless size specified
  template <size_t size = std::string::npos>
  struct string {
    /// The length of the string
    size_t length() {
      return size;
    }
    
    /// Return the last path component, if such exists
    std::string lastPathComponent() {
      std::string str = *this;
      size_t idx = str.rfind(':');
      if (idx == std::string::npos) return "";
      return str.substr(idx + 1, std::string::npos);
    }
    
    operator std::string() {
      return std::string(reinterpret_cast<char*>(string), size);
    }
    
    operator const char*() {
      return (const char*)string;
    }
    
    operator void*() {
      return (void*)(&string);
    }
    
    auto operator ==(struct string& str) {
      return std::string(str) == std::string(this);
    }
    
    auto operator ==(std::string str) {
      return std::string(str) == std::string(reinterpret_cast<char*>(string), size);
    }
    
    auto operator ==(const char *str) {
      return std::string(str) == std::string(reinterpret_cast<char*>(string), size);
    }
    
    auto operator =(std::string& str) {
      std::memset(string.data(), 0, size);
      std::memcpy(string.data(), str.data(), size);
    }
    
  private:
    std::array<char8, size> string;
  };
  
#pragma mark - Serializer
  
  struct serializer {
    template<typename T>void add(T v) {
      
    }
  };
  
  struct serializable {
    //virtual void serialize(serializer& s);
  };
  
#pragma mark - Structure predeclarations
  
  /* globals */
  struct stEngineStructure;
  struct stEngineTimer;
  struct stCineManager;
  struct stLanguageStructure;
  struct stRandom;
  
  /* input */
  struct stInputStructure;
  struct stInputEntryElement;
  struct stPadReadingOutput;
  
  /* object */
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
  
  /* intelligence */
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
  
  /* dynamics */
  struct stDynam;
  struct stDynamics;
  struct stDynamicsBaseBlock;
  struct stDynamicsAdvancedBlock;
  struct stDynamicsComplexBlock;
  struct stDynamicsReport;
  struct stDynamicsRotation;
  struct stDynamicsMovevement;
  struct stDynamicsParsingData;
  
  /* collision */
  struct stPhysicalCollideSet;
  struct stCollideElementIndexedTriangles;
  struct stCollideElementIndexedTrianglesVisual;
  struct stCollideMaterial; // defined in GMT under previous versions
  
  /* graphics */
  struct stGameMaterial;
  struct stVertex2D;
  struct stCamera;
  struct stTexture;
  struct stAnimatedTextureNode;
  
  /* waypoints and graphs */
  struct stWayPoint;
  struct stGraph;
  struct stGraphNode;
  struct stGraphChainList;
  
#pragma mark - Structure -
  
  #if CPA_PLATFORM == NATIVE
  # define padding(S)
  #else
  # define padding(S) uint8_t UNIQUE_NAME(padding) [S];
  #endif
  
  struct structure : serializable {
    /* ... */
  };
  
  /** ``STRUCTURE BEGIN`` **/
  
#pragma mark - Common types
  
  struct stVector3D {
    float32 x = 0.0f;
    float32 y = 0.0f;
    float32 z = 0.0f;
    
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
  };
  
  struct stVector4D {
    float32 x = 0.0f;
    float32 y = 0.0f;
    float32 z = 0.0f;
    float32 w = 0.0f;
    
    const stVector4D operator +(stVector4D v) { x += v.x; y += v.y; z += v.z; w += v.w; return *this; }
    const stVector4D operator -(stVector4D v) { x -= v.x; y -= v.y; z -= v.z; w -= v.w; return *this; }
    const stVector4D operator *(stVector4D v) { x *= v.x; y *= v.y; z *= v.z; w *= v.w; return *this; }
    const stVector4D operator /(stVector4D v) { x /= v.x; y /= v.y; z /= v.z; w /= v.w; return *this; }
    const stVector4D operator *(float s) { x *= s; y *= s; z *= s; w *= s; return *this; }
    const stVector4D operator /(float s) { x /= s; y /= s; z /= s; w /= s; return *this; }
  };
  
  struct stMatrix3D {
    float32 m00 = 1.0f, m01 = 0.0f, m02 = 0.0f;
    float32 m10 = 0.0f, m11 = 1.0f, m12 = 0.0f;
    float32 m20 = 0.0f, m21 = 0.0f, m22 = 1.0f;
  };
  
  struct stMatrix4D {
    float32 m00 = 1.0f, m01 = 0.0f, m02 = 0.0f, m03 = 0.0f;
    float32 m10 = 0.0f, m11 = 1.0f, m12 = 0.0f, m13 = 0.0f;
    float32 m20 = 0.0f, m21 = 0.0f, m22 = 1.0f, m23 = 0.0f;
    float32 m30 = 0.0f, m31 = 0.0f, m32 = 0.0f, m33 = 1.0f;
  };
  
#pragma mark - Containers
  
  template <typename T = uint32>
  struct stLinkedList : structure {
    pointer<T>& begin();
    pointer<T>& end();
    void serialize(serializer& s);
  private:
    pointer<T> first;
    int32 numEntries;
  };
  
  template <typename T = uint32>
  struct stDoublyLinkedList : structure {
    pointer<T>& begin();
    pointer<T>& end();
    void serialize(serializer& s);
  private:
    pointer<T> first;
    pointer<T> last;
    int32 numEntries;
  };
  
#pragma mark - stTransform
  
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
  
#pragma mark - stObjectType
  
  struct stObjectTypeElement : structure {
    pointer<stObjectTypeElement> next;
    pointer<stObjectTypeElement> prev;
    pointer<stDoublyLinkedList<stObjectTypeElement>> father;
    pointer<string<>> name;
    uint8 priority;
    uint8 identifier;
    padding(2)
    void serialize(serializer& s);
  };
  
  struct stObjectType : structure {
    stDoublyLinkedList<stObjectTypeElement> family;
    stDoublyLinkedList<stObjectTypeElement> model;
    stDoublyLinkedList<stObjectTypeElement> instance;
    void serialize(serializer& s);
  };
  
#pragma mark - Engine
  
  struct stEngineTimer : structure {
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
  
  struct stEngineStructure : structure {
    
    enum EngineMode : uint8_t {
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
      normal = 0,
      commands = 1,
    };
    
    EngineMode mode = Invalid;
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
  
  struct stRandom {
    static const unsigned defaultIndex = 0;
    
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
  };
  
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
    pointer<stOctreeNode> selectedOctreeNodes[100]; // 100 on r3 gcn
    float32 selectedOctreeCoefficients[100];
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
  
#pragma mark - HIE
  
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
  
  /** ``STRUCTURE END`` **/
  
  #undef padding
  
#pragma mark - Namespace aliases
  
  /// WayPoint
  struct WP {
    using tdstWayPoint = stWayPoint;
    using stGraphNode = stGraphNode;
    using tdstGraph = stGraph;
    using tdstGraphChainList = stGraphChainList;
    using tdstMSWay = stMSWay;
  };
  
  /// Hierarchy
  struct HIE {
    using tdstSuperObject = stSuperObject;
  };
  
  /// Intelligence
  struct AI {
    using tdstBrain = stBrain;
    using tdstMind = stMind;
    using tdstAIModel = stAIModel;
    using tdstNodeInterpret = stNodeInterpret;
    using tdstTreeInterpret = stTreeInterpret;
    using tdstActionParam = stActionParam;
    using tdstActionTableEntry = stActionTableEntry;
    using tdstActionTable = stActionTable;
    using tdstBehavior = stBehavior;
    using tdstMacro = stMacro;
    using tdstMacroList = stMacroList;
    using tdstScriptAI = stScriptAI;
    using tdstIntelligence = stIntelligence;
    using tdstDsgVarInfo = stDsgVarInfo;
    using tdstDsgVar = stDsgVar;
    using tdstDsgMem = stDsgMem;
  };
  
  /// Collision
  struct COL {
    using tdstOctreeNode = stOctreeNode;
    using tdstOctree = stOctree;
    using tdstCollideObject = stCollideObject;
    using tdstPhysicalCollideSet = stPhysicalCollideSet;
    using tdstColliderInfo = stColliderInfo;
    using tdstZdxListEntry = stZdxListEntry;
    using tdstZdxList = stZdxList;
    using tdstCsaList = stCsaList;
    using tdstCollideSet = stCollideSet;
    using tdstCollideElementIndexedTriangles = stCollideElementIndexedTriangles;
    using tdstCollideElementIndexedSphere = stCollideElementIndexedSphere;
    using tdstCollideElementSpheres = stCollideElementSpheres;
    using tdstCollisionCase = stCollisionCase;
  };
  
private:
  void *baseAddress;
};

#endif /* cpa_hh */

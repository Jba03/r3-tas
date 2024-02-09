#ifndef cpa_hh
#define cpa_hh

#include <array>
#include <bit>
#include <cstddef>
#include <map>
#include <string>
#include <type_traits>
#include <vector>

#define platform GCN

#if defined(htonl) && defined(htons)
# define OPTIMIZED_BYTESWAP
#endif

/* macros for synthesis of unique struct field names */
#define CONCAT(a, b) CONCAT_INNER(a, b)
#define CONCAT_INNER(a, b) a ## b
#define UNIQUE_NAME(base) CONCAT(base, __LINE__)

// template <typename Platform> struct cpa
struct cpa {
  
  /** target platform endianness */
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
  
  /** swap integral byteorder */
  auto memory_swap(std::integral auto v);
  
#pragma mark - Address
  
  template <typename T = address_type>
  struct address {
    operator T() const;
    /** unmapped hardware address */
    T physicalAddress();
    /** emulated address */
    T effectiveAddress();
    /** is the address valid? */
    bool valid();
  private:
    T addr = 0;
  };
  
#pragma mark - Type
  
  template <typename T, typename OpT>
  struct type {
    type() : data(0) { /* ... */ }
    /** construct from arbitrary type */
    template <typename S> type(const S v);
    /** assign arbitrary type */
    template <typename S> type& operator =(const S v);
    /** cast to computational type */
    operator OpT() const;
    /** is the type bound to platform memory? */
    bool memoryBound();
    
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
  
  template <typename T = local_address_type>
  struct pointer {
    /** construct from address */
    pointer(address<> addr);
    /** construct from host address */
    pointer(void* addr);
    /** construct from other pointer */
    template <typename X> pointer(pointer<X>& other);
    
    /** get the memory offset of this pointer */
    address<> memoryOffset();
    /** return the address the pointer points to */
    address<> pointeeAddress();
    
    /** return object pointed at */
    template <typename X = T> X* pointee();
    /** return object pointed at */
    template <typename X = T> operator X*();
    /** access the pointer type */
    template <typename X = T> X* operator ->();
    /** access the pointer as array */
    template <typename X = T> X& operator [](std::integral auto idx);
    /** assign from host value */
    template <typename X> void operator =(X *other);
    
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
    /** construct from address */
    doublepointer(address<> addr);
    /** construct from other doublepointer */
    template <typename X> doublepointer(doublepointer<X>& other);
    /** return the object pointed at */
    template <typename X = T> X* pointee();
    /** return the object pointed at */
    template <typename X = T> operator X*();
    
  private:
    address<> ptr;
  };
  
#pragma mark - String
  
  /** a string, '\0'-terminated if size unspecified */
  template <size_t size = std::string::npos>
  struct string {
    constexpr size_t length();
    operator std::string();
    operator const char*();
    operator void*();
    
    bool operator ==(string& other);
    bool operator ==(std::string str);
    bool operator ==(const char* str);
    void operator =(std::string& str);
    /** return the last path component, if such exists */
    std::string lastPathComponent();
    
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
  
  #if platform == NATIVE
  # define padding(S)
  #else
  # define padding(S) uint8_t UNIQUE_NAME(padding) [S];
  #endif
  
  struct structure : serializable {
    /* ... */
  };
  
  /** ``STRUCTURE BEGIN`` **/
  
  template <typename T>struct x { T x; };
  template <typename T>struct y { T y; };
  template <typename T>struct z { T z; };
  template <typename T>struct w { T w; };
  
  /** arbitrary vector */
  template <typename T, int order>
  struct stVector {
    stVector() {}
    
//    std::enable_if_t<true>
//    A& get() { return obj; }
    
  private:
    std::array<T, order> elements;
  };
  
//  using stVector2D = stVector<float32, 2>;
//  using stVector3D = stVector<float32, 3>;
//  using stVector4D = stVector<float32, 4>;
  
  struct stTransform {
    
  };
  
  
  
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
  
#pragma mark -
  
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
  
#pragma mark -
  
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
    
    /** reload the current level */
    void reloadLevel();
    /** load level by name */
    void loadLevel(std::string levelName);
    
    void serialize(serializer& s);
  };
  
  
  /** ``STRUCTURE END`` **/
  
  #undef padding
  
};

#endif /* cpa_hh */

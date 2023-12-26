#ifndef library_hh
#define library_hh

#include <cstdint>
#include <string>
#include <type_traits>

#include "endianness.hh"
#include "constants.hh"

#define platformGCN 0
#define platformPS2 1
#define platformPC  2
#define platform    platformGCN

#pragma mark - Memory

const extern uint8_t * memoryBase;

#define GCN_POINTER_ENGINE_STRUCTURE        0x803E7C0C
#define GCN_POINTER_INPUT_STRUCTURE         0x8042F5A8
#define GCN_POINTER_FIX                     0x804334CC
#define GCN_POINTER_LVL                     0x804334D0
#define GCN_POINTER_RND                     0x80436924
#define GCN_POINTER_GHOST_MODE              0x805D8580
#define GCN_POINTER_INACTIVE_DYNAMIC_WORLD  0x805D8594
#define GCN_POINTER_FATHER_SECTOR           0x805D8598
#define GCN_POINTER_DYNAMIC_WORLD           0x805D859C
#define GCN_POINTER_ACTUAL_WORLD            0x805D85A0
#define GCN_POINTER_MENU_SELECTION_V        0x805D884C
#define GCN_POINTER_MENU_RUMBLEPAD          0x805D89B0

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

static float host_byteorder_f32(uint32_t f)
{
    uint32_t v = host_byteorder_32(f);
    float result = 0.0f;
    memcpy(&result, &v, sizeof v);
    return result;
}

/** game_byteorder_f32: convert host-byteorder float to game byteorder */
static uint32_t game_byteorder_f32(float f)
{
  float v = f;
  uint32_t bits = 0;
  memcpy(&bits, &f, sizeof v);
  return game_byteorder_32(bits);
}

#pragma mark - Type

template <typename T, bool fp = false> struct datatype
{
  T data;
    
  template <typename A> datatype(A type)
  {
      if (std::is_same<A, T>::value)
      {
        data = type;
      }
      else
      {
        if (std::is_floating_point<A>::value)
        {
          //printf("copying from: %f (target fp = %d)\n", type, fp);
          if (fp)
          {
            data = game_byteorder_f32(type);
          }
          else
          {
            data = type;
          }
          //printf("resulting from: %f(%X) (target fp = %d)\n", host_byteorder_f32(data), host_byteorder_32(data), fp);
        }
        else
        {
          data = type;
        }
      }
    }
    
  operator T()
  {
    switch (sizeof data)
    {
      case 1: return data;
      case 2: return swap16(data);
      case 4: return fp ? host_byteorder_f32(data) : swap32(data);
      default: return data;
    }
  }
    
  operator float() const { return fp ? host_byteorder_f32(data) : float(data); }
    
  float f() const { return host_byteorder_f32(data); }
        
//    datatype& operator =(datatype other)
//    {
//        switch (sizeof(T))
//        {
//            case 1: data = other;
//            case 2: data = game_byteorder_16(data);
//            case 4: fp ? game_byteorder_32(data) : host_byteorder_f32(data);
//            default: data = other;
//        }
//
//      return *this;
//
//      //return *this;
//
//        //data = game_byteorder_ other.data;
//    }
        
    
        
//    void operator= (const float& other)
//    {
//        printf("assign: %f -> %X\n", other, game_byteorder_f32(other));
//        data = game_byteorder_f32(other);
//        //return data;
//    }
    
    
  datatype operator [](int i)
  {
    return (T*)&data + i;
  }
        
        //template <typename A> datatype operator * (A v) { printf("mul: %d\n", fp); return fp ? float((host_byteorder_f32(data) * host_byteorder_f32(v))) : 2; }
    
    
    /* op. */
    //template <typename T> datatype operator+(T other) { return data + other; } /* pre */
    //template <typename T> datatype operator-(T other) { return data - other; } /* pre */
    //template <typename T> datatype operator*(T other) { return data * other; } /* pre */
    //template <typename T> datatype operator/(T other) { return data / other; } /* pre */
    
  datatype& operator++() { data++; return *this; } /* pre */
  datatype operator++(int) { datatype tmp = *this; data++; return tmp; } /* post */
  bool operator==(datatype& other) { return data == other.data; }
  bool operator!=(datatype& other) { return data == other.data; }
  bool operator<(datatype& other) { return data < other.data; }
  bool operator>(datatype& other) { return data > other.data; }
        
  datatype operator |=(datatype other) { return data |= game_byteorder_32(other.data);  }
  datatype operator &=(datatype other) { return data &= game_byteorder_32(other.data);  }
    
  uint32_t offset()
  {
    if ((uint8_t*)&data - memoryBase < 0x00) return 0x00;
    return ((uint8_t*)&data - (uint8_t*)memoryBase) | 0x80000000;
  }
    
//
//    operator std::string (const size_t sz = std::string::npos) const {
//        return std::string(&data, sz);
//    }
};

#ifndef UNDEF_TYPES
using char8     = datatype<int8_t>;
using uchar8    = datatype<uint8_t>;
using int8      = datatype<int8_t>;
using uint8     = datatype<uint8_t>;
using int16     = datatype<int16_t>;
using uint16    = datatype<uint16_t>;
using int32     = datatype<int32_t>;
using uint32    = datatype<uint32_t>;
using int64     = datatype<int64_t>;
using uint64    = datatype<uint64_t>;
using float32   = datatype<uint32_t, true>;
#endif

template <bool isHostByteOrder = false> uint32_t const read(uint32_t addr)
{
  uint32_t R = *(uint32_t*)(memoryBase + ((isHostByteOrder ? addr : host_byteorder_32(addr)) & 0x7FFFFFFF));
  return (!isHostByteOrder ? R : host_byteorder_32(R)) & 0x7FFFFFFF;
}

template <typename T = uint32_t, bool isHostByteOrder = false> struct pointer
{
#if platform == GCN
    static const uint32_t engineStructure =         0x803E7C0C;
    static const uint32_t inputStructure =          0x8042F5A8;
    static const uint32_t fix =                     0x804334CC;
    static const uint32_t lvl =                     0x804334D0;
    static const uint32_t rnd =                     0x80436924;
    static const uint32_t ghostMode =               0x805D8580;
    static const uint32_t inactiveDynamicWorld =    0x805D8594;
    static const uint32_t fatherSector =            0x805D8598;
    static const uint32_t dynamicWorld =            0x805D859C;
    static const uint32_t actualWorld =             0x805D85A0;
    static const uint32_t menuSelectionV =          0x805D884C;
    static const uint32_t menuOptionRumblepad =     0x805D89B0;
#endif
    
    uint32_t ptr;
    
    pointer(uint32_t addr) : ptr(addr) {}
    pointer(pointer& other) : ptr(other.ptr) {}
    
//    template <typename H> // Host address pointer to
//    pointer(H *other) : ptr((uint8_t*)other - memoryBase) {}
    
    auto address() -> uint32_t { return (isHostByteOrder ? ptr : host_byteorder_32(ptr)) & 0x7FFFFFFF; }
    auto offset()  -> uint32_t { return host_byteorder_32(ptr); }
    auto realAddress()-> void* { return &ptr; }
    auto offsetOfType()-> uint32_t { return (uint32_t)((long)&ptr - (long)memoryBase) | 0x80000000; }
  auto value() -> uint32_t { return offset(); }
    template <typename Q> Q index(int i) { return *((Q*)(memoryBase + address()) + i); }
    
    template <typename A>       operator A*()      { return address() ? (A*)(memoryBase + address()) : nullptr; }
    template <typename A = T> A*operator ->()      { return   (T*)(memoryBase + address()); }
    template <typename A = T> A&operator [](int i) { return *(T*)(memoryBase + address() + i * sizeof(T)); }
    operator bool () { return address() != 0x00; }
};

template <typename T = uint32_t, bool isHostByteOrder = false> struct doublepointer
{
    uint32_t ptr;
    
    doublepointer(uint32_t addr) : ptr(addr) {}
    
    auto address() -> uint32_t { return read<isHostByteOrder>(ptr); }
    auto offset()  -> uint32_t { return host_byteorder_32(address()); }
    auto realAddress()-> void* { return &ptr; }
    
    template <typename A>       operator A*()      { return address() ? (A*)(memoryBase + address()) : nullptr; }
//    template <typename A = T> A*operator ->()      { return   (T*)(memoryBase + address()); }
//    template <typename A = T> A operator [](int i) { return *((T*)(memoryBase + address()) + i); }
};

template <size_t size = std::string::npos>
struct string
{
  int8_t string[size];
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
};

template <size_t size = 0> struct padd { uint8_t _[size]; };
    
#include <fstream>
  
struct serializer {
  std::fstream stream;
    
  serializer(std::string filename) : stream(filename) {
    //std::ofstream file(filename);
  }
    
  template<typename T> auto write(T what) -> void {
    stream.write((const char*)what, sizeof(T));
  }
};
  
#define readonly // const

/* macros to synthesize unique struct padding field names */
#define concat(a, b) concat_inner(a, b)
#define concat_inner(a, b) a ## b
#define unique_name(base) concat(base, __LINE__)

/** padding: insert padding bytes. */
#define padding(S) readonly uint8_t unique_name(padding) [S];

#endif /* library_hh */

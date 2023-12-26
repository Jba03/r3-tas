#ifndef library_hh
#define library_hh

#include <cstdint>
#include <type_traits>

#include "endianness.hh"

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

#define GCN_POINTER_MENU_SELECTION_V    0x805D884C
#define GCN_POINTER_MENU_RUMBLEPAD      0x805D89B0

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

#define swap16(data) \
    ((((data) >> 8) & 0x00FF) | (((data) << 8) & 0xFF00))

#define swap32(data) \
    ((((data) >> 24) & 0x000000FF) | (((data) >>  8) & 0x0000FF00) | \
    ( ((data) <<  8) & 0x00FF0000) | (((data) << 24) & 0xFF000000) )

static inline float host_byteorder_f32(uint32_t f)
{
    uint32_t v = host_byteorder_32(f);
    return (*(float*)(&v));
}

/** game_byteorder_f32: convert host-byteorder float to game byteorder */
static inline uint32_t game_byteorder_f32(float f)
{
    uint32_t v = *(uint32_t*)(&f);
    return game_byteorder_32(v);
}

template <typename GameType, bool fp>
struct datatype {
    GameType data;
    datatype(GameType type) : data(type) {}
    
    operator GameType() {
        switch (sizeof data) {
            case 1: return data;
            case 2: return swap16(data);
            case 4: return fp ? host_byteorder_f32(data) : swap32(data);
            default: return data;
        }
    }
    
    operator float() const {
        return fp ? (float)data : host_byteorder_f32(data);
    }
    
//    datatype& operator =(datatype& other)
//    {
//        switch (sizeof(GameType))
//        {
//            case 1: return data;
//            case 2: return swap16(data);
//            case 4: return fp ? swap32(data) : host_byteorder_f32(data);
//            default: return data;
//        }
//        
//        data = game_byteorder_ other.data;
//    }
    
//    datatype& operator =(float other)
//    {
//        data = game_byteorder_f32(other);
//    }
    
    
    datatype operator [](int i)
    {
        return (GameType*)&data + i;
    }
    
    
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
    
    uint32_t offset() {
        if ((uint8_t*)&data - memoryBase < 0x00) return 0x00;
        return ((uint8_t*)&data - (uint8_t*)memoryBase) | 0x80000000;
    }
//
//    operator std::string (const size_t sz = std::string::npos) const {
//        return std::string(&data, sz);
//    }
};

using char8     = datatype<int8_t, false>;
using uchar8    = datatype<uint8_t, false>;
using int8      = datatype<int8_t, false>;
using uint8     = datatype<uint8_t, false>;
using int16     = datatype<int16_t, false>;
using uint16    = datatype<uint16_t, false>;
using int32     = datatype<int32_t, false>;
using uint32    = datatype<uint32_t, false>;
using int64     = datatype<int64_t, false>;
using uint64    = datatype<uint64_t, false>;
using float32   = datatype<uint32_t, true>;

template <bool isHostByteOrder = false> uint32_t const read(uint32_t addr) {
    uint32_t R = *(uint32_t*)(memoryBase + ((isHostByteOrder ? addr : host_byteorder_32(addr)) & 0x7FFFFFFF));
    return (!isHostByteOrder ? R : host_byteorder_32(R)) & 0x7FFFFFFF;
}

template <typename T = uint32_t, bool isHostByteOrder = false>
struct pointer {
    uint32_t ptr;
    
    pointer(uint32_t addr) : ptr(addr) {}
    pointer(pointer& other) : ptr(other.ptr) {}
    
//    template <typename H> // Host address pointer to 
//    pointer(H *other) : ptr((uint8_t*)other - memoryBase) {}
    
    auto address() -> uint32_t { return (isHostByteOrder ? ptr : host_byteorder_32(ptr)) & 0x7FFFFFFF; }
    auto offset()  -> uint32_t { return host_byteorder_32(ptr); }
    auto realAddress()-> void* { return &ptr; }
    template <typename Q> Q index(int i) { return *((Q*)(memoryBase + address()) + i); }
    
    template <typename A>       operator A*()      { return address() ? (A*)(memoryBase + address()) : nullptr; }
    template <typename A = T> A*operator ->()      { return   (T*)(memoryBase + address()); }
    template <typename A = T> A operator [](int i) { return *(T*)(memoryBase + address() + i * sizeof(T)); }
};

template <typename T = uint32_t, bool isHostByteOrder = false>
struct doublepointer {
    uint32_t ptr;
    
    doublepointer(uint32_t addr) : ptr(addr) {}
    
    auto address() -> uint32_t { return read<isHostByteOrder>(ptr); }
    auto offset()  -> uint32_t { return host_byteorder_32(address()); }
    auto realAddress()-> void* { return &ptr; }
    
    template <typename A>       operator A*()      { return address() ? (A*)(memoryBase + address()) : nullptr; }
//    template <typename A = T> A*operator ->()      { return   (T*)(memoryBase + address()); }
//    template <typename A = T> A operator [](int i) { return *((T*)(memoryBase + address()) + i); }
};

#define readonly // const

/* macros to synthesize unique struct padding field names */
#define concat(a, b) concat_inner(a, b)
#define concat_inner(a, b) a ## b
#define unique_name(base) concat(base, __LINE__)

/** padding: insert padding bytes. */
#define padding(S) readonly uint8_t unique_name(padding) [S];

#endif /* library_hh */

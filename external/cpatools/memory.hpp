#pragma once

#include <cstdint>
#include <fstream>
#include <type_traits>
#include <bit>

#if defined(htonl) && defined(htons)
# define OPTIMIZED_BYTESWAP
#endif

namespace cpa::memory {

using HostAddressType = void*;
using TargetAddressType = uint32_t;

static constexpr std::endian endianness = std::endian::big;

/// Swap integral byteorder
static auto bswap(auto v) {
  if constexpr (endianness != std::endian::native) {
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

/// Engine base address
extern memory::HostAddressType baseAddress;
/// Size of the memory space
extern size_t size;
/// Is the memory marked readonly?
extern bool readonly;

inline auto memoryBound(memory::HostAddressType addr) -> bool {
  return addr >= static_cast<uint8_t*>(baseAddress) && addr <= static_cast<uint8_t*>(baseAddress) + size;
}
  
};

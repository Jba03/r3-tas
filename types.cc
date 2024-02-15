#include "types.hh"

#include <algorithm>
#include <numeric>

namespace CPA {
  
  Address::Address() {
    /* ... */
  }
  
  Address::Address(Memory::TargetAddressType address) {
    addr = address;
  }
  
  Address::Address(Memory::HostAddressType hostAddress) {
    addr = Memory::bswap(uint32_t(long(hostAddress) - long(Memory::baseAddress)));
  }
  
  Memory::TargetAddressType Address::physicalAddress() {
    return Memory::bswap(addr);
  }
  
  Memory::TargetAddressType Address::effectiveAddress() {
    return Memory::bswap(addr) & 0x7FFFFFFF;
  }
  
  Memory::HostAddressType Address::hostAddress() {
    return valid() ? static_cast<uint8_t*>(Memory::baseAddress) + effectiveAddress() : nullptr;
  }
  
  bool Address::valid() {
    return effectiveAddress() != 0;
  }
  
  Address::operator bool() {
    return valid();
  }
  
};

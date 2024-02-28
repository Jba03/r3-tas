#include "memory.hh"

namespace CPA::Memory {
  /// Engine base address
  Memory::HostAddressType baseAddress = nullptr;
  /// Size of the memory space
  size_t size = 0;
};

namespace CPA {
  
  Stream::Stream(std::string filename) {
    std::ifstream s(filename, std::ios::binary | std::ios::ate);
    if (s.is_open()) {
      size = s.tellg();
      data = new uint8_t[size];
      s.seekg(0);
      s.read((char*)data, size);
    }
  }
  
  void Stream::seek(size_t off) {
    pos = std::clamp(off, 0ul, size);
  }
  
  void Stream::advance(size_t bytes) {
    pos += bytes;
  }
  
  size_t Stream::position() {
    return pos;
  }
  
};

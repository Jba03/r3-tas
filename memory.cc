#include "memory.hh"

namespace CPA::Memory {
  /// Engine base address
  Memory::HostAddressType baseAddress = nullptr;
  /// Size of the memory space
  size_t size = 0;
};

namespace CPA {
  
  Stream::Stream(std::string filename) {
    std::fstream s(filename, std::ios::binary | std::ios::ate);
    size = s.tellg();
    s.read((char*)data, size);
  }
  
  void Stream::seek(size_t off) {
    pos = std::clamp(off, 0ul, size);
  }
  
};

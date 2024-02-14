#ifndef fileformat_hh
#define fileformat_hh

#include "types.hh"
#include "memory.hh"

namespace CPA {
  namespace FileFormat {
    
    struct TPL {
      struct FileHeader {
        uint32 code;
        uint32 imageCount;
        uint32 imageTableOffset;
      };
      
      struct PaletteHeader {
        uint16 count;
        uint16 unknown;
        uint32 format;
        uint32 paletteDataOffset;
      };
      
      struct ImageHeader {
        uint16 height;
        uint16 width;
        uint32 format;
        uint32 imageDataOffset;
        // Unused?
        uint32 wrapS;
        uint32 wrapT;
        uint32 minFilter;
        uint32 magFilter;
        float32 lodBias;
        uint8 edgeLodEnable;
        uint8 minLod;
        uint8 maxLod;
        uint8 unpacked;
      };
      
      TPL(Stream& s);
      
      FileHeader header;
    };
    
  };
};

#endif /* fileformat_hh */

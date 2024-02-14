#include "fileformat.hh"
#include <stdio.h>

static void TPLBlockRGBA8Decode(int w, int h, uint8_t *data_in, uint32_t *data_out) {
  for (int bx = 0; bx < ceil(float(w) / 4.0f); bx++) {
    for (int by = 0; by < ceil(float(h) / 4.0f); by++) {
      for (int j = 0; j < 4 * 4; j++) {
        int py = bx * 4 + (j / 4);
        int px = by * 4 + (j % 4);
        uint8_t a = data_in[j * 2 + 0];
        uint8_t r = data_in[j * 2 + 1];
        uint8_t g = data_in[j * 2 + 32 + 0];
        uint8_t b = data_in[j * 2 + 32 + 1];
        data_out[px + py * h] = (a << 24) + (r << 16) + (g << 8) + (b);
      }
      data_in += 64;
    }
  }
}

namespace CPA {
  
  namespace FileFormat {
    
    TPL::TPL(Stream& s) {
      if (TPL::isTPL(s)) {
        FileHeader tplHeader = s.read<FileHeader>();
        s.seek(tplHeader.imageTableOffset);
        for (int i = 0; i < tplHeader.imageCount; i++) {
          uint32 off_imgheader = s.read<uint32>();
          uint32 off_palheader = s.read<uint32>();
          size_t ret = s.position();
          
          s.seek(off_imgheader);
          ImageHeader imgh = s.read<ImageHeader>();
          
          s.seek(imgh.imageDataOffset);
          uint32_t rgba[imgh.width * imgh.height * 4];
          if (imgh.format == 0x6) TPLBlockRGBA8Decode(imgh.height, imgh.width, s.data + s.position(), rgba);
          
          s.seek(ret);
        }
        
      }
    }
    
    template <typename FileIndex>
    PTR<FileIndex>::PTR(Stream& s) {
      uint32_t numPointers = s.read<uint32>();
      while (numPointers--) {
        FileIndex fileID = static_cast<FileIndex>(uint32_t(s.read<uint32>()));
        pointer<> ptr = s.read<pointer<>>();
        pointers.push_back({ fileID, ptr });
      }
      
      uint32_t numFillInPointers = (s.size - s.position()) / 16;
      while (numFillInPointers--) {
        pointer<> dptr = s.read<pointer<>>();
        FileIndex sourceFileID = static_cast<FileIndex>(uint32_t(s.read<uint32>()));
        pointer<> realptr = s.read<pointer<>>();
        FileIndex targetFileID = static_cast<FileIndex>(uint32_t(s.read<uint32>()));
        fillInPointers.push_back({ sourceFileID, dptr, targetFileID, realptr });
      }
    }
    
    template struct PTR<DataFileIndex>;
    
    
  };
  
};

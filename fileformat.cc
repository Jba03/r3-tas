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
      header = s.read<FileHeader>();
      if (header.code == 0x0020AF30) {
        s.seek(header.imageTableOffset);
        for (int i = 0; i < header.imageCount; i++) {
          uint32 off_imgheader = s.read<uint32>();
          uint32 off_palheader = s.read<uint32>();
          size_t check = s.position();
          
          s.seek(off_imgheader);
          ImageHeader imgh = s.read<ImageHeader>();
          
          s.seek(imgh.imageDataOffset);
          uint32_t rgba[imgh.width * imgh.height * 4];
          if (imgh.format == 0x6) TPLBlockRGBA8Decode(imgh.height, imgh.width, s.data + s.position(), rgba);
          
          s.seek(check);
        }
        
      }
    }
    
  };
  
};

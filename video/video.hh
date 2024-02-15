//
//  video.hpp
//  r3-tas
//
//  Created by Jba03 on 2024-01-14.
//

#ifndef video_hpp
#define video_hpp

#include "interface.hh"

struct VideoInterface {
  
  VideoInterface(Interface *ctx = nullptr);
  
  void Render(void *data = nullptr);
  
private:
  Interface *interface = nullptr;
};

#endif /* video_hpp */

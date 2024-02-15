//
//  video.cpp
//  r3-tas
//
//  Created by Jba03 on 2024-01-14.
//

#include "video.hh"
#include "interface.hh"
#include "gui.hh"

struct GCNVideoPayload {
  void *texture;
  void *context;
  bool windowed;
};

VideoInterface::VideoInterface(Interface *ctx) {
  interface = ctx;


  if (interface->Host == "GCN") {
    
  } else if (interface->Host == "PC") {
    
  }
  
  gui::initialize();
}

void VideoInterface::Render(void *data) {
#if platform == GCN
  GCNVideoPayload *payload = (GCNVideoPayload*)data;
  gui::draw(payload->context, payload->texture, &payload->windowed);
#endif
}

//
//  gui.hpp
//  r3lib
//
//  Created by Jba03 on 2022-11-20.
//

#ifndef gui_hpp
#define gui_hpp

#include <stdio.h>

#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

EXTERNC void render_callback(void* ctx);

#endif /* gui_hpp */

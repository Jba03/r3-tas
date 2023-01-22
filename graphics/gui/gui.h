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

EXTERNC void gui_render_callback(void* ctx);

EXTERNC void gui_render_game(void* xfb_texture);

EXTERNC void superobject_info(struct superobject* so);

#endif /* gui_hpp */

//
//  opengl.h
//  r3lib
//
//  Created by Jba03 on 2021-05-21.
//  Copyright © 2022 Jba03. All rights reserved.
//

#ifndef gl_h
#define gl_h

#if defined(__APPLE__)
#   define GL_SILENCE_DEPRECATION
#   include <OpenGL/gl.h>
#   include <OpenGL/gl3.h>
#   include <OpenGL/glu.h>
#else
#   include <GL/gl.h>
#endif

#endif /* opengl_h */

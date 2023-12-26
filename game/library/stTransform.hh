#ifndef stTransform_h
#define stTransform_h

#include "library.hh"
#include "stMatrix4D.hh"

#define transformTypeUninitialized        0
#define transformTypeIdentity             1
#define transformTypeTranslate            2
#define transformTypeZoom                 3
#define transformTypeScale                4
#define transformTypeRotation             5
#define transformTypeRotationZoom         6
#define transformTypeRotationScale        7
#define transformTypeRotationScaleComplex 8
#define transformTypeUndefined            9

struct stTransform {
  readonly uint32 type;
  readonly stMatrix4D matrix;
  readonly stVector4D scale;
  
  stTransform() : type(transformTypeIdentity) {
    matrix = stMatrix4D();
    scale = stVector4D(1.0f, 1.0f, 1.0f, 1.0f);
  }
    
  auto position() -> stVector3D {
    return stVector3D(matrix.m30, matrix.m31, matrix.m32).host();
  }
  
  auto setTranslation(stVector3D P) {
    *(uint32_t*)&matrix.m30 = game_byteorder_f32(P.x);
    *(uint32_t*)&matrix.m31 = game_byteorder_f32(P.y);
    *(uint32_t*)&matrix.m32 = game_byteorder_f32(P.z);
  }
};

#endif /* stTransform_h */

#ifndef stCameraGLI_h
#define stCameraGLI_h

#include "structure.h"
#include "stTransform.h"

typedef struct stVertex2DGLI tdstVertex2DGLI;
typedef struct stCameraGLI tdstCameraGLI;

struct stVertex2DGLI
{
    readonly float32 x;
    readonly float32 y;
    readonly float32 dz;
};

struct stCameraGLI
{
    readonly int32 cameraMode;
    readonly tdstTransform transform;
    readonly float32 xAlpha;
    readonly float32 yAlpha;
    readonly float32 near;
    readonly float32 far;
    readonly float32 screen;
    readonly tdstVertex2DGLI scale;
    readonly tdstVertex2DGLI trans;
    readonly float32 xProjectionR;
    readonly float32 yprojectionR;
    readonly tdstVector3D left;
    readonly float32 dLeft;
    readonly tdstVector3D right;
    readonly float32 dRight;
    readonly tdstVector3D up;
    readonly float32 dUp;
    readonly tdstVector3D down;
    readonly float32 dDown;
    readonly float32 ratio;
    readonly uint8 transparency;
    readonly float32 transpDist;
    readonly uint8 mirrored;
};

#endif /* stCameraGLI_h */

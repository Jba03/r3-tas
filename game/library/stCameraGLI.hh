#ifndef stCameraGLI_h
#define stCameraGLI_h

#include "library.hh"
#include "stTransform.hh"

struct stVertex2DGLI
{
    readonly float32 x;
    readonly float32 y;
    readonly float32 dz;
};

struct stCameraGLI
{
    readonly int32 cameraMode;
    readonly stTransform transform;
    readonly float32 xAlpha;
    readonly float32 yAlpha;
    readonly float32 near;
    readonly float32 far;
    readonly float32 screen;
    readonly stVertex2DGLI scale;
    readonly stVertex2DGLI trans;
    readonly float32 xProjectionR;
    readonly float32 yprojectionR;
    readonly stVector3D left;
    readonly float32 dLeft;
    readonly stVector3D right;
    readonly float32 dRight;
    readonly stVector3D up;
    readonly float32 dUp;
    readonly stVector3D down;
    readonly float32 dDown;
    readonly float32 ratio;
    readonly uint8 transparency;
    readonly float32 transpDist;
    readonly uint8 mirrored;
};

#endif /* stCameraGLI_h */

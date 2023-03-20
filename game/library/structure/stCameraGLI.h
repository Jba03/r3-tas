#ifndef stCameraGLI_h
#define stCameraGLI_h

#include "structure.h"
#include "stTransform.h"

struct stVertex2DGLI {
    readonly float32 x;
    readonly float32 y;
    readonly float32 dz;
} typedef tdstVertex2DGLI;

struct stCameraGLI {
    readonly int32 camera_mode;
    readonly tdstTransform transform;
    readonly float32 alpha_x;
    readonly float32 alpha_y;
    readonly float32 near;
    readonly float32 far;
    readonly float32 screen;
    readonly tdstVertex2DGLI scale;
    readonly tdstVertex2DGLI trans;
    readonly float32 projection_xr;
    readonly float32 projection_yr;
    readonly tdstVector3D left;
    readonly float32 d_left;
    readonly tdstVector3D right;
    readonly float32 d_right;
    readonly tdstVector3D up;
    readonly float32 d_up;
    readonly tdstVector3D down;
    readonly float32 d_down;
    readonly float32 ratio;
    readonly uint8 transparency;
    readonly float32 transp_dist;
    readonly uint8 mirrored;
} typedef tdstCameraGLI;

#endif /* stCameraGLI_h */

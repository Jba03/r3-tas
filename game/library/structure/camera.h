//
//  camera.h
//  r3-tas-memorymap
//
//  Created by Jba03 on 2023-03-14.
//

#ifndef camera_h
#define camera_h

#include "structure.h"
#include "transform.h"

struct gli_vertex2D
{
    readonly float32 x;
    readonly float32 y;
    readonly float32 dz;
};

struct gli_camera
{
    readonly int32 camera_mode;
    readonly struct transform transform;
    readonly float32 alpha_x;
    readonly float32 alpha_y;
    readonly float32 near;
    readonly float32 far;
    readonly float32 screen;
    readonly struct gli_vertex2D scale;
    readonly struct gli_vertex2D trans;
    readonly float32 projection_xr;
    readonly float32 projection_yr;
    readonly struct vector3 left;
    readonly float32 d_left;
    readonly struct vector3 right;
    readonly float32 d_right;
    readonly struct vector3 up;
    readonly float32 d_up;
    readonly struct vector3 down;
    readonly float32 d_down;
    readonly float32 ratio;
    readonly uint8 transparency;
    readonly float32 transp_dist;
    readonly uint8 mirrored;
};

#endif /* camera_h */

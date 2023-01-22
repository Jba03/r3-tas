//
//  camera.h
//  r3lib
//
//  Created by Jba03 on 2022-06-23.
//

#ifndef camera_h
#define camera_h

#include <stdbool.h>

#include "vector3.h"
#include "matrix4.h"

#define CAMERA_NEAR 0.1f
#define CAMERA_FAR  1000.0f

struct camera
{
    struct vector3 position;
    struct vector3 front;
    struct vector3 up;
    struct vector3 right;
    struct vector3 world_up;
    
    float yaw;
    float pitch;
    float speed;
    float sensitivity;
    float pitch_constrain;
    float zoom;
    float near;
    float far;
};

/**
 * Create a new perspective-projection camera.
 * 
 * @param yaw X-axis rotation
 * @param pitch Z-axis rotation
 * @param speed Camera movement speed
 * @param sens Panning sensitivity
 * @param zoom Camera zoom
 */
struct camera *camera_create(const float yaw, const float pitch, const float speed, const float sens, const float zoom);

/**
 * Create a new orthographic-projection camera.
 */
struct camera *camera_create_orthographic(const struct vector3 viewspace_dimensions);

/**
 * Updates the first-person camera using mouse coordinates
 *
 * @param camera Camera to be updated
 * @param x Mouse on-screen X coordinate
 * @param y Mouse on-screen Y coordinate
 * @param constrain_pitch Constrain the pitch within range of [+90, -90] degrees
 */
void camera_update(struct camera *camera, const float x, const float y, const bool constrain_pitch);

/**
 * Make the camera look at specified point
 *
 * @param camera Camera to be updated
 * @param point Point which camera should look at
 */
void camera_look_at(struct camera *camera, const struct vector3 point);

/**
 * Calculate specified camera's projection matrix
 */
const struct matrix4 camera_projection_matrix(const struct camera *camera, const float aspect_ratio);

/**
 * Calculate specified camera's view matrix
 */
const struct matrix4 camera_view_matrix(const struct camera *camera);

#endif /* camera_h */

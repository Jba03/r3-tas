//
//  camera.c
//  r3lib
//
//  Created by Jba03 on 2022-06-23.
//

#include "camera.h"
#include "global.h"

#pragma mark - Camera

const struct Matrix4 camera_projection_matrix(const struct Camera* camera, const float aspect_ratio) {
    return matrix4_perspective(radians(camera->zoom), aspect_ratio, camera->near, camera->far);
}

const struct Matrix4 camera_view_matrix(const struct Camera* camera) {
    return matrix4_lookat(camera->position, vector3_add(camera->position, camera->front), camera->up);
}

static void camera_update_vectors(struct Camera* camera) {
    
    const double yaw = radians(camera->yaw);
    const double pitch = radians(camera->pitch);
    
    const double x = cos(yaw) * cos(pitch);
    const double y = sin(pitch);
    const double z = sin(yaw) * cos(pitch);
    
    camera->front = vector3_normalize(vector3_new(x, y, z));
    camera->right = vector3_normalize(vector3_cross(camera->front, camera->world_up));
    camera->up = vector3_normalize(vector3_cross(camera->right, camera->front));
}

/* Updates the first-person camera with relative mouse position values */
void camera_update(struct Camera* camera, float x, float y, bool constrain) {
    camera->yaw += x * camera->sensitivity;
    camera->pitch += y * camera->sensitivity;
    
    if (constrain && camera->pitch > camera->pitch_constrain) camera->pitch = camera->pitch_constrain;
    if (constrain && camera->pitch < -camera->pitch_constrain) camera->pitch = -camera->pitch_constrain;
    camera_update_vectors(camera);
}

void camera_look_at(struct Camera* camera, const struct Vector3 point) {
    struct Vector3 dir = vector3_sub(camera->position, point);
    dir = vector3_normalize(dir);
    
    camera->yaw = -180.0f + degrees(atan2(dir.z, dir.x));
    camera->pitch = -(-degrees(atan2(sqrt(dir.z * dir.z + dir.x * dir.x), dir.y)) + 45.0f);
    
    camera_update_vectors(camera);
}

struct Camera *camera_create(const float yaw, const float pitch, const float speed, const float sens, const float zoom) {
    struct Camera *camera = malloc(sizeof *camera);
    
    camera->position = vector3_new(0.0f, 0.0f, 0.0f);
    camera->up = vector3_new(0.0f, 0.0f, 1.0f);
    camera->front = vector3_new(0.0f, 0.0f, -1.0f);
    camera->world_up = camera->up;
    
    camera->yaw = yaw;
    camera->pitch = pitch;
    camera->speed = speed;
    camera->sensitivity = sens;
    camera->pitch_constrain = 90.0f;
    camera->zoom = zoom;
    
    camera->near = CAMERA_NEAR;
    camera->far = CAMERA_FAR;
    
    camera_update_vectors(camera);
    
    return camera;
}

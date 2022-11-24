//
//  graphics.c
//  r3lib
//
//  Created by Jba03 on 2022-11-22.
//

#include "graphics.h"
#include "camera.h"
#include "common.h"
#include "opengl.h"

#include "engine.h"
#include "physical_object.h"
#include "glmesh.h"

#if defined(__APPLE__)
#include "SDL.h"
#include <dispatch/dispatch.h>

static dispatch_queue_t sdl_timer_queue;
static dispatch_source_t sdl_timer_source;
#endif

static SDL_Window* window;
static SDL_GLContext context;
struct Camera* camera;
static GLuint shader_main;

static int width;
static int height;

static struct Vector4 bg =
{
    .r = 0.01f,
    .g = 0.01f,
    .b = 0.01f,
    .a = 1.00f,
};

#define VERTEX                                                      \
"#version 330 core\n"                                               \
"layout (location = 0) in vec3 aPos;\n"                             \
"layout (location = 1) in vec3 aNormal;\n"                          \
"out vec3 position;\n"                                              \
"out vec3 normal;\n"                                                \
"uniform mat4 projection;\n"                                        \
"uniform mat4 view;\n"                                              \
"uniform mat4 model;\n"                                             \
"void main() {\n"                                                   \
"   position = vec3(model * vec4(aPos, 1.0f));\n"                   \
"   normal = aNormal;\n"                                            \
"   gl_Position = projection * view * model * vec4(aPos, 1.0f);\n"  \
"}"

#define FRAGMENT                                                    \
"#version 330 core\n"                                               \
"layout (location = 0) out vec4 out_color;\n"                             \
"in vec3 position;\n"                                              \
"in vec3 normal;\n" \
"uniform vec3 look_at;\n"    \
"void main() {\n"                                                   \
"   out_color = vec4(normal, 1.0f);\n"                                      \
"}"


static GLuint compile_shader(const char* source, const GLenum type)
{
    GLint status = 0;
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, 0);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        GLchar messages[1024];
        glGetShaderInfoLog(shader, sizeof messages, 0, &messages[0]);
        fprintf(stderr, "GLSL shader error: %s\n", messages);
    }
    
    return shader;
}

static GLuint shader_create(const char* vertex, const char* fragment)
{
    GLuint vs = 0;
    GLuint fs = 0;
    
    vs = compile_shader(vertex, GL_VERTEX_SHADER);
    fs = compile_shader(fragment, GL_FRAGMENT_SHADER);
    
    GLuint program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    
    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
    {
        GLchar messages[1024];
        glGetProgramInfoLog(program, sizeof messages, 0, &messages[0]);
        fprintf(stderr, "GLSL program error: %s", messages);
        return status;
    }
    
    /* Delete shaders */
    glDeleteShader(vs);
    glDeleteShader(fs);
    
    return program;
}

static void draw_ipo(struct SuperObject *obj, void* p)
{
    if (obj->type == SUPEROBJECT_TYPE_IPO && obj->data)
    {
        struct IPO *ipo = obj->data;
        if (ipo != NULL)
        {
            if (ipo->data && ipo->data_ptr != 0x00)
            {
                struct PhysicalObject *po = ipo->data;
                if (po->collide_set_ptr != 0x00 && po->geometry)
                {
                    struct CollisionGeometry *geom = po->geometry;
                    if (geom->element_types)
                    {
                        for (unsigned e = 0; e < geom->n_elements; e++)
                        {
                            if (geom->element_types[e] == 1) /* mesh */
                            {
                                struct Mesh *mesh = geom->elements[e];
                                //info("Mesh of %s\n", ipo->name);
                                if (mesh)
                                {
                                    if (mesh->glmesh)
                                    {
                                        //info("Drawing mesh: %d vertices, %d indices, vao: %d\n", mesh->glmesh->n_vertices, mesh->glmesh->n_indices, mesh->glmesh->vao);
                                        obj->matrix_default = matrix4_read(obj->matrix_default.offset);
                                        
                                        glUniformMatrix4fv(glGetUniformLocation(shader_main, "model"), 1, GL_FALSE, &obj->matrix_default.m00);
                                        glmesh_draw(mesh->glmesh);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

struct GLMesh* sphere;

static void graphics_main_loop()
{
    SDL_Event e;
    SDL_PollEvent(&e);
    
    SDL_GetWindowSize(window, &width, &height);
    
    camera->position = vector3_read(0x00c531bc);
    
    /* GCN default fov: 1.3rad */
    const float fov = memory.read_float(0x00C751B4);
    camera->zoom = fov == 0.0f ? degrees(1.30f) : degrees(fov);
    
    /* Camera parameters */
    struct Vector3 eye = camera->position;
    struct Vector3 up = vector3_new(-0.0f, 0.0f, 1.0f);
    struct Vector3 look_at = vector3_read(0x00c53910);
    
    struct Matrix4 projection = camera_projection_matrix(camera, (float)width / (float)height);
    struct Matrix4 view = matrix4_lookat(camera->position, look_at, up);
    
    /* Set program */
    glUseProgram(shader_main);
    
    glViewport(0, 0, width, height);
    
    /* Clear screen */
    glClearColor(bg.r, bg.g, bg.b, bg.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    /* Enable depth testing */
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    /* Disable face culling */
    glDisable(GL_CULL_FACE);
    
    /* Upload uniforms */
    glUniformMatrix4fv(glGetUniformLocation(shader_main, "view"), 1, GL_FALSE, &view.m00);
    glUniformMatrix4fv(glGetUniformLocation(shader_main, "projection"), 1, GL_FALSE, &projection.m00);
    glUniform3f(glGetUniformLocation(shader_main, "look_at"), look_at.x, look_at.y, look_at.z);
    
    if (engine)
    {
        if (engine->root)
        {
            superobject_for_every(SUPEROBJECT_TYPE_IPO, engine->root, &draw_ipo, engine->root);
        }
    }
    
    struct Vector3 rayman = vector3_read(0x00BF0D98);
    
    struct Matrix4 T;
    T = matrix4_make_scale(1,1,1);
    T = matrix4_mul(T, matrix4_make_translation(rayman.x, rayman.y, rayman.z));
    T = matrix4_transpose(T);
    
    glUniformMatrix4fv(glGetUniformLocation(shader_main, "model"), 1, GL_FALSE, &T.m00);
    glmesh_draw(sphere);
    
    SDL_GL_SwapWindow(window);
}

void graphics_init(void)
{
    sdl_timer_queue = dispatch_get_main_queue();
    sdl_timer_source = dispatch_source_create(DISPATCH_SOURCE_TYPE_TIMER, 0, 0, sdl_timer_queue);
    
    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow("", 0, 0, 800, 600, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
    
    /* Set OpenGL parameters */
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 2);
    SDL_GL_SetSwapInterval(0);
    
    context = SDL_GL_CreateContext(window);
    
    camera = camera_create(-90.0f, 0.0f, 0.1f, 0.25f, 45.0f);
    //camera->up = camera->world_up = vector3_new(0.0f, 1.0f, 0.0f);
    
    sphere = glmesh_sphere(20, 20);
    
    if ((shader_main = shader_create(VERTEX, FRAGMENT)) == 0)
    {
        warning("Could not create shader program\n");
        exit(-1);
    }
    else
    {
        info(BOLD COLOR_GREEN "GLSL shaders compiled successfully\n");
    }
    
    dispatch_source_set_event_handler(sdl_timer_source, ^{
        graphics_main_loop();
    });

    dispatch_source_set_timer(sdl_timer_source, dispatch_time(DISPATCH_TIME_NOW, NSEC_PER_SEC), NSEC_PER_SEC * (1.0f / 60.0f), 0);
    dispatch_resume(sdl_timer_source);
}

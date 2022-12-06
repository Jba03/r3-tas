//
//  graphics.c
//  r3lib
//
//  Created by Jba03 on 2022-11-22.
//

#include "graphics.h"
#include "camera.h"
#include "global.h"
#include "opengl.h"
#include "configuration.h"

#include "engine.h"
#include "physical_object.h"
#include "glmesh.h"
#include "predict.h"

#if defined(__APPLE__)
#   include "SDL.h"
#   include <dispatch/dispatch.h>
static dispatch_queue_t sdl_timer_queue;
static dispatch_source_t sdl_timer_source;
#endif

static SDL_Window* window;
static SDL_GLContext context;

static int width;
static int height;

static GLuint shader_main;
static GLuint checkerboard_texture;

static struct GLMesh *box = NULL;
static struct GLMesh *sphere = NULL;

static struct Vector4 bg =
{
    .r = 0.01f,
    .g = 0.01f,
    .b = 0.01f,
    .a = 1.00f,
};

#pragma mark - Shader

#define VERTEX                                                      \
"#version 330 core\n"                                               \
"layout (location = 0) in vec3 aPos;\n"                             \
"layout (location = 1) in vec3 aNormal;\n"                          \
"layout (location = 2) in vec2 aTexCoord;\n"                        \
"out vec3 position;\n"                                              \
"out vec3 normal;\n"                                                \
"out vec2 texcoord;\n"                                              \
"uniform mat4 projection;\n"                                        \
"uniform mat4 view;\n"                                              \
"uniform mat4 model;\n"                                             \
"void main() {\n"                                                   \
"   position = vec3(model * vec4(aPos, 1.0f));\n"                   \
"   normal = aNormal;\n"                                            \
"   texcoord = aTexCoord;\n"                                        \
"   gl_Position = projection * view * model * vec4(aPos, 1.0f);\n"  \
"}"

#define FRAGMENT                                                    \
"#version 330 core\n"                                               \
"layout (location = 0) out vec4 out_color;\n"                             \
"in vec3 position;\n"                                              \
"in vec3 normal;\n" \
"in vec2 texcoord;\n" \
"uniform sampler2D checkerboard;\n" \
"uniform vec3 camera;\n"    \
"vec3 lightPos = vec3(0, 100, 0);\n" \
"uniform bool display_normals = false;" \
"uniform vec4 color = vec4(1);\n" \
"void main() {\n"                                                   \
"   vec3 color2 = vec3(texture(checkerboard, texcoord * 4).r);\n" \
"   vec3 normal2 = normalize(normal);\n" \
"   vec3 lightColor = vec3(1,1,1);\n" \
"   vec3 ambient = vec3(0.1);\n" \
"   vec3 lightDir = normalize(lightPos - position);\n" \
"   float diff = max(dot(lightDir, normal2), 0.0);\n" \
"   vec3 diffuse = diff * lightColor;\n" \
"   vec3 viewDir = normalize(camera - position);\n" \
"   vec3 reflectDir = reflect(-lightDir, normal2);\n" \
"   float spec = 0.0;\n" \
"   vec3 halfwayDir = normalize(lightDir + viewDir);\n" \
"   spec = pow(max(dot(normal2, halfwayDir), 0.0), 64.0);\n" \
"   vec3 specular = spec * lightColor * 1.0f;\n" \
"   out_color = display_normals ? vec4(normal, 1.0f) :" \
"   vec4(vec3(diffuse + specular + ambient) * color2, 1.0f) * color;\n"                                      \
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

#pragma mark - Draw

void graphics_set_viewport(int w, int h)
{
    width = w;
    height = h;
}

int graphics_get_texture()
{
    return checkerboard_texture;
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
                                        
                                        glEnable(GL_DEPTH_TEST);
                                        
                                        struct Vector4 color = vector4_new(1.0f, 1.0, 1.0f, 1.0f);
                                        glUniform4fv(glGetUniformLocation(shader_main, "color"), 1, &color.x);
                                        
                                        glUniformMatrix4fv(glGetUniformLocation(shader_main, "model"), 1, GL_FALSE, &obj->matrix_default.m00);
                                        glmesh_draw(mesh->glmesh);
                                    }
                                    
//                                    for (int i = 0; i < mesh->n_triangles * 3; i++) {
//                                        struct Vector3 p1 = mesh->processed.vertices[i];
//                                        struct Vector3 p2 = vector3_new(p1.x, p1.z, p1.y);
//
//                                        struct Vector4 color = vector4_new(0.0f, 1.0, 1.0f, 1.0f);
//                                        glUniform4fv(glGetUniformLocation(shader_main, "color"), 1, &color.x);
//                                        struct Vector3 rayman_position = vector3_read(0x00BF0D98);
//                                        graphics_draw_line(rayman_position, p2);
//                                        //graphics_draw_sphere(p2, 0.1, color);
//                                    }
                                }
                            }
                        }
                    }
                    
                    if (geom->octree && geom->octree_offset != 0x00)
                    {
                        struct Octree* octree = geom->octree;
                        
                        log_indent = 0;
//                        info("Octree @ %X: %d faces, elements @ %X\n", octree->offset, octree->n_faces, octree->element_base_table_ptr);
//                        info("\tmin: %f %f %f\n", octree->min.x, octree->min.y, octree->min.z);
//                        info("\tmax: %f %f %f\n", octree->max.x, octree->max.y, octree->max.z);
//                        info("\tmiddle: %f %f %f\n\n\n", halfway.x, halfway.y, halfway.z);

                        
                        glDisable(GL_DEPTH_TEST);
                        
                        struct Vector4 color = vector4_new(0.0f, 10.0, 0.0f, 1.0f);
                        glUniform4fv(glGetUniformLocation(shader_main, "color"), 1, &color.x);
                        graphics_draw_line(octree->min, octree->max);
                        
                        struct OctreeNode* root = octree->root;
                        if (root)
                        {
                            struct Vector3 rayman = vector3_read(0x00BF0D98);
                            
                            struct Vector3 position = vector3_new(obj->matrix_default.m30, obj->matrix_default.m31, obj->matrix_default.m32);
                            
                            struct Vector3 local = vector3_sub(rayman, position);
                            
                            struct OctreeNode* intersect = NULL;
                            if ((intersect = octree_intersect_point(root, local)))
                            {
                                //info("Octree contains %d vertices\n", octree->n_faces);
//                                info("Rayman intersects %s @ %X with %d faces\n", ipo->name, intersect->offset, intersect->n_face_indices);
//                                info("\tThese faces were intersected:\n");
//                                info("\t\t");
//                                for (int i = 0; i < intersect->n_face_indices; i++)
//                                    info("%d, ", intersect->face_indices[i]);
//                                info("\n");
                            }
                        }
                        
                        
                        glEnable(GL_DEPTH_TEST);
                    }
                }
            }
        }
    }
}

static void gen_checkerboard_texture()
{
    glGenTextures(1, &checkerboard_texture);
    glBindTexture(GL_TEXTURE_2D, checkerboard_texture);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    
    uint8_t data[2 * 2] = {96, 128, 128, 96};
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 2, 2, 0, GL_RED, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void graphics_draw_triangle(struct Vector3 a, struct Vector3 b, struct Vector3 c)
{
    GLuint vao;
    GLuint vbo;
    
    const GLfloat vertices[] =
    {
        a.x, a.y, a.z,
        b.x, b.y, b.z,
        c.x, c.y, c.z,
    };
    
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof vertices, vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glDrawArrays(GL_TRIANGLES, 0, 3);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}

void graphics_draw_line(struct Vector3 start, struct Vector3 end)
{
    GLuint vao;
    GLuint vbo;
    
    const GLfloat vertices[] =
    {
        start.x, start.y, start.z,
        end.x, end.y, end.z,
    };
    
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof vertices, vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glDrawArrays(GL_LINES, 0, 2);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}

void graphics_draw_box(struct Vector3 position, struct Vector3 scale, struct Vector3 rotation, struct Vector4 color)
{
    struct Matrix4 T = matrix4_identity;
    
    /* Rotate */
    T = matrix4_mul(T, matrix4_make_rotation_x(radians(rotation.x)));
    T = matrix4_mul(T, matrix4_make_rotation_y(radians(rotation.y)));
    T = matrix4_mul(T, matrix4_make_rotation_z(radians(rotation.z)));
    
    /* Scale & translate. */
    T = matrix4_mul(T, matrix4_make_scale(scale.x, scale.y, scale.z));
    T = matrix4_mul(T, matrix4_make_translation(position.x, position.y, position.z));

    T = matrix4_transpose(T);
    
    glUniformMatrix4fv(glGetUniformLocation(shader_main, "model"), 1, GL_FALSE, &T.m00);
    glmesh_draw(box);
}

void graphics_draw_sphere(struct Vector3 center, const float r, struct Vector4 color)
{
    struct Matrix4 T;
    T = matrix4_make_scale(r * 2.0f, r * 2.0f, r * 2.0f);
    T = matrix4_mul(T, matrix4_make_translation(center.x, center.y, center.z));
    T = matrix4_transpose(T);
    
    glUniformMatrix4fv(glGetUniformLocation(shader_main, "model"), 1, GL_FALSE, &T.m00);
    glmesh_draw(sphere);
}

static void graphics_main_loop()
{
    SDL_Event e;
    SDL_PollEvent(&e);
    
    SDL_GetWindowSize(window, &width, &height);
    
    /* GCN default fov: 1.3rad */
    const float fov = memory.read_float(0x00C751B4);
    camera->zoom = fov == 0.0f ? degrees(1.30f) : degrees(fov);
    
    struct Matrix4 view = camera_view_matrix(camera);
    
    /* Camera parameters */
    if (!configuration.camera_unlocked)
    {
        camera->position = vector3_read(0x00c531bc);
        struct Vector3 eye = camera->position;
        struct Vector3 up = vector3_new(-0.0f, 0.0f, 1.0f);
        struct Vector3 look_at = vector3_read(0x00c53910);
        view = matrix4_lookat(camera->position, look_at, up);
    }
    
    struct Matrix4 projection = camera_projection_matrix(camera, (float)width / (float)height);
    
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
    
    /* Enable blending */
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, checkerboard_texture);
    
    /* Upload uniforms */
    glUniformMatrix4fv(glGetUniformLocation(shader_main, "view"), 1, GL_FALSE, &view.m00);
    glUniformMatrix4fv(glGetUniformLocation(shader_main, "projection"), 1, GL_FALSE, &projection.m00);
    glUniform3f(glGetUniformLocation(shader_main, "camera"), camera->position.x, camera->position.y, camera->position.z);
    glUniform1i(glGetUniformLocation(shader_main, "display_normals"), configuration.graphics_display_mode == 1);
    
    if (engine)
    {
        if (engine->root)
        {
            superobject_for_each(SUPEROBJECT_TYPE_IPO, engine->root, &draw_ipo, engine->root);
        }
    }
    
//    glUniform1i(glGetUniformLocation(shader_main, "checkerboard"), 0);
//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_2D, checkerboard_texture);
    struct Vector3 rayman_position = vector3_read(0x00BF0D98);
//    graphics_draw_box(rayman, vector3_new(1, 1, 1), vector3_new(45.0, 0.0, 0.0), vector4_new(0, 0, 0, 0));
//    //graphics_draw_sphere(rayman, 0.5f, vector4_new(0, 0, 0, 0));
//
//
//    glUniformMatrix4fv(glGetUniformLocation(shader_main, "model"), 1, GL_FALSE, &matrix4_identity.m00);
//    struct LineSegment segment;
//
//    segment.start = vector3_new(-1.16, -7, -15);
//    segment.end = vector3_new(-4, 7, -15);
//
//    graphics_draw_line(segment.start, segment.end);
//    struct Vector3 closest = closest_point_on_segment(rayman, segment);
//    closest = vector3_new(closest.x, closest.y, closest.z);
//    graphics_draw_box(closest, vector3_new(0.1, 0.1, 0.1), vector3_new(0, 0, 0), vector4_new(0, 0, 0, 0));
    
    
    if (rayman)
    {
        rayman->superobject->matrix_default = matrix4_read(rayman->superobject->matrix_default.offset);
    }
    
    
    SDL_GL_SwapWindow(window);
}

int graphics_shader_id()
{
    return shader_main;
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
    
    box = glmesh_box();
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
    
    gen_checkerboard_texture();
    
    dispatch_source_set_event_handler(sdl_timer_source, ^{
        graphics_main_loop();
    });

    dispatch_source_set_timer(sdl_timer_source, dispatch_time(DISPATCH_TIME_NOW, NSEC_PER_SEC), NSEC_PER_SEC * (1.0f / 60.0f), 0);
    dispatch_resume(sdl_timer_source);
}

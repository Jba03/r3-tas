//
//  graphics.c
//  r3lib
//
//  Created by Jba03 on 2022-11-22.
//

#include "game.h"
#include "graphics.h"
#include "camera.h"
#include "opengl.h"
#include "configuration.h"
#include "log.h"

#include "engine.h"
#include "dynamics.h"

#if !defined(__APPLE__)

static int width;
static int height;

static GLuint shader_main;
static GLuint checkerboard_texture;
static GLuint framebuffer;
static GLuint framebuffer_texture;
static GLuint depth_renderbuffer;

static struct camera *camera = NULL;
static struct glmesh *box = NULL;
static struct glmesh *sphere = NULL;

static const vector4 bg =
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
"uniform bool use_texture = false;\n" \
"uniform vec3 camera;\n"    \
"uniform vec3 lightPos;\n" \
"uniform bool display_normals = false;" \
"uniform vec4 color = vec4(1);\n" \
"void main() {\n"                                                   \
"   vec3 color2 = use_texture ? texture(checkerboard, texcoord * 4).rgb : vec3(1);\n" \
"   vec3 normal2 = normalize(normal);\n" \
"   vec3 lightColor = vec3(1,1,1)*0.75;\n" \
"   vec3 ambient = vec3(0.25);\n" \
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

static void gen_framebuffer()
{
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    
    glGenTextures(1, &framebuffer_texture);
    glBindTexture(GL_TEXTURE_2D, framebuffer_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 512, 512, 0, GL_RGBA32F, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    
    glGenRenderbuffers(1, &depth_renderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depth_renderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1024, 768);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_renderbuffer);
    
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, framebuffer_texture, 0);
    GLenum drawbuffers[] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, drawbuffers);
}

static void gen_checkerboard_texture()
{
    glGenTextures(1, &checkerboard_texture);
    glBindTexture(GL_TEXTURE_2D, checkerboard_texture);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    
    const uint32_t data[2 * 2] =
    {
        0xFFFFFFFF, 0xBBBBBBBB,
        0xBBBBBBBB, 0xFFFFFFFF,
    };
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void graphics_draw_triangle(const vector3 a, const vector3 b, const vector3 c)
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

void graphics_draw_line(const vector3 start, struct vector3 end)
{
    GLuint vao;
    GLuint vbo;
    
    const GLfloat vertices[] =
    {
        start.x, start.y, start.z,
        end.x, end.y, end.z,
    };
    
    glUniformMatrix4fv(glGetUniformLocation(shader_main, "model"), 1, GL_FALSE, &matrix4_identity.m00);
    
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

void graphics_draw_box(struct vector3 position, struct vector3 scale, struct vector3 rotation, struct vector4 color)
{
    matrix4 T = matrix4_identity;
    
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

void graphics_draw_sphere(struct vector3 center, const float r, struct vector4 color)
{
    matrix4 T;
    T = matrix4_make_scale(r * 2.0f, r * 2.0f, r * 2.0f);
    T = matrix4_mul(T, matrix4_make_translation(center.x, center.y, center.z));
    T = matrix4_transpose(T);
    
    glUniform4fv(glGetUniformLocation(shader_main, "color"), 1, &color.x);
    glUniformMatrix4fv(glGetUniformLocation(shader_main, "model"), 1, GL_FALSE, &T.m00);
    glmesh_draw(sphere);
}

static void graphics_main_loop()
{
    /* GCN default fov: 1.3rad */
    const float fov = *(float*)(memory.base + 0x00C751B4);
    camera->zoom = fov == 0.0f ? degrees(1.30f) : degrees(fov);
    
    matrix4 view = camera_view_matrix(camera);
    
    /* Camera parameters */
    if (!configuration.camera_unlocked)
    {
        camera->position = *(vector3*)(memory.base + 0x00c531bc);
        struct vector3 eye = camera->position;
        struct vector3 up = vector3_new(-0.0f, 0.0f, 1.0f);
        struct vector3 look_at = *(vector3*)(memory.base + 0x00c53910);
        view = matrix4_lookat(camera->position, look_at, up);
    }
    
    matrix4 projection = camera_projection_matrix(camera, (float)width / (float)height);
    
    /* Set program */
    glUseProgram(shader_main);
    
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
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
    glUniform3f(glGetUniformLocation(shader_main, "lightPos"), camera->position.x, camera->position.y + 100, camera->position.z);
    glUniform1i(glGetUniformLocation(shader_main, "display_normals"), configuration.graphics_display_mode == 1);
    
    /* Default framebuffer */
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    //struct vector3 rayman_position = *(vector3*)(memory.base + 0x00BF0D98);
}

int graphics_shader_id()
{
    return shader_main;
}

void graphics_init(void)
{
    //camera = camera_create(-90.0f, 0.0f, 0.1f, 0.25f, 45.0f);
    //box = glmesh_box();
    //sphere = glmesh_sphere(20, 20);
    
//    if ((shader_main = shader_create(VERTEX, FRAGMENT)) == 0)
//    {
//        warning("Could not create shader program\n");
//        exit(-1);
//    }
//    else
//    {
//        info(BOLD COLOR_GREEN "GLSL shaders compiled successfully\n");
//    }
//
//    gen_checkerboard_texture();
//    gen_framebuffer();
}

void glmesh_data(struct glmesh *mesh)
{
    /* Generate arrays and buffers */
    glGenVertexArrays(1, &mesh->vao);
    glGenBuffers(1, &mesh->vbo);
    glGenBuffers(1, &mesh->ebo);
    
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attributepointer(s).
    glBindVertexArray(mesh->vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
    glBufferData(GL_ARRAY_BUFFER, mesh->n_vertices * sizeof(struct glvertex), mesh->vertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->n_indices * sizeof(unsigned int), &mesh->indices[0], GL_STATIC_DRAW);
    
    /* Position */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(struct glvertex), (GLvoid*)offsetof(struct glvertex, position));
    glEnableVertexAttribArray(0);
    
    /* Normal */
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(struct glvertex), (GLvoid*)offsetof(struct glvertex, normal));
    glEnableVertexAttribArray(1);
    
    /* Texture coordinate */
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(struct glvertex), (GLvoid*)offsetof(struct glvertex, texcoord));
    glEnableVertexAttribArray(2);
    
    /* Unbind VAO */
    glBindVertexArray(0);
}

void glmesh_draw(struct glmesh *mesh)
{
    glBindVertexArray(mesh->vao);
    glDrawElements(GL_TRIANGLES, mesh->n_indices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void glmesh_destroy(struct glmesh *mesh)
{
    glDeleteVertexArrays(1, &mesh->vao);
    glDeleteBuffers(1, &mesh->vbo);
    glDeleteBuffers(1, &mesh->ebo);
    
    free(mesh->vertices);
    free(mesh->indices);
    free(mesh);
}


#endif

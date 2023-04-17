//
//  metal.m
//  r3-tas
//
//  Created by Jba03 on 2022-12-29.
//

#import <QuartzCore/QuartzCore.h>
#import <Metal/Metal.h>
#import <MetalKit/MTKView.h>
#import <Cocoa/Cocoa.h>

#include <dispatch/dispatch.h>
#include "AAPLMathUtilities.h"

extern "C"
{
#include "graphics.h"
#include "stCameraGLI.h"
#include "SDL.h"
#include "stDynamics.h"
#include "game.h"
#include "memory.h"
#include "stOctree.h"
#include "stSector.h"
}

#include "gui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_metal.h"

static dispatch_queue_t timer_queue;
static dispatch_source_t timer_source;

static id<MTLDevice> device = NULL;
static id<MTLCommandQueue> command_queue = NULL;
static id<MTLRenderPipelineState> pipeline_state = NULL;
static id<MTLDepthStencilState> depth_state = NULL;
static id<MTLTexture> depth_texture = NULL;
static id<MTLTexture> checker_texture = NULL;
static id<MTLTexture> render_texture = NULL;

static MTLVertexDescriptor* vertex_descriptor = NULL;
static MTLRenderPassDescriptor* renderpass_descriptor = NULL;

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static MTKView* metal_view = NULL;
static CAMetalLayer* layer = NULL;

static ImGuiContext* imctx;

extern bool main_render;

static id<MTLLibrary> shader_library()
{
    printf("path: %s\n", LIBR3TAS_DIR);
    NSString* path = [NSString stringWithFormat: @"%s%@", LIBR3TAS_DIR, @"/graphics/metal/main.metal"];
    printf("path: %s", path.UTF8String);
    NSError *error = NULL; 
    
    NSString *source = [NSString stringWithContentsOfFile: path encoding: NSUTF8StringEncoding error: &error];
    if (error)
    {
        fprintf(stderr, "error: %s\n", error.localizedDescription.UTF8String);
        return NULL;
    }
    
    id<MTLLibrary> lib = [device newLibraryWithSource: source options: NULL error: &error];
    if (error)
    {
        fprintf(stderr, "error: %s\n", error.localizedDescription.UTF8String);
        return NULL;
    }
    
    return lib;
}

static id<MTLTexture> create_depth_texture()
{
    MTLTextureDescriptor* desc = [[MTLTextureDescriptor alloc] init];
    desc.width = 800 * 2;
    desc.height = 600 * 2;
    desc.pixelFormat = MTLPixelFormatDepth32Float_Stencil8;
    desc.mipmapLevelCount = 1;
    desc.storageMode = MTLStorageModePrivate;
    desc.usage = MTLTextureUsageRenderTarget;
    
    id<MTLTexture> depthTexture = [device newTextureWithDescriptor: desc];
    assert(depthTexture);
    
    return depthTexture;
}

static id<MTLTexture> create_checkerboard_texture()
{
    MTLTextureDescriptor* desc = [[MTLTextureDescriptor alloc] init];
    desc.width = 2;
    desc.height = 2;
    desc.depth = 1;
    desc.textureType = MTLTextureType2D;
    desc.pixelFormat = MTLPixelFormatR32Float;
    desc.mipmapLevelCount = 1;
    desc.storageMode = MTLStorageModeShared;
    desc.usage = MTLTextureUsageShaderRead;
    
    id<MTLTexture> depthTexture = [device newTextureWithDescriptor: desc];
    assert(depthTexture);
    
    MTLRegion region = MTLRegionMake2D(0, 0, 2, 2);
    const float data[2 * 2] = { 1.0f, 0.5f, 0.5f, 1.0f };
    
    [depthTexture replaceRegion: region mipmapLevel: 0 withBytes: data bytesPerRow: 4 * 2];
    
    return depthTexture;
}

static id<MTLTexture> create_render_texture()
{
    MTLTextureDescriptor* desc = [[MTLTextureDescriptor alloc] init];
    desc.width = 800;
    desc.height = 600;
    desc.depth = 1;
    desc.textureType = MTLTextureType2D;
    desc.pixelFormat = MTLPixelFormatRGBA8Unorm;
    desc.mipmapLevelCount = 1;
    desc.storageMode = MTLStorageModeShared;
    desc.usage = MTLTextureUsageRenderTarget;
    
    id<MTLTexture> renderTexture = [device newTextureWithDescriptor: desc];
    assert(renderTexture);
    
    return renderTexture;
}

static struct mesh* sphere_mesh;
static struct mesh* box_mesh;

static int metal_setup()
{
    id<MTLLibrary> library = shader_library();
    if (!library) return -1;
    
    id<MTLFunction> vertexfn = [library newFunctionWithName: @"vertex_main"];
    id<MTLFunction> fragmentfn = [library newFunctionWithName: @"fragment_main"];
    
//    if (!vertexfn)
//    {
//        fprintf(stderr, "error: %s\n", error.localizedDescription.UTF8String);
//        return NULL;
//    }
    
    MTLDepthStencilDescriptor *depthStateDesc = [MTLDepthStencilDescriptor new];
    depthStateDesc.depthCompareFunction = MTLCompareFunctionLess;
    depthStateDesc.depthWriteEnabled = YES;
    depth_state = [device newDepthStencilStateWithDescriptor:depthStateDesc];
    
    
    vertex_descriptor = [[MTLVertexDescriptor alloc] init];

    // Positions.
    vertex_descriptor.attributes[0].format = MTLVertexFormatFloat4;
    vertex_descriptor.attributes[0].offset = offsetof(struct vertex, position);
    vertex_descriptor.attributes[0].bufferIndex = 0;

    // Normals.
    vertex_descriptor.attributes[1].format = MTLVertexFormatFloat4;
    vertex_descriptor.attributes[1].offset = offsetof(struct vertex, normal);
    vertex_descriptor.attributes[1].bufferIndex = 0;

    // Texture coordinates.
    vertex_descriptor.attributes[2].format = MTLVertexFormatFloat4;
    vertex_descriptor.attributes[2].offset = offsetof(struct vertex, texcoord);
    vertex_descriptor.attributes[2].bufferIndex = 0;
    
    vertex_descriptor.layouts[0].stride = sizeof(tdstVector3D);
    vertex_descriptor.layouts[0].stepRate = 1;
    vertex_descriptor.layouts[0].stepFunction = MTLVertexStepFunctionPerVertex;
    
    
    MTLRenderPipelineDescriptor *pipelineStateDescriptor = [MTLRenderPipelineDescriptor new];
    pipelineStateDescriptor.label = @"Simple Pipeline";
    pipelineStateDescriptor.vertexFunction = vertexfn;
    pipelineStateDescriptor.fragmentFunction = fragmentfn;
    
    pipelineStateDescriptor.colorAttachments[0].pixelFormat = MTLPixelFormatRGBA8Unorm;
    pipelineStateDescriptor.colorAttachments[0].blendingEnabled = YES;
    pipelineStateDescriptor.colorAttachments[0].rgbBlendOperation = MTLBlendOperationAdd;
    pipelineStateDescriptor.colorAttachments[0].alphaBlendOperation = MTLBlendOperationAdd;
    pipelineStateDescriptor.colorAttachments[0].sourceRGBBlendFactor = MTLBlendFactorSourceAlpha;
    pipelineStateDescriptor.colorAttachments[0].sourceAlphaBlendFactor = MTLBlendFactorSourceAlpha;
    pipelineStateDescriptor.colorAttachments[0].destinationRGBBlendFactor = MTLBlendFactorOneMinusSourceAlpha;
    pipelineStateDescriptor.colorAttachments[0].destinationAlphaBlendFactor = MTLBlendFactorOneMinusSourceAlpha;
    
    pipelineStateDescriptor.vertexDescriptor = vertex_descriptor;
    pipelineStateDescriptor.depthAttachmentPixelFormat = MTLPixelFormatDepth32Float_Stencil8;
    pipelineStateDescriptor.stencilAttachmentPixelFormat = MTLPixelFormatDepth32Float_Stencil8;

    
    depth_texture = create_depth_texture();
    checker_texture = create_checkerboard_texture();
    render_texture = create_render_texture();
    
    //printf("render texture: %p\n", render_texture);
    
    
    //printf("dvaa\n");
    NSError* error = NULL;
    pipeline_state = [device newRenderPipelineStateWithDescriptor: pipelineStateDescriptor error: &error];
    if (!pipeline_state)
    {
        fprintf(stderr, "error: %s\n", error.localizedDescription.UTF8String);
        return NULL;
    }
    
    sphere_mesh = mesh_sphere(20.0f, 20.0f);
    box_mesh = mesh_box();
    
    return 0;
}

struct mesh_data
{
    id<MTLBuffer> vertex_buffer;
    id<MTLBuffer> index_buffer;
};

void mesh_process(struct mesh *mesh)
{
    if (!mesh) return;
    
    id<MTLBuffer> v = [device newBufferWithBytes: mesh->vertices length: mesh->n_vertices * sizeof(struct vertex) options: MTLResourceStorageModeShared];
    id<MTLBuffer> i = [device newBufferWithBytes: mesh->indices length: mesh->n_indices * sizeof(unsigned) options: MTLResourceStorageModeShared];
    
    struct mesh_data* data = (struct mesh_data*)malloc(sizeof *data);
    data->vertex_buffer = v;
    data->index_buffer = i;
    
    mesh->internal_data = (void*)data;
}


#include "game.h"
#include "structure.h"
#include "stSuperObject.h"
#include "stInstantiatedPhysicalObject.h"
#include "stPhysicalObject.h"
#include "stCollideSet.h"
#include "stCollideObject.h"
#include "stCollideElementIndexedTriangles.h"
#include "geometry.h"

struct Uniform
{
    simd_float3 camera_pos;
    simd_float4x4 view;
    simd_float4x4 projection;
    simd_float4x4 model;
    simd_float3x3 normal_matrix;
    simd_float4 color;
    bool use_texture;
} uniform;

extern tdstCollideObject* collide_objects[1000];
extern tdstMatrix4D matrices[1000];
extern struct mesh* meshlist[1000];
extern int current_mesh;
extern int current_matrix;
extern int current_collide_object;
extern tdstEngineObject* current_actor;

void copyDepthStencilConfigurationFrom(MTLRenderPassDescriptor *src, MTLRenderPassDescriptor *dest)
{
    dest.depthAttachment.loadAction     = src.depthAttachment.loadAction;
    dest.depthAttachment.clearDepth     = src.depthAttachment.clearDepth;
    dest.depthAttachment.texture        = src.depthAttachment.texture;
    dest.stencilAttachment.loadAction   = src.stencilAttachment.loadAction;
    dest.stencilAttachment.clearStencil = src.stencilAttachment.clearStencil;
    dest.stencilAttachment.texture      = src.stencilAttachment.texture;
}

void* graphics_get_texture()
{
    id<CAMetalDrawable> drawable = [layer nextDrawable];
    return (__bridge void*)drawable.texture;
}

int n_triangles = 0;
struct triangle* triangles;

tdstVector3D sphere_pos;

static void draw_sphere(tdstVector3D center, const float radius, tdstVector4D color, const id<MTLRenderCommandEncoder> renderEncoder)
{
    uniform.color = simd_make_float4(color.x, color.y, color.z, color.w);
    uniform.model = matrix4x4_translation(center.x, center.z, center.y);
    uniform.model = simd_mul(uniform.model, matrix4x4_scale(radius * 2.0f, radius * 2.0f, radius * 2.0f));
    
    id<MTLBuffer> uniformbuffer = [device newBufferWithBytes: &uniform length: sizeof uniform options: MTLResourceStorageModeShared];
        
    struct mesh_data* data = (struct mesh_data*)sphere_mesh->internal_data;
    [renderEncoder setVertexBuffer: data->vertex_buffer offset: 0 atIndex: 0];
    [renderEncoder setVertexBuffer: uniformbuffer offset: 0 atIndex: 1];
    [renderEncoder setFragmentTexture: checker_texture atIndex: 0];
    [renderEncoder setFragmentBuffer: uniformbuffer offset: 0 atIndex: 0];
    
    [renderEncoder drawIndexedPrimitives: MTLPrimitiveTypeTriangle
                              indexCount: sphere_mesh->n_indices
                               indexType: MTLIndexTypeUInt32
                             indexBuffer: data->index_buffer
                       indexBufferOffset: 0];
}

static void draw_box(tdstVector3D center, tdstVector3D size, tdstVector4D color, const id<MTLRenderCommandEncoder> renderEncoder)
{
    uniform.color = simd_make_float4(color.x, color.y, color.z, color.w);
    uniform.model = matrix4x4_translation(center.x, center.z, center.y);
    uniform.model = simd_mul(uniform.model, matrix4x4_scale(size.x, size.y, size.z));
    uniform.use_texture = false;
    
    id<MTLBuffer> uniformbuffer = [device newBufferWithBytes: &uniform length: sizeof uniform options: MTLResourceStorageModeShared];
        
    struct mesh_data* data = (struct mesh_data*)box_mesh->internal_data;
    [renderEncoder setVertexBuffer: data->vertex_buffer offset: 0 atIndex: 0];
    [renderEncoder setVertexBuffer: uniformbuffer offset: 0 atIndex: 1];
    [renderEncoder setFragmentTexture: checker_texture atIndex: 0];
    [renderEncoder setFragmentBuffer: uniformbuffer offset: 0 atIndex: 0];
    
    [renderEncoder drawIndexedPrimitives: MTLPrimitiveTypeTriangle
                              indexCount: box_mesh->n_indices
                               indexType: MTLIndexTypeUInt32
                             indexBuffer: data->index_buffer
                       indexBufferOffset: 0];
}

/*
See LICENSE folder for this sample’s licensing information.

Abstract:
Implementation of vector, matrix, and quaternion math utility functions useful for 3D graphics
 rendering with Metal

 Metal uses column-major matrices and column-vector inputs.

    linearIndex     cr              example with reference elements
     0  4  8 12     00 10 20 30     sx  10  20   tx
     1  5  9 13 --> 01 11 21 31 --> 01  sy  21   ty
     2  6 10 14     02 12 22 32     02  12  sz   tz
     3  7 11 15     03 13 23 33     03  13  1/d  33

  The "cr" names are for <column><row>
*/
static simd_float4x4 GameToMetalMatrix(const tdstMatrix4D mat)
{
    simd_float4x4 m;
    m.columns[0] = simd_make_float4(mat.m00, mat.m01, mat.m02, mat.m03);
    m.columns[1] = simd_make_float4(mat.m10, mat.m11, mat.m12, mat.m13);
    m.columns[2] = simd_make_float4(mat.m20, mat.m21, mat.m22, mat.m23);
    m.columns[3] = simd_make_float4(mat.m30, mat.m31, mat.m32, mat.m33);
    
    return m;
}


static void RenderOctreeNode(const tdstMatrix4D transform, const tdstOctreeNode* nd, tdstVector4D color, id<MTLRenderCommandEncoder> render_encoder)
{
    if (!nd) return;
    
    tdstVector3D min = vector3_host_byteorder(nd->min);
    tdstVector3D max = vector3_host_byteorder(nd->max);
    
    const float x = min.x + (max.x - min.x) / 2.0f;
    const float y = min.y + (max.y - min.y) / 2.0f;
    const float z = min.z + (max.z - min.z) / 2.0f;
    
    tdstVector4D c = vector4_new(x, y, z, 1.0f);
    c = vector4_mul_matrix4(c, transform);
    
    tdstVector3D size = vector3_new(max.x - min.x, max.z - min.z, max.y - min.y);
    draw_box(vector3_new(c.x, c.y, c.z), size, color, render_encoder);
    //vector4_new(1.0f, 0.0f, 0.4f, 0.5f)
    
    const pointer* childlist = (const pointer*)pointer(nd->children);
    if (childlist)
    {
        for (int i = 0; i < 8; i++)
        {
            const tdstOctreeNode* node = (const tdstOctreeNode*)pointer(*(childlist + i));
            RenderOctreeNode(transform, node, color, render_encoder);
        }
    }
}

static void RenderOctrees(tdstSuperObject* so, tdstVector4D color, id<MTLRenderCommandEncoder> render_encoder)
{
    if (superobject_type(so) == superobject_type_ipo)
    {
        const tdstInstantiatedPhysicalObject* ipo = (const tdstInstantiatedPhysicalObject*)superobject_data(so);
        if (!ipo) return;
        
        const tdstCollideObject* zdr = fnIPOGetCollideObject(ipo);
        if (!zdr) return;
        
        const tdstOctree* octree = (const tdstOctree*)pointer(zdr->octree);
        if (!octree) return;
        
        const tdstOctreeNode* root = (const tdstOctreeNode*)pointer(octree->root);
        if (!root) return;
        
        
//            int n_selected = 0;
//            tdstOctreeNode* selected[OCTREE_MAX_SELECTED_NODES];
//            memset(selected, 0, sizeof(octree_node*) * OCTREE_MAX_SELECTED_NODES);
//            float st[OCTREE_MAX_SELECTED_NODES];
//
//            const tdstMatrix4D raymat = fnActorGetMatrix(actor_rayman);
//            const tdstVector3D raypos = game_matrix4_position(raymat);
//            octree_traverse_line_segment(root, fnSuperobjectGetGlobalMatrix(child), raypos, vector3_sub(vector3_new(0.0f, 0.0f, 0.0f), raypos), selected, &n_selected, st);
//
//            for (int i = 0; i < n_selected; i++)
//                RenderOctreeNode(fnSuperobjectGetGlobalMatrix(child), selected[i], vector4_new(255.0f, 0.0f, 1.0f, 1.0f), render_encoder);
        
        RenderOctreeNode(fnSuperobjectGetGlobalMatrix(so), root, color, render_encoder);
    }
    
    superobject_for_each(so, child)
    {
        RenderOctrees(child, color, render_encoder);
    };
}

static void DrawGeometryRecursive(const tdstSuperObject* root, const tdstMatrix4D transform, id<MTLRenderCommandEncoder> render_encoder)
{
    if (!root) return;
    
    /* Calculate the new transformation matrix */
    const tdstMatrix4D T = matrix4_mul(fnSuperobjectGetGlobalMatrix(root), transform);
    
    if (superobject_type(root) == superobject_type_ipo)
    {
        const tdstInstantiatedPhysicalObject* ipo = (const tdstInstantiatedPhysicalObject*)superobject_data(root);
        if (ipo)
        {
            const tdstCollideObject* zdr = fnIPOGetCollideObject(ipo);
            if (zdr)
            {
                int mesh_idx = 0;
                const tdstCollideElementIndexedTriangles* mesh;
                while ((mesh = fnCollideObjectGetElementIndexedTriangles(zdr, mesh_idx)))
                {
                    uint16* indices = (uint16*)pointer(mesh->faceIndices);
                    tdstVector3D* vertices = (tdstVector3D*)pointer(zdr->vertices);
                    
                    if (vertices && indices)
                    {
                        
                        const int16 n_faces = host_byteorder_16(mesh->numFaces);
                        const int16 n_vertices = host_byteorder_16(zdr->numVertices);
                        //printf("n faces: %d %d\n", n_faces, n_vertices);
                        
                        id<MTLBuffer> indexBuffer = [device newBufferWithBytes: (void*)indices length: n_faces * 3 * sizeof(uint16) options: MTLResourceStorageModeShared];
                        id<MTLBuffer> vertexBuffer = [device newBufferWithBytes: (void*)vertices length: n_vertices * sizeof(tdstVector3D) options: MTLResourceStorageModeShared];
                        
                        uniform.model = GameToMetalMatrix(T);
                        
                        id<MTLBuffer> uniformbuffer = [device newBufferWithBytes: &uniform length: sizeof uniform options: MTLResourceStorageModeShared];
                        [render_encoder setVertexBuffer: vertexBuffer offset: 0 atIndex: 0];
                        [render_encoder setVertexBuffer: uniformbuffer offset: 0 atIndex: 1];
                        [render_encoder setFragmentTexture: checker_texture atIndex: 0];
                        [render_encoder setFragmentBuffer: uniformbuffer offset: 0 atIndex: 0];
                        [render_encoder drawIndexedPrimitives: MTLPrimitiveTypeTriangle
                                                   indexCount: n_faces
                                                    indexType: MTLIndexTypeUInt16
                                                  indexBuffer: indexBuffer
                                            indexBufferOffset: 0];
                    }
                    
                    mesh_idx++;
                }
            }
        }
    }
    
    /* Draw children */
    superobject_for_each(root, child) DrawGeometryRecursive(child, T, render_encoder);
}


void graphics_loop()
{
    //return;
//    if (main_render)
//    {
//        SDL_HideWindow(window);
//        return;
//    }
//    else
//    {
//        SDL_ShowWindow(window);
//    }
    
    //return;
    
    SDL_ShowWindow(window);

    //camera->position = tdstVector3D_new(0, 0, 0);
    
//    printf("position: (%f, %f, %f)\n", camera->position.x, camera->position.z, camera->position.y);
//    printf("lookat: (%f, %f, %f)\n", lookat.x, lookat.z, lookat.y);
    
    //return;
    
    //dispatch_sync(dispatch_get_main_queue(), ^{
        
        SDL_PumpEvents();
        
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            //printf("event!\n");
            //ImGui_ImplSDL2_ProcessEvent(&event);
            
            if (event.type == SDL_MOUSEMOTION)
            {
            }
            
            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                if (event.button.button == SDL_BUTTON_LEFT) { ImGui::GetIO().MouseDown[ImGuiMouseButton_Left] = true; }
                if (event.button.button == SDL_BUTTON_RIGHT) ImGui::GetIO().MouseDown[ImGuiMouseButton_Right] = true;
            }
            
            if (event.type == SDL_MOUSEBUTTONUP)
            {
                if (event.button.button == SDL_BUTTON_LEFT) ImGui::GetIO().MouseDown[ImGuiMouseButton_Left] = false;
                if (event.button.button == SDL_BUTTON_RIGHT) ImGui::GetIO().MouseDown[ImGuiMouseButton_Right] = false;
            }
            
            if (event.type == SDL_QUIT) exit(0);
            //        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
            //            done = true;
        }
        
        int width, height;
        SDL_GetRendererOutputSize(renderer, &width, &height);
        layer.drawableSize = CGSizeMake(width, height);
        id<CAMetalDrawable> drawable = [layer nextDrawable];
        
        id<MTLCommandBuffer> commandBuffer = [command_queue commandBuffer];
        renderpass_descriptor.colorAttachments[0].clearColor = MTLClearColorMake(0.0f, 0.0f, 0.0f, 1.0f);
        renderpass_descriptor.colorAttachments[0].texture = drawable.texture;
        renderpass_descriptor.colorAttachments[0].loadAction = MTLLoadActionClear;
        renderpass_descriptor.colorAttachments[0].storeAction = MTLStoreActionStore;
    
//    renderpass_descriptor.depthAttachment.texture = depth_texture;
//    renderpass_descriptor.depthAttachment.clearDepth = 1.0f;
//    renderpass_descriptor.depthAttachment.loadAction = MTLLoadActionClear;
//    renderpass_descriptor.depthAttachment.storeAction = MTLStoreActionStore;
//
//    renderpass_descriptor.stencilAttachment.texture = depth_texture;
//    renderpass_descriptor.stencilAttachment.loadAction = MTLLoadActionClear;
//    renderpass_descriptor.stencilAttachment.storeAction = MTLStoreActionStore;
    
    id <MTLRenderCommandEncoder> renderEncoder = [commandBuffer renderCommandEncoderWithDescriptor: renderpass_descriptor];
    [renderEncoder pushDebugGroup: @"ImGui demo"];
    [renderEncoder setCullMode: MTLCullModeNone];
    [renderEncoder setFrontFacingWinding: MTLWindingClockwise];
    [renderEncoder setRenderPipelineState: pipeline_state];
    [renderEncoder setDepthStencilState: depth_state];
    [renderEncoder setViewport: (MTLViewport){0, 0, static_cast<double>(width), static_cast<double>(height), 0.0f, 1.0f /* 1.0f: important! */}];
    
    tdstVector3D campos = *(tdstVector3D*)(memoryBase + 0x00c531bc);
    tdstVector3D lookato = *(tdstVector3D*)(memoryBase + 0x00c53910);
    float fov = host_byteorder_f32(*(float32*)(memoryBase + 0x00C751B4));
    fov = fov == 0.0f ? 1.3 : fov;
    
    simd_float3 position;
    position.x = host_byteorder_f32(*(uint32_t*)&campos.x);
    position.z = host_byteorder_f32(*(uint32_t*)&campos.y);
    position.y = host_byteorder_f32(*(uint32_t*)&campos.z);
    
    simd_float3 lookat;
    lookat.x = host_byteorder_f32(*(uint32_t*)&lookato.x);
    lookat.z = host_byteorder_f32(*(uint32_t*)&lookato.y);
    lookat.y = host_byteorder_f32(*(uint32_t*)&lookato.z);
    
    /* Construct camera matrices */
    simd_float4x4 view = matrix_look_at_left_hand(position, lookat, (simd_float3){0, 1, 0});
    simd_float4x4 projection = matrix_perspective_left_hand(fov, (float)width / (float)height, 0.1f, 1000.0f);
    
    uniform.view = view;
    uniform.projection = projection;
    uniform.model = matrix_identity_float4x4;
    uniform.camera_pos = position;
    uniform.color = simd_make_float4(1.0f, 1.0f, 1.0f, 1.0f);
    uniform.use_texture = true;
    
    for (int i = 0; i < current_mesh; i++)
    {
        struct mesh* mesh = meshlist[i];

        const tdstCollideMaterial* material = (const tdstCollideMaterial*)pointer(mesh->material);
        if (material)
        {
            //printf("material: %X\n", offset(material));
            const uint16 identifier = host_byteorder_16(material->identifier);
            if (identifier & collide_material_grabbable_ledge) uniform.color = simd_make_float4(0.1f, 1.0f, 0.1f, 1.0f);
            if (identifier & collide_material_trampoline) uniform.color = simd_make_float4(0.0f, 0.0f, 1.0f, 1.0f);
            if (identifier & collide_material_water) uniform.color = simd_make_float4(0.0f, 1.0f, 1.0f, 0.5f);
            if (identifier & collide_material_unknown) uniform.color = simd_make_float4(1.0f, 1.0f, 1.0f, 1.0f);
        }

        struct mesh_data* data = (struct mesh_data*)mesh->internal_data;
        if (!data) continue;

        /* Get transform */
        const tdstMatrix4D mat = fnSuperobjectGetGlobalMatrix(mesh->superobject);
        uniform.model = GameToMetalMatrix(mat);
        uniform.normal_matrix = matrix3x3_upper_left(uniform.model);

        id<MTLBuffer> uniformbuffer = [device newBufferWithBytes: &uniform length: sizeof uniform options: MTLResourceStorageModeShared];
        [renderEncoder setVertexBuffer: data->vertex_buffer offset: 0 atIndex: 0];
        [renderEncoder setVertexBuffer: uniformbuffer offset: 0 atIndex: 1];
        [renderEncoder setFragmentTexture: checker_texture atIndex: 0];
        [renderEncoder setFragmentBuffer: uniformbuffer offset: 0 atIndex: 0];
        [renderEncoder drawIndexedPrimitives: MTLPrimitiveTypeTriangle
                                  indexCount: mesh->n_indices
                                   indexType: MTLIndexTypeUInt32
                                 indexBuffer: data->index_buffer
                            indexBufferOffset: 0];
    }
    
    
    //DrawGeometryRecursive(father_sector, matrix4_identity, renderEncoder);
    
    uniform.use_texture = false;
    
//    /* Octree rendering */
//    [renderEncoder setTriangleFillMode: MTLTriangleFillModeLines];
//    if (father_sector)
//    {
//        superobject_for_each(father_sector, subsector)
//        {
//            tdstVector4D color = vector4_new(1.0f, 0.5f, 0.1f, 0.25f);
//            extern tdstSuperObject* viewed_sector;
//            if (subsector == viewed_sector) color = vector4_new(0.0f, 1.0f, 0.75f, 0.25f);
//            RenderOctrees(subsector, color, renderEncoder);
//        };
//    }
//    [renderEncoder setTriangleFillMode: MTLTriangleFillModeFill];
    
    
//    extern struct xray xray;
//    for (int i = 0; i < xray.n_points; i++)
//    {
//        const tdstVector3D point = vector3_host_byteorder(*xray.pointset[i]);
//        const tdstVector4D vertexT = vector4_mul_matrix4(vector4_new(point.x, point.y, point.z, 1.0f), T);
//
//        draw_sphere(point, 0.1f, vector4_new(1.0f, 0.0f, 0.0f, 1.0f), renderEncoder);
//
//
//    }
    
    if (false)
    {
        superobject_for_each_type(superobject_type_actor, superobject_first_child(hierarchy), object)
        {
            const tdstEngineObject* actor = (const tdstEngineObject*)superobject_data(object);
            if (!actor) continue;
            
            /* The camera blocks the view; skip it. */
            if (offset(object) == 0xC533E0) continue;
            
            struct mesh_data* data = (struct mesh_data*)sphere_mesh->internal_data;
            
            const tdstTransform* T = (const tdstTransform*)pointer(object->globalTransform);
            const tdstMatrix4D mat = ( matrix4_host_byteorder(T->matrix));
            tdstVector3D pos = game_matrix4_position(mat);
            
            //printf("matrix: %X\n", offset(&T->matrix));
            
//            const tdstDynam* dynam = (const tdstDynam*)actor_dynam(actor);
//            if (dynam)
//            {
//                const tdstDynamics* dynamics = (const tdstDynamics*)pointer(dynam->dynamics);
//                if (dynamics)
//                {
//                    pos = vector3_add(pos, vector3_host_byteorder(dynamics->advanced.wall_normal));
//                }
//            }
            
            
            
            //printf("pos: %f %f %f\n", pos.x, pos.y, pos.z);
            
            uniform.color = simd_make_float4(1.0f, 0.0f, 0.0f, 0.5f);
            if (actor == current_actor) uniform.color = simd_make_float4(1.0f, 1.0f, 0.0f, 1.0f);
            
            uniform.model = matrix4x4_translation(pos.x, pos.z, pos.y);
            
            id<MTLBuffer> uniformbuffer = [device newBufferWithBytes: &uniform length: sizeof uniform options: MTLResourceStorageModeShared];
            
            
            [renderEncoder setVertexBuffer: data->vertex_buffer offset: 0 atIndex: 0];
            [renderEncoder setVertexBuffer: uniformbuffer offset: 0 atIndex: 1];
            [renderEncoder setFragmentTexture: checker_texture atIndex: 0];
            [renderEncoder setFragmentBuffer: uniformbuffer offset: 0 atIndex: 0];
            
            [renderEncoder drawIndexedPrimitives: MTLPrimitiveTypeTriangle
                                      indexCount: sphere_mesh->n_indices
                                       indexType: MTLIndexTypeUInt32
                                     indexBuffer: data->index_buffer
                               indexBufferOffset: 0];
        }
    }
    
    draw_sphere(sphere_pos, 1.0f, vector4_new(1.0f, 0.0f, 1.0f, 1.0f), renderEncoder);
    
//    for (int i = 0; i < n_triangles; i++) /* draw ipo vertex calc */
//    {
//        struct triangle t = triangles[i];
//        //printf("t: %f %f %f\n", t.a.x, t.a.y, t.a.z);
//
//
//
////        draw_sphere(t.a, 0.5f, vector4_new(1.0f, 0.0f, 1.0f, 1.0f), renderEncoder);
////        draw_sphere(t.b, 0.5f, vector4_new(1.0f, 0.0f, 1.0f, 1.0f), renderEncoder);
////        draw_sphere(t.c, 0.5f, vector4_new(1.0f, 0.0f, 1.0f, 1.0f), renderEncoder);
//    }
    
    /* Draw ZDx */
    if (false)
    {
        superobject_for_each_type(superobject_type_actor, superobject_first_child(hierarchy), object)
        {
            const tdstEngineObject* actor = (const tdstEngineObject*)superobject_data(object);
            if (!actor) continue;

            const tdstCollideSet* collset = (const tdstCollideSet*)actor_collset(actor);
            if (!collset) continue;
            
            const tdstZdxList* zddlist = (const tdstZdxList*)pointer(collset->zdeList);
            //printf("zdd list: %X\n", offset(&zddlist->list));
            if (zddlist)
            {
                for (unsigned int i = 0; i < host_byteorder_32(zddlist->list.numEntries); i++)
                {
                    pointer* zonesetptr = (pointer*)pointer(zddlist->list.first);
                    if (zonesetptr)
                    {
                        const tdstCollideObject* obj = (const tdstCollideObject*)pointer(*zonesetptr);
                        if (obj)
                        {
                            //printf("zdd list: %X\n", offset(obj));
                            
                            printf("sphere center: %f %f %f\n",
                                    host_byteorder_f32(obj->boundingSpherePosition.x),
                                   host_byteorder_f32(obj->boundingSpherePosition.y),
                                   host_byteorder_f32(obj->boundingSpherePosition.z));
                        }
                    }
                    
//                    const pointer* zddoffset = ((pointer*)pointer(zddlist->first_element)) + i;
//                    const tdstCollideObject* zdd = (const tdstCollideObject*)zddoffset;
//                    if (zdd)
//                    {
//                        printf("list: %X, %d\n", offset(zdd), host_byteorder_16(zdd->n_elements));
//                    }

                    //tdstCollideObject* zdd = (tdstCollideObject*)(*list);
//                    if (zdd)
//                    {
//                        for (unsigned int e = 0; e < host_byteorder_16(zdd->n_elements); e++)
//                        {
//                            const pointer element = (*((pointer*)pointer(zdd->elements) + i));
//                            const int16_t type = host_byteorder_16(*((int16_t*)pointer(zdd->element_types) + i));
//                            const void* block = pointer(element);
//
//                            printf("element %d: %d\n", e, type);
//
//                            //printf("draw! %X\n", offset(block));
//
//                            //printf("type: %X\n", offset(block));
//
//
//                            //                        switch (type)
//                            //                        {
//                            //                            case collide_object_indexed_triangles:
//                            //                            {
//                            //
//                            //                    printf("sphere center: %f %f %f\n",
//                            //                           host_byteorder_f32(zdd->bounding_sphere_position.x), host_byteorder_f32(zdd->bounding_sphere_position.y), host_byteorder_f32(zdd->bounding_sphere_position.z));
//                        }
//                    }
                }
            }
            
            
        };

        //printf("\n");
    }
    
    
//    struct mesh_data* data = (struct mesh_data*)mesh->internal_data;
//
//    [renderEncoder setVertexBuffer: data->vertex_buffer offset: 0 atIndex: 0];
//
//    [renderEncoder setVertexBuffer: uniformBuffer offset: 0 atIndex: 1];
//
//    //[renderEncoder drawPrimitives: MTLPrimitiveTypeTriangle vertexStart: 0 vertexCount: mesh->n_vertices];
//
//    [renderEncoder drawIndexedPrimitives: MTLPrimitiveTypeTriangle
//                              indexCount: mesh->n_indices
//                               indexType: MTLIndexTypeUInt32
//                             indexBuffer: data->index_buffer
//                       indexBufferOffset: 0];
    
    
        
//        // Start the Dear ImGui frame
//        ///ImGuiContext* prev = ImGui::GetCurrentContext();
//        ImGui::SetCurrentContext(imctx);
//
//        ImGui_ImplMetal_NewFrame(renderpass_descriptor);
//
//
//
//        ImGui::GetIO().DisplaySize = ImVec2(width, height);
//
//
//        ImGui::NewFrame();
//
//        static bool show = true;
//        static float f = 0.0f;
//        static int counter = 0;
//
//    superobject_info(superobject_first_child(hierarchy));
//
//        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
//
//        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
//        ImGui::Checkbox("Demo Window", &show);      // Edit bools storing our window open/close state
//        ImGui::Checkbox("Another Window", &show);
//
//        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
//        //ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
//
//        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
//            counter++;
//        ImGui::SameLine();
//        ImGui::Text("counter = %d", counter);
//
//        if (ImGui::Button("End debugging session"))
//            main_render = true;
//
//        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
//        ImGui::End();
//
//        ImGui::EndFrame();
//
//        ImGui::Render();
//        ImGui_ImplMetal_RenderDrawData(ImGui::GetDrawData(), commandBuffer, renderEncoder);
    
    
        
        [renderEncoder popDebugGroup];
        [renderEncoder endEncoding];
        
        [commandBuffer presentDrawable:drawable];
        [commandBuffer commit];
        
    //});
    
    //ImGui::SetCurrentContext(prev);
}


void graphics_init(void)
{
    
    SDL_Init(SDL_INIT_EVERYTHING);
    
    if (!(window = SDL_CreateWindow("Breakpoint debug", 0, 0, 800, 600, SDL_WINDOW_METAL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_HIDDEN)))
    {
        fprintf(stderr, "failed to create SDL window\n");
        return;
    }
    
    metal_view = (__bridge MTKView*)SDL_Metal_CreateView(window);
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    
    layer = (__bridge CAMetalLayer*)SDL_RenderGetMetalLayer(renderer);
    layer.pixelFormat = MTLPixelFormatBGRA8Unorm;
    
    imctx = ImGui::CreateContext();
    
    
    
    ImGui_ImplSDL2_InitForMetal(window);
    ImGui_ImplMetal_Init(layer.device);
    
    device = layer.device;
    
    command_queue = [device newCommandQueue];

    renderpass_descriptor = [MTLRenderPassDescriptor new];
    
    metal_setup();
    
    // Create dispatch timers
    timer_source = dispatch_source_create(DISPATCH_SOURCE_TYPE_TIMER, 0, 0, dispatch_get_main_queue());
    dispatch_source_set_event_handler(timer_source, ^{ graphics_loop(); });
    dispatch_source_set_timer(timer_source, dispatch_time(DISPATCH_TIME_NOW, NSEC_PER_SEC), NSEC_PER_SEC * 0.016, 0);
    dispatch_resume(timer_source);
}

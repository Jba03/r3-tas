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
#include "camera.h"
#include "SDL.h"
#include "dynamics.h"
#include "game.h"
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

struct camera* camera;

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
    desc.width = 800*2;
    desc.height = 600*2;
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
    
    vertex_descriptor.layouts[0].stride = sizeof(struct vertex);
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
#include "superobject.h"
#include "ipo.h"
#include "physical_object.h"
#include "collideset.h"
#include "collide_object.h"
#include "collide_mesh.h"

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

extern struct mesh* meshlist[1000];
extern int current_mesh;
extern struct actor* current_actor;

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


void graphics_loop()
{
    
//    if (main_render)
//    {
//        SDL_HideWindow(window);
//        return;
//    }
//    else
//    {
//        SDL_ShowWindow(window);
//    }
    
    return;
    
    SDL_ShowWindow(window);

    //camera->position = struct vector3_new(0, 0, 0);
    
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
                ImGui::GetIO().MousePos = ImVec2(event.motion.x, event.motion.y);
                camera_update(camera, event.motion.x, event.motion.y, true);
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
    
    struct vector3 campos = *(struct vector3*)(memory.base + 0x00c531bc);
    struct vector3 lookato = *(struct vector3*)(memory.base + 0x00c53910);
    float fov = host_byteorder_f32(*(float32*)(memory.base + 0x00C751B4));
    fov = fov == 0.0f ? 1.3 : fov;
    
    simd_float3 position;
    position.x = host_byteorder_f32(*(uint32_t*)&campos.x);
    position.z = host_byteorder_f32(*(uint32_t*)&campos.y);
    position.y = host_byteorder_f32(*(uint32_t*)&campos.z);
    
    simd_float3 lookat;
    lookat.x = host_byteorder_f32(*(uint32_t*)&lookato.x);
    lookat.z = host_byteorder_f32(*(uint32_t*)&lookato.y);
    lookat.y = host_byteorder_f32(*(uint32_t*)&lookato.z);
    
//    printf("pos: %f, %f, %f\n", position.x, position.y, position.z);
//    printf("lookat: %f, %f, %f\n", lookat.x, lookat.y, lookat.z);
    
    simd_float4x4 view = matrix_look_at_left_hand(position,
                                                   lookat,
                                                   (simd_float3){0, 1, 0});
    
    simd_float4x4 projection = matrix_perspective_left_hand(fov, (float)width / (float)height, 0.1f, 1000.0f);
    simd_float4x4 correction = matrix_make_rows(1.0f, 0.0f, 0.0f, 0.0f,
                                                0.0f, 1.0f, 0.0f, 0.0f,
                                                0.0f, 0.0f, 0.5f, 0.5f,
                                                0.0f, 0.0f, 0.0f, 1.0f);
    
    //projection = matrix_multiply(correction, projection);
    
    uniform.view = view;
    uniform.projection = projection;
    uniform.model = matrix_identity_float4x4;
    uniform.camera_pos = position;
    uniform.color = simd_make_float4(1.0f, 1.0f, 1.0f, 1.0f);
    
    uniform.use_texture = true;
    
    for (int i = 0; i < current_mesh; i++)
    {
        struct mesh* mesh = meshlist[i];
        
        const struct collide_material* material = (const struct collide_material*)pointer(mesh->material);
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
        
        const struct transform* T = (const struct transform*)mesh->transform_global;
        if (T)
        {
            const struct matrix4 mat = matrix4_host_byteorder(T->matrix);
            struct vector3 pos = game_matrix4_position(mat);
            uniform.model = matrix4x4_translation(pos.x, pos.z, pos.y);
        }
        
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
    
    uniform.use_texture = false;
    
    if (hierarchy)
    {
        superobject_for_each_type(superobject_type_actor, superobject_first_child(hierarchy), object)
        {
            const struct actor* actor = (const struct actor*)superobject_data(object);
            if (!actor) continue;
            
            /* The camera blocks the view; skip it. */
            if (offset(object) == 0xC533E0) continue;
            
            struct mesh_data* data = (struct mesh_data*)sphere_mesh->internal_data;
            
            const struct transform* T = (const struct transform*)pointer(object->transform_global);
            const struct matrix4 mat = ( matrix4_host_byteorder(T->matrix));
            struct vector3 pos = game_matrix4_position(mat);
            
            //printf("matrix: %X\n", offset(&T->matrix));
            
//            const struct dynam* dynam = (const struct dynam*)actor_dynam(actor);
//            if (dynam)
//            {
//                const struct dynamics* dynamics = (const struct dynamics*)pointer(dynam->dynamics);
//                if (dynamics)
//                {
//                    pos = vector3_add(pos, vector3_host_byteorder(dynamics->advanced.wall_normal));
//                }
//            }
            
            
            
            //printf("pos: %f %f %f\n", pos.x, pos.y, pos.z);
            
            uniform.color = simd_make_float4(1.0f, 0.0f, 0.0f, 1.0f);
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
    
    /* Draw ZDx */
    if (false)
    {
        superobject_for_each_type(superobject_type_actor, superobject_first_child(hierarchy), object)
        {
            const struct actor* actor = (const struct actor*)superobject_data(object);
            if (!actor) continue;

            const struct actor_collideset* collset = (const struct actor_collideset*)actor_collset(actor);
            if (!collset) continue;
            
            const struct zdx_list* zddlist = (const struct zdx_list*)pointer(collset->zde_list);
            //printf("zdd list: %X\n", offset(&zddlist->list));
            if (zddlist)
            {
                for (unsigned int i = 0; i < host_byteorder_32(zddlist->list.n_entries); i++)
                {
                    pointer* zonesetptr = (pointer*)pointer(zddlist->list.first);
                    if (zonesetptr)
                    {
                        const struct collide_object* obj = (const struct collide_object*)pointer(*zonesetptr);
                        if (obj)
                        {
                            //printf("zdd list: %X\n", offset(obj));
                            
                            printf("sphere center: %f %f %f\n",
                                    host_byteorder_f32(obj->bounding_sphere_position.x),
                                   host_byteorder_f32(obj->bounding_sphere_position.y),
                                   host_byteorder_f32(obj->bounding_sphere_position.z));
                        }
                    }
                    
//                    const pointer* zddoffset = ((pointer*)pointer(zddlist->first_element)) + i;
//                    const struct collide_object* zdd = (const struct collide_object*)zddoffset;
//                    if (zdd)
//                    {
//                        printf("list: %X, %d\n", offset(zdd), host_byteorder_16(zdd->n_elements));
//                    }

                    //struct collide_object* zdd = (struct collide_object*)(*list);
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
    
    camera = camera_create(-90.0f, 0.0f, 0.1f, 0.25f, 90.0f);
    
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

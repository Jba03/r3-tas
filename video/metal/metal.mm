//
//  metal.m
//  r3-tas
//
//  Created by Jba03 on 2022-12-29.
//

#import <QuartzCore/QuartzCore.h>
#import <Metal/Metal.h>
#import <MetalKit/MTKView.h>
#import <ModelIO/ModelIO.h>
#import <Cocoa/Cocoa.h>

#include <dispatch/dispatch.h>


#include "graphics.hh"
#include "stCameraGLI.hh"
#include "SDL.h"
#include "game.hh"
#include "memory.hh"
#include "library.hh"

#include "stSuperObject.hh"
#include "stInstantiatedPhysicalObject.hh"
#include "stPhysicalObject.hh"
#include "stCollideSet.hh"
#include "stCollideObject.hh"
#include "stCollideElementIndexedTriangles.hh"
#include "stOctree.hh"
#include "solver.hh"

//#include "fnCollision.c"



#include "emulator.hh"

#include "gui.hh"
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

#pragma mark - Texture

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

#pragma mark - Primitives

static unsigned int sphereNumIndices = 0;
static unsigned int sphereNumVertices = 0;
static id<MTLBuffer> sphereVertexBuffer = NULL;
static id<MTLBuffer> sphereNormalBuffer = NULL;
static id<MTLBuffer> sphereIndexBuffer = NULL;

static int metal_setup()
{
    id<MTLLibrary> library = shader_library();
    if (!library) return -1;
    
    id<MTLFunction> vertexfn = [library newFunctionWithName: @"vertex_main"];
    id<MTLFunction> fragmentfn = [library newFunctionWithName: @"fragment_main"];
    
    MTLDepthStencilDescriptor *depthStateDesc = [MTLDepthStencilDescriptor new];
    depthStateDesc.depthCompareFunction = MTLCompareFunctionLess;
    depthStateDesc.depthWriteEnabled = YES;
    depth_state = [device newDepthStencilStateWithDescriptor:depthStateDesc];
    
    vertex_descriptor = [[MTLVertexDescriptor alloc] init];
    vertex_descriptor.layouts[0].stride = 0;
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
    
    NSError* error = NULL;
    pipeline_state = [device newRenderPipelineStateWithDescriptor: pipelineStateDescriptor error: &error];
    if (!pipeline_state)
    {
        fprintf(stderr, "error: %s\n", error.localizedDescription.UTF8String);
        return NULL;
    }
    
    #pragma mark Sphere
    {
        unsigned *sphereIndices = NULL;
        stVector3D *sphereVertices = NULL, *sphereNormals = NULL;
        sphereMeshGen(20, &sphereNumVertices, &sphereVertices, &sphereNormals, &sphereNumIndices, &sphereIndices);
        
        sphereVertexBuffer = [device newBufferWithBytes: (void*)sphereVertices length: sphereNumVertices * sizeof(stVector3D) options: MTLResourceStorageModeShared];
        sphereNormalBuffer = [device newBufferWithBytes: (void*)sphereNormals length: sphereNumVertices * sizeof(stVector3D) options: MTLResourceStorageModeShared];
        sphereIndexBuffer = [device newBufferWithBytes: (void*)sphereIndices length: sphereNumIndices * sizeof(unsigned int) options: MTLResourceStorageModeShared];
    }
    
    return 0;
}

static struct Uniform
{
    simd_float3 camera_pos;
    simd_float4x4 view;
    simd_float4x4 projection;
    simd_float4x4 model;
    simd_float3x3 normal_matrix;
    simd_float4 color;
    bool use_texture;
    bool useGameIndexing;
} uniform;

extern stCollideObject* collide_objects[1000];
extern stMatrix4D matrices[1000];
extern struct mesh* meshlist[1000];
extern int current_mesh;
extern int current_matrix;
extern int current_collide_object;
extern stEngineObject* current_actor;

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

static simd_float4x4 GameToMetalMatrix(const stMatrix4D mat)
{
    simd_float4x4 m;
    m.columns[0] = simd_make_float4(mat.m00, mat.m01, mat.m02, mat.m03);
    m.columns[1] = simd_make_float4(mat.m10, mat.m11, mat.m12, mat.m13);
    m.columns[2] = simd_make_float4(mat.m20, mat.m21, mat.m22, mat.m23);
    m.columns[3] = simd_make_float4(mat.m30, mat.m31, mat.m32, mat.m33);
    
    return m;
}

static simd::float3x3 getNormalMatrix(simd::float4x4 view, simd::float4x4 model)
{
    simd::float4x4 MV = view * model;
    simd::float3x3 normalMatrix = { MV.columns[0].xyz, MV.columns[1].xyz, MV.columns[2].xyz };
    return simd::transpose(simd::inverse(normalMatrix));
}

#pragma mark - Draw primitives

static void drawSphere(const stVector3D center, const float radius, const stVector4D color, id<MTLRenderCommandEncoder> renderEncoder)
{
    uniform.useGameIndexing = false;
    uniform.color = *(simd_float4*)&color;
    
    stMatrix4D T = matrix4_make_scale(radius, radius, radius);
    T = matrix4_mul(T, matrix4_make_translation(center.x, center.y, center.z));
    uniform.model = GameToMetalMatrix(matrix4_transpose(T));
    uniform.normal_matrix = getNormalMatrix(uniform.view, uniform.model);
    id<MTLBuffer> uniformBuffer = [device newBufferWithBytes: &uniform length: sizeof uniform options: MTLResourceStorageModePrivate];
    
    [renderEncoder setVertexBuffer: sphereNormalBuffer offset: 0 atIndex: 1];
    [renderEncoder setVertexBuffer: sphereVertexBuffer offset: 0 atIndex: 0];
    [renderEncoder setVertexBuffer: uniformBuffer offset: 0 atIndex: 3];
    [renderEncoder setFragmentBuffer: uniformBuffer offset: 0 atIndex: 0];
    [renderEncoder drawIndexedPrimitives: MTLPrimitiveTypeTriangle indexCount: sphereNumIndices indexType: MTLIndexTypeUInt32 indexBuffer: sphereIndexBuffer indexBufferOffset: 0];
    
    [uniformBuffer release];
    
    uniform.useGameIndexing = true;
}

static void drawTriangle(stVector3D A, stVector3D B, stVector3D C, const stVector4D color, id<MTLRenderCommandEncoder> renderEncoder)
{
    uniform.color = *(simd_float4*)&color;
    uniform.useGameIndexing = false;
    uniform.model = GameToMetalMatrix(matrix4_identity);
    uniform.normal_matrix = getNormalMatrix(uniform.view, uniform.model);
    id<MTLBuffer> uniformBuffer = [device newBufferWithBytes: &uniform length: sizeof uniform options: MTLResourceStorageModePrivate];
    
    const stVector3D triangle[] = { A, B, C };
    id<MTLBuffer> vertexBuffer = [device newBufferWithBytes: (void*)triangle length: sizeof(stVector3D) * 3 options: MTLResourceStorageModePrivate];
    [renderEncoder setVertexBuffer: vertexBuffer offset: 0 atIndex: 0];
    [renderEncoder setVertexBuffer: uniformBuffer offset: 0 atIndex: 3];
    [renderEncoder setFragmentBuffer: uniformBuffer offset: 0 atIndex: 0];
    [renderEncoder drawPrimitives: MTLPrimitiveTypeTriangle vertexStart: 0 vertexCount: 3];
    
    [vertexBuffer release];
    [uniformBuffer release];
    
    uniform.useGameIndexing = true;
}

static void drawPoint(const stVector3D p, const stVector4D color, id<MTLRenderCommandEncoder> renderEncoder)
{
    uniform.color = *(simd_float4*)&color;
    uniform.model = GameToMetalMatrix(matrix4_identity);
    uniform.useGameIndexing = false;
    id<MTLBuffer> uniformBuffer = [device newBufferWithBytes: &uniform length: sizeof uniform options: MTLResourceStorageModePrivate];
    
    id<MTLBuffer> vertexBuffer = [device newBufferWithBytes: (void*)&p length: sizeof(stVector3D) options: MTLResourceStorageModePrivate];
    [renderEncoder setVertexBuffer: vertexBuffer offset: 0 atIndex: 0];
    [renderEncoder setVertexBuffer: uniformBuffer offset: 0 atIndex: 3];
    [renderEncoder setFragmentBuffer: uniformBuffer offset: 0 atIndex: 0];
    [renderEncoder drawPrimitives: MTLPrimitiveTypePoint vertexStart: 0 vertexCount: 1];
    
    [vertexBuffer release];
    [uniformBuffer release];
    
    uniform.useGameIndexing = true;
}

static void drawLine(stVector3D A, stVector3D B, const stVector4D color, id<MTLRenderCommandEncoder> renderEncoder)
{
    uniform.color = *(simd_float4*)&color;
    uniform.useGameIndexing = false;
    uniform.model = GameToMetalMatrix(matrix4_identity);
    uniform.normal_matrix = getNormalMatrix(uniform.view, uniform.model);
    id<MTLBuffer> uniformBuffer = [device newBufferWithBytes: &uniform length: sizeof uniform options: MTLResourceStorageModePrivate];
    
    const stVector3D line[] = { A, B };
    id<MTLBuffer> vertexBuffer = [device newBufferWithBytes: (void*)line length: sizeof(stVector3D) * 2 options: MTLResourceStorageModePrivate];
    [renderEncoder setVertexBuffer: vertexBuffer offset: 0 atIndex: 0];
    [renderEncoder setVertexBuffer: uniformBuffer offset: 0 atIndex: 3];
    [renderEncoder setFragmentBuffer: uniformBuffer offset: 0 atIndex: 0];
    [renderEncoder drawPrimitives: MTLPrimitiveTypeLine vertexStart: 0 vertexCount: 2];
    
    [vertexBuffer release];
    [uniformBuffer release];
    
    uniform.useGameIndexing = true;
}



static void RenderOctreeNode(const stMatrix4D transform, const stOctreeNode* nd, stVector4D color, id<MTLRenderCommandEncoder> render_encoder)
{
    if (!nd) return;
    
    stVector3D min = vector3_host_byteorder(nd->min);
    stVector3D max = vector3_host_byteorder(nd->max);
    
    const float x = min.x + (max.x - min.x) / 2.0f;
    const float y = min.y + (max.y - min.y) / 2.0f;
    const float z = min.z + (max.z - min.z) / 2.0f;
    
    stVector4D c = vector4_new(x, y, z, 1.0f);
    c = vector4_mul_matrix4(c, transform);
    
    stVector3D size = vector3_new(max.x - min.x, max.z - min.z, max.y - min.y);
   // draw_box(vector3_new(c.x, c.y, c.z), size, color, render_encoder); d
    
    const pointer* childlist = (const pointer*)pointer(nd->children);
    if (childlist)
    {
        for (int i = 0; i < 8; i++)
        {
            const stOctreeNode* node = (const stOctreeNode*)pointer(*(childlist + i));
            RenderOctreeNode(transform, node, color, render_encoder);
        }
    }
}

static void RenderOctrees(stSuperObject* so, stVector4D color, id<MTLRenderCommandEncoder> render_encoder)
{
    if (superobject_type(so) == superobject_type_ipo)
    {
        const stInstantiatedPhysicalObject* ipo = (const stInstantiatedPhysicalObject*)superobject_data(so);
        if (!ipo) return;
        
        const stCollideObject* zdr = fnIPOGetCollideObject(ipo);
        if (!zdr) return;
        
        const stOctree* octree = (const stOctree*)pointer(zdr->octree);
        if (!octree) return;
        
        const stOctreeNode* root = (const stOctreeNode*)pointer(octree->root);
        if (!root) return;
        
        
//            int n_selected = 0;
//            stOctreeNode* selected[OCTREE_MAX_SELECTED_NODES];
//            memset(selected, 0, sizeof(octree_node*) * OCTREE_MAX_SELECTED_NODES);
//            float st[OCTREE_MAX_SELECTED_NODES];
//
//            const stMatrix4D raymat = fnActorGetMatrix(actor_rayman);
//            const stVector3D raypos = game_matrix4_position(raymat);
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

static const simd_float4 collideMaterialGetColor(stCollideMaterial *material)
{
    uint16 identifier = host_byteorder_16(material->identifier);
    
    simd_float4 color = simd_make_float4(0.5f, 0.5f, 0.5f, 1.0);
    if (identifier & collide_material_grabbable_ledge) color = simd_make_float4(0.0f, 1.0f, 0.0f, 1.0);
    if (identifier & collide_material_no_collision) color = simd_make_float4(0.5f, 0.5f, 0.5f, 1.0);
    //color.w = 0.5f;
    //if (identifier & collide_material_unknown) color = simd_make_float4(0.5f, 0.5f, 0.5f, 1.0);
    //if (identifier & collide_material_unknown2) color = simd_make_float4(0.5f, 0.5f, 0.5f, 1.0);
    //if (identifier & collide_material_unknown3) color = simd_make_float4(0.5f, 0.5f, 0.5f, 1.0);
    
    return color;
}

extern void PrintMatrix(const stMatrix4D m);

static void drawGeometryRecursive(const stSuperObject* root, const stMatrix4D transform, id<MTLRenderCommandEncoder> render_encoder)
{
    if (!root) return;
    
    /* Calculate the new transformation matrix */
    const stMatrix4D T = matrix4_mul(fnSuperobjectGetGlobalMatrix(root), transform);
    uniform.useGameIndexing = true;
    
    if (superobject_type(root) == superobject_type_ipo)
    {
        const stInstantiatedPhysicalObject* ipo = (const stInstantiatedPhysicalObject*)superobject_data(root);
        if (ipo)
        {
            //if (strcmp(fnIPOGetName(ipo), "IPO_w_e_clairiere") != 0) goto end;
            
            const stCollideObject* zdr = fnIPOGetCollideObject(ipo);
            if (zdr)
            {
                const int16 n_vertices = host_byteorder_16(zdr->numVertices);
                
                //if (n_vertices < 500) goto end;
                
                
                stVector3D pos = fnActorGetPosition(actor_rayman);
                pos.z += 0.85f;
                //stMatrix4D start = matrix4_transpose(matrix4_make_translation(pos.x, pos.y, pos.z));
                stMatrix4D start = (fnActorGetMatrix(actor_rayman));
                start.m23 += 0.9f + (((9.81f*-1.0f)*0.016f)*0.016f);
                //pos.z += 0.8f;
                stMatrix4D end = start; //(fnActorGetMatrix(actor_rayman));
                //end.m13 += 1.0f;
                
                
                //PrintMatrix(start);
                
                uniform.color = simd_make_float4(0.75f, 0.75f, 0.75f, 1.0f);
                
                CollisionRecord record = {};
                if (CollideSphereWithGeometry(0.9f, start, end, zdr, matrix4_transpose(T), &record))
                {
                    //printf("collision!\n");
                    for (int i = 0; i < record.numCollisions; i++)
                    {
                        Collision *col = &record.collisions[i];
                        //printf("%.2f %.2f %.2f\n", col->normal.x, col->normal.y, col->normal.z);
                        //drawSphere(col->contact, 0.9f, vector4_new(1.0f, 0.0f, 0.0f, 0.5f), render_encoder);
                        
                        //drawSphere(vector3_add(col->contact,col->translation), 0.25f, vector4_new(1.0f, 0.0f, 1.0f, 0.5f), render_encoder);
                        //drawSphere(vector3_add(col->contact,col->normal), 0.25f, vector4_new(1.0f, 0.9f, 0.0f, 0.5f), render_encoder);
                        
//                        if (col->staticType == COLLISION_TYPE_TRIANGLE)
//                            drawTriangle(col->triangle.A, col->triangle.B, col->triangle.C, vector4_new(1.0f, 0.0f, 1.0f, 1.0f), render_encoder);
                        //drawSphere(pos, 0.9f, vector4_new(0.0f, 0.0f, 1.0f, 1.0f), render_encoder);
                        
//                        for (int i = 0; i < result.n_triangles; i++)
//                        {
//                            struct Triangle t = result.triangles[i];
//                            drawTriangle(t.A, t.B, t.C, vector4_new(1.0f, 0.0f, 0.0f, 0.6f), render_encoder);
//                        }
                    }
                        
                    //drawLine(result.segmentEnd, result.segmentStart, vector4_new(1.0f, 0.0f, 0.0f, 0.5f), render_encoder);
                    uniform.color = simd_make_float4(0.75f, 0.75f, 0.75f, 1.0f);
                }
                
//                printf("%.2f %.2f %.2f\n", result.segmentEnd.x, result.segmentEnd.y, result.segmentEnd.z);
//                drawSphere(result.segmentEnd, 0.9f, vector4_new(1.0f, 0.0f, 0.0f, 1.0f), render_encoder);
//                drawSphere(result.segmentStart, 0.9f, vector4_new(0.0f, 0.0f, 1.0f, 1.0f), render_encoder);
                
                
                
                int mesh_idx = 0;
                const stCollideElementIndexedTriangles* mesh;
                while ((mesh = fnCollideObjectGetElementIndexedTriangles(zdr, mesh_idx)))
                {
                    uint16* indices = (uint16*)pointer(mesh->faceIndices);
                    stVector3D* vertices = (stVector3D*)pointer(zdr->vertices);
                    stVector3D* normals = (stVector3D*)pointer(mesh->normals);
                    
                    if (vertices && indices && normals)
                    {
                        const int16 n_faces = host_byteorder_16(mesh->numFaces);
                        
                            
                        stCollideMaterial *material = (stCollideMaterial*)pointer(mesh->material);
                        
                        //uniform.color = collideMaterialGetColor(material);
                        uniform.model = GameToMetalMatrix(T);
                        uniform.normal_matrix = getNormalMatrix(uniform.view, uniform.model);
                        id<MTLBuffer> uniformBuffer = [device newBufferWithBytes: &uniform length: sizeof uniform options: MTLResourceStorageModePrivate];
                        
                        /* Cannot use newBufferWithBytesNoCopy here, since data is not page-aligned. Mark the buffers as private: they aren't written to by the CPU. */
                        id<MTLBuffer> indexBuffer = [device newBufferWithBytes: (void*)indices length: n_faces * 3 * sizeof(uint16) options: MTLResourceStorageModePrivate];
                        id<MTLBuffer> normalBuffer = [device newBufferWithBytes: (void*)normals length: n_faces * sizeof(stVector3D) options: MTLResourceStorageModePrivate];
                        id<MTLBuffer> vertexBuffer = [device newBufferWithBytes: (void*)vertices length: n_vertices * sizeof(stVector3D) options: MTLResourceStorageModePrivate];
                        
                        [render_encoder setVertexBuffer: vertexBuffer offset: 0 atIndex: 0];
                        [render_encoder setVertexBuffer: normalBuffer offset: 0 atIndex: 1];
                        [render_encoder setVertexBuffer: indexBuffer offset: 0 atIndex: 2];
                        [render_encoder setVertexBuffer: uniformBuffer offset: 0 atIndex: 3];
                        [render_encoder setFragmentTexture: checker_texture atIndex: 0];
                        [render_encoder setFragmentBuffer: uniformBuffer offset: 0 atIndex: 0];
                        
                        [render_encoder drawPrimitives: MTLPrimitiveTypeTriangle vertexStart: 0 vertexCount: n_faces * 3];
                        
                        [indexBuffer release];
                        [normalBuffer release];
                        [vertexBuffer release];
                        [uniformBuffer release];
                    }
                    
                    mesh_idx++;
                }
            }
        }
    }
    end:
    /* Draw children */
    superobject_for_each(root, child) drawGeometryRecursive(child, T, render_encoder);
    
    uniform.useGameIndexing = false;
}

static void drawZdx(const stActor *actor, const stCollideSet *collSet, int which, id<MTLRenderCommandEncoder> renderEncoder)
{
    stZdxList *zdxList = NULL;
    stVector4D color;
    
    switch (which)
    {
        case 0: zdxList = (stZdxList*)pointer(collSet->zddList); color = vector4_new(1.0f, 1.0f, 1.0f, 0.5f); break; // ZDD = transparent
        case 1: zdxList = (stZdxList*)pointer(collSet->zdeList); color = vector4_new(0.0f, 0.0f, 1.0f, 0.5f); break; // ZDE = blue
        case 2: zdxList = (stZdxList*)pointer(collSet->zdmList); color = vector4_new(1.0f, 1.0f, 0.0f, 0.5f); break; // ZDM = yellow
        case 3: zdxList = (stZdxList*)pointer(collSet->zdrList); color = vector4_new(1.0f, 0.0f, 0.0f, 0.5f); break; // ZDR = red
    }
    
    if (zdxList)
    {
        stZdxListEntry *entry = (stZdxListEntry*)pointer(zdxList->list.first);
        if (entry)
        {
            //printf("num: %d\n", host_byteorder_32(zdxList->list.numEntries));
            for (unsigned int i = 0; i < host_byteorder_32(zdxList->list.numEntries); i++)
            {
                const stCollideObject *collideObject = (const stCollideObject*)pointer(entry->data);
                if (collideObject)
                {
                    stVector4D pos = vector4_host_byteorder(collideObject->boundingSpherePosition);
                    stVector3D pos3 = vector3_new(pos.x, pos.y, pos.z); //vector4_mul_matrix4(pos, fnActorGetMatrix(actor));
                    pos3 = vector3_add(pos3, fnActorGetPosition(actor));
                    float radius = host_byteorder_f32(collideObject->boundingSphereRadius);
                    pos.w = 0.5f;
                    
                    drawSphere(*(stVector3D*)&pos3, radius, color, renderEncoder);
                }
                
                entry++;
            }
        }
    }
}

static void drawSpheresRecursive(const stSuperObject* root, id<MTLRenderCommandEncoder> render_encoder)
{
    if (!root) return;
    
    struct zdxData {
        pointer next;
        pointer prev;
        pointer parent;
        pointer data;
    };
    
    if (superobject_type(root) == superobject_type_actor)
    {
        const stEngineObject *actor = (const stEngineObject*)pointer(root->data);
        if (actor)
        {
            const stCollideSet *collSet = (const stCollideSet*)pointer(actor->collSet);
            if (collSet)
            {
                drawZdx(actor, collSet, 0, render_encoder);
                drawZdx(actor, collSet, 1, render_encoder);
                drawZdx(actor, collSet, 2, render_encoder);
                drawZdx(actor, collSet, 3, render_encoder);
            }
        }
    }
    
    /* Draw children */
    superobject_for_each(root, child) drawSpheresRecursive(child, render_encoder);
}

//static void collideGeometry(const stSuperObject* root, const stMatrix4D transform, id<MTLRenderCommandEncoder> render_encoder)
//{
//    if (!root) return;
//
//    /* Calculate the new transformation matrix */
//    const stMatrix4D T = matrix4_mul(fnSuperobjectGetGlobalMatrix(root), transform);
//    uniform.useGameIndexing = true;
//
//    if (superobject_type(root) == superobject_type_ipo)
//    {
//        const stInstantiatedPhysicalObject* ipo = (const stInstantiatedPhysicalObject*)superobject_data(root);
//        if (ipo)
//        {
//            const stCollideObject* zdr = fnIPOGetCollideObject(ipo);
//            if (zdr)
//            {
//
//            }
//        }
//    }
//
//    /* Draw children */
//    superobject_for_each(root, child) drawGeometryRecursive(child, T, render_encoder);
//
//    uniform.useGameIndexing = false;
//}

static int frame = 0;

void graphics_loop()
{
    if (!hierarchy) return;
    if (engine->mode != 9) return;
    
//    if (saveState) {
//        emulatorSaveState(1, false);
//        saveState = false;
//    }
//
//    if (loadState) {
//        emulatorLoadState(1);
//        loadState = false;
//    }
//
//    if (frameAdvance) {
//        emulatorFrameAdvance();
//        padStatus = makePadStatus(0, 0);
//    }
    
    SDL_ShowWindow(window);
    
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
    [renderEncoder pushDebugGroup: @"r3-render"];
    [renderEncoder setCullMode: MTLCullModeNone];
    [renderEncoder setFrontFacingWinding: MTLWindingClockwise];
    [renderEncoder setRenderPipelineState: pipeline_state];
    [renderEncoder setDepthStencilState: depth_state];
    [renderEncoder setViewport: (MTLViewport){0, 0, static_cast<double>(width), static_cast<double>(height), 0.0f, 1.0f /* 1.0f: important! */}];
    
    stCameraGLI* camera = (stCameraGLI*)pointer(engine->viewportCamera[0]);
    
    /* Construct camera matrices. Use negative fov in order to account for flipped transformations. */
    stMatrix4D projection = matrix4_perspective(-host_byteorder_f32(camera->xAlpha), (float)width / (float)height, 1.1f, 1000.0f);
    stMatrix4D view = matrix4_host_byteorder(camera->transform.matrix);
    view.m01 = -view.m01; view.m11 = -view.m11;
    view.m21 = -view.m21; view.m22 = -view.m22;
    view.m31 = -view.m31; view.m32 = -view.m32;
    view.m02 = -view.m02; view.m12 = -view.m12;
    
    uniform.view = GameToMetalMatrix(view);
    uniform.projection = GameToMetalMatrix(projection);
    uniform.model = matrix_identity_float4x4;
    //uniform.camera_pos = position;
    uniform.color = simd_make_float4(1.0f, 1.0f, 1.0f, 1.0f);
    uniform.use_texture = true;
    
    
//
//    stVector3D A = vector3_new(152.45, 69.02, 1.51);
//    stVector3D B = vector3_new(145, 60, 1.50);
//    stVector3D C = vector3_new(139, 66.84, 1.50);
//
//    stVector4D color = vector4_new(0.0f, 1.0f, 1.0f, 1.0f);
//
//    struct CollisionContext c;
//    c.sphere.center = fnActorGetPosition(actor_rayman);
//    c.sphere.radius = 0.9;
//    c.triangle.A = A;
//    c.triangle.B = B;
//    c.triangle.C = C;
//
//    c.movementStart = c.sphere.center;
//    c.movementDirection = vector3_add(c.sphere.center, vector3_new(0.0f, 0.0f, -1.0f));
//    c.movementEnd = vector3_add(c.sphere.center, vector3_new(0.0f, 0.0f, -1.0f));
//
//    if (sphereTriangleIntersection(&c.sphere, &c.triangle))
//        color = vector4_new(1.0f, 0.0f, 1.0f, 1.0f);
//    
//    drawTriangle(A, B, C, color, renderEncoder);
    
    uniform.use_texture = true;
    
    
    
    
//    struct GeometrySolver g;
//    g.start = fnActorGetPosition(actor_rayman); //vector3_new(145, 59, 1.30);
//    g.end = vector3_new(-34.5, 11.43, -5.20);
//    g.numTriangles = 0;
//    g.numPoints = 0;
//
//    solveGeometry(father_sector, &g);
    
    //printf("points: %d\n", g.numPoints);
    
    //printf("%.2f %.2f %.2f", g.triangles[0].A.x, g.triangles[0].A.y, g.triangles[0].A.z);
    //printf("num tris: %d\n", g.numTriangles);
//    for (int i = 0; i < g.numPoints; i++)
//    {
//        //struct Triangle t = g.triangles[i];
//        //drawTriangle(t.A, t.B, t.C, vector4_new(1.0f, 0.0f, 1.0f, 1.0f), renderEncoder);
//        drawPoint(g.points[i], vector4_new(1.0f, 0.0f, 1.0f, 0.75f), renderEncoder);
//    }
    
    
    
    drawGeometryRecursive(father_sector, matrix4_identity, renderEncoder);
    
    stVector3D start = fnActorGetPosition(actor_rayman);
    stVector3D end = vector3_add(start, fnActorGetSpeed(actor_rayman));
    
    stDynamics dynamics;
    dynamics.base.previousTransform.matrix = matrix4_transpose(matrix4_make_translation(start.x, start.y, start.z)); //fnActorGetMatrix(actor_rayman);
    dynamics.base.currentTransform.matrix = matrix4_transpose(matrix4_make_translation(end.x, end.y, end.z)); //fnActorGetMatrix(actor_rayman);
    
    float x = sin(frame * 0.1) * 20.0f;
    float y = cos(frame * 0.1) * 20.0f;
    dynamics.base.proposedSpeed = vector3_new(50, 0, 1.0f);
    dynamics.base.previousSpeed = vector3_new(0.0f, 0.0f, -10);
    
    //TransformAndCollide(&dynamics, 1.0f / 60.0f);
    
    drawSphere(start, 0.9f, vector4_new(1.0f, 0.64f, 0.0f, 0.8f), renderEncoder);
    
    //drawSphere(start, 0.5f, vector4_new(1.0f, 0.0f, 1.0f, 0.8f), renderEncoder);
    //drawSphere(end, 0.5f, vector4_new(0.0f, 1.0f, 1.0f, 0.8f), renderEncoder);
    
    stVector3D newPos = game_matrix4_position(dynamics.base.currentTransform.matrix);
   //printf("%.2f %.2f %.2f\n", newPos.x, newPos.y, newPos.z);
    
    //drawSphere(game_matrix4_position(dynamics.base.currentTransform.matrix), 0.9f, vector4_new(0.0f, 0.0f, 1.0f, 0.8f), renderEncoder);
    
    frame++;
    
    //drawSpheresRecursive(dynamic_world, renderEncoder);
    
//    /* Octree rendering */
//    [renderEncoder setTriangleFillMode: MTLTriangleFillModeLines];
//    if (father_sector)
//    {
//        superobject_for_each(father_sector, subsector)
//        {
//            stVector4D color = vector4_new(1.0f, 0.5f, 0.1f, 0.25f);
//            extern stSuperObject* viewed_sector;
//            if (subsector == viewed_sector) color = vector4_new(0.0f, 1.0f, 0.75f, 0.25f);
//            RenderOctrees(subsector, color, renderEncoder);
//        };
//    }
//    [renderEncoder setTriangleFillMode: MTLTriangleFillModeFill];
    
    
//    extern struct xray xray;
//    for (int i = 0; i < xray.n_points; i++)
//    {
//        const stVector3D point = vector3_host_byteorder(*xray.pointset[i]);
//        const stVector4D vertexT = vector4_mul_matrix4(vector4_new(point.x, point.y, point.z, 1.0f), T);
//
//        draw_sphere(point, 0.1f, vector4_new(1.0f, 0.0f, 0.0f, 1.0f), renderEncoder);
//
//
//    }
    
//    if (false)
//    {
//        superobject_for_each_type(superobject_type_actor, superobject_first_child(hierarchy), object)
//        {
//            const stEngineObject* actor = (const stEngineObject*)superobject_data(object);
//            if (!actor) continue;
//            
//            /* The camera blocks the view; skip it. */
//            if (offset(object) == 0xC533E0) continue;
//            
//            struct mesh_data* data = (struct mesh_data*)sphere_mesh->internal_data;
//            
//            const stTransform* T = (const stTransform*)pointer(object->globalTransform);
//            const stMatrix4D mat = ( matrix4_host_byteorder(T->matrix));
//            stVector3D pos = game_matrix4_position(mat);
//            
//            //printf("matrix: %X\n", offset(&T->matrix));
//            
////            const stDynam* dynam = (const stDynam*)actor_dynam(actor);
////            if (dynam)
////            {
////                const stDynamics* dynamics = (const stDynamics*)pointer(dynam->dynamics);
////                if (dynamics)
////                {
////                    pos = vector3_add(pos, vector3_host_byteorder(dynamics->advanced.wall_normal));
////                }
////            }
//            
//            
//            
//            //printf("pos: %f %f %f\n", pos.x, pos.y, pos.z);
//            
//            uniform.color = simd_make_float4(1.0f, 0.0f, 0.0f, 0.5f);
//            if (actor == current_actor) uniform.color = simd_make_float4(1.0f, 1.0f, 0.0f, 1.0f);
//            
//            uniform.model = matrix4x4_translation(pos.x, pos.z, pos.y);
//            
//            id<MTLBuffer> uniformbuffer = [device newBufferWithBytes: &uniform length: sizeof uniform options: MTLResourceStorageModeShared];
//            
//            
//            [renderEncoder setVertexBuffer: data->vertex_buffer offset: 0 atIndex: 0];
//            [renderEncoder setVertexBuffer: uniformbuffer offset: 0 atIndex: 1];
//            [renderEncoder setFragmentTexture: checker_texture atIndex: 0];
//            [renderEncoder setFragmentBuffer: uniformbuffer offset: 0 atIndex: 0];
//            
//            [renderEncoder drawIndexedPrimitives: MTLPrimitiveTypeTriangle
//                                      indexCount: sphere_mesh->n_indices
//                                       indexType: MTLIndexTypeUInt32
//                                     indexBuffer: data->index_buffer
//                               indexBufferOffset: 0];
//        }
//    }
//    
//    draw_sphere(sphere_pos, 1.0f, vector4_new(1.0f, 0.0f, 1.0f, 1.0f), renderEncoder);
//    
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
            const stEngineObject* actor = (const stEngineObject*)superobject_data(object);
            if (!actor) continue;

            const stCollideSet* collset = (const stCollideSet*)actor_collset(actor);
            if (!collset) continue;
            
            const stZdxList* zddlist = (const stZdxList*)pointer(collset->zdeList);
            //printf("zdd list: %X\n", offset(&zddlist->list));
            if (zddlist)
            {
                for (unsigned int i = 0; i < host_byteorder_32(zddlist->list.numEntries); i++)
                {
                    pointer* zonesetptr = (pointer*)pointer(zddlist->list.first);
                    if (zonesetptr)
                    {
                        const stCollideObject* obj = (const stCollideObject*)pointer(*zonesetptr);
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
//                    const stCollideObject* zdd = (const stCollideObject*)zddoffset;
//                    if (zdd)
//                    {
//                        printf("list: %X, %d\n", offset(zdd), host_byteorder_16(zdd->n_elements));
//                    }

                    //stCollideObject* zdd = (stCollideObject*)(*list);
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
    
    //NSWindow *mainWindow = [NSApp mainWindow];
    //NSLog(@"%@", [mainWindow description]);
    
    dispatch_async(dispatch_get_main_queue(), ^{
    
        SDL_Init(SDL_INIT_VIDEO);
        
        
        
        if (!(window = SDL_CreateWindow("Breakpoint debug", 0, 0, 800, 600, SDL_WINDOW_METAL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_HIDDEN)))
        {
            fprintf(stderr, "failed to create SDL window\n");
            return;
        }


    metal_view = (__bridge MTKView*)SDL_Metal_CreateView(window);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

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
        
    });
}

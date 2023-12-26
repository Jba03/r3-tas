//
//  metal.m
//  r3-tas
//
//  Created by Jba03 on 2022-12-29.
//
    
#import <Metal/Metal.h>

#define UNDEF_TYPES
#include "library.hh"
using char8     = datatype<int8_t>;
using uchar8    = datatype<uint8_t>;
using int8      = datatype<int8_t>;
using int16     = datatype<int16_t>;
using int32     = datatype<int32_t>;
using int64     = datatype<int64_t>;
using float32   = datatype<uint32_t, true>;
#include "game.hh"

//#include "stSuperObject.hh"
//#include "stInstantiatedPhysicalObject.hh"
//#include "stPhysicalObject.hh"
//#include "stCollideSet.hh"
//#include "stCollideObject.hh"
//#include "stCollideElementIndexedTriangles.hh"
//#include "stOctree.hh"
#include "stCameraGLI.hh"


namespace graphics
{
    static id<MTLDevice> metalDevice = nullptr;
    static id<MTLCommandQueue> commandQueue = nullptr;
    static id<MTLCommandBuffer> commandBuffer = nullptr;
    static id<MTLRenderPipelineState> pipelineState = nullptr;
    static id<MTLRenderCommandEncoder> renderEncoder = nullptr;
    static id<MTLDepthStencilState> depthState = nullptr;
    static id<MTLLibrary> shaderLibrary = nullptr;
    static id<MTLTexture> depthTexture = nullptr;
    static id<MTLTexture> checkerTexture = nullptr;
    static id<MTLTexture> renderTexture = nullptr;
    static MTLVertexDescriptor *vertexDescriptor = nullptr;
    static MTLRenderPassDescriptor *renderpassDescriptor = nullptr;
    
    static const id<MTLLibrary> loadShaderLibrary()
    {
        NSString* path = [NSString stringWithFormat: @"%s%@", LIBR3TAS_DIR, @"/graphics/metal/main.metal"];
        printf("path: %s", path.UTF8String);
        NSError *error = nullptr;
        
        NSString *source = [NSString stringWithContentsOfFile: path encoding: NSUTF8StringEncoding error: &error];
        if (error)
        {
            fprintf(stderr, "error: %s\n", error.localizedDescription.UTF8String);
            return nullptr;
        }
        
        id<MTLLibrary> lib = [metalDevice newLibraryWithSource: source options: NULL error: &error];
        if (error)
        {
            fprintf(stderr, "error: %s\n", error.localizedDescription.UTF8String);
            return nullptr;
        }
        
        return lib;
    }
    
    static void createTextures()
    {
        MTLTextureDescriptor *depthTextureDescriptor = nullptr;
        MTLTextureDescriptor *renderTextureDescriptor = nullptr;
        
        depthTextureDescriptor = [[MTLTextureDescriptor alloc] init];
        depthTextureDescriptor.width = 800 * 2;
        depthTextureDescriptor.height = 600 * 2;
        depthTextureDescriptor.pixelFormat = MTLPixelFormatDepth32Float_Stencil8;
        depthTextureDescriptor.mipmapLevelCount = 1;
        depthTextureDescriptor.storageMode = MTLStorageModePrivate;
        depthTextureDescriptor.usage = MTLTextureUsageRenderTarget;
        assert(depthTexture = [metalDevice newTextureWithDescriptor: depthTextureDescriptor]);
        
        renderTextureDescriptor = [[MTLTextureDescriptor alloc] init];
        renderTextureDescriptor.width = 800;
        renderTextureDescriptor.height = 600;
        renderTextureDescriptor.depth = 1;
        renderTextureDescriptor.textureType = MTLTextureType2D;
        renderTextureDescriptor.pixelFormat = MTLPixelFormatRGBA8Unorm;
        renderTextureDescriptor.mipmapLevelCount = 1;
        renderTextureDescriptor.storageMode = MTLStorageModeShared;
        renderTextureDescriptor.usage = MTLTextureUsageRenderTarget;
        assert(renderTexture = [metalDevice newTextureWithDescriptor: renderTextureDescriptor]);
    }
    
    bool initialize()
    {
        NSError* error = NULL;
        
        commandQueue = [metalDevice newCommandQueue];
        renderpassDescriptor = [MTLRenderPassDescriptor new];
        
        if (!(shaderLibrary = loadShaderLibrary()))
            return false;
        
        id<MTLFunction> vertexFunction = [shaderLibrary newFunctionWithName: @"vertex_main"];
        id<MTLFunction> fragmentFunction = [shaderLibrary newFunctionWithName: @"fragment_main"];
        
        MTLDepthStencilDescriptor *depthStateDesc = [MTLDepthStencilDescriptor new];
        depthStateDesc.depthCompareFunction = MTLCompareFunctionLess;
        depthStateDesc.depthWriteEnabled = YES;
        depthState = [metalDevice newDepthStencilStateWithDescriptor:depthStateDesc];
        
        vertexDescriptor = [[MTLVertexDescriptor alloc] init];
        vertexDescriptor.layouts[0].stride = 0;
        vertexDescriptor.layouts[0].stepRate = 1;
        vertexDescriptor.layouts[0].stepFunction = MTLVertexStepFunctionPerVertex;
        
        
        MTLRenderPipelineDescriptor *pipelineStateDescriptor = [MTLRenderPipelineDescriptor new];
        pipelineStateDescriptor.label = @"Simple Pipeline";
        pipelineStateDescriptor.vertexFunction = vertexFunction;
        pipelineStateDescriptor.fragmentFunction = fragmentFunction;
        
        pipelineStateDescriptor.colorAttachments[0].pixelFormat = MTLPixelFormatRGBA8Unorm;
        pipelineStateDescriptor.colorAttachments[0].blendingEnabled = YES;
        pipelineStateDescriptor.colorAttachments[0].rgbBlendOperation = MTLBlendOperationAdd;
        pipelineStateDescriptor.colorAttachments[0].alphaBlendOperation = MTLBlendOperationAdd;
        pipelineStateDescriptor.colorAttachments[0].sourceRGBBlendFactor = MTLBlendFactorSourceAlpha;
        pipelineStateDescriptor.colorAttachments[0].sourceAlphaBlendFactor = MTLBlendFactorSourceAlpha;
        pipelineStateDescriptor.colorAttachments[0].destinationRGBBlendFactor = MTLBlendFactorOneMinusSourceAlpha;
        pipelineStateDescriptor.colorAttachments[0].destinationAlphaBlendFactor = MTLBlendFactorOneMinusSourceAlpha;
        
        pipelineStateDescriptor.vertexDescriptor = vertexDescriptor;
        pipelineStateDescriptor.depthAttachmentPixelFormat = MTLPixelFormatDepth32Float_Stencil8;
        pipelineStateDescriptor.stencilAttachmentPixelFormat = MTLPixelFormatDepth32Float_Stencil8;
        
        createTextures();
        
        if (!(pipelineState = [metalDevice newRenderPipelineStateWithDescriptor: pipelineStateDescriptor error: &error]))
        {
            fprintf(stderr, "error: %s\n", error.localizedDescription.UTF8String);
            return false;
        }
        
        return 0;
    }
    
    static struct Uniform
    {
//        simd_float3 camera_pos;
//        simd_float4x4 view;
//        simd_float4x4 projection;
//        simd_float4x4 model;
//        simd_float3x3 normal_matrix;
//        simd_float4 color;
//        bool use_texture;
//        bool useGameIndexing;
    } uniform;
    
//    static simd_float4x4 GameToMetalMatrix(const stMatrix4D mat)
//    {
//        simd_float4x4 m;
//        m.columns[0] = simd_make_float4(mat.m00, mat.m01, mat.m02, mat.m03);
//        m.columns[1] = simd_make_float4(mat.m10, mat.m11, mat.m12, mat.m13);
//        m.columns[2] = simd_make_float4(mat.m20, mat.m21, mat.m22, mat.m23);
//        m.columns[3] = simd_make_float4(mat.m30, mat.m31, mat.m32, mat.m33);
//
//        return m;
//    }
//
//    static simd::float3x3 getNormalMatrix(simd::float4x4 view, simd::float4x4 model)
//    {
//        simd::float4x4 MV = view * model;
//        simd::float3x3 normalMatrix = { MV.columns[0].xyz, MV.columns[1].xyz, MV.columns[2].xyz };
//        return simd::transpose(simd::inverse(normalMatrix));
//    }
    
    void beginFrame(float w, float h)
    {
        commandBuffer = [commandQueue commandBuffer];
        renderpassDescriptor.colorAttachments[0].clearColor = MTLClearColorMake(0.0f, 0.0f, 0.0f, 1.0f);
        renderpassDescriptor.colorAttachments[0].texture = renderTexture;
        renderpassDescriptor.colorAttachments[0].loadAction = MTLLoadActionClear;
        renderpassDescriptor.colorAttachments[0].storeAction = MTLStoreActionStore;
        
        //    renderpass_descriptor.depthAttachment.texture = depth_texture;
        //    renderpass_descriptor.depthAttachment.clearDepth = 1.0f;
        //    renderpass_descriptor.depthAttachment.loadAction = MTLLoadActionClear;
        //    renderpass_descriptor.depthAttachment.storeAction = MTLStoreActionStore;
        //
        //    renderpass_descriptor.stencilAttachment.texture = depth_texture;
        //    renderpass_descriptor.stencilAttachment.loadAction = MTLLoadActionClear;
        //    renderpass_descriptor.stencilAttachment.storeAction = MTLStoreActionStore;
        
        renderEncoder = [commandBuffer renderCommandEncoderWithDescriptor: renderpassDescriptor];
        [renderEncoder pushDebugGroup: @"r3-render"];
        [renderEncoder setCullMode: MTLCullModeNone];
        [renderEncoder setFrontFacingWinding: MTLWindingClockwise];
        [renderEncoder setRenderPipelineState: pipelineState];
        [renderEncoder setDepthStencilState: depthState];
        [renderEncoder setViewport: (MTLViewport){0, 0, static_cast<double>(w), static_cast<double>(h), 0.0f, 1.0f}];
        
        stCameraGLI *camera = game::g_stEngineStructure->viewportCamera[0];
        /* Construct camera matrices. Use negative fov in order to account for flipped transformations. */
        stMatrix4D projection = stMatrix4D::perspective(-host_byteorder_f32(camera->xAlpha), w/h, 1.1f, 1000.0f);
        stMatrix4D view = camera->transform.matrix.hostByteOrder();
        view.m01 = -view.m01; view.m11 = -view.m11;
        view.m21 = -view.m21; view.m22 = -view.m22;
        view.m31 = -view.m31; view.m32 = -view.m32;
        view.m02 = -view.m02; view.m12 = -view.m12;
        
//        uniform.view = GameToMetalMatrix(view);
//        uniform.projection = GameToMetalMatrix(projection);
//        uniform.model = matrix_identity_float4x4;
//        //uniform.camera_pos = position;
//        uniform.color = simd_make_float4(1.0f, 1.0f, 1.0f, 1.0f);
//        uniform.use_texture = true;
    }
    
    void endFrame()
    {
        [renderEncoder popDebugGroup];
        [renderEncoder endEncoding];
        [commandBuffer commit];
    }
};

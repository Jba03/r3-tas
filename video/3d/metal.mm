//
//  metal.m
//  r3-tas
//
//  Created by Jba03 on 2022-12-29.
//
    
#import <Metal/Metal.h>
#import <MetalKit/MetalKit.h>
#import <Cocoa/Cocoa.h>
 
#include <iomanip>
#include <vector>

#include "structure.hh"
namespace r3 = library;


namespace graphics {
  r3::stMatrix4D viewMatrix;
  r3::stMatrix4D projectionMatrix;
    
  std::vector<r3::pointer<>> highlightList;
  
    void (*createTexture)(const char *name, unsigned width, unsigned height, unsigned samples);
    void (*updateTexture)(const char *name, unsigned width, unsigned height, unsigned row_length, uint8_t *buffer, size_t buffer_size);
    void (*uploadTexture)(const char *name, void* texture, unsigned width, unsigned height);
    void*(*getTexture)(const char *name);
    float* (*getProjectionMatrix)(void);
    
    static id<MTLDevice> metalDevice = nullptr;
    static id<MTLCommandQueue> commandQueue = nullptr;
    static id<MTLCommandBuffer> commandBuffer = nullptr;
    static id<MTLRenderPipelineState> pipelineState = nullptr;
    static id<MTLRenderCommandEncoder> renderEncoder = nullptr;
    static id<MTLDepthStencilState> depthState = nullptr;
    static id<MTLLibrary> shaderLibrary = nullptr;
    static id<MTLTexture> depthTexture = nullptr;
    static id<MTLTexture> checkerTexture = nullptr;
    static MTLVertexDescriptor *vertexDescriptor = nullptr;
    static MTLRenderPassDescriptor *renderpassDescriptor = nullptr;
    
  auto initializePrimitives() -> void;
    
  static id<MTLTexture> renderTexture = nullptr;
  static id<MTLBuffer> renderBuffer = nullptr;
    
  static auto loadShaderLibrary() -> id<MTLLibrary> {
    NSError *error = nullptr;
    NSString *path = [NSString stringWithFormat: @"%s%@", LIBR3TAS_DIR, @"/video/3d/main.metal"];
    NSString *source = [NSString stringWithContentsOfFile: path encoding: NSUTF8StringEncoding error: &error];
    id<MTLLibrary> lib = [metalDevice newLibraryWithSource: source options: NULL error: &error];
    if (error) {
      std::cerr << "error: " << error.localizedDescription << "\n";
      return nullptr;
    } else {
      return lib;
    }
  }
    
    static void createTextures()
    {
        MTLTextureDescriptor *depthTextureDescriptor = nullptr;
        MTLTextureDescriptor *checkerTextureDescriptor = nullptr;
        MTLTextureDescriptor *renderTextureDescriptor = nullptr;
        
        depthTextureDescriptor = [[MTLTextureDescriptor alloc] init];
        depthTextureDescriptor.width = 800;
        depthTextureDescriptor.height = 600;
        depthTextureDescriptor.pixelFormat = MTLPixelFormatDepth32Float_Stencil8;
        depthTextureDescriptor.mipmapLevelCount = 1;
        depthTextureDescriptor.storageMode = MTLStorageModePrivate;
        depthTextureDescriptor.usage = MTLTextureUsageRenderTarget;
        assert(depthTexture = [metalDevice newTextureWithDescriptor: depthTextureDescriptor]);
        
        
        checkerTextureDescriptor = [[MTLTextureDescriptor alloc] init];
        checkerTextureDescriptor.width = 2;
        checkerTextureDescriptor.height = 2;
        checkerTextureDescriptor.depth = 1;
        checkerTextureDescriptor.textureType = MTLTextureType2D;
        checkerTextureDescriptor.pixelFormat = MTLPixelFormatR32Float;
        checkerTextureDescriptor.mipmapLevelCount = 1;
        checkerTextureDescriptor.storageMode = MTLStorageModeShared;
        checkerTextureDescriptor.usage = MTLTextureUsageShaderRead;
        assert(checkerTexture = [metalDevice newTextureWithDescriptor: checkerTextureDescriptor]);
        const float data[2 * 2] = { 1.0f, 0.5f, 0.5f, 1.0f };
        [checkerTexture replaceRegion: MTLRegionMake2D(0, 0, 2, 2) mipmapLevel: 0 withBytes: data bytesPerRow: 4 * 2];
        
        renderTextureDescriptor = [[MTLTextureDescriptor alloc] init];
        renderTextureDescriptor.width = 640;
        renderTextureDescriptor.height = 528;
        renderTextureDescriptor.depth = 1;
        renderTextureDescriptor.textureType = MTLTextureType2D;
        renderTextureDescriptor.pixelFormat = MTLPixelFormatRGBA8Unorm;
        renderTextureDescriptor.mipmapLevelCount = 1;
        renderTextureDescriptor.storageMode = MTLStorageModeShared;
        renderTextureDescriptor.usage = MTLTextureUsageRenderTarget | MTLTextureUsageShaderRead;
        
        renderBuffer = [metalDevice newBufferWithLength: 640 * 528 * sizeof(uint32_t) options: MTLResourceStorageModeShared];
        assert(renderTexture = [renderBuffer newTextureWithDescriptor: renderTextureDescriptor offset: 0 bytesPerRow: 640 * sizeof(uint32_t)]);
        
      
        //graphics::createTexture("Render Texture", 640, 528, 1);
    }
    
    bool initialize()
    {
      initializePrimitives();
      
        NSError* error = nullptr;
        
        metalDevice = MTLCreateSystemDefaultDevice();
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
        
        return true;
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
      bool useFaceNormals;
      bool enableShading;
    } uniform;
    
    static simd_float4x4 GameToMetalMatrix(r3::stMatrix4D mat)
    {
        simd_float4x4 m;
        m.columns[0] = simd_make_float4(mat.m00, mat.m01, mat.m02, mat.m03);
        m.columns[1] = simd_make_float4(mat.m10, mat.m11, mat.m12, mat.m13);
        m.columns[2] = simd_make_float4(mat.m20, mat.m21, mat.m22, mat.m23);
        m.columns[3] = simd_make_float4(mat.m30, mat.m31, mat.m32, mat.m33);

        return m;
    }
//
//    static simd::float3x3 getNormalMatrix(simd::float4x4 view, simd::float4x4 model)
//    {
//        simd::float4x4 MV = view * model;
//        simd::float3x3 normalMatrix = { MV.columns[0].xyz, MV.columns[1].xyz, MV.columns[2].xyz };
//        return simd::transpose(simd::inverse(normalMatrix));
//    }
    
  auto beginFrame(float w, float h) -> void {
    commandBuffer = [commandQueue commandBuffer];
    
    renderpassDescriptor.colorAttachments[0].clearColor = MTLClearColorMake(0.0f, 0.0f, 0.0f, 1.0f);
    renderpassDescriptor.colorAttachments[0].texture = renderTexture;
    renderpassDescriptor.colorAttachments[0].loadAction = MTLLoadActionClear;
    renderpassDescriptor.colorAttachments[0].storeAction = MTLStoreActionStore;
      
    //renderpassDescriptor.depthAttachment.texture = depthTexture;
    renderpassDescriptor.depthAttachment.clearDepth = 1.0f;
    renderpassDescriptor.depthAttachment.loadAction = MTLLoadActionClear;
    renderpassDescriptor.depthAttachment.storeAction = MTLStoreActionStore;
    
//      renderpassDescriptor.stencilAttachment.texture = depthTexture;
//      renderpassDescriptor.stencilAttachment.loadAction = MTLLoadActionClear;
//      renderpassDescriptor.stencilAttachment.storeAction = MTLStoreActionStore;
        
    renderEncoder = [commandBuffer renderCommandEncoderWithDescriptor: renderpassDescriptor];
    [renderEncoder pushDebugGroup: @"r3-render"];
    [renderEncoder setCullMode: MTLCullModeBack];
    [renderEncoder setFrontFacingWinding: MTLWindingCounterClockwise];
    [renderEncoder setRenderPipelineState: pipelineState];
    [renderEncoder setDepthStencilState: depthState];
    [renderEncoder setViewport: (MTLViewport){0, 0, static_cast<double>(w), static_cast<double>(h), 0.0f, 1.0f}];
        
    uniform.view = GameToMetalMatrix(graphics::viewMatrix);
    uniform.projection = GameToMetalMatrix(graphics::projectionMatrix);
    uniform.model = matrix_identity_float4x4;
    uniform.color = simd_make_float4(1.0f, 1.0f, 1.0f, 1.0f);
    uniform.use_texture = true;
    uniform.useGameIndexing = false;
  }
    
  auto drawPoint(r3::stVector3D point, r3::stVector4D color) -> void {
    uniform.useGameIndexing = false;
    uniform.color = simd_make_float4(color.x, color.y, color.z, color.w);
    uniform.use_texture = false;
    uniform.enableShading = false;
    uniform.model = GameToMetalMatrix(r3::stMatrix4D());
    
    id<MTLBuffer> vertexBuffer = [metalDevice newBufferWithBytes: (void*)&point length: sizeof point options: MTLResourceStorageModeShared];
    id<MTLBuffer> uniformBuffer = [metalDevice newBufferWithBytes: (void*)&uniform length: sizeof uniform options: MTLResourceStorageModeShared];
    [renderEncoder setVertexBuffer: vertexBuffer offset: 0 atIndex: 0];
    [renderEncoder setVertexBuffer: uniformBuffer offset: 0 atIndex: 3];
    [renderEncoder setFragmentBuffer: uniformBuffer offset: 0 atIndex: 0];
    [renderEncoder drawPrimitives: MTLPrimitiveTypePoint vertexStart: 0 vertexCount: 1];
        
    [vertexBuffer release];
    [uniformBuffer release];
    
    uniform.color = simd_make_float4(1.0f, 1.0f, 1.0f, 1.0f);
    uniform.enableShading = true;
  }
  
//  auto drawSphere(r3::stVector3D point, float radius, r3::stVector4D color) -> void {
//    uniform.color = simd_make_float4(1.0f, 1.0f, 1.0f, 0.5f);
//    uniform.use_texture = false;
//    
//    r3::stMatrix4D matrix = r3::stMatrix4D();
//    matrix.m00 = radius * 2.0f;
//    matrix.m11 = radius * 2.0f;
//    matrix.m22 = radius * 2.0f;
//    
//    uniform.model = GameToMetalMatrix(matrix);
//    
//    id<MTLBuffer> vertexBuffer = [metalDevice newBufferWithBytes: (void*)&point length: sizeof point options: MTLResourceStorageModeShared];
//    id<MTLBuffer> uniformBuffer = [metalDevice newBufferWithBytes: (void*)&uniform length: sizeof uniform options: MTLResourceStorageModeShared];
//    [renderEncoder setVertexBuffer: vertexBuffer offset: 0 atIndex: 0];
//    [renderEncoder setVertexBuffer: uniformBuffer offset: 0 atIndex: 3];
//    [renderEncoder setFragmentBuffer: uniformBuffer offset: 0 atIndex: 0];
//    [renderEncoder drawPrimitives: MTLPrimitiveTypePoint vertexStart: 0 vertexCount: 1];
//        
//    [vertexBuffer release];
//    [uniformBuffer release];
//  }
  
  void drawLine(r3::stVector3D p1, r3::stVector3D p2, r3::stVector4D color) {
    uniform.color = simd_make_float4(color.x, color.y, color.z, color.w);
    uniform.use_texture = false;
    uniform.useGameIndexing = false;
    
    r3::stVector3D buf[2] = { p1, p2 };
    id<MTLBuffer> vertexBuffer = [metalDevice newBufferWithBytes: (void*)buf length: sizeof buf options: MTLResourceStorageModeShared];
    id<MTLBuffer> uniformBuffer = [metalDevice newBufferWithBytes: (void*)&uniform length: sizeof uniform options: MTLResourceStorageModeShared];
    [renderEncoder setVertexBuffer: vertexBuffer offset: 0 atIndex: 0];
    [renderEncoder setVertexBuffer: uniformBuffer offset: 0 atIndex: 3];
    [renderEncoder setFragmentBuffer: uniformBuffer offset: 0 atIndex: 0];
    [renderEncoder drawPrimitives: MTLPrimitiveTypeLine vertexStart: 0 vertexCount: 2];
    
    [vertexBuffer release];
    [uniformBuffer release];
  }
  
  auto draw(r3::stVector3D *vertices, r3::stVector3D *normals, r3::uint16 *indices, unsigned numVertices, unsigned numIndices, r3::stMatrix4D T, r3::stVector4D color) -> void {
    uniform.useGameIndexing = true;
    uniform.model = GameToMetalMatrix(T);
    uniform.useFaceNormals = false;
    uniform.color = simd_make_float4(color.x, color.y, color.z, color.w);
    
    id<MTLBuffer> indexBuffer = [metalDevice newBufferWithBytes: indices length: numIndices * sizeof(r3::uint16) options: MTLResourceStorageModePrivate];
    id<MTLBuffer> normalBuffer = [metalDevice newBufferWithBytes: normals length: numVertices * sizeof(r3::stVector3D) options: MTLResourceStorageModePrivate];
    id<MTLBuffer> vertexBuffer = [metalDevice newBufferWithBytes: vertices length: numVertices * sizeof(r3::stVector3D) options: MTLResourceStorageModePrivate];
    id<MTLBuffer> uniformBuffer = [metalDevice newBufferWithBytes: &uniform length: sizeof uniform options: MTLResourceStorageModePrivate];
    
    [renderEncoder setVertexBuffer: vertexBuffer offset: 0 atIndex: 0];
    [renderEncoder setVertexBuffer: normalBuffer offset: 0 atIndex: 1];
    [renderEncoder setVertexBuffer: indexBuffer offset: 0 atIndex: 2];
    [renderEncoder setVertexBuffer: uniformBuffer offset: 0 atIndex: 3];
    [renderEncoder setFragmentBuffer: uniformBuffer offset: 0 atIndex: 0];
    //[renderEncoder setFragmentTexture: checkerTexture atIndex: 0];
    [renderEncoder drawPrimitives: MTLPrimitiveTypeTriangle vertexStart: 0 vertexCount: numIndices];
    
    [indexBuffer release];
    [normalBuffer release];
    [vertexBuffer release];
    [uniformBuffer release];
    
    uniform.useFaceNormals = true;
    uniform.useGameIndexing = false;
    uniform.model = GameToMetalMatrix(r3::stMatrix4D());
  }
  
  auto drawIPO(r3::stInstantiatedPhysicalObject *ipo, r3::stMatrix4D T) -> void {
    r3::stCollideObject *zdr = nullptr;
    if ((zdr = ipo->zdr())) {
      zdr->forEachElement([&](r3::int16 type, void *m) {
        if (type == collideElementIndexedTriangles && m) {
          r3::stCollideElementIndexedTriangles *mesh = static_cast<r3::stCollideElementIndexedTriangles*>(m);
          
          r3::stVector3D* vertices = zdr->vertices;
          r3::stVector3D* normals = mesh->normals;
          r3::uint16* indices = mesh->faceIndices;
          
          r3::int16 numVertices = zdr->numVertices;
          r3::int16 numFaces = mesh->numFaces;

          //uniform.color = collideMaterialGetColor(material);
          uniform.model = GameToMetalMatrix(T);
          uniform.useGameIndexing = true;
          uniform.use_texture = true;
          uniform.useFaceNormals = true;
          
          if (highlightList.size() != 0) {
            uniform.color.w = 0.1f;
          }
          
          for (r3::pointer<>& p : highlightList) {
            if (ipo == p) {
              uniform.color.w = 1.0f;
            }
          }
          
          //uniform.normal_matrix = getNormalMatrix(uniform.view, uniform.model);
          id<MTLBuffer> indexBuffer = [metalDevice newBufferWithBytes: indices length: int(numFaces) * 3 * sizeof(r3::uint16) options: MTLResourceStorageModePrivate];
          id<MTLBuffer> normalBuffer = [metalDevice newBufferWithBytes: normals length: int(numFaces) * sizeof(r3::stVector3D) options: MTLResourceStorageModePrivate];
          id<MTLBuffer> vertexBuffer = [metalDevice newBufferWithBytes: vertices length: int(numVertices) * sizeof(r3::stVector3D) options: MTLResourceStorageModePrivate];
          id<MTLBuffer> uniformBuffer = [metalDevice newBufferWithBytes: &uniform length: sizeof uniform options: MTLResourceStorageModePrivate];
          
          [renderEncoder setVertexBuffer: vertexBuffer offset: 0 atIndex: 0];
          [renderEncoder setVertexBuffer: normalBuffer offset: 0 atIndex: 1];
          [renderEncoder setVertexBuffer: indexBuffer offset: 0 atIndex: 2];
          [renderEncoder setVertexBuffer: uniformBuffer offset: 0 atIndex: 3];
          [renderEncoder setFragmentBuffer: uniformBuffer offset: 0 atIndex: 0];
          [renderEncoder setFragmentTexture: checkerTexture atIndex: 0];
          [renderEncoder drawPrimitives: MTLPrimitiveTypeTriangle vertexStart: 0 vertexCount: int(numFaces) * 3];

          [indexBuffer release];
          [normalBuffer release];
          [vertexBuffer release];
          [uniformBuffer release];

          uniform.useGameIndexing = false;
        }
      });

//                int mesh_idx = 0;
//                const stCollideElementIndexedTriangles* mesh;
//                while ((mesh = fnCollideObjectGetElementIndexedTriangles(zdr, mesh_idx)))
//                {
//                    uint16* indices = (uint16*)pointer(mesh->faceIndices);
//                    stVector3D* vertices = (stVector3D*)pointer(zdr->vertices);
//                    stVector3D* normals = (stVector3D*)pointer(mesh->normals);
//
//                    if (vertices && indices && normals)
//                    {
//                        const int16 n_faces = host_byteorder_16(mesh->numFaces);
//
//
//                        stCollideMaterial *material = (stCollideMaterial*)pointer(mesh->material);
//
//                        //uniform.color = collideMaterialGetColor(material);
//                        uniform.model = GameToMetalMatrix(T);
//                        uniform.normal_matrix = getNormalMatrix(uniform.view, uniform.model);
//                        id<MTLBuffer> uniformBuffer = [device newBufferWithBytes: &uniform length: sizeof uniform options: MTLResourceStorageModePrivate];
//
//                        /* Cannot use newBufferWithBytesNoCopy here, since data is not page-aligned. Mark the buffers as private: they aren't written to by the CPU. */
//                        id<MTLBuffer> indexBuffer = [device newBufferWithBytes: (void*)indices length: n_faces * 3 * sizeof(uint16) options: MTLResourceStorageModePrivate];
//                        id<MTLBuffer> normalBuffer = [device newBufferWithBytes: (void*)normals length: n_faces * sizeof(stVector3D) options: MTLResourceStorageModePrivate];
//                        id<MTLBuffer> vertexBuffer = [device newBufferWithBytes: (void*)vertices length: n_vertices * sizeof(stVector3D) options: MTLResourceStorageModePrivate];
//
//                        [render_encoder setVertexBuffer: vertexBuffer offset: 0 atIndex: 0];
//                        [render_encoder setVertexBuffer: normalBuffer offset: 0 atIndex: 1];
//                        [render_encoder setVertexBuffer: indexBuffer offset: 0 atIndex: 2];
//                        [render_encoder setVertexBuffer: uniformBuffer offset: 0 atIndex: 3];
//                        [render_encoder setFragmentTexture: checker_texture atIndex: 0];
//                        [render_encoder setFragmentBuffer: uniformBuffer offset: 0 atIndex: 0];
//
//                        [render_encoder drawPrimitives: MTLPrimitiveTypeTriangle vertexStart: 0 vertexCount: n_faces * 3];
//
//                        [indexBuffer release];
//                        [normalBuffer release];
//                        [vertexBuffer release];
//                        [uniformBuffer release];
//                    }
//
//                    mesh_idx++;
//                }
        }
    }
    
  auto endFrame() -> void {
    [renderEncoder popDebugGroup];
    [renderEncoder endEncoding];
    [commandBuffer commit];
    [commandBuffer waitUntilCompleted];
  }
    
  auto texture() -> void* {
//    uint32_t W = (uint32_t)renderTexture.width;
//    uint32_t H = (uint32_t)renderTexture.width;
//    uint8_t *data = (uint8_t*)renderTexture.buffer.contents;
//    size_t length = renderTexture.buffer.length;
//    graphics::updateTexture("Render Texture", W, H, W, data, length);
        
    //[commandBuffer waitUntilCompleted];
    
    static struct MetalTextureWrapper {
      uint32_t width;
      uint32_t height = 0;
      uint32_t levels = 0;
      uint32_t layers = 1;
      uint32_t samples = 1;
      uint32_t format = 0;
      uint32_t flags = 0;
      uint32_t type = 0;
      uint32_t typ2e = 0;
      void *pointer;
      /* 8 bytes of padding */
    } *data = (struct MetalTextureWrapper*)malloc(sizeof(struct MetalTextureWrapper));
    
    memset(data, 0, sizeof(struct MetalTextureWrapper));
    data->levels = 1;
    data->layers = 1;
    data->samples = 1;
    
    data->width = 640; //()renderTexture.width;
    data->height = 528; //renderTexture.height;
    //data->format =
    data->pointer = renderTexture;

        
    return data;  //graphics::getTexture("Render Texture");
  }
};

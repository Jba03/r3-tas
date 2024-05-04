#include "metal.hh"
 
#include <iomanip>
#include <iostream>
#include <vector>

#include "game.hh"
#include "graphics.hh"

namespace r3 = cpa;

static auto GameToMetalMatrix(stMatrix4D mat) -> simd_float4x4 {
  simd_float4x4 m;
  m.columns[0] = simd_make_float4(mat(0,0), mat(0,1), mat(0,2), mat(0,3));
  m.columns[1] = simd_make_float4(mat(1,0), mat(1,1), mat(1,2), mat(1,3));
  m.columns[2] = simd_make_float4(mat(2,0), mat(2,1), mat(2,2), mat(2,3));
  m.columns[3] = simd_make_float4(mat(3,0), mat(3,1), mat(3,2), mat(3,3));
  return m;
}

MetalGraphicsContext::MetalGraphicsContext() {
  
  metalDevice = MTLCreateSystemDefaultDevice();
  commandQueue = [metalDevice newCommandQueue];
  renderpassDescriptor = [MTLRenderPassDescriptor new];
  
  if (!loadShaderLibrary()) return;
  if (!createTextures()) return;
  
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
  pipelineStateDescriptor.colorAttachments[0].destinationAlphaBlendFactor = MTLBlendFactorOne;
  pipelineStateDescriptor.vertexDescriptor = vertexDescriptor;
  pipelineStateDescriptor.depthAttachmentPixelFormat = MTLPixelFormatDepth32Float_Stencil8;
  pipelineStateDescriptor.stencilAttachmentPixelFormat = MTLPixelFormatDepth32Float_Stencil8;
  
  NSError* error = nullptr;
  if (!(pipelineState = [metalDevice newRenderPipelineStateWithDescriptor: pipelineStateDescriptor error: &error])) {
    std::cerr << "failed to create pipeline state: " << error.localizedDescription.UTF8String << "\n";
  }
  
  // Initialize the model matrix
  uniforms.model = matrix_identity_float4x4;
  
  std::cout << "[graphics-metal] context created\n";
}

static auto shaderFilePath() -> NSString* {
  return [NSString stringWithFormat: @"%s%@", LIBR3TAS_DIR, @"/video/3d/main.metal"];
}

auto MetalGraphicsContext::loadShaderLibrary() -> bool {
  NSError *error = nullptr;
  NSString *source = [NSString stringWithContentsOfFile: shaderFilePath() encoding: NSUTF8StringEncoding error: &error];
  shaderLibrary = [metalDevice newLibraryWithSource: source options: nullptr error: &error];
  if (error) {
    std::cerr << "error: " << error.localizedDescription << "\n";
    return false;
  } else {
    return true;
  }
}

auto MetalGraphicsContext::createTextures() -> bool {
  MTLTextureDescriptor *depthTextureDescriptor = nullptr;
  MTLTextureDescriptor *checkerTextureDescriptor = nullptr;
  MTLTextureDescriptor *renderTextureDescriptor = nullptr;
  
  depthTextureDescriptor = [[MTLTextureDescriptor alloc] init];
  depthTextureDescriptor.width = 640*6;
  depthTextureDescriptor.height = 528*6;
  depthTextureDescriptor.pixelFormat = MTLPixelFormatDepth32Float_Stencil8;
  depthTextureDescriptor.mipmapLevelCount = 1;
  depthTextureDescriptor.storageMode = MTLStorageModeShared;
  depthTextureDescriptor.usage = MTLTextureUsageRenderTarget | MTLTextureUsageShaderRead;
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
  renderTextureDescriptor.width = 640*6;
  renderTextureDescriptor.height = 528*6;
  renderTextureDescriptor.depth = 1;
  //renderTextureDescriptor.sampleCount = 2;
  renderTextureDescriptor.textureType = MTLTextureType2DArray;
  renderTextureDescriptor.pixelFormat = MTLPixelFormatRGBA8Unorm;
  renderTextureDescriptor.mipmapLevelCount = 1;
  renderTextureDescriptor.storageMode = MTLStorageModeShared;
  renderTextureDescriptor.usage = MTLTextureUsageRenderTarget | MTLTextureUsageShaderRead;
    
  assert(renderTexture = [metalDevice newTextureWithDescriptor: renderTextureDescriptor]);
  return true;
}



auto MetalGraphicsContext::beginFrame(void* subtex) -> void {
  commandBuffer = [commandQueue commandBuffer];
  
  //gameTexture = ((DolphinAbstractTexture<id<MTLTexture>>*)subtex)->data;
  renderpassDescriptor.colorAttachments[0].clearColor = MTLClearColorMake(0.0f, 0.0f, 0.0f, 1.0f);
  renderpassDescriptor.colorAttachments[0].texture = renderTexture;
  renderpassDescriptor.colorAttachments[0].loadAction = MTLLoadActionClear;
  renderpassDescriptor.colorAttachments[0].storeAction = MTLStoreActionStore;
    
  renderpassDescriptor.depthAttachment.texture = depthTexture;
  renderpassDescriptor.depthAttachment.clearDepth = 1.0f;
  renderpassDescriptor.depthAttachment.loadAction = MTLLoadActionClear;
  renderpassDescriptor.depthAttachment.storeAction = MTLStoreActionStore;
  
  renderpassDescriptor.stencilAttachment.texture = depthTexture;
  renderpassDescriptor.stencilAttachment.loadAction = MTLLoadActionClear;
  renderpassDescriptor.stencilAttachment.storeAction = MTLStoreActionStore;
  
  renderEncoder = [commandBuffer renderCommandEncoderWithDescriptor: renderpassDescriptor];
  [renderEncoder pushDebugGroup: @"r3-render"];
  [renderEncoder setCullMode: MTLCullModeBack];
  [renderEncoder setFrontFacingWinding: MTLWindingCounterClockwise];
  [renderEncoder setRenderPipelineState: pipelineState];
  [renderEncoder setDepthStencilState: depthState];
  [renderEncoder setViewport: (MTLViewport){0, 0, static_cast<double>(renderTexture.width), static_cast<double>(renderTexture.height), 0.0f, 1.0f}];
  assert(renderEncoder);
}

// Draw host primitives
auto MetalGraphicsContext::draw(const PrimitiveType primitive, uint32_t vertexCount, stVector3D* vertices) -> void {
  setUseGameIndexing(false);
  
  id<MTLBuffer> vertexBuffer = [metalDevice newBufferWithBytes: vertices length: vertexCount * sizeof(stVector3D) options: MTLResourceStorageModeShared];
  id<MTLBuffer> uniformBuffer = [metalDevice newBufferWithBytes: &uniforms length: sizeof uniforms options: MTLResourceStorageModeShared];
  
  //printf("draw: %.2f %.2f %.2f\n", vertices[0], vertices[1], vertices[2]);
  //printf("draw: %.2f %.2f %.2f\n\n", vertices[3], vertices[4], vertices[5]);
  
  MTLPrimitiveType primitiveType = MTLPrimitiveTypeTriangle;
  if (primitive == Points) primitiveType = MTLPrimitiveTypePoint;
  if (primitive == Lines) primitiveType = MTLPrimitiveTypeLineStrip;
  if (primitive == Triangles) primitiveType= MTLPrimitiveTypeTriangle;
  
  [renderEncoder setVertexBuffer: vertexBuffer offset: 0 atIndex: 0];
  [renderEncoder setVertexBuffer: uniformBuffer offset: 0 atIndex: 3];
  [renderEncoder setFragmentBuffer: uniformBuffer offset: 0 atIndex: 0];
  //[renderEncoder setFragmentTexture: checkerTexture atIndex: 0];
  [renderEncoder drawPrimitives: primitiveType vertexStart: 0 vertexCount: vertexCount];
  
  [vertexBuffer release];
  [uniformBuffer release];
}


auto MetalGraphicsContext::drawIndexed(const PrimitiveType primitive, uint32_t vertexCount, stVector3D* vertices, stVector3D* normals, uint32_t indexCount, void* indices) -> void {
  bool gameIndexing = uniforms.useGameIndexing;
  size_t indexSize = uniforms.useGameIndexing ? sizeof(Index3D) : sizeof(uint32_t);
  uniforms.cameraPosition.x = game::g_stEngineStructure->standardCamera->position().x;
  uniforms.cameraPosition.y = game::g_stEngineStructure->standardCamera->position().y;
  uniforms.cameraPosition.z = game::g_stEngineStructure->standardCamera->position().z;
  
  
  id<MTLBuffer> normalBuffer = nullptr;
  id<MTLBuffer> indexBuffer = nullptr;
  id<MTLBuffer> vertexBuffer = [metalDevice newBufferWithBytes: vertices length: vertexCount * sizeof(stVector3D) options: MTLResourceStorageModeShared];
  id<MTLBuffer> uniformBuffer = [metalDevice newBufferWithBytes: &uniforms length: sizeof uniforms options: MTLResourceStorageModeShared];
  indexBuffer = [metalDevice newBufferWithBytes: indices length: indexCount * indexSize options: MTLResourceStorageModeShared];
  if (normals) normalBuffer = [metalDevice newBufferWithBytes: normals length: indexCount * sizeof(stVector3D) options: MTLResourceStorageModeShared];
  
  MTLPrimitiveType primitiveType = MTLPrimitiveTypeTriangle;
  if (primitive == Points) primitiveType = MTLPrimitiveTypePoint;
  if (primitive == Lines) primitiveType = MTLPrimitiveTypeLine;
  if (primitive == Triangles) primitiveType = MTLPrimitiveTypeTriangle;
  
  [renderEncoder setVertexBuffer: vertexBuffer offset: 0 atIndex: 0];
  if (normals) [renderEncoder setVertexBuffer: normalBuffer offset: 0 atIndex: 1];
  if (gameIndexing) [renderEncoder setVertexBuffer: indexBuffer offset: 0 atIndex: 2];
  [renderEncoder setVertexBuffer: uniformBuffer offset: 0 atIndex: 3];
  [renderEncoder setFragmentBuffer: uniformBuffer offset: 0 atIndex: 0];
  [renderEncoder setFragmentTexture: checkerTexture atIndex: 0];
  //[renderEncoder setTriangleFillMode: MTLTriangleFillModeFill];
  
  if (gameIndexing) {
    [renderEncoder setTriangleFillMode: MTLTriangleFillModeFill];
    [renderEncoder drawPrimitives: primitiveType vertexStart: 0 vertexCount: indexCount];
   // [renderEncoder setTriangleFillMode: MTLTriangleFillModeLines];
//    [uniformBuffer release];
//    uniforms.useShading = false;
//    uniforms.color = simd_make_float4(0.35f, 0.35f, 0.35f, 1.0f);
//    uniformBuffer = [metalDevice newBufferWithBytes: &uniforms length: sizeof uniforms options: MTLResourceStorageModeShared];
//    [renderEncoder setFragmentBuffer: uniformBuffer offset: 0 atIndex: 0];
//    [renderEncoder drawPrimitives: primitiveType vertexStart: 0 vertexCount: indexCount];
//    uniforms.useShading = true;
    
  } else {
    [renderEncoder drawIndexedPrimitives: primitiveType indexCount: indexCount indexType: MTLIndexTypeUInt32 indexBuffer: indexBuffer indexBufferOffset: 0];
  }
    
  [vertexBuffer release];
  [uniformBuffer release];
  [indexBuffer release];
  if (normals) [normalBuffer release];
}

auto MetalGraphicsContext::setViewMatrix(const stMatrix4D& view) -> void {
  uniforms.view = GameToMetalMatrix(view);
}

auto MetalGraphicsContext::setProjectionMatrix(const stMatrix4D& projection) -> void {
  uniforms.projection = GameToMetalMatrix(projection);
}

auto MetalGraphicsContext::setModelMatrix(const stMatrix4D& model) -> void {
  uniforms.model = GameToMetalMatrix(model);
}

auto MetalGraphicsContext::setUseGameIndexing(bool value) -> void {
  uniforms.useGameIndexing = value;
}

auto MetalGraphicsContext::setUseCheckerTexture(bool value) -> void {
  uniforms.useCheckerTexture = value;
}

auto MetalGraphicsContext::setUseFaceNormals(bool value) -> void {
  uniforms.useFaceNormals = value;
}

auto MetalGraphicsContext::setDisableShading(bool value) -> void {
  uniforms.useShading = !value;
}

auto MetalGraphicsContext::setColor(const stVector4D& color) -> void {
  uniforms.color[0] = color.x;
  uniforms.color[1] = color.y;
  uniforms.color[2] = color.z;
  uniforms.color[3] = color.w;
}

auto MetalGraphicsContext::setCameraPosition(const stVector3D& pos) -> void {
  uniforms.cameraPosition[0] = pos.x;
  uniforms.cameraPosition[1] = pos.y;
  uniforms.cameraPosition[2] = pos.z;
}
    
auto MetalGraphicsContext::endFrame() -> void {
  [renderEncoder popDebugGroup];
  [renderEncoder endEncoding];
  [commandBuffer commit];
  [commandBuffer waitUntilCompleted];
}

auto MetalGraphicsContext::texture() -> void* {
  memset(&abstractRenderTexture, 0, sizeof(DolphinAbstractTexture<id<MTLTexture>>));
  abstractRenderTexture.levels = 1;
  abstractRenderTexture.layers = 1;
  abstractRenderTexture.samples = 1;
  abstractRenderTexture.width = renderTexture.width;
  abstractRenderTexture.height = renderTexture.height;
  abstractRenderTexture.data = renderTexture;
  return &abstractRenderTexture;
}
    
   
    
    
//
//    static simd::float3x3 getNormalMatrix(simd::float4x4 view, simd::float4x4 model)
//    {
//        simd::float4x4 MV = view * model;
//        simd::float3x3 normalMatrix = { MV.columns[0].xyz, MV.columns[1].xyz, MV.columns[2].xyz };
//        return simd::transpose(simd::inverse(normalMatrix));
//    }
    
//  auto drawPoint(stVector3D point, stVector4D color) -> void {
//    uniform.useGameIndexing = false;
//    uniform.color = simd_make_float4(color.x, color.y, color.z, color.w);
//    uniform.use_texture = false;
//    uniform.enableShading = false;
//    uniform.model = GameToMetalMatrix(stMatrix4D());
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
//
//    uniform.color = simd_make_float4(1.0f, 1.0f, 1.0f, 1.0f);
//    uniform.enableShading = true;
//  }
  
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
  
//  void drawLine(stVector3D p1, stVector3D p2, stVector4D color) {
//    uniform.color = simd_make_float4(color.x, color.y, color.z, color.w);
//    uniform.use_texture = false;
//    uniform.useGameIndexing = false;
//
//    stVector3D buf[2] = { p1, p2 };
//    id<MTLBuffer> vertexBuffer = [metalDevice newBufferWithBytes: (void*)buf length: sizeof buf options: MTLResourceStorageModeShared];
//    id<MTLBuffer> uniformBuffer = [metalDevice newBufferWithBytes: (void*)&uniform length: sizeof uniform options: MTLResourceStorageModeShared];
//    //printf("renderencoder: %p\n", renderEncoder);
//   // if (renderEncoder) {
//      //printf("renderencoder: %p\n", renderEncoder);
//      [renderEncoder setVertexBuffer: vertexBuffer offset: 0 atIndex: 0];
//      [renderEncoder setVertexBuffer: uniformBuffer offset: 0 atIndex: 3];
//      [renderEncoder setFragmentBuffer: uniformBuffer offset: 0 atIndex: 0];
//      [renderEncoder drawPrimitives: MTLPrimitiveTypeLine vertexStart: 0 vertexCount: 2];
//    //}
//
//    [vertexBuffer release];
//    [uniformBuffer release];
//  }
  
//  auto draw(stVector3D *vertices, stVector3D *normals, r3::uint16 *indices, unsigned numVertices, unsigned numIndices, stMatrix4D T, stVector4D color) -> void {
//    uniform.useGameIndexing = true;
//    uniform.model = GameToMetalMatrix(T);
//    uniform.useFaceNormals = false;
//    uniform.color = simd_make_float4(color.x, color.y, color.z, color.w);
//
//    id<MTLBuffer> indexBuffer = [metalDevice newBufferWithBytes: indices length: numIndices * sizeof(r3::uint16) options: MTLResourceStorageModePrivate];
//    id<MTLBuffer> normalBuffer = [metalDevice newBufferWithBytes: normals length: numVertices * sizeof(stVector3D) options: MTLResourceStorageModePrivate];
//    id<MTLBuffer> vertexBuffer = [metalDevice newBufferWithBytes: vertices length: numVertices * sizeof(stVector3D) options: MTLResourceStorageModePrivate];
//    id<MTLBuffer> uniformBuffer = [metalDevice newBufferWithBytes: &uniform length: sizeof uniform options: MTLResourceStorageModePrivate];
//
//    [renderEncoder setVertexBuffer: vertexBuffer offset: 0 atIndex: 0];
//    [renderEncoder setVertexBuffer: normalBuffer offset: 0 atIndex: 1];
//    [renderEncoder setVertexBuffer: indexBuffer offset: 0 atIndex: 2];
//    [renderEncoder setVertexBuffer: uniformBuffer offset: 0 atIndex: 3];
//    [renderEncoder setFragmentBuffer: uniformBuffer offset: 0 atIndex: 0];
//    //[renderEncoder setFragmentTexture: checkerTexture atIndex: 0];
//    [renderEncoder drawPrimitives: MTLPrimitiveTypeTriangle vertexStart: 0 vertexCount: numIndices];
//
//    [indexBuffer release];
//    [normalBuffer release];
//    [vertexBuffer release];
//    [uniformBuffer release];
//
//    uniform.useFaceNormals = true;
//    uniform.useGameIndexing = false;
//    uniform.model = GameToMetalMatrix(stMatrix4D());
//  }
//

//auto drawIPO(stInstantiatedPhysicalObject *ipo, stMatrix4D T) -> void {
//  try {
//    pointer<stCollideObject> zdr = ipo->physicalObject->physicalCollideset->zdr;
//    for (int i = 0; i < zdr->numElements; i++) {
//      int16_t type = zdr->elementTypes[i];
//      if (type == stCollideObject::type::IndexedTriangles) {
//        pointer<stCollideElementIndexedTriangles> mesh = zdr->elements[i];
//        if (!mesh) continue;
//
//        stVector3D* vertices = zdr->vertices;
//        stVector3D* normals = mesh->normals;
//        r3::uint16* indices = mesh->faceIndices;
//
//        r3::int16 numVertices = zdr->numVertices;
//        r3::int16 numFaces = mesh->numFaces;
//
//        pointer<stCollideMaterial> material = mesh->material;
//
//        stVector4D mat = getCollideMaterialColor(material);
//        simd_float4 col = {mat.x, mat.y, mat.z, mat.w};
//
//
//        uniform.color = col;
//        uniform.model = GameToMetalMatrix(T);
//        uniform.useGameIndexing = true;
//        uniform.use_texture = true;
//        uniform.useFaceNormals = true;
//
//        id<MTLBuffer> indexBuffer = [metalDevice newBufferWithBytes: indices length: int(numFaces) * 3 * sizeof(r3::uint16) options: MTLResourceStorageModeShared];
//        id<MTLBuffer> normalBuffer = [metalDevice newBufferWithBytes: normals length: int(numFaces) * sizeof(stVector3D) options: MTLResourceStorageModeShared];
//        id<MTLBuffer> vertexBuffer = [metalDevice newBufferWithBytes: vertices length: int(numVertices) * sizeof(stVector3D) options: MTLResourceStorageModeShared];
//        id<MTLBuffer> uniformBuffer = [metalDevice newBufferWithBytes: &uniform length: sizeof uniform options: MTLResourceStorageModeShared];
//
//        [renderEncoder setVertexBuffer: vertexBuffer offset: 0 atIndex: 0];
//        [renderEncoder setVertexBuffer: normalBuffer offset: 0 atIndex: 1];
//        [renderEncoder setVertexBuffer: indexBuffer offset: 0 atIndex: 2];
//        [renderEncoder setVertexBuffer: uniformBuffer offset: 0 atIndex: 3];
//        [renderEncoder setFragmentBuffer: uniformBuffer offset: 0 atIndex: 0];
//        [renderEncoder setFragmentTexture: checkerTexture atIndex: 0];
//        [renderEncoder drawPrimitives: MTLPrimitiveTypeTriangle vertexStart: 0 vertexCount: int(numFaces) * 3];
//
//        [indexBuffer release];
//        [normalBuffer release];
//        [vertexBuffer release];
//        [uniformBuffer release];
//
//        uniform.useGameIndexing = false;
//      }
//    }
//  } catch (...) {
//
//  }
//}

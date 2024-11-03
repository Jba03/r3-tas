#pragma once

#include "graphics.hh"

#import <Metal/Metal.h>
#import <MetalKit/MetalKit.h>
#import <Cocoa/Cocoa.h>

struct MetalGraphicsContext : GraphicsContext {
  MetalGraphicsContext();
  
  struct Uniform {
    simd_float4x4 view;
    simd_float4x4 projection;
    simd_float4x4 model;
    simd_float3 cameraPosition;
    simd_float4 color;
    simd_bool useCheckerTexture;
    simd_bool useGameIndexing;
    simd_bool useFaceNormals;
    simd_bool useShading;
  };
  
  // Draw
  auto beginFrame(void* subtex) -> void;
  auto draw(const PrimitiveType primitive, uint32_t vertexCount, cpa::structure::stVector3D* vertices) -> void;
  auto drawIndexed(const PrimitiveType primitive, uint32_t vertexCount, cpa::structure::stVector3D* vertices, cpa::structure::stVector3D* normals, uint32_t indexCount, void* indices) -> void;
  auto endFrame() -> void;
  auto texture() -> void*;
  
  // Uniform
  auto setViewMatrix(const cpa::structure::stMatrix4D& view) -> void;
  auto setProjectionMatrix(const cpa::structure::stMatrix4D& projection) -> void;
  auto setModelMatrix(const cpa::structure::stMatrix4D& model) -> void;
  auto setUseGameIndexing(bool value) -> void;
  auto setUseCheckerTexture(bool value) -> void;
  auto setUseFaceNormals(bool value) -> void;
  auto setDisableShading(bool value) -> void;
  auto setColor(cpa::structure::stVector4D color) -> void;
  auto setCameraPosition(cpa::structure::stVector3D pos) -> void;
  
  // Primitives
  auto drawPoint(cpa::structure::stVector3D point) -> void;
  auto drawSphere(cpa::structure::stVector3D point, float radius) -> void;
  auto drawLine(cpa::structure::stVector3D p1, cpa::structure::stVector3D p2, cpa::structure::stVector4D color) -> void;
  
  // Object
  auto drawActor(cpa::structure::stEngineObject *actor) -> void;
  auto drawIPO(cpa::structure::stInstantiatedPhysicalObject *IPO, cpa::structure::stMatrix4D T) -> void;
  auto drawGraph(cpa::structure::stGraph *graph) -> void;
  
private:
  id<MTLDevice> metalDevice = nullptr;
  id<MTLCommandQueue> commandQueue = nullptr;
  id<MTLCommandBuffer> commandBuffer = nullptr;
  id<MTLRenderPipelineState> pipelineState = nullptr;
  id<MTLRenderCommandEncoder> renderEncoder = nullptr;
  id<MTLDepthStencilState> depthState = nullptr;
  id<MTLLibrary> shaderLibrary = nullptr;
  id<MTLTexture> depthTexture = nullptr;
  id<MTLTexture> depthStencilTexture = nullptr;
  id<MTLTexture> checkerTexture = nullptr;
  id<MTLTexture> renderTexture = nullptr;
  MTLVertexDescriptor *vertexDescriptor = nullptr;
  MTLRenderPassDescriptor *renderpassDescriptor = nullptr;
  
  DolphinAbstractTexture<id<MTLTexture>> abstractRenderTexture;
  
  Uniform uniforms;
  
  auto loadShaderLibrary() -> bool;
  auto createTextures() -> bool;
};

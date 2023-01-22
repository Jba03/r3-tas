//
//  main.metal
//  r3-tas
//
//  Created by Jba03 on 2022-12-29.
//

#include <metal_stdlib>
#include <simd/simd.h>

using namespace metal;

struct RasterizerData
{
    // The [[position]] attribute of this member indicates that this value
    // is the clip space position of the vertex when this structure is
    // returned from the vertex function.
    float4 position [[position]];
    
    float3 normal;
    float2 texcoord;
    
    // Since this member does not have a special attribute, the rasterizer
    // interpolates its value with the values of the other triangle vertices
    // and then passes the interpolated value to the fragment shader for each
    // fragment in the triangle.
//    float4 color;
};

struct Vertex
{
    float4 position;
    float4 normal;
    float4 texcoord;
};

struct Uniform
{
    float3 camera_pos;
    matrix_float4x4 view;
    matrix_float4x4 projection;
    matrix_float4x4 model;
};

constexpr sampler linearSampler (address::repeat,
                                 mip_filter::linear,
                                 mag_filter::linear,
                                 min_filter::linear);

constexpr sampler nearestSampler(address::repeat,
                                 min_filter::nearest,
                                 mag_filter::nearest,
                                 mip_filter::none);


vertex RasterizerData vertex_main(uint vertexID [[ vertex_id ]],
                                  constant Vertex *vertices [[ buffer(0) ]],
                                  constant Uniform *uniform [[ buffer(1) ]])
{
    RasterizerData out;

    // Index into the array of positions to get the current vertex.
    // The positions are specified in pixel dimensions (i.e. a value of 100
    // is 100 pixels from the origin).
    //float2 pixelSpacePosition = vertices[vertexID].position.xy;

    // Get the viewport size and cast to float.
    //vector_float2 viewportSize = vector_float2(*viewportSizePointer);
    

    // To convert from positions in pixel space to positions in clip-space,
    //  divide the pixel coordinates by half the size of the viewport.
//    out.position = vector_float4(0.0, 0.0, 0.0, 1.0);
//    out.position.xy = pixelSpacePosition / (viewportSize / 2.0);
    
    
//    "void main() {\n"                                                   \
//    "   position = vec3(model * vec4(aPos, 1.0f));\n"                   \
//    "   normal = aNormal;\n"                                            \
//    "   texcoord = aTexCoord;\n"                                        \
//    "   gl_Position = projection * view * model * vec4(aPos, 1.0f);\n"  \
    
    float3 pos = vertices[vertexID].position.xyz;
    //pos.x = -pos.x;
    
    float4 position = uniform->projection * uniform->view * float4(pos, 1.0f);
    //position.z = pos.z; //1.0-vertices[vertexID].position.z;
    
    //position.x = -position.x;
    
    out.texcoord = vertices[vertexID].texcoord.xy;
    out.position = position;
    out.normal = vertices[vertexID].normal.xyz;
    
    //float4(vertices[vertexID].normal.xyz, 1.0f);
    
    return out;
}

struct FragmentOutput {
    float4 color [[ color(0) ]];
};

constant float3 lightPos = float3(0, 100, 0);

fragment FragmentOutput fragment_main(RasterizerData in [[stage_in]],
                                      texture2d<float> texture [[ texture(0) ]],
                                      constant Uniform *uniform [[ buffer(0) ]])
{
    FragmentOutput out;
    
    float4 color = texture.sample(nearestSampler, in.texcoord, 0); //use_texture ? texture(checkerboard, texcoord * 4).rgb : vec3(1);
    float3 normal = normalize(in.normal);
    
    float3 lightColor = float3(1,1,1);
    float3 ambient = float3(0.25);
    float3 lightDir = normalize(lightPos - in.position.xyz);
    
    float diff = max(dot(lightDir, normal), 0.0);
    float3 diffuse = diff * lightColor;
    float3 viewDir = normalize(uniform->camera_pos.xyz - in.position.xyz);
    float3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    float3 halfwayDir = normalize(lightDir + viewDir);
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    float3 specular = spec * lightColor * 1.0f;
    
    out.color = float4(float3(diffuse + specular + ambient) * color.xyz, 1.0f) * 1;
    //out.depth = 0.5f;
    
    return out;
}

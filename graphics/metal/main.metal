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
    
    float3 eye;
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
    matrix_float3x3 normal_matrix;
    float4 color;
    bool use_texture;
};

constexpr sampler linearSampler (address::repeat,
                                 mip_filter::linear,
                                 mag_filter::linear,
                                 min_filter::linear);

constexpr sampler nearestSampler(address::repeat,
                                 min_filter::nearest,
                                 mag_filter::nearest,
                                 mip_filter::none);

#define swap16(data) \
    ((((data) >> 8) & 0x00FF) | (((data) << 8) & 0xFF00))

#define swap32(data) \
    ((((data) >> 24) & 0x000000FF) | (((data) >>  8) & 0x0000FF00) | \
    ( ((data) <<  8) & 0x00FF0000) | (((data) << 24) & 0xFF000000) )

float3 swap_float3(float3 input)
{
    float3 result;
    result.x = as_type<float>(swap32(as_type<uint32_t>(input.x)));
    result.y = as_type<float>(swap32(as_type<uint32_t>(input.y)));
    result.z = as_type<float>(swap32(as_type<uint32_t>(input.z)));
    
    return result;
}

vertex RasterizerData vertex_main(uint vertexID [[ vertex_id ]],
                                  constant Vertex *vertices [[ buffer(0) ]],
                                  constant Uniform &uniform [[ buffer(1) ]])
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
    
    //vertexID = swap16(vertexID);
    
    float4 pos = vertices[vertexID].position;
    //pos = swap_float3(pos);
    
    //pos.x = -pos.x;
    
    float4 position = uniform.projection * uniform.view * float4(pos.xyz, 1.0f);
    //position.z = pos.z; //1.0-vertices[vertexID].position.z;
    
    //position.x = -position.x;
    
    out.texcoord = vertices[vertexID].texcoord.xy;
    
    out.position = position;
    out.eye = -(uniform.view * uniform.model * pos).xyz;
    out.normal = uniform.normal_matrix * vertices[vertexID].normal.xyz;
    
    //float4(vertices[vertexID].normal.xyz, 1.0f);
    
    return out;
}

struct FragmentOutput {
    float4 color [[ color(0) ]];
};

constant float3 lightPos = float3(0, 1000, 0);

fragment FragmentOutput fragment_main(RasterizerData in [[stage_in]],
                                      texture2d<float> texture [[ texture(0) ]],
                                      constant Uniform &uniform [[ buffer(0) ]])
{
    FragmentOutput out;
    
    float tex = uniform.use_texture ? texture.sample(nearestSampler, in.texcoord, 0).r : 1;
    float3 normal = normalize(in.normal);

    float3 ambientTerm = float3(0.2f);

    float3 lightDir = float3(0,1,0);
    float diffuseIntensity = saturate(dot(normal, lightDir));
    float3 diffuseTerm = float3(0.1f) * diffuseIntensity;

    float3 specularTerm(0);
    if (diffuseIntensity > 0)
    {
        float3 eyeDirection = normalize(in.eye);
        float3 halfway = normalize(lightDir + eyeDirection);
        float specularFactor = pow(saturate(dot(normal, halfway)), 0.75f);
        specularTerm = specularFactor * 0.75f;
    }

    out.color = float4(ambientTerm + diffuseTerm + specularTerm, 1) * uniform.color;
    //out.color.xyz = in.position.xyz;
    
    return out;
}


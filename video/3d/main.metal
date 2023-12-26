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
    float point_size [[point_size]];
    
    float3 eye;
    float3 normal;
    float2 texcoord;
    
    // Since this member does not have a special attribute, the rasterizer
    // interpolates its value with the values of the other triangle vertices
    // and then passes the interpolated value to the fragment shader for each
    // fragment in the triangle.
//    float4 color;
};

struct Uniform
{
    float3 camera_pos;
    
    matrix_float4x4 view;
    matrix_float4x4 projection;
    matrix_float4x4 model;
    matrix_float3x3 normalMatrix;
    
    float4 color;
    bool use_texture;
    bool useGameIndexing;
  bool faceNormals;
  bool enableShading;
    int slopeMode;
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
    
    uint32_t vx = as_type<uint32_t>(input.x);
    uint32_t vy = as_type<uint32_t>(input.y);
    uint32_t vz = as_type<uint32_t>(input.z);
    
    vx = swap32(vx);
    vy = swap32(vy);
    vz = swap32(vz);
    
    result.x = as_type<float>(vx);
    result.y = as_type<float>(vy);
    result.z = as_type<float>(vz);
    
    return result;
}

vertex RasterizerData vertex_main(uint vertexID [[ vertex_id ]],
                                  constant packed_float3 *vertices [[ buffer(0) ]],
                                  constant packed_float3 *normals [[ buffer(1) ]],
                                  constant uint16_t *indices [[ buffer(2) ]],
                                  constant Uniform &uniform [[ buffer(3) ]])
{
    RasterizerData out;
    
    float4 position = float4(0.0f);
    float3 normal = float3(0.0f);
    
    if (uniform.useGameIndexing)
    {
      uint16_t index = swap16(indices[vertexID]);
      position = float4(swap_float3(vertices[index]), 1.0f);
      normal = swap_float3(uniform.faceNormals ? normals[vertexID/3] : normals[index]);
    }
    else
    {
        position = float4(swap_float3(vertices[vertexID]), 1.0f);
        normal = normals[vertexID];
    }
    
    //out.texcoord = vertices[vertexID].texcoord.xy;
    out.position = uniform.projection * uniform.view * uniform.model * position;
    out.eye = -(uniform.view * uniform.model * position).xyz;
    out.normal = normal.xyz; //uniform.normal_matrix * vertices[vertexID].normal.xyz;
    
    //float4(vertices[vertexID].normal.xyz, 1.0f);
    
    out.point_size = 5.0f;
    
    float4 div = position / 20.0f;
    float n = max(max(normal.x, normal.y), normal.z);
    
    if (n == fabs(normal.x)) {
        out.texcoord.x = div.y;
        out.texcoord.y = div.z;
    } else if (n == fabs(normal.y)) {
        out.texcoord.x = div.x;
        out.texcoord.y = div.z;
    } else {
        out.texcoord.x = div.x;
        out.texcoord.y = div.y;
    }
    
    return out;
}

struct FragmentOutput {
    float4 color [[ color(0) ]];
};

constant float3 lightPos = float3(0, 0, 1000);

fragment FragmentOutput fragment_main(RasterizerData in [[stage_in]],
                                      texture2d<float> texture [[ texture(0) ]],
                                      constant Uniform &uniform [[ buffer(0) ]])
{
  FragmentOutput out;
    
  float tex = uniform.use_texture ? texture.sample(nearestSampler, in.texcoord, 0).r : 1.0f;
    
  float3 normal = normalize(in.normal);
  float3 position = in.position.xyz;
  float3 ambientTerm = float3(tex * 0.25f); //float3(tex * 0.25f);
    
  float3 lightDir = float3(0.0f, 0.0f, 1.0f);
  float diffuseIntensity = saturate(dot(normal, lightDir));
  float3 diffuseTerm = diffuseIntensity * 0.25f;

  float3 specularTerm(0);
  if (diffuseIntensity > 0)
  {
    float3 eyeDirection = normalize(in.eye);
    float3 halfway = normalize(lightDir + eyeDirection);
    float specularFactor = pow(saturate(dot(normal, halfway)), 0.75f);
    specularTerm = specularFactor * 0.55f;
  }
    
    /* Determine slope visibility */
    
    float4 uniformColor = uniform.color;
    float3 up = float3(0, 0, 1);
    float dot = metal::dot(in.normal, up);
    if (dot < M_SQRT1_2_F && uniform.useGameIndexing)
    {
       // discard_fragment();
        //uniformColor.w = 0.0f;
    }
//    else if (dot > MIN_SLOPE)
//    {
//        printf("wall\n");
//    }
    
    //cool color:
    //out.color = float4(ambientTerm + diffuseTerm + in.normal, 1) * uniform.color;
    //out.color.w = ;
    
  //out.color = float4(uniform.color); //float4(in.normal, 1.0f);
  
  if (uniform.enableShading) {
    out.color = float4(ambientTerm + diffuseTerm + specularTerm, 1) * uniform.color;
  } else {
    out.color = uniform.color;
  }
    
    return out;
}


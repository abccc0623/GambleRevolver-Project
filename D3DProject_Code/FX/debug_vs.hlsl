#include "MainHeader.hlsli"
#include "Function.hlsli"


struct VertexInputType
{
    float3 position     : POSITION;
    float4 Color        : COLOR;
};

struct PixelInputType
{
    float4 pos : SV_POSITION;
    float4 posW : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 tangentL : TANGENT;
    float4 ShadowPosH : TEXCOORD1;
};

PixelInputType main(VertexInputType input)
{
    PixelInputType output;
    
    float4 posH = mul(float4(input.position.xyz, 1.0f), ObjworldMatrix);
    output.pos = mul(posH, viewproj);
    
  
    output.posW = input.Color;
    
    return output;
}
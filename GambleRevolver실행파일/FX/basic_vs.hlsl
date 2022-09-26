#include "MainHeader.hlsli"
#include "Function.hlsli"

struct VertexInputType
{
    float3 position		: POSITION;
    float3 NormalL		: NORMAL;
    float3 TangentL		: TANGENT;
    float2 Tex			: TEXCOORD;
};

struct PixelInputType
{
    float4 posH			: SV_POSITION;
    float3 NormalW		: NORMAL;
    float3 TangentW		: TANGENT;
    float2 Tex          : TEXCOORD;
    float4 ShadowPosH	: TEXCOORD1;
};

PixelInputType main(VertexInputType input)
{
    PixelInputType output;

    //¿ùµåº¯È¯
    float3 shadowPos = mul(float4(input.position.xyz, 1.0f), ObjworldMatrix).xyz;
    output.posH = mul(float4(input.position.xyz,1.0f), ObjworldMatrix);
    output.posH = mul(float4(output.posH.xyz, 1.0f), viewproj);
    
    
    //uv°ª
    output.Tex = mul(float4(input.Tex.xy, 0.0f,1.0f), TexMatrix);
    
    //³ë¸»
    output.NormalW = mul(input.NormalL, (float3x3) viewproj);
    
    //ÅºÁ¨Æ®
    output.TangentW = mul(input.TangentL, (float3x3) ObjworldMatrix);
    
    //½¦µµ¿ì
    output.ShadowPosH = mul(float4(shadowPos.xyz,1.0f), ShadowMatrix);
    
    return output;
}
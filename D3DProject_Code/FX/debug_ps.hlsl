#include "MainHeader.hlsli"


struct PixelInputType
{
    float4 pos : SV_POSITION;
    float4 posW : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 tangentL : TANGENT;
    float4 ShadowPosH : TEXCOORD1;
};

struct PixeloutputType
{
    float4 color    : SV_Target0;
    float4 normal   : SV_Target1;
    float4 Pos      : SV_Target2;
    float4 tangent  : SV_Target3;
    float4 shadow   : SV_Target4;
};


PixeloutputType main(PixelInputType input) : SV_TARGET
{
    PixeloutputType output;
    output.color    = input.posW;
    output.Pos      = input.pos;
    return output;
};
#include "MainHeader.hlsli"
#include "Function.hlsli"


struct PixelInputType
{
    float4 PosH : SV_POSITION;
    float3 PosL : POSITION;
    float3 NormalL : NORMAL;
    float2 Tex : TEXCOORD;
    float3 TangentW : TEXCOORD2;
    float4 Weights : WEIGHTS;
    uint4 BoneIndices : BONEINDICES;
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

    float4 Tex = { 1, 1, 1, 1 };
   
    if (Texture_Mask & TEXTURE_MASK_MAIN)
    {
        Tex = MainTexture.Sample(SampleType, input.Tex);
    }
   
    output.color    = float4(pow(Tex.rgb, 2.2f).xyz, 1);
    output.tangent  = float4(input.TangentW.xyz, 1);
    output.Pos      = float4(input.PosL.xyz,1);
    output.normal   = float4(input.NormalL, 1.0f);
    output.shadow   = float4(input.ShadowPosH);
    return output;
}
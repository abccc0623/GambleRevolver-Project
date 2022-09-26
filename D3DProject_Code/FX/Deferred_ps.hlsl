#include "MainHeader.hlsli"
#include "Function.hlsli"


struct PixelInputType
{
    float4 pos          : SV_POSITION;
    float4 posW         : POSITION;
    float2 tex          : TEXCOORD0;
    float3 normal       : NORMAL;
    float3 tangentL     : TANGENT;
    float4 ShadowPosH   : TEXCOORD1;
};

struct PixeloutputType
{
    float4 color        : SV_Target0;
    float4 normal       : SV_Target1;
    float4 Pos          : SV_Target2;
    float4 tangent      : SV_Target3;
    float4 shadow       : SV_Target4;
};

PixeloutputType main(PixelInputType input) : SV_TARGET
{
    PixeloutputType output;
    
    
    //컬러도 감마컬렉션 적용
    float4 text = { 0, 0, 0, 0 };
    if (Texture_Mask & TEXTURE_MASK_MAIN)
    {
        text   = MainTexture.Sample(SampleType, input.tex);
    }
    
    //output.color = float4(0, 0, 0, 0);
    output.color = float4(pow(text.rgba, 2.2f));
    //else
    //{
    //    output.color = float4(pow(text.rgb, 2.2f).xyz, 1);
    //}
    //output.color = float4(pow(text.rgba, 2.2f));
    //output.color = text;
    
  
    //포지션 보내줌
    output.Pos      = float4(input.posW);
    
    //탄젠트값 계산해서 넘겨줌
    //노말텍스쳐가 있다면 계산해서 넘겨주고 아니면 그냥 노말을넣어줌
    float3 bumpedNormalW = 1;
    
    output.normal = float4(input.normal, 1.0f);
    output.tangent = float4(input.tangentL, 1.0f);
    if (Texture_Mask & TEXTURE_MASK_NOMAL)
    {
        float4 Tex03 = NomalTexture.Sample(SampleType, input.tex);
        bumpedNormalW = NormalSampleToWorldSpace(Tex03.rgb, output.normal.xyz, input.tangentL.xyz);
        output.normal = float4(bumpedNormalW.xyz,1.0f);
    }
    
    output.shadow = float4(input.ShadowPosH);
    //output.shadow = float4(output.shadow);
    
    return output;
}
#include "MainHeader.hlsli"


struct PixelInputType
{
    float4 posH : SV_POSITION;
    float3 PosL : POSITION;
    float2 Tex : TEXCOORD0;
};

struct PixeloutputType
{
    float4 color : SV_Target0;
};
////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
PixeloutputType main(PixelInputType input) : SV_TARGET
{
    PixeloutputType output;
    
	float4 textureColor;
	float4 finalColor;


    // 이 텍스처 좌표 위치에서 샘플러를 사용하여 텍스처에서 픽셀 색상을 샘플링합니다.
    output.color = MainTexture.Sample(SampleType, input.Tex);
   
    output.color = output.color* ParticleColor;

    return output;
}
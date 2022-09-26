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


    // �� �ؽ�ó ��ǥ ��ġ���� ���÷��� ����Ͽ� �ؽ�ó���� �ȼ� ������ ���ø��մϴ�.
    output.color = MainTexture.Sample(SampleType, input.Tex);
   
    output.color = output.color* ParticleColor;

    return output;
}
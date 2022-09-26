#include "MainHeader.hlsli"
#include "Function.hlsli"

#define TEXTURE_MASK_MAIN   0x00000001 //�����ؽ��� ����
#define TEXTURE_MASK_SUB    0x00000010 //�����ؽ��� ����
#define TEXTURE_MASK_NOMAL  0x00000100 //�븻�ؽ��� ����

struct PixelInputType
{
    float4 posH			: SV_POSITION;
    float3 PosL			: POSITION;
    float2 Tex			: TEXCOORD0;
};


struct PixeloutputType
{
    float4 color : SV_Target0;
};
PixeloutputType main(PixelInputType input) : SV_TARGET
{ 
    PixeloutputType output;
    float4 Tex01;
    Tex01 = MainTexture.Sample(SampleType, input.Tex);
    
    output.color = Tex01;
    //output.color = pow(output.color, 1 / 2.2f);
    //output.color.a =1;
    return output;
}
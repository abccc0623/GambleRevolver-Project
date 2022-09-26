#include "MainHeader.hlsli"
#include "Function.hlsli"

#define TEXTURE_MASK_MAIN   0x00000001 //메인텍스쳐 여부
#define TEXTURE_MASK_SUB    0x00000010 //서브텍스쳐 여부
#define TEXTURE_MASK_NOMAL  0x00000100 //노말텍스쳐 여부

struct PixelInputType
{
    float4 posH			: SV_POSITION;
    float3 PosL			: POSITION;
    float2 Tex			: TEXCOORD0;
};

float4 main(PixelInputType input) : SV_TARGET
{ 
    float4  UI = D_UI.Sample(SampleType, input.Tex);
    float4 Light = D_LIGHT.Sample(SampleType, input.Tex);
    float4 alpha = D_Alpha.Sample(SampleType, input.Tex);
    float4 Final = { 0, 0, 0,0};
   
    //alpha = pow(alpha, 1 / 2.2f);
    
    //최종 색 결정
   // alpha = pow(alpha, 1 / 2.2f);
    //Final = (1 - alpha.a) * Light + alpha.a * alpha;
    //Final = ((1 - alpha.a) * Light) + (alpha.a * alpha);

    Final = Light +alpha;
    Final = Final + UI;
    //Final = ((1 - UI.a) * Final) + (UI.a * UI);
   
   
    return Final;
}
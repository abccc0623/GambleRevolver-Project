#include "MainHeader.hlsli"
struct PixelInputType
{
    float4 PosH : SV_POSITION;
    float3 PosL : POSITION;
};

float4 main(PixelInputType input) : SV_TARGET
{
    
    return SkyTexture.Sample(SampleType, input.PosL);
}
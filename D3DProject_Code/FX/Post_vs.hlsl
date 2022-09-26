#include "MainHeader.hlsli"

struct VertexInputType
{
	float4 PosL			: POSITION;
	float2 Tex			: TEXCOORD;
};

struct PixelInputType
{
	float4 posH			: SV_POSITION;
	float3 PosL			: POSITION;
	float2 Tex			: TEXCOORD0;
};


PixelInputType main(VertexInputType input)
{
    PixelInputType output;
    input.PosL.w = 1.0f;
	//월드변환
    output.posH = mul(input.PosL, ObjworldMatrix);
    output.posH = mul(float4(output.posH.xyz, 1.0f), viewproj);
	
    output.PosL = input.PosL;

	//uv값
    output.Tex = mul(float4(input.Tex, 0.0f, 1.0f), TexMatrix);
       
    return output;
}
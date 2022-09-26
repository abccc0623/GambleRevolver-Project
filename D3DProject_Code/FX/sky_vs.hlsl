#include "MainHeader.hlsli"


struct VertexInputType
{
	float3 PosL : POSITION;
};

struct PixelInputType
{
	float4 PosH : SV_POSITION;
	float3 PosL : POSITION;
};

PixelInputType main(VertexInputType input)
{
	PixelInputType output;

	//월드변환
	output.PosH = mul(float4(input.PosL.xyz, 1.0f), ObjworldMatrix);
	output.PosH = mul(float4(output.PosH.xyz, 1.0f), viewproj);

	output.PosL = input.PosL;

	return output;
}
#include "MainHeader.hlsli"

struct VertexInputType
{
    float4 pos		: POSITION;
    float2 tex		: TEXCOORD;
    float3 nomal	: NORMAL;
    float3 tangentL : TANGENT;
    
};

struct PixelInputType
{
    float4 pos      : SV_POSITION;
    float4 posW     : POSITION;
    float2 tex      : TEXCOORD0;
    float3 normal   : NORMAL;
    float3 tangentL : TANGENT;
    float4 ShadowPosH : TEXCOORD1;
};


PixelInputType main(VertexInputType input)
{
	PixelInputType output;

    //��ġ��
    output.pos = mul(input.pos, ObjworldMatrix);
    output.posW = output.pos;
    
    output.pos = mul(output.pos, viewproj);
    
    //uv
    output.tex = mul(float4(input.tex.xy, 0.0f, 1.0f), TexMatrix);
    
    //�븻
    output.normal = mul(input.nomal, (float3x3) ObjworldMatrix);
    //output.normal = mul(output.normal, (float3x3) viewproj);
    
    //ź��Ʈ
    output.tangentL = mul(input.tangentL, (float3x3) ObjworldMatrix);

    float3 shadowPos = mul(float4(input.pos.xyz, 1.0f), ObjworldMatrix);
    output.ShadowPosH = mul(float4(shadowPos.xyz, 1.0f), ShadowMatrix);
	return output;
}
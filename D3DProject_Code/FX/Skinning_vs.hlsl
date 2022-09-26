#include "MainHeader.hlsli"

struct VertexInputType
{
	float3  PosL        : POSITION;
	float3  NormalL     : NORMAL;
	float4  Weights     : WEIGHTS;
	float3  TangentL    : TANGENT;
	float2  Tex         : TEXCOORD;
	uint4   BoneIndices : BONEINDICES;
};

struct PixelInputType
{
	float4  PosH		: SV_POSITION;
	float3  PosL		: POSITION;
	float3  NormalL		: NORMAL;
	float2  Tex			: TEXCOORD;
	float3  TangentW	:TEXCOORD2;
	float4  Weights		: WEIGHTS;
	uint4   BoneIndices	: BONEINDICES;
    float4  ShadowPosH	: TEXCOORD1;
};

PixelInputType main(VertexInputType input)
{
	PixelInputType output;

    float weights[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    weights[0] = input.Weights.x;
    weights[1] = input.Weights.y;
    weights[2] = input.Weights.z;
    weights[3] = 1.0f - weights[0] - weights[1] - weights[2];
	
    //weights[3] = input.Weights.w;
	
	
    float3 posL = float3(0.0f, 0.0f, 0.0f);
    float3 normalL = float3(0.0f, 0.0f, 0.0f);
	
    for (int i = 0; i < 4; i++)
    {
        posL += weights[i] * mul(float4(input.PosL, 1.0f), gBoneTransforms[input.BoneIndices[i]]).xyz;
        normalL += weights[i] * mul(input.NormalL, (float3x3) gBoneTransforms[input.BoneIndices[i]]);
    }
   // output.PosH = mul(float4(posL, 1.0f), viewproj);
	
    output.PosL = mul(float4(posL, 1.0f), SkinningworldMatrix).xyzw;
    output.PosH = mul(float4(posL, 1.0f), SkinningworldMatrix).xyzw;
    output.PosH = mul(output.PosH, viewproj);
	

    output.NormalL = mul(normalL, (float3x3) SkinningworldMatrix); // 이때는 역전치행렬을 쓴다
    output.TangentW = mul(input.TangentL, (float3x3) SkinningworldMatrix);
	
	//uv값
    output.Tex = mul(float4(input.Tex, 0.0f, 1.0f), SkinningTexMatrix);
	
    float3 shadowPos = mul(float4(posL.xyz, 1.0f), SkinningworldMatrix).xyz;
    output.ShadowPosH = mul(float4(shadowPos.xyz, 1.0f), ShadowMatrix);

	return output;
}
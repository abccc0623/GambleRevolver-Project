#include "MainHeader.hlsli"
struct VertexInputType
{
    float4 PosL : POSITION;
    float2 Tex : TEXCOORD;
};

struct PixelInputType
{
    float4 posH : SV_POSITION;
    float3 PosL : POSITION;
    float2 Tex : TEXCOORD0;
};


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType main(VertexInputType input)
{
    PixelInputType output;
    

	// 적절한 행렬 계산을 위해 위치 벡터를 4 단위로 변경합니다.
    input.PosL.w = 1.0f;

	// 월드, 뷰 및 투영 행렬에 대한 정점의 위치를 ​​계산합니다.
    output.posH = mul(input.PosL, ParticleMatrix);
    output.posH = mul(output.posH, viewproj);
    
	// 픽셀 쉐이더의 텍스처 좌표를 저장한다.
    output.Tex = mul(float4(input.Tex, 0.0f, 1.0f), ParticleTexMatrix);
    
	// 픽셀 쉐이더의 입자 색상을 저장합니다. 
    //output.color = input.color;

    return output;
}
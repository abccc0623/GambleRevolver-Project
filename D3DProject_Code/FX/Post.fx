
#include "LightHelper.fx"
cbuffer cbPerFrame
{
	float4x4	gWorld;
	float4x4	gWorldViewProj;
	float2		gWindowSize;
	float4x4	gWorldInvTranspose;
	float4x4	gTexTransform;

	float4x4	gLightViewMatrix;
	float4x4	gLightProjectionMatrix;
};

Texture2D gBlur;
Texture2D gShadowMap;

TextureCube gBlurMap;

SamplerState samTriLinearSam
{
	Filter = MIN_MAG_MIP_LINEAR;
	//Filter = MIN_MAG_MIP_POINT;
	AddressU = CLAMP;
	AddressV = CLAMP;
};
SamplerComparisonState testSam
{
	Filter = MIN_MAG_MIP_LINEAR;
	//Filter = MIN_MAG_MIP_POINT;
	AddressU = CLAMP;
	AddressV = CLAMP;
};


struct VertexIn
{
	float4 PosL			: POSITION;
	float4 Tex			: TEXCOORD;
};

struct VertexOut
{
	float4 PosH				: SV_POSITION;
	float4 PosW				: POSITION;
	float2 Tex				: TEXCOORD;
	float4 ProjTex			: TEXCOORD1;
};
 
VertexOut Basic_VS(VertexIn vin)
{
	VertexOut vout;

	// 세계 공간 변환
	vout.PosW = mul(vin.PosL, gWorld);
	//vout.NormalW = mul(vin.NormalL, (float3x3)gWorldInvTranspose);
	//vout.TangentW = mul(vin.TangentL, (float3x3)gWorld);

	//동차 공간으로 변환
	vout.PosH = mul(vin.PosL, gWorldViewProj);


	// 정점 특성 출력(삼각형 표면을 따라 보간
	vout.Tex = mul(vin.Tex, gTexTransform);
	
	return vout;
}

//쉐도우맵 
VertexOut Shadow_VS(VertexIn vin)
{
	VertexOut vout;
	vout.PosW			= mul(vin.PosL, gWorldViewProj);
	vout.Tex			= mul(vin.Tex, gTexTransform).xy;
	vout.PosH			= mul(vin.PosL, gWorldViewProj);
	//동차 공간으로 변환

	float4 num = vin.PosL;
	num.w = 1.0f;
	//vout.ProjTex = mul(num, gLightProjectionMatrix);


	return vout;
}



//가우시안블러
float4 Gaussian_BlurPS(VertexOut pin) : SV_Target
{
	//litColor.a = gMaterial.Diffuse.a * texColor.a;
	float4 num = {0,0,0,0};
	float x1 = 1.0f / gWindowSize.x;
	float y1 = 1.0f / gWindowSize.y;

	
	num +=  psBlur_X(gBlur ,   samTriLinearSam, float2(x1, y1), pin.Tex.xy);
	num +=  psBlur_Y(gBlur ,   samTriLinearSam, float2(x1, y1), pin.Tex.xy);
	return num;
}

//블러 라디아블러
float4 Radial_BlurPS(VertexOut pin) : SV_Target
{
	//litColor.a = gMaterial.Diffuse.a * texColor.a;
	float4 num = {0,0,0,0};
	float x1 = 1.0f / gWindowSize.x;
	float y1 = 1.0f / gWindowSize.y;


	num += psRadialBlur(gBlur ,   samTriLinearSam, float2(x1, y1), pin.Tex);
	
	
	return num;
}

//기본출력
float4 BasicPS(VertexOut pin) : SV_Target
{
	return gBlur.Sample(samTriLinearSam, pin.Tex);
}

//투영 출력
float4 Shadow_PS(VertexOut pin) : SV_Target
{
	pin.ProjTex.xyz /= pin.ProjTex.w;
	float depth = pin.ProjTex.z;

	return gShadowMap.Sample(samTriLinearSam, pin.Tex);
}



RasterizerState NoCull
{
    CullMode = None;
};

DepthStencilState LessEqualDSS
{
	// Make sure the depth function is LESS_EQUAL and not just LESS.  
	// Otherwise, the normalized depth values at z = 1 (NDC) will 
	// fail the depth test if the depth buffer was cleared to 1.
    DepthFunc = LESS_EQUAL;
};

technique11 BlurTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, Basic_VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, BasicPS()));
	}

    pass P1
    {
        SetVertexShader( CompileShader( vs_5_0, Basic_VS()));
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, Gaussian_BlurPS()));

    }

	pass P2
	{
		SetVertexShader(CompileShader(vs_5_0, Basic_VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, Radial_BlurPS()));
	}

	pass P3
	{
		SetVertexShader(CompileShader(vs_5_0, Shadow_VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, Shadow_PS()));
	}
}

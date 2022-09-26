//***************************************************************************************
// LightHelper.fx by Frank Luna (C) 2011 All Rights Reserved.
//
// Structures and functions for lighting calculations.
//***************************************************************************************

struct DirectionalLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float3 Direction;
	float pad;
};

struct PointLight
{ 
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;

	float3 Position;
	float Range;

	float3 Att;
	float pad;
};

struct SpotLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;

	float3 Position;
	float Range;

	float3 Direction;
	float Spot;

	float3 Att;
	float pad;
};

struct Material
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular; // w = SpecPower
	float4 Reflect;
};

//---------------------------------------------------------------------------------------
// Computes the ambient, diffuse, and specular terms in the lighting equation
// from a directional light.  We need to output the terms separately because
// later we will modify the individual terms.
//---------------------------------------------------------------------------------------
void ComputeDirectionalLight(Material mat, DirectionalLight L, 
                             float3 normal, float3 toEye,
					         out float4 ambient,
						     out float4 diffuse,
						     out float4 spec)
{
	// Initialize outputs.
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec    = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// The light vector aims opposite the direction the light rays travel.
	float3 lightVec = -L.Direction;

	// Add ambient term.
	ambient = mat.Ambient * L.Ambient;	

	// Add diffuse and specular term, provided the surface is in 
	// the line of site of the light.
	
	float diffuseFactor = dot(lightVec, normal);

	// Flatten to avoid dynamic branching.
	[flatten]
	if( diffuseFactor > 0.0f )
	{
		float3 v         = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(v, toEye), 0.0f), mat.Specular.w);
					
		diffuse = diffuseFactor * mat.Diffuse * L.Diffuse;
		spec    = specFactor * mat.Specular * L.Specular;
	}
}

//---------------------------------------------------------------------------------------
// Computes the ambient, diffuse, and specular terms in the lighting equation
// from a point light.  We need to output the terms separately because
// later we will modify the individual terms.
//---------------------------------------------------------------------------------------
void ComputePointLight(Material mat, PointLight L, float3 pos, float3 normal, float3 toEye,
				   out float4 ambient, out float4 diffuse, out float4 spec)
{
	// Initialize outputs.
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec    = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// The vector from the surface to the light.
	float3 lightVec = L.Position - pos;
		
	// The distance from surface to light.
	float d = length(lightVec);
	
	// Range test.
	if( d > L.Range )
		return;
		
	// Normalize the light vector.
	lightVec /= d; 
	
	// Ambient term.
	ambient = mat.Ambient * L.Ambient;	

	// Add diffuse and specular term, provided the surface is in 
	// the line of site of the light.

	float diffuseFactor = dot(lightVec, normal);

	// Flatten to avoid dynamic branching.
	[flatten]
	if( diffuseFactor > 0.0f )
	{
		float3 v         = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(v, toEye), 0.0f), mat.Specular.w);
					
		diffuse = diffuseFactor * mat.Diffuse * L.Diffuse;
		spec    = specFactor * mat.Specular * L.Specular;
	}

	// Attenuate
	float att = 1.0f / dot(L.Att, float3(1.0f, d, d*d));

	diffuse *= att;
	spec    *= att;
}

//---------------------------------------------------------------------------------------
// Computes the ambient, diffuse, and specular terms in the lighting equation
// from a spotlight.  We need to output the terms separately because
// later we will modify the individual terms.
//---------------------------------------------------------------------------------------
void ComputeSpotLight(Material mat, SpotLight L, float3 pos, float3 normal, float3 toEye,
				  out float4 ambient, out float4 diffuse, out float4 spec)
{
	// Initialize outputs.
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec    = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// The vector from the surface to the light.
	float3 lightVec = L.Position - pos;
		
	// The distance from surface to light.
	float d = length(lightVec);
	
	//길이만큼 넘어가게 된다면  빛연산을 하지않음
	if( d > L.Range )
		return;
		
	// Normalize the light vector.
	lightVec /= d; 
	
	// Ambient term.
	ambient = mat.Ambient * L.Ambient;	

	// Add diffuse and specular term, provided the surface is in 
	// the line of site of the light.

	float diffuseFactor = dot(lightVec, normal);

	// Flatten to avoid dynamic branching.
	[flatten]
	if( diffuseFactor > 0.0f )
	{
		float3 v         = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(v, toEye), 0.0f), mat.Specular.w);
					
		diffuse = diffuseFactor * mat.Diffuse * L.Diffuse;
		spec    = specFactor * mat.Specular * L.Specular;
	}
	
	// Scale by spotlight factor and attenuate.
	float spot = pow(max(dot(-lightVec, L.Direction), 0.0f), L.Spot);

	// Scale by spotlight factor and attenuate.
	float att = spot / dot(L.Att, float3(1.0f, d, d*d));

	ambient *= spot;
	diffuse *= att;
	spec    *= att;
}

//---------------------------------------------------------------------------------------
// Transforms a normal map sample to world space.
//---------------------------------------------------------------------------------------
float3 NormalSampleToWorldSpace(float3 normalMapSample, float3 unitNormalW, float4 tangentW)
{
	// Uncompress each component from [0,1] to [-1,1].
	float3 normalT = 2.0f*normalMapSample - 1.0f;

	// Build orthonormal basis.
	float3 N = unitNormalW;
	float3 T = normalize(tangentW.xyz - dot(tangentW.xyz, N)*N);
	float3 B = tangentW.w*cross(N, T);

	float3x3 TBN = float3x3(T, B, N);

	// Transform from tangent space to world space.
	float3 bumpedNormalW = mul(normalT, TBN);

	return bumpedNormalW;
}

//---------------------------------------------------------------------------------------
// Performs shadowmap test to determine if a pixel is in shadow.
//---------------------------------------------------------------------------------------

static const float SMAP_SIZE = 2048.0f;
static const float SMAP_DX = 1.0f / SMAP_SIZE;

float CalcShadowFactor(SamplerComparisonState samShadow, 
                       Texture2D shadowMap, 
					   float4 shadowPosH)
{
	// Complete projection by doing division by w.
	shadowPosH.xyz /= shadowPosH.w;
	
	// Depth in NDC space.
	float depth = shadowPosH.z-0.0001f;

	// Texel size.
	const float dx = SMAP_DX;

	float percentLit = 0.0f;
	const float2 offsets[9] = 
	{
		float2(-dx,  -dx), float2(0.0f,  -dx), float2(dx,  -dx),
		float2(-dx, 0.0f), float2(0.0f, 0.0f), float2(dx, 0.0f),
		float2(-dx,  +dx), float2(0.0f,  +dx), float2(dx,  +dx)
	};

	[unroll]
	for(int i = 0; i < 9; ++i)
	{
		percentLit += shadowMap.SampleCmpLevelZero(samShadow, 
			shadowPosH.xy + offsets[i], depth).r;
	}

	return percentLit /= 9.0f;
}

//블러 X 값으로 블러를 먹임 GAUSSIAN BLUR
float4 psBlur_X(Texture2D  texture1, SamplerState st, float2 winsize, float2 texCoord : TEXCOORD) : COLOR
{
	//입력한 값으로 블러를 먹임
	float weights[9] =
	{
		0.013519569015984728,
        0.047662179108871855,
        0.11723004402070096,
        0.20116755999375591,
        0.240841295721373,
        0.20116755999375591,
        0.11723004402070096,
        0.047662179108871855,
        0.013519569015984728
	};
	//가중치를 모두 합치면 1.1을 넘으면안됨


	//x 축으로 9개의 픽셀들을 가져옴
	float indices[9] = {-4,-3,-2,-1,0,1,2,3,4 };
	//축 방향
	float2  dir = float2(1.0, 0.0);
	

	//float size = 1.0f / 200.0f;
	float2 step = float2(dir.x * winsize.x, dir.y * winsize.y);
	float4 Result = 0.0f;

	//계산
	for (int i = 0; i < 9; i++)
	{
		Result += texture1.Sample(st, float2(texCoord + indices[i] * step)) * weights[i];
	}

	return float4(Result.rgb,1);
}

float4 psBlur_Y(Texture2D  texture1, SamplerState st, float2 winsize, float2 texCoord : TEXCOORD) : COLOR
{
	//입력한 값으로 블러를 먹임
	float weights[9] =
	{
		0.013519569015984728,
		0.047662179108871855,
		0.11723004402070096,
		0.20116755999375591,
		0.240841295721373,
		0.20116755999375591,
		0.11723004402070096,
		0.047662179108871855,
		0.013519569015984728
	};

//x 축으로 9개의 픽셀들을 가져옴
float indices[9] = {-4,-3,-2,-1,0,1,2,3,4 };
//축 방향
float2  dir = float2(0.0, 1.0);


float size = 1.0f / 200.0f;
float2 step = float2(dir.x * winsize.x, dir.y * winsize.y);
float4 Result = 0.0f;

//계산
for (int i = 0; i < 9; i++)
{
	Result += texture1.Sample(st, float2(texCoord + indices[i] * step)) * weights[i];
}

return float4(Result.rgb,1);
}

float4 psRadialBlur(Texture2D  texture1, SamplerState st, float2 winsize, float2 texCoord : TEXCOORD) : COLOR0
{
	#define NUM_RADIALBLUR_SAMPLES 10
	// 샘플링될 위치값들
	float samples[NUM_RADIALBLUR_SAMPLES] = { -0.08f,-0.05f,-0.03f,-0.02f,-0.01f,0.01f,0.02f,0.03f,0.05f,0.08f };

	float4  vRadialBlurValue = float4(0, 0, 0, 0);
	vRadialBlurValue.x = 0.5f;
	vRadialBlurValue.y = 0.5f;
	vRadialBlurValue.z = 1.0f;
	vRadialBlurValue.w = 1.5f;
	
	// 현재 위치에서 레디얼 블러가 시작되는 지점까지의 벡터를 구한다
	float2 dir = vRadialBlurValue.xy - texCoord;
	
	// 블러 시작점까지의 거리(벡터의 크기)
	float dist = length(dir);
	
	// 단위벡터로 만든다
	dir = dir / dist;
	
	// 최초의 픽셀 컬러
	float4 color = texture1.Sample(st, float2(texCoord));
	
	float4 sum = color;
	
	// 현재 위치에서 블러 시작점 방향으로 NUM_SAMPLES개의 픽셀을 샘플링해서 누적한다
	for (int i = 0; i < NUM_RADIALBLUR_SAMPLES; i++)
	{
		sum += texture1.Sample(st, texCoord + dir * samples[i] * vRadialBlurValue.z);
		sum += texture1.Sample(st, texCoord + dir * ( ( i - NUM_RADIALBLUR_SAMPLES / 2 ) * 0.01 ) * vRadialBlurValue.z );
	}
	
	// 최초 픽셀 + NUM_RADIALBLUR_SAMPLES개의 누적픽셀 = NUM_RADIALBLUR_SAMPLES+1
	// 누적값을 샘플링된 픽셀개수로 나누어 평균값을 구한다
	sum *= 1.0 / (NUM_RADIALBLUR_SAMPLES + 1);
	
	// 시작점과의 거리에 따라서 가중치를 준다(멀어질수록 더 Blur시킨다)
	float t = saturate(dist * vRadialBlurValue.w);
	
	// 현재 픽셀과 누적평균 픽셀을 합성한다
	float4 blur = lerp(color, sum, t);
	//    float4 src = tex2D(SamplerSrc1, texCoord);
	
	return blur;
}

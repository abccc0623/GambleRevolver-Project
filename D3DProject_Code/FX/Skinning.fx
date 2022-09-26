//***************************************************************************************
// color.fx by Frank Luna (C) 2011 All Rights Reserved.
//
// Transforms and colors geometry.
//***************************************************************************************

#include "LightHelper.fx"

cbuffer cbPerFrame
{
    DirectionalLight gDirLights;
    PointLight gPointLight;
    SpotLight gSpotLight;

    float3 gEyePosW;

    float gFogStart;
    float gFogRange;
    float4 gFogColor;
};

cbuffer cbPerObject
{
    float4x4 gWorld;
    float4x4 gWorldInvTranspose;
	float4x4 gWorldViewProj; 
    float4x4 gTexTransform;
    Material gMaterial;
};

Texture2D gDiffuseMap;
Texture2D gNormalMap;
TextureCube gCubeMap;

cbuffer cbSkinned
{
    float4x4 gBoneTransforms[96];
};

SamplerState samAnisotropic
{
    Filter = ANISOTROPIC;
    MaxAnisotropy = 4;

    AddressU = WRAP;
    AddressV = WRAP;
};


SamplerState samLinear
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = WRAP;
    AddressV = WRAP;
};


struct VertexIn
{
	float3 PosL  : POSITION;
    float3 NormalL : NORMAL;
    float2 Tex : TEXCOORD;
    float3 TangentL : TANGENT;
};

struct VertexOut
{
	float4 PosH  : SV_POSITION;
    float3 PosW : POSITION;
    float3 NormalW: NORMAL;
    float4 Color : COLOR;
    float2 Tex: TEXCOORD;
    float3 TangentW : TANGENT;
};

struct SkinnedVertexIn
{
    float3 PosL       : POSITION;
    float3 NormalL    : NORMAL;
    float2 Tex        : TEXCOORD;
    float3 Weights    : WEIGHTS;
    float3 TangentL   : TANGENT;
    uint4 BoneIndices : BONEINDICES;
};

struct SkinnedVertexOut
{
    float3 PosL       : POSITION;
    float3 NormalL    : NORMAL;
    float2 Tex        : TEXCOORD;
    float3 Weights    : WEIGHTS;
    uint4 BoneIndices : BONEINDICES;
};






VertexOut VS(VertexIn vin)
{
    VertexOut vout;

    // 세계 공간 변환
    vout.PosW = mul(float4(vin.PosL, 1.0f), gWorld).xyz;
    vout.NormalW = mul(vin.NormalL, (float3x3)gWorldInvTranspose);
    vout.TangentW = mul(vin.TangentL, (float3x3)gWorld);

    //동차 공간으로 변환
    vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);


    // 정점 특성 출력(삼각형 표면을 따라 보간
    vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTransform).xy;

    //vout.Color.x = 1.0f;
    //vout.Color.y = 0;
    //vout.Color.z = 0;
    //vout.Color.w = 1.0f;


    return vout;
}

VertexOut SkinnedVS(SkinnedVertexIn vin)
{
    VertexOut vout;

    // Init array or else we get strange warnings about SV_POSITION.
    // 버텍스 가중치 배열을 초기화한다. 아니면 SV_POSITION 값에 대한 이상한 워닝을 볼 것이다.
    float weights[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    weights[0] = vin.Weights.x;
    weights[1] = vin.Weights.y;
    weights[2] = vin.Weights.z;
    weights[3] = 1.0f - weights[0] - weights[1] - weights[2];

    float3 posL = float3(0.0f, 0.0f, 0.0f);
    float3 normalL = float3(0.0f, 0.0f, 0.0f);

    for (int i = 0; i < 4; ++i)
    {
        // Assume no nonuniform scaling when transforming normals, so 
        // that we do not have to use the inverse-transpose.
        // 논유니폼 스케일이 없다고 가정하므로, 노말값을 위한 역행렬의 전치행렬이 필요없다.
        posL += weights[i] * mul(float4(vin.PosL, 1.0f), gBoneTransforms[vin.BoneIndices[i]]).xyz;
        normalL += weights[i] * mul(vin.NormalL, (float3x3)gBoneTransforms[vin.BoneIndices[i]]);
    }

    // Transform to world space space.
    // 월드 스페이스로 변환
    vout.PosW = mul(float4(posL, 1.0f), gWorld).xyz;
    vout.NormalW = mul(normalL, (float3x3)gWorldInvTranspose);	// 이때는 역전치행렬을 쓴다.

    // Transform to homogeneous clip space.
    vout.PosH = mul(float4(posL, 1.0f), gWorldViewProj);

    // Output vertex attributes for interpolation across triangle.
    vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), gTexTransform).xy;
    vout.TangentW = mul(vin.TangentL, (float3x3)gWorld);
    return vout;
}

float3 NormalSampleToWorldSpace(float3 normalMapSample, float3 unitNormalW, float3 tangentW)
{
    // Uncompress each component from [0,1] to [-1,1].
    float3 normalT = 2.0f * normalMapSample - 1.0f;

    // Build orthonormal basis.
    float3 N = unitNormalW;
    float3 T = normalize(tangentW - dot(tangentW, N) * N);
    float3 B = cross(N, T);

    float3x3 TBN = float3x3(T, B, N);

    // Transform from tangent space to world space.
    float3 bumpedNormalW = mul(normalT, TBN);

    return bumpedNormalW;
}

float4 Shadow_PS(VertexOut pin) : SV_Target
{
    float depthValue = pin.PosH.z / (pin.PosH.w);
    float num = 1 - depthValue.x;

    return float4(num, num, num,1);
}

float4 PS(VertexOut pin, uniform int gLightCount, uniform bool gUseTexture, uniform bool gUseTexureNomal) : SV_Target
{
    //법선이 단위벡터가 아닐 수 있으므로 다시 정규화 한다.
     pin.NormalW = normalize(pin.NormalW);

    //toEye 벡터로 조명 계산
    float3 toEye = gEyePosW - pin.PosW;
    
    //시점과 이 표면 점 사이의 거리를 보관
    float distToEye = length(toEye);
    

    float3 bumpedNormalW = 0;
    if (gUseTexureNomal)
    {
        float3 normalMapSample = gNormalMap.Sample(samLinear, pin.Tex).rgb;
        bumpedNormalW = NormalSampleToWorldSpace(normalMapSample, pin.NormalW, pin.TangentW);
    }

    //정규화
    toEye /= distToEye;
    
    //기본 텍스쳐 색상은 곱셈의 항등원
    float4 texColor = float4(1, 1, 1, 1);
    if (gUseTexture)
    {
        //텍스쳐에서 표본을 추출한다.
        texColor = gDiffuseMap.Sample(samAnisotropic, pin.Tex);
    }
    
    float3 toEyeW = normalize(gEyePosW - pin.PosW);
   
   float4 litColor = texColor;

   float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
   float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
   float4 specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

   float4 A, D, S;

    //gDirLights.Direction.y
    //DirectionalLight L = gDirLights;
    //L.Direction.x *= -1;
    //L.Direction.y *= -1;

   ComputeDirectionalLight(gMaterial, gDirLights, bumpedNormalW, toEye, A, D, S);
   ambient += A;
   diffuse += D;
   specular += S;
   //
   ComputePointLight(gMaterial, gPointLight, pin.PosW, pin.NormalW, toEyeW, A, D, S);
   ambient += A;
   diffuse += D;
   specular += S;
   
   ComputeSpotLight(gMaterial, gSpotLight, pin.PosW, pin.NormalW, toEyeW, A, D, S);
   ambient += A;
   diffuse += D;
   specular += S;

   //litColor = texColor * (ambient + diffuse) + specular;
    



    //일반적으로 분산광 재질으리 알파 성분을 최종 알파 값으로 사용한다.
    //litColor.a = gMaterial.Diffuse.a * texColor.a;
    
    
    return litColor;
}

technique11 SkinningTech
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, SkinnedVS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PS(1, true, false)));
    }

    pass P1
    {
        SetVertexShader(CompileShader(vs_5_0, SkinnedVS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, Shadow_PS()));
    }
};


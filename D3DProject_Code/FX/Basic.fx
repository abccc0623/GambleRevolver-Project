//***************************************************************************************
// color.fx by Frank Luna (C) 2011 All Rights Reserved.
//
// Transforms and colors geometry.
//***************************************************************************************

#include "LightHelper.fx"

cbuffer cbPerFrame
{
    DirectionalLight gDirLights;
    PointLight  gPointLight;
    SpotLight   gSpotLight;
    float3 gEyePosW;
    float gFogStart;
    float gFogRange;
    float4 gFogColor;
};

cbuffer cbPerObject
{
    float4x4 gShadowPos;
    float4x4 gWorld;
    float4x4 gWorldInvTranspose;
	float4x4 gWorldViewProj; 
    float4x4 gTexTransform;
    
    Material gMaterial;
};

Texture2D gDiffuseMap;
Texture2D gSubMap;
Texture2D gNormalMap;
Texture2D gShadowMap;
TextureCube gCubeMap;

SamplerState samAnisotropic
{
    Filter = ANISOTROPIC;
    MaxAnisotropy = 4;

    AddressU = WRAP;
    AddressV = WRAP;
};

SamplerComparisonState samShadow
{
    Filter = COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
    AddressU = BORDER;
    AddressV = BORDER;
    AddressW = BORDER;
    BorderColor = float4 (0.0f, 0.0f, 0.0f, 0.0f);
    ComparisonFunc = LESS_EQUAL;
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
    float2 Tex: TEXCOORD0;
    float3 TangentW : TANGENT;
    float4 ShadowPosH:TEXCOORD1;
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
    vout.ShadowPosH = mul(float4(vout.PosW,1.0f), gShadowPos);

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


float4 PS(VertexOut pin,
    uniform int gLightCount,
    uniform bool gUseTexure,
    uniform bool gUseTexureNomal,
    uniform bool gAlphaClip,
    uniform bool gFogEnabled,
    uniform bool gReflectionEnabled) : SV_Target
{
    // Interpolating normal can unnormalize it, so normalize it.
    pin.NormalW = normalize(pin.NormalW);

// The toEye vector is used in lighting.
    float3 toEye = gEyePosW - pin.PosW;
    
    // Cache the distance to the eye from this surface point.
    float distToEye = length(toEye);
    
    // Normalize.
    toEye /= distToEye;
    

    // Default to multiplicative identity.
    float4 texColor = float4(1, 1, 1, 1);
    if (gUseTexure)
    {
        // Sample texture.
        float4 Tex01 = gDiffuseMap.Sample(samLinear, pin.Tex);
        Tex01.rgb = pow(Tex01.rgb, 2.2f); //감마콜렉션..

        //float4 Tex02 = gSubMap.Sample(samLinear, pin.Tex);
        //Tex02.rgb = pow(Tex02.rgb, 2.2f); //감마콜렉션


        texColor = Tex01;
    
        if (gAlphaClip)
        {
            // Discard pixel if texture alpha < 0.1.  Note that we do this
            // test as soon as possible so that we can potentially exit the shader 
            // early, thereby skipping the rest of the shader code.
            clip(texColor.a - 0.1f);
        }

    }

  
    float3 bumpedNormalW = 0;
    if (gUseTexureNomal)
    {
        float3 normalMapSample = gNormalMap.Sample(samLinear, pin.Tex).rgb;
       bumpedNormalW = NormalSampleToWorldSpace(normalMapSample, pin.NormalW, pin.TangentW);
    }
    
    float3 toEyeW = normalize(gEyePosW - pin.PosW);
    //
    // Lighting.
    //
    
    float4 litColor = texColor;
    
    
        
    // Start with a sum of zero. 
    float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    
    float3 shadow = float3 (1.0f, 1.0f, 1.0f);
    shadow = CalcShadowFactor(samShadow, gShadowMap,
        pin.ShadowPosH);


    float4 A, D, S;
    ComputeDirectionalLight(gMaterial, gDirLights, bumpedNormalW, toEye, A, D, S);
    ambient     += A;
    diffuse     += shadow[0]* D;
    specular    += shadow[0]* S;
    
    // Sum the light contribution from each light source.  
 
    ComputePointLight(gMaterial, gPointLight, pin.PosW, pin.NormalW, toEyeW, A, D, S);
    ambient += A;
    diffuse += D;
    specular += S;
    
    ComputeSpotLight(gMaterial, gSpotLight, pin.PosW, pin.NormalW, toEyeW, A, D, S);
    ambient += A;
    diffuse += D;
    specular += S;
    
    litColor = texColor * (ambient + diffuse) + specular;
    
    if (gReflectionEnabled)
    {
        float3 incident = -toEye;
        float3 reflectionVector = reflect(incident, bumpedNormalW);
        float4 reflectionColor = gCubeMap.Sample(samLinear, reflectionVector);
        litColor += gMaterial.Reflect * reflectionColor;
    }



    //
    // Fogging
    //

    if (gFogEnabled)
    {
        float fogLerp = saturate((distToEye - gFogStart) / gFogRange);
    
        // Blend the fog color and the lit color.
        litColor = lerp(litColor, gFogColor, fogLerp);
    }

    litColor.a += gMaterial.Diffuse.a * texColor.a;
    litColor.rgb = pow(litColor.rgb, 1 / 2.2f);
return litColor;
}

float4 Shadow_PS(VertexOut pin) : SV_Target
{
    float depthValue = pin.PosH.z / (pin.PosH.w);
    float num = 1 - depthValue.x;

    return float4(num, num, num,1);
}



technique11 BasicTech
{
    pass P0
    {   
        //텍스쳐o 노멀 o
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PS(1, true, true, false, false, true)));
    }

    pass P1
    {
        //텍스쳐x 노멀 o
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PS(1, false, true, false, false, true)));
    }

    pass P2
    {
        //텍스쳐x 노멀 x
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PS(1, false, false, false, false, true)));
    }

    pass P3
    {
        //텍스쳐o 노멀 x
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PS(1, true, false, false, false, true)));
    }

};








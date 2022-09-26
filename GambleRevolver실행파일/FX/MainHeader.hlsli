#define TEXTURE_MASK_MAIN   0x00000001 //메인텍스쳐 여부
#define TEXTURE_MASK_SUB    0x00000010 //서브텍스쳐 여부
#define TEXTURE_MASK_NOMAL  0x00000100 //노말텍스쳐 여부

#define LIGHT_MASK_DIR      0x00000001 // 디렉션 라이트
#define LIGHT_MASK_SPOT     0x00000010 // 스포트라이트
#define LIGHT_MASK_POINT    0x00000100 // 포인트라이트

#include "StructData.hlsli"

//카메라 관련 버퍼
cbuffer CameraBuffer : register(b0)
{
    float4x4 viewproj;
    float4x4 ViewMatrix;
    float3 camPos;
};

//오브젝트 관련 버퍼
cbuffer ObjectBuffer : register(b1)
{
    float4x4 ObjworldMatrix;
    float4x4 TexMatrix;
    uint Texture_Mask;
    float alpha;
};

//스키닝 관련 버퍼
cbuffer SkinningBuffer : register(b2)
{
    float4x4 SkinningworldMatrix;
    float4x4 SkinningTexMatrix;
    float4x4 gBoneTransforms[96];
};

//라이트 관련 버퍼
cbuffer DirLightBuffer : register(b3)
{
    DirectionalLight DirLight;
    Material Mat;
};

//라이트 관련 버퍼
cbuffer PointLightBuffer : register(b4)
{
    PointLight point_Light[50];
    uint PointLightSize;
};
//라이트 관련 버퍼
cbuffer SpotLightBuffer : register(b5)
{
    SpotLight Spot_Light;
};
//쉐도우
cbuffer ShadowBuffer : register(b6)
{
    float4x4 ShadowMatrix;
};

//UI
cbuffer UIBuffer : register(b7)
{
    float Alpha;
    float4x4 UIworldMatrix;
    float4x4 UITexMatrix;
};

cbuffer Particlebuffer : register(b8)
{
    float4x4 ParticleMatrix;
    float4x4 ParticleTexMatrix;
    float4   ParticleColor;
};



//텍스쳐
Texture2D MainTexture   : register(t0); //메인
Texture2D SubTexture    : register(t1); //서브
Texture2D NomalTexture  : register(t2); //노말
TextureCube SkyTexture  : register(t3); //스카이


//디퍼드에서 합치기위한 텍스쳐
Texture2D D_Shadow  : register(t4); //쉐도우 
Texture2D D_COLOR   : register(t5); //컬러
Texture2D D_NOMAL   : register(t6); //노말
Texture2D D_POS     : register(t7); //위치
Texture2D D_TANGENT : register(t8); //탄젠트
Texture2D D_LIGHT   : register(t9); //라이트
Texture2D D_ShadowPos   : register(t10); //쉐도우 위치
Texture2D D_UI          : register(t11); //UI텍스쳐들
Texture2D D_Alpha       : register(t12); //UI텍스쳐들



SamplerState SampleType : register(s0)
{
    Filter = MIN_MAG_MIP_POINT; //텍스쳐를 샘플링 할때 사용할 필터 방식
    
    AddressU = BORDER; //0~1범위를 벗어나는 u텍스쳐 좌표를해결하는방법
    AddressV = BORDER; //0~1범위를 벗어나는 v텍스쳐 좌표를해결하는방법
    AddressW = BORDER; //0~1범위를 벗어나는 w텍스쳐 좌표를해결하는방법
    
    
    BorderColor = float4(0.0f, 0.0f, 0.0f, 0.0f); //D3D1_TEX일경우 사용할 테두리 색주소 0~1사이
    ComparisonFunc = LESS_EQUAL; //샘플링된 데이트를 기존 새믈링된 데이터와 비교하는 함수

                                                    //LESS_EQUAL = 소스 데이터가 대상 데이터보다 작거나 같으면 통과
};

SamplerComparisonState samShadow : register(s1);














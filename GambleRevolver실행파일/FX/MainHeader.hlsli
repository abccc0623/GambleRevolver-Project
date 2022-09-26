#define TEXTURE_MASK_MAIN   0x00000001 //�����ؽ��� ����
#define TEXTURE_MASK_SUB    0x00000010 //�����ؽ��� ����
#define TEXTURE_MASK_NOMAL  0x00000100 //�븻�ؽ��� ����

#define LIGHT_MASK_DIR      0x00000001 // �𷺼� ����Ʈ
#define LIGHT_MASK_SPOT     0x00000010 // ����Ʈ����Ʈ
#define LIGHT_MASK_POINT    0x00000100 // ����Ʈ����Ʈ

#include "StructData.hlsli"

//ī�޶� ���� ����
cbuffer CameraBuffer : register(b0)
{
    float4x4 viewproj;
    float4x4 ViewMatrix;
    float3 camPos;
};

//������Ʈ ���� ����
cbuffer ObjectBuffer : register(b1)
{
    float4x4 ObjworldMatrix;
    float4x4 TexMatrix;
    uint Texture_Mask;
    float alpha;
};

//��Ű�� ���� ����
cbuffer SkinningBuffer : register(b2)
{
    float4x4 SkinningworldMatrix;
    float4x4 SkinningTexMatrix;
    float4x4 gBoneTransforms[96];
};

//����Ʈ ���� ����
cbuffer DirLightBuffer : register(b3)
{
    DirectionalLight DirLight;
    Material Mat;
};

//����Ʈ ���� ����
cbuffer PointLightBuffer : register(b4)
{
    PointLight point_Light[50];
    uint PointLightSize;
};
//����Ʈ ���� ����
cbuffer SpotLightBuffer : register(b5)
{
    SpotLight Spot_Light;
};
//������
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



//�ؽ���
Texture2D MainTexture   : register(t0); //����
Texture2D SubTexture    : register(t1); //����
Texture2D NomalTexture  : register(t2); //�븻
TextureCube SkyTexture  : register(t3); //��ī��


//���۵忡�� ��ġ������ �ؽ���
Texture2D D_Shadow  : register(t4); //������ 
Texture2D D_COLOR   : register(t5); //�÷�
Texture2D D_NOMAL   : register(t6); //�븻
Texture2D D_POS     : register(t7); //��ġ
Texture2D D_TANGENT : register(t8); //ź��Ʈ
Texture2D D_LIGHT   : register(t9); //����Ʈ
Texture2D D_ShadowPos   : register(t10); //������ ��ġ
Texture2D D_UI          : register(t11); //UI�ؽ��ĵ�
Texture2D D_Alpha       : register(t12); //UI�ؽ��ĵ�



SamplerState SampleType : register(s0)
{
    Filter = MIN_MAG_MIP_POINT; //�ؽ��ĸ� ���ø� �Ҷ� ����� ���� ���
    
    AddressU = BORDER; //0~1������ ����� u�ؽ��� ��ǥ���ذ��ϴ¹��
    AddressV = BORDER; //0~1������ ����� v�ؽ��� ��ǥ���ذ��ϴ¹��
    AddressW = BORDER; //0~1������ ����� w�ؽ��� ��ǥ���ذ��ϴ¹��
    
    
    BorderColor = float4(0.0f, 0.0f, 0.0f, 0.0f); //D3D1_TEX�ϰ�� ����� �׵θ� ���ּ� 0~1����
    ComparisonFunc = LESS_EQUAL; //���ø��� ����Ʈ�� ���� ���ɸ��� �����Ϳ� ���ϴ� �Լ�

                                                    //LESS_EQUAL = �ҽ� �����Ͱ� ��� �����ͺ��� �۰ų� ������ ���
};

SamplerComparisonState samShadow : register(s1);














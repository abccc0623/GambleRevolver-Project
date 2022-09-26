#pragma once
/// <summary>
/// �������� �����ϴ� ������Ʈ���� �⺻ ���̴� ���۵��� �������ִ°�
/// </summary>


struct CameraBuffer
{
	CameraBuffer()
	{
		viewproj = XMMatrixIdentity();
		CamPos = { 0,0,0 };
		ViewMatrix = XMMatrixIdentity();
	};
	void ReSet()
	{
		viewproj = XMMatrixIdentity();
		CamPos = { 0,0,0 };
		ViewMatrix = XMMatrixIdentity();
	}
	XMMATRIX viewproj;
	XMMATRIX ViewMatrix;
	XMFLOAT3 CamPos;
};

struct ObjectBuffer
{
	ObjectBuffer()
	{
		world = XMMatrixIdentity();
		TexMatrix = XMMatrixIdentity();
		Texture_Mask = 0x00000000;
	};
	void ReSet()
	{
		world = XMMatrixIdentity();
		TexMatrix = XMMatrixIdentity();
		Texture_Mask = 0x00000000;
	}

	XMMATRIX world;
	XMMATRIX TexMatrix;
	UINT Texture_Mask;
	float Alpha;
};

struct SkinningBuffer
{
	XMMATRIX SkinningworldMatrix = XMMatrixIdentity();
	XMMATRIX SkinningTexMatrix = XMMatrixIdentity();
	XMMATRIX gBoneTransforms[96];
	UINT Texture_Mask;
};

struct DirLightBuffer
{
	DirectionalLight DirLight;
	Material Mat;
};

struct hsPointLightBuffer
{
	hsPointLight pointLight[50];
	UINT PointLightSize;
};

struct ShadowData
{
	XMMATRIX ShadowMatrix;
};

struct UIbuffer
{
	float Alpha;
	XMMATRIX UIMatrix;
	XMMATRIX UITexMatrix;
};

struct Particlebuffer
{
	XMMATRIX ParticleMatrix;
	XMMATRIX ParticleTexMatrix;
	XMFLOAT4 ParticleColor;
};





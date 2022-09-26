//***************************************************************************************
// Effects.cpp by Frank Luna (C) 2011 All Rights Reserved.
//***************************************************************************************

#include "EnginePCH.h"

#pragma region Effect
Effect::Effect(ID3D11Device* device, const std::wstring& filename)
	: mFX(0)
{
	std::ifstream fin(filename, std::ios::binary);

	fin.seekg(0, std::ios_base::end);
	int size = (int)fin.tellg();
	fin.seekg(0, std::ios_base::beg);
	std::vector<char> compiledShader(size);

	fin.read(&compiledShader[0], size);
	fin.close();
	
	HR(D3DX11CreateEffectFromMemory(&compiledShader[0], size, 
		0, device, &mFX));
}

Effect::~Effect()
{
	ReleaseCOM(mFX);
}

#pragma region Effects

BasicEffect*		Effects::BasicFX		= 0;
SkyEffect*			Effects::SkyFX			= 0;
SkinningEffect*		Effects::SkinningFX		= 0;
ColorEffect*		Effects::ColorFX		= 0;
PostEffect*			Effects::PostFX			= 0;
void Effects::InitAll(ID3D11Device* device)
{
	////�⺻ ������Ʈ ���� FX
	BasicFX		= new BasicEffect(device, L"../../FX/Basic.cso");
	
	//��ī�̹ڽ� ���� FX
	SkyFX		= new SkyEffect(device, L"../../FX/Sky.cso");
	
	//��Ű�� ������Ʈ ���� FX
	SkinningFX	= new SkinningEffect(device, L"../../FX/Skinning.cso");
	
	//Debug���� FX
	//ColorFX		= new ColorEffect(device, L"../../FX/color.cso");
	
	PostFX		= new PostEffect(device, L"../../FX/Post.cso");
}

void Effects::DestroyAll()
{
	SafeDelete(BasicFX);
	SafeDelete(SkyFX);
	SafeDelete(SkinningFX);
	SafeDelete(ColorFX);
}
#pragma endregion
#include "EnginePCH.h"
#include "Basic.h"
BasicEffect::BasicEffect(ID3D11Device* device, const std::wstring& filename)
	: Effect(device, filename)
{
	BasicTech			= mFX->GetTechniqueByName("BasicTech");	
	WorldViewProj		= mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
	World				= mFX->GetVariableByName("gWorld")->AsMatrix();
	WorldInvTranspose	= mFX->GetVariableByName("gWorldInvTranspose")->AsMatrix();
	TexTransform		= mFX->GetVariableByName("gTexTransform")->AsMatrix();
	EyePosW				= mFX->GetVariableByName("gEyePosW")->AsVector();
	DirLights			= mFX->GetVariableByName("gDirLights");
	Mat					= mFX->GetVariableByName("gMaterial");
	ShadowPos			= mFX->GetVariableByName("gShadowPos")->AsMatrix();


	DiffuseMap			= mFX->GetVariableByName("gDiffuseMap")->AsShaderResource();
	SubMap				= mFX->GetVariableByName("gSubMap")->AsShaderResource();
	NomalMap			= mFX->GetVariableByName("gNormalMap")->AsShaderResource();
	CubeMap				= mFX->GetVariableByName("gCubeMap")->AsShaderResource();
	ShadowMap			= mFX->GetVariableByName("gShadowMap")->AsShaderResource();

	PointLights			= mFX->GetVariableByName("gPointLight");
	SpotLights			= mFX->GetVariableByName("gSpotLight");
}

BasicEffect::~BasicEffect()
{
	SubMap->Release();
	BasicTech->Release();
	WorldViewProj->Release();
	World->Release();
	WorldInvTranspose->Release();
	TexTransform->Release();
	EyePosW->Release();
	DirLights->Release();
	Mat->Release();
	DiffuseMap->Release();
	CubeMap->Release();
	NomalMap->Release();
}
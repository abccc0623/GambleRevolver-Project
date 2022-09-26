#include "EnginePCH.h"
#include "SkinningFX.h"

SkinningEffect::SkinningEffect(ID3D11Device* device, const std::wstring& filename)
	: Effect(device, filename)
{
	SkinningTech		= mFX->GetTechniqueByName("SkinningTech");
	WorldViewProj		= mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
	World				= mFX->GetVariableByName("gWorld")->AsMatrix();
	WorldInvTranspose	= mFX->GetVariableByName("gWorldInvTranspose")->AsMatrix();
	TexTransform		= mFX->GetVariableByName("gTexTransform")->AsMatrix();
	EyePosW				= mFX->GetVariableByName("gEyePosW")->AsVector();
	Mat					= mFX->GetVariableByName("gMaterial");
	BoneTM				= mFX->GetVariableByName("gBoneTransforms")->AsMatrix();

	DiffuseMap			= mFX->GetVariableByName("gDiffuseMap")->AsShaderResource();
	NomalMap			= mFX->GetVariableByName("gNormalMap")->AsShaderResource();


	DirLights			= mFX->GetVariableByName("gDirLights");
	PointLights			= mFX->GetVariableByName("gPointLight");
	SpotLights			= mFX->GetVariableByName("gSpotLight");
}

SkinningEffect::~SkinningEffect()
{
	SkinningTech->Release();
	WorldViewProj->Release();
	WorldViewProj->Release();
	World->Release();
	WorldInvTranspose->Release();
	TexTransform->Release();
	EyePosW->Release();
	DirLights->Release();
	Mat->Release();
	BoneTM->Release();
	DiffuseMap->Release();
	NomalMap->Release();
}
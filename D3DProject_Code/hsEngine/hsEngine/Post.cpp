#include "EnginePCH.h"
#include "Post.h"

PostEffect::PostEffect(ID3D11Device* device, const std::wstring& filename)
	: Effect(device, filename)
{
	BlurTech = mFX->GetTechniqueByName("BlurTech");

	BlurMap = mFX->GetVariableByName("gBlur")->AsShaderResource();
	ShadowMap = mFX->GetVariableByName("gShadowMap")->AsShaderResource();
	
	WindowSize = mFX->GetVariableByName("gWindowSize")->AsVector();
	WorldViewProj = mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
	WorldInvTranspose = mFX->GetVariableByName("gWorldInvTranspose")->AsMatrix();
	TexTransform = mFX->GetVariableByName("gTexTransform")->AsMatrix();
	World = mFX->GetVariableByName("gWorld")->AsMatrix();

	gLightViewMatrix = mFX->GetVariableByName("gLightViewMatrix")->AsMatrix();
	gLightProjectionMatrix = mFX->GetVariableByName("gLightProjectionMatrix")->AsMatrix();
}

PostEffect::~PostEffect()
{
	BlurTech->Release();
	BlurMap->Release();
	ShadowMap->Release();
	WindowSize->Release();
	WorldViewProj->Release();
	WorldInvTranspose->Release();
	World->Release();
	TexTransform->Release();
}
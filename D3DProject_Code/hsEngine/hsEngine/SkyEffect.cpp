#include "EnginePCH.h"
#include "SkyEffect.h"

SkyEffect::SkyEffect(ID3D11Device* device, const std::wstring& filename)
	: Effect(device, filename)
{
	SkyTech			= mFX->GetTechniqueByName("SkyTech");
	WorldViewProj	= mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
	CubeMap			= mFX->GetVariableByName("gCubeMap")->AsShaderResource();
}

SkyEffect::~SkyEffect()
{

}
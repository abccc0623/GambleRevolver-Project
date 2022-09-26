#include "EnginePCH.h"
#include "ColorEffect.h"

ColorEffect::ColorEffect(ID3D11Device* device, const std::wstring& filename)
	: Effect(device, filename)
{
	ColorTech = mFX->GetTechniqueByName("ColorTech");
	WorldViewProj = mFX->GetVariableByName("gWorldViewProj")->AsMatrix();
	
}

ColorEffect::~ColorEffect()
{
	ColorTech->Release();
	WorldViewProj->Release();
}
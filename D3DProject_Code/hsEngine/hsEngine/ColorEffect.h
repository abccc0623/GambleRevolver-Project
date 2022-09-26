#pragma once
class ColorEffect : public Effect
{
public:
	ColorEffect(ID3D11Device* device, const std::wstring& filename);
	~ColorEffect();

	void SetWorldViewProj(CXMMATRIX M) { WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }

	ID3DX11EffectTechnique* ColorTech;
	ID3DX11EffectMatrixVariable* WorldViewProj;
	
};
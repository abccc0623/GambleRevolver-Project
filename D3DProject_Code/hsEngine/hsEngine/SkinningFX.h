#pragma once

class SkinningEffect : public Effect
{
public:
	SkinningEffect(ID3D11Device* device, const std::wstring& filename);
	~SkinningEffect();



	void SetWorldViewProj(CXMMATRIX M) { WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorld(CXMMATRIX M) { World->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorldInvTranspose(CXMMATRIX M) { WorldInvTranspose->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetTexTransform(CXMMATRIX M) { TexTransform->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetEyePosW(const XMFLOAT3& v) { EyePosW->SetRawValue(&v, 0, sizeof(XMFLOAT3)); }
	void SetMaterial(const Material& mat) { Mat->SetRawValue(&mat, 0, sizeof(Material)); }
	void SetDiffuseMap(ID3D11ShaderResourceView* tex) { DiffuseMap->SetResource(tex); }
	void SetNomalMap(ID3D11ShaderResourceView* tex) { NomalMap->SetResource(tex); }

	//라이트
	//void SetDirLights(const DirectionalLight* lights) { DirLights->SetRawValue(lights, 0, sizeof(DirectionalLight)); }
	void SetPointLights(const hsPointLight* lights) { PointLights->SetRawValue(lights, 0, sizeof(hsPointLight)); }
	void SetSpotLights(const SpotLight* lights) { SpotLights->SetRawValue(lights, 0, sizeof(SpotLight)); }


	void SetBoneTM(const std::vector<XMFLOAT4X4> boneTMList)
	{
		BoneTM->SetMatrixArray(reinterpret_cast<const float*>(&boneTMList[0].m), 0, boneTMList.size());
	}

	ID3DX11EffectTechnique* SkinningTech;

	ID3DX11EffectMatrixVariable* WorldViewProj;
	ID3DX11EffectMatrixVariable* World;
	ID3DX11EffectMatrixVariable* WorldInvTranspose;
	ID3DX11EffectMatrixVariable* TexTransform;
	ID3DX11EffectVectorVariable* EyePosW;
	ID3DX11EffectMatrixVariable* BoneTM;

	//라이트
	ID3DX11EffectVariable* DirLights;
	ID3DX11EffectVariable* PointLights;
	ID3DX11EffectVariable* SpotLights;
	ID3DX11EffectVariable* Mat;

	ID3DX11EffectShaderResourceVariable* DiffuseMap;
	ID3DX11EffectShaderResourceVariable* NomalMap;
};

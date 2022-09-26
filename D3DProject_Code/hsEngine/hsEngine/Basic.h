#pragma once
class BasicEffect : public Effect
{
public:
	BasicEffect(ID3D11Device* device, const std::wstring& filename);
	~BasicEffect();

	void SetWorldViewProj(CXMMATRIX M) { WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorld(CXMMATRIX M) { World->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorldInvTranspose(CXMMATRIX M) { WorldInvTranspose->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetTexTransform(CXMMATRIX M) { TexTransform->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetEyePosW(const XMFLOAT3& v) { EyePosW->SetRawValue(&v, 0, sizeof(XMFLOAT3)); }

	void SetShadowPos(CXMMATRIX m) { ShadowPos->SetMatrix(reinterpret_cast<const float*>(&m));}


	void SetDirLights(const DirectionalLight* lights) { DirLights->SetRawValue(lights, 0, sizeof(DirectionalLight)); }
	void SetPointLights(const hsPointLight* lights) { PointLights->SetRawValue(lights, 0, sizeof(hsPointLight)); }
	void SetSpotLights(const SpotLight* lights) { SpotLights->SetRawValue(lights, 0, sizeof(SpotLight)); }
	

	void SetMaterial(const Material& mat) { Mat->SetRawValue(&mat, 0, sizeof(Material)); }
	void SetDiffuseMap(ID3D11ShaderResourceView* tex) { DiffuseMap->SetResource(tex); }
	void SetSubMap(ID3D11ShaderResourceView* tex) { SubMap->SetResource(tex); }
	void SetNomalMap(ID3D11ShaderResourceView* tex) { NomalMap->SetResource(tex); }
	void SetDinamicCubeMap(ID3D11ShaderResourceView* tex) { CubeMap->SetResource(tex); }
	void SetShadowMap(ID3D11ShaderResourceView* tex) { ShadowMap->SetResource(tex); }



	ID3DX11EffectTechnique* BasicTech;
	ID3DX11EffectMatrixVariable* WorldViewProj;
	ID3DX11EffectMatrixVariable* World;
	ID3DX11EffectMatrixVariable* WorldInvTranspose;
	ID3DX11EffectMatrixVariable* TexTransform;
	ID3DX11EffectVectorVariable* EyePosW;
	ID3DX11EffectMatrixVariable* ShadowPos;

	ID3DX11EffectVariable* DirLights;
	ID3DX11EffectVariable* PointLights;
	ID3DX11EffectVariable* SpotLights;

	ID3DX11EffectVariable* Mat;
	ID3DX11EffectShaderResourceVariable* DiffuseMap;
	ID3DX11EffectShaderResourceVariable* SubMap;
	ID3DX11EffectShaderResourceVariable* NomalMap;
	ID3DX11EffectShaderResourceVariable* CubeMap;
	ID3DX11EffectShaderResourceVariable* ShadowMap;

};

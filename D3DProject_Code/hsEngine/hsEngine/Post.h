#pragma once
class PostEffect : public Effect
{
public:
	PostEffect(ID3D11Device* device, const std::wstring& filename);
	~PostEffect();


	void SetBlureMap(ID3D11ShaderResourceView* _Blurmap) { BlurMap->SetResource(_Blurmap);}
	void SetShadowMap(ID3D11ShaderResourceView* _ShadowMap) { ShadowMap->SetResource(_ShadowMap); }
	void SetWindowSize(const XMFLOAT2& size){ WindowSize->SetRawValue(&size, 0, sizeof(XMFLOAT2)); }
	void SetWorldViewProj(CXMMATRIX M) { WorldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorldInvTranspose(CXMMATRIX M) { WorldInvTranspose->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetTexTransform(CXMMATRIX M) { TexTransform->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorld(CXMMATRIX M) { World->SetMatrix(reinterpret_cast<const float*>(&M)); }

	void SetLightViewMatrix(CXMMATRIX M) { gLightViewMatrix->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetLightProjectionmatrix(CXMMATRIX M) { gLightProjectionMatrix->SetMatrix(reinterpret_cast<const float*>(&M)); }



	ID3DX11EffectTechnique* BlurTech;

	//윈도우의 사이즈
	ID3DX11EffectVariable* WindowSize;

	ID3DX11EffectShaderResourceVariable* BlurMap;
	ID3DX11EffectShaderResourceVariable* ShadowMap;

	ID3DX11EffectMatrixVariable* WorldViewProj;
	ID3DX11EffectMatrixVariable* WorldInvTranspose;
	ID3DX11EffectMatrixVariable* World;
	ID3DX11EffectMatrixVariable* TexTransform;


	ID3DX11EffectMatrixVariable* gLightViewMatrix;
	ID3DX11EffectMatrixVariable* gLightProjectionMatrix;

};
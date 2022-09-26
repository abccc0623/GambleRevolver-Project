#pragma once
class SkyBox:public HsMesh
{
public:
	SkyBox(float x = 0, float y = 0, float z = 0);
	virtual ~SkyBox();

	virtual void init(hsEngine* _ENGINE, LoadData* _LoadData);
	virtual void Render(const hsViewData& view) override;
	virtual void Update();
	void SetSkyTextures(ID3D11ShaderResourceView* Textures);
private:
	ID3D11Buffer* Skybuffer;
	ID3D11ShaderResourceView* SkyTextures;
	XMMATRIX vew;
	XMMATRIX WVP;
};
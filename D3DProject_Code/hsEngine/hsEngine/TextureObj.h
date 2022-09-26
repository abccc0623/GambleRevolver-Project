#pragma once
class TextureMap;
class TextureObj : public HsMesh
{
public:
	TextureObj(float x = 0, float y = 0, float z = 0);
	virtual ~TextureObj();

	void init(hsEngine* _ENGINE);
	virtual void Update() override;
	virtual void Render(const hsViewData& view) override;
	virtual void Debug();


	void SetRenderState(POST_TYPE Type);
	bool isUI = false;
private:
	float TexSize;
	int RenderType;
	
	ID3D11Buffer* mWorldBuffer;
	ID3D11Buffer* mCamBuffer;
};
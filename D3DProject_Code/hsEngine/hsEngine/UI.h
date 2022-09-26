#pragma once

/// <summary>
/// UI 들의 최상위 객체이다
/// </summary>
class UI : public Rendering 
{
public:
	UI(float x =0,float y = 0,float z = 0);
	virtual~UI();

	Transform transform;

	virtual void init(hsEngine* _ENGINE);
	

	virtual void Update();
	virtual void Render();
	virtual void Delete();
	virtual void Debug();

	//알파값 조절
	void SetAlpha(float number);
	//피봇을 설정한다 이것을 설정하면 피벗기준으로 움직인다
	void SetPivot(UI_PIVOT  pivot);
	void SetTexture(ID3D11ShaderResourceView* mTexture);
	void ChangeImg(string Name);
	void PushImg(string Name, ID3D11ShaderResourceView* Texture);
	
private:

	//버텍스 데이터
	vector<UI32> vData;
	//인덱스 데이터
	vector<UINT> uData;


	ID3D11Buffer* mWorldBuffer;

protected:
	XMMATRIX mWorld;
	hsEngine* ENGINE;
	float Alpha;
	int pivotX;
	int pivotY;
	bool isRender;

	//스크린좌표계로 바꾼 위치값들
	XMFLOAT3 mScreen_Pos;
	XMFLOAT3 mScreen_Rot;
	XMFLOAT3 mScreen_Scl;


	map<string, ID3D11ShaderResourceView*> ImgList;
};
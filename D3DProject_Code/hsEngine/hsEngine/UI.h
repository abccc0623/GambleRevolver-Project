#pragma once

/// <summary>
/// UI ���� �ֻ��� ��ü�̴�
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

	//���İ� ����
	void SetAlpha(float number);
	//�Ǻ��� �����Ѵ� �̰��� �����ϸ� �ǹ��������� �����δ�
	void SetPivot(UI_PIVOT  pivot);
	void SetTexture(ID3D11ShaderResourceView* mTexture);
	void ChangeImg(string Name);
	void PushImg(string Name, ID3D11ShaderResourceView* Texture);
	
private:

	//���ؽ� ������
	vector<UI32> vData;
	//�ε��� ������
	vector<UINT> uData;


	ID3D11Buffer* mWorldBuffer;

protected:
	XMMATRIX mWorld;
	hsEngine* ENGINE;
	float Alpha;
	int pivotX;
	int pivotY;
	bool isRender;

	//��ũ����ǥ��� �ٲ� ��ġ����
	XMFLOAT3 mScreen_Pos;
	XMFLOAT3 mScreen_Rot;
	XMFLOAT3 mScreen_Scl;


	map<string, ID3D11ShaderResourceView*> ImgList;
};
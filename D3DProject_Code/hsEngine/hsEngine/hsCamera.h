#pragma once
class Box;
class hsCamera :public GameObject, public Rendering,public Debugging
{
public:
	hsCamera(float X = 0,float Y = 0,float Z = 0);
	virtual~hsCamera();

	

	
	void SetLens(float fovY, float aspect, float zn, float zf);
	void SetLens();

	///������� �̾Ƴ���
	XMMATRIX GetView();

	///���� ����� �̾Ƴ���
	XMMATRIX GetProj();
	///ī�޶��� ��������� �����´�
	XMMATRIX GetWorld();


	
	///ī�޶� ȸ����Ų��
	void SetRotation(float AngleX, float AngleY, float AngleZ);
	void SetPosition(float X, float Y, float Z);
	void SetProj(XMMATRIX _Proj);

	void init(hsEngine* ENGINE);
	

	bool isMain;
	float Speed;

	virtual void Update() override;
	virtual void Delete() override;
	virtual void Debug() override;
	virtual void Render(const hsViewData& view) override;

	XMFLOAT3 GetClickPoint();
private:
	ID3D11Buffer* m_GlobalBuffer;
	XMMATRIX LocalPos;
	//ī�޶� �� ��Ʈ����
	XMFLOAT4X4 mView;
	//���� ���
	XMFLOAT4X4 mProj;

	float mNearZ;
	float mFarZ;
	float mAspect;
	float mFovY;
	float mNearWindowHeight;
	float mFarWindowHeight;

};
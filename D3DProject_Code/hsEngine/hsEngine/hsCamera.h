#pragma once
class Box;
class hsCamera :public GameObject, public Rendering,public Debugging
{
public:
	hsCamera(float X = 0,float Y = 0,float Z = 0);
	virtual~hsCamera();

	

	
	void SetLens(float fovY, float aspect, float zn, float zf);
	void SetLens();

	///뷰행렬을 뽑아낸다
	XMMATRIX GetView();

	///투영 행렬을 뽑아낸다
	XMMATRIX GetProj();
	///카메라의 월드행렬을 가져온다
	XMMATRIX GetWorld();


	
	///카메라를 회전시킨다
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
	//카메라 뷰 매트릭스
	XMFLOAT4X4 mView;
	//투영 행렬
	XMFLOAT4X4 mProj;

	float mNearZ;
	float mFarZ;
	float mAspect;
	float mFovY;
	float mNearWindowHeight;
	float mFarWindowHeight;

};
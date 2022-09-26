#pragma once

///모든 라이트 들을 관리할 라이트매니저 디퍼드 랜더링을통해 화면으로 그려야해서 
///객체 인덱스버퍼와 버텍스버퍼를 생성하고 랜더링을 해줌
class Factory;
class LightObject;
class LightManager : public GameObject,public Rendering
{

public:
	LightManager(float x=0,float y = 0,float z = 0);
	virtual~LightManager();

	// GameObject을(를) 통해 상속됨
	virtual void Update() override;
	virtual void Render(const hsViewData& view) override;
	virtual void Delete() override;
	virtual void Debug() override;
	void init(hsEngine* ENGINE,Factory* factory);


	void PushLightObject(LightObject* obj);
	
	ID3D11ShaderResourceView* mSRV;
private:
	Factory* mFactory;


	//라이트 구조체
	XMFLOAT4 Ambient;
	XMFLOAT4 Diffuse;
	XMFLOAT4 Specular;
	XMFLOAT3 Att;
	float Range;
	XMFLOAT3 Position;

	//static vector<LightManager> PointLightList;

	//메터리얼
	Material Mat;

	ID3D11Buffer* PointLightbuffer;
	ID3D11Buffer* objectbuffer;
	ID3D11Buffer* Dirbuffer;

	
	//포인트 라이트 리스트
	vector<LightObject*>PointLightList;
	//스포트 라이트 리스트
	vector<LightObject*>SpotLightList;
	//디렉션 라이트 
	LightObject* DirLight;
};
#pragma once
class LightObject :public GameObject, public Debugging
{
public:
	LightObject(float x = 0, float y = 0, float z = 0);
	~LightObject();
	


	void init(hsEngine* _ENGINE, int type);
	virtual void Render(const hsViewData& view) override;
	virtual void Debug()override;
	virtual void Update()override;
	virtual void Delete()override;


	const int GetType();
public:
	///스포트 라이트
public:
	///디렉션 라이트

	//디렉션 라이트 생성
	void SetDirLight(DirectionalLight DirLight);
	//엠비언트 스펙큘러 디퓨즈
	void SetDirLight(XMFLOAT4 A, XMFLOAT4 D, XMFLOAT4 S);
	//디렉션 라이트 컬러
	void SetDirLightColor(XMFLOAT3 color);
	//디렉션 라이트 가져오기
	DirectionalLight* GetDirLight();
public:
	///포인트 라이트

	//포인트라이트 생성
	void SetPointLight(hsPointLight Light);
	//엠비언트 스펙큘러 디퓨즈
	void SetPointLight(XMFLOAT4 A, XMFLOAT4 D, XMFLOAT4 S);
	//포인트 라이트 길이
	void SetPointLightRange(float _Range);
	//포인트 라이트 컬러
	void SetPointLightColor(XMFLOAT3 color);
	//포인트 라이트 가져오기
	const hsPointLight* GetPointLight();
private:
	int Myindex = 0;

	//어떤타입인지몰라서 일단 다 넣어놈

	//포인트 라이트 타입구조체
	hsPointLight PointLight;

	//디렉션 라이트 타입 구조체
	DirectionalLight DirLight;
	//hsSpotLight		SpritLight;

	//라이트의 타입
	int LightType;

	// GameObject을(를) 통해 상속됨
};
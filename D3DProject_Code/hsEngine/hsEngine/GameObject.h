#pragma once
///지금은 최상위오브젝트인 GameObject에서 랜더링 관련 변수들을 모조리 들고있는데
///랜더링 클래스를 하나더만들어서 상속시키는게 좋을듯..

class GameObject
{
public:
	GameObject();
	virtual~GameObject();

	///위치값
	Transform transfrom;

	///랜더링을 할것인가
	bool isRender;

	//랜더 스테이트
	void SetRasterizerState(ID3D11RasterizerState* render);

	//자기자신의 mWorld행렬을 구하는 업데이트
	virtual void Update() = 0;

	//카메라의 view proj을 받아와 그려줌
	virtual void Render(const hsViewData& view)= 0;
	//삭제
	virtual void Delete()	= 0;
	//디버깅
	virtual void Debug()	= 0;

	
	//계층구조로 라면..
	std::string ParentName;
	std::string MyName;

	//랜더링 할건지 여부
	bool OnRender;
	bool GetDeleteCheck();
	bool isDelete;

	float Alpha;
	void SetParentTR(Transform* tr);
	void UpdateParentTr();
protected:
	hsEngine*				ENGINE;			// 엔진
	ID3D11Device*			Device;			// D3D11 디바이스
	ID3D11DeviceContext*	mContext;		// 디바이스 컨텍스트

	XMMATRIX mWorld;	//월드 행렬
	XMMATRIX mView;		//뷰 행렬
	XMMATRIX mProj;		//프로젝션 행렬

	//랜더 스테이트
	ID3D11RasterizerState* m_pRenderstate;

	Transform* ParentTranform = nullptr;
private:
};
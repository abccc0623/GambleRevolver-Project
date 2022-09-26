#pragma once
class TextureMap;
class Particle : public HsMesh
{
public:
	Particle(float x = 0, float y = 0, float z = 0);
	virtual ~Particle();

	void init(hsEngine* _ENGINE);
	virtual void Update() override;
	virtual void Render(const hsViewData& view) override;

	//파티클 업데이트
	void ParticleUpdate();
	void Setting();
public:
	///파티클 정보
	//생성 주기
	float CreateMaxTime;
	//삭제 주기
	float DeleteMaxTime;
	//파티클 속도
	float Speed;
	//파티클 방향
	XMFLOAT3 direction;
	//파티클 컬러
	XMFLOAT3 Color;
	//파티클의 시작 위치
	XMFLOAT3 StartPos;
	//파티클의 시작 사이즈
	XMFLOAT3 StartSize;
	//파티클의 시작 회전
	XMFLOAT3 StartRot;
	XMFLOAT3 EndSize;

	//플레이 중인 파티클인지 판별
	bool isStart;
private:
	///알필요없는것들
	float CreateTime;
	float DeleteTime;

	ID3D11Buffer* mWorldBuffer;
public:
	XMFLOAT3 Local_Up;
	XMFLOAT3 Local_Look;
	XMFLOAT3 Local_Right;
};
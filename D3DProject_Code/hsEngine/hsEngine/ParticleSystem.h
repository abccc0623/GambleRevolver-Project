#pragma once

struct Particle_float;
struct Particle_float3;
class ParticleSystem : public GameObject,public Rendering,public Debugging
{
public:
	ParticleSystem(float x = 0,float y = 0,float z = 0);
	virtual ~ParticleSystem();


	virtual void init(hsEngine* _ENGINE);
	virtual void Update() override;
	virtual void Render(const hsViewData& view) override;
	virtual void Delete() override;
	virtual void Debug() override;

	//텍스쳐를 넣어준다
	void SetTexture(ID3D11ShaderResourceView* Texture);

	//파티클들이 카메라를 처다보도록해준다
	XMMATRIX CamLookAt();

	//파티클 업데이트
	void UpDateParticle();
	//로컬 좌표를 업데이트 한다
	void UpDateLocal();
	void Play();
	void Stop();
public:
	///파티클 정보 받기
	//스피드를 넣어준다 랜덤이 아니라면 min값에 스피드를 넣는다
	void SetSpeed(Particle_float speed);
	//파티클생성주기 삭제주기를 넣어준다
	void SetParticleTime(float mCreate, float mDelete);
	//시작 위치를 넣어준다 파티클 오브젝트 중심으로 어디로움직일것인지
	void SetStartPos(Particle_float3 pos);
	//시작 크기를 넣어준다 
	void SetStartScl(Particle_float3 scl);
	//시작 회전을 넣어준다
	void SetStartRot(Particle_float3 rot);
	//컬러색 받기
	void SetStartColor(Particle_float3 color);
	//마지막 색 넣기
	void SetEndColor(Particle_float3 color);
	//파티클 방향넣기
	void SetDirection(float x,float y,float z);
	//한번에 나와야하는 파티클의 개수
	void SetParticleCount(int count);

	void SetParicleEndTime(float time);
public:
	///파티클 함수들
	//파티클 재생
	void PlayParticle(float mCreateTime, float mDeleteTime);
	//파티클을생성해준다
	void CreateParticle(int count);
	//현재 실행되고있지않은 파티클을 찾는다
	Particle* FindParticle(int count);
private:
	///파티클 정보
	//한번에 나와야하는 파티클의 수
	int ParticleCount;
	//생성 주기
	float CreateMaxTime;
	//삭제 주기
	float DeleteMaxTime;

	float ParticleEndTime;
	float ParticleEndMaxTime;
	//파티클 방향
	XMFLOAT3 direction;
	XMFLOAT3 EndSize;
	//파티클 스피드
	Particle_float Speed;
	//처음 위치
	Particle_float3 StartPos;
	//처음 크기
	Particle_float3 StartScl;
	//처음 회전
	Particle_float3 StartRot;
	//처음 색
	Particle_float3 StartColor;
	//마지막 크기
	Particle_float3 EndScl;
	//마지막 회전
	Particle_float3 EndRot;
	//마지막 색
	Particle_float3 EndColor;


	bool isPlay;
private:
	///하위 파티클에서 알필요없는것들
	//생성시간
	float CreateTime;
	//삭제 시간 
	float DeleteTime;
	//파티클 리스트
	vector<Particle*> ParticleList;
private:
	///파티클 시스템 로컬좌표
	XMFLOAT3 Local_Up;
	XMFLOAT3 Local_Look;
	XMFLOAT3 Local_Right;
};
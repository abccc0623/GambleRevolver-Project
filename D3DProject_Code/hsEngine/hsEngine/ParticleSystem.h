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

	//�ؽ��ĸ� �־��ش�
	void SetTexture(ID3D11ShaderResourceView* Texture);

	//��ƼŬ���� ī�޶� ó�ٺ��������ش�
	XMMATRIX CamLookAt();

	//��ƼŬ ������Ʈ
	void UpDateParticle();
	//���� ��ǥ�� ������Ʈ �Ѵ�
	void UpDateLocal();
	void Play();
	void Stop();
public:
	///��ƼŬ ���� �ޱ�
	//���ǵ带 �־��ش� ������ �ƴ϶�� min���� ���ǵ带 �ִ´�
	void SetSpeed(Particle_float speed);
	//��ƼŬ�����ֱ� �����ֱ⸦ �־��ش�
	void SetParticleTime(float mCreate, float mDelete);
	//���� ��ġ�� �־��ش� ��ƼŬ ������Ʈ �߽����� ���ο����ϰ�����
	void SetStartPos(Particle_float3 pos);
	//���� ũ�⸦ �־��ش� 
	void SetStartScl(Particle_float3 scl);
	//���� ȸ���� �־��ش�
	void SetStartRot(Particle_float3 rot);
	//�÷��� �ޱ�
	void SetStartColor(Particle_float3 color);
	//������ �� �ֱ�
	void SetEndColor(Particle_float3 color);
	//��ƼŬ ����ֱ�
	void SetDirection(float x,float y,float z);
	//�ѹ��� ���;��ϴ� ��ƼŬ�� ����
	void SetParticleCount(int count);

	void SetParicleEndTime(float time);
public:
	///��ƼŬ �Լ���
	//��ƼŬ ���
	void PlayParticle(float mCreateTime, float mDeleteTime);
	//��ƼŬ���������ش�
	void CreateParticle(int count);
	//���� ����ǰ��������� ��ƼŬ�� ã�´�
	Particle* FindParticle(int count);
private:
	///��ƼŬ ����
	//�ѹ��� ���;��ϴ� ��ƼŬ�� ��
	int ParticleCount;
	//���� �ֱ�
	float CreateMaxTime;
	//���� �ֱ�
	float DeleteMaxTime;

	float ParticleEndTime;
	float ParticleEndMaxTime;
	//��ƼŬ ����
	XMFLOAT3 direction;
	XMFLOAT3 EndSize;
	//��ƼŬ ���ǵ�
	Particle_float Speed;
	//ó�� ��ġ
	Particle_float3 StartPos;
	//ó�� ũ��
	Particle_float3 StartScl;
	//ó�� ȸ��
	Particle_float3 StartRot;
	//ó�� ��
	Particle_float3 StartColor;
	//������ ũ��
	Particle_float3 EndScl;
	//������ ȸ��
	Particle_float3 EndRot;
	//������ ��
	Particle_float3 EndColor;


	bool isPlay;
private:
	///���� ��ƼŬ���� ���ʿ���°͵�
	//�����ð�
	float CreateTime;
	//���� �ð� 
	float DeleteTime;
	//��ƼŬ ����Ʈ
	vector<Particle*> ParticleList;
private:
	///��ƼŬ �ý��� ������ǥ
	XMFLOAT3 Local_Up;
	XMFLOAT3 Local_Look;
	XMFLOAT3 Local_Right;
};
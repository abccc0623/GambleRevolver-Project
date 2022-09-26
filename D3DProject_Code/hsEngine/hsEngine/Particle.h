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

	//��ƼŬ ������Ʈ
	void ParticleUpdate();
	void Setting();
public:
	///��ƼŬ ����
	//���� �ֱ�
	float CreateMaxTime;
	//���� �ֱ�
	float DeleteMaxTime;
	//��ƼŬ �ӵ�
	float Speed;
	//��ƼŬ ����
	XMFLOAT3 direction;
	//��ƼŬ �÷�
	XMFLOAT3 Color;
	//��ƼŬ�� ���� ��ġ
	XMFLOAT3 StartPos;
	//��ƼŬ�� ���� ������
	XMFLOAT3 StartSize;
	//��ƼŬ�� ���� ȸ��
	XMFLOAT3 StartRot;
	XMFLOAT3 EndSize;

	//�÷��� ���� ��ƼŬ���� �Ǻ�
	bool isStart;
private:
	///���ʿ���°͵�
	float CreateTime;
	float DeleteTime;

	ID3D11Buffer* mWorldBuffer;
public:
	XMFLOAT3 Local_Up;
	XMFLOAT3 Local_Look;
	XMFLOAT3 Local_Right;
};
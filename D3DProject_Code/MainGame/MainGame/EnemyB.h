#pragma once
class EnemyB: public Skinning,public Navigation2D,public Collider
{
public:
	EnemyB(float x = 0,float y = 0,float z= 0);
	virtual~EnemyB();
	virtual void init(hsEngine* mENGINE);
	virtual void Update();
	virtual void Debug();
	virtual void Render(const hsViewData& view);

	void PushHitParticle(ParticleSystem* p);
private:
	void Idle();
	void Attack();
	void Dead();
	void Trace();
	void Defence();

	void HPDown(float Damage);
	void LookAt();
	float GetPlayerDistance();

	ParticleSystem* HitParticle;

	//���µ�
	int State;
	float HP;
	float Speed;
	float BaseSpeed;
	float SkillSpeed;

	//���ݹ���
	float AttackRange;
	//��Ʈ����
	float HitRange;
	float HitTime;

	float AttackTime;

	//��ų ����Ÿ��
	float SkillTime;
	//��ų ��Ÿ��
	float SkillCoolDownTime;
	float SkillCoolTime;

	//�������
	bool isGuard;
	bool isAttack;
	bool isGuardOn;
	bool isDeadSound;
};
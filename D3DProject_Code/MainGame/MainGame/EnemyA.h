#pragma once
class EnemyA:public Skinning, public Navigation2D, public Collider
{
public:
	EnemyA(float x = 0, float y = 0, float z = 0);
	virtual~EnemyA();
	virtual void init(hsEngine* mENGINE);
	virtual void Update();
	virtual void Debug();
	virtual void Render(const hsViewData& view);

	void PushHitParticle(ParticleSystem* p);
	void PushWeapon(GameObject* weapon);
private:
	ParticleSystem* hit_P;
	void Idle();
	void Attack();
	void Dead();
	void Trace();
	float GetPlayerDistance();
	void HPDown(float Damage);
	void LookAt();


	//���µ�
	int State;
	float HP;
	float Speed;
	float AttackRange;
	float AttackTime;
	float DebugSize;
	//��Ʈ����
	float HitRange;
	bool isAttack;
	bool isDeadSound;

	//��ų ����Ÿ��
	float SkillTime;
	//��ų ��Ÿ��
	float SkillCoolDownTime;
	GameObject* weapon;
	hsBone* LiftHand;
	Vector3 testRot;
};
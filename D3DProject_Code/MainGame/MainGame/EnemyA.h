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


	//상태들
	int State;
	float HP;
	float Speed;
	float AttackRange;
	float AttackTime;
	float DebugSize;
	//히트범위
	float HitRange;
	bool isAttack;
	bool isDeadSound;

	//스킬 지속타임
	float SkillTime;
	//스킬 쿨타임
	float SkillCoolDownTime;
	GameObject* weapon;
	hsBone* LiftHand;
	Vector3 testRot;
};
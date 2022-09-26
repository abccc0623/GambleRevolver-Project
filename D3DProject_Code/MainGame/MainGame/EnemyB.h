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

	//상태들
	int State;
	float HP;
	float Speed;
	float BaseSpeed;
	float SkillSpeed;

	//공격범위
	float AttackRange;
	//히트범위
	float HitRange;
	float HitTime;

	float AttackTime;

	//스킬 지속타임
	float SkillTime;
	//스킬 쿨타임
	float SkillCoolDownTime;
	float SkillCoolTime;

	//가드상태
	bool isGuard;
	bool isAttack;
	bool isGuardOn;
	bool isDeadSound;
};
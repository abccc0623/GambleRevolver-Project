#pragma once
class Bullet;
class Boss: public Skinning, public Navigation2D,public Collider
{
public:
	Boss(float x = 0, float y = 0, float z = 0);
	virtual~Boss();
	virtual void init(hsEngine* mENGINE);
	virtual void Update();
	virtual void Debug();
	virtual void Render(const hsViewData& view);

	void PushParticle(ParticleSystem* mhit);
	void PushBullet(Bullet* bullet);
private:
	void Idle();
	void JumpAttack();
	void BaseAttack();
	void GunAttack();
	void Dead();
	void Trace();
	float GetPlayerDistance();
	void HPDown(float Damage);
	void LookAt();
	void Groggy01();
	void Groggy02();

	void CoolDownTime();
	void CoolDownTimeStart();
	
	ParticleSystem* hit;
	int State;
	float Speed;
	float AngrySpeed;
	float BaseSpeed;
	int HP;
	float AttackRange;
	bool isAttack;


	bool isGroggy01;
private:
	float AllSkileTime;
	float AllSkileMaxTime;
	bool OnSkill;
private:
	//총쏘기스킬
	Bullet* BossBullet;
	float GunAttackTime;
	float GunAttackMaxTime;
	int GunAttackCount;
	bool isGunAttackOne;
private:
	///점프 스킬관련 변수들
	float JumpSpeed;
	float jumpAttackTime;
	float jumpAttackMaxTime;
	XMFLOAT3 JumpPoint;
	bool isJumpAttack;
	bool isJumpChack;
};
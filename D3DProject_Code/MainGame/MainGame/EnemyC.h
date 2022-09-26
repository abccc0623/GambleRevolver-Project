#pragma once
class EnemyC : public Skinning,public Navigation2D,public Collider
{
public:
	EnemyC(float x = 0, float y = 0, float z = 0);
	virtual~EnemyC();
	virtual void init(hsEngine* mENGINE);
	virtual void Update();
	virtual void Debug();
	virtual void Render(const hsViewData& view);

	void PushHitParticle(ParticleSystem* p);
	void PushBullet(Bullet* bullet);
	void PushWeapon(GameObject* mWeapon);
private:
	void Idle();
	void Attack();
	void AttackReady();
	void Dead();
	void Trace();
	void Reload();
	void RunAway();
	void FindRunAwayPoint();

	void HPDown(float Damage);
	void LookAt();
	float GetPlayerDistance();

	vector<Bullet*> bulletList;
	GameObject* Weapon;
	ParticleSystem* hitParticle;
	//상태들
	int State;
	float HP;
	float Speed;
	float AttackRange;
	float AttackTime;
	float AttackMaxTime;
	float AttackReadyTime;


	bool isHit;
	int	AttackCount;
	XMFLOAT3 dir;

	float HitRange;
	float DebugSize;


	XMFLOAT3 Right;
	XMFLOAT3 pos;
	//스킬 지속타임
	float SkillTime;
	//스킬 쿨타임
	float SkillCoolDownTime;
	bool isAttackSound;
	bool isReloadSound;
	bool isDeadSound;

};
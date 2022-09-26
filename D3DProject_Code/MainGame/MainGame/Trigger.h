#pragma once
/// <summary>
/// 별거없다 충돌하면 씬을전환하는 오브젝트
/// </summary>
class Trigger :public HsMesh,public Collider
{
public:
	Trigger(float x = 0,float y = 0,float z = 0);
	virtual ~Trigger();

	void init(hsEngine* Engine, LoadData* data);

	// GameObject을(를) 통해 상속됨
	virtual void Update() override;
	virtual void Render(const hsViewData& view) override;
	virtual void Delete() override;
	virtual void Debug() override;
	void SetScreenName(string name);
	void SetParticle(ParticleSystem* p);

	void SetMoney(int num);
	static void SetEnemyCount(int num);
	static void SetEnemyCountDown();

private:
	string ScreenName;
	ParticleSystem* Particle_P;
	int Money = 0;
	static int EnemyCount;
	bool isSound = false;
};
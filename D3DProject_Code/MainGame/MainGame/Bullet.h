#pragma once
class Bullet: public HsMesh,public Collider
{
public:
	Bullet(float x,float y,float z);
	virtual~Bullet();

	void init(hsEngine* ENGINE, LoadData* Data);
	void Shoot(XMFLOAT3 dir, XMFLOAT3 pos);
	void Update();
	void Debug();

	void Setting(int type);
	void BulletSetting(float speed, float max);
	void BulletSetting();
	void SetBulletSpeed(float num);
	bool isShoot;

	void PushParticle(ParticleSystem* p);
private:
	void PlayerSetting();
	void EnemySetting();

	ParticleSystem* mParticle;

	int type;
	XMFLOAT3 MyPos;
	XMFLOAT3 Mydir;
	float bulletTime;
	float bulletSpeed;
	float bulletMaxTime;
};
#pragma once
class CountFont;
class Reload;
class Stage01 : public Screen
{
public:
	Stage01();
	virtual ~Stage01();

	// Screen을(를) 통해 상속됨
	virtual void init() override;
	virtual void Updata() override;
	virtual void Render(const hsViewData& data) override;
	virtual void End();

private:
	///이게 맞는 구조인것같음..씬이 넘어갈때마다 
	///같은 오브젝트라도 옵션이달라질수있으니...색깔놀이같은거
	///최대한 깔끔하게만 하자

	ParticleSystem* test;

	LightObject* dir;

	Image* bulletImg;
	Slider* slider;
	Slider* Skill01;
	Slider* Skill02;
	CountFont* Font;



	void Create_Boss(float x = 0, float y = 0, float z = 0);
	
	void Create_EnemyA(float x = 0, float y = 0, float z = 0);
	void Create_EnemyB(float x = 0, float y = 0, float z = 0);
	void Create_EnemyC(float x = 0, float y = 0, float z = 0);
	void CreateMap(float x = 0, float y = 0, float z = 0);
	void CreateTrigger(int Type, int Money,string stageName,float x = 0, float y = 0, float z = 0);


	Bullet* CreateEnemyBullet(float x = 0, float y = 0, float z = 0);


	//박스를 만든다 테스트용?
	GameObject* Create_Box(float x = 0, float y = 0, float z = 0);


	GameObject* player;
};
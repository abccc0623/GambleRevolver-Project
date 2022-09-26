#pragma once
class CountFont;
class Slider;
class Tutorial :public Screen
{
public:
	Tutorial();
	virtual ~Tutorial();

	hsCamera* Main;
	hsCamera* Sub;


	// Screen을(를) 통해 상속됨
	virtual void init() override;
	virtual void Updata();
	virtual void End();


	void CreateMap(float x=0,float y=0,float z =0);
	CountFont* Create_ImgFont(float x = 0, float y = 0, float z = 0);
	Player* Create_Player(float x = 0, float y = 0, float z = 0);
	Bullet* CreatePlayerBullet(float x = 0, float y = 0, float z = 0);
	GameObject* Create_Box(float x = 0, float y = 0, float z = 0);
	void CreateTrigger(float x, float y, float z);
};
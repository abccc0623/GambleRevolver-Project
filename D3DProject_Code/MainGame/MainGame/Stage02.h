#pragma once

//씬 스테이지 2
class Stage02 :public Screen
{
public:
	Stage02();
	virtual ~Stage02();

	hsCamera* sub;
	virtual void init() override;
	virtual void Updata();
	virtual void End();

	void Create_EnemyA(float x = 0, float y = 0, float z = 0);
	void Create_EnemyB(float x = 0, float y = 0, float z = 0);
	void Create_EnemyC(float x = 0, float y = 0, float z = 0);
	Bullet* CreateEnemyBullet(float x = 0, float y = 0, float z = 0);
private:
	

	//맵을만든다
	void CreateMap(float x = 0, float y = 0, float z = 0);

	void Create_Grid();

	//박스를 만든다 테스트용?
	GameObject* Create_Box(float x = 0, float y = 0, float z = 0);
	void CreateTrigger(int Type, int Money, string stageName, float x=0, float y=0, float z=0);
};
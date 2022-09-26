#pragma once

//�� �������� 2
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
	

	//���������
	void CreateMap(float x = 0, float y = 0, float z = 0);

	void Create_Grid();

	//�ڽ��� ����� �׽�Ʈ��?
	GameObject* Create_Box(float x = 0, float y = 0, float z = 0);
	void CreateTrigger(int Type, int Money, string stageName, float x=0, float y=0, float z=0);
};
#pragma once
class CountFont;
class Reload;
class Stage01 : public Screen
{
public:
	Stage01();
	virtual ~Stage01();

	// Screen��(��) ���� ��ӵ�
	virtual void init() override;
	virtual void Updata() override;
	virtual void Render(const hsViewData& data) override;
	virtual void End();

private:
	///�̰� �´� �����ΰͰ���..���� �Ѿ������ 
	///���� ������Ʈ�� �ɼ��̴޶�����������...������̰�����
	///�ִ��� ����ϰԸ� ����

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


	//�ڽ��� ����� �׽�Ʈ��?
	GameObject* Create_Box(float x = 0, float y = 0, float z = 0);


	GameObject* player;
};
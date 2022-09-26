#pragma once

class Slider;
class CountFont;
class Player : public Skinning, public Collider
{

public:
	Player(float x = 0, float y = 0, float z = 0);
	virtual ~Player();
	void init(hsEngine* ENGINE);

	void Update();
	void Render(const hsViewData& view);
	void Debug();

	//�ҷ����� �÷��̾ �����ϱ⶧����
	void BulletUpdate();
	void BulletRender();
	void BulletDebug();
	void MasterKey();
	void PushUI(Slider* Hp, CountFont* Font, CountFont* MoneyFont,Image* bulletImg);
	void PushGun(GameObject* obj);
	void PushBullet(Bullet* bullet);
	void PushParticle(ParticleSystem* p, ParticleSystem* hit);
	static void Hit(float Damage);
	static XMFLOAT3 GetPlayerPosition();
	static void SetCamMove(bool move);


	static const int GetMoney();
	static void MoneyDown(int num);
	static void MoneyUp(int num);
	static int GetStage();
	static void SetStage(int number);
	static void SetNextStage();

	static void MoneyHP_Up(int num);
	static void MoneyBullet_UP(int num);
	void Reset();
private:
	static int Money;
	vector<Bullet*>bulletList;
	GameObject* weapon;
	static ParticleSystem* AttackParticle;
	static ParticleSystem* HitParticle;

	//������ 
	void Avoid();
	//Ű�Է�
	bool KeyUpdate();
	void AnimetionUpdate();
	//������ ������ ���������غ���
	void LookAt(float x, float z);
	//�÷��̾�� ���� �Ÿ������Ѵ�
	float GetPlayerDistance();
	void Skill01();
private:
	///���¿���
	//������ �ִ���
	static bool isAvoid;
	//�����̰� �ִ���
	bool isMove;
	//�����ϰ��ִ���
	bool isAttack;
	//���ݹ�ư�� ��������
	bool isAttackClick;
	bool isReload;
	bool isSkill01_ON;
	bool isDashSound;
	bool isReloadSound;
	static bool isDead;
private:
	///�⺻ ���µ�
	//������ �Ÿ�
	float Avoidlength;
	//������ ���ǵ�
	float AvoidSpeed;
	//�̵��ӵ�
	float Speed;
	//ü��
	static float HP;
	//���ΰ��� �⺻���̽� ���ǵ� ����������
	float BaseSpeed;
	int BulletCount;
	static int BulletMaxCount;
private:
	///���� �ð��� ��� float��
	//�ѽ�½ð�������
	float AttackTime;
	float AttackMaxTime;
	//���ݾִϸ��̼� �ð�
	float AttackAnimeTime;
	//�����½ð�
	float AvoidTime;
	//�����ð�
	float ReloadTime;
private:
	///ĳ������ ���⺤�͵�
	//ĳ������ ������ ���⺤��
	Vector3 Right;
	//ĳ���Ͱ� �ٶ󺸴��� ���⺤��
	Vector3 Look;
	//Ű�Է� ����
	Vector3 KeyDir;
	//������������ Ű����
	XMFLOAT3 PastKeyDir;

private:
	///��ų1�� �ʿ����Լ���
	Vector3 ClickRight;
	Vector3 ClickLook;
	XMFLOAT2 Click;
	float Skill01_Time;
	float Skill01_MaxTime;
	int SkillDir;
	int SkillBulletCount;
private:
	///����ƽ �Լ���
	//�÷��̾��� ��ġ�� ��𿡼��� �����ü��ֵ���
	static XMFLOAT3 GlobalPlayerPos;
	//�÷��̾ ��𿡼��� �������ֵ���
	static bool CamNOMove;
	XMFLOAT3 Avoid_dir;
	bool ShiftKeyDown;
	static int StageNumber;
private:
	///UI��
	Slider* HP_Slider;
	Slider* Skill_01_Slider;
	Slider* Skill_02_Slider;
	CountFont* BulletCount_Font;
	CountFont* MoneyCount_Font;
	Image* bulletImg;
};
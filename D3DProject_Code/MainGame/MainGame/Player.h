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

	//불렛들은 플레이어에 관리하기때문에
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

	//구르기 
	void Avoid();
	//키입력
	bool KeyUpdate();
	void AnimetionUpdate();
	//방향을 넣으면 그쪽을향해본다
	void LookAt(float x, float z);
	//플레이어와 나의 거리를구한다
	float GetPlayerDistance();
	void Skill01();
private:
	///상태여부
	//구르고 있는지
	static bool isAvoid;
	//움직이고 있는지
	bool isMove;
	//공격하고있는지
	bool isAttack;
	//공격버튼을 눌렀는지
	bool isAttackClick;
	bool isReload;
	bool isSkill01_ON;
	bool isDashSound;
	bool isReloadSound;
	static bool isDead;
private:
	///기본 상태들
	//구르기 거리
	float Avoidlength;
	//구르기 스피드
	float AvoidSpeed;
	//이동속도
	float Speed;
	//체력
	static float HP;
	//주인공의 기본베이스 스피드 변하지않음
	float BaseSpeed;
	int BulletCount;
	static int BulletMaxCount;
private:
	///각종 시간을 재는 float들
	//총쏘는시간을젠다
	float AttackTime;
	float AttackMaxTime;
	//공격애니메이션 시간
	float AttackAnimeTime;
	//구르는시간
	float AvoidTime;
	//장전시간
	float ReloadTime;
private:
	///캐릭터의 방향벡터들
	//캐릭터의 오른쪽 방향벡터
	Vector3 Right;
	//캐릭터가 바라보는쪽 방향벡터
	Vector3 Look;
	//키입력 방향
	Vector3 KeyDir;
	//이전프레임의 키방향
	XMFLOAT3 PastKeyDir;

private:
	///스킬1에 필요한함수들
	Vector3 ClickRight;
	Vector3 ClickLook;
	XMFLOAT2 Click;
	float Skill01_Time;
	float Skill01_MaxTime;
	int SkillDir;
	int SkillBulletCount;
private:
	///스테틱 함수들
	//플레이어의 위치를 어디에서나 가져올수있도록
	static XMFLOAT3 GlobalPlayerPos;
	//플레이어를 어디에서나 때릴수있도록
	static bool CamNOMove;
	XMFLOAT3 Avoid_dir;
	bool ShiftKeyDown;
	static int StageNumber;
private:
	///UI들
	Slider* HP_Slider;
	Slider* Skill_01_Slider;
	Slider* Skill_02_Slider;
	CountFont* BulletCount_Font;
	CountFont* MoneyCount_Font;
	Image* bulletImg;
};
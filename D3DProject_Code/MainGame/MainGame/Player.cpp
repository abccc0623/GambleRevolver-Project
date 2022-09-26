#include "pch.h"
#include "Player.h"

XMFLOAT3 Player::GlobalPlayerPos = { 0,0,0 };
float Player::HP = 1;
int	  Player::Money = 0;
int	  Player::BulletMaxCount = 6;
int	  Player::StageNumber = 0;

bool  Player::isAvoid = false;
bool  Player::isDead = false;
bool  Player::CamNOMove = false;
ParticleSystem* Player::HitParticle = nullptr;
ParticleSystem* Player::AttackParticle = nullptr;


Player::Player(float x, float y, float z)
{
	transfrom.Position = { x,y,z };
	Speed = 3.5f;
	Avoidlength = 1;
	HP = 1;
	isAvoid = false;
	ShiftKeyDown = false;
	BaseSpeed = 3.5f;
	
	AttackTime = 0;
	AttackMaxTime = 0.5f;
	BulletCount = 6;
	ReloadTime = 0;
	BulletMaxCount = 6;
	PastKeyDir = { 1,0,1 };
}

Player::~Player()
{

}

void Player::init(hsEngine* mENGINE)
{
	GlobalPlayerPos = transfrom.Position;
	transfrom.Scale = { 2,2 ,2 };
	transfrom.Rotation = { 90,0,0 };

	Skinning::init(mENGINE);
	float size = 0.25f;
	//디버깅 생성
	Debugging::Create_Box_Buffers(XMFLOAT3(0, 1 * size, 0), XMFLOAT3(1 * size, 1 * size, 1 * size), XMFLOAT4(0, 0, 1, 1));
	Debugging::Create_2D_Y_Circle_Buffers(XMFLOAT3(0, 1 * size, 0), 1*size, XMFLOAT4(1, 1, 0, 1));
	Debugging::Create_Line_Buffers(XMFLOAT3(0, 0, 0), XMFLOAT3(1 * size, 0, 0), XMFLOAT4(0, 0, 1, 1));
	Debugging::Create_Line_Buffers(XMFLOAT3(0, 0, 0), XMFLOAT3(0, 1 * size, 0), XMFLOAT4(0, 1, 0, 1));
	Debugging::Create_Line_Buffers(XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, -1 * size), XMFLOAT4(1, 0, 0, 1));
	Debugging::CreateDebugBuffers();
	//충돌 객체
	Collider::SetBoundingSphere(transfrom.Position, 0.5f,Collider::Player);
}

void Player::Update()
{
	if (isDead == true)
	{
		Speed = 0;
		int End = MeshAnimation::PlayAnimation("PlayerDead", 30,false);

		if (End >= MeshAnimation::GetEndFrame()-2)
		{
			Reset();
			return;
		}
	}

	BulletCount_Font->SetCount(BulletCount);
	MoneyCount_Font->SetCount(Money);
	HP_Slider->SetHandleValue(HP);
	KeyUpdate();
	MasterKey();
	//구르기
	if (ENGINE->GetKey()->GetKeyDown(VK_SPACE))
	{
		isAvoid = true;
	}

	if (isDead == false)
	{
		AnimetionUpdate();
	}

	Skill01();
	Avoid();

	if (weapon != nullptr)
	{
		weapon->UpdateParentTr();
		weapon->Update();
		weapon->transfrom.Rotation = {0,90,70};
	}

	//다른놈들에게 나의 좌표를넘겨줄 위치
	GlobalPlayerPos = transfrom.Position;
	//충돌 검사를 한다
	BulletUpdate();
	Collider::Updata(XMFLOAT3(transfrom.Position.x,1, transfrom.Position.z));
	Skinning::Update();
}

void Player::Render(const hsViewData& view)
{
	if (weapon != nullptr)
	{
		weapon->Render(view);
	}

	BulletRender();
	Skinning::Render(view);
}

void Player::Debug()
{
	XMMATRIX T = transfrom.SetPosition4X4(XMFLOAT3(transfrom.Position.x, transfrom.Position.y, transfrom.Position.z));
	XMMATRIX R = transfrom.SetRotation4X4(XMFLOAT3(transfrom.Rotation.x - 90, transfrom.Rotation.z, -transfrom.Rotation.y));
	XMMATRIX S = transfrom.SetScale4X4(XMFLOAT3(1, 1, 1));
	XMMATRIX mDebug = S * R * T;
	BulletDebug();
	Debugging::Update(mDebug);
}

void Player::BulletUpdate()
{
	for (int i = 0; i < bulletList.size(); i++)
	{
		if (bulletList[i]->OnRender == true)
		{
			bulletList[i]->Update();
		}
	}
}

void Player::BulletRender()
{
	for (int i = 0; i < bulletList.size(); i++)
	{
		if (bulletList[i]->OnRender == true)
		{
			bulletList[i]->Render(hsViewData());
		}
	}
}

void Player::BulletDebug()
{
	for (int i = 0; i < bulletList.size(); i++)
	{
		if (bulletList[i]->OnRender == true)
		{
			bulletList[i]->Debug();
		}
	}
}

void Player::MasterKey()
{
	if (ENGINE->GetKey()->GetKeyDown(VK_F9))
	{
		Player::SetStage(3);
		Trigger::SetEnemyCount(0);
		ScreenManager::MoveScreen("Bonus");
	}
	if (ENGINE->GetKey()->GetKeyDown(VK_F10))
	{
		Player::SetStage(7);
		Trigger::SetEnemyCount(0);
		ScreenManager::MoveScreen("Bonus");
	}
	if (ENGINE->GetKey()->GetKeyDown(VK_F11))
	{
		Player::SetStage(10);
		Trigger::SetEnemyCount(0);
		ScreenManager::MoveScreen("Bonus");
	}
	if (ENGINE->GetKey()->GetKeyDown(VK_F12))
	{
		Player::SetStage(13);
		Trigger::SetEnemyCount(0);
		ScreenManager::MoveScreen("Bonus");
	}
	//칩마스터
	if (ENGINE->GetKey()->GetKeyDown(VK_F8))
	{
		MoneyUp(5000);
	}

}

void Player::PushUI(Slider* mHp, CountFont* mFont, CountFont* MoneyFont, Image* mbulletImg)
{
	HP_Slider = mHp;
	BulletCount_Font = mFont;
	MoneyCount_Font = MoneyFont;
	bulletImg = mbulletImg;
}

void Player::PushGun(GameObject* obj)
{
	hsBone* LiftHand = Skinning::FindBone("mixamorig:RightHand");
	weapon = obj;
	obj->ParentName = "mixamorig:RightHand";
	obj->SetParentTR(&(LiftHand->transfrom));
}

void Player::PushBullet(Bullet* bullet)
{
	bulletList.push_back(bullet);
}

void Player::PushParticle(ParticleSystem* p, ParticleSystem* hit)
{
	AttackParticle = p;
	HitParticle = hit;
}

XMFLOAT3 Player::GetPlayerPosition()
{
	return GlobalPlayerPos;
}

void Player::SetCamMove(bool move)
{
	CamNOMove = move;
}

const int Player::GetMoney()
{
	return Money;
}

void Player::MoneyDown(int num)
{
	Money -= num;
}

void Player::MoneyUp(int num)
{
	Money += num;
}

int Player::GetStage()
{
	return StageNumber;
}

void Player::SetStage(int number)
{
	StageNumber = number;
}

void Player::SetNextStage()
{
	StageNumber++;
}

void Player::MoneyHP_Up(int num)
{
	HP += num* 0.01f;
	if (HP >= 1)
	{
		HP = 1;
	}
}

void Player::MoneyBullet_UP(int num)
{
	BulletMaxCount += num;
}

void Player::Reset()
{
	Money = 0;
	HP_Slider = nullptr;
	BulletCount_Font = nullptr;
	bulletImg = nullptr;
	AttackParticle->isDelete = true;
	HitParticle->isDelete = true;
	Speed = BaseSpeed;
	AttackParticle = nullptr;
	HitParticle = nullptr;
	StageNumber = 0;
	delete weapon;
	weapon = nullptr;
	isDelete = true;
	SetStage(-1);
	ScreenManager::MoveScreen("GameOver");
	isDead = false;
}



void Player::Avoid()
{

	if (isAvoid == true  && isAttack == false)
	{
		//구르기 시간을 잰다
		AvoidTime += ENGINE->GetDeltaTime();
		MeshAnimation::PlayAnimation("PlayerRoll",37,false);
		
		if (AvoidTime <= 1)
		{
			float x = Avoid_dir.x;
			float z = Avoid_dir.z;
			LookAt(x, z);
			if (isDashSound == false)
			{
				hsSound::GM()->Play_EXSound("Dash");
				isDashSound = true;
			}
			float speed = 3.0f * ENGINE->GetDeltaTime();
			float n = sqrt((x * x) + (z * z));
			x /= n;
			z /= n;

			XMFLOAT3 dir = XMFLOAT3(x, 0, z) * speed;
			//구르기 시간만큼 이동
			float Posx = transfrom.Position.x + dir.x;
			float Posz = transfrom.Position.z + dir.z;
			//맵이동을막음
			if (Navigation2D::FindObject(Posx, Posz) == false)
			{
				transfrom.SetTranlate(dir.x, dir.y, dir.z);
			}

			KeyDir = Avoid_dir;

			Speed = 0;
		}
		else
		{
			//구르기가끝나면 초기화
			AvoidTime = 0;
			Speed = BaseSpeed;
			isAvoid = false;
			isDashSound = false;
		}
	}
	else
	{
		if (PastKeyDir.x != 0 && PastKeyDir.z != 0)
		{
			Avoid_dir = PastKeyDir;
		}
	}

}

bool Player::KeyUpdate()
{
	//디버깅 모드일때는 작동하지않음
	
	bool KeyDown = false;

	//플레이어를 카메라가 보는방향으로 움직이게하기위해
	//카메라의 right벡터
	Right = CamManager::GetMainCamRightVector();
	//플레이어의 업벡터
	XMFLOAT3 Up = XMFLOAT3(0, 1, 0);
	//이두개를 외적하면 플레이어의 Look벡터가 나옴
	Look = XMFLOAT3(0, 0, 0);
	//두개의벡터의 외적을구함
	DirectX::XMVECTOR R = XMLoadFloat3(&Right);
	DirectX::XMVECTOR U = XMLoadFloat3(&Up);
	U = XMVector3Cross(R, U);
	//플레이어의 앞방향 구함
	XMStoreFloat3(&Look, U);

	float RightN = sqrt((Right.x * Right.x) + (Right.z * Right.z));
	float LookN = sqrt((Look.x * Look.x) + (Look.z * Look.z));
	Right.x /= RightN;
	Right.z /= RightN;
	Look.x /= LookN;
	Look.z /= LookN;



	//XMFLOAT3 move = {Right}
	

	KeyDir = { 0,0,0 };
	//캐릭터를 이동 시켜줌
	float MoveSpeed = Speed * ENGINE->GetDeltaTime();
	if (ENGINE->GetKey()->GetKey('D'))
	{
		KeyDir += Right;
		Vector3 mRight = Right* Speed * ENGINE->GetDeltaTime();
		if (Navigation2D::FindObject(transfrom.Position.x + mRight.x, transfrom.Position.z + mRight.z ) == false)
		{
			transfrom.SetTranlate(mRight.x,0, mRight.z);
		}
		KeyDown = true;
	}
	if (ENGINE->GetKey()->GetKey('A'))
	{
		KeyDir -= Right;
		Vector3 mRight = -Right  * Speed * ENGINE->GetDeltaTime();
		if (Navigation2D::FindObject(transfrom.Position.x  + mRight.x, transfrom.Position.z  + mRight.z ) == false)
		{
			transfrom.SetTranlate(mRight.x, 0, mRight.z);
		}
		KeyDown = true;
	}
	if (ENGINE->GetKey()->GetKey('W'))
	{
		KeyDir += Look;
		Vector3 mLook = Look * Speed * ENGINE->GetDeltaTime();
		if (Navigation2D::FindObject(transfrom.Position.x + mLook.x, transfrom.Position.z  + mLook.z) == false)
		{
			transfrom.SetTranlate(mLook.x, 0, mLook.z);
		}
		KeyDown = true;
	}
	if (ENGINE->GetKey()->GetKey('S'))
	{
		KeyDir -= Look ;
		Vector3 mLook = -Look * Speed * ENGINE->GetDeltaTime();
		if (Navigation2D::FindObject(transfrom.Position.x  + mLook.x, transfrom.Position.z + mLook.z) == false)
		{
			transfrom.SetTranlate(mLook.x, 0, mLook.z);
		}
		KeyDown = true;
	}


	if (ENGINE->GetKey()->GetKeyDown('R') && isReload == false)
	{
		hsSound::GM()->Play_EXSound("PlayerReload");
		isReload = true;
	}




	hsCamera* Main = CamManager::GetMainCam();

	if (ENGINE->OnDebug == false)
	{
		if (CamNOMove == true)
		{
			//디버깅 상태가 아닐경우에만 화면고정으로
			Vector3 Back = -Look * 10;
			Main->transfrom.Position = { transfrom.Position.x + Back.x,10,transfrom.Position.z + Back.z };
			Main->transfrom.Rotation = { 45, 45, 0 };
		}
		else
		{
			Main->transfrom.Position = { -5,8,-15 };
			Main->transfrom.Rotation = { 20,45,0 };
		}
	}
	
	XMFLOAT3 dir = KeyDir;
	
	//이동하고있을때의 키의방향을 가져옴 이동안할떄는  계속이동했을때의 키를가져옴
	if (KeyDir.x != 0 && KeyDir.z != 0)
	{
		PastKeyDir = KeyDir;
	}


	//어택 딜레이를 주기위해
	AttackTime += ENGINE->GetDeltaTime();
	//왼쪽버튼을 눌렀을떄
	if (ENGINE->GetKey()->GetKeyDown(VK_LBUTTON) && isAvoid == false && isReload ==false)
	{
		if (BulletCount <= 0) { isReload = true; return false; }
		if (AttackTime <= AttackMaxTime) { return false; }
		hsSound::GM()->Play_EXSound("PlayerAttack");
		isAttackClick = true;
		//클릭한지점의 좌표를 보게한다
		XMFLOAT3 ClickPoint = Main->GetClickPoint();
		float x = ClickPoint.x - transfrom.Position.x;
		float z = ClickPoint.z - transfrom.Position.z;
		float n = sqrt((x * x) + (z * z));
		x /= n;
		z /= n;
		LookAt(x, z);
		for (int i = 0;i< bulletList.size(); i++)
		{
			if (bulletList[i]->isShoot == false)
			{
				bulletList[i]->BulletSetting();
				bulletList[i]->Shoot(XMFLOAT3(x,0,z), transfrom.Position);
				
				bulletList[i]->transfrom.Position = { transfrom.Position.x +(x ),1.25f,transfrom.Position.z+z };
				AttackParticle->transfrom.Position = { transfrom.Position.x + (x),1.25f,transfrom.Position.z + z };
				isAttack = true;
				AttackTime = 0;
				Speed = 0;
				BulletCount--;
				break;
			}
		}
	}
	else
	{
		//각도설정 처음캐릭터는 x축으로 돌아가있고 -z방향을 보고있기떄문에 90도를 빼준다 (오른쪽을바라보고있도록하기위해)
		if (KeyDir != XMFLOAT3(0, 0, 0) && isAvoid == false && Speed != 0)
		{
			LookAt(dir.x, dir.z);
			isMove = true;
		}
		else if (isAvoid == false)
		{
			isMove = false;
		}
	}
	return KeyDown;
}

void Player::AnimetionUpdate()
{
	if (isAttack == true)
	{
		AttackAnimeTime += ENGINE->GetDeltaTime();
		AttackParticle->Play();
		int AttackEnd = MeshAnimation::PlayAnimation("PlayerAttack", 50, false);
		if (AttackEnd == MeshAnimation::GetEndFrame()/2 && isAttackClick == true)
		{
			isAttackClick = false;
		}



		if (AttackAnimeTime >= 0.5f)
		{
			AttackAnimeTime = 0;
			isAttack = false;
			isAttackClick = false;
			AttackParticle->Stop();
			Speed = BaseSpeed;
		}
		return;
	}


	if (isAvoid == false)
	{
		if (isMove == true)
		{
			if (isReload == true)
			{
				ReloadTime += ENGINE->GetDeltaTime();
				MeshAnimation::PlayAnimation("PlayerReloadRun", 30);
				bulletImg->ChangeImg("Reload");
				if (isReloadSound == false) 
				{
					hsSound::GM()->Play_EXSound("PlayerReload");
					isReloadSound = true;
				}
			}
			else
			{
				MeshAnimation::PlayAnimation("PlayerRun", 45);
			}
		}
		else
		{
			if (isReload == true)
			{
				ReloadTime += ENGINE->GetDeltaTime();
				MeshAnimation::PlayAnimation("PlayerReloadIdle", 30);
				bulletImg->ChangeImg("Reload");
				if (isReloadSound == false)
				{
					hsSound::GM()->Play_EXSound("PlayerReload");
					isReloadSound = true;
				}
			}
			else
			{
				MeshAnimation::PlayAnimation("PlayerIdle", 30);
			}
		}
	}



	if (ReloadTime >= 3)
	{
		ReloadTime = 0;
		BulletCount = BulletMaxCount;
		bulletImg->ChangeImg("Main");
		isReload = false;
		isReloadSound = false;
	}
}

void Player::LookAt(float x, float z)
{
	float Angle = -atan2(z, x) * 180 / MathHelper::Pi;
	transfrom.Rotation = { 90,0,90 - Angle };
}

float Player::GetPlayerDistance()
{
	XMFLOAT3 PlayerPos = Player::GetPlayerPosition();
	float x = PlayerPos.x - transfrom.Position.x;
	float z = PlayerPos.z - transfrom.Position.z;
	

	return sqrt((x * x) + (z * z));
}

void Player::Skill01()
{
	if (ENGINE->GetKey()->GetKeyDown(VK_RBUTTON) && isAvoid == false && isReload == false)
	{
		hsCamera* Main = CamManager::GetMainCam();
		if (BulletCount <= 0) { isReload = true; return; }
		if (AttackTime <= AttackMaxTime) { return; }

		isSkill01_ON = true;
		isAttackClick = true;
		//클릭한지점의 좌표를 보게한다
		XMFLOAT3 ClickPoint = Main->GetClickPoint();
		
		float x = ClickPoint.x - transfrom.Position.x;
		float z = ClickPoint.z - transfrom.Position.z;
		float n = sqrt((x * x) + (z * z));
		x /= n;
		z /= n;

		Click.x = x;
		Click.y = z;
		LookAt(x, z);
		SkillBulletCount = 0;
		SkillDir = -3;
	}

	if (isSkill01_ON == true)
	{
		//총알이 없을땐 스킬을멈춘다
		if (BulletCount <= 0)
		{
			isReload = true;
			isSkill01_ON = false;
		}


		Speed = 0;
		Skill01_Time += ENGINE->GetDeltaTime();

		//PlayAnimation("PlayerAttack", 40);

		if (Skill01_Time >= 0.1f)
		{
			
			XMFLOAT3 look = XMFLOAT3(Click.x, 0, Click.y);
			XMFLOAT3 Up = XMFLOAT3(0, 1, 0);
			//두개의벡터의 외적을구함
			DirectX::XMVECTOR L = XMLoadFloat3(&look);
			DirectX::XMVECTOR U = XMLoadFloat3(&Up);
			U = XMVector3Cross(L, U);
			//플레이어의 앞방향 구함
			XMStoreFloat3(&ClickRight, U);
			




			for (int i = 0; i < bulletList.size(); i++)
			{
				if (bulletList[i]->isShoot == false)
				{
					float x = (look.x+ (ClickRight.x* (0.05f* SkillDir)));
					float y = (look.z+ (ClickRight.y* (0.05f* SkillDir)));
					bulletList[i]->BulletSetting();
					bulletList[i]->Shoot(XMFLOAT3(x, 0, y), transfrom.Position);
					bulletList[i]->transfrom.Position = { transfrom.Position.x + x,1.25f,transfrom.Position.z + y };
					AttackParticle->transfrom.Position = { transfrom.Position.x + x,1.25f,transfrom.Position.z + y };
					bulletList[i]->BulletSetting(25, 0.2f);


					hsSound::GM()->Play_EXSound("PlayerAttack");
					isAttack = true;
					AttackTime = 0;
					Speed = 0;
					BulletCount--;
					SkillDir++;
					SkillBulletCount++;
					Skill01_Time = 0;
					break;
				}
			}
		}


		if (SkillBulletCount >= 3)
		{
			isSkill01_ON = false;
			SkillBulletCount = 0;
			Skill01_Time = 0;
			SkillDir = -3;
		}

	}
}



void Player::Hit(float Damage)
{
	if (isAvoid == true) { return; }
	if (isDead == true) { return; }

	HP -= Damage;
	HitParticle->transfrom.Position = Player::GetPlayerPosition();
	HitParticle->Play();

	hsSound::GM()->Play_EXSound("PlayerHit");
	if (HP <= 0)
	{
		isDead = true;
		hsSound::GM()->Play_EXSound("PlayerDead");
	}
}



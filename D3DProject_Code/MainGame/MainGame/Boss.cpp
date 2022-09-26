#include "pch.h"
#include "Boss.h"

Boss::Boss(float x, float y, float z)
{
	transfrom.Position = { x,y,z };
	State = 0;
	Speed = 2.5f;
	HP = 6000;
	AttackRange = 3;
	isAttack = false;
	isJumpAttack = false;
	jumpAttackMaxTime = 5;
	JumpSpeed = 3;

	GunAttackTime = 0;
	GunAttackMaxTime = 1.4f;
	GunAttackCount = 0;

	AngrySpeed = 4.5f;
	BaseSpeed = 2.5f;

	AllSkileTime = 0;
	AllSkileMaxTime = 5;
}

Boss::~Boss()
{

}

void Boss::init(hsEngine* mENGINE)
{
	transfrom.Scale = { 0.03f,0.03f,0.03f };
	

	Skinning::init(mENGINE);

	//디버깅 생성
	Debugging::Create_Box_Buffers(XMFLOAT3(0, 500, 0), XMFLOAT3(500, 500, 500), XMFLOAT4(0, 0, 1, 1));
	Debugging::Create_2D_Y_Circle_Buffers(XMFLOAT3(0, 500, 0), 1000, XMFLOAT4(1, 1, 0, 1));
	Debugging::Create_Line_Buffers(XMFLOAT3(0, 0, 0), XMFLOAT3(1 * 1000, 0, 0), XMFLOAT4(0, 0, 1, 1));
	Debugging::Create_Line_Buffers(XMFLOAT3(0, 0, 0), XMFLOAT3(0, 1 * 1000, 0), XMFLOAT4(0, 1, 0, 1));
	Debugging::Create_Line_Buffers(XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, -1 * 1000), XMFLOAT4(1, 0, 0, 1));
	Debugging::CreateDebugBuffers();


	Collider::SetBoundingSphere(transfrom.Position, 1, Collider::Enemy);
}

void Boss::Update()
{
	//사거리가 짧아지면 일반공격
	//if (GetPlayerDistance()  < 3)
	//{
	//	State = 4;
	//}

	Collider::Updata(XMFLOAT3(transfrom.Position.x, 1, transfrom.Position.z));
	if (Collider::OnCollisionStay())
	{
		//히트 오브젝트와 충돌하면 HP 감소
		if (Collider::SphereCollider.HitType & Collider::hitObj)
		{
			HPDown(100);
			hsSound::GM()->Play_EXSound("hit");
			hit->transfrom.Position = transfrom.Position;
			hit->Play();
		}
	}

	CoolDownTime();
	

	//State = 4;

	switch (State)
	{
	case 0:
		Idle();
		break;
	case 1:
		Trace();
		break;
	case 2://점프 공격
		JumpAttack();
		break;
	case 3://원거리 공격
		GunAttack();
		break;
	case 4://기본공격
		BaseAttack();
		break;
	case 5://그로기상태
		Groggy01();
		break;
	case 6://그로기상태
		Groggy02();
		break;
	case 7://죽음
		Dead();
		break;
	}


	if (BossBullet != nullptr)
	{
		BossBullet->Update();
	}

	Collider::Updata(XMFLOAT3(transfrom.Position.x, 1, transfrom.Position.z));
	Skinning::Update();
}

void Boss::Debug()
{
	//축을 맞추기위해 ... 
	XMMATRIX T = transfrom.SetPosition4X4(XMFLOAT3(transfrom.Position.x, transfrom.Position.y, transfrom.Position.z));
	XMMATRIX R = transfrom.SetRotation4X4(XMFLOAT3(transfrom.Rotation.x - 90, transfrom.Rotation.z, -transfrom.Rotation.y));
	XMMATRIX S = transfrom.SetScale4X4(XMFLOAT3(1, 1, 1));
	XMMATRIX mDebug = S * R * T;

	
	Debugging::Update(mDebug);
}

void Boss::Render(const hsViewData& view)
{

	if (BossBullet != nullptr)
	{
		BossBullet->Render(hsViewData());
	}
	Skinning::Render(view);
}

void Boss::PushParticle(ParticleSystem* mhit)
{
	//히트 파티클
	hit = mhit;
}

void Boss::PushBullet(Bullet* bullet)
{
	BossBullet = bullet;
}

void Boss::Idle()
{
	Navigation2D::SetStartPoint(transfrom.Position.x, transfrom.Position.z);
	Navigation2D::SetEndPoint(Player::GetPlayerPosition().x, Player::GetPlayerPosition().z);
	Navigation2D::StartNavigation();

	State = 1;
}


void Boss::BaseAttack()
{
	LookAt();
	int EndAnime = MeshAnimation::PlayAnimation("BossPunch01",30);
	if (EndAnime == MeshAnimation::GetEndFrame()/2)
	{
		if (isAttack == false)
		{
			Player::Hit(0.1f);
			hsSound::GM()->Play_EXSound("EnemyB_Attack");
			isAttack = true;
		}
	}
	else if (EndAnime == MeshAnimation::GetEndFrame() - 2)
	{
		isAttack = false;
		State = 0;
	}
}

void Boss::GunAttack()
{
	int EndAnim= MeshAnimation::PlayAnimation("BossSkill02", 50);
	LookAt();
	if (EndAnim == MeshAnimation::GetEndFrame() -2)
	{
		if (isGunAttackOne == false)
		{
			//원거리 공격
			float x = Player::GetPlayerPosition().x - transfrom.Position.x;
			float z = Player::GetPlayerPosition().z - transfrom.Position.z;

			float n = sqrt((x * x) + (z * z));
			x /= n;
			z /= n;
			XMFLOAT3 pos = { transfrom.Position.x + x ,1,transfrom.Position.z + z };

			hsSound::GM()->Play_EXSound("EnemyC_Attack");

			BossBullet->Shoot(XMFLOAT3(x, 1.25f, z), pos);
			GunAttackTime = 0;
			GunAttackCount++;
			isGunAttackOne = true;
			if (GunAttackCount >= 3)
			{
				State = 0;
				GunAttackCount = 0;
				CoolDownTimeStart();
			}
		}
	}

	if (EndAnim == 0)
	{
		isGunAttackOne = false;
	}
}


void Boss::Dead()
{
	int EndAnime = MeshAnimation::PlayAnimation("BossDead", 30,false);
	if (EndAnime >= GetEndFrame() - 2)
	{
		transfrom.SetPosition(0, -ENGINE->GetDeltaTime(), 0);
	}
}

void Boss::Trace()
{
	MeshAnimation::PlayAnimation("BossRun", 30);
	
	//네비게션에 따라 움직인다
	XMFLOAT3 rot = { 0,0,transfrom.Rotation.y};
	bool isMoveEnd = Navigation2D::NavigationMove(transfrom.Position, rot, Speed * ENGINE->GetDeltaTime(), 0.4f);
	transfrom.Rotation = { 0,-rot.z * -1,0 };
	


	//만약 도착지점에 왔다면 새롭게 위치를 잡아줘야함
	if (isMoveEnd == true)
	{
		Navigation2D::SetStartPoint(transfrom.Position.x, transfrom.Position.z);
		Navigation2D::SetEndPoint(Player::GetPlayerPosition().x, Player::GetPlayerPosition().z);
		Navigation2D::StartNavigation();
		isMoveEnd = false;
	}
}

void Boss::HPDown(float Damage)
{
	HP -= Damage;

	if (HP == 3000)
	{
		State = 5;
		isGroggy01 = true;
	}


	if (HP <= 0)
	{
		State = 7;
	}
}

void Boss::LookAt()
{
	float x = transfrom.Position.x - Player::GetPlayerPosition().x;
	float z = transfrom.Position.z - Player::GetPlayerPosition().z;

	//그곳의 각도를 구함
	float angle = -atan2(z, x) * 180 / MathHelper::Pi;
	//각도설정 처음캐릭터는 x축으로 돌아가있고 -z방향을 보고있기떄문에 90도를 빼준다 (오른쪽을바라보고있도록하기위해)

	transfrom.Rotation = { 0,-angle - 90,0 };
}

void Boss::JumpAttack()
{
	int EndAnime = MeshAnimation::PlayAnimation("BossPunch03",40);

	float DTime = ENGINE->GetDeltaTime();
	XMFLOAT3 MyPos = transfrom.Position;
	XMFLOAT3 Dir = Player::GetPlayerPosition() - MyPos;
	float n = sqrt((Dir.x * Dir.x) + (Dir.z * Dir.z));
	Dir.x /= n;
	Dir.z /= n;
	transfrom.SetTranlate(Dir.x* JumpSpeed *DTime, 0, Dir.z* JumpSpeed *DTime);


	if (EndAnime == MeshAnimation::GetEndFrame() -50) 
	{
		//상태를 되돌림
		State = 0;
		isJumpAttack = true;
		//공격이끝났을때 플레이어와의 거리가 짧으면데미지
		if (GetPlayerDistance() <= 4)
		{
			Player::Hit(0.1f);
		}
		CoolDownTimeStart();
	}
}

void Boss::Groggy01()
{
	int EndAnime1 = MeshAnimation::PlayAnimation("BossGroggy1", 30);
	if (EndAnime1 == MeshAnimation::GetEndFrame() - 2)
	{
		State = 6;
	}

}
void Boss::Groggy02()
{
	LookAt();
	int EndAnime1 =  MeshAnimation::PlayAnimation("BossGroggy2", 30);
	if (EndAnime1 == MeshAnimation::GetEndFrame() - 2)
	{
		State = 0;
		Speed = AngrySpeed;
		isGroggy01 = false;
	}
}
void Boss::CoolDownTime()
{
	if (OnSkill == false && isGroggy01 == false)
	{
		float DTime = ENGINE->GetDeltaTime();
		AllSkileTime += DTime;
		if (AllSkileTime >= AllSkileMaxTime)
		{
			srand(time(nullptr));
			int num = (rand()) % (int)2;
			
			State = num+2;
			OnSkill = true;
		}
	}
	else
	{
		//if (GetPlayerDistance()  < 3)
		//{
		//	State = 4;
		//}
	}
}
void Boss::CoolDownTimeStart()
{
	OnSkill = false;
	AllSkileTime = 0;
}
float Boss::GetPlayerDistance()
{
	XMFLOAT3 PlayerPos = Player::GetPlayerPosition();
	float x = PlayerPos.x - transfrom.Position.x;
	float z = PlayerPos.z - transfrom.Position.z;


	return sqrt((x * x) + (z * z));
}
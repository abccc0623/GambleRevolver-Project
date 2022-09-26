#include "pch.h"
#include "EnemyA.h"

EnemyA::EnemyA(float x, float y, float z)
{
	transfrom.Position = XMFLOAT3(x, y, z);
	State = 0;
	Speed = 4.5f;
	HP = 100;

	AttackRange = 1.5f; //사거리 크기가줄어든만큼
	DebugSize = 40;
	HitRange = 0.5f;
}

EnemyA::~EnemyA()
{
	delete weapon;
	weapon = nullptr;
}

void EnemyA::init(hsEngine* mENGINE)
{
	transfrom.Scale = { 0.025f,0.025f ,0.025f };
	transfrom.Rotation = { 90,0,0 };

	Skinning::init(mENGINE);

	float size = 20;
	//디버깅 생성
	Debugging::Create_2D_Y_Circle_Buffers(XMFLOAT3(0, 1 * size, 0), DebugSize * AttackRange, XMFLOAT4(1, 1, 0, 1));
	Debugging::Create_2D_Y_Circle_Buffers(XMFLOAT3(0, 1 * size, 0), DebugSize * HitRange, XMFLOAT4(0, 1, 0, 1));
	Debugging::Create_Line_Buffers(XMFLOAT3(0, 0, 0), XMFLOAT3(1 * size, 0, 0), XMFLOAT4(0, 0, 1, 1));
	Debugging::Create_Line_Buffers(XMFLOAT3(0, 0, 0), XMFLOAT3(0, 1 * size, 0), XMFLOAT4(0, 1, 0, 1));
	Debugging::Create_Line_Buffers(XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, -1 * size), XMFLOAT4(1, 0, 0, 1));
	Debugging::CreateDebugBuffers();


	Speed = 2+ (rand()) % (int)5;
	Collider::SetBoundingSphere(transfrom.Position, HitRange,Collider::Enemy);
}

void EnemyA::Update()
{
	time(nullptr);
	if (isDelete == true) { return; }
	//충돌 처리

	if (State != 3)
	{
		Collider::Updata(XMFLOAT3(transfrom.Position.x, 1, transfrom.Position.z));
		if (Collider::OnCollisionStay())
		{
			if (Collider::SphereCollider.HitType & Collider::hitObj)
			{
				HPDown(51);
				hit_P->Play();
				hit_P->transfrom.Position = transfrom.Position;				
			}
		}

		//플레이어와 거리측정
		if (GetPlayerDistance() <= AttackRange)
		{
			State = 2;
		}
	}

	switch (State)
	{
	case 0: //Idle
		Idle();
		break;
	case 1: //Trace
		Trace();
		break;
	case 2: //Attack
		Attack();
		break;
	case 3: //Dead
		Dead();
		break;
	}
	
	
	if (weapon != nullptr)
	{
		weapon->Update();
		weapon->UpdateParentTr();
	}
	Skinning::Update();
}

void EnemyA::Debug()
{
	XMMATRIX T = transfrom.SetPosition4X4(XMFLOAT3(transfrom.Position.x, transfrom.Position.y, transfrom.Position.z));
	XMMATRIX R = transfrom.SetRotation4X4(XMFLOAT3(transfrom.Rotation.x - 90, transfrom.Rotation.z, -transfrom.Rotation.y));
	XMMATRIX S = transfrom.SetScale4X4(XMFLOAT3(1, 1, 1));
	XMMATRIX mDebug = S * R * T;

	Debugging::Update(mDebug);
}

void EnemyA::Render(const hsViewData& view)
{
	if (weapon != nullptr) 
	{
		weapon->Render(view);
	}
	Skinning::Render(view);
}

void EnemyA::PushHitParticle(ParticleSystem* p)
{
	hit_P = p;
}

void EnemyA::PushWeapon(GameObject* mWeapon)
{
	LiftHand = Skinning::FindBone("mixamorig:RightHand");
	weapon = mWeapon;
	weapon->SetParentTR(&LiftHand->transfrom);
}

void EnemyA::Idle()
{
	Navigation2D::SetStartPoint(transfrom.Position.x, transfrom.Position.z);
	Navigation2D::SetEndPoint(Player::GetPlayerPosition().x, Player::GetPlayerPosition().z);
	Navigation2D::StartNavigation();

	State = 1;
}

void EnemyA::Attack()
{
	int EndAnime = MeshAnimation::PlayAnimation("EnemyAAttack1", 110);
	LookAt();
	//충돌을 하지않았거나 충돌했어도 다른적과충돌했다면 통과
	if (Collider::OnCollisionStay() == false || Collider::SphereCollider.HitType == Collider::Enemy)
	{
		//애니메이션이 끝났다면통과
		if (EndAnime >= MeshAnimation::GetEndFrame() - 2)
		{
			Navigation2D::SetStartPoint(transfrom.Position.x, transfrom.Position.z);
			Navigation2D::SetEndPoint(Player::GetPlayerPosition().x, Player::GetPlayerPosition().z);
			Navigation2D::StartNavigation();
			State = 1;
		}
	}

	if (EndAnime == MeshAnimation::GetEndFrame() / 2 && isAttack == false)
	{
		isAttack = true;
		Player::Hit(0.1f);
		hsSound::GM()->Play_EXSound("EnemyA_Attack");
	}

	if (EndAnime == MeshAnimation::GetEndFrame() - 2)
	{
		isAttack = false;
		//Player::Hit(10);
	}

}

void EnemyA::Dead()
{
	Collider::Updata(XMFLOAT3(1000, 0, 1000));

	//반복재생 안함...
	int EndAnime = MeshAnimation::PlayAnimation("EnemyADead", 50, false);

	//애니메이션이 종료됨
	if (EndAnime >= MeshAnimation::GetEndFrame() - 1)
	{

		//그리고 적을 밑으로 계속내림..
		transfrom.SetTranlate(0, 0.5f * -ENGINE->GetDeltaTime(), 0);
	}

	//어느정도 밑에까지 내려왔다면
	if (transfrom.Position.y <= -2)
	{
		isDelete = true;
		delete weapon;
		weapon = nullptr;
	}

	
	if (isDeadSound == false)
	{
		isDeadSound = true;
		hsSound::GM()->Play_EXSound("EnemyA_Dead");
	}
}

void EnemyA::Trace()
{
	MeshAnimation::PlayAnimation("EnemyAMove", 50);
	//네비게션에 따라 움직인다
	bool isMoveEnd = Navigation2D::NavigationMove(transfrom.Position, transfrom.Rotation, Speed * ENGINE->GetDeltaTime(),0.4f);

	//만약 도착지점에 왔다면 새롭게 위치를 잡아줘야함
	if (isMoveEnd == true)
	{
		Navigation2D::SetStartPoint(transfrom.Position.x, transfrom.Position.z);
		Navigation2D::SetEndPoint(Player::GetPlayerPosition().x, Player::GetPlayerPosition().z);
		Navigation2D::StartNavigation();
		isMoveEnd = false;
	}
}

void EnemyA::HPDown(float Damage)
{
	HP -= Damage;
	if (HP <= 0)
	{
		State = 3;
		Trigger::SetEnemyCountDown();
	}
	hsSound::GM()->Play_EXSound("Enemy_Hit01");
}

void EnemyA::LookAt()
{
	//원점으로부터의 가야하는곳의 벡터를 구함
	//플레이어와 나의거리를 뺀다
	float X = Player::GetPlayerPosition().x - transfrom.Position.x;
	float Y = Player::GetPlayerPosition().z - transfrom.Position.z;

	//그곳의 각도를 구함
	float angle = -atan2(Y, X) * 180 / MathHelper::Pi;

	//각도설정 처음캐릭터는 x축으로 돌아가있고 -z방향을 보고있기떄문에 90도를 빼준다 (오른쪽을바라보고있도록하기위해)
	transfrom.Rotation = { transfrom.Rotation.x,transfrom.Rotation.y,90 - angle };
}

float EnemyA::GetPlayerDistance()
{
	//플레이어에 위치를 가져온다
	XMFLOAT3 PlayerPos = Player::GetPlayerPosition();

	float x = PlayerPos.x - transfrom.Position.x;
	float z = PlayerPos.z - transfrom.Position.z;

	return sqrt((x * x) + (z * z));
}
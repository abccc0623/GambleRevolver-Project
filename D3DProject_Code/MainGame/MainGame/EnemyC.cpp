#include "pch.h"
#include "EnemyC.h"

EnemyC::EnemyC(float x, float y, float z)
{
	transfrom.Position = { x,y,z };
	State = 0;
	Speed = 1.5f;
	HP = 100;
	AttackRange =10; //사거리 크기가줄어든만큼
	HitRange = 0.5f;
	DebugSize = 40;
	AttackMaxTime = 3;
	AttackTime = 0;
	AttackCount = 0;
}

EnemyC::~EnemyC()
{


}

void EnemyC::init(hsEngine* mENGINE)
{
	transfrom.Scale = { 0.025f,0.025f ,0.025f };
	transfrom.Rotation = { 90,0,0 };

	Skinning::init(mENGINE);

	float size = 20;
	//디버깅 생성
	Debugging::Create_2D_Y_Circle_Buffers(XMFLOAT3(0, 1 * size, 0), DebugSize* AttackRange, XMFLOAT4(1, 1, 0, 1));
	Debugging::Create_2D_Y_Circle_Buffers(XMFLOAT3(0, 1 * size, 0), DebugSize* HitRange, XMFLOAT4(0, 1, 0, 1));
	Debugging::Create_Line_Buffers(XMFLOAT3(0, 0, 0), XMFLOAT3(1 * size, 0, 0), XMFLOAT4(0, 0, 1, 1));
	Debugging::Create_Line_Buffers(XMFLOAT3(0, 0, 0), XMFLOAT3(0, 1 * size, 0), XMFLOAT4(0, 1, 0, 1));
	Debugging::Create_Line_Buffers(XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, -1 * size), XMFLOAT4(1, 0, 0, 1));
	Debugging::CreateDebugBuffers();
	Speed = 2 + (rand()) % (int)5;
	Collider::SetBoundingSphere(transfrom.Position, HitRange, Collider::Enemy);

	
	

}

void EnemyC::Update()
{
	time(nullptr);
	if (isDelete == true) { return; }
	//충돌 처리
	if (State != 3)
	{
		//충돌 처리
		Collider::Updata(XMFLOAT3(transfrom.Position.x,1 ,transfrom.Position.z));
		if (Collider::OnCollisionStay())
		{
			//히트 오브젝트와 충돌하면 HP 감소
			if (Collider::SphereCollider.HitType & Collider::hitObj)
			{
				HPDown(50);
				hsSound::GM()->Play_EXSound("hit");
				hitParticle->transfrom.Position = transfrom.Position;
				hitParticle->Play();
			}
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
		AttackReady();
		break;
	case 3: //Dead
		Dead();
		break;
	case 4: //Reload
		Reload();
		break;
	case 5: //Attack
		Attack();
		break;
	case 6:
		RunAway();
		break;
	}



	for (int i = 0; i < bulletList.size(); i++)
	{
		if (bulletList[i] == nullptr) { continue; }

		if (bulletList[i]->OnRender == true)
		{
			bulletList[i]->Update();
		}
	}

	if (Weapon != nullptr)
	{
		Weapon->transfrom.Rotation = {0,90,70};
		Weapon->Update();
		Weapon->UpdateParentTr();

	}
	Skinning::Update();
}

void EnemyC::Debug()
{
	//축을 맞추기위해 ... 
	XMMATRIX T = transfrom.SetPosition4X4(XMFLOAT3(transfrom.Position.x, transfrom.Position.y, transfrom.Position.z));
	XMMATRIX R = transfrom.SetRotation4X4(XMFLOAT3(transfrom.Rotation.x - 90, transfrom.Rotation.z, -transfrom.Rotation.y));
	XMMATRIX S = transfrom.SetScale4X4(XMFLOAT3(1, 1, 1));
	XMMATRIX mDebug = S * R * T;

	Debugging::Update(mDebug);

}

void EnemyC::Render(const hsViewData& view)
{
	for (int i = 0; i < bulletList.size(); i++)
	{
		if (bulletList[i] == nullptr) { continue; }

		if (bulletList[i]->OnRender == true)
		{
			bulletList[i]->Render(hsViewData());
		}
	}

	if (Weapon != nullptr) 
	{
		Weapon->Render(view);
	}
	Skinning::Render(view);
}

void EnemyC::PushHitParticle(ParticleSystem* p)
{
	hitParticle = p;
}

void EnemyC::PushBullet(Bullet* bullet)
{
	bulletList.push_back(bullet);
}

void EnemyC::PushWeapon(GameObject* mWeapon)
{
	hsBone* Left = Skinning::FindBone("mixamorig:RightHand");
	Weapon = mWeapon;
	Weapon->SetParentTR(&Left->transfrom);
}

void EnemyC::Idle()
{
	Navigation2D::SetStartPoint(transfrom.Position.x, transfrom.Position.z);
	Navigation2D::SetEndPoint(Player::GetPlayerPosition().x, Player::GetPlayerPosition().z);
	Navigation2D::StartNavigation();

	State = 1;
}

void EnemyC::Attack()
{
	int EndAnime = MeshAnimation::PlayAnimation("EnemyCAttack", 75);
	int End = MeshAnimation::GetEndFrame();
	

	AttackTime += ENGINE->GetDeltaTime();
	if (AttackTime >= 0.5f)
	{

		float x = Player::GetPlayerPosition().x - transfrom.Position.x;
		float z = Player::GetPlayerPosition().z - transfrom.Position.z;

		float n = sqrt((x * x) + (z * z));
		x /= n;
		z /= n;
		XMFLOAT3 pos = {transfrom.Position.x+ x ,1,transfrom.Position.z+ z };

		hsSound::GM()->Play_EXSound("EnemyC_Attack");
		isAttackSound = true;
		AttackCount++;
		AttackTime = 0;
		bulletList[AttackCount]->Shoot(XMFLOAT3(x,1.25f,z), pos);



		if (EndAnime >= MeshAnimation::GetEndFrame() - 2)
		{
			isAttackSound = false;
		}
	}
	
	LookAt();
	if (AttackCount >= 4)
	{
		State = 4;
		isAttackSound = false;
		AttackCount = 0;
	}
}

void EnemyC::AttackReady()
{
	int EndAnime = MeshAnimation::PlayAnimation("EnemyCTarget", 50);
	LookAt();
	AttackReadyTime += ENGINE->GetDeltaTime();
	if ( EndAnime >= MeshAnimation::GetEndFrame() - 2)
	{
		State = 5;
		AttackReadyTime = 0;
	}
}

void EnemyC::Dead()
{
	//반복재생 안함...
	int EndAnime = MeshAnimation::PlayAnimation("EnemyCDead", 100, false);

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
		//총알 삭제
		for (int i = 0; i < bulletList.size(); i++)
		{
			delete bulletList[i];
			bulletList[i] = nullptr;
		}
		bulletList.clear();
		hitParticle->isDelete = true;
		delete Weapon;
		Weapon = nullptr;
	}


	if (isDeadSound == false)
	{
		isDeadSound = true;
		hsSound::GM()->Play_EXSound("EnemyC_Dead");
	}


	Collider::Updata(XMFLOAT3(1000,0,1000));
}

void EnemyC::Trace()
{
	///추격 상태 
	//플레이어와 거리측정


	//플레이어와의 거리를 구한다 
	if (GetPlayerDistance() <= AttackRange)
	{
		State = 2;
	}

	//네비게션에 따라 움직인다
	bool isMoveEnd = Navigation2D::NavigationMove(transfrom.Position, transfrom.Rotation, Speed * ENGINE->GetDeltaTime());

	//만약 도착지점에 왔다면 새롭게 위치를 잡아줘야함
	if (isMoveEnd == true)
	{
		Navigation2D::SetStartPoint(transfrom.Position.x, transfrom.Position.z);
		Navigation2D::SetEndPoint(Player::GetPlayerPosition().x, Player::GetPlayerPosition().z);
		Navigation2D::StartNavigation();
		isMoveEnd = false;
	}

	MeshAnimation::PlayAnimation("EnemyCMove", 30);
}

void EnemyC::Reload()
{
	int EndAnime = MeshAnimation::PlayAnimation("EnemyCReload", 60, true);
	if (isReloadSound == false)
	{
		hsSound::GM()->Play_EXSound("EnemyC_Reload");
		isReloadSound = true;
	}



	//충돌을 하지않았거나 충돌했어도 다른적과충돌했다면 통과
	if (Collider::OnCollisionStay() == false || Collider::SphereCollider.HitType == Collider::Enemy)
	{
		//애니메이션이 끝났다면통과
		if (EndAnime >= MeshAnimation::GetEndFrame() - 2)
		{

			Navigation2D::SetStartPoint(transfrom.Position.x, transfrom.Position.z);
			Navigation2D::SetEndPoint(Player::GetPlayerPosition().x, Player::GetPlayerPosition().z);
			Navigation2D::StartNavigation();

			FindRunAwayPoint();
			State = 6;
			isReloadSound = false;
		}
	}
	
}

void EnemyC::RunAway()
{
	//transfrom.SetTranlate(Right.x *ENGINE->GetDeltaTime(),0, Right.z*ENGINE->GetDeltaTime());
	
	//transfrom.Position = pos;

	MeshAnimation::PlayAnimation("EnemyCMove", 30);
	bool isMoveEnd = Navigation2D::NavigationMove(transfrom.Position, transfrom.Rotation, 3 * ENGINE->GetDeltaTime());
	//만약 도착지점에 왔다면 새롭게 위치를 잡아줘야함
	if (isMoveEnd == true)
	{
		isMoveEnd = false;
		State = 1;
	}

}

void EnemyC::FindRunAwayPoint()
{

	float x = Player::GetPlayerPosition().x - transfrom.Position.x;
	float z = Player::GetPlayerPosition().z - transfrom.Position.z;
	float n = sqrt((x * x) + (z * z));
	x /= n;
	z /= n;

	XMFLOAT3 look = XMFLOAT3(x, 0, z);
	XMFLOAT3 Up = XMFLOAT3(0, 1, 0);
	//두개의벡터의 외적을구함
	DirectX::XMVECTOR L = XMLoadFloat3(&look);
	DirectX::XMVECTOR U = XMLoadFloat3(&Up);
	U = XMVector3Cross(L, U);
	//플레이어의 앞방향 구함
	XMStoreFloat3(&Right, U);
	static int d = 1;
	//오른쪽으로 갈껀지 왠쪽으로 갈껀지

	
	if (d >= 0)
	{
		d = -1;
	}
	else
	{
		d = 1;
	}
	float RunAwayRange = -10;

	while (RunAwayRange == 0)
	{
		pos = { transfrom.Position.x + (Right.x * RunAwayRange*d),0,transfrom.Position.z + (Right.z * RunAwayRange * d) };
		if (Navigation2D::FindObject(pos.x, pos.z) == false)
		{
			//공격을당했다면 도망가야함 플레이어 방향의 오른쪽 왼쪽
			Navigation2D::SetStartPoint(transfrom.Position.x, transfrom.Position.z);
			Navigation2D::SetEndPoint(pos.x, pos.z);
			Navigation2D::StartNavigation();
			break;;
		}
		RunAwayRange--;
		if (d >= 0)
		{
			d = -1;
		}
		else
		{
			d = 1;
		}

		if (RunAwayRange == 10)
		{
			int num = 0;
		}
	}


	State = 6;
	isHit = false;
}

void EnemyC::HPDown(float Damage)
{
	HP -= Damage;

	hsSound::GM()->Play_EXSound("Enemy_Hit01");
	//한대 맞으면 방어 상태로 변함 
	if (SkillCoolDownTime <= 0)
	{
		SkillTime = 5;
	}

	if (HP <= 0)
	{
		State = 3;
		Trigger::SetEnemyCountDown();
	}
}

void EnemyC::LookAt()
{
	//원점으로부터의 가야하는곳의 벡터를 구함
	dir.x = Player::GetPlayerPosition().x - transfrom.Position.x;
	dir.y = Player::GetPlayerPosition().z - transfrom.Position.z;

	//그곳의 각도를 구함
	float angle = -atan2(dir.y, dir.x) * 180 / MathHelper::Pi;
	//각도설정 처음캐릭터는 x축으로 돌아가있고 -z방향을 보고있기떄문에 90도를 빼준다 (오른쪽을바라보고있도록하기위해)
	transfrom.Rotation = { transfrom.Rotation.x,transfrom.Rotation.y,90 - angle };
}
float EnemyC::GetPlayerDistance()
{
	XMFLOAT3 PlayerPos = Player::GetPlayerPosition();
	float x = PlayerPos.x - transfrom.Position.x;
	float z = PlayerPos.z - transfrom.Position.z;


	return sqrt((x * x) + (z * z));
}
#include "pch.h"
#include "EnemyB.h"

EnemyB::EnemyB(float x, float y, float z)
{
	transfrom.Position = XMFLOAT3(x, y, z);
	State = 0;
	Speed = 3.5f;
	BaseSpeed = 3.5f;
	SkillSpeed = 1;
	HP = 100;
	AttackRange = 2;
	HitRange = 1;
	isGuard = false;
	isGuardOn = true;
}

EnemyB::~EnemyB()
{


}

void EnemyB::init(hsEngine* mENGINE)
{
	transfrom.Scale = { 2.5f,2.5f ,2.5f };
	transfrom.Rotation = { 90,0,0 };

	Skinning::init(mENGINE);

	float size = 0.25f;
	//디버깅 생성
	Debugging::Create_2D_Y_Circle_Buffers(XMFLOAT3(0, 1 * size, 0), AttackRange/transfrom.Scale.x, XMFLOAT4(1, 1, 0, 1));
	Debugging::Create_2D_Y_Circle_Buffers(XMFLOAT3(0, 1 * size, 0), HitRange / transfrom.Scale.x, XMFLOAT4(0, 1, 0, 1));
	Debugging::Create_Line_Buffers(XMFLOAT3(0, 0, 0), XMFLOAT3(1 * size, 0, 0), XMFLOAT4(0, 0, 1, 1));
	Debugging::Create_Line_Buffers(XMFLOAT3(0, 0, 0), XMFLOAT3(0, 1 * size, 0), XMFLOAT4(0, 1, 0, 1));
	Debugging::Create_Line_Buffers(XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, -1 * size), XMFLOAT4(1, 0, 0, 1));
	Debugging::CreateDebugBuffers();

	Speed = 2 + (rand()) % (int)5;
	Collider::SetBoundingSphere(transfrom.Position, HitRange, Collider::Enemy);
}

void EnemyB::Update()
{
	time(nullptr);

	//가드상태가 되었다면
	if (isGuard == true)
	{
		//스킬 쿨타임을 동시에 올림
		SkillTime += ENGINE->GetDeltaTime();
		//일단 스킬 지속시간이 끝났다면 가드상태는 풀린다
		if (SkillTime >= 2.5f)
		{
			isGuard = false;
			SkillCoolDownTime = 0;
		}
	}


	if (isGuard == false)
	{
		SkillCoolDownTime += ENGINE->GetDeltaTime();
		if (SkillCoolDownTime >= 5)
		{
			isGuardOn = true;
			SkillTime = 0;
			SkillCoolDownTime = 0;
		}
	}

	



	if (State != 3)
	{
		//충돌 처리
		Collider::Updata(transfrom.Position);
		if (Collider::OnCollisionStay())
		{
			//히트 오브젝트와 충돌하면 HP 감소
			if (Collider::SphereCollider.HitType & Collider::hitObj)
			{
				HPDown(25);
				HitParticle->transfrom.Position = transfrom.Position;
				HitTime = 0;
			}
		}


		//플레이어와 거리측정
		if (GetPlayerDistance() <= AttackRange)
		{
			State = 2;
		}
	}


	//FSM
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
	case 4: //Defance
		break;
	}

	//업데이트
	Skinning::Update();
}

void EnemyB::Debug()
{
	XMMATRIX T = transfrom.SetPosition4X4(XMFLOAT3(transfrom.Position.x, transfrom.Position.y, transfrom.Position.z));
	XMMATRIX R = transfrom.SetRotation4X4(XMFLOAT3(transfrom.Rotation.x - 90, transfrom.Rotation.z, -transfrom.Rotation.y));
	XMMATRIX S = transfrom.SetScale4X4(XMFLOAT3(1, 1, 1));
	XMMATRIX mDebug = S * R * T;

	Debugging::Update(mDebug);
}

void EnemyB::Render(const hsViewData& view)
{
	Skinning::Render(view);
}

void EnemyB::PushHitParticle(ParticleSystem* p)
{
	HitParticle = p;
}

void EnemyB::Idle()
{
	/// 대기 상태 없이 바로 추격상태로 

	Navigation2D::SetStartPoint(transfrom.Position.x, transfrom.Position.z);
	Navigation2D::SetEndPoint(Player::GetPlayerPosition().x,Player::GetPlayerPosition().z);
	Navigation2D::StartNavigation();

	State = 1;
}

void EnemyB::Attack()
{
	int EndAnime = MeshAnimation::PlayAnimation("EnemyBAttack1", 50);
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

	if (EndAnime == MeshAnimation::GetEndFrame()/2 && isAttack == false)
	{
		isAttack = true;
		Player::Hit(0.1f);
		hsSound::GM()->Play_EXSound("EnemyB_Attack");
	}

	if (EndAnime == MeshAnimation::GetEndFrame()-2)
	{
		isAttack = false;
	}
}

void EnemyB::Dead()
{
	Collider::Updata(XMFLOAT3(1000, 0, 1000));

	//반복재생 안함...
	int EndAnime = MeshAnimation::PlayAnimation("EnemyBDead", 40,false);
	
	//애니메이션이 종료됨
	if (EndAnime >= MeshAnimation::GetEndFrame() -1)
	{
		//그리고 적을 밑으로 계속내림..
		transfrom.SetTranlate(0, 0.5f* -ENGINE->GetDeltaTime(),0);
	}

	//어느정도 밑에까지 내려왔다면
	if (transfrom.Position.y <= -2)
	{
		isDelete = true;
		HitParticle->isDelete = true;
	}

	if (isDeadSound == false)
	{
		isDeadSound = true;
		hsSound::GM()->Play_EXSound("EnemyB_Dead");
	}
}

void EnemyB::Trace()
{
	///추격 상태 
	Speed = BaseSpeed;
	bool isMoveEnd = Navigation2D::NavigationMove(transfrom.Position, transfrom.Rotation, BaseSpeed * ENGINE->GetDeltaTime(),0.5f);

	//만약 도착지점에 왔다면 새롭게 위치를 잡아줘야함
	if (isMoveEnd == true)
	{
		Navigation2D::SetStartPoint(transfrom.Position.x, transfrom.Position.z);
		Navigation2D::SetEndPoint(Player::GetPlayerPosition().x, Player::GetPlayerPosition().z);
		Navigation2D::StartNavigation();
		isMoveEnd = false;
	}

	//가드 상태일떄
	if (isGuard == false)
	{
		MeshAnimation::PlayAnimation("EnemyBMove", 30);
	}
	else
	{
		MeshAnimation::PlayAnimation("GuardWalk", 30);
	}
}

void EnemyB::Defence()
{

}

void EnemyB::HPDown(float Damage)
{
	//한대 맞으면 방어 상태로 변함 
	if (isGuard == true)
	{
		HitParticle->Stop();
		hsSound::GM()->Play_EXSound("EnemyB_Skill");
		return;
	}
	else
	{
		//가드상태가 아닐때 맞았으면
		//가드를올려도 되는지확인
		if (isGuardOn == true)
		{
			//올려도된다면 가드올림
			isGuard = true;
		}
		//그래도 한대는맞음
		HP -= Damage;
		hsSound::GM()->Play_EXSound("Enemy_Hit01");
		HitParticle->Play();


		if (HP <= 0)
		{
			Trigger::SetEnemyCountDown();
			State = 3;
		}
	}
}

void EnemyB::LookAt()
{
	//원점으로부터의 가야하는곳의 벡터를 구함
	float X = Player::GetPlayerPosition().x - transfrom.Position.x;
	float Y = Player::GetPlayerPosition().z - transfrom.Position.z;

	//그곳의 각도를 구함
	float angle = -atan2(Y, X) * 180 / MathHelper::Pi;
	//각도설정 처음캐릭터는 x축으로 돌아가있고 -z방향을 보고있기떄문에 90도를 빼준다 (오른쪽을바라보고있도록하기위해)
	transfrom.Rotation = {transfrom.Rotation.x,transfrom.Rotation.y,90 - angle };
}

float EnemyB::GetPlayerDistance()
{
	XMFLOAT3 PlayerPos = Player::GetPlayerPosition();
	float x = PlayerPos.x - transfrom.Position.x;
	float z = PlayerPos.z - transfrom.Position.z;


	return sqrt((x * x) + (z * z));
}

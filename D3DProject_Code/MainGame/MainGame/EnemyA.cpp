#include "pch.h"
#include "EnemyA.h"

EnemyA::EnemyA(float x, float y, float z)
{
	transfrom.Position = XMFLOAT3(x, y, z);
	State = 0;
	Speed = 4.5f;
	HP = 100;

	AttackRange = 1.5f; //��Ÿ� ũ�Ⱑ�پ�縸ŭ
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
	//����� ����
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
	//�浹 ó��

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

		//�÷��̾�� �Ÿ�����
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
	//�浹�� �����ʾҰų� �浹�߾ �ٸ������浹�ߴٸ� ���
	if (Collider::OnCollisionStay() == false || Collider::SphereCollider.HitType == Collider::Enemy)
	{
		//�ִϸ��̼��� �����ٸ����
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

	//�ݺ���� ����...
	int EndAnime = MeshAnimation::PlayAnimation("EnemyADead", 50, false);

	//�ִϸ��̼��� �����
	if (EndAnime >= MeshAnimation::GetEndFrame() - 1)
	{

		//�׸��� ���� ������ ��ӳ���..
		transfrom.SetTranlate(0, 0.5f * -ENGINE->GetDeltaTime(), 0);
	}

	//������� �ؿ����� �����Դٸ�
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
	//�׺�Լǿ� ���� �����δ�
	bool isMoveEnd = Navigation2D::NavigationMove(transfrom.Position, transfrom.Rotation, Speed * ENGINE->GetDeltaTime(),0.4f);

	//���� ���������� �Դٸ� ���Ӱ� ��ġ�� ��������
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
	//�������κ����� �����ϴ°��� ���͸� ����
	//�÷��̾�� ���ǰŸ��� ����
	float X = Player::GetPlayerPosition().x - transfrom.Position.x;
	float Y = Player::GetPlayerPosition().z - transfrom.Position.z;

	//�װ��� ������ ����
	float angle = -atan2(Y, X) * 180 / MathHelper::Pi;

	//�������� ó��ĳ���ʹ� x������ ���ư��ְ� -z������ �����ֱ⋚���� 90���� ���ش� (���������ٶ󺸰��ֵ����ϱ�����)
	transfrom.Rotation = { transfrom.Rotation.x,transfrom.Rotation.y,90 - angle };
}

float EnemyA::GetPlayerDistance()
{
	//�÷��̾ ��ġ�� �����´�
	XMFLOAT3 PlayerPos = Player::GetPlayerPosition();

	float x = PlayerPos.x - transfrom.Position.x;
	float z = PlayerPos.z - transfrom.Position.z;

	return sqrt((x * x) + (z * z));
}
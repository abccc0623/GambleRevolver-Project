#include "pch.h"
#include "EnemyC.h"

EnemyC::EnemyC(float x, float y, float z)
{
	transfrom.Position = { x,y,z };
	State = 0;
	Speed = 1.5f;
	HP = 100;
	AttackRange =10; //��Ÿ� ũ�Ⱑ�پ�縸ŭ
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
	//����� ����
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
	//�浹 ó��
	if (State != 3)
	{
		//�浹 ó��
		Collider::Updata(XMFLOAT3(transfrom.Position.x,1 ,transfrom.Position.z));
		if (Collider::OnCollisionStay())
		{
			//��Ʈ ������Ʈ�� �浹�ϸ� HP ����
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
	//���� ���߱����� ... 
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
	//�ݺ���� ����...
	int EndAnime = MeshAnimation::PlayAnimation("EnemyCDead", 100, false);

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
		//�Ѿ� ����
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
	///�߰� ���� 
	//�÷��̾�� �Ÿ�����


	//�÷��̾���� �Ÿ��� ���Ѵ� 
	if (GetPlayerDistance() <= AttackRange)
	{
		State = 2;
	}

	//�׺�Լǿ� ���� �����δ�
	bool isMoveEnd = Navigation2D::NavigationMove(transfrom.Position, transfrom.Rotation, Speed * ENGINE->GetDeltaTime());

	//���� ���������� �Դٸ� ���Ӱ� ��ġ�� ��������
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



	//�浹�� �����ʾҰų� �浹�߾ �ٸ������浹�ߴٸ� ���
	if (Collider::OnCollisionStay() == false || Collider::SphereCollider.HitType == Collider::Enemy)
	{
		//�ִϸ��̼��� �����ٸ����
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
	//���� ���������� �Դٸ� ���Ӱ� ��ġ�� ��������
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
	//�ΰ��Ǻ����� ����������
	DirectX::XMVECTOR L = XMLoadFloat3(&look);
	DirectX::XMVECTOR U = XMLoadFloat3(&Up);
	U = XMVector3Cross(L, U);
	//�÷��̾��� �չ��� ����
	XMStoreFloat3(&Right, U);
	static int d = 1;
	//���������� ������ �������� ������

	
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
			//���������ߴٸ� ���������� �÷��̾� ������ ������ ����
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
	//�Ѵ� ������ ��� ���·� ���� 
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
	//�������κ����� �����ϴ°��� ���͸� ����
	dir.x = Player::GetPlayerPosition().x - transfrom.Position.x;
	dir.y = Player::GetPlayerPosition().z - transfrom.Position.z;

	//�װ��� ������ ����
	float angle = -atan2(dir.y, dir.x) * 180 / MathHelper::Pi;
	//�������� ó��ĳ���ʹ� x������ ���ư��ְ� -z������ �����ֱ⋚���� 90���� ���ش� (���������ٶ󺸰��ֵ����ϱ�����)
	transfrom.Rotation = { transfrom.Rotation.x,transfrom.Rotation.y,90 - angle };
}
float EnemyC::GetPlayerDistance()
{
	XMFLOAT3 PlayerPos = Player::GetPlayerPosition();
	float x = PlayerPos.x - transfrom.Position.x;
	float z = PlayerPos.z - transfrom.Position.z;


	return sqrt((x * x) + (z * z));
}
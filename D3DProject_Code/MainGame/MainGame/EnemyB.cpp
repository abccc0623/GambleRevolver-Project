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
	//����� ����
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

	//������°� �Ǿ��ٸ�
	if (isGuard == true)
	{
		//��ų ��Ÿ���� ���ÿ� �ø�
		SkillTime += ENGINE->GetDeltaTime();
		//�ϴ� ��ų ���ӽð��� �����ٸ� ������´� Ǯ����
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
		//�浹 ó��
		Collider::Updata(transfrom.Position);
		if (Collider::OnCollisionStay())
		{
			//��Ʈ ������Ʈ�� �浹�ϸ� HP ����
			if (Collider::SphereCollider.HitType & Collider::hitObj)
			{
				HPDown(25);
				HitParticle->transfrom.Position = transfrom.Position;
				HitTime = 0;
			}
		}


		//�÷��̾�� �Ÿ�����
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

	//������Ʈ
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
	/// ��� ���� ���� �ٷ� �߰ݻ��·� 

	Navigation2D::SetStartPoint(transfrom.Position.x, transfrom.Position.z);
	Navigation2D::SetEndPoint(Player::GetPlayerPosition().x,Player::GetPlayerPosition().z);
	Navigation2D::StartNavigation();

	State = 1;
}

void EnemyB::Attack()
{
	int EndAnime = MeshAnimation::PlayAnimation("EnemyBAttack1", 50);
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

	//�ݺ���� ����...
	int EndAnime = MeshAnimation::PlayAnimation("EnemyBDead", 40,false);
	
	//�ִϸ��̼��� �����
	if (EndAnime >= MeshAnimation::GetEndFrame() -1)
	{
		//�׸��� ���� ������ ��ӳ���..
		transfrom.SetTranlate(0, 0.5f* -ENGINE->GetDeltaTime(),0);
	}

	//������� �ؿ����� �����Դٸ�
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
	///�߰� ���� 
	Speed = BaseSpeed;
	bool isMoveEnd = Navigation2D::NavigationMove(transfrom.Position, transfrom.Rotation, BaseSpeed * ENGINE->GetDeltaTime(),0.5f);

	//���� ���������� �Դٸ� ���Ӱ� ��ġ�� ��������
	if (isMoveEnd == true)
	{
		Navigation2D::SetStartPoint(transfrom.Position.x, transfrom.Position.z);
		Navigation2D::SetEndPoint(Player::GetPlayerPosition().x, Player::GetPlayerPosition().z);
		Navigation2D::StartNavigation();
		isMoveEnd = false;
	}

	//���� �����ϋ�
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
	//�Ѵ� ������ ��� ���·� ���� 
	if (isGuard == true)
	{
		HitParticle->Stop();
		hsSound::GM()->Play_EXSound("EnemyB_Skill");
		return;
	}
	else
	{
		//������°� �ƴҶ� �¾�����
		//���带�÷��� �Ǵ���Ȯ��
		if (isGuardOn == true)
		{
			//�÷����ȴٸ� ����ø�
			isGuard = true;
		}
		//�׷��� �Ѵ�¸���
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
	//�������κ����� �����ϴ°��� ���͸� ����
	float X = Player::GetPlayerPosition().x - transfrom.Position.x;
	float Y = Player::GetPlayerPosition().z - transfrom.Position.z;

	//�װ��� ������ ����
	float angle = -atan2(Y, X) * 180 / MathHelper::Pi;
	//�������� ó��ĳ���ʹ� x������ ���ư��ְ� -z������ �����ֱ⋚���� 90���� ���ش� (���������ٶ󺸰��ֵ����ϱ�����)
	transfrom.Rotation = {transfrom.Rotation.x,transfrom.Rotation.y,90 - angle };
}

float EnemyB::GetPlayerDistance()
{
	XMFLOAT3 PlayerPos = Player::GetPlayerPosition();
	float x = PlayerPos.x - transfrom.Position.x;
	float z = PlayerPos.z - transfrom.Position.z;


	return sqrt((x * x) + (z * z));
}

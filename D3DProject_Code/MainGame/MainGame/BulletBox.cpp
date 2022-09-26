#include "pch.h"
#include "BulletBox.h"

BulletBox::BulletBox(float x, float y, float z)
{
	transfrom.Position = { x,1,z };
}

BulletBox::~BulletBox()
{

}

void BulletBox::init(hsEngine* ENGINE, LoadData* Data)
{
	HsMesh::init(ENGINE, Data);
	//Debugging::init(ENGINE);
	//Debugging::Create_Box_Buffers(XMFLOAT3(0, 0.5f, 0), XMFLOAT3(0.5f, 0.5f, 0.5f));
	//Debugging::CreateDebugBuffers();
	Collider::SetBoundingSphere(transfrom.Position, 1, Collider::Obj);
}

void BulletBox::Debug()
{
	//Debugging::Update(mWorld);
}

void BulletBox::Update()
{
	if (Collider::OnCollisionEnter())
	{
		if (Collider::SphereCollider.HitType & hitObj)
		{

			if (Player::GetMoney() >= 100)
			{
				//플레이어 돈 1감소
				Player::MoneyDown(100);

				//블렛 1증가
				Player::MoneyBullet_UP(6);
				hsSound::GM()->Play_EXSound("UI");

			}
			else
			{

				hsSound::GM()->Play_EXSound("EnemyB_Skill");
			}
		}
	}



	Collider::Updata(XMFLOAT3(transfrom.Position.x, 1, transfrom.Position.z));
	//HsMesh::Update();
}

void BulletBox::Render()
{
	//HsMesh::Render(hsViewData());
}

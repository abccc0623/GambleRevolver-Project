#include "pch.h"
#include "HPBox.h"

HPBox::HPBox(float x, float y, float z)
{
	transfrom.Position = { x,y,z };
}

HPBox::~HPBox()
{

}

void HPBox::init(hsEngine* ENGINE, LoadData* Data)
{
	HsMesh::init(ENGINE, Data);
	//Debugging::init(ENGINE);
	//Debugging::Create_Box_Buffers(XMFLOAT3(0, 0.5f, 0), XMFLOAT3(0.5f, 0.5f, 0.5f));
	//Debugging::CreateDebugBuffers();
	Collider::SetBoundingSphere(transfrom.Position, 1, Collider::Obj);
}

void HPBox::Debug()
{

}

void HPBox::Update()
{
	if (Collider::OnCollisionEnter())
	{
		if (Collider::SphereCollider.HitType & hitObj)
		{

			if (Player::GetMoney() >= 50)
			{
				//플레이어 돈 1감소
				Player::MoneyDown(50);
				//블렛 1증가
				Player::MoneyHP_Up(10);
				hsSound::GM()->Play_EXSound("Healing");
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

void HPBox::Render()
{
	
	//HsMesh::Render(hsViewData());
}

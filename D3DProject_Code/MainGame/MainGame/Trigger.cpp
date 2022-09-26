#include "pch.h"
#include "Trigger.h"

int Trigger::EnemyCount = 0;
Trigger::Trigger(float x, float y, float z)
{
	transfrom.Position = { x,y,z };
	Money = 0;
}

Trigger::~Trigger()
{


}

void Trigger::init(hsEngine* Engine,LoadData* data)
{
	HsMesh::init(Engine, data);
	//디버깅용
	
	Debugging::Create_Circle_Buffers(XMFLOAT3(0, 1, 0),1 , XMFLOAT4(1,0,0,1));
	Debugging::CreateDebugBuffers();

	//충돌용
	Collider::SetBoundingSphere(transfrom.Position, 1,Collider::Obj);
	
}

void Trigger::Update()
{
	//포탈열림
	if (EnemyCount <= 0)
	{
		if (isSound == false) 
		{
			hsSound::GM()->Play_EXSound("Clear");
			isSound = true;
		}
		mWorld = transfrom.GetMaster();
		Collider::Updata(transfrom.Position);
		Particle_P->transfrom.Position = transfrom.Position;
		Particle_P->Play();
		//충돌했을때 한번
		if (Collider::OnCollisionEnter())
		{
			if (Collider::SphereCollider.HitType == Collider::Player)
			{
				ScreenManager::MoveScreen(ScreenName);
				isDelete = true;
				Particle_P->isDelete = true;
				Player::MoneyUp(Money);
				isSound = false;
			}
		}
	}
}

void Trigger::Render(const hsViewData& view)
{

}

void Trigger::Delete()
{

}

void Trigger::Debug()
{
	Debugging::Update(mWorld);
}

void Trigger::SetScreenName(string name)
{
	ScreenName = name;
}

void Trigger::SetParticle(ParticleSystem* p)
{
	Particle_P = p;
}

void Trigger::SetMoney(int num)
{
	Money = num;
}

void Trigger::SetEnemyCount(int num)
{
	EnemyCount = num;
}

void Trigger::SetEnemyCountDown()
{
	EnemyCount--;
}

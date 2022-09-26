#include "pch.h"
#include "Bullet.h"

Bullet::Bullet(float x, float y, float z)
{
	transfrom.Position = { x,y,z };
	isShoot = false;
	bulletTime = 0;
	bulletSpeed = 25;
	bulletMaxTime = 0.35f;
}

Bullet::~Bullet()
{
	mParticle->isDelete = true;
	mParticle = nullptr;
}

void Bullet::init(hsEngine* ENGINE, LoadData* Data)
{
	HsMesh::init(ENGINE, Data);
	Debugging::Create_Box_Buffers(XMFLOAT3(0,0.5f,0),XMFLOAT3(0.5f, 0.5f, 0.5f));
	Debugging::CreateDebugBuffers();
	
}



void Bullet::Shoot(XMFLOAT3 dir,XMFLOAT3 pos)
{
	isShoot = true;
	transfrom.Position = {pos.x,pos.y,pos.z };
	Mydir = dir;
	OnRender = true;
}

void Bullet::Update()
{

	if (isShoot == true)
	{
		mParticle->transfrom.Position = transfrom.Position;
		mParticle->Play();

		Collider::SphereCollider.isCollision = true;
		bulletTime += ENGINE->GetDeltaTime();

		if (bulletTime >= bulletMaxTime)
		{
			mParticle->Stop();
			isShoot = false;
			bulletTime = 0;
			OnRender = false;
			transfrom.Position = { -1000,1000,1000 };
		}
		else
		{
			XMFLOAT3 d = Mydir* bulletSpeed * ENGINE->GetDeltaTime();
			transfrom.SetTranlate(d.x, 0, d.z);
		}


		if (Collider::OnCollisionStay())
		{
			if (type == 0 &&Collider::SphereCollider.HitType & Collider::Enemy)
			{
				isShoot = false;
				bulletTime = 0;
				OnRender = false;
				transfrom.Position = { -1000,1000,1000 };
				mParticle->Stop();
			}

			if (type == 1 && Collider::SphereCollider.HitType & Collider::Player)
			{
				isShoot = false;
				bulletTime = 0;
				OnRender = false;
				transfrom.Position = { -1000,1000,1000 };
				mParticle->Stop();
				Player::Hit(0.1f);
			}
			
		}


		if (Navigation2D::FindObject(transfrom.Position.x, transfrom.Position.z) == true) 
		{
			mParticle->Stop();
			isShoot = false;
			bulletTime = 0;
			OnRender = false;
			transfrom.Position = { -1000,1000,1000 };
		}

	}
	else
	{
		
		transfrom.Position = { -1000,1000,1000 };
	}



	Collider::Updata(transfrom.Position);	
	HsMesh::Update();
}

void Bullet::Debug()
{
	HsMesh::Debug();
}

void Bullet::Setting(int mtype)
{
	//∫Ì∑ø¿« ≈∏¿‘¿ª º¬∆√«ÿ¡‹
	type = mtype;
	switch (type)
	{
	case 0:
		PlayerSetting();
		break;
	case 1:
		EnemySetting();
		break;
	case 2:
		break;
	case 3:
		break;
	}
}

void Bullet::BulletSetting(float speed, float max)
{
	bulletSpeed = speed;
	bulletMaxTime = max;
}

void Bullet::BulletSetting()
{
	bulletSpeed = 25;
	bulletMaxTime = 0.35f;
}



void Bullet::PushParticle(ParticleSystem* p)
{
	mParticle = p;
}

void Bullet::PlayerSetting()
{
	Collider::SetBoundingSphere(transfrom.Position, 0.5f, Collider::hitObj);
	OnRender = false;
}

void Bullet::EnemySetting()
{
	Collider::SetBoundingSphere(transfrom.Position, 0.5f, Collider::Obj);
	OnRender = false;
}


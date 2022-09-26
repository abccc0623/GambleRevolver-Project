#include "pch.h"
#include "Stage02.h"

Stage02::Stage02()
{

}

Stage02::~Stage02()
{

}

void Stage02::init()
{
	//스테이지 전환 테스트용
	Navigation2D::ClearMap();
	Navigation2D::CreateMapSize(60, 40);
	MainPlayer->transfrom.Position = { 3,0,-21 };
	MainPlayer->Update();
	Navigation2D::SetObjectSetting(XMFLOAT2(16, 0), 2, 2);
	Navigation2D::SetObjectSetting(XMFLOAT2(32, 0), 2, 2);
	Navigation2D::SetObjectSetting(XMFLOAT2(16, -38), 2, 2);
	Navigation2D::SetObjectSetting(XMFLOAT2(32, -38), 2, 2);


	Navigation2D::SetObjectSetting(XMFLOAT2(2, -3), 7, 1);
	Navigation2D::SetObjectSetting(XMFLOAT2(2, -6), 7, 1);
	Navigation2D::SetObjectSetting(XMFLOAT2(2, -12), 7, 1);
	Navigation2D::SetObjectSetting(XMFLOAT2(2, -12), 7, 1);
	Navigation2D::SetObjectSetting(XMFLOAT2(2, -15), 7, 1);
	Navigation2D::SetObjectSetting(XMFLOAT2(2, -24), 7, 1);
	Navigation2D::SetObjectSetting(XMFLOAT2(2, -27), 7, 1);
	Navigation2D::SetObjectSetting(XMFLOAT2(2, -33), 7, 1);
	Navigation2D::SetObjectSetting(XMFLOAT2(2, -36), 7, 1);
	Navigation2D::SetObjectSetting(XMFLOAT2(42, -3), 7, 1);
	Navigation2D::SetObjectSetting(XMFLOAT2(42, -6), 7, 1);
	Navigation2D::SetObjectSetting(XMFLOAT2(42, -33), 7, 1);
	Navigation2D::SetObjectSetting(XMFLOAT2(42, -36), 7, 1);


	Navigation2D::SetObjectSetting(XMFLOAT2(2, -4), 7, 2);
	Navigation2D::SetObjectSetting(XMFLOAT2(2, -13), 7, 2);
	Navigation2D::SetObjectSetting(XMFLOAT2(2, -25), 7, 2);
	Navigation2D::SetObjectSetting(XMFLOAT2(2, -34), 7, 2);
	Navigation2D::SetObjectSetting(XMFLOAT2(42, -4), 7, 2);
	Navigation2D::SetObjectSetting(XMFLOAT2(42, -34), 7, 2);


	Navigation2D::SetObjectSetting(XMFLOAT2(32, -11), 7, 3);
	Navigation2D::SetObjectSetting(XMFLOAT2(32, -15), 7, 3);
	Navigation2D::SetObjectSetting(XMFLOAT2(32, -22), 7, 3);
	Navigation2D::SetObjectSetting(XMFLOAT2(32, -26), 7, 3);

	Navigation2D::SetObjectSetting(XMFLOAT2(1, -18), 1, 1);
	Navigation2D::SetObjectSetting(XMFLOAT2(1, -21), 1, 1);
	Navigation2D::SetObjectSetting(XMFLOAT2(58, -18), 1, 1);
	Navigation2D::SetObjectSetting(XMFLOAT2(58, -21), 1, 1);

	Navigation2D::SetObjectSetting(XMFLOAT2(17, -17), 6, 6);

	Navigation2D::SetObjectSetting(XMFLOAT2(51, -1), 8, 8);
	Navigation2D::SetObjectSetting(XMFLOAT2(51, -31), 8, 8);

	sub = mCam->GetCam("Sub");



	switch (Player::GetStage())
	{
	case 5:
		hsSound::GM()->Play_BGMSound("Stage02");
		Create_EnemyA(34, 0, -14);
		Create_EnemyA(34, 0, -32);
		Create_EnemyA(52, 0, -13);
		Create_EnemyA(47, 0, -22);
		Create_EnemyC(38, 0, -4);
		Create_EnemyC(46, 0, -27);
		Create_EnemyC(47, 0, -12);
		Create_EnemyC(55, 0, -15);
		Trigger::SetEnemyCount(8);
		CreateTrigger(2, 50, "Stage02", 58, 0, -20);
		break;
	case 6:
		Create_EnemyA(16, 0, -5);
		Create_EnemyA(15, 0, -29);
		Create_EnemyA(24, 0, -3);
		Create_EnemyA(18, 0, -32);
		Create_EnemyA(43, 0, -25);
		Create_EnemyA(51, 0, -14);
		//EnemyC
		Create_EnemyC(5, 0, -8);
		Create_EnemyC(44, 0, -11);
		Create_EnemyC(5, 0, -30);
		Create_EnemyC(54, 0, -26);
		Trigger::SetEnemyCount(10);
		CreateTrigger(2, 50, "Stage02", 58, 0, -20);
		break;
	case 7:
		//EnemyA
		Create_EnemyA(55, 0, -14);
		Create_EnemyA(56, 0, -28);
		//EnemyB
		Create_EnemyB(36, 0, -5);
		Create_EnemyB(36, 0, -32);
		Create_EnemyB(47, 0, -18);
		//EnemyC
		Create_EnemyC(45, 0, -10);
		Create_EnemyC(43, 0, -29);
		Create_EnemyC(41, 0, -14);
		Create_EnemyC(49, 0, -25);
		Trigger::SetEnemyCount(9);
		CreateTrigger(1, 50, "Bonus", 58, 0, -20);
		break;
	case 12:
		hsSound::GM()->Play_BGMSound("Stage02");
		//EnemyA
		Create_EnemyA(10, 0, -26);
		Create_EnemyA(14, 0, -21);
		Create_EnemyA(14, 0, -29);
		Create_EnemyA(24, 0, -6);
		Create_EnemyA(26, 0, -24);
		//EnemyB
		Create_EnemyB(44, 0, -19);
		Create_EnemyB(48, 0, -8);
		Create_EnemyB(48, 0, -30);
		//EnemyC
		Create_EnemyC(24, 0, -27);
		Create_EnemyC(28, 0, -8);
		Create_EnemyC(30, 0, -12);
		Create_EnemyC(29, 0, -22);
		Create_EnemyC(53, 0, -10);
		Create_EnemyC(53, 0, -17);
		Create_EnemyC(53, 0, -24);
		Create_EnemyC(53, 0, -29);
		Create_EnemyC(55, 0, -20);
		Trigger::SetEnemyCount(17);
		CreateTrigger(2, 100, "Stage02", 58, 0, -20);
		break;
	case 13:
		//EnemyA
		Create_EnemyA(8, 0, -20);
		Create_EnemyA(58, 0, -10);
		Create_EnemyA(58, 0, -16);
		Create_EnemyA(58, 0, -20);
		Create_EnemyA(58, 0, -23);
		Create_EnemyA(58, 0, -29);
		//EnemyB
		Create_EnemyB(14, 0, -18);
		Create_EnemyB(50, 0, -28);
		Create_EnemyB(52, 0, -16);
		Create_EnemyB(54, 0, -13);
		Create_EnemyB(54, 0, -19);
		Create_EnemyB(54, 0, -26);
		//EnemyC
		Create_EnemyC(11, 0, -19);
		Create_EnemyC(14, 0, -13);
		Create_EnemyC(14, 0, -25);
		Create_EnemyC(44, 0, -12);
		Create_EnemyC(44, 0, -20);
		Create_EnemyC(44, 0, -28);
		Create_EnemyC(48, 0, -9);
		Create_EnemyC(48, 0, -17);
		Create_EnemyC(48, 0, -25);
		Create_EnemyC(48, 0, -31);
		Trigger::SetEnemyCount(22);
		CreateTrigger(1, 100, "Bonus", 58, 0, -20);
		break;
	}

	
	
	CreateMap(-0.5f, 0, 0.5f);
}

void Stage02::Updata()
{
	XMFLOAT3 pos = Player::GetPlayerPosition();
	sub->transfrom.Position = { pos.x,sub->transfrom.Position.y,pos.z - 15 };
	Screen::Updata();
}

void Stage02::End()
{
	Player::SetNextStage();
}


void Stage02::Create_Grid()
{
	GameObject* temp = mFactory->Create_Grid(15, 15, XMFLOAT4(1, 1, 1, 1), XMFLOAT3(15.5f, 0.1f, -15.5f));
	PushObjList(temp);
}
GameObject* Stage02::Create_Box(float x, float y, float z)
{
	Bulider Data;
	Data.Nomal = "stone";
	Data.Textures = "stone";
	Data.mesh = "box";
	Data.shader = "Deferred";
	Box* temp = mFactory->CreateObject<Box>(Data, x, y, z);
	PushObjList(temp);
	Navigation2D::SetObjectSetting(temp->transfrom.Position);
	return temp;
}
void Stage02::CreateMap(float x, float y, float z)
{
	XMFLOAT3 scl = { 0.001f,0.001f ,0.001f };
	XMFLOAT3 rot = { 90,0,-90 };

	Bulider Data;
	Data.Textures = "2s_basicboard";
	Data.mesh = "2s_basicboard";
	Data.shader = "Deferred";
	hsMeshList* temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);


	Data.Textures = "2s_billboard";
	Data.mesh = "2s_billboard";
	Data.shader = "Deferred";
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);
	//
	//
	//
	Data.Textures = "2s_Dia";
	Data.mesh = "2s_Dia";
	Data.shader = "Deferred";
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);
	//
	//
	Data.Textures = "2s_door";
	Data.mesh = "2s_door";
	Data.shader = "Deferred";
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);


	Data.Textures = "2s_enterance";
	Data.mesh = "2s_enterance";
	Data.shader = "Deferred";
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);

	Data.Textures = "2s_floor";
	Data.mesh = "2s_floor";
	Data.shader = "Deferred";
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);


	Data.Textures = "2s_pillar_R";
	Data.mesh = "2s_pillar_R";
	Data.shader = "Deferred";
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);

	Data.Textures = "2s_pillar_L";
	Data.mesh = "2s_pillar_L";
	Data.shader = "Deferred";
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);

	//
	//
	Data.Textures = "2s_pillar_seastory";
	Data.mesh = "2s_pillar_seastory";
	Data.shader = "Deferred";
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);
	//
	//
	Data.Textures = "2s_scoreboard";
	Data.mesh = "2s_scoreboard";
	Data.shader = "Deferred";
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);


	Data.Textures = "2s_shark";
	Data.mesh = "2s_shark";
	Data.shader = "Deferred";
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);


	Data.Textures = "2s_slot_basic";
	Data.mesh = "2s_slot_basic";
	Data.shader = "Deferred";
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);


	Data.Textures = "2s_slot_basic";
	Data.mesh = "2s_slot_basic_7_L";
	Data.shader = "Deferred";
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);


	Data.Textures = "2s_slot_basic";
	Data.mesh = "2s_slot_basic_7_R";
	Data.shader = "Deferred";
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);


	Data.Textures = "2s_slot_round";
	Data.mesh = "2s_slot_round";
	Data.shader = "Deferred";
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);


	Data.Textures = "2s_slot_seastory";
	Data.mesh = "2s_slot_seastory";
	Data.shader = "Deferred";
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);


	Data.Textures = "2s_wall_L";
	Data.mesh = "2s_wall_L";
	Data.shader = "Deferred";
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);


	Data.Textures = "2s_wall_lower";
	Data.mesh = "2s_wall_lower";
	Data.shader = "Deferred";
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);



	Data.Textures = "2s_wall_R";
	Data.mesh = "2s_wall_R";
	Data.shader = "Deferred";
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);


	Data.Textures = "2s_wall_upper";
	Data.mesh = "2s_wall_upper";
	Data.shader = "Deferred";
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);
}
void Stage02::Create_EnemyA(float x, float y, float z)
{
	Bulider Data;
	Data.Textures = "EnemyC";
	Data.shader = "Skinning";
	Data.mesh = "EnemyA";

	//애니메이션 넣어줌
	Data.AnimList.push_back("EnemyAMove");
	Data.AnimList.push_back("EnemyAAttack1");
	Data.AnimList.push_back("EnemyAAttack2");
	Data.AnimList.push_back("EnemyADead");
	EnemyA* temp = mFactory->CreateSkinnedObject<EnemyA>(Data, x, y, z);

	//무기생성
	Data.Textures = "white";
	Data.shader = "Deferred";
	Data.mesh = "EnemyA_Weapon";
	HsMesh* weapon = mFactory->CreateObject<HsMesh>(Data, x, y, z);

	ParticleSystem* hit_P = mAlpha->CreateParticle("Smoke", 1, 10, 1, -15);
	hit_P->SetParticleTime(0.1f, 1);
	hit_P->SetParticleCount(2);
	hit_P->SetSpeed(Particle_float(0));
	hit_P->SetStartPos(Particle_float3(Particle_float(0, 1), Particle_float(0, 1), Particle_float(0)));
	hit_P->SetStartScl(Particle_float3(Particle_float(1.5f), Particle_float(1.5f), Particle_float(1.5f)));
	hit_P->SetStartRot(Particle_float3(Particle_float(0), Particle_float(0), Particle_float(0)));
	hit_P->SetStartColor(Particle_float3(Particle_float(0, 1), Particle_float(0), Particle_float(0)));
	hit_P->SetParicleEndTime(0.5f);
	temp->PushHitParticle(hit_P);
	temp->PushWeapon(weapon);

	PushObjList(temp);
}
void Stage02::Create_EnemyC(float x, float y, float z)
{
	Bulider Data;
	Data.Textures = "EnemyC";
	Data.shader = "Skinning";
	Data.mesh = "EnemyC";

	//애니메이션 넣어줌
	Data.AnimList.push_back("EnemyCMove");
	Data.AnimList.push_back("EnemyCAttack");
	Data.AnimList.push_back("EnemyCReload");
	Data.AnimList.push_back("EnemyCDead");
	Data.AnimList.push_back("EnemyCTarget");
	EnemyC* temp = mFactory->CreateSkinnedObject<EnemyC>(Data, x, y, z);

	//총알넣어줌
	for (int i = 0; i < 10; i++)
	{
		Bullet* b = CreateEnemyBullet(0, 0, 0);
		temp->PushBullet(b);
		b->Setting(1);
	}

	//무기넣어줌
	Data.Textures = "white";
	Data.shader = "Deferred";
	Data.mesh = "EnemyC_Weapon";
	HsMesh* weapon = mFactory->CreateObject<HsMesh>(Data, x, y, z);
	temp->PushWeapon(weapon);


	ParticleSystem* hit_P = mAlpha->CreateParticle("Smoke", 1, 10, 1, -15);
	hit_P->SetParticleTime(0.1f, 1);
	hit_P->SetParticleCount(2);
	hit_P->SetSpeed(Particle_float(0));
	hit_P->SetStartPos(Particle_float3(Particle_float(0, 1), Particle_float(0, 1), Particle_float(0)));
	hit_P->SetStartScl(Particle_float3(Particle_float(1.5f), Particle_float(1.5f), Particle_float(1.5f)));
	hit_P->SetStartRot(Particle_float3(Particle_float(0), Particle_float(0), Particle_float(0)));
	hit_P->SetStartColor(Particle_float3(Particle_float(0, 1), Particle_float(0), Particle_float(0)));
	hit_P->SetParicleEndTime(0.5f);
	temp->PushHitParticle(hit_P);





	PushObjList(temp);
}
void Stage02::Create_EnemyB(float x, float y, float z)
{
	Bulider Data;
	Data.Textures = "EnemyB";
	Data.shader = "Skinning";
	Data.mesh = "EnemyB";

	Data.AnimList.push_back("EnemyBMove");
	Data.AnimList.push_back("EnemyBAttack2");
	Data.AnimList.push_back("EnemyBAttack1");
	Data.AnimList.push_back("GuardWalk");
	Data.AnimList.push_back("EnemyBDead");
	EnemyB* temp = mFactory->CreateSkinnedObject<EnemyB>(Data, x, y, z);


	ParticleSystem* hit_P = mAlpha->CreateParticle("Smoke", 1, 10, 1, -15);
	hit_P->SetParticleTime(0.1f, 1);
	hit_P->SetParticleCount(2);
	hit_P->SetSpeed(Particle_float(0));
	hit_P->SetStartPos(Particle_float3(Particle_float(0, 1), Particle_float(0, 1), Particle_float(0)));
	hit_P->SetStartScl(Particle_float3(Particle_float(1.5f), Particle_float(1.5f), Particle_float(1.5f)));
	hit_P->SetStartRot(Particle_float3(Particle_float(0), Particle_float(0), Particle_float(0)));
	hit_P->SetStartColor(Particle_float3(Particle_float(0, 1), Particle_float(0), Particle_float(0)));
	hit_P->SetParicleEndTime(0.5f);
	temp->PushHitParticle(hit_P);
	PushObjList(temp);
}

Bullet* Stage02::CreateEnemyBullet(float x, float y, float z)
{
	Bulider Data;
	Data.Textures = "white";
	Data.mesh = "box";
	Data.shader = "Deferred";
	Bullet* temp = mFactory->CreateObject<Bullet>(Data, x, y, z);
	temp->transfrom.Scale = { 0.1f,0.1f ,0.1f };

	ParticleSystem* bulletP = mAlpha->CreateParticle("Smoke", 1, 10, 1, -15);
	bulletP->SetParticleTime(0.0025f, 0.5f);
	bulletP->SetSpeed(Particle_float(0));
	bulletP->SetStartPos(Particle_float3(Particle_float(0), Particle_float(0), Particle_float(0)));
	bulletP->SetStartScl(Particle_float3(Particle_float(0.3f), Particle_float(0.3f), Particle_float(0.3f)));
	bulletP->SetStartColor(Particle_float3(Particle_float(0.5f), Particle_float(0), Particle_float(0)));

	temp->PushParticle(bulletP);
	return temp;
}
void Stage02::CreateTrigger(int Type, int Money, string stageName, float x, float y, float z)
{
	Bulider Data;
	Data.Textures = "";
	Data.mesh = "box";
	Data.shader = "Deferred";
	Trigger* temp = mFactory->CreateObject<Trigger>(Data, x, y, z);
	temp->SetMoney(Money);
	ParticleSystem* P = mAlpha->CreateParticle("star", 1, 0, 0, 0);
	P->SetParticleTime(0.0025f, 0.5f);
	P->SetSpeed(Particle_float(0, 2));
	P->SetStartPos(Particle_float3(Particle_float(0, 1), Particle_float(0), Particle_float(0, 1)));
	P->SetStartScl(Particle_float3(Particle_float(0.3f), Particle_float(0.3f), Particle_float(0.3f)));
	switch (Type)
	{
	case 0:
		P->SetStartColor(Particle_float3(Particle_float(0), Particle_float(0), Particle_float(1)));
		break;
	case 1:
		P->SetStartColor(Particle_float3(Particle_float(0.5f), Particle_float(0.5f), Particle_float(0)));
		break;
	case 2:
		P->SetStartColor(Particle_float3(Particle_float(0.5f), Particle_float(0), Particle_float(1)));
		break;
	}


	P->transfrom.Rotation = { -90,0,0 };
	temp->SetScreenName(stageName);
	temp->SetParticle(P);
	PushObjList(temp);
}


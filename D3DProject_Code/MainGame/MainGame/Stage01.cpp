#include "pch.h"
#include "Stage01.h"

Stage01::Stage01()
{
	player = nullptr;
}

Stage01::~Stage01()
{


}

void Stage01::init()
{
	if (mENGINE == nullptr) { return; }
	if (mFactory == nullptr) { return; }

	//사운드 재생
	//hsSound::GM()->Play_BGMSound("Main");
	Navigation2D::ClearMap();
	Navigation2D::CreateMapSize(30,25,false);
	Navigation2D::SetObjectSetting(XMFLOAT2(1,0),1,24);
	Navigation2D::SetObjectSetting(XMFLOAT2(1,0),24,1);
	Navigation2D::SetObjectSetting(XMFLOAT2(24, 0), 1, 5);
	Navigation2D::SetObjectSetting(XMFLOAT2(25, -4), 5, 1);
	Navigation2D::SetObjectSetting(XMFLOAT2(29, -5), 1, 10);
	Navigation2D::SetObjectSetting(XMFLOAT2(28, -14), 1, 10);
	Navigation2D::SetObjectSetting(XMFLOAT2(2, -23), 26, 1);


	Navigation2D::SetObjectSetting(XMFLOAT2(2, -1), 1,1);
	Navigation2D::SetObjectSetting(XMFLOAT2(4, -2), 5, 5);
	Navigation2D::SetObjectSetting(XMFLOAT2(24, -5), 5, 4);
	Navigation2D::SetObjectSetting(XMFLOAT2(26, -9), 3, 1);
	Navigation2D::SetObjectSetting(XMFLOAT2(4, -15), 2, 4);
	Navigation2D::SetObjectSetting(XMFLOAT2(7, -15), 2, 4);
	Navigation2D::SetObjectSetting(XMFLOAT2(20, -15), 6, 6);
	Navigation2D::SetObjectSetting(XMFLOAT2(27, -22), 1, 1);
	Navigation2D::SetObjectSetting(XMFLOAT2(12, -1), 3, 1);
	Navigation2D::SetObjectSetting(XMFLOAT2(17, -1), 3, 1);
	MainPlayer->transfrom.Position = { 3,0,-13 };
	MainPlayer->Update();

	for (int i = 0; i < 30; i++)
	{
		for (int j = 0; j < 25; j++)
		{
			if (Navigation2D::FindObject(i, j))
			{
				Create_Box(i, 0, -j);
			}
			
		}
	}
	
	



	switch (Player::GetStage())
	{
	case 1:
		hsSound::GM()->Play_BGMSound("Stage01");
		Create_EnemyA(9, 0, -18);
		Create_EnemyA(13, 0, -4);
		Create_EnemyA(26, 0, -12);
		Trigger::SetEnemyCount(3);
		CreateTrigger(0, 30,"Stage01", 27, 0, -12);
		break;
	case 2:
		Create_EnemyC(15, 0, -3);
		Create_EnemyC(26, 0, -12);
		Trigger::SetEnemyCount(2);
		CreateTrigger(0, 30,"Stage01", 27, 0, -12);
		break;
	case 3:
		Create_EnemyB(26, 0, -12);
		Trigger::SetEnemyCount(1);
		CreateTrigger(1, 40,"Bonus", 27, 0, -12);
		break;
	case 9:
		hsSound::GM()->Play_BGMSound("Stage01");
		//EnemyA
		Create_EnemyA(2, 0, -19);
		Create_EnemyA(8, 0, -21);
		Create_EnemyA(9, 0, -15);
		Create_EnemyA(11, 0, -3);
		Create_EnemyA(21, 0, -3);
		Create_EnemyA(25, 0, -9);
		Create_EnemyA(22, 0, -21);
		Create_EnemyA(26, 0, -17);
		//EnemyC
		Create_EnemyC(11, 0, -17);
		Create_EnemyC(13, 0, -7);
		Create_EnemyC(18, 0, -9);
		Create_EnemyC(23, 0, -12);
		Trigger::SetEnemyCount(12);
		CreateTrigger(0, 100,"Stage01", 27, 0, -12);
		break;
	case 10:
		//EnemyA
		Create_EnemyA(3, 0, -16);
		Create_EnemyA(7, 0, -20);
		Create_EnemyA(15, 0, -16);
		Create_EnemyA(18, 0, -20);
		//EnemyB
		Create_EnemyB(15, 0, -4);
		Create_EnemyB(10, 0, -17);
		Create_EnemyB(25, 0, -10);
		//EnemyC
		Create_EnemyC(9, 0, -7);
		Create_EnemyC(10, 0, -13);
		Create_EnemyC(21, 0, -2);
		Create_EnemyC(21, 0, -14);
		Trigger::SetEnemyCount(11);
		CreateTrigger(1, 100,"Bonus", 27, 0, -12);
		break;
	case 15:
		hsSound::GM()->Play_BGMSound("StageBoss");
		Create_Boss(9, 0, -18);
		Trigger::SetEnemyCount(1);
		CreateTrigger(1, 100, "Win", 27, 0, -12);
		break;
	}



	CreateMap(-0.5f, 0.0f, 0.5f);
}

void Stage01::Updata()
{
	hsCamera* sub = mCam->GetCam("Sub");
	XMFLOAT3 pos = Player::GetPlayerPosition();
	sub->transfrom.Position = { pos.x,sub->transfrom.Position.y,pos.z - 15 };
	

	//오브젝트들을 업데이트 시켜줌
	Screen::Updata();
}

void Stage01::Render(const hsViewData& data)
{
	//오브젝트들을 랜더링 시켜줌
	Screen::Render(data);
}

void Stage01::End()
{
	Player::SetNextStage();
}



void Stage01::Create_Boss(float x, float y, float z)
{
	Bulider Data;
	Data.Textures	= "Boss";
	Data.shader		= "Skinning";
	Data.mesh		= "Boss";

	//애니메이션 넣어줌
	Data.AnimList.push_back("BossRun");
	Data.AnimList.push_back("BossPunch01");
	Data.AnimList.push_back("BossPunch02");
	Data.AnimList.push_back("BossPunch03");
	Data.AnimList.push_back("BossSkill01");
	Data.AnimList.push_back("BossSkill02");
	Data.AnimList.push_back("BossGroggy1");
	Data.AnimList.push_back("BossGroggy2");
	Data.AnimList.push_back("BossDead");
	Boss* boss = mFactory->CreateSkinnedObject<Boss>(Data, x, y, z);



	Data.Textures = "Reward_chip";
	Data.mesh = "BossBullet";
	Data.shader = "Deferred";
	Bullet* temp = mFactory->CreateObject<Bullet>(Data, x, y, z);
	temp->transfrom.Scale = { 0.1f,0.1f ,0.1f };
	temp->Setting(1);

	//총알에 파티클넣기
	ParticleSystem* bulletP = mAlpha->CreateParticle("Smoke", 1, 10, 1, -15);
	bulletP->SetParticleTime(0.0025f, 0.5f);
	bulletP->SetSpeed(Particle_float(0));
	bulletP->SetStartPos(Particle_float3(Particle_float(0), Particle_float(0), Particle_float(0)));
	bulletP->SetStartScl(Particle_float3(Particle_float(0.3f), Particle_float(0.3f), Particle_float(0.3f)));
	bulletP->SetStartColor(Particle_float3(Particle_float(0.5f), Particle_float(0), Particle_float(0)));

	//파티클넣기
	ParticleSystem* hit_P = mAlpha->CreateParticle("Smoke", 1, 10, 1, -15);
	hit_P->SetParticleTime(0.1f, 1);
	hit_P->SetParticleCount(2);
	hit_P->SetSpeed(Particle_float(0));
	hit_P->SetStartPos(Particle_float3(Particle_float(0, 1), Particle_float(0, 1), Particle_float(0)));
	hit_P->SetStartScl(Particle_float3(Particle_float(1.5f), Particle_float(1.5f), Particle_float(1.5f)));
	hit_P->SetStartRot(Particle_float3(Particle_float(0), Particle_float(0), Particle_float(0)));
	hit_P->SetStartColor(Particle_float3(Particle_float(0, 1), Particle_float(0), Particle_float(0)));
	hit_P->SetParicleEndTime(0.5f);



	temp->PushParticle(bulletP);
	boss->PushBullet(temp);
	boss->PushParticle(hit_P);



	PushObjList(boss);
}



void Stage01::Create_EnemyA(float x, float y, float z)
{
	Bulider Data;
	Data.Textures	= "EnemyC";
	Data.shader		= "Skinning";
	Data.mesh		= "EnemyA";

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

void Stage01::Create_EnemyC(float x, float y, float z)
{
	Bulider Data;
	Data.Textures		= "EnemyC";
	Data.shader			= "Skinning";
	Data.mesh			= "EnemyC";

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

void Stage01::Create_EnemyB(float x, float y, float z)
{
	Bulider Data;
	Data.Textures		= "EnemyB";
	Data.shader			= "Skinning";
	Data.mesh			= "EnemyB";

	Data.AnimList.push_back("EnemyBMove");
	Data.AnimList.push_back("EnemyBAttack2");
	Data.AnimList.push_back("EnemyBAttack1");
	Data.AnimList.push_back("GuardWalk");
	Data.AnimList.push_back("EnemyBDead");
	EnemyB* temp = mFactory->CreateSkinnedObject<EnemyB>(Data, x, y, z);


	ParticleSystem* hit_P = mAlpha->CreateParticle("Smoke", 1, 10, 1, -15);
	hit_P->SetParticleTime(0.1f,1);
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

void Stage01::CreateTrigger(int Type,int Money  ,string stageName, float x, float y, float z)
{
	Bulider Data;
	Data.Textures	= "";
	Data.mesh		= "box";
	Data.shader		= "Deferred";
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



Bullet* Stage01::CreateEnemyBullet(float x, float y, float z)
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

GameObject* Stage01::Create_Box(float x, float y, float z)
{
	Bulider Data;
	Data.Textures		= "";
	Data.mesh			= "box";
	Data.shader			= "Deferred";
	Box* temp = mFactory->CreateObject<Box>(Data, x, y, z);
	temp->transfrom.Scale = { 1,1 ,1};
	PushObjList(temp);
	Navigation2D::SetObjectSetting(temp->transfrom.Position);
	return temp;
}

void Stage01::CreateMap(float x, float y, float z)
{
	XMFLOAT3 scl = { 0.001f,0.001f ,0.001f };
	XMFLOAT3 rot = { 90,0,-90 };
	string shaderName = "Deferred";

	Bulider Data;
	Data.Textures		= "carpet_L_lower";
	Data.mesh			= "carpet_L_lower";
	Data.shader			= shaderName;
	hsMeshList* temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z );
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);

	Data.Textures = "carpet_L_upper";
	Data.mesh = "carpet_L_upper";
	Data.shader = shaderName;
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);
	
	Data.Textures = "carpet_R_lower";
	Data.mesh = "carpet_R_lower";
	Data.shader = shaderName;
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);

	Data.Textures = "cashier_light_Real";
	Data.mesh = "cashier";
	Data.shader = shaderName;
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);

	Data.Textures = "door";
	Data.mesh = "door_t1";
	Data.shader = shaderName;
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);

	Data.Textures = "enterance_01";
	Data.mesh = "enterance_01";
	Data.shader = shaderName;
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);

	Data.Textures = "enterance_02";
	Data.mesh = "enterance_02";
	Data.shader = shaderName;
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);

	Data.Textures = "enterance_03";
	Data.mesh = "enterance_03";
	Data.shader = shaderName;
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);

	Data.Textures = "floor";
	Data.mesh = "floor";
	Data.shader = shaderName;
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);

	Data.Textures = "piller_L";
	Data.mesh = "piller_L";
	Data.shader = shaderName;
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);

	Data.Textures = "piller_M";
	Data.mesh = "piller_M";
	Data.shader = shaderName;
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);

	Data.Textures = "piller_M";
	Data.mesh = "piller_M";
	Data.shader = shaderName;
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x+5.6f, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);

	Data.Textures = "piller_S";
	Data.mesh = "piller_S";
	Data.shader = shaderName;
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);

	Data.Textures = "table_long_lower";
	Data.mesh = "table_long_lower";
	Data.shader = shaderName;
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);

	Data.Textures = "table_long_upper";
	Data.mesh = "table_long_upper";
	Data.shader = shaderName;
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);

	Data.Textures = "table_round_1-1";
	Data.mesh = "table_round_1-1";
	Data.shader = shaderName;
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);

	Data.Textures = "table_round_1-2";
	Data.mesh = "table_round_1-2";
	Data.shader = shaderName;
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);

	Data.Textures = "table_round_1-3";
	Data.mesh = "table_round_1-3";
	Data.shader = shaderName;
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);

	Data.Textures = "table_round_1-4";
	Data.mesh = "table_round_1-4";
	Data.shader = shaderName;
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);

	Data.Textures = "table_round_2-1";
	Data.mesh = "table_round_2-1";
	Data.shader = shaderName;
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);

	Data.Textures = "table_round_2-2";
	Data.mesh = "table_round_2-2";
	Data.shader = shaderName;
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);

	Data.Textures = "table_round_2-3";
	Data.mesh = "table_round_2-3";
	Data.shader = shaderName;
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);

	Data.Textures = "table_round_2-4";
	Data.mesh = "table_round_2-4";
	Data.shader = shaderName;
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);

	Data.Textures = "wall_L_lower";
	Data.mesh = "wall_L_lower";
	Data.shader = shaderName;
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);

	Data.Textures = "wall_L_upper";
	Data.mesh = "wall_L_upper";
	Data.shader = shaderName;
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);

	Data.Textures = "wall_R_lower";
	Data.mesh = "wall_R_lower";
	Data.shader = shaderName;
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);

	Data.Textures = "wall_R_upper";
	Data.mesh = "wall_R_upper";
	Data.shader = shaderName;
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);
}
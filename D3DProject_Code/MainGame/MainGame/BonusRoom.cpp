#include "pch.h"
#include "BonusRoom.h"

BonusRoom::BonusRoom()
{

}

BonusRoom::~BonusRoom()
{
	
}

void BonusRoom::init()
{
	Navigation2D::ClearMap();
	Navigation2D::CreateMapSize(11, 12);
	Navigation2D::SetObjectSetting(XMFLOAT3(9, 0, -1));
	Navigation2D::SetObjectSetting(XMFLOAT3(8, 0, -1));
	Navigation2D::SetObjectSetting(XMFLOAT3(9, 0, -2));

	for (int i = 0; i < 11; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			if (Navigation2D::FindObject(i, -1 * j) == true)
			{
				Create_Box(i, 0, -1 * j);
			}
		}
	}

	Player::SetCamMove(false);
	hsCamera* Main = mCam->GetCam("Main");
	hsCamera* Sub = mCam->GetCam("Sub");
	Main->transfrom.Position = { -5,8,-15 };
	Main->transfrom.Rotation = { 20,45,0 };



	switch (Player::GetStage())
	{
	case 4:
		Trigger::SetEnemyCount(0);
		CreateTrigger(2,100,"Stage02", 9, 0, -9);
		break;
	case 8:
		Trigger::SetEnemyCount(0);
		CreateTrigger(0, 150, "Stage01", 9, 0, -9);
		break;
	case 11:
		Trigger::SetEnemyCount(0);
		CreateTrigger(2, 200, "Stage02", 9, 0, -9);
		break;
	case 14:
		Trigger::SetEnemyCount(0);
		CreateTrigger(2, 200, "Stage01", 9, 0, -9);
		break;
	}



	MainPlayer->transfrom.Position = { 5, 0, -5 };
	hsSound::GM()->Play_BGMSound("BonusRoom");

	CreateHPBOX(3.5f, 0, -1);
	CreateBulletBOX(10, 0, -6.5);
	CreateSlotMahine(8.5f, 0, -2);


	CreateMap(-1, 0, 0);
}

void BonusRoom::Updata()
{
	hsCamera* sub = mCam->GetCam("Sub");
	XMFLOAT3 pos = Player::GetPlayerPosition();
	sub->transfrom.Position = { pos.x,sub->transfrom.Position.y,pos.z - 15 };
	Screen::Updata();
}

void BonusRoom::End()
{
	Player::SetNextStage();
	//if (Player::SetNextStage() == ) 
	//{
	//	
	//}

	Player::SetCamMove(true);
}

void BonusRoom::CreateMap(float x, float y, float z)
{
	XMFLOAT3 scl = { 0.0005f,0.0005f ,0.0005f };
	XMFLOAT3 rot = { 90,0,-90 };
	string shaderName = "Deferred";

	Bulider Data;
	Data.Textures = "Reward_carpet";
	Data.mesh = "Reward_carpet";
	Data.shader = shaderName;
	hsMeshList* temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);

	Data.Textures = "Reward_door";
	Data.mesh = "Reward_door";
	Data.shader = shaderName;
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);

	Data.Textures = "Reward_enterance";
	Data.mesh = "Reward_enterance";
	Data.shader = shaderName;
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);

	Data.Textures = "Reward_floor";
	Data.mesh = "Reward_floor";
	Data.shader = shaderName;
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);

	Data.Textures = "Reward_Pillar_L_lower";
	Data.mesh = "Reward_Pillar_L_lower";
	Data.shader = shaderName;
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);

	Data.Textures = "Reward_Pillar_R_upper";
	Data.mesh = "Reward_Pillar_R_upper";
	Data.shader = shaderName;
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);

	Data.Textures = "Reward_threshold";
	Data.mesh = "thresholdr";
	Data.shader = shaderName;
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);

	Data.Textures = "Reward_UI_pannel";
	Data.mesh = "UI_pannel_Center";
	Data.shader = shaderName;
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);

	Data.Textures = "Reward_UI_pannel";
	Data.mesh = "UI_pannel_L";
	Data.shader = shaderName;
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);

	Data.Textures = "Reward_wall_high";
	Data.mesh = "wall_high";
	Data.shader = shaderName;
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);

	Data.Textures = "Reward_wall_low";
	Data.mesh = "wall_low";
	Data.shader = shaderName;
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);

	Data.Textures = "Reward_wood_long";
	Data.mesh = "wood_long";
	Data.shader = shaderName;
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);

	Data.Textures = "Reward_wood_Short";
	Data.mesh = "wood_short";
	Data.shader = shaderName;
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);


	Data.Textures = "Reward_bullet";
	Data.mesh = "BonusRoom_bullet";
	Data.shader = shaderName;
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);

	Data.Textures = "Reward_heart";
	Data.mesh = "BonusRoom_heart";
	Data.shader = shaderName;
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);

	Data.Textures = "white";
	Data.mesh = "BonusRoom_6";
	Data.shader = shaderName;
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z-1);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);

	Data.Textures = "white";
	Data.mesh = "BonusRoom_10";
	Data.shader = shaderName;
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);

	//Data.Textures = "Reward_Rolling";
	//Data.mesh = "BonusRoom_Rolling";
	//Data.shader = shaderName;
	//temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	//temp->transfrom.Scale = scl;
	//temp->transfrom.Rotation = rot;
	//PushObjList(temp);

	Data.Textures = "Reward_acc";
	Data.mesh = "BonusRoom_acc";
	Data.shader = shaderName;
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);

	Data.Textures = "Reward_chip";
	Data.mesh = "BonusRoom_chip";
	Data.shader = shaderName;
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);

	

	Data.Textures = "Reward_specialslot";
	Data.mesh = "BonusRoom_specialslot";
	Data.shader = shaderName;
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);

	
	
}

void BonusRoom::CreateTrigger(int Type, int Money, string stageName, float x, float y, float z)
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

GameObject* BonusRoom::Create_Box(float x, float y, float z)
{
	Bulider Data;
	Data.Textures = "";
	Data.mesh = "box";
	Data.shader = "Deferred";
	Box* temp = mFactory->CreateObject<Box>(Data, x, y, z);
	temp->transfrom.Scale = { 1,1 ,1 };
	PushObjList(temp);
	Navigation2D::SetObjectSetting(temp->transfrom.Position);
	return temp;
}

void BonusRoom::CreateHPBOX(float x, float y, float z)
{
	Bulider Data;
	Data.Textures = "";
	Data.mesh = "box";
	Data.shader = "Deferred";
	HPBox* temp = mFactory->CreateObject<HPBox>(Data, x, y, z);
	temp->transfrom.Scale = { 1,1 ,1 };
	PushObjList(temp);
}

void BonusRoom::CreateBulletBOX(float x, float y, float z)
{
	Bulider Data;
	Data.Textures = "";
	Data.mesh = "box";
	Data.shader = "Deferred";
	BulletBox* temp = mFactory->CreateObject<BulletBox>(Data, x, y, z);
	temp->transfrom.Scale = { 1,1 ,1 };
	PushObjList(temp);
}

void BonusRoom::CreateSlotMahine(float x, float y, float z)
{
	Bulider Data;
	Data.Textures = "white";
	Data.mesh = "box";
	Data.shader = "Deferred";
	SlotMachine* temp = mFactory->CreateObjectList<SlotMachine>(Data, x, y, z);

	
	Data.Textures = "Reward_Rolling";
	Data.mesh = "BonusRoom_Rolling";
	Data.shader = "Deferred";
	HsMesh* R001 = mFactory->CreateObject<HsMesh>(Data, 1, 0, 0);
	R001->transfrom.Scale = { 0.0005f,0.0005f ,0.0005f };
	R001->MyName = "ROll01";
	R001->ParentName = "TOP_OBJ";
	temp->ChildPush("ROll01", R001);


	Data.Textures = "Reward_Rolling";
	Data.mesh = "BonusRoom_Rolling";
	Data.shader = "Deferred";
	HsMesh* R002 = mFactory->CreateObject<HsMesh>(Data, 2, 0, 0);
	R002->transfrom.Scale = { 0.0005f,0.0005f ,0.0005f };
	R002->MyName = "ROll02";
	R002->ParentName = "TOP_OBJ";
	temp->ChildPush("ROll02", R002);


	Data.Textures = "Reward_Rolling";
	Data.mesh = "BonusRoom_Rolling";
	Data.shader = "Deferred";
	HsMesh* R003 = mFactory->CreateObject<HsMesh>(Data, 3, 0, 0);
	R003->transfrom.Scale = { 0.0005f,0.0005f ,0.0005f };
	R003->MyName = "ROll03";
	R003->ParentName = "TOP_OBJ";
	temp->ChildPush("ROll03", R003);




	


	Data.Textures = "Reward_jocker";
	Data.mesh = "BonusRoom_jocker";
	Data.shader = "Deferred";
	hsMeshList* num = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	num->transfrom.Scale = { 0.0005f,0.0005f ,0.0005f };
	num->transfrom.Position = { x,y + 6,z };
	num->transfrom.Rotation = { 90,0,315 };
	PushObjList(num);
	
	//Data.Textures = "Reward_lever";
	//Data.mesh = "BonusRoom_lever";
	//Data.shader = "Deferred";
	//num = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	//num->transfrom.Scale = { 0.0005f,0.0005f ,0.0005f };
	//num->transfrom.Position = { x+1,y + 2.5f,z-1.25f };
	//num->transfrom.Rotation = { 90,0,315 };
	//PushObjList(num);
	//
	
	PushObjList(temp);
	temp->LinkMesh();
}

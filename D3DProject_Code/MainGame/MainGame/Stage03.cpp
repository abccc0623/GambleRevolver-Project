#include "pch.h"
#include "Stage03.h"

Stage03::Stage03()
{

}

Stage03::~Stage03()
{

}

void Stage03::init()
{
	//LoadMap();
	CreateMap();
}

void Stage03::CreateMap(float x, float y,float z)
{
	XMFLOAT3 scl = { 0.001f,0.001f ,0.001f };
	XMFLOAT3 rot = { 90,0,-90 };

	Bulider Data;
	Data.Textures	= "white";
	Data.mesh		= "2s_basicboard";
	Data.shader		= "Deferred";
	hsMeshList* temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);


	Data.Textures = "white";
	Data.mesh = "2s_billboard";
	Data.shader = "Deferred";
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);
	//
	//
	//
	Data.Textures = "white";
	Data.mesh = "2s_Dia";
	Data.shader = "Deferred";
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);
	//
	//
	Data.Textures = "white";
	Data.mesh = "2s_door";
	Data.shader = "Deferred";
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);


	Data.Textures = "white";
	Data.mesh = "2s_enterance";
	Data.shader = "Deferred";
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);

	Data.Textures = "white";
	Data.mesh = "2s_floor";
	Data.shader = "Deferred";
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);
	
	
	Data.Textures = "white";
	Data.mesh = "2s_pillar_R";
	Data.shader = "Deferred";
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);
	//
	//
	Data.Textures = "white";
	Data.mesh = "2s_pillar_seastory";
	Data.shader = "Deferred";
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);
	//
	//
	Data.Textures = "white";
	Data.mesh = "2s_scoreboard";
	Data.shader = "Deferred";
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);
	
	
	Data.Textures = "white";
	Data.mesh = "2s_shark";
	Data.shader = "Deferred";
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);
	
	
	Data.Textures = "white";
	Data.mesh = "2s_slot_basic";
	Data.shader = "Deferred";
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);
	
	
	Data.Textures = "white";
	Data.mesh = "2s_slot_basic_7_L";
	Data.shader = "Deferred";
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);
	
	
	Data.Textures = "white";
	Data.mesh = "2s_slot_basic_7_R";
	Data.shader = "Deferred";
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);
	
	
	Data.Textures = "white";
	Data.mesh = "2s_slot_round";
	Data.shader = "Deferred";
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);
	
	
	Data.Textures = "white";
	Data.mesh = "2s_slot_seastory";
	Data.shader = "Deferred";
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);
	
	
	Data.Textures = "white";
	Data.mesh = "2s_wall_L";
	Data.shader = "Deferred";
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);
	
	
	Data.Textures = "white";
	Data.mesh = "2s_wall_lower";
	Data.shader = "Deferred";
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);
	
	
	
	Data.Textures = "white";
	Data.mesh = "2s_wall_R";
	Data.shader = "Deferred";
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);
	
	
	Data.Textures = "white";
	Data.mesh = "2s_wall_upper";
	Data.shader = "Deferred";
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);
}

void Stage03::LoadMap()
{
	mFactory->SetResourece(LOAD_TYPE::FBX, "Stage02/2s_basicboard.fbx", "2s_basicboard");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Stage02/2s_billboard.fbx", "2s_billboard");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Stage02/2s_Dia.fbx", "2s_Dia");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Stage02/2s_door.fbx", "2s_door");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Stage02/2s_enterance.fbx", "2s_enterance");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Stage02/2s_floor.fbx", "2s_floor");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Stage02/2s_pillar_R.fbx", "2s_pillar_R");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Stage02/2s_pillar_seastory.fbx", "2s_pillar_seastory");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Stage02/2s_scoreboard.fbx", "2s_scoreboard");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Stage02/2s_shark.fbx", "2s_shark");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Stage02/2s_slot_basic.fbx", "2s_slot_basic");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Stage02/2s_slot_basic_7_L.fbx", "2s_slot_basic_7_L");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Stage02/2s_slot_basic_7_R.fbx", "2s_slot_basic_7_R");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Stage02/2s_slot_round.fbx", "2s_slot_round");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Stage02/2s_slot_seastory.fbx", "2s_slot_seastory");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Stage02/2s_wall_L.fbx", "2s_wall_L");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Stage02/2s_wall_lower.fbx", "2s_wall_lower");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Stage02/2s_wall_R.fbx", "2s_wall_R");
	mFactory->SetResourece(LOAD_TYPE::FBX, "Stage02/2s_wall_upper.fbx", "2s_wall_upper");
}

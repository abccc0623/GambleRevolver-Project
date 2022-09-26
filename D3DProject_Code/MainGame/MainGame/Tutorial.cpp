#include "pch.h"
#include "Tutorial.h"

Tutorial::Tutorial()
{
}

Tutorial::~Tutorial()
{

}

void Tutorial::init()
{
	mCanvas->ClearUI();
	mCanvas->CreateUILayout(3);
	hsSound::GM()->Play_BGMSound("Tutorial");

	Navigation2D::ClearMap();
	Navigation2D::CreateMapSize(11, 12);
	Navigation2D::SetObjectSetting(XMFLOAT3(9,0,-1));
	Navigation2D::SetObjectSetting(XMFLOAT3(8,0,-1));
	Navigation2D::SetObjectSetting(XMFLOAT3(9,0,-2));
	CreateMap(-1,0,0);

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


	Slider* HP_Slider = mCanvas->Create_Slider("Back", "Fill", "Front", "", XMFLOAT3(0, 100, 0), XMFLOAT3(10 * 0.175f, 1 * 0.1f, 1), UI_PIVOT::MID_BOTTOM);
	CountFont* mFont = Create_ImgFont(-365, 195, 0);
	CountFont* mMoneyFont = Create_ImgFont(590, 70, 0);
	mMoneyFont->transform.Scale = { 0.1f,0.1f ,0.1f };
	Image* bulletImg = mCanvas->Create_Image("bullet_UI", 0, 0.7f, XMFLOAT3(-400, 225, 0), XMFLOAT3(0.275f, 0.275f, 1), UI_PIVOT::MID_BOTTOM);
	Image* ChipImg = mCanvas->Create_Image("Chip_UI", 0, 0.7f, XMFLOAT3(580, 100, 0), XMFLOAT3(0.2f, 0.2f, 1), UI_PIVOT::MID_BOTTOM);
	bulletImg->SetAlpha(1);
	bulletImg->PushImg("Reload", mFactory->GetTexture("Reroad_UI"));

	Player* mPlayer = Create_Player(5, 0, -5);
	//플레이어와 UI를 연결
	mPlayer->PushUI(HP_Slider, mFont, mMoneyFont, bulletImg);
	
	MainPlayer = mPlayer;

	Main = mCam->GetCam("Main");
	Sub = mCam->GetCam("Sub");
	Main->transfrom.Position = { -5,8,-15 };
	Main->transfrom.Rotation = { 20,45,0 };

	Player::SetCamMove(false);

	Trigger::SetEnemyCount(0);
	CreateTrigger(9, 0, -9);
	CreateGrid(XMFLOAT2(30, 30), XMFLOAT3(15.5f, 0.5f, -15.5f));
	CreateDirLight();
}

void Tutorial::CreateMap(float x, float y, float z)
{
	XMFLOAT3 scl = { 0.0005f,0.0005f ,0.0005f };
	XMFLOAT3 rot = { 90,0,-90 };
	string shaderName = "Deferred";

	Bulider Data;
	Data.Textures = "Tutorial_Carpet";
	Data.mesh = "Reward_carpet";
	Data.shader = shaderName;
	hsMeshList* temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);

	Data.Textures = "Tutorial_door";
	Data.mesh = "Reward_door";
	Data.shader = shaderName;
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);

	Data.Textures = "Tutorial_enterance";
	Data.mesh = "Reward_enterance";
	Data.shader = shaderName;
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);

	Data.Textures = "Tutorial_floor";
	Data.mesh = "Reward_floor";
	Data.shader = shaderName;
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);

	Data.Textures = "Tutorial_Pillar_L_lower";
	Data.mesh = "Reward_Pillar_L_lower";
	Data.shader = shaderName;
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);

	Data.Textures = "Tutorial_Pillar_R_upper";
	Data.mesh = "Reward_Pillar_R_upper";
	Data.shader = shaderName;
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);

	Data.Textures = "Tutorial_threshold";
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

	Data.Textures = "Tutorial_wall_high";
	Data.mesh = "wall_high";
	Data.shader = shaderName;
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);

	Data.Textures = "Tutorial_wall_low";
	Data.mesh = "wall_low";
	Data.shader = shaderName;
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);

	Data.Textures = "Tutorial_Wood_long";
	Data.mesh = "wood_long";
	Data.shader = shaderName;
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);

	Data.Textures = "Tutorial_Wood_short";
	Data.mesh = "wood_short";
	Data.shader = shaderName;
	temp = mFactory->CreateObjectList<hsMeshList>(Data, x, y, z);
	temp->transfrom.Scale = scl;
	temp->transfrom.Rotation = rot;
	PushObjList(temp);

}

Player* Tutorial::Create_Player(float x, float y, float z)
{
	Bulider Data;
	Data.Textures = "Player";
	Data.shader = "Skinning";
	Data.mesh = "Player";
	
	//애니메이션 넣어줌
	Data.AnimList.push_back("PlayerRun");
	Data.AnimList.push_back("PlayerRoll");
	Data.AnimList.push_back("PlayerAttack");
	Data.AnimList.push_back("PlayerReloadIdle");
	Data.AnimList.push_back("PlayerReloadRun");
	Data.AnimList.push_back("PlayerIdle");
	Data.AnimList.push_back("PlayerDead");
	Data.AnimList.push_back("PlayerSkill1");
	Data.AnimList.push_back("PlayerSkill2");
	Player* temp = mFactory->CreateSkinnedObject<Player>(Data, x, y, z);
	
	//총알넣기
	for (int i = 0; i < 20; i++)
	{
		Bullet* b = CreatePlayerBullet(0, 0, 0);
		temp->PushBullet(b);
		b->Setting(0);
	}
	
	PushDontDestroyList(temp);
	
	//무기 넣기
	Data;
	Data.Textures = "Pistol2";
	Data.shader = "Deferred";
	Data.mesh = "Pistol";
	HsMesh* temp00 = mFactory->CreateObject<HsMesh>(Data, x, y, z);
	temp00->transfrom.Scale = { 0.015f,0.015f ,0.015f };
	temp->PushGun(temp00);
	
	//쏘는 임팩트 넣기
	ParticleSystem* Attack_P = mAlpha->CreateParticle("Shoot", 1, 10, 1, -15);
	Attack_P->SetParticleTime(0.25f, 0.25f);
	Attack_P->SetParticleCount(15);
	Attack_P->SetSpeed(Particle_float(0));
	Attack_P->SetStartPos(Particle_float3(Particle_float(0), Particle_float(0), Particle_float(0)));
	Attack_P->SetStartScl(Particle_float3(Particle_float(0.75f), Particle_float(0.75f), Particle_float(0.75f)));
	Attack_P->SetStartRot(Particle_float3(Particle_float(0), Particle_float(0, 360), Particle_float(0)));
	Attack_P->SetStartColor(Particle_float3(Particle_float(1), Particle_float(1), Particle_float(1)));

	//맞는임팩트 넣기
	ParticleSystem* hit_P = mAlpha->CreateParticle("Smoke", 1, 10, 1, -15);
	hit_P->SetParticleTime(0.1f, 1);
	hit_P->SetParticleCount(2);
	hit_P->SetSpeed(Particle_float(0));
	hit_P->SetStartPos(Particle_float3(Particle_float(0, 1), Particle_float(0, 1), Particle_float(0)));
	hit_P->SetStartScl(Particle_float3(Particle_float(1.5f), Particle_float(1.5f), Particle_float(1.5f)));
	hit_P->SetStartRot(Particle_float3(Particle_float(0), Particle_float(0), Particle_float(0)));
	hit_P->SetStartColor(Particle_float3(Particle_float(0, 1), Particle_float(0), Particle_float(0)));
	hit_P->SetParicleEndTime(0.5f);


	temp->PushParticle(Attack_P, hit_P);


	return temp;
}

Bullet* Tutorial::CreatePlayerBullet(float x, float y, float z)
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
	bulletP->SetStartScl(Particle_float3(Particle_float(0.25f), Particle_float(0.25f), Particle_float(0.25f)));
	bulletP->SetStartColor(Particle_float3(Particle_float(0.5f), Particle_float(0.5f), Particle_float(0)));

	temp->PushParticle(bulletP);
	return temp;
}

CountFont* Tutorial::Create_ImgFont(float x, float y, float z)
{
	CountFont* CFont = new CountFont(x, y, z);
	CFont->init(mENGINE);
	CFont->transform.Scale = { 0.15f,0.15f ,0.15f };
	Bulider data;
	data.shader = "Texture";
	Image* Number[10];
	ID3D11ShaderResourceView* TextureNumber[10];

	Number[0] = mFactory->Create_Image(data);
	Number[1] = mFactory->Create_Image(data);
	Number[2] = mFactory->Create_Image(data);
	Number[3] = mFactory->Create_Image(data);
	Number[4] = mFactory->Create_Image(data);
	Number[5] = mFactory->Create_Image(data);
	Number[6] = mFactory->Create_Image(data);
	Number[7] = mFactory->Create_Image(data);
	Number[8] = mFactory->Create_Image(data);
	Number[9] = mFactory->Create_Image(data);

	TextureNumber[0] = mFactory->GetTexture("0");
	TextureNumber[1] = mFactory->GetTexture("1");
	TextureNumber[2] = mFactory->GetTexture("2");
	TextureNumber[3] = mFactory->GetTexture("3");
	TextureNumber[4] = mFactory->GetTexture("4");
	TextureNumber[5] = mFactory->GetTexture("5");
	TextureNumber[6] = mFactory->GetTexture("6");
	TextureNumber[7] = mFactory->GetTexture("7");
	TextureNumber[8] = mFactory->GetTexture("8");
	TextureNumber[9] = mFactory->GetTexture("9");



	for (int i = 0; i < 10; i++)
	{
		Number[i]->init(mENGINE);
		Number[i]->SetPivot(UI_PIVOT::MID_BOTTOM);
	}

	CFont->SetImg
	(
		TextureNumber[1],
		TextureNumber[2],
		TextureNumber[3],
		TextureNumber[4],
		TextureNumber[5],
		TextureNumber[6],
		TextureNumber[7],
		TextureNumber[8],
		TextureNumber[9],
		TextureNumber[0]
	);

	CFont->SetImgObj
	(
		Number[1],
		Number[2],
		Number[3],
		Number[4],
		Number[5],
		Number[6],
		Number[7],
		Number[8],
		Number[9],
		Number[0]
	);

	mCanvas->PushUI(1, CFont);
	return CFont;
}

GameObject* Tutorial::Create_Box(float x, float y, float z)
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

void Tutorial::CreateTrigger(float x, float y, float z)
{
	Bulider Data;
	Data.Textures = "";
	Data.mesh = "box";
	Data.shader = "Deferred";
	Trigger* temp = mFactory->CreateObject<Trigger>(Data, x, y, z);
	temp->SetMoney(0);
	ParticleSystem* P = mAlpha->CreateParticle("star", 1, 0, 0, 0);
	P->SetParticleTime(0.0025f, 0.5f);
	P->SetSpeed(Particle_float(0,2));
	P->SetStartPos(Particle_float3(Particle_float(0,1), Particle_float(0), Particle_float(0, 1)));
	P->SetStartScl(Particle_float3(Particle_float(0.3f), Particle_float(0.3f), Particle_float(0.3f)));
	P->SetStartColor(Particle_float3(Particle_float(0), Particle_float(0), Particle_float(1)));
	P->transfrom.Rotation = {-90,0,0};
	temp->SetScreenName("Stage01");
	temp->SetParticle(P);
	PushObjList(temp);
}

void Tutorial::Updata()
{
//	Sub->transfrom.Position = {-10,10,-10};
	hsCamera* sub = mCam->GetCam("Sub");
	XMFLOAT3 pos = Player::GetPlayerPosition();
	sub->transfrom.Position = { pos.x,sub->transfrom.Position.y,pos.z - 15 };
	Screen::Updata();
}

void Tutorial::End()
{
	Player::SetNextStage();
	Player::SetCamMove(true);
}

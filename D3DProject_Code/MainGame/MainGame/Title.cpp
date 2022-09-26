#include "pch.h"
#include "Title.h"

Title::Title()
{

}

Title::~Title()
{

}

void Title::init()
{
	//켄버스의 레이아웃을 3개로
	mCanvas->CreateUILayout(3);

	//로드 경로 지정
	mFactory->SetLoadPath(LOAD_TYPE::TEXTURES, "../../Textures/");
	mFactory->SetLoadPath(LOAD_TYPE::ASE, "../../ASEFile/");
	mFactory->SetLoadPath(LOAD_TYPE::FBX, "../../FBXFile/");


	//UI
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "UI/Stemina_line.dds", "Front");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "UI/Stemina_base.dds", "Back");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "UI/Stemina_bar.dds", "Fill");

	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Title.dds",		"Title");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "UI/Loading.dds",	"Loading");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "UI/gameover.dds",	"UI_GAMEOVER");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "UI/Win.dds",		"UI_WIN");

	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "UI/IngameUI_bullet.dds",		"bullet_UI");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "UI/IngameUI_Chip.dds",		"Chip_UI");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "UI/IngameUI_road_02.dds",		"Reroad_UI");


	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "Stage01/test.dds", "x3");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "start.dds",	"start");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "end.dds",		"end");
	mFactory->SetResourece(LOAD_TYPE::TEXTURES, "stone.dds",	"stone");


	Image* img = mCanvas->Create_Image("Title", 0, 0.7f, XMFLOAT3(0, 0, 0), XMFLOAT3(1, 1, 1), UI_PIVOT::MID);
	img->FullScreen = true;

	

	hsSound::GM()->LoadSound_BGM("../../Sound/Title.mp3",		"Title");
	hsSound::GM()->LoadSound_BGM("../../Sound/Tutorial.mp3",	"Tutorial");
	hsSound::GM()->LoadSound_BGM("../../Sound/RewardRoom.mp3",	"BonusRoom");
	hsSound::GM()->LoadSound_BGM("../../Sound/Stage01.mp3",		"Stage01");
	hsSound::GM()->LoadSound_BGM("../../Sound/2StageTheme.mp3",		"Stage02");
	hsSound::GM()->LoadSound_BGM("../../Sound/BossTheme.mp3",		"StageBoss");
	hsSound::GM()->LoadSound_BGM("../../Sound/EndingSound.wav",	"Win");



	hsSound::GM()->LoadSound_EX("../../Sound/GameOverSound.mp3",	"GameOver");
	hsSound::GM()->LoadSound_EX("../../Sound/Effect/PlayerHitSound2.wav",	"PlayerHit");
	hsSound::GM()->LoadSound_EX("../../Sound/Effect/RevolverReload.mp3",	"PlayerReload");
	hsSound::GM()->LoadSound_EX("../../Sound/Effect/RevolverShoot.mp3",		"PlayerAttack");
	hsSound::GM()->LoadSound_EX("../../Sound/Effect/DashSound.mp3",			"Dash");
	hsSound::GM()->LoadSound_EX("../../Sound/Effect/HealingSelect.mp3",	"Healing");
	hsSound::GM()->LoadSound_EX("../../Sound/Effect/MenuSelectSound.mp3",	"UI");
	hsSound::GM()->LoadSound_EX("../../Sound/Effect/PlayerDeathSound.mp3",	"PlayerDead");


	hsSound::GM()->LoadSound_EX("../../Sound/Effect/EnemyA_Attack.mp3", "EnemyA_Attack");
	hsSound::GM()->LoadSound_EX("../../Sound/Effect/EnemyB_punch.mp3", "EnemyB_Attack");
	hsSound::GM()->LoadSound_EX("../../Sound/Effect/EnemyBDefenceSoundEffect.mp3", "EnemyB_Skill");
	hsSound::GM()->LoadSound_EX("../../Sound/Effect/EnemyC_ReloadSound.mp3", "EnemyC_Reload");
	hsSound::GM()->LoadSound_EX("../../Sound/Effect/gun.mp3",					"EnemyC_Attack");
	hsSound::GM()->LoadSound_EX("../../Sound/Effect/EnemyHit.mp3",					"Enemy_Hit01");
	hsSound::GM()->LoadSound_EX("../../Sound/Effect/EnemyHit2.mp3",					"Enemy_Hit02");

	hsSound::GM()->LoadSound_EX("../../Sound/Effect/EnemyADeadSound.wav",					"EnemyA_Dead");
	hsSound::GM()->LoadSound_EX("../../Sound/Effect/EnemyBDeathSound.wav",					"EnemyB_Dead");
	hsSound::GM()->LoadSound_EX("../../Sound/Effect/EnemyCDeadSound.wav",					"EnemyC_Dead");

	hsSound::GM()->LoadSound_EX("../../Sound/Effect/SlotMachineRolling.mp3", "Rolling");
	hsSound::GM()->LoadSound_EX("../../Sound/Effect/JackPotSound.wav", "JackPot");
	hsSound::GM()->LoadSound_EX("../../Sound/Effect/StageClearSound.mp3", "Clear");


	hsSound::GM()->Play_BGMSound("Title");
}

void Title::End()
{
	Screen::End();
}

void Title::Updata()
{

	if (mENGINE->GetKey()->GetKeyDown(VK_SPACE) || mENGINE->GetKey()->GetKeyDown(VK_RETURN))
	{

		if (count == 1)
		{
			//게임종료
		}
		else
		{
			mCanvas->ClearUI();
			mCanvas->CreateUILayout(3);
			Image* img = mCanvas->Create_Image("Loading", 0, 0.7f, XMFLOAT3(0, 0, 0), XMFLOAT3(1, 1, 1), UI_PIVOT::MID);
			img->FullScreen = true;

			hsSound::GM()->Play_EXSound("UI");
			ScreenManager::MoveScreen("Loading");
		}
	}

	Screen::Updata();
}

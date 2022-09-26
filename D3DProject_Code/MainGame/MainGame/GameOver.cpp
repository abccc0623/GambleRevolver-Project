#include "pch.h"
#include "GameOver.h"

GameOver::GameOver()
{

}

GameOver::~GameOver()
{

}

void GameOver::init()
{
	hsSound::GM()->Play_EXSound("GameOver");
	hsSound::GM()->Stop_BGM();
	mCanvas->ClearUI();
	mCanvas->CreateUILayout(3);
	Image* img = mCanvas->Create_Image("UI_GAMEOVER", 0, 0.7f, XMFLOAT3(0, 0, 0), XMFLOAT3(1, 1, 1), UI_PIVOT::MID);
	img->FullScreen = true;
}

void GameOver::End()
{
	mCanvas->ClearUI();
	mCanvas->CreateUILayout(3);
	Screen::End();
}

void GameOver::Updata()
{
	if (mENGINE->GetKey()->GetKeyDown(VK_SPACE) || mENGINE->GetKey()->GetKeyDown(VK_RETURN))
	{
		ScreenManager::MoveScreen("Tutorial");
	}


	Screen::Updata();
}


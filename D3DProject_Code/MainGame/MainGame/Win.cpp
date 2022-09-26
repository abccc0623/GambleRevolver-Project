#include "pch.h"
#include "Win.h"

Win::Win()
{
}

Win::~Win()
{
}

void Win::init()
{
	//mCanvas->ClearUI();
	//mCanvas->CreateUILayout(3);
	Image* img = mCanvas->Create_Image("UI_WIN", 0, 0.7f, XMFLOAT3(0, 0, 0), XMFLOAT3(1, 1, 1), UI_PIVOT::MID);
	hsSound::GM()->Play_BGMSound("Win");
	img->FullScreen = true;
}

void Win::End()
{
	mCanvas->ClearUI();
	mCanvas->CreateUILayout(3);
	Screen::End();
}

void Win::Updata()
{
	Screen::Updata();
}

//#include <afxwin.h>

#ifdef SHOW_HOW_TO_USE_TCI #define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)
#else #define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)
#endif


#include "pch.h"
GameManager::GameManager()
{
	ENGINE = nullptr;
	m_Factory = nullptr;
	m_CamManager = nullptr;
}

GameManager::~GameManager()
{
	ENGINE = nullptr;

}

void GameManager::init(hsEngine* _Engine)
{
	ENGINE = _Engine;

	//펙토리 생성
	m_Factory = new Factory();
	m_Factory->init(_Engine);

	//스크린 매니저 생성
	m_ScreenManager = new ScreenManager();
	m_ScreenManager->init(_Engine, m_Factory);

	//카메라 매니저 생성
	m_CamManager = new CamManager();
	m_CamManager->init(_Engine,m_Factory);

	//스크린에 카메라 매니저 넘겨줌
	m_ScreenManager->SetCamManager(m_CamManager);

	//사운드매니저 생성
	hsSound::GM()->init();
	
	//랜더링 매니저 생성
	RenderManager::GetRM()->init(_Engine);
	RenderManager::GetRM()->CreateTextureMap("기본텍스쳐", TEXTURE_BUFFER_TYPE::Basic,256,256);

	Create();
}

void GameManager::Update()
{
	//매니저들 업데이트
	m_CamManager->MainCamInputKeyUpdate();
	m_ScreenManager->Update();
	hsSound::GM()->Updata();
	//Player->transfrom.SetRotation(0, 20 *ENGINE->GetDeltaTime(), 0);


	if (ENGINE->GetKey()->GetKeyDown(VK_F7))
	{
		m_CamManager->ChoiceMainCam("Main");
	}

	if (ENGINE->GetKey()->GetKeyDown(VK_F6))
	{
		m_CamManager->ChoiceMainCam("Sub");
	}

	if (ENGINE->GetKey()->GetKeyDown(VK_F8))
	{
		m_CamManager->ChoiceMainCam("Look");
	}
}

void GameManager::Render()
{
	hsCamera* main = m_CamManager->GetMainCam();
	hsCamera* Shadow = m_CamManager->GetCam("Sub");
	hsCamera* UICam = m_CamManager->GetCam("UI");


	//쉐도우를 그려주기위해 한번 랜더링
	RenderManager::GetRM()->ShadowMapRender(Shadow, m_ScreenManager);
	//디퍼드를 위해 한번더 랜더링
	RenderManager::GetRM()->DeferredRender(main, m_ScreenManager);
	//알파랜더링
	RenderManager::GetRM()->AlphaRender(main, m_ScreenManager);
	//UI 랜더링
	RenderManager::GetRM()->UIRender(UICam, m_ScreenManager);
	
	Debug();
}

void GameManager::Debug()
{
	if (ENGINE->GetKey()->GetKeyToggle(VK_F1))
	{
		ENGINE->OnDebug = true;

		int point = 0;
		//ENGINE->GetFont()->DrawTextColor(0, point + 25, XMFLOAT4(1, 0, 0, 1), (TCHAR*)L"F1 디버깅 ON/OFF");
		//ENGINE->GetFont()->DrawTextColor(0, point + 50, XMFLOAT4(1, 0, 0, 1), (TCHAR*)L"F2 블러 ON/OFF");
		//ENGINE->GetFont()->DrawTextColor(0, point + 75, XMFLOAT4(1, 0, 0, 1), (TCHAR*)L"-------카메라 변경---------");
		//ENGINE->GetFont()->DrawTextColor(0, point + 100, XMFLOAT4(1, 0, 0, 1), (TCHAR*)L"F6 메인");
		//ENGINE->GetFont()->DrawTextColor(0, point + 125, XMFLOAT4(1, 0, 0, 1), (TCHAR*)L"F7 서브");
		//ENGINE->GetFont()->DrawTextColor(0, point + 150, XMFLOAT4(1, 0, 0, 1), (TCHAR*)L"Time:%f",ENGINE->GetDeltaTime());
		//ENGINE->GetFont()->DrawTextColor(0, point + 175, XMFLOAT4(1, 0, 0, 1), (TCHAR*)L"FPS:%d",ENGINE->GetFPS());

		
		//XMFLOAT3 pos = m_CamManager->GetMainCam()->GetClickPoint();
		//Light->transfrom.Position = pos;
	}
	else
	{
		ENGINE->OnDebug = false;
	}
}


void GameManager::Create()
{
	///포스트 프로세싱을 위한 준비들
	//오브젝트를 찍을 카메라
	hsCamera* RenderCam = m_CamManager->CreateCamera("Look");
	hsCamera* LookCam	= m_CamManager->CreateCamera("Render");

	Bulider data;
	data.shader = "Post";
	postObj = m_Factory->Create_TextureObj(data);
	postObj->SetRenderState(POST_TYPE::BLUR_Radial);

	//포스트 프로세싱 준비완료
	RenderManager::GetRM()->Post_Processing_Start(RenderCam, LookCam, postObj);
	RenderManager::GetRM()->Deferred_Render_Start();

	//카메라 매니저에 서브 카메라를 넣어줌
	hsCamera* Sub = m_CamManager->CreateCamera("Sub", 15, 15, -30);
	m_CamManager->GetCam("Sub")->SetRotation(50, 0, 0);

	//ui를 보여줄 카메라 생성
	hsCamera* ui = m_CamManager->CreateCamera("UI",0, 0, 0);

	//메인카메라를 선택(선택하면 방향키로 이동가능...)
	hsCamera* main = m_CamManager->CreateCamera("Main", 0, 10, -20);
	m_CamManager->ChoiceMainCam("Main");
	m_CamManager->GetMainCam()->transfrom.SetRotation(45, 45, 0);

	//스크린을 만들자
	Tutorial*	tutorial	= new Tutorial();
	Title*		title		= new Title();
	Loading*	loading		= new Loading();
	BonusRoom*	Bonus		= new BonusRoom();
	GameOver*	Gameover	= new GameOver();
	Win*		GameWin		= new Win();


	Stage01*	stage01		= new Stage01();
	Stage02*	stage02		= new Stage02();
	Stage03*	stage03		= new Stage03();
	
	//만든 스크린을 넣어주고

	//안헷갈리게 순서대로
	m_ScreenManager->SetScreen("Title",		title);
	m_ScreenManager->SetScreen("Loading",	loading);
	m_ScreenManager->SetScreen("Tutorial",	tutorial);
	m_ScreenManager->SetScreen("Bonus",		Bonus);
	m_ScreenManager->SetScreen("GameOver",	Gameover);
	m_ScreenManager->SetScreen("GameWin", GameWin);

	m_ScreenManager->SetScreen("Stage01", stage01);
	m_ScreenManager->SetScreen("Stage02", stage02);
	m_ScreenManager->SetScreen("Stage03", stage03);


	//시작 스크린을 지정해줌
	m_ScreenManager->ChoiceScreen(title);
	//디버깅을 위해 서브카메라와 메인카메라를 넣어줌
	m_ScreenManager->PushDontDestroy(Sub);

	m_ScreenManager->PushDontDestroy(main);
	//UI를 찍을 ui 카메라..
	m_ScreenManager->PushDontDestroy(ui);
}

void GameManager::Delete()
{
	m_ScreenManager->DeleteCheck();
}

TextureObj* GameManager::Create_Texture(float x, float y, float z)
{
	Bulider Data;
	Data.Textures		= "stone";
	Data.SubTextures	= "dirt";
	Data.shader			= "Texture";
	TextureObj* temp = m_Factory->Create_TextureObj(Data,x, y, z);
	return temp;
}




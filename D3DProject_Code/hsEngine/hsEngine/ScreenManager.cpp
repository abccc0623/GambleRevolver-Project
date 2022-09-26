#include "EnginePCH.h"
#include "ScreenManager.h"


map<string, Screen*> ScreenManager::ScreenList;
bool	ScreenManager::NextScreen = false;
string	ScreenManager::NextScreenName = "";
ScreenManager::ScreenManager()
{
	NowScreen = nullptr;
	isStart = false;
}

ScreenManager::~ScreenManager()
{
	mENGINE = nullptr;
	mFactory = nullptr;
}

void ScreenManager::init(hsEngine* mEngine, Factory* factory)
{
	mENGINE = mEngine;
	mFactory = factory;

	//라이트를 위한 라이트 매니저를 만들어줌
	mLight = new LightManager;
	mLight->init(mEngine, mFactory);

	//UI를 위한켄버스를 만들어줌
	canvas = new Canvas();
	canvas->init(mENGINE,mFactory);

	//알파매니저를 만들어줌
	mAlpha = new AlphaManager();
	mAlpha->init(mENGINE, mFactory);
}

void ScreenManager::PushDontDestroy(GameObject* obj)
{
	DontDestroyList.push_back(obj);
}

void ScreenManager::PushObject(GameObject* obj)
{
	NowObjList.push_back(obj);
}

void ScreenManager::SetScreen(string Name, Screen* mScreen)
{
	//엔진, 펙토리 ,켄버스, 라이트매니저 넘겨줌
	mScreen->Create(mENGINE, mFactory,mLight,canvas,mAlpha,mCam, &NowObjList,&DontDestroyList);
	ScreenList.insert({ Name,mScreen });
}

void ScreenManager::MoveScreen(string Name)
{
	//이동할 스크린이름을 받아놓는다
	NextScreenName = Name;
	NextScreen = true;
}

void ScreenManager::MoveScreen()
{
	if (NowScreen == nullptr) { return; }


	//이전스크린의 정리가 필요함
	NowScreen->Clear();
	NowScreen->End();


	//맵을검색해서 현재 스크린을 변경해줌
	map<string, Screen*>::iterator it = ScreenList.begin();
	for (it; it != ScreenList.end(); it++)
	{
		if (it->first == NextScreenName)
		{
			//새로 스크린을 지정한다
			NowScreen = it->second;
			isStart		= false;
			NextScreen	= false;
			return;
		}
	}

	//만약 찾지못하면
	if (NowScreen == nullptr)
	{
		//에러뿜뿜
	}
}

void ScreenManager::ChoiceScreen(Screen* mScreen)
{
	NowScreen = mScreen;
	isStart = false;
}

void  ScreenManager::GetObjList(vector<GameObject*>* obj, vector<GameObject*>* DDL)
{
	if (NowScreen == nullptr) { return ; }
	obj = &NowObjList;
	DDL = &DontDestroyList;
}

void ScreenManager::Update()
{
	//한번만 실행시켜주기위해서
	if (isStart == false)
	{
		NowScreen->init();
		isStart = true;
	}
	else
	{
		NowScreen->Updata();
	}
}

void ScreenManager::Render(const hsViewData& data)
{
	//랜더링은 업데이트 다음이기때문에 init을 실행했냐안했냐만 체크
	if (isStart == true)
	{
		NowScreen->Render(data);
	}


	//모든 랜더링 업데이트가 끝나고 씬은변경할건지 체크한다
	if (NextScreen == true) 
	{
		MoveScreen();
	}
}

void ScreenManager::DeleteCheck()
{
	if (NowScreen == nullptr) { return; }

	//삭제할 오브젝트들은 체크한다
	NowScreen->Delete();
}

void ScreenManager::SetCamManager(CamManager* cam)
{
	mCam = cam;
}

Canvas* ScreenManager::GetCanvas()
{
	return canvas;
}

LightManager* ScreenManager::GetLightManager()
{
	return mLight;
}

AlphaManager* ScreenManager::GetAlphaManager()
{
	return mAlpha;
}

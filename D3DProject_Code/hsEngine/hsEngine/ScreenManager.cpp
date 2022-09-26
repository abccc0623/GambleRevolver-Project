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

	//����Ʈ�� ���� ����Ʈ �Ŵ����� �������
	mLight = new LightManager;
	mLight->init(mEngine, mFactory);

	//UI�� �����˹����� �������
	canvas = new Canvas();
	canvas->init(mENGINE,mFactory);

	//���ĸŴ����� �������
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
	//����, ���丮 ,�˹���, ����Ʈ�Ŵ��� �Ѱ���
	mScreen->Create(mENGINE, mFactory,mLight,canvas,mAlpha,mCam, &NowObjList,&DontDestroyList);
	ScreenList.insert({ Name,mScreen });
}

void ScreenManager::MoveScreen(string Name)
{
	//�̵��� ��ũ���̸��� �޾Ƴ��´�
	NextScreenName = Name;
	NextScreen = true;
}

void ScreenManager::MoveScreen()
{
	if (NowScreen == nullptr) { return; }


	//������ũ���� ������ �ʿ���
	NowScreen->Clear();
	NowScreen->End();


	//�����˻��ؼ� ���� ��ũ���� ��������
	map<string, Screen*>::iterator it = ScreenList.begin();
	for (it; it != ScreenList.end(); it++)
	{
		if (it->first == NextScreenName)
		{
			//���� ��ũ���� �����Ѵ�
			NowScreen = it->second;
			isStart		= false;
			NextScreen	= false;
			return;
		}
	}

	//���� ã�����ϸ�
	if (NowScreen == nullptr)
	{
		//�����ջ�
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
	//�ѹ��� ��������ֱ����ؼ�
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
	//�������� ������Ʈ �����̱⶧���� init�� �����߳ľ��߳ĸ� üũ
	if (isStart == true)
	{
		NowScreen->Render(data);
	}


	//��� ������ ������Ʈ�� ������ ���������Ұ��� üũ�Ѵ�
	if (NextScreen == true) 
	{
		MoveScreen();
	}
}

void ScreenManager::DeleteCheck()
{
	if (NowScreen == nullptr) { return; }

	//������ ������Ʈ���� üũ�Ѵ�
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

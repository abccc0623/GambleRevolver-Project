#include "EnginePCH.h"
#include "Screen.h"



GameObject* Screen::MainPlayer = nullptr;

hsCamera* Screen::MainCam	= nullptr;
hsCamera* Screen::ShadowCam = nullptr;
hsCamera* Screen::SubCam	= nullptr;
Screen::Screen()
{
	mENGINE = nullptr;
	mFactory = nullptr;
}

Screen::~Screen()
{
	mENGINE = nullptr;
	mFactory = nullptr;

	Clear();
}

void Screen::Create(hsEngine* Engine, Factory* factory,LightManager* _Light,Canvas* _canvas,AlphaManager* _alpha,CamManager* Cam,vector<GameObject*>* objlist, vector<GameObject*>* DDLlist)
{
	mENGINE = Engine;
	mFactory = factory;
	mObjList = objlist;
	mDontDestroyList = DDLlist;
	mLight	= _Light;
	mCanvas = _canvas;
	mAlpha	= _alpha;
	mCam	= Cam;
}

void Screen::Updata()
{
	vector<GameObject*>::iterator it = (*mObjList).begin();
	for (it; it != (*mObjList).end(); it++)
	{
		if ((*it) == nullptr) { continue; }

		(*it)->Update();
	}

	vector<GameObject*>::iterator DDL = (*mDontDestroyList).begin();
	for (DDL; DDL != (*mDontDestroyList).end(); DDL++)
	{
		if ((*DDL) == nullptr) { continue; }

		(*DDL)->Update();
	}

	mCanvas->Update();
	mLight->Update();
	mAlpha->Update();
}

void Screen::Render(const hsViewData& data)
{
	//오브젝트 랜더
	vector<GameObject*>::iterator it = (*mObjList).begin();
	for (it; it != (*mObjList).end(); it++)
	{
		if ((*it) == nullptr) { continue; }

		if ((*it)->OnRender == true)
		{
			(*it)->Render(data);
		}


		if (mENGINE->OnDebug == true)
		{
			(*it)->Debug();
		}
	}


	//삭제하면안되는 오브젝트들 랜더
	vector<GameObject*>::iterator DDL = (*mDontDestroyList).begin();
	for (DDL; DDL != (*mDontDestroyList).end(); DDL++)
	{
		if ((*DDL) == nullptr) { continue; }

		(*DDL)->Render(data);
		if (mENGINE->OnDebug == true)
		{
			(*DDL)->Debug();
		}
	}

	
	if (mENGINE->OnDebug == true)
	{
		mCanvas->Debug();
		mLight->Debug();
		mAlpha->Debug();
	}
}

void Screen::Delete()
{
	vector<GameObject*>::iterator it = (*mObjList).begin();
	for (it; it != (*mObjList).end(); it++)
	{
		if ((*it) == nullptr) { continue; }

		if ((*it)->GetDeleteCheck() == true)
		{
			delete (*it);
			(*it) = nullptr;
		}
	}

	vector<GameObject*>::iterator DDL = (*mDontDestroyList).begin();
	for (DDL; DDL != (*mDontDestroyList).end(); DDL++)
	{
		if ((*DDL) == nullptr) { continue; }

		if ((*DDL)->GetDeleteCheck() == true)
		{
			delete(*DDL);
			(*DDL) = nullptr;
		}
	}
	mAlpha->Delete();
	
}

void Screen::End()
{
	///다음씬에 넘어가기전에 준비같은거? 해주면될듯
	
}

void Screen::Clear()
{
	//이씬을 정리해준다

	vector<GameObject*>::iterator it = (*mObjList).begin();
	for (it; it != (*mObjList).end(); it++)
	{
		if ((*it) == nullptr) { continue; }

		delete (*it);
	}

	mObjList->clear();
	mObjList->resize(0);
	mLight->Delete();
}

void Screen::PushObjList(GameObject* temp)
{
	//오브젝트를 생성하고 넣어줄때 빈곳을 찾아 넣어준다
	vector<GameObject*>::iterator it = (*mObjList).begin();
	for (it; it != (*mObjList).end(); it++)
	{
		if ((*it) == nullptr) 
		{
			(*it) = temp;
			return;
		}
	}


	mObjList->push_back(temp);
}

void Screen::PushDontDestroyList(GameObject* temp)
{

	//오브젝트 리스트에 빼주고 삭제하면 안되는 리스트에 넣어준다
	vector<GameObject*>::iterator it = (*mObjList).begin();
	for (it; it != (*mObjList).end(); it++)
	{
		if ((*it) == temp)
		{
			(*it) = nullptr;
			break;
		}
	}

	//삭제하면 안되는 리스트에 넣는다
	vector<GameObject*>::iterator DDL = (*mDontDestroyList).begin();
	for (DDL; DDL != (*mDontDestroyList).end(); DDL++)
	{
		if ((*DDL) == nullptr)
		{
			(*DDL) = temp;
			return;
		}
	}
	mDontDestroyList->push_back(temp);
}

LightObject* Screen::CreateLight(float x, float y, float z)
{
	LightObject* temp = mFactory->Create_LightObj(1, x,y, z);
	mLight->PushLightObject(temp);
	return temp;
}

LightObject* Screen::CreateDirLight()
{

	LightObject* temp = mFactory->Create_LightObj(0, 0, 10, 0);
	temp->transfrom.Rotation = { 0,0,90 };
	mLight->PushLightObject(temp);
	return temp;
}

void Screen::CreateGrid(XMFLOAT2 size, XMFLOAT3 pos, XMFLOAT4 color)
{
	GameObject* temp = mFactory->Create_Grid(size.x, size.y, color, pos);
	PushObjList(temp);
}



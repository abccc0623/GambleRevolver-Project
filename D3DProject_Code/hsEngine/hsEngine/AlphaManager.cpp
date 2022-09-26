#include "EnginePCH.h"
#include "AlphaManager.h"

AlphaManager::AlphaManager()
{
	mENGINE = nullptr;
	mFactory = nullptr;
}

AlphaManager::~AlphaManager()
{
	mENGINE = nullptr;
	mFactory = nullptr;
}

void AlphaManager::init(hsEngine* _Engine, Factory* _Factory)
{
	mENGINE = _Engine;
	mFactory = _Factory;
}

void AlphaManager::Update()
{
	vector<GameObject*>::iterator it = AlphaList.begin();
	for (it; it != AlphaList.end(); it++)
	{
		if ((*it) == nullptr) { continue; }

		(*it)->Update();
	}
}

void AlphaManager::Render()
{
	vector<GameObject*>::iterator it = AlphaList.begin();
	for (it; it != AlphaList.end(); it++)
	{
		if ((*it) == nullptr) { continue; }

		(*it)->Render(hsViewData());
	}
}

void AlphaManager::Debug()
{
	vector<GameObject*>::iterator it = AlphaList.begin();
	for (it; it != AlphaList.end(); it++)
	{
		if ((*it) == nullptr) { continue; }

		(*it)->Debug();
	}
}

void AlphaManager::Delete()
{
	vector<GameObject*>::iterator it = AlphaList.begin();
	for (it; it != AlphaList.end(); it++)
	{
		if ((*it) == nullptr) { continue; }
		if ((*it)->isDelete == true) 
		{
			(*it)->Delete();
		}
	}
}

void AlphaManager::PushObj(GameObject* obj)
{
	AlphaList.push_back(obj);
}

ParticleSystem* AlphaManager::CreateParticle(string TextureName, int Count, float x, float y, float z)
{
	Bulider data;
	data.Textures = TextureName;
	data.shader = "Particle";
	ParticleSystem* temp = mFactory->Create_Particle(data, x, y, z);
	PushObj(temp);
	return temp;
}

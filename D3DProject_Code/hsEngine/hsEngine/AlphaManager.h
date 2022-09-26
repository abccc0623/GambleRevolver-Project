#pragma once

/// <summary>
/// 알파 랜더링이 되어야하는 오브젝트들을 따로 관리하기위해서
/// </summary>
class AlphaManager
{
public:
	AlphaManager();
	~AlphaManager();

	void init(hsEngine* _Engine, Factory* _Factory);
	void Update();
	void Render();
	void Debug();
	void Delete();

	void PushObj(GameObject* obj);
	ParticleSystem* CreateParticle(string TextureName,int Count, float x  = 0, float y = 0, float z = 0);

private:
	hsEngine* mENGINE;
	Factory* mFactory;
	vector<GameObject*> AlphaList;
};
#pragma once

/// <summary>
/// ���� �������� �Ǿ���ϴ� ������Ʈ���� ���� �����ϱ����ؼ�
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
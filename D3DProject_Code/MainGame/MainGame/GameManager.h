#pragma once

class intro;
class GameManager
{
public:
	GameManager();
	~GameManager();

	//펙토리 매니저
	Factory*		m_Factory;
	//카메라 매니저
	CamManager*		m_CamManager;
	//스크린 매니저 
	ScreenManager*	m_ScreenManager;
	//라이트 매니저
	LightManager*	m_LightManager;
	
	
	void init(hsEngine* _Engine);
	void Update();
	void Render();
	void Debug();
	
	//생성
	void Create();
	void Delete();
private:
	hsEngine* ENGINE;
	TextureObj* postObj;
	TextureObj*	Create_Texture(float x = 0, float y = 0, float z = 0);
};
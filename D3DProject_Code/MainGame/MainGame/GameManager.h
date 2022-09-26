#pragma once

class intro;
class GameManager
{
public:
	GameManager();
	~GameManager();

	//���丮 �Ŵ���
	Factory*		m_Factory;
	//ī�޶� �Ŵ���
	CamManager*		m_CamManager;
	//��ũ�� �Ŵ��� 
	ScreenManager*	m_ScreenManager;
	//����Ʈ �Ŵ���
	LightManager*	m_LightManager;
	
	
	void init(hsEngine* _Engine);
	void Update();
	void Render();
	void Debug();
	
	//����
	void Create();
	void Delete();
private:
	hsEngine* ENGINE;
	TextureObj* postObj;
	TextureObj*	Create_Texture(float x = 0, float y = 0, float z = 0);
};
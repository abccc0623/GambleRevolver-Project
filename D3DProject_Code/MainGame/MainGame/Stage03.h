#pragma once
class Stage03 : public Screen
{
public:
	Stage03();
	virtual~Stage03();
	// Screen��(��) ���� ��ӵ�
	virtual void init() override;
	void CreateMap(float x = 0, float y = 0,float z =0);
	void LoadMap();
};


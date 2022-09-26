#pragma once
class Stage03 : public Screen
{
public:
	Stage03();
	virtual~Stage03();
	// Screen을(를) 통해 상속됨
	virtual void init() override;
	void CreateMap(float x = 0, float y = 0,float z =0);
	void LoadMap();
};


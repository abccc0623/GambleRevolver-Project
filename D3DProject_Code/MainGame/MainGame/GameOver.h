#pragma once
class GameOver: public Screen
{
public:
	GameOver();
	virtual ~GameOver();
	// Screen��(��) ���� ��ӵ�
	virtual void init() override;
	virtual void End();
	virtual void Updata();
};
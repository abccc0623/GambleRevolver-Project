#pragma once
class GameOver: public Screen
{
public:
	GameOver();
	virtual ~GameOver();
	// Screen을(를) 통해 상속됨
	virtual void init() override;
	virtual void End();
	virtual void Updata();
};
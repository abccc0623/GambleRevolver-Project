#pragma once
class AI 
{
public:
	AI(float x = 0, float y = 0, float z = 0);
	virtual ~AI();


protected:
	int State;
	float HP;

	float AttackTime;
};
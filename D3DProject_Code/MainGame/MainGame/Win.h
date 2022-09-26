#pragma once
class Win :public Screen
{
public:
	Win();
	virtual ~Win();

	virtual void init() override;
	virtual void End();
	virtual void Updata();
};

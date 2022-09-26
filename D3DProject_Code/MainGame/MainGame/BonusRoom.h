#pragma once
class BonusRoom : public Screen
{
public:
	BonusRoom();
	virtual ~BonusRoom();
	virtual void init() override;
	virtual void Updata();
	virtual void End();
private:
	void CreateMap(float x = 0, float y = 0, float z = 0);
	void CreateTrigger(int Type, int Money, string stageName, float x, float y, float z);
	GameObject* Create_Box(float x = 0, float y = 0, float z = 0);


	void CreateHPBOX(float x = 0, float y = 0, float z = 0);
	void CreateBulletBOX(float x = 0, float y = 0, float z = 0);
	void CreateSlotMahine(float x = 0, float y = 0, float z = 0);
};
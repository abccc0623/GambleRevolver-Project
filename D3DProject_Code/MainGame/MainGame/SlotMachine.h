#pragma once
class SlotMachine :public hsMeshList ,public Collider
{
public:
	SlotMachine(float x = 0, float y = 0, float z = 0);
	virtual~SlotMachine();
	
	virtual void Initialize(hsEngine* ENGINE);
	virtual void Update();
	virtual void Render(const hsViewData& view);
	void PushSlot(GameObject* one, GameObject* two, GameObject* three);
	void Choice();
	void Reset();
	void GetMoney();



	//0 = ·¹¸ó
	//1 = Ã¼¸®
	//2 = 777
	//3 = ²Î

	int state;

	GameObject* Slot01;
	GameObject* Slot02;
	GameObject* Slot03;


	bool ishit;
	bool isGO;
	bool ischoice;
	bool isSound;

	bool num1_ON;
	bool num2_ON;
	bool num3_ON;
	


	float RollTime;
	float GoTime;


	float one = 0;
	float two = 0;
	float three = 0;
	float speed = 100;


	
};
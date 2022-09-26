#include "pch.h"
#include "SlotMachine.h"

SlotMachine::SlotMachine(float x, float y, float z)
{
	transfrom.Position = { x,y,z };
	ishit = false;

	num1_ON = false;
	num2_ON = false;
	num3_ON = false;

	RollTime = 0;
	GoTime = 0;
	ischoice = false;
	state = 3;
}

SlotMachine::~SlotMachine()
{

}

void SlotMachine::Initialize(hsEngine* ENGINE)
{
	hsMeshList::Initialize(ENGINE);
	
	Collider::SetBoundingSphere(transfrom.Position, 1, Collider::Obj);
}

void SlotMachine::Update()
{
	time(NULL);
	transfrom.Rotation = { 0,-45,0 };
	GameObject* obj01 = MeshList["ROll01"];
	GameObject* obj02 = MeshList["ROll02"];
	GameObject* obj03 = MeshList["ROll03"];
	
	
	if (ENGINE->GetKey()->GetKeyDown(VK_NUMPAD0))
	{
		obj01->transfrom.SetRotation(10, 0, 0);
		obj02->transfrom.SetRotation(10, 0, 0);
		obj03->transfrom.SetRotation(10, 0, 0);
	}
	
	
	
	
	
	
	obj01->transfrom.Position = { -0.008f,  -0.01f,-0.0275f };
	obj02->transfrom.Position = { 0.000f	,-0.01f,-0.0275f };
	obj03->transfrom.Position = { 0.008f	,-0.01f,-0.0275f };
	
	
	if (Collider::OnCollisionEnter())
	{
		if (Collider::SphereCollider.HitType & hitObj)
		{
			hsSound::GM()->Play_EXSound("EnemyB_Skill");
	
	
			if (Player::GetMoney() >= 25)
			{
				//ÇÃ·¹ÀÌ¾î µ· 1°¨¼Ò
				ishit = true;
				Player::MoneyDown(25);
			}
		}
	}
	
	float DTime = ENGINE->GetDeltaTime();
	
	
	if (ishit == true)
	{
		if (isSound == false) 
		{
			hsSound::GM()->Play_EXSound("Rolling");
			isSound = true;
		}
	
		Collider::Updata(XMFLOAT3(1000, 1000, 1000));
		RollTime += DTime;
		speed += 0.25f;
		if (RollTime >= 0.1f && num1_ON ==false)
		{
			one += DTime* speed;
			obj01->transfrom.Rotation = { one, 0, 0 };
		}
		if (RollTime >= 0.5f && num2_ON == false)
		{
			two += DTime * speed;
			obj02->transfrom.Rotation = { two, 0, 0 };
		}
		if (RollTime >= 1 && num3_ON == false)
		{
			three += DTime * speed;
			obj03->transfrom.Rotation = { three, 0, 0 };
		}


		if (RollTime >= 4)
		{
			//ishit = false;
			isGO = true;
			if (ischoice == false)
			{
				int number = (rand()) % (int)100;
				//·¹¸ó
				if (number >= 0 && number < 20)
				{
					//·¹¸ó
					Player::MoneyUp(50);
					state = 0;
				}
				else if (number >= 20 && number < 30) 
				{
					//Ã¼¸®
					state = 1;
					Player::MoneyUp(250);
				}
				else if (number >= 30 && number < 35)
				{
					//ÀèÆÌ
					Player::MoneyUp(2500);
					state = 2;
				}
				else if (number >= 35 && number < 60)
				{
					state = 3;
				}
				else if (number >= 60 && number < 80) 
				{
					state = 4;
				}
				else
				{
					state = 5;
				}
	
				ischoice = true;
			}
	
	
		}
	}
	else
	{
		Collider::Updata(XMFLOAT3(8, 1, -4));
	}
	
	if (isGO == true)
	{
		GoTime += DTime;
	
		if (GoTime >= 0.1f)
		{
			num1_ON = true;
			switch (state)
			{
			case 0:
				obj01->transfrom.Rotation = { 110, 0, 0 };
				break;
			case 1:
				obj01->transfrom.Rotation = { 50, 0, 0 };
				break;
			case 2:
				obj01->transfrom.Rotation = { 0, 0, 0 };
				break;

				//¹ØÀ¸·Î ²Î
			case 3:
				obj01->transfrom.Rotation = { 50, 0, 0 };
				break;
			case 4:
				obj01->transfrom.Rotation = { 0, 0, 0 };
				break;
			case 5:
				obj01->transfrom.Rotation = { 100, 0, 0 };
				break;
	
			}
		}
		if (GoTime >= 0.5f)
		{
			num2_ON = true;
			switch (state)
			{
			case 0:
				obj02->transfrom.Rotation = { 110, 0, 0 };
				break;
			case 1:
				obj02->transfrom.Rotation = { 50, 0, 0 };
				break;
			case 2:
				obj02->transfrom.Rotation = { 0, 0, 0 };
				break;
			case 3:
				obj02->transfrom.Rotation = { 50, 0, 0 };
				break;
			case 4:
				obj02->transfrom.Rotation = { 100, 0, 0 };
				break;
			case 5:
				obj02->transfrom.Rotation = { 0, 0, 0 };
				break;
	
			}
		}
		if (GoTime >= 1)
		{
			num3_ON = true;
			switch (state)
			{
			case 0:
				hsSound::GM()->Play_EXSound("JackPot");
				obj03->transfrom.Rotation = { 110, 0, 0 };
				break;
			case 1:
				hsSound::GM()->Play_EXSound("JackPot");
				obj03->transfrom.Rotation = { 50, 0, 0 };
				break;
			case 2:
				hsSound::GM()->Play_EXSound("JackPot");
				obj03->transfrom.Rotation = { 0, 0, 0 };
				break;
			case 3:
				obj03->transfrom.Rotation = { 0, 0, 0 };
				break;
			case 4:
				obj03->transfrom.Rotation = { 100, 0, 0 };
				break;
			case 5:
				obj03->transfrom.Rotation = { 50, 0, 0 };
				break;
			}
			Reset();
		}
	}


	hsMeshList::Update();
}

void SlotMachine::Render(const hsViewData& view)
{
	hsMeshList::Render(view);
}

void SlotMachine::PushSlot(GameObject* one, GameObject* two, GameObject* three)
{
	Slot01 = one;
	Slot02 = two;
	Slot03 = three;
}

void SlotMachine::Choice()
{
	
}

void SlotMachine::Reset()
{
	isGO = false;
	ishit = false;
	RollTime = 0;
	speed = 100;
	GoTime = 0;
	isSound = false;
	num1_ON = false;
	num2_ON = false;
	num3_ON = false;
	ischoice = false;
	one = 0;
	two = 0;
	three = 0;
}



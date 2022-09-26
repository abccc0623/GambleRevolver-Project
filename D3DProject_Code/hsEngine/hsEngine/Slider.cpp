#include "EnginePCH.h"
#include "Slider.h"

Slider::Slider(float x,float y,float z)
{
	transform.Position = { x,y,z };
	SliderStartPos = {-500,0};
	SliderEndPos = {0,0};
	Slider_Fill_Pos = 0;
	Slider_Fild_Scl = 0;
	Slider_Value = 1;
	Fill = nullptr;
	Slider_Front = nullptr;
	Hand = nullptr;
}

Slider::~Slider()
{

}

void Slider::Update()
{
	XMFLOAT3 pos = transform.Position;
	XMFLOAT3 rot = transform.Rotation;
	XMFLOAT3 scl = transform.Scale;


	//������ �����̴��� �����̸� ���� �����̰�
	//�����̴�������ϴ� �̹����� ũ�Ⱑ �þ������ ��ġ���� �޶������Ѵ�
	

	Slider_Fill_Pos = MathHelper::Lerp<float>(0, 256 * scl.x, Slider_Value);
	Slider_Fild_Scl = MathHelper::Lerp<float>(0, scl.x, Slider_Value);
	
	Slider_Back->transform.Position = { pos.x, pos.y, pos.z };
	Slider_Back->transform.Rotation = rot;
	Slider_Back->transform.Scale = { scl.x,scl.y ,scl.z };

	Fill->transform.Position	= { pos.x +((-256*scl.x)+Slider_Fill_Pos),pos.y ,pos.z };
	Fill->transform.Rotation	= rot;
	Fill->transform.Scale		= { Slider_Fild_Scl, scl.y  ,scl.z };

	Slider_Front->transform.Position	= { pos.x, pos.y, pos.z };
	Slider_Front->transform.Rotation	= rot;
	Slider_Front->transform.Scale		= { scl.x,scl.y,scl.z };
	
	Hand->transform.Position	= { pos.x, pos.y, pos.z };
	Hand->transform.Rotation	= rot;
	Hand->transform.Scale		= { scl.x,scl.y ,scl.z };



	Slider_Back->Update();
	Fill->Update();
	Slider_Front->Update();
	Hand->Update();
}

void Slider::Render()
{
	//������ ������ ���Ѿ��Ѵ�
	//���ϸ��� ���̹��� �ϰ�
	//�״��� ���̹���
	//�״��� �ڵ� (�ڵ��� ���������ִ�)
	Slider_Back->Render();
	Fill->Render();
	Slider_Front->Render();
	Hand->Render();
}

void Slider::Delete()
{
	delete Slider_Back;
	delete Fill;
	delete Slider_Front;
	delete Hand;
}

void Slider::SetFillImg(Image* img)
{
	Fill = img;
}

void Slider::SetSliderImg(Image* img)
{
	Slider_Front = img;
}

void Slider::SetHandImg(Image* img)
{
	Hand = img;
}

void Slider::SetBackGroundImg(Image* img)
{
	Slider_Back = img;
}

void Slider::SetHandleValue(float mValue)
{
	if (mValue > 1)
	{
		Slider_Value = 1;
	}
	else if (mValue < 0)
	{
		Slider_Value = 0;
	}
	

	Slider_Value = mValue;
}

float Slider::GetHandleValue()
{
	
	return Slider_Value;
}



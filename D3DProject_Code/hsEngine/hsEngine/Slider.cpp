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


	//원본인 슬라이더가 움직이면 따라서 움직이게
	//슬라이더를담당하는 이미지는 크기가 늘어날때마다 위치값도 달라져야한다
	

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
	//랜더링 순서를 지켜야한다
	//제일먼저 뒤이미지 하고
	//그다음 앞이미지
	//그다음 핸들 (핸들은 없을수도있다)
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



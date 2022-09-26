#include "EnginePCH.h"
#include "Button.h"

Button::Button(float x, float y, float z)
{
	transform.Position = { x,y,z };
	
}

Button::~Button()
{


}

void Button::init(hsEngine* _ENGINE)
{
	UI::init(_ENGINE);
}

void Button::SetRect(float Left, float Top, float Right, float Bottom)
{
	Rect = { Left,Top,Right,Bottom };
}

void Button::SetAutoRect()
{
	float Left		= (-256 * mScreen_Scl.x) + mScreen_Pos.x;
	float Top		= ( 256 * mScreen_Scl.y) + mScreen_Pos.y;
	float Right		= ( 256 * mScreen_Scl.x) + mScreen_Pos.x;
	float Bottom	= (-256 * mScreen_Scl.y) + mScreen_Pos.y;
}

void Button::Update()
{
	LPPOINT point = ENGINE->GetKey()->GetMousePos();
	



	UI::Update();
}

void Button::Render()
{
	UI::Render();
}

void Button::Delete()
{
	UI::Delete();
}

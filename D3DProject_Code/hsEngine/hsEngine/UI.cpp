#include "EnginePCH.h"
#include "UI.h"

UI::UI(float x, float y, float z)
{
	ENGINE = nullptr;
	mWorldBuffer = nullptr;
	mWorld = XMMATRIX();
	transform.Position = { x,y,z };
	Alpha = 1;
	pivotX = 0;
	pivotY = 0;
}

UI::~UI()
{
	mWorldBuffer = nullptr;
}

void UI::init(hsEngine* _ENGINE)
{
	ENGINE = _ENGINE;
	Rendering::init(_ENGINE);

	vector<UI32> vData;
	vector<UINT> uData;

	vData.resize(4);
	uData.resize(6);

	vData[0] = { Vector4(-1,1, 0,1),	Vector2(0,0) };
	vData[1] = { Vector4(1,	1, 0,1),	Vector2(1,0) };
	vData[2] = { Vector4(-1,-1, 0,1),	Vector2(0,1) };
	vData[3] = { Vector4(1,-1, 0,1),	Vector2(1,1) };

	uData[0] = 0;
	uData[1] = 1;
	uData[2] = 2;
	uData[3] = 1;
	uData[4] = 3;
	uData[5] = 2;

	Rendering::Create_IB_VB_Buffer(vData, uData);
	mWorldBuffer = GetBuffer("UIBuffer");
}

void UI::Update()
{
	///스크린좌표에서 움직일수있도록 좌표를 변경함


	//기준점을 오른쪽 제일위로 잡았음 그래야 화면이 늘어나도 UI가 자리를 지킨다
	//이동량 * 이미지의크기(현재는 256을1로 잡음) * 한픽셀의 크기 

	mScreen_Pos = { pivotX +(Canvas::OnePixelX * transform.Position.x) , pivotY +(Canvas::OnePixelY * transform.Position.y),0};
	mScreen_Rot = { transform.Rotation.x, transform.Rotation.y, transform.Rotation.z };
	mScreen_Scl = {Canvas::OnePixelX * 256 * transform.Scale.x , Canvas::OnePixelY * 256 * transform.Scale.y, 0.1f };


	XMMATRIX P = transform.SetPosition4X4(mScreen_Pos);
	XMMATRIX R = transform.SetRotation4X4(mScreen_Rot);
	XMMATRIX S = transform.SetScale4X4(mScreen_Scl);	

	mWorld = S * R * P;
}

void UI::ChangeImg(string Name)
{
	//현재 이미지를 잠시넣어둠
	ID3D11ShaderResourceView* temp = mMainSRV;
	bool Swap = false;
	bool NoPush = false;
	

	//현재 이미지와 다른이미지를변경한다
	map<string, ID3D11ShaderResourceView*>::iterator it = ImgList.begin();
	for (it; it != ImgList.end(); it++)
	{
		//찾는 이미지가 있으면
		if (it->first == Name)
		{
			//이미지를 변경
			mMainSRV = it->second;
			Swap = true;
		}

		//리스트에 이미 스왑할 이미지가 있다면 안넣어줘도됨
		if (it->second == temp)
		{
			NoPush = true;
		}

		//스왑도 했고 다시넣어줄필요도없다면 그대로 리턴
		if (Swap == true && NoPush == true)
		{
			return;
		}
	}
	
	//만약넣어줘야한다면 Main이라는 이름으로 넣어줌
	if (NoPush == false)
	{
		ImgList.insert({"Main",temp});
	}
}

void UI::PushImg(string Name, ID3D11ShaderResourceView* Texture)
{
	ImgList.insert({ Name,Texture });
}


void UI::Render()
{
	Set_UIBuffer_Parameter(mWorldBuffer,mWorld,XMFLOAT3(1,1,1), Alpha);
	MeshRendering();
}

void UI::Debug()
{
	
}
void UI::Delete()
{
	ENGINE = nullptr;
	mWorldBuffer = nullptr;
}
void UI::SetAlpha(float number)
{
	Alpha = number;
}
void UI::SetPivot(UI_PIVOT pivot)
{
	switch (pivot)
	{
	case UI_PIVOT::LEFT_TOP:
		pivotX = -1;
		pivotY =  1;
		break;
	case UI_PIVOT::LEFT_BOTTOM:
		pivotX = -1;
		pivotY = -1;
		break;
	case UI_PIVOT::RIGHT_TOP:
		pivotX = 1;
		pivotY = 1;
		break;
	case UI_PIVOT::RIGHT_BOTTOM:
		pivotX = 1;
		pivotY = -1;
		break;
	case UI_PIVOT::MID_UP:
		pivotX = 0;
		pivotY = 1;
		break;
	case UI_PIVOT::MID_RIGHT:
		pivotX = 1;
		pivotY = 0;
		break;
	case UI_PIVOT::MID_LEFT:
		pivotX = -1;
		pivotY = 0;
		break;
	case UI_PIVOT::MID_BOTTOM:
		pivotX = 0;
		pivotY = -1;
		break;
	case UI_PIVOT::MID:
		pivotX = 0;
		pivotY = 0;
		break;
	}
}

void UI::SetTexture(ID3D11ShaderResourceView* mTexture)
{
	Rendering::mMainSRV = mTexture;
}




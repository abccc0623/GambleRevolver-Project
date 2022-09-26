#include "EnginePCH.h"
#include "Canvas.h"


float Canvas::OnePixelX = 0;
float Canvas::OnePixelY = 0;
float Canvas::MaxX = 0;
float Canvas::MaxY = 0;
float Canvas::MinX = 0;
float Canvas::MinY = 0;
float Canvas::OneSizeX = 0;
float Canvas::OneSizeY = 0;
void Canvas::init(hsEngine* _Engine, Factory* _Factory)
{
	mENGINE = _Engine;
	mFactory = _Factory;
}

void Canvas::Update()
{
	for (int i = 0; i < UILauoutList.size(); i++)
	{
		for (int j = 0; j < UILauoutList[i].size(); j++)
		{
			if (UILauoutList[i][j] == nullptr) { continue; }

			UILauoutList[i][j]->Update();
		}
	}
}

void Canvas::Render()
{
	for (int i = 0; i < UILauoutList.size();i++)
	{
		for (int j = 0; j < UILauoutList[i].size(); j++)
		{
			if (UILauoutList[i][j] == nullptr) { continue; }

			UILauoutList[i][j]->Render();
		}
	}
}

void Canvas::Debug()
{
	for (int i = 0; i < UILauoutList.size(); i++)
	{
		for (int j = 0; j < UILauoutList[i].size(); j++)
		{
			if (UILauoutList[i][j] == nullptr) { continue; }

			UILauoutList[i][j]->Debug();
		}
	}
}

void Canvas::Delete()
{
	for (int i = 0; i < UILauoutList.size(); i++)
	{
		for (int j = 0; j < UILauoutList[i].size(); j++)
		{
			if (UILauoutList[i][j] == nullptr) { continue; }

			UILauoutList[i][j]->Delete();
		}
	}
	
}

void Canvas::PushUI(int layout, UI* ui)
{
	UILauoutList[layout].push_back(ui);
}

void Canvas::CreateUILayout(int LayoutSize)
{
	UILauoutList.resize(LayoutSize);
}

void Canvas::ClearUI()
{
	for (int i = 0; i < UILauoutList.size(); i++)
	{
		for (int j = 0; j < UILauoutList[i].size(); j++)
		{
			if (UILauoutList[i][j] == nullptr) { continue; }

			UILauoutList[i][j]->Delete();
			delete UILauoutList[i][j];
			UILauoutList[i][j] = nullptr;
		}
	}
	UILauoutList.clear();
}

void Canvas::CreateScreenNumber(int Width, int Height)
{
	//ȭ���� ũ�⸦ ���Ѵ�
	float BoundaryX = Width;
	float BoundaryY = Height;

	//���ȼ��� ũ�⸦ ���Ѵ�
	OnePixelX = 1 / BoundaryX;
	OnePixelY = 1 / BoundaryY;

	//ȭ���� �������� ��ǥ
	MaxX = OnePixelX * BoundaryX;
	MaxY = OnePixelY * BoundaryY;

	//ȭ���� ���ʾƷ� ��ǥ
	MinX = OnePixelX * -BoundaryX;
	MinY = OnePixelY * -BoundaryY;
}

Image* Canvas::Create_Image(string TextureName,int Layout,float Alpha,XMFLOAT3 Pos,XMFLOAT3 Scl,UI_PIVOT pivot)
{
	Bulider data;
	data.Textures = TextureName;
	data.shader = "Texture";
	Image* ui = mFactory->Create_Image(data, Pos.x, Pos.y, Pos.z);
	ui->transform.Scale = Scl;
	ui->SetPivot(pivot);
	ui->SetAlpha(Alpha);
	ui->init(mENGINE);
	PushUI(Layout, ui);

	return ui;
}

Image* Canvas::Create_Button(string TextureName, int Layout, float Alpha, XMFLOAT3 Pos, XMFLOAT3 Scl, UI_PIVOT pivot)
{
	Bulider data;
	data.Textures = TextureName;
	data.shader = "Texture";
	Image* ui = mFactory->Create_Image(data, Pos.x, Pos.y, Pos.z);
	ui->transform.Scale = Scl;
	ui->SetPivot(pivot);
	ui->SetAlpha(Alpha);
	ui->init(mENGINE);
	PushUI(Layout, ui);

	return nullptr;
}

Slider* Canvas::Create_Slider(string Back, string Fill, string Front, string hand, XMFLOAT3 Pos, XMFLOAT3 Scl, UI_PIVOT pivot)
{
	//�ּ� �̹����� 2�� �ʿ��� (�����̴� �ʿ������ ������)

	Slider* slider = new Slider(Pos.x,Pos.y,Pos.z);
	slider->transform.Scale = Scl;
	
	//3���� �̹������������
	Bulider data;
	data.Textures = Back;
	data.shader = "Texture";
	Image* B = mFactory->Create_Image(data, Pos.x, Pos.y, Pos.z);
	//B->transform.Scale = Scl;
	B->SetPivot(pivot);
	B->SetAlpha(1);
	B->init(mENGINE);
	
	data.Textures = Front;
	data.shader = "Texture";
	Image* F = mFactory->Create_Image(data, Pos.x, Pos.y, Pos.z);
	//F->transform.Scale = Scl;
	F->SetPivot(pivot);
	F->SetAlpha(1);
	F->init(mENGINE);

	data.Textures = Fill;
	data.shader = "Texture";
	Image* FILL = mFactory->Create_Image(data, Pos.x, Pos.y, Pos.z);
	//H->transform.Scale = Scl;
	FILL->SetPivot(pivot);
	FILL->SetAlpha(1);
	FILL->init(mENGINE);
	
	data.Textures = hand;
	data.shader = "Texture";
	Image* H = mFactory->Create_Image(data, Pos.x, Pos.y, Pos.z);
	//H->transform.Scale = Scl;
	H->SetPivot(pivot);
	H->SetAlpha(1);
	H->init(mENGINE);


	//�����̴��� �̹����� �־���
	slider->SetFillImg(FILL);
	slider->SetSliderImg(F);
	slider->SetHandImg(H);
	slider->SetBackGroundImg(B);

	PushUI(0, slider);

	return slider;
}

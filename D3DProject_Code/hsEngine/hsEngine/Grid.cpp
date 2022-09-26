
#include "EnginePCH.h"
#include "Grid.h"

Grid::Grid(float x, float y, float z)
{
	transfrom.Position = { x,y,z };
	SizeX = 20;
	SizeY = 20;
	m_Color = { 1,1,1,1 };
}

Grid::~Grid()
{
}

void Grid::init(hsEngine* _ENGINE)
{
	//엔진을 가져옴
	ENGINE = _ENGINE;
	Device = ENGINE->GetDeviceData()->m_device;
	mContext = ENGINE->GetDeviceData()->m_deviceContext;
	Debugging::init(_ENGINE);
	//debug->init(_ENGINE);
	//
	//그리드를 그린다
	for (int i = -SizeX; i <= SizeX; i++)
	{
		Create_Line_Buffers(XMFLOAT3(i,0,-SizeX), XMFLOAT3(i,0, SizeX), m_Color);
	}
	
	for (int j = -SizeY; j <= SizeY; j++)
	{
		Create_Line_Buffers(XMFLOAT3(-SizeY, 0, j), XMFLOAT3(SizeY, 0, j), m_Color);
	}
	//
	////축도 그린다
	Create_Line_Buffers(XMFLOAT3(0, 0.1f, 0), XMFLOAT3(0, 0.1f, -50), XMFLOAT4(1, 0, 0, 1));
	Create_Line_Buffers(XMFLOAT3(0, 0.1f, 0), XMFLOAT3(0, 50, 0), XMFLOAT4(0, 1, 0, 1));
	Create_Line_Buffers(XMFLOAT3(0, 0.1f, 0), XMFLOAT3(50, 0.1f, 0), XMFLOAT4(0, 0, 1, 1));
	//
	////이대로 버퍼생성
	CreateDebugBuffers();
}

void Grid::Render(const hsViewData& view)
{
	mView = view.view;
	mProj = view.proj;
	//debug->SetWorldMT(transfrom.GetMaster());
	//debug->Render(view);
}
void Grid::SetOption(float X, float Y, XMFLOAT4 Color)
{
	SizeX = X;
	SizeY = Y;
	m_Color = Color;
}
void Grid::Update()
{
	
}

void Grid::Delete()
{

}

void Grid::Debug()
{
	mWorld = transfrom.GetMaster();
	Debugging::Update(mWorld);
}
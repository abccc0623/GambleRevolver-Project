//#pragma comment(lib,"d3dcompiler.lib") //���̴� �ڵ带 �ҷ����µ� �ʿ���

#include "DeviceData.h"
#include "GameObject.h"
#include "EnginePCH.h"

Box::Box(float X, float Y, float Z)
{
	transfrom.Position = { X,Y,Z };
}

Box::~Box()
{
	
}

void Box::init(hsEngine* _Engine,LoadData* data)
{	
	HsMesh::init(_Engine, data);
	Debugging::Create_Box_Buffers(XMFLOAT3(0, 0.5f, 0),XMFLOAT3(0.5f, 0.5f, 0.5f));
	CreateDebugBuffers();
	boxbuffer = GetBuffer("ObjectBuffer");
}

void Box::Update()
{
	HsMesh::Update();
}

void Box::Render(const hsViewData& view)
{
	//HsMesh::Render(view);
}

void Box::Debug()
{
	Debugging::Update(mWorld);
}







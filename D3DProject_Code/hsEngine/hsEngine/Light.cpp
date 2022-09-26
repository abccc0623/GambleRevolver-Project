#include "EnginePCH.h"
#include "Light.h"

Light::Light(float x, float y, float z)
{
	transfrom.Position = { x,y,z };
}

Light::~Light()
{
}

void Light::init(hsEngine* _ENGINE)
{
	ENGINE = _ENGINE;
	Device = ENGINE->GetDeviceData()->m_device;
	mContext = ENGINE->GetDeviceData()->m_deviceContext;

	Rendering::init(_ENGINE);


	Debugging::init(ENGINE);
	Debugging::Create_Box_Buffers(transfrom.Position, XMFLOAT3(2, 2, 2));
	Debugging::Create_Circle_Buffers(transfrom.Position, 1, XMFLOAT4(1, 1, 1, 1));
	Debugging::CreateDebugBuffers();

	Rendering::GetBuffer("");
}

void Light::Update()
{
	mWorld = transfrom.GetMaster();
}

void Light::Render(const hsViewData& view)
{

}

void Light::Delete()
{

}

void Light::Debug()
{
	Debugging::Update(mWorld);
}

#include "EnginePCH.h"
#include "TextureObj.h"

TextureObj::TextureObj(float x, float y, float z)
{
	transfrom.Position = { x,y,z };
	RenderType = 0;
}

TextureObj::~TextureObj()
{
	ENGINE = nullptr;
	Device = nullptr;
	mContext = nullptr;
}

void TextureObj::init(hsEngine* _ENGINE)
{
	ENGINE = _ENGINE;
	Device = ENGINE->GetDeviceData()->m_device;
	mContext = ENGINE->GetDeviceData()->m_deviceContext;
	Rendering::init(_ENGINE);
	//Debugging::init(_ENGINE);
	

	vector<Light32> vData;
	vector<UINT> uData;

	vData.resize(4);
	uData.resize(6);
	TexSize = transfrom.Scale.x;
	vData[0] = { Vector4(-TexSize,	 TexSize, 0,1),	Vector2(0,0) };
	vData[1] = { Vector4(TexSize,	 TexSize, 0,1),	Vector2(1,0) };
	vData[2] = { Vector4(-TexSize,	-TexSize, 0,1),	Vector2(0,1) };
	vData[3] = { Vector4(TexSize,	-TexSize, 0,1),	Vector2(1,1) };
	

	uData[0] = 0;
	uData[1] = 1;
	uData[2] = 2;
	uData[3] = 1;
	uData[4] = 3;
	uData[5] = 2;
	

	Create_IB_VB_Buffer(vData, uData);
	mWorldBuffer = GetBuffer("ObjectBuffer");
	//mCamBuffer = GetBuffer("CameraBuffer");
}

void TextureObj::Update()
{
	mWorld = transfrom.GetMaster();
}

void TextureObj::Render(const hsViewData& view)
{
	Set_ObjectBuffer_Parameter(mWorldBuffer,transfrom.GetMaster(), XMFLOAT3(1, 1, 1));
	MeshRendering();
}

void TextureObj::Debug()
{
	//Debugging::Update(mWorld);
}

void TextureObj::SetRenderState(POST_TYPE Type)
{
	RenderType = (int)Type;
}







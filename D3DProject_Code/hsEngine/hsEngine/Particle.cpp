#include "EnginePCH.h"
#include "Particle.h"

Particle::Particle(float x, float y, float z)
{
	transfrom.Position = { x,y,z };
	isRender = false;
	CreateMaxTime = 0;
	DeleteMaxTime = 0;
	CreateTime = 0;
	DeleteTime = 0;
	mWorldBuffer = nullptr;
	StartSize = { 0,0,0 };
}

Particle::~Particle()
{
	ENGINE = nullptr;
	Device = nullptr;
	mContext = nullptr;
	mWorldBuffer = nullptr;
}

void Particle::init(hsEngine* _ENGINE)
{
	ENGINE = _ENGINE;
	Device = ENGINE->GetDeviceData()->m_device;
	mContext = ENGINE->GetDeviceData()->m_deviceContext;
	Rendering::init(_ENGINE);
	
	//인덱스버퍼와 버텍스버퍼생성
	vector<Light32> vData;
	vector<UINT> uData;
	vData.resize(4);
	uData.resize(6);
	float size = 0.5f;
	vData[0] = { Vector4(-size,size, 0,1),	Vector2(0,0) };
	vData[1] = { Vector4(size,size, 0,1),	Vector2(1,0) };
	vData[2] = { Vector4(-size,-size, 0,1),	Vector2(0,1) };
	vData[3] = { Vector4(size,-size, 0,1),	Vector2(1,1) };
	
	uData[0] = 0;
	uData[1] = 1;
	uData[2] = 2;
	uData[3] = 1;
	uData[4] = 3;
	uData[5] = 2;
	
	Create_IB_VB_Buffer(vData, uData);
	mWorldBuffer = GetBuffer("Particlebuffer");
}

void Particle::Update()
{
	if (isStart == true)
	{
		ParticleUpdate();
	}
	

	mWorld = transfrom.GetMaster();
}

void Particle::Render(const hsViewData& view)
{
	Set_ParticleBuffer_Parameter(mWorldBuffer, transfrom.WorldTM * mWorld,XMFLOAT3(1,1,1)  ,XMFLOAT4(Color.x,Color.y,Color.z,1));

	Rendering::MeshRendering();
}

void Particle::ParticleUpdate()
{
	float DTime = ENGINE->GetDeltaTime();
	DeleteTime += DTime;

	float x = DTime * Speed * Local_Look.x;
	float y = DTime * Speed * Local_Look.y;
	float z = DTime * Speed * -Local_Look.z;

	transfrom.SetTranlate(x,y,z);
	transfrom.SetScale(-DTime/2, -DTime/2, -DTime/2);
	Color.x -= DTime;
	Color.y -= DTime;
	Color.z -= DTime;

	

	//죽을 타이밍이면
	if (DeleteTime >= DeleteMaxTime)
	{
		//값초기화
		Setting();
		isStart = false;
		DeleteTime -= DeleteMaxTime;
	}
}

void Particle::Setting()
{
	transfrom.Position	= StartPos;
	transfrom.Scale		= StartSize;
	transfrom.Rotation	= StartRot;
}








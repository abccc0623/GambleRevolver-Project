#include "EnginePCH.h"
#include "ParticleSystem.h"


ParticleSystem::ParticleSystem(float x, float y, float z)
{
	transfrom.Position = { x,y,z };

	CreateTime= CreateMaxTime;
	CreateMaxTime = 1;

	DeleteTime = 0;
	DeleteMaxTime = 1;

	ParticleCount = 1;
	isPlay = false;
	Local_Up = { 0,0,0 };
	Local_Look = { 0,0,0 };
	Local_Right = { 0,0,0 };
	srand((unsigned int)time(nullptr));
}

ParticleSystem::~ParticleSystem()
{

}

void ParticleSystem::init(hsEngine* _ENGINE)
{
	ENGINE = _ENGINE;
	Debugging::init(_ENGINE);
	Debugging::Create_Line_Buffers(XMFLOAT3(0, 0.1f, 0), XMFLOAT3(0, 0.1f, -1), XMFLOAT4(1, 0, 0, 1));
	Debugging::Create_Line_Buffers(XMFLOAT3(0, 0.1f, 0), XMFLOAT3(0, 1, 0), XMFLOAT4(0, 1, 0, 1));
	Debugging::Create_Line_Buffers(XMFLOAT3(0, 0.1f, 0), XMFLOAT3(1, 0.1f, 0), XMFLOAT4(0, 0, 1, 1));
	Debugging::CreateDebugBuffers();
}

void ParticleSystem::Update()
{
	if (isPlay == true)
	{
		ParticleEndTime += ENGINE->GetDeltaTime();
		if (ParticleEndTime >= ParticleEndMaxTime)
		{
			isPlay = false;
		}


		//파티클시스템 로컬좌표 업데이트
		UpDateLocal();

		//파티클 시스템 시작
		PlayParticle(CreateMaxTime, DeleteMaxTime);
	}
	else
	{
		ParticleEndTime = 0;
	}

	//파티클업데이트
	UpDateParticle();

	//나의 월드좌표 업데이트
	mWorld = transfrom.GetMaster();
}

void ParticleSystem::Render(const hsViewData& view)
{
	vector<Particle*>::iterator it = ParticleList.begin();
	for (it; it != ParticleList.end(); it++)
	{
		if ((*it) == nullptr) { continue; }
		if ((*it)->isStart == false) { continue; }


		(*it)->Render(hsViewData());
	}
}

void ParticleSystem::Delete()
{
	isDelete = true;
}

void ParticleSystem::Debug()
{
	Debugging::Update(mWorld);
}

void ParticleSystem::SetTexture(ID3D11ShaderResourceView* Texture)
{
	Rendering::mMainSRV = Texture;
}

void ParticleSystem::CreateParticle(int count)
{

	for (int i = 0; i < count; i++)
	{
		Particle* obj = new Particle(transfrom.Position.x, transfrom.Position.y, transfrom.Position.z);
		ShaderData data;
		data.Layout = Layout;
		data.PixelShader = PixelShader;
		data.VertexShader = VertexShader;
		data.cBufferList = cBufferList;
		data.SamplerList = SamplerList;

		obj->SetShaderData(data);
		obj->SetTextures(mMainSRV);
		obj->init(ENGINE);
		obj->isStart = true;
		ParticleList.push_back(obj);
	}
}



XMMATRIX ParticleSystem::CamLookAt()
{
	//카메라
	hsCamera* cam = CamManager::GetMainCam();
	XMFLOAT3 CamRot = cam->transfrom.Rotation;

	XMMATRIX CamYMX = transfrom.Create_RotY(CamRot.y);
	XMMATRIX CamXMX = transfrom.Create_RotX(CamRot.x);
	XMMATRIX CamZMX = transfrom.Create_RotZ(CamRot.z);
	XMMATRIX ROT = CamXMX * CamYMX * CamZMX;

	return ROT;
}



void ParticleSystem::PlayParticle(float mCreateTime, float mDeleteTime)
{
	CreateTime += ENGINE->GetDeltaTime();

	//생성주기가 되었다면
	if (CreateTime >= mCreateTime)
	{
		//파티클을 개수만큼 찾아서 재생시킨다
		FindParticle(ParticleCount);

		//생성주기를 처음으로
		CreateTime = 0;
	}
}

void ParticleSystem::UpDateParticle()
{
	XMMATRIX CamYMX = CamLookAt();
	vector<Particle*>::iterator it = ParticleList.begin();
	for (it; it != ParticleList.end(); it++)
	{
		if ((*it) == nullptr) { continue; }
		if ((*it)->isStart == false) { continue; }

		(*it)->transfrom.WorldTM = CamYMX;
		(*it)->Update();
	}
}

void ParticleSystem::UpDateLocal()
{
	XMMATRIX Master = transfrom.GetMaster();

	XMMATRIX LocalPos = DirectX::XMMatrixInverse(nullptr, Master);
	XMFLOAT4X4 A_Master4x4;
	XMStoreFloat4x4(&A_Master4x4, LocalPos);

	///로컬좌표를 구한 값의 up Look right 를 각각 넣어줌
	Local_Right.x = A_Master4x4._11;
	Local_Right.y = A_Master4x4._12;
	Local_Right.z = A_Master4x4._13;

	Local_Up.x = A_Master4x4._21;
	Local_Up.y = A_Master4x4._22;
	Local_Up.z = A_Master4x4._23;

	Local_Look.x = A_Master4x4._31;
	Local_Look.y = A_Master4x4._32;
	Local_Look.z = A_Master4x4._33;
}

void ParticleSystem::SetSpeed(Particle_float mSpeed)
{
	Speed = mSpeed;
}

void ParticleSystem::SetParticleTime(float mCreate, float mDelete)
{
	CreateMaxTime = mCreate;
	DeleteMaxTime = mDelete;

	CreateTime = CreateMaxTime;
}

void ParticleSystem::SetStartPos(Particle_float3 pos)
{
	StartPos = pos;
}

void ParticleSystem::SetStartScl(Particle_float3 scl)
{
	StartScl = scl;
}

void ParticleSystem::SetStartRot(Particle_float3 rot)
{
	rot.DataX.Data += transfrom.Rotation.x;
	rot.DataY.Data += transfrom.Rotation.y;
	rot.DataZ.Data += transfrom.Rotation.z;

	StartRot = rot;
}

void ParticleSystem::SetStartColor(Particle_float3 color)
{
	StartColor = color;
}

void ParticleSystem::SetEndColor(Particle_float3 color)
{
	EndColor = color;
}

void ParticleSystem::SetDirection(float x, float y, float z)
{
	direction = { x,y,z };
}

void ParticleSystem::SetParticleCount(int count)
{
	ParticleCount = count;
}

void ParticleSystem::SetParicleEndTime(float time)
{
	ParticleEndMaxTime = time;
}

void ParticleSystem::Play()
{
	isPlay = true;
}

void ParticleSystem::Stop()
{
	isPlay = false;
}

Particle* ParticleSystem::FindParticle(int count)
{
	Particle* temp = nullptr;
	vector<Particle*>::iterator it = ParticleList.begin();
	for (it; it != ParticleList.end(); it++)
	{
		if ((*it) == nullptr) { continue; }
		if ((*it)->isStart == false)
		{
			temp = (*it);
			temp->CreateMaxTime = CreateMaxTime;
			temp->DeleteMaxTime = DeleteMaxTime;
			temp->StartPos = transfrom.Position + StartPos.GetData();
			temp->StartSize = StartScl.GetData();
			temp->StartRot = StartRot.GetData();
			temp->Color = StartColor.GetData();
			temp->direction = direction;
			temp->Speed = Speed.GetData();
			temp->EndSize = EndSize;


			//파티클 로컬좌표 업데이트
			temp->Local_Look = Local_Look;
			temp->Local_Right = Local_Right;
			temp->Local_Up = Local_Up;

			temp->Setting();
			temp->isStart = true;
			count--;
			if (count <= 0)
			{
				break;
			}


		}
	}

	//만약 파티클이 모자를때
	if (count > 0)
	{
		//없다면 만들어서 줌
		CreateParticle(count);
	}


	return temp;
}



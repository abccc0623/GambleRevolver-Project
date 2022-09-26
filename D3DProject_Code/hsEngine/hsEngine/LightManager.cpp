#include "EnginePCH.h"
#include "LightManager.h"


LightManager::LightManager(float x, float y, float z)
{
	transfrom.Position = { x,y,z };

	Mat.Ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	Mat.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	Mat.Specular = XMFLOAT4(0.6f, 0.6f, 0.6f, 16.0f);
	Mat.Reflect = XMFLOAT4(1, 1, 1, 1);

	Ambient		= XMFLOAT4(1, 1, 1, 1.0f);
	Diffuse		= XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	Specular	= XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	Att			= XMFLOAT3(0.1f, 0.5f, 1);
	Range		= 1;
	Position	= transfrom.Position;

	//디렉션 라이트
	//DirLight.Ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	//DirLight.Diffuse = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	//DirLight.Direction = XMFLOAT3(0.57735f, -0.57735f, 0.57735f);
	//DirLight.Pad;
	//DirLight.Specular = XMFLOAT4(0.1f, 0.1f, 0.1f,1.0f);
}

LightManager::~LightManager()
{
		
}

void LightManager::init(hsEngine* _ENGINE, Factory* factory)
{
	ENGINE		= _ENGINE;
	Device		= ENGINE->GetDeviceData()->m_device;
	mContext	= ENGINE->GetDeviceData()->m_deviceContext;
	mFactory	= factory;
	SetShaderData(mFactory->GetShader("Light"));

	Rendering::init(_ENGINE);
	vector<Light32> vData;
	vector<UINT> uData;
	vData.resize(4);
	uData.resize(6);
	int TexSize = 1;
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

	Rendering::Create_IB_VB_Buffer(vData, uData);

	PointLightbuffer	= GetBuffer("PointLightBuffer");
	objectbuffer		= GetBuffer("ObjectBuffer");
	Dirbuffer			= GetBuffer("DirLightBuffer");
}
void LightManager::PushLightObject(LightObject* obj)
{
	int m_type = obj->GetType();
	switch (m_type)
	{
	case 0:
		DirLight = obj;
		break;
	case 1:
		PointLightList.push_back(obj);
		break;
	case 2:
		break;
	}

}
void LightManager::Update()
{
	mWorld = transfrom.GetMaster();

	for (int i = 0; i <PointLightList.size(); i++)
	{
		if (PointLightList[i] == nullptr) { continue; }

		PointLightList[i]->Update();
	}

	if (DirLight != nullptr)
	{
		DirLight->Update();
	}
}

void LightManager::Render(const hsViewData& view)
{
	//리스트에 들어있는 포인트라이트 값 전달
	hsPointLightBuffer point;
	for (int i = 0; i < PointLightList.size(); i++)
	{
		point.pointLight[i]				=  *(PointLightList[i]->GetPointLight());
		point.pointLight[i].Position	=    PointLightList[i]->transfrom.Position;
		point.pointLight[i].pad = 1;
		point.pointLight[i].Att = { 0,0.5f, 1.0f};
		
	}
	//포인트 라이트 사이즈 지정
	point.PointLightSize = PointLightList.size();
	mContext->UpdateSubresource(PointLightbuffer, 0, nullptr, &point, 0, 0);
	mContext->VSSetConstantBuffers(4, 1, &PointLightbuffer);
	mContext->PSSetConstantBuffers(4, 1, &PointLightbuffer);



	if (DirLight != nullptr)
	{
		DirLightBuffer dir;
		dir.DirLight = *DirLight->GetDirLight();
		dir.DirLight.Pad = 1;
		dir.Mat.Ambient = Mat.Ambient;
		dir.Mat.Diffuse = Mat.Diffuse;
		dir.Mat.Reflect = Mat.Reflect;
		dir.Mat.Specular = Mat.Specular;


		mContext->UpdateSubresource(Dirbuffer, 0, nullptr, &dir, 0, 0);
		mContext->VSSetConstantBuffers(3, 1, &Dirbuffer);
		mContext->PSSetConstantBuffers(3, 1, &Dirbuffer);
	}
	
	Rendering::Set_ObjectBuffer_Parameter(objectbuffer, XMMatrixIdentity(), XMFLOAT3(1, 1, 1));
	Rendering::MeshRendering();
}

void LightManager::Delete()
{
	for (int i = 0; i < PointLightList.size(); i++)
	{
		if (PointLightList[i] == nullptr) { continue;; }

		delete PointLightList[i];
	}

	PointLightList.clear();
}

void LightManager::Debug()
{
	if (ENGINE->OnDebug == false) { return; }

	for (int i = 0; i < PointLightList.size(); i++)
	{
		if (PointLightList[i] == nullptr) { continue;; }

		PointLightList[i]->Debug();
	}

	if (DirLight != nullptr)
	{
		DirLight->Debug();
	}
}






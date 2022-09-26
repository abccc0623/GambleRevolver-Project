#include "EnginePCH.h"
#include "LightObject.h"

void LightObject::init(hsEngine* _ENGINE,int type)
{
	ENGINE = _ENGINE;
	mContext = ENGINE->GetDeviceData()->m_deviceContext;
	Device = ENGINE->GetDeviceData()->m_device;

	Debugging::init(_ENGINE);
	LightType = type;
	switch (LightType)
	{
	case 0:
		Debugging::Create_Circle_Buffers(XMFLOAT3(0, 0, 0), 1, PointLight.Diffuse);
		Create_Line_Buffers(XMFLOAT3(0, -0.25f, 0), XMFLOAT3(5, -0.25f, 0));
		Create_Line_Buffers(XMFLOAT3(0, 0, 0), XMFLOAT3(5, 0, 0));
		Create_Line_Buffers(XMFLOAT3(0, 0.25f, 0), XMFLOAT3(5, 0.25f, 0));

		DirLight.Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1);
		DirLight.Diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1);
		DirLight.Direction = XMFLOAT3(0, 0,90);
		DirLight.Pad = 1;
		DirLight.Specular = XMFLOAT4(0.1f, 0.1f, 0.1f, 1);
		break;
	case 1:
		Debugging::Create_Circle_Buffers(XMFLOAT3(0, 0, 0), PointLight.Range, PointLight.Diffuse);
		PointLight.Ambient = XMFLOAT4(1, 1, 1, 1);
		PointLight.Diffuse = XMFLOAT4(0.1f, 0.1f, 0.1f, 1);
		PointLight.Specular = XMFLOAT4(0.1f, 0.1f, 0.1f, 1);
		PointLight.Position = transfrom.Position;
		PointLight.Att = XMFLOAT3(0.1f, 0.5f, 1);
		PointLight.Range = 1;
		PointLight.pad = 1;
		break;
	case 2:
		break;
	}

	Debugging::CreateDebugBuffers();
}

LightObject::LightObject(float x, float y, float z)
{
	transfrom.Position = { x,y,z };
	PointLight = hsPointLight();
	DirLight = DirectionalLight();
}

LightObject::~LightObject()
{

}

void LightObject::Debug()
{
	Debugging::Update(mWorld);
}

void LightObject::Update()
{
	mWorld = transfrom.GetMaster();


	if (LightType == 0)
	{
		

		XMVECTOR det = DirectX::XMMatrixDeterminant(transfrom.GetMaster());
		////Right벡터를 구하기위해 
		XMMATRIX LocalPos = DirectX::XMMatrixInverse(&det, transfrom.GetMaster());
		XMFLOAT4X4 A_Master4x4;
		XMStoreFloat4x4(&A_Master4x4, LocalPos);

		transfrom.Right_Vector.x = A_Master4x4._11;
		transfrom.Right_Vector.y = A_Master4x4._12;
		transfrom.Right_Vector.z = A_Master4x4._13;

		DirLight.Direction.x = transfrom.Right_Vector.x;
		DirLight.Direction.y = -transfrom.Right_Vector.y;
		DirLight.Direction.z = transfrom.Right_Vector.z;

		////라이트 객채 옵션 조정
		DirLight.Pad = 1;
	}

}

void LightObject::Delete()
{

}

const int LightObject::GetType()
{
	return LightType;
}

void LightObject::SetDirLight(DirectionalLight m_DirLight)
{
	DirLight = m_DirLight;
}

void LightObject::SetDirLight(XMFLOAT4 A, XMFLOAT4 D, XMFLOAT4 S)
{
	DirLight.Diffuse = D;
	DirLight.Ambient = A;
	DirLight.Specular = S;
}

void LightObject::SetDirLightColor(XMFLOAT3 color)
{
	DirLight.Diffuse = XMFLOAT4(color.x, color.y, color.z, 1.0f);
}

DirectionalLight* LightObject::GetDirLight()
{
	return &DirLight;
}

void LightObject::SetPointLight(hsPointLight Light)
{
	PointLight = Light;

	PointLight.Position = transfrom.Position;
}

void LightObject::SetPointLight(XMFLOAT4 A, XMFLOAT4 D, XMFLOAT4 S)
{
	PointLight.Ambient	= A;
	PointLight.Diffuse	= D;
	PointLight.Specular = S;

	PointLight.Position = transfrom.Position;
}

void LightObject::SetPointLightRange(float _Range)
{
	PointLight.Range = _Range;
}

void LightObject::SetPointLightColor(XMFLOAT3 color)
{
	PointLight.Diffuse = XMFLOAT4(color.x, color.y, color.z, 1.0f);
}

const hsPointLight* LightObject::GetPointLight()
{
	return &PointLight;
}


void LightObject::Render(const hsViewData& view)
{
	
}

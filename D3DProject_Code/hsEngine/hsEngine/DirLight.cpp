#include "EnginePCH.h"
#include "DirLight.h"

hsDirLight::hsDirLight()
{
	mDirLights = nullptr;
	rot = 0;
}

hsDirLight::hsDirLight(float x, float y, float z)
{
	mDirLights = nullptr;
	transfrom.Position = { x,y,z };

	Mat.Ambient = XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
	Mat.Diffuse = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	Mat.Specular = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	Mat.Reflect = XMFLOAT4(0.5f, 0.5f, 0.5f, 0.5f);

	mDirLights = new DirectionalLight();
	Ambient		= XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	Diffuse		= XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	Specular	= XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	Direction	= XMFLOAT3(0.57735f, -0.57735f, 0.57735f);
}

void hsDirLight::init(hsEngine* _ENGINE)
{
	ENGINE		= _ENGINE;
	Device		= ENGINE->GetDeviceData()->m_device;
	mContext	= ENGINE->GetDeviceData()->m_deviceContext;

	Debugging::init(_ENGINE);
	Rendering::init(_ENGINE);
	
	//디버깅 관련 인덱스버퍼와 버텍스 버퍼 새성
	Create_Circle_Buffers(XMFLOAT3(0, 0, 0), 2, XMFLOAT4(1, 1, 0, 1));
	Create_Line_Buffers(XMFLOAT3(0, -0.25f, 0),	XMFLOAT3(5, -0.25f, 0));
	Create_Line_Buffers(XMFLOAT3(0, 0, 0),		XMFLOAT3(5, 0, 0));
	Create_Line_Buffers(XMFLOAT3(0, 0.25f, 0),	XMFLOAT3(5, 0.25f, 0));
	CreateDebugBuffers();

	Dirbuffer = GetBuffer("DirLightBuffer");
}

hsDirLight::~hsDirLight()
{

}

void hsDirLight::Update()
{
	transfrom.SetRotation(0, 0, rot* ENGINE->GetDeltaTime());
	mWorld	= transfrom.GetMaster();
}

void hsDirLight::Render(const hsViewData& view)
{
	//랜더링 정보
	
	//행렬을 벡터로 변경시켜줌
	XMVECTOR det = DirectX::XMMatrixDeterminant(transfrom.GetMaster());
	////Right벡터를 구하기위해 
	XMMATRIX LocalPos = DirectX::XMMatrixInverse(&det, transfrom.GetMaster());
	XMFLOAT4X4 A_Master4x4;
	XMStoreFloat4x4(&A_Master4x4, LocalPos);
	
	transfrom.Right_Vector.x = A_Master4x4._11;
	transfrom.Right_Vector.y = A_Master4x4._12;
	transfrom.Right_Vector.z = A_Master4x4._13;
	
	Direction.x = transfrom.Right_Vector.x;
	Direction.y = -transfrom.Right_Vector.y;
	Direction.z = transfrom.Right_Vector.z;
	
	////라이트 객채 옵션 조정
	mDirLights->Pad = 1;
	
	//Light 버퍼를 업데이트 해줌
	DirLightBuffer dir;
	dir.DirLight.Ambient	= Ambient;
	dir.DirLight.Diffuse	= Diffuse;
	dir.DirLight.Direction	= Direction;
	dir.DirLight.Specular	= Specular;
	dir.DirLight.Pad = 1;
	

	dir.Mat.Ambient = Mat.Ambient;
	dir.Mat.Diffuse = Mat.Diffuse;
	dir.Mat.Reflect = Mat.Reflect;
	dir.Mat.Specular = Mat.Specular;
	

	//dir.Pos = transfrom.Position;
	//dir.Light_Mask = 0x00000100;
	//dir.Pos = Direction;

	//리소스를 복사
	mContext->UpdateSubresource(Dirbuffer, 0, nullptr, &dir, 0, 0);
	
	mContext->VSSetConstantBuffers(3, 1, &Dirbuffer);
	mContext->PSSetConstantBuffers(3, 1, &Dirbuffer);
}

void hsDirLight::Delete()
{

}

void hsDirLight::Debug()
{
	Debugging::Update(mWorld);
}


void hsDirLight::SetColor(float R, float G, float B)
{
	Diffuse = XMFLOAT4(R, G, B, 1.0f);
}

void hsDirLight::SetRot(float _rot)
{
	rot = _rot;
}






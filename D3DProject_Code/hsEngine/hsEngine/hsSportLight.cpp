#include "EnginePCH.h"
#include "hsSportLight.h"



hsSpotLight::hsSpotLight(float x, float y, float z)
{
	transfrom.Position	= { x,y,z};
	transfrom.Scale		= { 1,1,1 };
	transfrom.Rotation	= { 0,0,0 };

	Mat.Ambient		= XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	Mat.Diffuse		= XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	Mat.Specular	= XMFLOAT4(0.6f, 0.6f, 0.6f, 16.0f);
	Mat.Reflect = XMFLOAT4(1, 1, 1, 1);

	//초기값 설정
	mSportLight = SpotLight();
	mSportLight.Ambient		= XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	mSportLight.Diffuse		= XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
	mSportLight.Specular	= XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
	mSportLight.Att			= XMFLOAT3(0.1f, 0, 0);
	mSportLight.Range		= 5;
	mSportLight.Position	= transfrom.Position;
	mSportLight.Direction	= XMFLOAT3(0, 0, -1);

}

hsSpotLight::~hsSpotLight()
{
}

void hsSpotLight::init(hsEngine* _ENGINE)
{
	ENGINE = _ENGINE;
	Device = ENGINE->GetDeviceData()->m_device;
	mContext = ENGINE->GetDeviceData()->m_deviceContext;

	//debug = new DebugObj(0, 0, 0);
	//debug->init(_ENGINE);
	////debug->Create_2D_X_Circle_Buffers(XMFLOAT3(0, 0, 0), 1,XMFLOAT4(1,1,1,1));
	//debug->Create_2D_Y_Circle_Buffers(XMFLOAT3(0, 0, 1), 1,XMFLOAT4(0,0,1,1));
	//debug->Create_2D_X_Circle_Buffers(XMFLOAT3(0, 0, 0), 1,XMFLOAT4(0,0,1,1));
	//
	//debug->Create_Line_Buffers(XMFLOAT3(0, 0, 0), XMFLOAT3(0,0,-1),XMFLOAT4(1,0,0,1));
	//
	//
	//debug->Create_Line_Buffers(XMFLOAT3(0, 0, 0), XMFLOAT3(1,0,-1),XMFLOAT4(1,0,0,1));
	//debug->Create_Line_Buffers(XMFLOAT3(0, 0, 0), XMFLOAT3(-1,0,-1),XMFLOAT4(1,0,0,1));
	//debug->Create_Line_Buffers(XMFLOAT3(0, 0, 0), XMFLOAT3(1,0,0),XMFLOAT4(1,1,0,1));
	//debug->Create_Line_Buffers(XMFLOAT3(0, 0, 0), XMFLOAT3(-1,0,0),XMFLOAT4(1,1,0,1));
	//debug->CreateBuffers();
}

void hsSpotLight::Update()
{
	transfrom.SetRotation(0,0.05f, 0);
	

	XMMATRIX xm = transfrom.WorldTM * transfrom.LocalTM * transfrom.GetMaster();

	///행렬을 벡터로 변경시켜줌
	XMVECTOR det = DirectX::XMMatrixDeterminant(xm);
	///Master의 카메라의 로컬좌표를 구하기위해 역행렬을 구함
	XMMATRIX LocalPos = DirectX::XMMatrixInverse(&det, xm);
	XMFLOAT4X4 A_Master4x4;
	XMStoreFloat4x4(&A_Master4x4, LocalPos);

	transfrom.Look_Vector.x = A_Master4x4._31;
	transfrom.Look_Vector.y = A_Master4x4._32;
	transfrom.Look_Vector.z = A_Master4x4._33;


	mSportLight.Direction.x = transfrom.Look_Vector.x;
	mSportLight.Direction.y = transfrom.Look_Vector.y;
	mSportLight.Direction.z = -transfrom.Look_Vector.z;

	//mWorld = view.world;
	//mView = view.view;
	//mProj = view.proj;
	//debug->Update(view);
}

void hsSpotLight::Render(const hsViewData& view)
{
	
	//라이트 객채 옵션 조정
	mSportLight.Ambient	= XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	//컬러
	
	//반사
	mSportLight.Specular	= XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	mSportLight.Att		= XMFLOAT3(0.1f,0, 0);
	
	
	//mSportLight->Direction  = XMFLOAT3(0, 0,-1);
	mSportLight.Spot		= 1;
	mSportLight.Pad			= 100;

	//라이트의 위치 조정
	mSportLight.Position = transfrom.Position;
	//라이트 범위 조정
	//float RangeSize = mSportLight.Range;
	

	//라이트에 범위에 따라 스케일 조정
	transfrom.Scale = { mSportLight.Range , mSportLight.Range , mSportLight.Range };


	mContext->RSSetState(m_pRenderstate);

	//라이트 자체에서는 그려지는건 없음
	Effects::BasicFX->SetMaterial(Mat);
	Effects::BasicFX->SetSpotLights(&mSportLight);

	//스키닝 객체에 쉐이더에 정보를 보내줌
	Effects::SkinningFX->SetSpotLights(&mSportLight);
	Effects::SkinningFX->SetMaterial(Mat);

	
}

void hsSpotLight::Delete()
{
	
}

void hsSpotLight::Debug()
{
	
}


void hsSpotLight::SetColor(float R, float G, float B)
{
	//컬러는 0 ~ 1 값 만들어온다
	//나머지는 밝기값으로 곱해질것
	mSportLight.Diffuse = XMFLOAT4(R, G, B, 1.0f);
}

void hsSpotLight::SetRange(float _Range)
{
	mSportLight.Range = _Range;
}





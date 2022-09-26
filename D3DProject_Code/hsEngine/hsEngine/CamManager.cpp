#include "EnginePCH.h"
#include "CamManager.h"


hsCamera* CamManager::MainCam = nullptr;
CamManager::CamManager()
{
	ENGINE = nullptr;
	FACTORY = nullptr;
	CameraCount = 0;
}

CamManager::~CamManager()
{
	ENGINE = nullptr;
	FACTORY = nullptr;
}

void CamManager::inputCam(string name,hsCamera* cam)
{
	//카메라를 map에 넣는다
	CamList.insert(pair<string, hsCamera*>(name, cam));
	CameraCount++;
}

hsCamera* CamManager::GetCam(string name)
{
	//카메라를 꺼내옴
	return CamList[name];
}

hsCamera* CamManager::GetMainCam()
{
	return MainCam;
}

XMFLOAT3 CamManager::GetMouseClickWorldPoint(float Range)
{
	XMMATRIX projectionMatrix;	//프로젝션 
	XMMATRIX viewMatrix;		//뷰
	XMMATRIX inverseViewMatrix;	//뷰의 역행렬
	XMMATRIX worldMatrix;		//월드 
	XMMATRIX translateMatrix;   //
	XMMATRIX inverseWorldMatrix;

	XMFLOAT3 direction;
	XMFLOAT3 origin;
	XMFLOAT3 rayOrigin;
	XMFLOAT3 rayDirection;



	//마우스의 커서를 -1에서 +1범위로 맞춘다
	float MouseX = ENGINE->GetKey()->GetMousePos()->x;
	float MouseY = ENGINE->GetKey()->GetMousePos()->y;


	float pointX = ((2.0f * (float)MouseX) / (float)ENGINE->mClientWidth) - 1.0f;
	float pointY = (((2.0f * (float)MouseY) / (float)ENGINE->mClientHeight) - 1.0f) * -1.0f;

	projectionMatrix = MainCam->GetProj();
	
	XMFLOAT3X3 projectionMatrix4;
	XMStoreFloat3x3(&projectionMatrix4, projectionMatrix);

	pointX = pointX / projectionMatrix4._11;
	pointY = pointY / projectionMatrix4._22;


	//뷰행렬 가져옴
	viewMatrix =MainCam->GetView();
	//뷰의 역행렬을 구함
	inverseViewMatrix = XMMatrixInverse(nullptr, viewMatrix);

	//행렬을 찢음
	XMFLOAT3X3 inverseViewMatrix4;
	XMStoreFloat3x3(&inverseViewMatrix4, inverseViewMatrix);

	///여기까지가 뷰화면 좌표계

	//뷰공간에서 피킹 레이의 방향을 계산
	direction.x = (pointX * inverseViewMatrix4._11) + (pointY * inverseViewMatrix4._21) + inverseViewMatrix4._31;
	direction.y = (pointX * inverseViewMatrix4._12) + (pointY * inverseViewMatrix4._22) + inverseViewMatrix4._32;
	direction.z = (pointX * inverseViewMatrix4._13) + (pointY * inverseViewMatrix4._23) + inverseViewMatrix4._33;

	//카메라 위치인 picking ray 의 원점을 가져옴
	origin = {MainCam->transfrom.Position};

	worldMatrix = MainCam->GetWorld(); 
	translateMatrix = MainCam->GetView();
	worldMatrix = XMMatrixMultiply(worldMatrix, translateMatrix);

	//이거의 역함수
	inverseWorldMatrix = XMMatrixInverse(nullptr, worldMatrix);

	//광선 원점과 광선 방향을 뷰 공간에서 월드 공간으로 변환
	XMStoreFloat3(&rayOrigin, XMVector3TransformCoord(XMVectorSet(origin.x, origin.y, origin.z, 0.0f), inverseWorldMatrix));
	XMStoreFloat3(&direction, XMVector3TransformNormal(XMVectorSet(direction.x, direction.y, direction.z, 0.0f), inverseWorldMatrix));

	//광선 방향을 표준화
	XMStoreFloat3(&rayDirection, XMVector3Normalize(XMVectorSet(direction.x, direction.y, direction.z, 0.0f)));


	//float a = (rayDirection.x * rayDirection.x) + (rayDirection.y * rayDirection.y) + (rayDirection.z * rayDirection.z);
	//float b = ((rayDirection.x * rayOrigin.x) + (rayDirection.y * rayOrigin.y) + (rayDirection.z * rayOrigin.z)) * 2.0f;
	//float c = ((rayOrigin.x * rayOrigin.x) + (rayOrigin.y * rayOrigin.y) + (rayOrigin.z * rayOrigin.z)) - (100 * 100);
	//
	//// 결과값을 얻는다
	//float discriminant = (b * b) - (4 * a * c);
	//
	//// 결과값이 음수이면 피킹 선이 구를 벗어난 것입니다. 그렇지 않으면 구를 선택합니다.
	//if (discriminant < 0.0f)
	//{
	//	int a = 0;
	//}
	//else
	//{
	//	int b = 0;
	//}


	//클릭한 지점의 위치
	XMFLOAT3 pos = {0,0,0};

	//이거아닌것같은데...
	Range = 0;
	while (true)
	{
		//길이가 Range인 직선의 위치를구함  rayDirection 방향벡터 , rayOrigin 레이를 쏜 위치
		pos = (rayDirection * Range) + rayOrigin;

		if (pos.y <= 0)
		{
			break;
		}
		Range++;
	}

	return pos;
}

XMFLOAT3 CamManager::GetMainCamRightVector()
{
	if(MainCam == nullptr){ return XMFLOAT3(); }

	return MainCam->transfrom.Right_Vector;
}

void CamManager::init(hsEngine* _ENGINE,Factory* mFactory)
{
	ENGINE = _ENGINE;
	FACTORY = mFactory;
}

int CamManager::GetCamCount()
{
	return CameraCount;
}

void CamManager::MainCamInputKeyUpdate()
{
	if (MainCam == nullptr) { return; }
	if (ENGINE->OnDebug == false) { return; }

	//메인 캠의 키조작을 담당함
	float speed = 15.0f*ENGINE->GetDeltaTime();

	if (ENGINE->GetKey()->GetKey(VK_RIGHT))
	{
		MainCam->SetPosition(speed, 0, 0);
	}

	if (ENGINE->GetKey()->GetKey(VK_LEFT))
	{
		MainCam->SetPosition(-speed, 0, 0);
	}

	if (ENGINE->GetKey()->GetKey(VK_DOWN))
	{
		MainCam->SetPosition(0, 0, -speed);
	}

	if (ENGINE->GetKey()->GetKey(VK_UP))
	{
		MainCam->SetPosition(0, 0, speed);
	}

	
	if (ENGINE->GetKey()->GetKeyDown(VK_RBUTTON) == true)
	{
		mLastMousePosX = ENGINE->GetKey()->GetMousePos()->x;
		mLastMousePosY = ENGINE->GetKey()->GetMousePos()->y;
	}
	if (ENGINE->GetKey()->GetKeyUp(VK_RBUTTON) == true)
	{
		mLastMousePosX = ENGINE->GetKey()->GetMousePos()->x;
		mLastMousePosY = ENGINE->GetKey()->GetMousePos()->y;
	}

	float RotSpeed = 5;
	float x = ENGINE->GetKey()->GetMousePos()->x;
	float y = ENGINE->GetKey()->GetMousePos()->y;
	if (ENGINE->GetKey()->GetKey(VK_RBUTTON) == true)
	{
		// Make each pixel correspond to a quarter of a degree.
		float dx = XMConvertToRadians(0.5f * static_cast<float>(x - mLastMousePosX));
		float dy = XMConvertToRadians(0.5f * static_cast<float>(y - mLastMousePosY));

		MainCam->transfrom.SetRotation(dy * RotSpeed, dx * RotSpeed, 0);

	}
	mLastMousePosX = x;
	mLastMousePosY = y;

	if (ENGINE->GetKey()->GetKeyDown(VK_RBUTTON))
	{
		GetMouseClickWorldPoint(20);
	}


}

void CamManager::ChoiceMainCam(string CamName)
{
	if (MainCam != nullptr)
	{
		MainCam->isMain = false;
	}
	MainCam = GetCam(CamName);
	MainCam->isMain = true;

	ENGINE->OnResize(ENGINE->mClientWidth, ENGINE->mClientHeight);
}

hsCamera* CamManager::CreateCamera(string CamName, XMFLOAT3 Pos)
{
	hsCamera* temp = FACTORY->Create_Camera(Pos.x, Pos.y, Pos.z);
	CamList.insert({ CamName,temp });
	return temp;
}

hsCamera* CamManager::CreateCamera(string CamName, float x, float y, float z)
{
	hsCamera* temp = FACTORY->Create_Camera(x,y,z);
	CamList.insert({ CamName,temp });
	return temp;
}

void CamManager::SetOnResizeProj(XMMATRIX _Proj)
{
	if (MainCam == nullptr) { return; };
	MainCam->SetProj(_Proj);
	
}






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
	//ī�޶� map�� �ִ´�
	CamList.insert(pair<string, hsCamera*>(name, cam));
	CameraCount++;
}

hsCamera* CamManager::GetCam(string name)
{
	//ī�޶� ������
	return CamList[name];
}

hsCamera* CamManager::GetMainCam()
{
	return MainCam;
}

XMFLOAT3 CamManager::GetMouseClickWorldPoint(float Range)
{
	XMMATRIX projectionMatrix;	//�������� 
	XMMATRIX viewMatrix;		//��
	XMMATRIX inverseViewMatrix;	//���� �����
	XMMATRIX worldMatrix;		//���� 
	XMMATRIX translateMatrix;   //
	XMMATRIX inverseWorldMatrix;

	XMFLOAT3 direction;
	XMFLOAT3 origin;
	XMFLOAT3 rayOrigin;
	XMFLOAT3 rayDirection;



	//���콺�� Ŀ���� -1���� +1������ �����
	float MouseX = ENGINE->GetKey()->GetMousePos()->x;
	float MouseY = ENGINE->GetKey()->GetMousePos()->y;


	float pointX = ((2.0f * (float)MouseX) / (float)ENGINE->mClientWidth) - 1.0f;
	float pointY = (((2.0f * (float)MouseY) / (float)ENGINE->mClientHeight) - 1.0f) * -1.0f;

	projectionMatrix = MainCam->GetProj();
	
	XMFLOAT3X3 projectionMatrix4;
	XMStoreFloat3x3(&projectionMatrix4, projectionMatrix);

	pointX = pointX / projectionMatrix4._11;
	pointY = pointY / projectionMatrix4._22;


	//����� ������
	viewMatrix =MainCam->GetView();
	//���� ������� ����
	inverseViewMatrix = XMMatrixInverse(nullptr, viewMatrix);

	//����� ����
	XMFLOAT3X3 inverseViewMatrix4;
	XMStoreFloat3x3(&inverseViewMatrix4, inverseViewMatrix);

	///��������� ��ȭ�� ��ǥ��

	//��������� ��ŷ ������ ������ ���
	direction.x = (pointX * inverseViewMatrix4._11) + (pointY * inverseViewMatrix4._21) + inverseViewMatrix4._31;
	direction.y = (pointX * inverseViewMatrix4._12) + (pointY * inverseViewMatrix4._22) + inverseViewMatrix4._32;
	direction.z = (pointX * inverseViewMatrix4._13) + (pointY * inverseViewMatrix4._23) + inverseViewMatrix4._33;

	//ī�޶� ��ġ�� picking ray �� ������ ������
	origin = {MainCam->transfrom.Position};

	worldMatrix = MainCam->GetWorld(); 
	translateMatrix = MainCam->GetView();
	worldMatrix = XMMatrixMultiply(worldMatrix, translateMatrix);

	//�̰��� ���Լ�
	inverseWorldMatrix = XMMatrixInverse(nullptr, worldMatrix);

	//���� ������ ���� ������ �� �������� ���� �������� ��ȯ
	XMStoreFloat3(&rayOrigin, XMVector3TransformCoord(XMVectorSet(origin.x, origin.y, origin.z, 0.0f), inverseWorldMatrix));
	XMStoreFloat3(&direction, XMVector3TransformNormal(XMVectorSet(direction.x, direction.y, direction.z, 0.0f), inverseWorldMatrix));

	//���� ������ ǥ��ȭ
	XMStoreFloat3(&rayDirection, XMVector3Normalize(XMVectorSet(direction.x, direction.y, direction.z, 0.0f)));


	//float a = (rayDirection.x * rayDirection.x) + (rayDirection.y * rayDirection.y) + (rayDirection.z * rayDirection.z);
	//float b = ((rayDirection.x * rayOrigin.x) + (rayDirection.y * rayOrigin.y) + (rayDirection.z * rayOrigin.z)) * 2.0f;
	//float c = ((rayOrigin.x * rayOrigin.x) + (rayOrigin.y * rayOrigin.y) + (rayOrigin.z * rayOrigin.z)) - (100 * 100);
	//
	//// ������� ��´�
	//float discriminant = (b * b) - (4 * a * c);
	//
	//// ������� �����̸� ��ŷ ���� ���� ��� ���Դϴ�. �׷��� ������ ���� �����մϴ�.
	//if (discriminant < 0.0f)
	//{
	//	int a = 0;
	//}
	//else
	//{
	//	int b = 0;
	//}


	//Ŭ���� ������ ��ġ
	XMFLOAT3 pos = {0,0,0};

	//�̰žƴѰͰ�����...
	Range = 0;
	while (true)
	{
		//���̰� Range�� ������ ��ġ������  rayDirection ���⺤�� , rayOrigin ���̸� �� ��ġ
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

	//���� ķ�� Ű������ �����
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






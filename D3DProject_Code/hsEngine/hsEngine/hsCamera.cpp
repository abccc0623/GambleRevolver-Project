#include "EnginePCH.h"
#include "hsCamera.h"
hsCamera::hsCamera(float X, float Y, float Z)
{
 	transfrom.Position = XMFLOAT3(X,Y,Z);
	transfrom.Scale = XMFLOAT3(1,1,1);
	transfrom.Rotation = XMFLOAT3(0, 0, 0);
	Speed = 1;
	SetLens(0.5f * MathHelper::Pi, 1, 0.1f, 4000.0f);

	isMain = false;
}

hsCamera::~hsCamera()
{
	//delete debug;
}


void hsCamera::SetLens(float fovY, float aspect, float zn, float zf)
{
	// cache properties
	mFovY = fovY;
	mAspect = aspect;
	mNearZ = zn;
	mFarZ = zf;


	mNearWindowHeight = 2.0f * mNearZ * tanf(0.5f * mFovY);
	mFarWindowHeight = 2.0f * mFarZ * tanf(0.5f * mFovY);
	
	XMMATRIX P = XMMatrixPerspectiveFovLH(mFovY, mAspect, mNearZ, mFarZ);
	//XMMATRIX P = XMMatrixOrthographicLH(mFovY, mAspect, mNearZ, mFarZ);

	
	XMStoreFloat4x4(&mProj, P);
}

void hsCamera::SetLens()
{
	//mFovY = fovY;
	//mAspect = aspect;
	//mNearZ = zn;
	//mFarZ = zf;


	mNearWindowHeight = 2.0f * mNearZ * tanf(0.5f * mFovY);
	mFarWindowHeight = 2.0f * mFarZ * tanf(0.5f * mFovY);

	//XMMATRIX P = XMMatrixPerspectiveFovLH(mFovY, mAspect, mNearZ, mFarZ);
	XMMATRIX P = XMMatrixOrthographicLH(mFovY, mAspect, mNearZ, mFarZ);
	XMStoreFloat4x4(&mProj, P);
}

XMMATRIX hsCamera::GetView()
{
	return XMLoadFloat4x4(&mView);
}

XMMATRIX hsCamera::GetProj()
{
	return XMLoadFloat4x4(&mProj);
}

XMMATRIX hsCamera::GetWorld()
{
	return mWorld;
}


void hsCamera::SetRotation(float AngleX, float AngleY, float AngleZ)
{
	transfrom.Rotation = XMFLOAT3(AngleX,AngleY,AngleZ);
}

void hsCamera::init(hsEngine* _ENGINE)
{
	ENGINE = _ENGINE;
	Device = ENGINE->GetDeviceData()->m_device;
	mContext = ENGINE->GetDeviceData()->m_deviceContext;

	Debugging::init(_ENGINE);
	Rendering::init(_ENGINE);

	
	Debugging::Create_Box_Buffers(XMFLOAT3(0, 0, 0), XMFLOAT3(0.25f, 0.25f, 0.5f), XMFLOAT4(0.5f, 0.5f,1 , 1));
	Debugging::Create_2D_X_Circle_Buffers(XMFLOAT3(0, 0, 10), 5, XMFLOAT4(1, 0, 0, 1));
	Debugging::Create_2D_X_Circle_Buffers(XMFLOAT3(0, 0, 5), 2.5f, XMFLOAT4(1, 0, 0, 1));
	Debugging::Create_Line_Buffers(XMFLOAT3(0, 0, 0.5f), XMFLOAT3(0, 5, 10), XMFLOAT4(1, 0, 0, 1));
	Debugging::Create_Line_Buffers(XMFLOAT3(0, 0, 0.5f), XMFLOAT3(0, -5, 10), XMFLOAT4(1, 0, 0, 1));
	Debugging::Create_Line_Buffers(XMFLOAT3(0, 0, 0.5f), XMFLOAT3(5, 0, 10), XMFLOAT4(1, 0, 0, 1));
	Debugging::Create_Line_Buffers(XMFLOAT3(0, 0, 0.5f), XMFLOAT3(-5, 0, 10), XMFLOAT4(1, 0, 0, 1));
	Debugging::Create_Line_Buffers(XMFLOAT3(0, 0, 0.5f), XMFLOAT3(0, 0, 100), XMFLOAT4(0, 0, 1, 1));
	Debugging::CreateDebugBuffers();

	m_GlobalBuffer = nullptr;
	m_GlobalBuffer = GetBuffer("CameraBuffer");
}


void hsCamera::Render(const hsViewData& view)
{
	XMFLOAT3 eyePos = transfrom.Position;
	XMMATRIX T = XMMatrixTranslation(eyePos.x, eyePos.y, eyePos.z);
	XMMATRIX vew= XMMatrixMultiply(T,GetView());
	XMMATRIX WVP = XMMatrixMultiply(vew, GetProj());

	//Effects::SkinningFX->SetEyePosW(eyePos);


	///SRT ��İ�����
	XMMATRIX Master = transfrom.GetMaster();
	//����� ���ͷ� ���������
	XMVECTOR det = DirectX::XMMatrixDeterminant(Master);
	/////Master�� ī�޶��� ������ǥ�� ���ϱ����� ������� ����
	LocalPos = DirectX::XMMatrixInverse(&det, Master);
	XMFLOAT4X4 A_Master4x4;
	XMStoreFloat4x4(&A_Master4x4, LocalPos);

	///������ǥ�� ���� ���� up Look right �� ���� �־���
	transfrom.Right_Vector.x = A_Master4x4._11;
	transfrom.Right_Vector.y = A_Master4x4._12;
	transfrom.Right_Vector.z = A_Master4x4._13;

	transfrom.UP_Vector.x = A_Master4x4._21;
	transfrom.UP_Vector.y = A_Master4x4._22;
	transfrom.UP_Vector.z = A_Master4x4._23;

	transfrom.Look_Vector.x = A_Master4x4._31;
	transfrom.Look_Vector.y = A_Master4x4._32;
	transfrom.Look_Vector.z = A_Master4x4._33;


	///������ ���� ���͵��� ����Ҽ� �ֵ��� ��ȯ
	XMVECTOR R = XMLoadFloat3(&transfrom.Right_Vector);
	XMVECTOR U = XMLoadFloat3(&transfrom.UP_Vector);
	XMVECTOR L = XMLoadFloat3(&transfrom.Look_Vector);
	XMVECTOR P = XMLoadFloat3(&transfrom.Position);


	float x = XMVectorGetX(XMVector3Dot(P, R));
	float y = XMVectorGetX(XMVector3Dot(P, U));
	float z = XMVectorGetX(XMVector3Dot(P, L));


	mView._11 = transfrom.Right_Vector.x;	mView._12 = transfrom.UP_Vector.x;			mView._13 = transfrom.Look_Vector.x; mView._14 = 0;
	mView._21 = transfrom.Right_Vector.y;	mView._22 = transfrom.UP_Vector.y;			mView._23 = transfrom.Look_Vector.y; mView._24 = 0;
	mView._31 = transfrom.Right_Vector.z;	mView._32 = transfrom.UP_Vector.z;			mView._33 = transfrom.Look_Vector.z; mView._34 = 0;
	mView._41 = -x;							mView._42 = -y;								mView._43 = -z;						 mView._44 = 1;

	XMMATRIX p = transfrom.SetPosition4X4(transfrom.Position);
	XMFLOAT3 rot = { transfrom.Rotation.x, transfrom.Rotation.y, transfrom.Rotation.z };
	XMMATRIX r = transfrom.EulerToQuaternion(rot);
	XMMATRIX s = transfrom.SetScale4X4(transfrom.Scale);

	if (isMain == true)
	{
		//ī�޶� ���۸� ������Ʈ����
		Rendering::Set_CameraBuffer_Parameter(m_GlobalBuffer, transfrom.Position, GetView(), GetProj());
	}
}

XMFLOAT3 hsCamera::GetClickPoint()
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

	projectionMatrix = GetProj();

	XMFLOAT3X3 projectionMatrix4;
	XMStoreFloat3x3(&projectionMatrix4, projectionMatrix);

	pointX = pointX / projectionMatrix4._11;
	pointY = pointY / projectionMatrix4._22;


	//����� ������
	viewMatrix = GetView();
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
	origin = { transfrom.Position };

	worldMatrix = mWorld;
	translateMatrix = GetView();
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
	XMFLOAT3 pos = { 0,0,0 };
	int Range = 10;

	while (true)
	{
		pos = (rayDirection * Range) + rayOrigin;
		if (pos.y <= 0.5f)
		{
			break;
		}

		//�ʹ� �־�����
		if (Range >= 100)
		{
			pos = (rayDirection * Range) + rayOrigin;
			break;
		}
		Range++;
	}
	//�װ����� �����Ŀ� ����

	//�װ��� ����.
	return pos;
}


void hsCamera::SetPosition(float X, float Y, float Z)
{
	transfrom.Position =
	{
		transfrom.Position.x + (transfrom.Right_Vector.x * X),
		transfrom.Position.y + (transfrom.Right_Vector.y * X),
		transfrom.Position.z + (transfrom.Right_Vector.z * X)
	};

	transfrom.Position =
	{
		transfrom.Position.x + (transfrom.UP_Vector.x * Y),
		transfrom.Position.y + (transfrom.UP_Vector.y * Y),
		transfrom.Position.z + (transfrom.UP_Vector.z * Y)
	};


	transfrom.Position =
	{
		transfrom.Position.x + (transfrom.Look_Vector.x * Z),
		transfrom.Position.y + (transfrom.Look_Vector.y * Z),
		transfrom.Position.z + (transfrom.Look_Vector.z * Z)
	};
}

void hsCamera::SetProj(XMMATRIX _Proj)
{
	XMStoreFloat4x4(&mProj, _Proj);
}

void hsCamera::Update()
{
	XMMATRIX p = transfrom.SetPosition4X4(transfrom.Position);
	XMFLOAT3 rot = { transfrom.Rotation.x, transfrom.Rotation.y, transfrom.Rotation.z };
	XMMATRIX r = transfrom.EulerToQuaternion(rot);
	XMMATRIX s = transfrom.SetScale4X4(transfrom.Scale);

	mWorld = s * r * p;
}

void hsCamera::Delete()
{

}

void hsCamera::Debug()
{
	if (isMain == false)
	{
		Debugging::Update(mWorld);
	}
}

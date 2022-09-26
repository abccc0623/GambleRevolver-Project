#include "EnginePCH.h"

Transform::Transform()
{
	LocalPosition = { 0,0,0 };

	Position = {0,0,0};
	Scale = { 1,1,1 };
	Rotation = { 0,0,0};
	Master = XMMATRIX();


	UP_Vector	= {0,0,0};
	Look_Vector	= { 0,0,0 };
	Right_Vector	= { 0,0,0 };
	LocalPosition	= { 0,0,0 };
	LocalScale = { 1,1,1 };
	LocalRotation = { 0,0,0 };

	ParentTM = XMMatrixIdentity();
	
	Master = XMMatrixIdentity();

	V_TM = XMMatrixIdentity();
	W_TM = XMMatrixIdentity();

	WorldTM = XMMatrixIdentity();
	LocalTM = XMMatrixIdentity();
	ParentTR = nullptr;

	isHierarchy = false;
}

Transform::~Transform()
{
	
}

void Transform::SetTranlate(float X, float Y, float Z)
{
	Position.x += X;
	Position.y += Y;
	Position.z += Z;
}

void Transform::SetPosition(float X, float Y, float Z)
{
	Position.x = X;
	Position.y = Y;
	Position.z = Z;
}

void Transform::SetPosition(XMFLOAT3 pos)
{
	Position.x += pos.x;
	Position.y += pos.y;
	Position.z += pos.z;
}

void Transform::SetLocalPosition(float X, float Y, float Z)
{
	LocalPosition.x = X;
	LocalPosition.y = Y;
	LocalPosition.z = Z;

	Position = 
	{
		Position.x + (Right_Vector.x *LocalPosition.x),
		Position.y + (Right_Vector.y *LocalPosition.x),
		Position.z + (Right_Vector.z *LocalPosition.x)
	};

	Position =
	{
		Position.x + (UP_Vector.x * LocalPosition.y),
		Position.y + (UP_Vector.y * LocalPosition.y),
		Position.z + (UP_Vector.z * LocalPosition.y)
	};


	Position =
	{
		Position.x + (Look_Vector.x * LocalPosition.z),
		Position.y + (Look_Vector.y * LocalPosition.z),
		Position.z + (Look_Vector.z * LocalPosition.z)
	};

	
}

XMFLOAT3 Transform::GetPosition()
{
	return Position;
}



void Transform::SetRotation(float _AngleX, float _AngleY, float _AngleZ)
{
	Rotation.x += _AngleX;
	Rotation.y += _AngleY;
	Rotation.z += _AngleZ;
}

void Transform::SetScale(float X, float Y, float Z)
{
	Scale.x += X;
	Scale.y += Y;
	Scale.z += Z;
}

XMMATRIX Transform::GetMaster()
{
	///계층구조 일떄
	if (isHierarchy == true)
	{
		Master = SetScale4X4(Scale) * EulerToQuaternion(Rotation) * SetPosition4X4(Position);
		return Master;
	}
	else ///그냥 일반일떄
	{
		Master = SetScale4X4(Scale) * SetRotation4X4(Rotation) * SetPosition4X4(Position);
		return Master;
	}
}

XMMATRIX Transform::GetLocalMaster()
{
	
	XMMATRIX m_Master = SetScale4X4(LocalScale) * SetRotation4X4(LocalRotation) * SetPosition4X4(LocalPosition);

	return m_Master;
}

XMMATRIX Transform::SetPosition4X4(XMFLOAT3 Pos)
{
	XMFLOAT4X4 Position_4x4;
	Position_4x4._11 = 1;				Position_4x4._12 = 0;			Position_4x4._13 = 0;			Position_4x4._14 = 0;
	Position_4x4._21 = 0;				Position_4x4._22 = 1;			Position_4x4._23 = 0;			Position_4x4._24 = 0; 
	Position_4x4._31 = 0;				Position_4x4._32 = 0;			Position_4x4._33 = 1;			Position_4x4._34 = 0;
	Position_4x4._41 = Pos.x;		Position_4x4._42 = Pos.y;	Position_4x4._43 = Pos.z;	Position_4x4._44 = 1;

	XMMATRIX T = XMLoadFloat4x4(&Position_4x4);

	return T;
}

XMMATRIX Transform::CreateRotX(float _Angle)
{
	float rad = _Angle * 3.141592f / 180;
	//x 축 회전 행렬
	XMFLOAT4X4 pitch;
	pitch._11 = 1;	pitch._12 = 0;				pitch._13 = 0;				pitch._14 = 0;
	pitch._21 = 0;	pitch._22 = cos(rad);		pitch._23 = -sin(rad);		pitch._24 = 0;
	pitch._31 = 0;	pitch._32 = sin(rad);		pitch._33 =  cos(rad);		pitch._34 = 0;
	pitch._41 = 0;	pitch._42 = 0;				pitch._43 = 0;				pitch._44 = 1;


	return XMLoadFloat4x4(&pitch);
}

XMMATRIX Transform::Create_RotX(float _Angle)
{
	float rad = _Angle * 3.141592f / 180;
	return XMMatrixRotationX(rad);
}

XMMATRIX Transform::CreateRotY(float _Angle)
{
	float rad = _Angle * 3.141592f / 180;
	//y축 회전 행렬
	XMFLOAT4X4 yaw;
	yaw._11 = cos(rad);			yaw._12 = 0;	yaw._13 = sin(rad);			yaw._14 = 0;
	yaw._21 = 0;				yaw._22 = 1;	yaw._23 = 0;				yaw._24 = 0;
	yaw._31 = -sin(rad);		yaw._32 = 0;	yaw._33 = cos(rad);			yaw._34 = 0;
	yaw._41 = 0;				yaw._42 = 0;	yaw._43 = 0;				yaw._44 = 1;

	return XMLoadFloat4x4(&yaw);
}

XMMATRIX Transform::Create_RotY(float _Angle)
{
	float rad = _Angle * 3.141592f / 180;
	return XMMatrixRotationY(rad);
}

XMMATRIX Transform::CreateRotZ(float _Angle)
{
	float rad = _Angle * 3.141592f / 180;
	//z축 회전 행렬
	XMFLOAT4X4 roll;
	roll._11 = cos(rad);		roll._12 = -sin(rad);		roll._13 = 0;	roll._14 = 0;
	roll._21 = sin(rad);		roll._22 = cos(rad);		roll._23 = 0;	roll._24 = 0;
	roll._31 = 0;				roll._32 = 0;				roll._33 = 1;	roll._34 = 0;
	roll._41 = 0;				roll._42 = 0;				roll._43 = 0;	roll._44 = 1;
	return XMLoadFloat4x4(&roll);
}

XMMATRIX Transform::Create_RotZ(float _Angle)
{
	float rad = _Angle * 3.141592f / 180;
	return XMMatrixRotationZ(rad);
}


XMMATRIX Transform::SetScale4X4(XMFLOAT3 _Scale)
{
	XMFLOAT4X4 Scale_4x4;
	Scale_4x4._11 = _Scale.x;	Scale_4x4._12 = 0;			Scale_4x4._13 = 0;			Scale_4x4._14 = 0;
	Scale_4x4._21 = 0;			Scale_4x4._22 = _Scale.y;	Scale_4x4._23 = 0;			Scale_4x4._24 = 0;
	Scale_4x4._31 = 0;			Scale_4x4._32 = 0;			Scale_4x4._33 = _Scale.z;	Scale_4x4._34 = 0;
	Scale_4x4._41 = 0;			Scale_4x4._42 = 0;			Scale_4x4._43 = 0;			Scale_4x4._44 = 1;

	XMMATRIX S = XMLoadFloat4x4(&Scale_4x4);
	return S;
}

XMFLOAT3 Transform::GetMultiplication(XMFLOAT3 v, XMMATRIX xm)
{

	XMFLOAT3 temp;
	float x = v.x;
	float y = v.y;
	float z = v.z;
	float w = 1;

	XMFLOAT4X4 data;
	XMStoreFloat4x4(&data, xm);

	temp.x = (x * data._11) + (y * data._21) + (z * data._31) + (w * data._41);
	temp.y = (x * data._12) + (y * data._22) + (z * data._32) + (w * data._42);
	temp.z = (x * data._13) + (y * data._23) + (z * data._33) + (w * data._43);
	
	return temp;
}


XMMATRIX Transform::SetRotation4X4(XMFLOAT3 axis)
{
	///회전 행렬 구하기 교수님꺼와 다른 버전  유니티 처럼 Angle 값만 넘기고 싶어서
	XMMATRIX _P = CreateRotX(axis.x);
	XMMATRIX _Y = CreateRotY(axis.y);
	XMMATRIX _R = CreateRotZ(axis.z);
	return _R * _Y * _P;
}

XMMATRIX Transform::Set_Rotation4X4(XMFLOAT3 _Rot)
{
	XMMATRIX _P = Create_RotX(_Rot.x);
	XMMATRIX _Y = Create_RotY(_Rot.y);
	XMMATRIX _R = Create_RotZ(_Rot.z);
	return _R * _Y * _P;
}


float Transform::GetNormalize()
{
	float n = sqrt(pow(Position.x, 2) + pow(Position.y, 2) + pow(Position.z, 2));
	return n;
}
XMFLOAT3 Transform::QuaternionToEuler(const SimpleMath::Vector4 Q)
{
	float sqw = Q.w * Q.w;
	float sqx = Q.x * Q.x;
	float sqy = Q.y * Q.y;
	float sqz = Q.z * Q.z;
	float unit = sqx + sqy + sqz + sqw; // if normalised is one, otherwise is correction factor
	float test = Q.x * Q.w - Q.y * Q.z;

	XMFLOAT3 v;
	///각도가 360을 넘어갔을때 처리
	if (test > 0.4995f * unit) 
	{ 

		v.y = 2.0f * atan2(Q.y, Q.x);

		v.x = MathHelper::Pi / 2;

		v.z = 0;

		return NormalizeAngles(v * 360 / (MathHelper::Pi * 2));

	}

	if (test < -0.4995f * unit)
	{

		v.y = -2.0f * atan2(Q.y, Q.x);

		v.x = -MathHelper::Pi / 2;

		v.z = 0;

		return NormalizeAngles(v * 360 / (MathHelper::Pi * 2));

	}
	Quaternion q = Quaternion(Q.w, Q.z, Q.x, Q.y);
	v.y = (float)atan2(2.0f * q.x * q.w + 2.0f * q.y * q.z, 1 - 2.0f * (q.z * q.z + q.w * q.w));// Yaw
	v.x = (float)asin(2.0f * (q.x * q.z - q.w * q.y));// Pitch
	v.z = (float)atan2(2.0f * q.x * q.y + 2.0f * q.z * q.w, 1 - 2.0f * (q.y * q.y + q.z * q.z));// Roll

	return NormalizeAngles(v * 360 / (MathHelper::Pi * 2));

}

void Transform::QuaternionToEuler(XMMATRIX xm)
{
	///이렇게 구한 TM을 Transfrom에 다시찢어서 넣어준다 로컬좌표
	XMVECTOR pos;
	XMVECTOR rot;
	XMVECTOR scl;
	XMFLOAT4 Q_Rot;
	XMMatrixDecompose(&scl, &rot, &pos, xm);
	XMStoreFloat3(&Position, pos);
	XMStoreFloat3(&Scale, scl);

	XMStoreFloat4(&Q_Rot, rot);
	Rotation = QuaternionToEuler(Q_Rot);
}

XMMATRIX Transform::EulerToQuaternion(XMFLOAT3& euler)
{
	///오일러로 된 로테이션값을 쿼터니언의 매트릭스로 변환

	
	//Set_Rotation4X4(euler);

	float x = euler.x * MathHelper::Pi / 180;
	float y = euler.y * MathHelper::Pi / 180;
	float z = euler.z * MathHelper::Pi / 180;

	
	Quaternion Q = Quaternion::CreateFromYawPitchRoll(y, x, z);

	return XMMatrixRotationQuaternion(XMLoadFloat4(&Q));
}

Quaternion Transform::MyEulerToQuaternion()
{
	float x = Rotation.x * MathHelper::Pi / 180;
	float y = Rotation.y * MathHelper::Pi / 180;
	float z = Rotation.z * MathHelper::Pi / 180;


	Quaternion Q = Quaternion::CreateFromYawPitchRoll(y, x, z);
	return  Q;
}

XMFLOAT3 Transform::NormalizeAngles(XMFLOAT3 angles)
{
	angles.x = NormalizeAngle(angles.x);

	angles.y = NormalizeAngle(angles.y);

	angles.z = NormalizeAngle(angles.z);

	return angles;
}

float Transform::NormalizeAngle(float angle)
{
	while (angle > 360)
	{
		angle -= 360;
	}


	while (angle < 0)
	{
		angle += 360;
	}


	return angle;
}


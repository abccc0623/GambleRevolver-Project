#include "EnginePCH.h"



DRCamera::DRCamera()
	: //mPosition(0.0f, 0.0f, 0.0f),
	mRight(1.0f, 0.0f, 0.0f),
	mUp(0.0f, 1.0f, 0.0f),
	mLook(0.0f, 0.0f, 1.0f)
{
	SetLens(0.25f * MathHelper::Pi, 1.0f, 0.0001f, 1000.0f);
	transfrom.GetMaster();
}

DRCamera::~DRCamera()
{

}

XMVECTOR DRCamera::GetPositionXM() const
{
	return XMLoadFloat3(&transfrom.Position);
}

XMFLOAT3 DRCamera::GetPosition() const
{
	return transfrom.Position;
}

//void DRCamera::SetPosition(float x, float y, float z)
//{
//	mPosition = XMFLOAT3(x, y, z);
//}

//void DRCamera::SetPosition(const XMFLOAT3& v)
//{
//	mPosition = v;
//}

XMVECTOR DRCamera::GetRightXM() const
{
	return XMLoadFloat3(&transfrom.Right_Vector);
}

XMFLOAT3 DRCamera::GetRight() const
{
	return transfrom.Right_Vector;
}

XMVECTOR DRCamera::GetUpXM() const
{
	return XMLoadFloat3(&transfrom.UP_Vector);
}

XMFLOAT3 DRCamera::GetUp() const
{
	return transfrom.UP_Vector;
}

XMVECTOR DRCamera::GetLookXM() const
{
	return XMLoadFloat3(&transfrom.Look_Vector);
}

XMFLOAT3 DRCamera::GetLook() const
{
	return transfrom.Look_Vector;
}

float DRCamera::GetNearZ() const
{
	return mNearZ;
}

float DRCamera::GetFarZ() const
{
	return mFarZ;
}

float DRCamera::GetAspect() const
{
	return mAspect;
}

float DRCamera::GetFovY() const
{
	return mFovY;
}

float DRCamera::GetFovX() const
{
	float halfWidth = 0.5f * GetNearWindowWidth();
	return 2.0f * atan(halfWidth / mNearZ);
}

float DRCamera::GetNearWindowWidth() const
{
	return mAspect * mNearWindowHeight;
}

float DRCamera::GetNearWindowHeight() const
{
	return mNearWindowHeight;
}

float DRCamera::GetFarWindowWidth() const
{
	return mAspect * mFarWindowHeight;
}

float DRCamera::GetFarWindowHeight() const
{
	return mFarWindowHeight;
}

void DRCamera::SetLens(float fovY, float aspect, float zn, float zf)
{
	// cache properties
	mFovY = fovY;
	mAspect = aspect;
	mNearZ = zn;
	mFarZ = zf;

	mNearWindowHeight = 2.0f * mNearZ * tanf(0.5f * mFovY);
	mFarWindowHeight = 2.0f * mFarZ * tanf(0.5f * mFovY);

	XMMATRIX P = XMMatrixPerspectiveFovLH(mFovY, mAspect, mNearZ, mFarZ);
	XMStoreFloat4x4(&mProj, P);
}

void DRCamera::LookAt(FXMVECTOR pos, FXMVECTOR target, FXMVECTOR worldUp)
{
	// 시선벡터와 월드업을 외적하면 right벡터가 나오고
	// 다시 그 right벡터와 시선벡터를 외적하면 내 up벡터가 나옴
	XMVECTOR L = XMVector3Normalize(XMVectorSubtract(target, pos));
	XMVECTOR R = XMVector3Normalize(XMVector3Cross(worldUp, L));
	XMVECTOR U = XMVector3Cross(L, R);

	//XMStoreFloat3(&mPosition, transfrom.Position);
	//mPosition = transfrom.Position;
	//mLook = transfrom.Look_Vector;
	//mUp = transfrom.UP_Vector;
	//XMStoreFloat3(&mLook, L);
	//XMStoreFloat3(&mRight, R);
	//XMStoreFloat3(&mUp, U);
}

void DRCamera::LookAt(const XMFLOAT3& pos, const XMFLOAT3& target, const XMFLOAT3& up)
{
	XMVECTOR P = XMLoadFloat3(&pos);
	XMVECTOR T = XMLoadFloat3(&target);
	XMVECTOR U = XMLoadFloat3(&up);

	LookAt(P, T, U);
}

XMMATRIX DRCamera::View() const
{
	return XMLoadFloat4x4(&mView);
}

XMMATRIX DRCamera::Proj() const
{
	return XMLoadFloat4x4(&mProj);
}

XMMATRIX DRCamera::ViewProj() const
{
	return XMMatrixMultiply(View(), Proj());
}

void DRCamera::Strafe(float d)
{
	// mPosition += d*mRight
	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR r = XMLoadFloat3(&transfrom.Right_Vector);
	XMVECTOR p = XMLoadFloat3(&transfrom.Position);
	XMStoreFloat3(&transfrom.Position, XMVectorMultiplyAdd(s, r, p));
}

void DRCamera::Walk(float d)
{
	// mPosition += d*mLook
	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR l = XMLoadFloat3(&transfrom.Look_Vector);
	XMVECTOR p = XMLoadFloat3(&transfrom.Position);
	XMStoreFloat3(&transfrom.Position, XMVectorMultiplyAdd(s, l, p));
}

void DRCamera::WorldUpDown(float d)
{
	// mPosition += d * mUp
	XMVECTOR scale = XMVectorReplicate(d);
	XMVECTOR up = XMLoadFloat3(&transfrom.UP_Vector);
	XMVECTOR position = XMLoadFloat3(&transfrom.Position);
	XMStoreFloat3(&transfrom.Position, XMVectorMultiplyAdd(scale, up, position));
}

void DRCamera::Pitch(float angle)
{
	// Rotate up and look vector about the right vector.

	XMMATRIX R = XMMatrixRotationAxis(XMLoadFloat3(&transfrom.Right_Vector), angle);

	XMStoreFloat3(&transfrom.UP_Vector, XMVector3TransformNormal(XMLoadFloat3(&transfrom.UP_Vector), R));
	XMStoreFloat3(&transfrom.Look_Vector, XMVector3TransformNormal(XMLoadFloat3(&transfrom.Look_Vector), R));
}

void DRCamera::RotateY(float angle)
{
	// Rotate the basis vectors about the world y-axis.

	XMMATRIX R = XMMatrixRotationY(angle);
	
	XMStoreFloat3(&transfrom.Right_Vector, XMVector3TransformNormal(XMLoadFloat3(&transfrom.Right_Vector), R));
	XMStoreFloat3(&transfrom.UP_Vector, XMVector3TransformNormal(XMLoadFloat3(&transfrom.UP_Vector), R));
	XMStoreFloat3(&transfrom.Look_Vector, XMVector3TransformNormal(XMLoadFloat3(&transfrom.Look_Vector), R));
}

void DRCamera::UpdateViewMatrix()
{
	XMVECTOR R = XMLoadFloat3(&transfrom.Right_Vector);
	XMVECTOR U = XMLoadFloat3(&transfrom.UP_Vector);
	XMVECTOR L = XMLoadFloat3(&transfrom.Look_Vector);
	XMVECTOR P = XMLoadFloat3(&transfrom.Position);

	// Keep camera's axes orthogonal to each other and of unit length.
	L = XMVector3Normalize(L);
	U = XMVector3Normalize(XMVector3Cross(L, R));

	// U, L already ortho-normal, so no need to normalize cross product.
	R = XMVector3Cross(U, L);

	// Fill in the view matrix entries.
	float x = -XMVectorGetX(XMVector3Dot(P, R));
	float y = -XMVectorGetX(XMVector3Dot(P, U));
	float z = -XMVectorGetX(XMVector3Dot(P, L));

	XMStoreFloat3(&transfrom.Right_Vector, R);
	XMStoreFloat3(&transfrom.UP_Vector, U);
	XMStoreFloat3(&transfrom.Look_Vector, L);

	mView(0, 0) = transfrom.Right_Vector.x;
	mView(1, 0) = transfrom.Right_Vector.y;
	mView(2, 0) = transfrom.Right_Vector.z;
	mView(3, 0) = x;

	mView(0, 1) = transfrom.UP_Vector.x;
	mView(1, 1) = transfrom.UP_Vector.y;
	mView(2, 1) = transfrom.UP_Vector.z;
	mView(3, 1) = y;

	mView(0, 2) = transfrom.Look_Vector.x;
	mView(1, 2) = transfrom.Look_Vector.y;
	mView(2, 2) = transfrom.Look_Vector.z;
	mView(3, 2) = z;

	mView(0, 3) = 0.0f;
	mView(1, 3) = 0.0f;
	mView(2, 3) = 0.0f;
	mView(3, 3) = 1.0f;
}

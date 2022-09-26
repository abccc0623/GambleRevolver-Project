#include "EnginePCH.h"
#include "GameObject.h"

GameObject::GameObject()
{
	isRender = true;
	transfrom = Transform();

	mWorld	= XMMatrixIdentity();
	mView	= XMMatrixIdentity();
	mProj	= XMMatrixIdentity();

	
	isDelete = false;
	m_pRenderstate = nullptr;

	OnRender = true;
	Alpha = 1;
}

GameObject::~GameObject()
{
	//엔진 정보들은 nullptr
	ENGINE		= nullptr;
	Device		= nullptr;
	mContext	= nullptr;
	m_pRenderstate = nullptr;
}

void GameObject::SetRasterizerState(ID3D11RasterizerState* render)
{
	m_pRenderstate = render;
}

bool GameObject::GetDeleteCheck()
{
	return isDelete;
}

void GameObject::SetParentTR(Transform* tr)
{
	ParentTranform = tr;
	transfrom.isHierarchy = true;
	XMMATRIX TM2_1 = DirectX::XMMatrixInverse(nullptr, ParentTranform->W_TM);
	////나의 로컬은  나의 월드 * 부모의 역행렬
	transfrom.LocalTM = transfrom.W_TM* TM2_1;
	//transfrom.Position = ParentTranform->Position;
	//transfrom.WorldTM = transfrom.W_TM* transfrom.LocalTM* ParentTranform->WorldTM;
}

void GameObject::UpdateParentTr()
{
	transfrom.WorldTM = transfrom.GetMaster()* ParentTranform->WorldTM;
}



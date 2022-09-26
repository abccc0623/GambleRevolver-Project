#include "EnginePCH.h"
#include "GameObject.h"

HsMesh::HsMesh(float x, float y, float z)
{
	m_pRenderstate	= nullptr;
	ParentName = "";
}

HsMesh::~HsMesh()
{
	
}

void HsMesh::init( hsEngine* _ENGINE, LoadData* _LoadData)
{
	//�̰Ŵ� �� �ҷ��ö� ���¿�
	ENGINE = _ENGINE;
	DeviceData* Data = ENGINE->GetDeviceData();

	Device			= Data->m_device;
	mContext		= Data->m_deviceContext;
	m_pRenderstate	= Data->mSolidRS;
	
	///��Ӱ��� ������Ʈ���
	//�θ𿩺�
	isParent = _LoadData->parent;
	//�θ��̸�
	ParentName = _LoadData->ParentName;
	//���� �̸�
	MyName = _LoadData->MyName;

	transfrom.V_TM = _LoadData->V_TM;
	transfrom.W_TM = _LoadData->W_TM;

	Rendering::init(_ENGINE);
	Debugging::init(_ENGINE);

	std::vector<Deferred32> vertex;
	vertex.resize(_LoadData->vcount);
	for (int i = 0; i < vertex.size(); i++)
	{
		float x = _LoadData->vertex_pos[i].x;
		float y = _LoadData->vertex_pos[i].y;
		float z = _LoadData->vertex_pos[i].z;
		vertex[i].Pos		= XMFLOAT4(x,y,z,1);
		vertex[i].Tex		= _LoadData->vertex_tex[i];
		vertex[i].Nomal		= _LoadData->vertex_nomal[i];
		vertex[i].Tangent	= {0,0,0};
	}

	Create_IB_VB_Buffer(vertex, _LoadData->vertex_index);

	Objbuffer = GetBuffer("ObjectBuffer");
}

void HsMesh::Update()
{
	if (transfrom.isHierarchy == true)
	{
		mWorld = transfrom.WorldTM;
	}
	else
	{
		mWorld = transfrom.GetMaster();
	}
}


void HsMesh::Render(const hsViewData& view)
{
	Set_ObjectBuffer_Parameter(Objbuffer, mWorld,XMFLOAT3(1,1,1));

	//�̻��·� �Ž��� �������մϴ�
	MeshRendering();
}

void HsMesh::Delete()
{


}

void HsMesh::Debug()
{
	Debugging::Update(mWorld);
}


void HsMesh::SetTextures(ID3D11ShaderResourceView* Textures)
{
	isTextures = true;
	mMainSRV = Textures;
}

void HsMesh::SetNomalTextures(ID3D11ShaderResourceView* Textures)
{
	isNomal = true;
	mNomalSRV = Textures;
}

void HsMesh::SetRasterizerState(ID3D11RasterizerState* render)
{
	m_pRenderstate = render;
}

void HsMesh::SetSubTextures(ID3D11ShaderResourceView* Textures)
{
	isSubTextures = true;
	mSubSRV = Textures;
}

void HsMesh::SetMaterial(Material mt)
{
	Mat = mt;
}








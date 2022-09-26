#include "EnginePCH.h"
#include "SkyBox.h"

SkyBox::SkyBox(float x, float y, float z)
{
	SkyTextures = nullptr;
}

SkyBox::~SkyBox()
{
	SkyTextures = nullptr;
}



void SkyBox::init(hsEngine* _ENGINE, LoadData* _LoadData)
{
	ENGINE = _ENGINE;
	Device = ENGINE->GetDeviceData()->m_device;
	mContext = ENGINE->GetDeviceData()->m_deviceContext;
	
	Rendering::init(_ENGINE);

	std::vector<Sky32> vertex;
	std::vector<UINT> indices16;
	int vcount = _LoadData->vertex_pos.size();
	int icount = _LoadData->vertex_index.size();
	
	
	vertex.resize(vcount);
	indices16.resize(icount);
	
	for (int i = 0; i < vcount; i++)
	{
		vertex[i].Pos = _LoadData->vertex_pos[i];
	}
	
	for (int j = 0; j < icount; j++)
	{
		indices16[j] = _LoadData->vertex_index[j];
	}

	Create_IB_VB_Buffer(vertex, indices16);
	Skybuffer = GetBuffer("ObjectBuffer");


	transfrom.Scale = { -10,-10,-10 };
	transfrom.Rotation = { 180,0,0 };
}


void SkyBox::Render(const hsViewData& view)
{
	XMFLOAT4X4 pos;
	XMStoreFloat4x4(&pos, view.world);
	transfrom.Position = { pos._41,pos._42 ,pos._43 };
	mWorld = transfrom.GetMaster();

	Set_ObjectBuffer_Parameter(Skybuffer, mWorld, XMFLOAT3(1, 1, 1));
	mContext->PSSetShaderResources(3, 1, &SkyTextures);
	MeshRendering(false);
}

void SkyBox::Update()
{
	
	mWorld = transfrom.GetMaster();
}

void SkyBox::SetSkyTextures(ID3D11ShaderResourceView* Textures)
{
	SkyTextures = Textures;
}



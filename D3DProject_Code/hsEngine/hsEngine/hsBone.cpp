#include "EnginePCH.h"
#include "hsBone.h"

void hsBone::Initialize(hsEngine* _ENGINE, LoadData* _LoadData)
{
	ENGINE = _ENGINE;
	DeviceData* Data = ENGINE->GetDeviceData();
	Device = Data->m_device;
	mContext = Data->m_deviceContext;
	m_pRenderstate = Data->mSolidRS;

	isParent = _LoadData->parent;
	ParentName = _LoadData->ParentName;
	MyName = _LoadData->MyName;

	isSkinning = _LoadData->isSkinnde;
	isBone = _LoadData->isBone;

	transfrom.V_TM = _LoadData->V_TM;
	transfrom.W_TM = _LoadData->W_TM;

	Rendering::init(_ENGINE);

	//���� �ε����� �־���

	if (isSkinning == true)
	{
		for (auto& temp : _LoadData->BoneNumberList)
		{
			isSkinning = true;
			BoneNumberList.insert({ temp.first ,temp.second });
		}
	}


	//�Ž� ������ �־���
	std::vector<Skinnde32> vertex;
	vertex.resize(_LoadData->vcount);
	for (int i = 0; i < vertex.size(); i++)
	{
		vertex[i].Pos = _LoadData->vertex_pos[i];
		vertex[i].Normal = _LoadData->vertex_nomal[i];
		vertex[i].Tex = _LoadData->vertex_tex[i];
		vertex[i].tagantW = {0,0,0};
		vertex[i].Weights = _LoadData->vertex_weights[i];
		
		for (int j = 0; j < 4; j++)
		{
			vertex[i].BoneIndex[j] = _LoadData->vertex_bonIndex[i][j];
		}
	}





	if (isSkinning == true)
	{
		//�ε��� ���ۿ� ���ؽ����� ����
		Create_IB_VB_Buffer(vertex, _LoadData->vertex_index);
	}



	//��Ű�� ���۸������ϰ� �޾ƿ�
	m_Skinning = GetBuffer("SkinningBuffer");
	
}

bool hsBone::MeshAnimation(int FPS)
{
	/// �ϴ� �ִϸ��̼��� ���ٸ� �������� �ʴ´�
	if (isAnime == false) { return false; }


	///�ڱ��� ������ ���´�
	XMVECTOR pos;
	XMVECTOR rot;
	XMVECTOR scl;
	XMMatrixDecompose(&scl, &rot, &pos, transfrom.LocalTM);



	///�ִϸ��̼��� ���������� �˻�
	XMFLOAT3 _pos;
	XMStoreFloat3(&_pos, pos);
	int POS_SIZE = Anime->m_position.size();

	if (POS_SIZE != 0 && POS_SIZE > POS_index)
	{
		if (Anime->m_position[POS_index]->m_time <= FPS)
		{
			XMStoreFloat3(&_pos, pos);
			_pos.x = Anime->m_position[POS_index]->m_pos.x;
			_pos.y = Anime->m_position[POS_index]->m_pos.y;
			_pos.z = Anime->m_position[POS_index]->m_pos.z;
			XM_POS = transfrom.SetPosition4X4(_pos);
			POS_index++;
		}

		if (Anime->m_position[POS_SIZE - 1]->m_time <= FPS)
		{
			POS_ANIMATION_END = true;
		}
	}
	else
	{
		XM_POS = transfrom.SetPosition4X4(_pos);
		POS_ANIMATION_END = true;
	}

	int ROT_SIZE = Anime->m_rotation.size();
	//���� ROT_SIZE�� �ε������� ũ�ٸ� �ִϸ��̼��� �ѹ��� �ٵ��Ҵ�
	if (ROT_SIZE != 0 && ROT_SIZE > ROT_index)
	{
		if (Anime->m_rotation[ROT_index]->m_time <= FPS)
		{
			SimpleMath::Quaternion Anime_Q = Anime->m_rotation[ROT_index]->m_rotQT_accumulation;
			XM_ROT = XMMatrixRotationQuaternion(Anime_Q);
			ROT_index++;
		}

		if (Anime->m_rotation[ROT_SIZE - 1]->m_time <= FPS)
		{
			ROT_ANIMATION_END = true;
		}
	}
	else
	{
		XM_ROT = XMMatrixRotationQuaternion(rot);
		ROT_ANIMATION_END = true;
	}

	//ũ�� ��� �籸��
	//����� ���ʹ����� �����鼭 �̼��� �� ������ �������� �۾����ų� Ŀ��
	//�׷��� �ѹ� ���������� ���������Ϸ� ��� �־���
	XMFLOAT3 _scl;
	_scl = transfrom.Scale;
	XMStoreFloat3(&_scl, scl);
	_scl = transfrom.Scale;


	XM_SCL = XMMatrixScalingFromVector(XMLoadFloat3(&_scl));

	//���� TM �籸��
	transfrom.LocalTM = XM_SCL * XM_ROT * XM_POS;


	if (POS_ANIMATION_END == true && ROT_ANIMATION_END == true)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void hsBone::SetTextures(ID3D11ShaderResourceView* textures)
{
	mMainSRV = textures;
}

void hsBone::SetNomalTextures(ID3D11ShaderResourceView* textures)
{
	mNomalSRV = textures;
}

void hsBone::SetBoneSkinningTM(XMMATRIX offset)
{
	BoneOffsetTM.push_back(offset);
}

void hsBone::SetBoneMesh(hsBone* Bone)
{
	BoneList.push_back(Bone);
}

void hsBone::AnimationReset()
{
	POS_ANIMATION_END = false;
	ROT_ANIMATION_END = false;
	POS_index = 0;
	ROT_index = 0;
}

hsBone::hsBone()
{
	Device = nullptr;
	mContext = nullptr;
}

hsBone::~hsBone()
{
	ENGINE = nullptr;
	Device = nullptr;
	mContext = nullptr;

	std::map<string, hsAnimation*>::iterator it = AnimationList.begin();
	for (it; it != AnimationList.end(); it++)
	{
		(*it).second = nullptr;
	}
	Anime = nullptr;
	//AnimationList.clear();
}

void hsBone::Update()
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

void hsBone::Render(const hsViewData& view)
{
	std::vector<DirectX::XMMATRIX> test;
	if (isSkinning == true)
	{
		//��Ų
		for (int i = 0; i < BoneList.size(); i++)
		{
			test.push_back(BoneOffsetTM[i] * BoneList[i]->transfrom.WorldTM);
		}
		Rendering::Set_SkinningBuffer_Parameter(m_Skinning, mWorld, &test);
		MeshRendering();
	}

}

void hsBone::Delete()
{

}

void hsBone::Debug()
{
	//Debugging::Update(mWorld);
}


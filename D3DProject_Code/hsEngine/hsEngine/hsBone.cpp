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

	//본의 인덱스를 넣어줌

	if (isSkinning == true)
	{
		for (auto& temp : _LoadData->BoneNumberList)
		{
			isSkinning = true;
			BoneNumberList.insert({ temp.first ,temp.second });
		}
	}


	//매쉬 정보를 넣어줌
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
		//인덱스 버퍼와 버텍스버퍼 생성
		Create_IB_VB_Buffer(vertex, _LoadData->vertex_index);
	}



	//스키닝 버퍼를생성하고 받아옴
	m_Skinning = GetBuffer("SkinningBuffer");
	
}

bool hsBone::MeshAnimation(int FPS)
{
	/// 일단 애니메이션이 없다면 실행하지 않는다
	if (isAnime == false) { return false; }


	///자기의 로컬을 찢는다
	XMVECTOR pos;
	XMVECTOR rot;
	XMVECTOR scl;
	XMMatrixDecompose(&scl, &rot, &pos, transfrom.LocalTM);



	///애니메이션을 돌릴것인지 검사
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
	//만약 ROT_SIZE가 인덱스보다 크다면 애니메이션은 한바퀴 다돌았다
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

	//크기 행렬 재구성
	//행렬을 백터단위로 나누면서 미세한 값 오차로 스케일이 작아지거나 커짐
	//그래서 한번 돌릴때마다 원래스케일로 계속 넣어줌
	XMFLOAT3 _scl;
	_scl = transfrom.Scale;
	XMStoreFloat3(&_scl, scl);
	_scl = transfrom.Scale;


	XM_SCL = XMMatrixScalingFromVector(XMLoadFloat3(&_scl));

	//로컬 TM 재구성
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
		//스킨
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



#include "atlbase.h"
#include "EnginePCH.h"
#include "ResoureceManager.h"

ResourceManager::ResourceManager()
{
	ENGINE = nullptr;
	ASE = nullptr;
	FBX = nullptr;

	Load_BaseOBJPath		= "";
	Load_ASE_MeshPath		= "";
	Load_TexturePath		= "";
	Load_NomalTexturePath	= "";
}

ResourceManager::~ResourceManager()
{
	Delete();
}

void ResourceManager::ClearMesh(string Name)
{
	MeshList.erase(Name);
}

void ResourceManager::ClearAnimation(string Name)
{
	map<string, LoadAnimation*>::iterator it = AnimationList.begin();
	for (it; it != AnimationList.end(); it++)
	{
		if (it->first == Name)
		{
			delete (&it);
			it->second = nullptr;
		}
	}
}

void ResourceManager::SetLoadPath(LOAD_TYPE type, string _path)
{
	switch (type)
	{
	case LOAD_TYPE::ASE:
		Load_ASE_MeshPath = _path;
		break;
	case LOAD_TYPE::TEXTURES:
		Load_TexturePath = _path;
		break;
	case LOAD_TYPE::FBX:
		FBX->SetLoadPath(_path);
		break;
	}
}

void ResourceManager::SetResource(LOAD_TYPE type, string Name, string ReName)
{
	string path;
	CString lpctpath;

	switch (type)
	{
	case LOAD_TYPE::FBX:
	{
		string AnimeName = GetNameString(Name, '.','_');
		//이미 매쉬에 정보가 있다면 매쉬는 읽지않음

		if (MeshList.find(ReName) == MeshList.end())
		{
			MeshList.insert(pair<string, LoadMesh*>(ReName, FBX->Load_MODELS_FBX(Name)));
		}
		else
		{
			FBX->Load_MODELS_FBX(Name, true);
		}

		LoadAnimation* temp = FBX->GetOneAnimation();

		//애니메이션 정보가 있다면 뽑아옴
		if (temp != nullptr)
		{
			AnimationList.insert({ AnimeName,temp});
			FBX->AnimeReset();
		}

		break;
	}
	case LOAD_TYPE::ASE:
	{
		MeshList.insert(pair<string, LoadMesh*>(ReName, Load_MODELS_ASE(Name, ReName)));
		break;
	}
	case LOAD_TYPE::TEXTURES:
	{
		path = Load_TexturePath;
		path += Name;
		lpctpath = path.c_str();
		TexturesList.insert(pair<string, ID3D11ShaderResourceView*>(ReName, Load_Textures(lpctpath)));
		break;
	}
	case LOAD_TYPE::NOMAL:
		path = Load_TexturePath;
		path += Name;
		lpctpath = path.c_str();
		NomalTexturesList.insert(pair<string, ID3D11ShaderResourceView*>(ReName, Load_Textures(lpctpath)));
		break;
	}
}

ID3D11ShaderResourceView* ResourceManager::GetTexture(string Name)
{
	return TexturesList[Name];
}

ID3D11ShaderResourceView* ResourceManager::GetNomalTexture(string Name)
{
	return NomalTexturesList[Name];
}

LoadMesh* ResourceManager::GetMesh(string Name)
{
	return  MeshList[Name];
}

LoadAnimation* ResourceManager::GetAnimation(string Name)
{
	return AnimationList[Name];
}

void ResourceManager::Delete()
{
	ENGINE = nullptr;

	//ASE매쉬 데이터 지우기(현재는 쓰지는않지만 나중에 쓸수도있어서 )
	std::map<string, LoadMesh*>::iterator temp01 = MeshList.begin();
	for (temp01; temp01 != MeshList.end(); temp01++)
	{
		LoadMesh* k = temp01->second;
		temp01->second = nullptr;
		delete k;
	}

	//텍스쳐 데이터 지우기
	std::map<string, ID3D11ShaderResourceView*>::iterator temp02 = TexturesList.begin();
	for (temp02; temp02 != TexturesList.end(); temp02++)
	{

		ID3D11ShaderResourceView* k = temp02->second;
		if (k != nullptr)
		{
			temp02->second = nullptr;
			k->Release();
		}
	}

	//노말 텍스쳐 지우기
	std::map<string, ID3D11ShaderResourceView*>::iterator temp03 = NomalTexturesList.begin();
	for (temp03; temp03 != NomalTexturesList.end(); temp03++)
	{
		ID3D11ShaderResourceView* k = temp03->second;
		if (k != nullptr)
		{
			temp03->second = nullptr;
			k->Release();
		}
	}

	//애니메이션 정보 지우기
	std::map<string, LoadAnimation*>::iterator temp04 = AnimationList.begin();
	for (temp04; temp04 != AnimationList.end(); temp04++)
	{
		LoadAnimation* k = temp04->second;
		temp04->second = nullptr;
		delete k;
	}
	delete ASE;
	FBX->Delete();
}

void ResourceManager::init(hsEngine* _ENGINE)
{
	ENGINE = _ENGINE;
	ASE = new CASEParser();
	FBX = new FBXParser();

	ASE->Init();
	FBX->init();
	
	

	//기본 원 로드
	Load_BaseOBJPath = "../../ASEFile/Sphere.ASE";
	MeshList.insert(pair<string, LoadMesh*>("Sphere", Load_MODELS_ASE(Load_BaseOBJPath, "Sphere")));
}

string ResourceManager::GetNameString(string Name, char EndStr, char Start)
{
	string str = Name;
	string temp = "";
	int Start_Index = 0;
	bool read = false;
	//처음부터 읽을것이다
	

	for (int i = 0; i < str.size(); i++)
	{
		if (str[i] == Start)
		{
			Start_Index = i + 1;
			read = true;
		}

		if (str[i] == EndStr && read == true)
		{
			read = false;
		}

		if (read == true && i >= Start_Index)
		{
			temp += str[i];
		}
	}

	int num = 0;
	return temp;
}

ID3D11ShaderResourceView* ResourceManager::Load_Textures(LPCTSTR _path)
{
	ID3D11ShaderResourceView* Textures = nullptr;
	ID3D11Resource* texResource = nullptr;

	DeviceData* data = ENGINE->GetDeviceData();

	if (FAILED(CreateDDSTextureFromFile(data->m_device, _path, &texResource, &Textures, 0)))
	{
		MessageBox(0, L"텍스쳐를 불러오는 도중 에러발생", 0, 0);
		return nullptr;
	}

	texResource->Release();
	return Textures;
}

LoadMesh* ResourceManager::Load_MODELS_ASE(string _path, string ReName)
{
	//string path;
	//CString lpctpath;
	//
	string path;
	CString lpctpath;

	path = Load_ASE_MeshPath;
	path += _path;
	LPCSTR temp = path.c_str();


	//LPSTR temp = (LPSTR)(temp);
	ASE->Load((LPSTR)(temp));
	int size = ASE->m_MeshList.size();
	ASE->m_list_animation;

	LoadMesh* hsMesh = nullptr;
	if (MeshList.count(ReName) == false)
	{
		vector<string> Top_child_list;
		hsMesh = new LoadMesh();

		for (int j = 0; j < size; j++)
		{
			LoadData* data = new LoadData;
			ASEParser::Mesh* mesh = ASE->GetMesh(j);

			///각종 정보들을 넘겨줌
			int FaceCount = mesh->m_meshface.size();
			data->vcount = mesh->m_meshvertex.size();
			data->tcount = FaceCount;

			//data->vertices.resize(data->vcount);
			data->vertex_index.resize(data->tcount * 3);
			data->parent = mesh->m_isparentexist;

			//버택스 정보들
			data->vertex_pos.resize(data->vcount);
			data->vertex_nomal.resize(data->vcount);
			data->vertex_weights.resize(data->vcount);
			data->vertex_bonIndex.assign(data->vcount, vector<UINT>(4, 0));
			data->vertex_tex.resize(data->vcount);
			data->vertex_tangentU.resize(data->vcount);



			data->isSkinnde = mesh->m_is_skinningobject;
			data->isBone = mesh->m_is_boneObject;

			//스키닝 오브젝트용 변수들	
			int BoneIndex = 0;
			std::map<int, string>::iterator it = mesh->m_is_BoneList.begin();
			for (it; it != mesh->m_is_BoneList.end(); it++)
			{
				data->BoneNumberList.insert({ mesh->m_is_BoneList[BoneIndex],BoneIndex });
				BoneIndex++;
			}

			//상속계층 변수들
			if (mesh->m_nodeparent != "")
			{
				data->ParentName = mesh->m_nodeparent;
			}
			else
			{
				data->ParentName = "TOP_OBJ";
			}
			data->MyName = mesh->m_nodename;
			data->W_TM = mesh->W_TM;


			///버택스의 월드 의 역행렬을 구함
			XMVECTOR det = DirectX::XMMatrixDeterminant(data->W_TM);
			XMMATRIX World_Inverse_TM = DirectX::XMMatrixInverse(&det, data->W_TM);

			XMFLOAT4X4 W_InverseTM;
			XMStoreFloat4x4(&W_InverseTM, World_Inverse_TM);

			///버택스의 로컬 행렬
			data->V_TM = World_Inverse_TM;

			for (int i = 0; i < data->vcount; i++)
			{
				data->vertex_nomal[i] = mesh->m_meshvertex[i]->m_normal;

				///위치값을 가져옴
				data->vertex_pos[i].x = mesh->m_meshvertex[i]->m_pos.x;
				data->vertex_pos[i].y = mesh->m_meshvertex[i]->m_pos.y;
				data->vertex_pos[i].z = mesh->m_meshvertex[i]->m_pos.z;


				///나의 버택스에 TM행렬의 역행렬을 곱함
				Vector4 temp;
				float x = data->vertex_pos[i].x;
				float y = data->vertex_pos[i].y;
				float z = data->vertex_pos[i].z;
				float w = 1;


				temp.x = (x * W_InverseTM._11) + (y * W_InverseTM._21) + (z * W_InverseTM._31) + (w * W_InverseTM._41);
				temp.y = (x * W_InverseTM._12) + (y * W_InverseTM._22) + (z * W_InverseTM._32) + (w * W_InverseTM._42);
				temp.z = (x * W_InverseTM._13) + (y * W_InverseTM._23) + (z * W_InverseTM._33) + (w * W_InverseTM._43);
				temp.w = (x * W_InverseTM._14) + (y * W_InverseTM._24) + (z * W_InverseTM._34) + (w * W_InverseTM._44);
				///이렇게 나온값은 로컬좌표임 이걸로 버택스의 위치를넣어줌
				
				data->vertex_pos[i].x = temp.x;
				data->vertex_pos[i].y = temp.y;
				data->vertex_pos[i].z = temp.z;
				
				data->vertex_tex[i].x = mesh->m_meshvertex[i]->u;
				data->vertex_tex[i].y = mesh->m_meshvertex[i]->v;
				//
				//
				data->vertex_weights[i].x = mesh->m_meshvertex[i]->m_bw1;
				data->vertex_weights[i].y = mesh->m_meshvertex[i]->m_bw2;
				data->vertex_weights[i].z = mesh->m_meshvertex[i]->m_bw3;

				
				mesh->m_meshvertex[i]->m_indices;



				BYTE byte[4] = { 0,0,0,0 };
				for (int k = 0; k < 4; k++)
				{
					data->vertex_bonIndex[i][k] = mesh->m_meshvertex[i]->m_indicesPointer[k];
				}
				int num = 0;

			}


			int index = 0;
			for (int i = 0; i <= (FaceCount * 3) - 1; i = i + 3)
			{
				///인덱스값을 가져옴
				data->vertex_index[i] = mesh->m_meshface[index]->m_vertexindex[0];
				data->vertex_index[i + 2] = mesh->m_meshface[index]->m_vertexindex[1];
				data->vertex_index[i + 1] = mesh->m_meshface[index]->m_vertexindex[2];

				///탄젠트 스페이스 구하기..
				//하나의 삼각형의 버텍스3개
				int in0 = mesh->m_meshface[index]->m_vertexindex[0];
				int in1 = mesh->m_meshface[index]->m_vertexindex[1];
				int in2 = mesh->m_meshface[index]->m_vertexindex[2];

				//그버텍스들의 위치값
				Vector3 p0 = data->vertex_pos[in0];
				Vector3 p1 = data->vertex_pos[in1];
				Vector3 p2 = data->vertex_pos[in2];

				//원점에서의 벡터
				Vector3 e1 = p1 - p0;
				Vector3 e2 = p1 - p0;

				//uv좌표
				float w0_x = data->vertex_tex[in0].x;
				float w0_y = data->vertex_tex[in0].y;

				float w1_x = data->vertex_tex[in1].x;
				float w1_y = data->vertex_tex[in1].y;

				float w2_x = data->vertex_tex[in2].x;
				float w2_y = data->vertex_tex[in2].y;


				float x1 = w1_x - w0_x;
				float y1 = w1_y - w0_y;

				float x2 = w2_x - w0_x;
				float y2 = w2_y - w0_y;

				float temp = (x1 * y2 - x2 * y1);
				float r = 1.0f / temp;

				Vector3 t = (e1 * y2 - e2 * y1) * r;

				data->vertex_tangentU[in0] += t;
				data->vertex_tangentU[in1] += t;
				data->vertex_tangentU[in2] += t;


				index++;
			}
			hsMesh->meshList.push_back(data);
		}
	}



	

	///매쉬들이 완성 되었다

	///애니메이션을 불러온다
	
	if (ASE->m_list_animation.size() != 0)
	{
		//애니메이션의 이름을 모델파일이름의 '_' 와 '.' 사이의 string 을 이름으로 정한다
		string str = temp;
		string AnimeName = "";
		int Start_Index = 0;
		bool read = false;
		for (int i = 0; i < str.size(); i++)
		{
			if (str[i] == '_')
			{
				Start_Index = i + 1;
				read = true;
			}

			if (str[i] == '.' && read == true)
			{
				read = false;
			}

			if (read == true && i >= Start_Index)
			{
				AnimeName += str[i];
			}
		}


		AnimationList.insert(pair<string, LoadAnimation* >(AnimeName, Load_Animation(ASE->m_list_animation, 10)));

		//애니메이션 단위 설정...
		AnimationList[AnimeName]->EndFrame		= ASE->m_scenedata.m_lastframe;
		AnimationList[AnimeName]->StartFrame	= ASE->m_scenedata.m_firstframe;
		AnimationList[AnimeName]->Speed			= ASE->m_scenedata.m_framespeed;
		AnimationList[AnimeName]->OneFrame		= ASE->m_scenedata.m_ticksperframe;
	}
	 

	ASE->m_MeshList.clear();
	ASE->m_list_animation.clear();
	
	return hsMesh;
}

LoadMesh* ResourceManager::Load_MODELS_FBX(string path, string ReName)
{
	LoadMesh* temp = FBX->Load_MODELS_FBX(path);
	MeshList.insert({ ReName ,temp });
	return temp;
}

LoadAnimation* ResourceManager::Load_Animation(std::list<hsAnimation*>& anim, int keyCount)
{
	LoadAnimation* hsAnime = new LoadAnimation();
	///기존 애니메이션
	std::list<hsAnimation*>::iterator it = anim.begin();
	for (it; it != anim.end(); it++)
	{
		///애니메이션을 새로 만든다
		hsAnimation* temp = new hsAnimation();
		temp->m_nodename = (*it)->m_nodename;


		///애니메이션 위치 만큼 반복문 돌림
		for (int i = 0; i + 1 < (*it)->m_position.size(); i++)
		{
			///이 사이로 값들을 넣어 주어야 한다
			CAnimation_pos* Start_Pos = (*it)->m_position[i];
			Start_Pos->m_time = (*it)->m_position[i]->m_time;


			CAnimation_pos* End_Pos = (*it)->m_position[i + 1];


			///처음 부분 넣어줌
			temp->m_position.push_back(Start_Pos);

			float Lerp = 1 / (float)keyCount;
			float CountLerp = Lerp;

			for (int i = 0; i < keyCount - 1; i++)
			{
				///보간 시작
				CAnimation_pos* _pos = new CAnimation_pos();
				Vector3 pos = Vector3::Lerp(Start_Pos->m_pos, End_Pos->m_pos, CountLerp);
				int time = MathHelper::Lerp(Start_Pos->m_time, End_Pos->m_time, CountLerp);
				_pos->m_time = time;
				_pos->m_pos = pos;

				///중간 부분 넣어줌
				temp->m_position.push_back(_pos);
				CountLerp += Lerp;
			}

		}

		///일단 쿼터니언 값을 누적시켜준다
		Quaternion Q = Quaternion::Identity;
		for (int i = 0; i < (*it)->m_rotation.size(); i++)
		{
			Q *= Quaternion::CreateFromAxisAngle((*it)->m_rotation[i]->m_rot, (*it)->m_rotation[i]->m_angle);
			(*it)->m_rotation[i]->m_rotQT_accumulation = Q;
		}


		///회전의 정보를 가지고 있다면
		for (int i = 0; i + 1 < (*it)->m_rotation.size(); i++)
		{
			CAnimation_rot* Start_Q = (*it)->m_rotation[i];
			CAnimation_rot* End_Q = (*it)->m_rotation[i + 1];

			temp->m_rotation.push_back(Start_Q);

			float Lerp = 1 / (float)keyCount;
			float CountLerp = Lerp;
			for (int i = 0; i < keyCount - 1; i++)
			{
				///보간 시작
				CAnimation_rot* _Rot = new CAnimation_rot();
				_Rot->m_rotQT_accumulation = XMQuaternionSlerp(Start_Q->m_rotQT_accumulation, End_Q->m_rotQT_accumulation, CountLerp);
				int time = MathHelper::Lerp(Start_Q->m_time, End_Q->m_time, CountLerp);
				_Rot->m_time = time;
				temp->m_rotation.push_back(_Rot);
				CountLerp += Lerp;
			}
		}


		///매쉬 하나의 애니메이션이 완성됨
		//hsAnime.Anim->push_back(temp);
		hsAnime->Anime.push_back(temp);
	}

	return hsAnime;
}


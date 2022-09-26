
#include "EnginePCH.h"
#include "FBXParser.h"
#pragma warning(disable:4996)
FBXParser::FBXParser()
{
	FBX_manager = nullptr;
	m_Scene = nullptr;
	FBX_Node = nullptr;
	FBX_Mesh = nullptr;
	ios = nullptr;
	importer = nullptr;

	data = nullptr;
	dataList = nullptr;
}

FBXParser::~FBXParser()
{


}

void FBXParser::init()
{
	//객체 생성
	FBX_manager = FbxManager::Create();


	//씬 한개 생성 만약 동시에 여러개의 파일을 로드하고싶으면 각 파일의 씬을 생성
	m_Scene = FbxScene::Create(FBX_manager, "scene");


	//FBX관리자 객체 생성
	ios = FbxIOSettings::Create(FBX_manager, IOSROOT);
	FBX_manager->SetIOSettings(ios);


	//매쉬 생성
	FBX_Mesh = FbxMesh::Create(m_Scene, "");

	//스크린 셋팅
	importer = FbxImporter::Create(FBX_manager, "");
}

void FBXParser::Delete()
{
	//생성의 역순으로 삭제
	//FBX_Mesh->Destroy();
	//FBX_Node->Destroy();
	//FBX_Scene->Destroy();
	//FBX_manager->Destroy();
}

LoadMesh* FBXParser::Load_MODELS_FBX(string FileName, bool LoadAnimation)
{
	string path = FilePath + FileName;

	//임포터 생성
	bool status = importer->Initialize(path.c_str(), -1, FBX_manager->GetIOSettings());

	importer->Import(m_Scene);

	SceneSetting(m_Scene);

	//파일 불러오고 실패여부
	if (!status) { return nullptr; }

	//애니메이션 정보만 빼올것인지
	OnlyAnimation = LoadAnimation;

	//씬의 루트 노드를 가져온다
	FBX_Node = m_Scene->GetRootNode();


	//매쉬리스트를 담은 놈
	dataList = nullptr;
	dataList = new LoadMesh();


	//애니메이션전체적인 내용을 가져옴
	//매쉬마다의 애니메이션정보는 매쉬를 만들면서
	GetAnimation();

	//재귀를 돌면서 매쉬를 찾고 애니메이션을하고 별거다함
	FindNode(FBX_Node);



	SkinListData.clear();
	BoneListData.clear();
	//끝
	return dataList;
}

void FBXParser::CreateSkeleton(FbxNode* node)
{
	if (OnlyAnimation == true) { return; }
	//스켈레톤 정보를찾는다
	string name = node->GetName();

	//본을 생성
	LoadData* bone = new LoadData();

	//나의 이름 넣기
	bone->isBone = true;
	bone->MyName = node->GetName();

	//부모가있으면 부모의 이름도 넣어줌
	FbxNode* Parent = node->GetParent();
	if (Parent != nullptr)
	{
		FbxString ParentName = Parent->GetName();

		//에드온에서 부모가없는 최상위 노드의 이름이 달라짐...
		if (ParentName == "RootNode" || ParentName == "root" || ParentName == "Armature")
		{
			//최상위오브젝트는 부모가없다는뜻으로 이것을
			bone->ParentName = "TOP_OBJ";
		}
		else
		{
			bone->ParentName = ParentName;
		}
	}

	//각각의 Tm들을 가져옴
	Bone* BoneData = BoneListData[bone->MyName];
	if (BoneData != nullptr)
	{
		XMFLOAT4X4 V_TM_4x4 = BoneListData[bone->MyName]->TransfromTm;
		XMFLOAT4X4 W_TM_4x4 = BoneListData[bone->MyName]->ParentTM;
		bone->V_TM = XMLoadFloat4x4(&V_TM_4x4);
		bone->W_TM = XMLoadFloat4x4(&W_TM_4x4);
		bone->isBone = true;
		bone->isSkinnde = false;

		dataList->meshList.push_back(bone);
	}

	//Bonelist.push_back(bone);
}

void FBXParser::CreateWeightTable(FbxNode* node, LoadData* m_data)
{
	FbxMesh* mesh = node->GetMesh();
	int  v_count = mesh->GetControlPointsCount();

	//웨이팅값을 가져오기위함
	int DeformerCount = mesh->GetDeformerCount();
	for (int i = 0; i < DeformerCount; i++)
	{
		//본들이 움직이게 만드는 스킨이다 
		FbxSkin* skin = reinterpret_cast<FbxSkin*>(mesh->GetDeformer(i, FbxDeformer::eSkin));

		//스킨만 사용할것이기때문에 스킨값이맞는지 확인
		if (skin == nullptr) { continue; };

		weights.resize(v_count);
		bonIndex.resize(v_count);
		for (int i = 0; i < bonIndex.size(); i++)
		{
			bonIndex[i].resize(4);
			bonIndex[i][0] = 0;
			bonIndex[i][1] = 0;
			bonIndex[i][2] = 0;
			bonIndex[i][3] = 0;
		}


		//이스킨의 본계수를 가져옴
		int ClusterCount = skin->GetClusterCount();

		for (int clusterIndex = 0; clusterIndex < ClusterCount; clusterIndex++)
		{

			FbxCluster* cluster = skin->GetCluster(clusterIndex);

			//값이들어오지않으면 조사안함
			if (cluster == nullptr) { continue; }

			//지금 조사하고있는 본에 이름을 가져온다
			string BoneName = cluster->GetLink()->GetName();


			//본이 없다면 새로만듬
			if (FindBone(BoneName) == false)
			{
				Bone* bone = new Bone;
				bone->MyName = BoneName;
				bone->index = clusterIndex;

				//월드 TM이 필요하다
				FbxAMatrix transformMatrix;
				FbxAMatrix transformLinkMatrix;

				//본의 로컬
				cluster->GetTransformMatrix(transformMatrix);
				bone->TransfromTm = ChangeDirMatrix(transformMatrix);

				//본의 열결된 스킨의 메트릭스
				cluster->GetTransformLinkMatrix(transformLinkMatrix);
				bone->ParentTM = ChangeDirMatrix(transformLinkMatrix);

				////본의 오프셋을저장
				BoneListData.insert({ bone->MyName,bone });
			}

			///이밑은 본의 웨이팅정보와 인덱스정보를 만들어줌

			//이본이 영향을 주고있는 버택스의 카운터
			int WeightMaxCount = cluster->GetControlPointIndicesCount();

			//기본값 설정
			vector<int>testindex;
			vector<double>testWeight;

			//이본과 연결되어있는 모든 버텍스의 인덱스를 가져옴
			for (int k = 0; k < WeightMaxCount; k++)
			{
				int testIndex = cluster->GetControlPointIndices()[k];
				float testWeight = cluster->GetControlPointWeights()[k];

				for (int m = 0; m < 4; m++)
				{
					//값이 들어있지않다면
					if (bonIndex[testIndex][m] == 0)
					{
						//본의 인덱스를 넘겨줌
						bonIndex[testIndex][m] = clusterIndex;

						//웨이팅 정보를 넘겨줌
						switch (m)
						{
						case 0:
							weights[testIndex].x = testWeight;
							break;
						case 1:
							weights[testIndex].y = testWeight;
							break;
						case 2:
							weights[testIndex].z = testWeight;
							break;
						case 3:
							weights[testIndex].w = testWeight;
							break;
						}
						break;
					}
				}
			}
		}
	}
}

void FBXParser::CreateVertexPosTable(FbxMesh* mesh)
{
	int count = mesh->GetControlPointsCount();
	for (int i = 0; i < count; i++)
	{
		XMFLOAT3 p;
		p.x = mesh->GetControlPointAt(i).mData[0];
		p.y = mesh->GetControlPointAt(i).mData[2];
		p.z = mesh->GetControlPointAt(i).mData[1];

		data->vertex_pos.push_back(p);
	}
}

void FBXParser::CreateVertexIndexTable(FbxMesh* mesh)
{
	//페이스 마다의 인덱스를 가져옴
	int count = mesh->GetPolygonCount();
	for (int i = 0; i < count; i++) //삼각형의 개수
	{
		for (int j = 0; j < 3; j++) //삼각형은 3개의 정점으로 구성
		{
			int controlPointIndex = mesh->GetPolygonVertex(i, j);
			Vertexindex.push_back(controlPointIndex);
		}
	}
	int num = 0;
}

void FBXParser::CreateTangentTable(FbxMesh* mesh)
{
	data->vertex_tangentU.resize(data->vertex_pos.size());
	int TrgCount = 0;
	int  v_count = mesh->GetControlPointsCount();
	for (int i = 0; i <= (v_count * 3); i += 3)
	{
		//페이스한개의 버텍스 인덱스들을 가져옴
		int index0 = mesh->GetPolygonVertex(TrgCount, 0);
		int index1 = mesh->GetPolygonVertex(TrgCount, 1);
		int index2 = mesh->GetPolygonVertex(TrgCount, 2);
		
		//페이스한개의 해당되는 위치값들을 갖옴
		Vector3 p0 = data->vertex_pos[index0];
		Vector3 p1 = data->vertex_pos[index1];
		Vector3 p2 = data->vertex_pos[index2];
		
		//원점에서의 벡터를구함
		Vector3 e1 = p1 - p0;
		Vector3 e2 = p1 - p0;
		
		float w0_x = data->vertex_tex[index0].x;
		float w0_y = data->vertex_tex[index0].y;
		
		float w1_x = data->vertex_tex[index1].x;
		float w1_y = data->vertex_tex[index1].y;
		
		float w2_x = data->vertex_tex[index2].x;
		float w2_y = data->vertex_tex[index2].y;
		
		float x1 = w1_x - w0_x;
		float y1 = w1_y - w0_y;
		
		float x2 = w2_x - w0_x;
		float y2 = w2_y - w0_y;
		
		float temp = (x1 * y2 - x2 * y1);
		float r = 1.0f / temp;
		
		Vector3 t = (e1 * y2 - e2 * y1) * r;
		
		data->vertex_tangentU[index0] = {0,0,0};
		data->vertex_tangentU[index1] = { 0,0,0 };
		data->vertex_tangentU[index2] = { 0,0,0 };
	
		TrgCount++;
	}

	int num = 0;
}

LoadData* FBXParser::CreateMesh(FbxNode* node)
{
	if (OnlyAnimation == true) { return nullptr; }
	//데이터를 저장할매쉬
	data = new LoadData;

	//일단 모두 읽는다

	FbxMesh* mesh = node->GetMesh();
	mesh->GetDeformerCount();

	int  v_count = mesh->GetControlPointsCount();

	//로드하기전 혹시 이전에 로드하던 데이터가 남았을지도 모르니
	//클리어 한번해줌
	DataClear();

	//스플릿을 하기전 웨이팅 테이블
	CreateWeightTable(node, data);
	//스플릿 하기전 버텍스 테이블 
	CreateVertexPosTable(mesh);
	//스플릿 하기전 인덱스 테이블
	CreateVertexIndexTable(mesh);

	//UV와 NOMAL 은 스플릿 하면서 가져올것임

	//위에 정보를가지고 스플릿
	Split(mesh);


	//매쉬는 끝났고 부가 내용을 추가


	//나의 이름을넣어줌
	data->MyName = node->GetName();
	FbxNode* ParentNode = node->GetParent();
	if (ParentNode != nullptr)
	{
		string ParentName = ParentNode->GetName();

		//에드온에서 부모가없는 최상위 노드의 이름이 달라짐...
		if (ParentName == "RootNode" || ParentName == "root")
		{
			//최상위오브젝트는 부모가없다는뜻으로 이것을
			data->ParentName = "TOP_OBJ";
		}
		else
		{
			data->ParentName = ParentName;
		}
	}

	//로컬좌표와 월드좌표를 넘겨줌

	//Utilities::GetGeometryTransformation


	//mesh->GetNode()->Ge
	XMFLOAT4X4 V_TM_4x4 = ChangeDirMatrix(node->GetScene()->GetAnimationEvaluator()->GetNodeLocalTransform(node));
	XMFLOAT4X4 W_TM_4x4 = ChangeDirMatrix(node->GetScene()->GetAnimationEvaluator()->GetNodeGlobalTransform(node));


	data->V_TM = CheckNegative(V_TM_4x4);
	data->W_TM = CheckNegative(W_TM_4x4);
	data->isSkinnde = true;
	data->isBone = false;
	//한개의 매쉬가 만들어졌다

	//스킨 오브젝트라면 본의 이름과 인덱스정보를 넘겨준다
	if (data->isSkinnde == true)
	{
		//스킨매쉬에 본의 이름과 인덱스정보를 넘겨줌
		std::map<string, Bone*>::iterator Boneit = BoneListData.begin();
		for (Boneit; Boneit != BoneListData.end(); Boneit++)
		{
			if (Boneit->second == nullptr)
			{
				continue;
			}
	
			data->BoneNumberList.insert({ Boneit->first,Boneit->second->index });
		}
	}

	//쓰던값들 다버림
	DataClear();

	return data;
}

void FBXParser::Split(FbxMesh* mesh)
{
	//삼각형의 총카운터
	int TrCount = mesh->GetPolygonCount();
	//버택스의 총카운터
	int  v_count = mesh->GetControlPointsCount();


	//일단 무조건 개수는 맞춰줘야 에러가 안남..
	data->vertex_nomal.resize(v_count);
	data->vertex_tangentU.resize(v_count);
	data->vertex_tex.resize(v_count);
	data->vertex_bonIndex.resize(v_count);
	
	//본정보가있으면 개수를 맞춰줌
	if (weights.size() != 0 || bonIndex.size() != 0)
	{
		data->vertex_weights.resize(v_count);
		data->vertex_bonIndex.resize(v_count);
		for (int i = 0; i<  data->vertex_bonIndex.size(); i++)
		{
			data->vertex_bonIndex[i].resize(4);
		}
	}
	

	//스플릿을 해야할 버텍스를 만나면 이인덱스로 넣어준다
	int pushIndex = v_count;

	int vertexCount = 0;
	for (int i = 0; i < TrCount; i++) //삼각형의 개수
	{
		for (int j = 0; j < 3; j++) //삼각형은 3개의 정점으로 구성
		{
			int controlPointIndex = mesh->GetPolygonVertex(i, j); // 제어점 인덱스를 가져온다.

			//현재 조사할 버텍스의 인덱스
			int NowIndex = Vertexindex[vertexCount];

			//여기부분은 ASE파싱할때와 같음
			//원본테이블과 현재 조사하고있는 버텍스와 비교


			//값을비교할 테이블 
			XMFLOAT3 table_Nomal = data->vertex_nomal[NowIndex];
			XMFLOAT2 table_UV = data->vertex_tex[NowIndex];
			XMFLOAT3 m_pos = data->vertex_pos[NowIndex];



			//이번에 조사할 uv
			int uvIndex = mesh->GetTextureUVIndex(i, j);
			XMFLOAT2 m_uv = GetUV(mesh, controlPointIndex, uvIndex);
			//이번에 조사할 노말
			XMFLOAT3 m_nomal = GetNomal(mesh, controlPointIndex, vertexCount);

			//최종테이블에 값들이 모두 0이다? 그건 아직 값이 안들어간것
			if (table_Nomal.x == 0 && table_Nomal.y == 0 && table_Nomal.z == 0 &&
				table_UV.x == 0 && table_UV.y == 0)
			{

				//값이 없으면 넣어주면됨
				data->vertex_pos[NowIndex] = m_pos;
				data->vertex_nomal[NowIndex] = m_nomal;
				data->vertex_tex[NowIndex] = m_uv;

				//본의 정보도 같이
				if (weights.size() != 0)
				{
					data->vertex_weights[NowIndex] = weights[NowIndex];

					data->vertex_bonIndex[NowIndex] = bonIndex[NowIndex];
					for (int i = 0; i < 4; i++)
					{
						data->vertex_bonIndex[NowIndex][i] = bonIndex[NowIndex][i];
					}

				}

			}
			else if (table_Nomal.x != m_nomal.x || table_Nomal.y != m_nomal.y || table_Nomal.z != m_nomal.z ||
				table_UV.x != m_uv.x || table_UV.y != m_uv.y)
			{
				//만약 값을비교하다 어느 하나라도 값이 다르다?
				//그러면 새로운 버텍스를 만들어준다


				data->vertex_nomal.push_back(XMFLOAT3());
				data->vertex_pos.push_back(XMFLOAT3());
				data->vertex_tex.push_back(XMFLOAT2());
				data->vertex_weights.push_back(XMFLOAT4());

				


				//값을 새롭게 밀어 넣어준다
				data->vertex_nomal[pushIndex] = m_nomal;
				data->vertex_pos[pushIndex] = m_pos;
				data->vertex_tex[pushIndex] = m_uv;

				//마찬가지로 본정보도 같이
				if (weights.size() != 0)
				{
					data->vertex_bonIndex.push_back(vector<UINT>());
					data->vertex_bonIndex[pushIndex].resize(4);

					data->vertex_weights[pushIndex] = weights[NowIndex];
					data->vertex_bonIndex[pushIndex] = bonIndex[NowIndex];
					for (int i = 0; i < 4; i++)
					{
						data->vertex_bonIndex[pushIndex][i] = bonIndex[NowIndex][i];
					}
				}


				//버택스의 인덱스도 변경
				Vertexindex[vertexCount] = pushIndex;


				//다넣어줬으니 다음을 위해 인덱스 증가
				pushIndex++;
			}
			vertexCount++;
		}
	}
	//CreateTangentTable(mesh);

	//인덱스의 순서를 y축 z축바꾸듯이 바꿔줌
	for (int i = 0; i < TrCount * 3; i += 3)
	{
		data->vertex_index.push_back(Vertexindex[i + 0]);
		data->vertex_index.push_back(Vertexindex[i + 2]);
		data->vertex_index.push_back(Vertexindex[i + 1]);
	}


	data->vcount = data->vertex_pos.size();
}

XMFLOAT2 FBXParser::GetUV(FbxMesh* mesh, int verindex, int uvindex)
{
	if (mesh->GetElementUVCount() < 1) { return XMFLOAT2(0, 0); }

	FbxGeometryElementUV* vertexUV = mesh->GetElementUV();
	string name = vertexUV->GetName();
	//int uvIndex = mesh->GetTextureUVIndex(pi, vi);

	XMFLOAT2 fbxUV = { 0,0 };


	switch (vertexUV->GetMappingMode()) // 매핑 모드 
	{
	case FbxGeometryElement::eByControlPoint: // control point mapping 
	{
		switch (vertexUV->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		{
			fbxUV.x = vertexUV->GetDirectArray().GetAt(verindex)[0];
			fbxUV.y = 1.0f - vertexUV->GetDirectArray().GetAt(verindex)[1];
		}
		break;
		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexUV->GetIndexArray().GetAt(verindex); // 인덱스를 얻어온다. 
			fbxUV.x = vertexUV->GetDirectArray().GetAt(index)[0];
			fbxUV.y = 1.0f - vertexUV->GetDirectArray().GetAt(index)[1];
		}
		break;
		}
	}
	break;

	case FbxGeometryElement::eByPolygonVertex:
	{
		switch (vertexUV->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
		case FbxGeometryElement::eIndexToDirect:
		{
			fbxUV.x = vertexUV->GetDirectArray().GetAt(uvindex)[0];
			fbxUV.y = 1.0f - vertexUV->GetDirectArray().GetAt(uvindex)[1];
		}
		break;

		}
	}
	break;

	}

	return fbxUV;
}

XMFLOAT3 FBXParser::GetBiNomal(FbxMesh* mesh, int pointindex, int vertexCount)
{
	FbxGeometryElementBinormal* vertexBinormal = mesh->GetElementBinormal(0);
	int type = vertexBinormal->GetMappingMode();


	XMFLOAT3 nom = { 0,0,0 };

	switch (type)
	{
	case FbxGeometryElementBinormal::eByControlPoint: //정점마다 하나의 맵핑 좌표가있음

		//한번더나뉨
		switch (vertexBinormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
			nom.x = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(pointindex).mData[0]);
			nom.y = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(pointindex).mData[2]);
			nom.z = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(pointindex).mData[1]);
			break;
		case FbxGeometryElement::eIndexToDirect:
			// 인덱스를 얻어온다.
			int index = vertexBinormal->GetIndexArray().GetAt(pointindex);
			nom.x = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[0]);
			nom.y = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[2]);
			nom.z = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[1]);
			break;
		}
		break;


	case FbxGeometryElementBinormal::eByPolygonVertex: //각꼭지점마다 하나의 맵핑좌표가있음

		//한번더...
		switch (vertexBinormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
			nom.x = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(vertexCount).mData[0]);
			nom.y = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(vertexCount).mData[2]);
			nom.z = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(vertexCount).mData[1]);
			break;
		case FbxGeometryElement::eIndexToDirect:
			int index = vertexBinormal->GetIndexArray().GetAt(vertexCount); // 인덱스를 얻어온다.
			nom.x = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[0]);
			nom.y = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[2]);
			nom.z = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[1]);
			break;
		}
		break;
	}

	return XMFLOAT3();
}

XMFLOAT3 FBXParser::GetNomal(FbxMesh* mesh, int pointindex, int vertexCount)
{
	//if (mesh->GetElementNormalCount() < 1){return XMFLOAT3(0, 0, 0);}


	FbxGeometryElementNormal* vertexnormal = mesh->GetElementNormal(0);
	int type = vertexnormal->GetMappingMode();


	XMFLOAT3 nom = { 0,0,0 };

	switch (type)
	{
	case FbxGeometryElementNormal::eByControlPoint: //정점마다 하나의 맵핑 좌표가있음

		//한번더나뉨
		switch (vertexnormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
			nom.x = static_cast<float>(vertexnormal->GetDirectArray().GetAt(pointindex).mData[0]);
			nom.y = static_cast<float>(vertexnormal->GetDirectArray().GetAt(pointindex).mData[2]);
			nom.z = static_cast<float>(vertexnormal->GetDirectArray().GetAt(pointindex).mData[1]);
			break;
		case FbxGeometryElement::eIndexToDirect:
			// 인덱스를 얻어온다.
			int index = vertexnormal->GetIndexArray().GetAt(pointindex);
			nom.x = static_cast<float>(vertexnormal->GetDirectArray().GetAt(index).mData[0]);
			nom.y = static_cast<float>(vertexnormal->GetDirectArray().GetAt(index).mData[2]);
			nom.z = static_cast<float>(vertexnormal->GetDirectArray().GetAt(index).mData[1]);
			break;
		}
		break;


	case FbxGeometryElementNormal::eByPolygonVertex: //각꼭지점마다 하나의 맵핑좌표가있음

		//한번더...
		switch (vertexnormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
			nom.x = static_cast<float>(vertexnormal->GetDirectArray().GetAt(vertexCount).mData[0]);
			nom.y = static_cast<float>(vertexnormal->GetDirectArray().GetAt(vertexCount).mData[2]);
			nom.z = static_cast<float>(vertexnormal->GetDirectArray().GetAt(vertexCount).mData[1]);
			break;
		case FbxGeometryElement::eIndexToDirect:
			int index = vertexnormal->GetIndexArray().GetAt(vertexCount); // 인덱스를 얻어온다.
			nom.x = static_cast<float>(vertexnormal->GetDirectArray().GetAt(index).mData[0]);
			nom.y = static_cast<float>(vertexnormal->GetDirectArray().GetAt(index).mData[2]);
			nom.z = static_cast<float>(vertexnormal->GetDirectArray().GetAt(index).mData[1]);
			break;
		}
		break;
	}

	return nom;
}

void FBXParser::SceneSetting(FbxScene* Scene)
{
	//m_SceneInfo = FbxDocumentInfo::Create(FBX_manager, "Scene");
	//m_SceneInfo->mTitle = "Test scene";
	//m_SceneInfo->mSubject = "Test for fbx Export Scene.";
	//m_SceneInfo->mAuthor = "Fbx_Expor_Test.";
	//m_SceneInfo->mRevision = "rever. 0.1";
	//m_SceneInfo->mKeywords = "Fbx_Test";
	//m_SceneInfo->mComment = "no particular comments required.";
	//
	//Scene->SetSceneInfo(m_SceneInfo);

	// 좌표축을 가져온다
	FbxAxisSystem sceneAxisSystem = Scene->GetGlobalSettings().GetAxisSystem();

	// 씬 내의 좌표축을 바꾼다 - 더 연구가필요 일단은 안하면 그냥 나옴
	//sceneAxisSystem = FBX_Scene->GetGlobalSettings().GetAxisSystem();
	//CalAxisMaxToDirectX(sceneAxis);

	//FbxAxisSystem::DirectX.ConvertScene(Scene);

	// 씬 내에서 삼각형화 할 수 있는 모든 노드를 삼각형화 시킨다
	// 맥스 안에서 Editable poly 상태라면 이 작업을 안해야 한다... 그래야 맥스와 동일한 수치가 나온다.
	FbxGeometryConverter geometryConverter(FBX_manager);
	geometryConverter.Triangulate(Scene, true, true);

}

void FBXParser::GetVerTex(FbxMesh* mesh)
{

	//버택스 정보를 읽어온다
	unsigned int count = mesh->GetControlPointsCount();
	//Vertex* vertex = new Vertex[count];

	//data->vertex_pos.resize(count);
	for (unsigned int i = 0; i < count; i++)
	{
		XMFLOAT3 pos;
		pos.x = static_cast<float>(mesh->GetControlPointAt(i).mData[0]);	// x
		pos.y = static_cast<float>(mesh->GetControlPointAt(i).mData[2]);	// y
		pos.z = static_cast<float>(mesh->GetControlPointAt(i).mData[1]);	// z
		position.push_back(pos);
	}
}

void FBXParser::GetAnimation()
{
	float framesPerSecond = (float)FbxTime::GetFrameRate(m_Scene->GetGlobalSettings().GetTimeMode());
	int numStacks = m_Scene->GetSrcObjectCount<FbxAnimStack>();

	//애니메이션의 수만큼
	for (int i = 0; i < numStacks; i++)
	{
		FbxAnimStack* m_AnimStack = FbxCast<FbxAnimStack>(m_Scene->GetSrcObject<FbxAnimStack>(0));

		//애니메이션의 정보를 담을 구조체
		AnimeData = new LoadAnimation();

		//애니메이션의 이름
		FbxString AnimeTotalName = m_AnimStack->GetName();
		AnimeData->Name = AnimeTotalName.Buffer();

		FbxTimeSpan LocalTime = m_AnimStack->GetLocalTimeSpan();

		//애니메이션의 시작 시간
		double StartTime = LocalTime.GetStart().GetSecondDouble();

		//애니메이션의 끝 시간
		double StopTime = LocalTime.GetStop().GetSecondDouble();

		//애니메이션의 전체 시간
		double TotalTime = StopTime - StartTime;

		//애니메이션의 총 프레임수
		double TotalFrames = (int)((StopTime - StartTime) * (double)framesPerSecond);

		//0부터 시작하기때문에
		TotalFrames += 1;

		//전체 시간은 = 1프레임의 시간 * 전체 프레임 수
		//한프레임의 시간
		double OneFramesTime = TotalTime / TotalFrames;



		///값을 넣어줌
		//끝 프레임
		AnimeData->EndFrame = (int)TotalFrames;
		//시작 프레임
		AnimeData->StartFrame = StartTime;
		//한프레임의 시간
		AnimeData->OneFrameTime = OneFramesTime;
		AnimeData->OneFrame = 1;

		AnimeData->TotalFrame = TotalFrames;
		//프레임속도
		AnimeData->Speed;
	}

	int num = 0;
}

void FBXParser::CreateAnimation(FbxNode* node)
{
	float framesPerSecond = (float)FbxTime::GetFrameRate(m_Scene->GetGlobalSettings().GetTimeMode());

	FbxAnimEvaluator* pSceneEvaluator = m_Scene->GetAnimationEvaluator();
	static float testTime = 0;


	//애니메이션 데이터
	hsAnimation* AData = new hsAnimation();

	FbxTime currTime;

	//반복문은 시작 프레임부터 마지막 프레임까지 돌꺼다
	int index = 0;
	for (int i = AnimeData->StartFrame; i < AnimeData->EndFrame; i++)
	{
		int StartFrame = AnimeData->StartFrame;

		switch ((int)framesPerSecond)
		{
		case 60:
			currTime.SetFrame(StartFrame + index, FbxTime::eFrames60);
			break;
		case 100:
			currTime.SetFrame(StartFrame + index, FbxTime::eFrames100);
			break;
		default:
			currTime.SetFrame(StartFrame + index, FbxTime::eFrames30);
			break;
		}

		//현재 프레임의 시간
		double NowFrameTime = AnimeData->OneFrameTime * index;

		//현재 프레임시간에 맞는 월드 행렬을 가져온다
		FbxAMatrix currentTransformOffset = pSceneEvaluator->GetNodeLocalTransform(node, currTime);


		AData->m_nodename = node->GetName();
		AData->m_minKeyframe = AnimeData->StartFrame;
		AData->m_minKeyframe = AnimeData->EndFrame;

		XMFLOAT4X4 World = ChangeDirMatrix(currentTransformOffset);
		XMMATRIX DIR_World = XMLoadFloat4x4(&World);

		//이렇게 구한 행렬을 스케일 쿼터니언 포지션으로 나눈다
		XMVECTOR vScale;
		XMVECTOR vQuatRot;
		XMVECTOR vTrans;
		XMMatrixDecompose(&vScale, &vQuatRot, &vTrans, DIR_World);


		//각각의 값들을 넣어줌
		CAnimation_pos* pos = new CAnimation_pos;
		CAnimation_rot* rot = new CAnimation_rot;
		CAnimation_scl* scl = new CAnimation_scl;

		//이건 원래 없어도됨.. ASE파일과 포멧을 맞추려고 넣어줌
		//ASE에서는 키프레임만넘어와서 몇프레임마다 보간을 할것인지 정해주는것
		//현재는 한프레임당 시간을 넣어줌
		testTime = i;


		pos->m_time = testTime;
		pos->m_pos = vTrans;

		rot->m_time = testTime;
		rot->m_rotQT_accumulation = vQuatRot;

		scl->m_time = testTime;
		scl->m_scale = vScale;


		AData->m_position.push_back(pos);
		AData->m_rotation.push_back(rot);
		AData->m_scale.push_back(scl);


		index++;
	}
	testTime = 0;
	AnimeData->Anime.push_back(AData);
}

void FBXParser::GetTexturepath(string fileName, FbxScene* scene)
{
	//리소스가 한 폴더 안에 있다는 가정
	FbxVideo* vid = scene->GetVideo(0);
	FbxSurfaceMaterial* pmat = scene->GetMaterial(0);

	if (vid != nullptr && pmat != nullptr)
	{
		FbxString name = vid->GetFileName();		// fbx 안에 저장된 파일 경로

		string tempname;							//	
		tempname = name;

		const TCHAR* pname = ConvertStringToTCHAR(tempname);

		TCHAR tempfname[256];
		//GetFileName(pname, tempfname);				// 실제 파일명 받아오기
		//
		//const TCHAR* filepath = ConvertStringToTCHAR(filename);
		//
		//TCHAR texfilename[256] = L"";				// 텍스쳐 로드할 경로
		//GetPath(filepath, texfilename);
		//
		//string finalpath = ConvertTCharToString(texfilename);
		//
		//pmodel->m_TexPath = finalpath;
	}
}

void FBXParser::FindNodeType_AndStart(int type, FbxNode* node)
{
	//필요 없는 타입도 있을것이다 그래도 알아보기쉽게 그냥다넣음


	int num = 0;
	switch (type)
	{
	case (int)FbxNodeAttribute::eUnknown:
		break;
	case FbxNodeAttribute::eNull:
		num = 0;
		break;
	case FbxNodeAttribute::eMarker:
		num = 0;
		break;
	case FbxNodeAttribute::eSkeleton:
	{
		CreateSkeleton(node);

		CreateAnimation(node);
		break;
	}
	case FbxNodeAttribute::eMesh:
	{
		//매쉬는  생성
		LoadData* temp = CreateMesh(node);

		dataList->meshList.push_back(temp);
		break;
	}
	case FbxNodeAttribute::ePatch:
		num = 0;
		break;
	case FbxNodeAttribute::eCamera:
		num = 0;
		break;
	case FbxNodeAttribute::eCameraStereo:
		num = 0;
		break;
	case FbxNodeAttribute::eCameraSwitcher:
		num = 0;
		break;
	case FbxNodeAttribute::eLight:
		num = 0;
		break;
	case FbxNodeAttribute::eOpticalReference:
		num = 0;
		break;
	case FbxNodeAttribute::eOpticalMarker:
		num = 0;
		break;
	case FbxNodeAttribute::eNurbsCurve:
		num = 0;
		break;
	case FbxNodeAttribute::eTrimNurbsSurface:
		num = 0;
		break;
	case FbxNodeAttribute::eBoundary:
		num = 0;
		break;
	case FbxNodeAttribute::eNurbsSurface:
		num = 0;
		break;
	case FbxNodeAttribute::eShape:
		num = 0;
		break;
	case FbxNodeAttribute::eLODGroup:
		num = 0;
		break;
	case FbxNodeAttribute::eSubDiv:
		num = 0;
		break;
	case FbxNodeAttribute::eCachedEffect:
		num = 0;
		break;
	case FbxNodeAttribute::eLine:
		num = 0;
		break;
	}

}

void FBXParser::AnimeReset()
{
	AnimeData = nullptr;
}

XMFLOAT4X4 FBXParser::ChangeDirMatrix(FbxAMatrix fbxTM)
{
	//FBX는 전용 매트릭스를 쓰고있어서 변경시켜줘야할듯
	//이것역시 좌표축이다르다 좌표축을 변경시켜주자 ㅎㅎ
	XMFLOAT4X4 TM;
	TM._11 = fbxTM.mData[0].mData[0];		TM._13 = fbxTM.mData[0].mData[1];	TM._12 = fbxTM.mData[0].mData[2];	TM._14 = fbxTM.mData[0].mData[3];
	TM._21 = fbxTM.mData[2].mData[0];		TM._23 = fbxTM.mData[2].mData[1];	TM._22 = fbxTM.mData[2].mData[2];	TM._24 = fbxTM.mData[2].mData[3];
	TM._31 = fbxTM.mData[1].mData[0];		TM._33 = fbxTM.mData[1].mData[1];	TM._32 = fbxTM.mData[1].mData[2];	TM._34 = fbxTM.mData[1].mData[3];
	TM._41 = fbxTM.mData[3].mData[0];		TM._43 = fbxTM.mData[3].mData[1];	TM._42 = fbxTM.mData[3].mData[2];	TM._44 = fbxTM.mData[3].mData[3];

	return TM;
}

XMMATRIX FBXParser::CheckNegative(XMFLOAT4X4& TM)
{
	XMMATRIX temp = XMLoadFloat4x4(&TM);


	Vector3 X = { TM._11,TM._12,TM._13 };
	Vector3 Y = { TM._21,TM._22,TM._23 };
	Vector3 Z = { TM._31,TM._32,TM._33 };

	///외적
	Vector3 Cro = X.Cross(Y);
	///내적
	float dot = Z.Dot(Cro);

	if (dot < 0)
	{
		//스케일값을 바꾸고 싶어서 모두 나눈다
		XMVECTOR _P;
		XMVECTOR _R;
		XMVECTOR _S;
		XMMatrixDecompose(&_S, &_R, &_P, temp);

		XMFLOAT3 V_S;
		XMStoreFloat3(&V_S, _S);
		V_S.x *= -1;
		V_S.y *= -1;
		V_S.z *= -1;
		data->W_TM = XMMatrixScaling(V_S.x, V_S.y, V_S.z) * XMMatrixRotationQuaternion(_R) * XMMatrixTranslationFromVector(_P);

		///노말값도 변경
		//int FCount = m_OneMesh->m_mesh_numfaces;

		int size = data->vertex_nomal.size();
		for (int i = 0; i < size; i++)
		{
			data->vertex_nomal[i].x *= -1;
			data->vertex_nomal[i].y *= -1;
			data->vertex_nomal[i].z *= -1;
		}
		return temp;
	}
	return temp;
}

TCHAR* FBXParser::ConvertStringToTCHAR(string& s)
{
	string tstr;
	const char* all = s.c_str();
	int len = 1 + strlen(all);
	wchar_t* t = new wchar_t[len];
	if (NULL == t) throw std::bad_alloc();
	mbstowcs(t, all, len);
	return (TCHAR*)t;
}

std::string* FBXParser::ConvertTCharToString(const TCHAR* ptsz)
{
	int len = wcslen((wchar_t*)ptsz);
	char* psz = new char[2 * len + 1];
	wcstombs(psz, (wchar_t*)ptsz, 2 * len + 1);
	std::string s = psz;
	delete[] psz;
	return &s;
}

void FBXParser::GetPath(const TCHAR* FullPathName, TCHAR* Path)
{
	std::wstring path;		// 경로 저장용 임시 변수
	path = FullPathName;
	size_t pos = path.find_last_of('\\');
	if (pos <= 0)
	{
		pos = path.find_last_of('/');
	}
	_tcsncpy(Path, FullPathName, pos + 1);
}

void FBXParser::FindNode(FbxNode* nextNode)
{
	//재귀 구조 노드찾기
	//나의 자식의 숫자를 가져옴
	int childCount = nextNode->GetChildCount();
	if (childCount == 0)
	{
		return;
	}
	else
	{
		//자식객체들의 타입을 확인하고 그것에맞는것을 실행
		for (int i = 0; i < childCount; i++)
		{
			//자식객체를 가져옴
			FbxNode* chlidNode = nextNode->GetChild(i);

			//자식객체의 타입
			FbxNodeAttribute* temp = chlidNode->GetNodeAttribute();
			int type = (int)temp->GetAttributeType();

			//타입을 찾고 맞는 함수를 실행시킴
			FindNodeType_AndStart(type, chlidNode);

			//현재노드는 할꺼다헀으니 자식의 노드로
			FindNode(nextNode->GetChild(i));
		}
	}

	return;
}

void FBXParser::DataClear()
{
	//BoneListData.clear();
	position.clear();
	nomal.clear();
	uv.clear();

	Vertexindex.clear();
	bonIndex.clear();
	weights.clear();
}

void FBXParser::SetLoadPath(string path)
{
	FilePath = path;
}

bool FBXParser::FindBone(string name)
{
	map<string, Bone*>::iterator it = BoneListData.begin();
	for (it; it != BoneListData.end(); it++)
	{
		//만약 본리스트중에 이름이 같은 본을 찾았다면
		if (it->first == name)
		{
			return true;
		}
	}

	return false;
}

LoadAnimation* FBXParser::GetOneAnimation()
{
	if (AnimeData == nullptr)
	{
		return nullptr;
	}
	else
	{
		return AnimeData;
	}
}















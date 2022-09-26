
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
	//��ü ����
	FBX_manager = FbxManager::Create();


	//�� �Ѱ� ���� ���� ���ÿ� �������� ������ �ε��ϰ������ �� ������ ���� ����
	m_Scene = FbxScene::Create(FBX_manager, "scene");


	//FBX������ ��ü ����
	ios = FbxIOSettings::Create(FBX_manager, IOSROOT);
	FBX_manager->SetIOSettings(ios);


	//�Ž� ����
	FBX_Mesh = FbxMesh::Create(m_Scene, "");

	//��ũ�� ����
	importer = FbxImporter::Create(FBX_manager, "");
}

void FBXParser::Delete()
{
	//������ �������� ����
	//FBX_Mesh->Destroy();
	//FBX_Node->Destroy();
	//FBX_Scene->Destroy();
	//FBX_manager->Destroy();
}

LoadMesh* FBXParser::Load_MODELS_FBX(string FileName, bool LoadAnimation)
{
	string path = FilePath + FileName;

	//������ ����
	bool status = importer->Initialize(path.c_str(), -1, FBX_manager->GetIOSettings());

	importer->Import(m_Scene);

	SceneSetting(m_Scene);

	//���� �ҷ����� ���п���
	if (!status) { return nullptr; }

	//�ִϸ��̼� ������ ���ð�����
	OnlyAnimation = LoadAnimation;

	//���� ��Ʈ ��带 �����´�
	FBX_Node = m_Scene->GetRootNode();


	//�Ž�����Ʈ�� ���� ��
	dataList = nullptr;
	dataList = new LoadMesh();


	//�ִϸ��̼���ü���� ������ ������
	//�Ž������� �ִϸ��̼������� �Ž��� ����鼭
	GetAnimation();

	//��͸� ���鼭 �Ž��� ã�� �ִϸ��̼����ϰ� ���Ŵ���
	FindNode(FBX_Node);



	SkinListData.clear();
	BoneListData.clear();
	//��
	return dataList;
}

void FBXParser::CreateSkeleton(FbxNode* node)
{
	if (OnlyAnimation == true) { return; }
	//���̷��� ������ã�´�
	string name = node->GetName();

	//���� ����
	LoadData* bone = new LoadData();

	//���� �̸� �ֱ�
	bone->isBone = true;
	bone->MyName = node->GetName();

	//�θ������� �θ��� �̸��� �־���
	FbxNode* Parent = node->GetParent();
	if (Parent != nullptr)
	{
		FbxString ParentName = Parent->GetName();

		//����¿��� �θ𰡾��� �ֻ��� ����� �̸��� �޶���...
		if (ParentName == "RootNode" || ParentName == "root" || ParentName == "Armature")
		{
			//�ֻ���������Ʈ�� �θ𰡾��ٴ¶����� �̰���
			bone->ParentName = "TOP_OBJ";
		}
		else
		{
			bone->ParentName = ParentName;
		}
	}

	//������ Tm���� ������
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

	//�����ð��� ������������
	int DeformerCount = mesh->GetDeformerCount();
	for (int i = 0; i < DeformerCount; i++)
	{
		//������ �����̰� ����� ��Ų�̴� 
		FbxSkin* skin = reinterpret_cast<FbxSkin*>(mesh->GetDeformer(i, FbxDeformer::eSkin));

		//��Ų�� ����Ұ��̱⶧���� ��Ų���̸´��� Ȯ��
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


		//�̽�Ų�� ������� ������
		int ClusterCount = skin->GetClusterCount();

		for (int clusterIndex = 0; clusterIndex < ClusterCount; clusterIndex++)
		{

			FbxCluster* cluster = skin->GetCluster(clusterIndex);

			//���̵����������� �������
			if (cluster == nullptr) { continue; }

			//���� �����ϰ��ִ� ���� �̸��� �����´�
			string BoneName = cluster->GetLink()->GetName();


			//���� ���ٸ� ���θ���
			if (FindBone(BoneName) == false)
			{
				Bone* bone = new Bone;
				bone->MyName = BoneName;
				bone->index = clusterIndex;

				//���� TM�� �ʿ��ϴ�
				FbxAMatrix transformMatrix;
				FbxAMatrix transformLinkMatrix;

				//���� ����
				cluster->GetTransformMatrix(transformMatrix);
				bone->TransfromTm = ChangeDirMatrix(transformMatrix);

				//���� ����� ��Ų�� ��Ʈ����
				cluster->GetTransformLinkMatrix(transformLinkMatrix);
				bone->ParentTM = ChangeDirMatrix(transformLinkMatrix);

				////���� ������������
				BoneListData.insert({ bone->MyName,bone });
			}

			///�̹��� ���� ������������ �ε��������� �������

			//�̺��� ������ �ְ��ִ� ���ý��� ī����
			int WeightMaxCount = cluster->GetControlPointIndicesCount();

			//�⺻�� ����
			vector<int>testindex;
			vector<double>testWeight;

			//�̺��� ����Ǿ��ִ� ��� ���ؽ��� �ε����� ������
			for (int k = 0; k < WeightMaxCount; k++)
			{
				int testIndex = cluster->GetControlPointIndices()[k];
				float testWeight = cluster->GetControlPointWeights()[k];

				for (int m = 0; m < 4; m++)
				{
					//���� ��������ʴٸ�
					if (bonIndex[testIndex][m] == 0)
					{
						//���� �ε����� �Ѱ���
						bonIndex[testIndex][m] = clusterIndex;

						//������ ������ �Ѱ���
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
	//���̽� ������ �ε����� ������
	int count = mesh->GetPolygonCount();
	for (int i = 0; i < count; i++) //�ﰢ���� ����
	{
		for (int j = 0; j < 3; j++) //�ﰢ���� 3���� �������� ����
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
		//���̽��Ѱ��� ���ؽ� �ε������� ������
		int index0 = mesh->GetPolygonVertex(TrgCount, 0);
		int index1 = mesh->GetPolygonVertex(TrgCount, 1);
		int index2 = mesh->GetPolygonVertex(TrgCount, 2);
		
		//���̽��Ѱ��� �ش�Ǵ� ��ġ������ ����
		Vector3 p0 = data->vertex_pos[index0];
		Vector3 p1 = data->vertex_pos[index1];
		Vector3 p2 = data->vertex_pos[index2];
		
		//���������� ���͸�����
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
	//�����͸� �����ҸŽ�
	data = new LoadData;

	//�ϴ� ��� �д´�

	FbxMesh* mesh = node->GetMesh();
	mesh->GetDeformerCount();

	int  v_count = mesh->GetControlPointsCount();

	//�ε��ϱ��� Ȥ�� ������ �ε��ϴ� �����Ͱ� ���������� �𸣴�
	//Ŭ���� �ѹ�����
	DataClear();

	//���ø��� �ϱ��� ������ ���̺�
	CreateWeightTable(node, data);
	//���ø� �ϱ��� ���ؽ� ���̺� 
	CreateVertexPosTable(mesh);
	//���ø� �ϱ��� �ε��� ���̺�
	CreateVertexIndexTable(mesh);

	//UV�� NOMAL �� ���ø� �ϸ鼭 �����ð���

	//���� ������������ ���ø�
	Split(mesh);


	//�Ž��� ������ �ΰ� ������ �߰�


	//���� �̸����־���
	data->MyName = node->GetName();
	FbxNode* ParentNode = node->GetParent();
	if (ParentNode != nullptr)
	{
		string ParentName = ParentNode->GetName();

		//����¿��� �θ𰡾��� �ֻ��� ����� �̸��� �޶���...
		if (ParentName == "RootNode" || ParentName == "root")
		{
			//�ֻ���������Ʈ�� �θ𰡾��ٴ¶����� �̰���
			data->ParentName = "TOP_OBJ";
		}
		else
		{
			data->ParentName = ParentName;
		}
	}

	//������ǥ�� ������ǥ�� �Ѱ���

	//Utilities::GetGeometryTransformation


	//mesh->GetNode()->Ge
	XMFLOAT4X4 V_TM_4x4 = ChangeDirMatrix(node->GetScene()->GetAnimationEvaluator()->GetNodeLocalTransform(node));
	XMFLOAT4X4 W_TM_4x4 = ChangeDirMatrix(node->GetScene()->GetAnimationEvaluator()->GetNodeGlobalTransform(node));


	data->V_TM = CheckNegative(V_TM_4x4);
	data->W_TM = CheckNegative(W_TM_4x4);
	data->isSkinnde = true;
	data->isBone = false;
	//�Ѱ��� �Ž��� ���������

	//��Ų ������Ʈ��� ���� �̸��� �ε��������� �Ѱ��ش�
	if (data->isSkinnde == true)
	{
		//��Ų�Ž��� ���� �̸��� �ε��������� �Ѱ���
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

	//�������� �ٹ���
	DataClear();

	return data;
}

void FBXParser::Split(FbxMesh* mesh)
{
	//�ﰢ���� ��ī����
	int TrCount = mesh->GetPolygonCount();
	//���ý��� ��ī����
	int  v_count = mesh->GetControlPointsCount();


	//�ϴ� ������ ������ ������� ������ �ȳ�..
	data->vertex_nomal.resize(v_count);
	data->vertex_tangentU.resize(v_count);
	data->vertex_tex.resize(v_count);
	data->vertex_bonIndex.resize(v_count);
	
	//�������������� ������ ������
	if (weights.size() != 0 || bonIndex.size() != 0)
	{
		data->vertex_weights.resize(v_count);
		data->vertex_bonIndex.resize(v_count);
		for (int i = 0; i<  data->vertex_bonIndex.size(); i++)
		{
			data->vertex_bonIndex[i].resize(4);
		}
	}
	

	//���ø��� �ؾ��� ���ؽ��� ������ ���ε����� �־��ش�
	int pushIndex = v_count;

	int vertexCount = 0;
	for (int i = 0; i < TrCount; i++) //�ﰢ���� ����
	{
		for (int j = 0; j < 3; j++) //�ﰢ���� 3���� �������� ����
		{
			int controlPointIndex = mesh->GetPolygonVertex(i, j); // ������ �ε����� �����´�.

			//���� ������ ���ؽ��� �ε���
			int NowIndex = Vertexindex[vertexCount];

			//����κ��� ASE�Ľ��Ҷ��� ����
			//�������̺�� ���� �����ϰ��ִ� ���ؽ��� ��


			//�������� ���̺� 
			XMFLOAT3 table_Nomal = data->vertex_nomal[NowIndex];
			XMFLOAT2 table_UV = data->vertex_tex[NowIndex];
			XMFLOAT3 m_pos = data->vertex_pos[NowIndex];



			//�̹��� ������ uv
			int uvIndex = mesh->GetTextureUVIndex(i, j);
			XMFLOAT2 m_uv = GetUV(mesh, controlPointIndex, uvIndex);
			//�̹��� ������ �븻
			XMFLOAT3 m_nomal = GetNomal(mesh, controlPointIndex, vertexCount);

			//�������̺� ������ ��� 0�̴�? �װ� ���� ���� �ȵ���
			if (table_Nomal.x == 0 && table_Nomal.y == 0 && table_Nomal.z == 0 &&
				table_UV.x == 0 && table_UV.y == 0)
			{

				//���� ������ �־��ָ��
				data->vertex_pos[NowIndex] = m_pos;
				data->vertex_nomal[NowIndex] = m_nomal;
				data->vertex_tex[NowIndex] = m_uv;

				//���� ������ ����
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
				//���� �������ϴ� ��� �ϳ��� ���� �ٸ���?
				//�׷��� ���ο� ���ؽ��� ������ش�


				data->vertex_nomal.push_back(XMFLOAT3());
				data->vertex_pos.push_back(XMFLOAT3());
				data->vertex_tex.push_back(XMFLOAT2());
				data->vertex_weights.push_back(XMFLOAT4());

				


				//���� ���Ӱ� �о� �־��ش�
				data->vertex_nomal[pushIndex] = m_nomal;
				data->vertex_pos[pushIndex] = m_pos;
				data->vertex_tex[pushIndex] = m_uv;

				//���������� �������� ����
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


				//���ý��� �ε����� ����
				Vertexindex[vertexCount] = pushIndex;


				//�ٳ־������� ������ ���� �ε��� ����
				pushIndex++;
			}
			vertexCount++;
		}
	}
	//CreateTangentTable(mesh);

	//�ε����� ������ y�� z��ٲٵ��� �ٲ���
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


	switch (vertexUV->GetMappingMode()) // ���� ��� 
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
			int index = vertexUV->GetIndexArray().GetAt(verindex); // �ε����� ���´�. 
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
	case FbxGeometryElementBinormal::eByControlPoint: //�������� �ϳ��� ���� ��ǥ������

		//�ѹ�������
		switch (vertexBinormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
			nom.x = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(pointindex).mData[0]);
			nom.y = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(pointindex).mData[2]);
			nom.z = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(pointindex).mData[1]);
			break;
		case FbxGeometryElement::eIndexToDirect:
			// �ε����� ���´�.
			int index = vertexBinormal->GetIndexArray().GetAt(pointindex);
			nom.x = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[0]);
			nom.y = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[2]);
			nom.z = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[1]);
			break;
		}
		break;


	case FbxGeometryElementBinormal::eByPolygonVertex: //������������ �ϳ��� ������ǥ������

		//�ѹ���...
		switch (vertexBinormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
			nom.x = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(vertexCount).mData[0]);
			nom.y = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(vertexCount).mData[2]);
			nom.z = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(vertexCount).mData[1]);
			break;
		case FbxGeometryElement::eIndexToDirect:
			int index = vertexBinormal->GetIndexArray().GetAt(vertexCount); // �ε����� ���´�.
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
	case FbxGeometryElementNormal::eByControlPoint: //�������� �ϳ��� ���� ��ǥ������

		//�ѹ�������
		switch (vertexnormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
			nom.x = static_cast<float>(vertexnormal->GetDirectArray().GetAt(pointindex).mData[0]);
			nom.y = static_cast<float>(vertexnormal->GetDirectArray().GetAt(pointindex).mData[2]);
			nom.z = static_cast<float>(vertexnormal->GetDirectArray().GetAt(pointindex).mData[1]);
			break;
		case FbxGeometryElement::eIndexToDirect:
			// �ε����� ���´�.
			int index = vertexnormal->GetIndexArray().GetAt(pointindex);
			nom.x = static_cast<float>(vertexnormal->GetDirectArray().GetAt(index).mData[0]);
			nom.y = static_cast<float>(vertexnormal->GetDirectArray().GetAt(index).mData[2]);
			nom.z = static_cast<float>(vertexnormal->GetDirectArray().GetAt(index).mData[1]);
			break;
		}
		break;


	case FbxGeometryElementNormal::eByPolygonVertex: //������������ �ϳ��� ������ǥ������

		//�ѹ���...
		switch (vertexnormal->GetReferenceMode())
		{
		case FbxGeometryElement::eDirect:
			nom.x = static_cast<float>(vertexnormal->GetDirectArray().GetAt(vertexCount).mData[0]);
			nom.y = static_cast<float>(vertexnormal->GetDirectArray().GetAt(vertexCount).mData[2]);
			nom.z = static_cast<float>(vertexnormal->GetDirectArray().GetAt(vertexCount).mData[1]);
			break;
		case FbxGeometryElement::eIndexToDirect:
			int index = vertexnormal->GetIndexArray().GetAt(vertexCount); // �ε����� ���´�.
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

	// ��ǥ���� �����´�
	FbxAxisSystem sceneAxisSystem = Scene->GetGlobalSettings().GetAxisSystem();

	// �� ���� ��ǥ���� �ٲ۴� - �� �������ʿ� �ϴ��� ���ϸ� �׳� ����
	//sceneAxisSystem = FBX_Scene->GetGlobalSettings().GetAxisSystem();
	//CalAxisMaxToDirectX(sceneAxis);

	//FbxAxisSystem::DirectX.ConvertScene(Scene);

	// �� ������ �ﰢ��ȭ �� �� �ִ� ��� ��带 �ﰢ��ȭ ��Ų��
	// �ƽ� �ȿ��� Editable poly ���¶�� �� �۾��� ���ؾ� �Ѵ�... �׷��� �ƽ��� ������ ��ġ�� ���´�.
	FbxGeometryConverter geometryConverter(FBX_manager);
	geometryConverter.Triangulate(Scene, true, true);

}

void FBXParser::GetVerTex(FbxMesh* mesh)
{

	//���ý� ������ �о�´�
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

	//�ִϸ��̼��� ����ŭ
	for (int i = 0; i < numStacks; i++)
	{
		FbxAnimStack* m_AnimStack = FbxCast<FbxAnimStack>(m_Scene->GetSrcObject<FbxAnimStack>(0));

		//�ִϸ��̼��� ������ ���� ����ü
		AnimeData = new LoadAnimation();

		//�ִϸ��̼��� �̸�
		FbxString AnimeTotalName = m_AnimStack->GetName();
		AnimeData->Name = AnimeTotalName.Buffer();

		FbxTimeSpan LocalTime = m_AnimStack->GetLocalTimeSpan();

		//�ִϸ��̼��� ���� �ð�
		double StartTime = LocalTime.GetStart().GetSecondDouble();

		//�ִϸ��̼��� �� �ð�
		double StopTime = LocalTime.GetStop().GetSecondDouble();

		//�ִϸ��̼��� ��ü �ð�
		double TotalTime = StopTime - StartTime;

		//�ִϸ��̼��� �� �����Ӽ�
		double TotalFrames = (int)((StopTime - StartTime) * (double)framesPerSecond);

		//0���� �����ϱ⶧����
		TotalFrames += 1;

		//��ü �ð��� = 1�������� �ð� * ��ü ������ ��
		//���������� �ð�
		double OneFramesTime = TotalTime / TotalFrames;



		///���� �־���
		//�� ������
		AnimeData->EndFrame = (int)TotalFrames;
		//���� ������
		AnimeData->StartFrame = StartTime;
		//���������� �ð�
		AnimeData->OneFrameTime = OneFramesTime;
		AnimeData->OneFrame = 1;

		AnimeData->TotalFrame = TotalFrames;
		//�����Ӽӵ�
		AnimeData->Speed;
	}

	int num = 0;
}

void FBXParser::CreateAnimation(FbxNode* node)
{
	float framesPerSecond = (float)FbxTime::GetFrameRate(m_Scene->GetGlobalSettings().GetTimeMode());

	FbxAnimEvaluator* pSceneEvaluator = m_Scene->GetAnimationEvaluator();
	static float testTime = 0;


	//�ִϸ��̼� ������
	hsAnimation* AData = new hsAnimation();

	FbxTime currTime;

	//�ݺ����� ���� �����Ӻ��� ������ �����ӱ��� ������
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

		//���� �������� �ð�
		double NowFrameTime = AnimeData->OneFrameTime * index;

		//���� �����ӽð��� �´� ���� ����� �����´�
		FbxAMatrix currentTransformOffset = pSceneEvaluator->GetNodeLocalTransform(node, currTime);


		AData->m_nodename = node->GetName();
		AData->m_minKeyframe = AnimeData->StartFrame;
		AData->m_minKeyframe = AnimeData->EndFrame;

		XMFLOAT4X4 World = ChangeDirMatrix(currentTransformOffset);
		XMMATRIX DIR_World = XMLoadFloat4x4(&World);

		//�̷��� ���� ����� ������ ���ʹϾ� ���������� ������
		XMVECTOR vScale;
		XMVECTOR vQuatRot;
		XMVECTOR vTrans;
		XMMatrixDecompose(&vScale, &vQuatRot, &vTrans, DIR_World);


		//������ ������ �־���
		CAnimation_pos* pos = new CAnimation_pos;
		CAnimation_rot* rot = new CAnimation_rot;
		CAnimation_scl* scl = new CAnimation_scl;

		//�̰� ���� �����.. ASE���ϰ� ������ ���߷��� �־���
		//ASE������ Ű�����Ӹ��Ѿ�ͼ� �������Ӹ��� ������ �Ұ����� �����ִ°�
		//����� �������Ӵ� �ð��� �־���
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
	//���ҽ��� �� ���� �ȿ� �ִٴ� ����
	FbxVideo* vid = scene->GetVideo(0);
	FbxSurfaceMaterial* pmat = scene->GetMaterial(0);

	if (vid != nullptr && pmat != nullptr)
	{
		FbxString name = vid->GetFileName();		// fbx �ȿ� ����� ���� ���

		string tempname;							//	
		tempname = name;

		const TCHAR* pname = ConvertStringToTCHAR(tempname);

		TCHAR tempfname[256];
		//GetFileName(pname, tempfname);				// ���� ���ϸ� �޾ƿ���
		//
		//const TCHAR* filepath = ConvertStringToTCHAR(filename);
		//
		//TCHAR texfilename[256] = L"";				// �ؽ��� �ε��� ���
		//GetPath(filepath, texfilename);
		//
		//string finalpath = ConvertTCharToString(texfilename);
		//
		//pmodel->m_TexPath = finalpath;
	}
}

void FBXParser::FindNodeType_AndStart(int type, FbxNode* node)
{
	//�ʿ� ���� Ÿ�Ե� �������̴� �׷��� �˾ƺ��⽱�� �׳ɴٳ���


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
		//�Ž���  ����
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
	//FBX�� ���� ��Ʈ������ �����־ �����������ҵ�
	//�̰Ϳ��� ��ǥ���̴ٸ��� ��ǥ���� ����������� ����
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

	///����
	Vector3 Cro = X.Cross(Y);
	///����
	float dot = Z.Dot(Cro);

	if (dot < 0)
	{
		//�����ϰ��� �ٲٰ� �; ��� ������
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

		///�븻���� ����
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
	std::wstring path;		// ��� ����� �ӽ� ����
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
	//��� ���� ���ã��
	//���� �ڽ��� ���ڸ� ������
	int childCount = nextNode->GetChildCount();
	if (childCount == 0)
	{
		return;
	}
	else
	{
		//�ڽİ�ü���� Ÿ���� Ȯ���ϰ� �װͿ��´°��� ����
		for (int i = 0; i < childCount; i++)
		{
			//�ڽİ�ü�� ������
			FbxNode* chlidNode = nextNode->GetChild(i);

			//�ڽİ�ü�� Ÿ��
			FbxNodeAttribute* temp = chlidNode->GetNodeAttribute();
			int type = (int)temp->GetAttributeType();

			//Ÿ���� ã�� �´� �Լ��� �����Ŵ
			FindNodeType_AndStart(type, chlidNode);

			//������� �Ҳ��������� �ڽ��� ����
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
		//���� ������Ʈ�߿� �̸��� ���� ���� ã�Ҵٸ�
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















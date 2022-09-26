#include "EnginePCH.h"


CASEParser::CASEParser()
{
	m_materialcount = 0;
	m_parsingmode = eNone;
}

CASEParser::~CASEParser()
{
	delete m_lexer;
}

bool CASEParser::Init()
{
	m_lexer = new ASE::CASELexer;

	return TRUE;
}
//---------------------------------------------------------------------------------------------------
// �ε��Ѵ�.
// �̰��� �����ٸ� ������ ���������� ��� �����͸� �о �� �־�� �Ѵ�.
//
// Parsing�� ����:
// �׻� ������ �������� parsing�̶�� ���� �����Ͱ� ���� �԰ݿ� �°� �� �ִٴ� ���� ������ �Ѵ�.
// ������, ���� ���ο� ������ �ٲ�� �� �ִ°��� �����ϴٴ��� �ϴ� ��Ģ�� �����
// �˻��ϴ� ��ƾ�� ���������� ����. �ϴ��� ������� ���� �Ծ����� �� �ִٴ°��� ��������.
// -> �׷���, ��� ȣ���� �ϴ� �ϳ��� ū �Լ��� �ذ�ô�.
// -> depth�� �������� �����ۿ� ���� ���� �ڵ带 �־�߰ڴ�
//---------------------------------------------------------------------------------------------------
bool CASEParser::Load(LPSTR p_File)
{
	/// 0) ������ �ε��Ѵ�.
	if (!m_lexer->Open(p_File))
	{
		TRACE("������ ���� �߿� ������ �߻��߽��ϴ�!");
		return FALSE;
	}

	/// 1) �ٷ��̾ƺ����豳���� �Ŀ�Ǯ�� ����Լ� �Ѱ��� ó�� ��!
	Parsing_DivergeRecursiveALL(0);

	return TRUE;
}

/// 1�� ��ȯ

// CScenedata ����
bool CASEParser::TranslateToD3DFormat_scene(ASEParser::Scenedata* pscene)
{
	// �� ����
	*pscene = this->m_scenedata;
	return TRUE;
}

/// <summary>
/// �޽��� ����ȭ�� �� �ش�.
/// �븻��, �ؽ��� ��ǥ�� ���� ���ؽ��� �ø���, ��ø�Ǵ°��� �����ϰ�..
/// </summary>
bool CASEParser::Convert_Optimize(ASEParser::Mesh* pMesh)
{
	// 2021.04.12
	// ���� ���� (���ÿ�)

	return FALSE;
}

bool CASEParser::ConvertAll(ASEParser::Mesh* pMesh)
{
	// ���ؽ����� �ϴ� ��� ����
	for (unsigned int i = 0; i < pMesh->m_meshvertex.size(); i++)
	{
		ASEParser::Vertex* _pVertex = new ASEParser::Vertex;
		_pVertex->m_pos = pMesh->m_meshvertex[i]->m_pos;

		pMesh->m_opt_vertex.push_back(_pVertex);
	}

	// �������� face�� �������� �� �ε����� ã�ƾ� ��
	unsigned int _faceCount = pMesh->m_meshface.size();
	//TRACE("FaceCount : %d", _faceCount);

	for (unsigned int i = 0; i < pMesh->m_meshface.size(); i++)
	{
		//TRACE("FaceNum : %d / %d\n", i, _faceCount);

		for (int j = 0; j < 3; j++)
		{
			ASEParser::Vertex* _nowVertex = pMesh->m_opt_vertex[pMesh->m_meshface[i]->m_vertexindex[j]];

			// face�������� ���ؽ��� �븻�� �־��ش�.
			// (���ÿ�)
			_nowVertex->m_normal.x = 0;
			_nowVertex->m_normal.y = 0;
			_nowVertex->m_normal.z = 0;
		}
	}

	// �ε����� �׳� ����
	pMesh->m_opt_index = new ASEParser::IndexList[pMesh->m_meshface.size()];

	for (unsigned int i = 0; i < pMesh->m_meshface.size(); i++)
	{
		for (int j = 0; j < 3; j++)
		{
			pMesh->m_opt_index[i].index[j] = pMesh->m_meshface[i]->m_vertexindex[j];
		}
	}

	return FALSE;
}

ASEParser::Mesh* CASEParser::GetMesh(int index)
{
	return m_MeshList[index];
}

//----------------------------------------------------------------
// ��� ȣ����� ������ �ϴ� �б� �Լ��̴�.
//
// �ϴ� �а�, ��ū�� ���� �����Ѵ�.
//
// ��͸� ���� ������ { �� ���� ����̴�.
// �����ϴ� ������ '}'�� �����ų� TOKEND_END�� ������ ���̴�.
//
// ���Ҿ�, ��͸� ���� �� ��ū�� �о�, ���� �� �Լ��� � ��������� �����Ѵ�.
//
//----------------------------------------------------------------
void CASEParser::Parsing_DivergeRecursiveALL(int depth)
{
	//----------------------------------------------------------------------
	// ���������� ����
	// ��� �����ٰ� ���� �̻� ������ ���� ���(����)�� üũ�ϱ� ���� ����
	int i = 0;
	// ����Ʈ�� �����͸� �־�� �� �� ���̴� ����Ʈ ī����.
	int listcount = 0;		// �ſ� C����� �ٸ� �ذ�å�� ������?
	int FACENOMALindex = 0;
	// �̰��� �� ��ū�� � �����ΰ��� �Ǻ����ش�.
	// �̰��� ��� ������ �ϰ� ��͸� �����°��� �����ϴ�? -> �߸��� ��͹���̴�?
	LONG nowtoken;

	//----------------------------------------------------------------------

	Matrix TM1, TM2;
	Quaternion tempQT;
	Quaternion prevQT;
	Quaternion resultQT;

	/// �̰��� �ϸ� �� ���� ��ū�� �а�, �� ������ ������ �� �� �ִ�.
	while (nowtoken = m_lexer->GetToken(m_TokenString), nowtoken != TOKEND_BLOCK_END)
	{
		// �ϴ� �� ���� ��ū�� �а�, �װ��� ��ȣ �ݱⰡ �ƴ϶��.
		// �Ѿ�� ��ū�� ���� ó�����ش�.

		static int iv = 0;

		switch (nowtoken)
		{
		case TOKEND_BLOCK_START:

			Parsing_DivergeRecursiveALL(depth++);
			break;

		case TOKENR_HELPER_CLASS:
			break;

			//--------------------
			// 3DSMAX_ASCIIEXPORT
			//--------------------

		case TOKENR_3DSMAX_ASCIIEXPORT:
			m_data_asciiexport = Parsing_NumberLong();
			break;

			//--------------------
			// COMMENT
			//--------------------

		case TOKENR_COMMENT:
			Parsing_String();	// �׳� m_TokenString�� �о������ ���� ��.
			//AfxMessageBox( m_TokenString, NULL, NULL);		/// �ӽ÷� �ڸ�Ʈ�� ����غ���
			break;

			//--------------------
			// SCENE
			//--------------------

		case TOKENR_SCENE:
			//
			break;
		case TOKENR_SCENE_FILENAME:
			m_scenedata.m_filename = Parsing_String();		// �ϰ��� �ִ� �Լ��� ����� ���� String�� Int�� ��������.
			break;
		case TOKENR_SCENE_FIRSTFRAME:
			m_scenedata.m_firstframe = Parsing_NumberLong();
			break;
		case TOKENR_SCENE_LASTFRAME:
			m_scenedata.m_lastframe = Parsing_NumberLong();
			break;
		case TOKENR_SCENE_FRAMESPEED:
			m_scenedata.m_framespeed = Parsing_NumberLong();
			break;
		case TOKENR_SCENE_TICKSPERFRAME:
			m_scenedata.m_ticksperframe = Parsing_NumberLong();
			break;
		case TOKENR_SCENE_MESHFRAMESTEP:
			m_scenedata.m_meshframestep = Parsing_NumberLong();
			break;
		case TOKENR_SCENE_KEYFRAMESTEP:
			m_scenedata.m_keyframestep = Parsing_NumberLong();
			break;
		case TOKENR_SCENE_BACKGROUND_STATIC:
			m_scenedata.m_scene_background_static.x = Parsing_NumberFloat();
			m_scenedata.m_scene_background_static.y = Parsing_NumberFloat();
			m_scenedata.m_scene_background_static.z = Parsing_NumberFloat();
			break;
		case TOKENR_SCENE_AMBIENT_STATIC:
			m_scenedata.m_scene_ambient_static.x = Parsing_NumberFloat();
			m_scenedata.m_scene_ambient_static.y = Parsing_NumberFloat();
			m_scenedata.m_scene_ambient_static.z = Parsing_NumberFloat();
			break;

		case TOKENR_SCENE_ENVMAP:
		{
			// �׳� ���� ������ �о������ }�� ���ö�����
			while (nowtoken = m_lexer->GetToken(m_TokenString), nowtoken != TOKEND_BLOCK_END) {
			}
		}
		break;

		//--------------------
		// MATERIAL_LIST
		//--------------------

		//--------------------
		// GEOMOBJECT
		//--------------------

		case TOKENR_GROUP:
			//	�� ���� �׷� ����. �� ������ �̸��� ��Ʈ������ ������� �ϴµ�.
			break;

		case TOKENR_HELPEROBJECT:
		{
			///���� �ֻ��� ��ü �̰�ü�� parent ������
			///�ϴ��� �׳� �⺻���� �־��ش�
			isMesh = true;
			m_OneMesh = new ASEParser::Mesh;

			ASEParser::Vertex* temp = new ASEParser::Vertex;
			//temp->m_indices = 0;
			temp->m_pos = { 0,0,0 };
			ASEParser::Face* face = new ASEParser::Face();
			face->m_vertexindex[0] = 0;
			face->m_vertexindex[1] = 0;
			face->m_vertexindex[2] = 0;

			m_OneMesh->m_meshvertex.push_back(temp);
			m_OneMesh->m_meshface.push_back(face);
			m_MeshList.push_back(m_OneMesh);


			if (isAnime == true)
			{
				m_list_animation.push_back(m_animation);
				isAnime = false;
			}

		}
		break;
		case TOKENR_GEOMOBJECT:
			isMesh = true;
			if (m_OneMesh == nullptr)
			{
				m_animation = nullptr;
				m_OneMesh = new ASEParser::Mesh;
				//m_MeshList.push_back(m_OneMesh);
			}
			else
			{
				Split();
				m_OneMesh = nullptr;
				m_OneMesh = new ASEParser::Mesh;
				m_MeshList.push_back(m_OneMesh);
			}

			if (isAnime == true)
			{
				m_list_animation.push_back(m_animation);
				isAnime = false;
			}

			/// �� ��ū�� �����ٴ°� ���ο� �޽ð� ����ٴ� ���̴�. ���� ������ mesh�� �ϳ� ����, �� �����͸� ����Ʈ�� �ְ�, m_onemesh�� �� �����͸� ����, �״�� ���� �ɱ�?
			break;

		case TOKENR_NODE_NAME:
			if (isAnime == true)
			{
				m_animation->m_nodename = Parsing_String();
			}
			else if (isMesh == true)
			{
				m_OneMesh->m_nodename = Parsing_String();
			}
			break;
		case TOKENR_NODE_PARENT:
			m_OneMesh->m_nodeparent = Parsing_String();
			///�̿�����Ʈ ������ ����������Ʈ�� �ִ�

			if (m_OneMesh->m_nodename != "")
			{

				m_OneMesh->m_isparentexist = true;
			}

			break;

			/// NODE_TM
		case TOKENR_NODE_TM:

			//m_parsingmode	=	eGeomobject;
			// (NODE_TM���� ���� �� NODE_NAME�� �ѹ� �� ���´�.)
			// (Animation���� ������ �ؾ� �ϱ� ������ �̷��� ��带 �����ش�)

			/// �Դٰ�,
			// ī�޶�� NodeTM�� �ι� ���´�. �ι�°��� ���� �ʴ´�.
			// �ƿ� �� ��Ϳ��� ������ ��Ű�� ������. �߰��� �д°��� �����ؾ� �ϱ� ������...
			//if (m_onemesh->m_camera_isloadTarget) {
			//	'}'�� ���ö����� �����°� ������ ����! �� �̷��� �ȵǳ�..
			// ��ͱ����� �������̴�....

			break;

		case TOKENR_INHERIT_POS:
			m_OneMesh->m_inherit_pos.x = Parsing_NumberFloat();
			m_OneMesh->m_inherit_pos.z = Parsing_NumberFloat();
			m_OneMesh->m_inherit_pos.y = Parsing_NumberFloat();

			// ī�޶�� NodeTM�� �ι� ���´�. �ι�°��� ���� �ʴ´�.
			break;
		case TOKENR_INHERIT_ROT:
			m_OneMesh->m_inherit_rot.x = Parsing_NumberFloat();
			m_OneMesh->m_inherit_rot.z = Parsing_NumberFloat();
			m_OneMesh->m_inherit_rot.y = Parsing_NumberFloat();
			break;
		case TOKENR_INHERIT_SCL:
			m_OneMesh->m_inherit_scl.x = Parsing_NumberFloat();
			m_OneMesh->m_inherit_scl.z = Parsing_NumberFloat();
			m_OneMesh->m_inherit_scl.y = Parsing_NumberFloat();
			break;
		case TOKENR_TM_ROW0:
			m_OneMesh->m_tm_row0.x = Parsing_NumberFloat();
			m_OneMesh->m_tm_row0.z = Parsing_NumberFloat();
			m_OneMesh->m_tm_row0.y = Parsing_NumberFloat();
			break;
		case TOKENR_TM_ROW1:
			m_OneMesh->m_tm_row1.x = Parsing_NumberFloat();
			m_OneMesh->m_tm_row1.z = Parsing_NumberFloat();
			m_OneMesh->m_tm_row1.y = Parsing_NumberFloat();
			break;
		case TOKENR_TM_ROW2:
			m_OneMesh->m_tm_row2.x = Parsing_NumberFloat();
			m_OneMesh->m_tm_row2.z = Parsing_NumberFloat();
			m_OneMesh->m_tm_row2.y = Parsing_NumberFloat();
			break;
		case TOKENR_TM_ROW3:
			m_OneMesh->m_tm_row3.x = Parsing_NumberFloat();
			m_OneMesh->m_tm_row3.z = Parsing_NumberFloat();
			m_OneMesh->m_tm_row3.y = Parsing_NumberFloat();
			break;
		case TOKENR_TM_POS:
			m_OneMesh->m_tm_pos.x = Parsing_NumberFloat();
			m_OneMesh->m_tm_pos.z = Parsing_NumberFloat();
			m_OneMesh->m_tm_pos.y = Parsing_NumberFloat();
			break;
		case TOKENR_TM_ROTAXIS:
			m_OneMesh->m_tm_rotaxis.x = Parsing_NumberFloat();
			m_OneMesh->m_tm_rotaxis.z = Parsing_NumberFloat();
			m_OneMesh->m_tm_rotaxis.y = Parsing_NumberFloat();
			break;
		case TOKENR_TM_ROTANGLE:
			m_OneMesh->m_tm_rotangle = Parsing_NumberFloat();
			break;
		case TOKENR_TM_SCALE:
			m_OneMesh->m_tm_scale.x = Parsing_NumberFloat();
			m_OneMesh->m_tm_scale.z = Parsing_NumberFloat();
			m_OneMesh->m_tm_scale.y = Parsing_NumberFloat();
			break;
		case TOKENR_TM_SCALEAXIS:
			m_OneMesh->m_tm_scaleaxis.x = Parsing_NumberFloat();
			m_OneMesh->m_tm_scaleaxis.z = Parsing_NumberFloat();
			m_OneMesh->m_tm_scaleaxis.y = Parsing_NumberFloat();
			break;
		case TOKENR_TM_SCALEAXISANG:
			m_OneMesh->m_tm_scaleaxisang = Parsing_NumberFloat();
			// ���� ī�޶� ���¿��ٸ� �̹� ��带 ���� ������ ǥ�����ش�.
			break;


			/// MESH
		case TOKENR_MESH:
			///������Ʈ�� 1���� �ƴҼ����ֱ⶧���� 
			///mesh�� ��������� ���ӰԸ���� �ƴϸ� �������� ����Ʈ�� �־��ְ� ���θ����
			break;
		case TOKENR_TIMEVALUE:
			break;
		case TOKENR_MESH_NUMBONE:
			// �̰� �ִٸ� �̰��� Skinned Mesh��� ������ ���´�.
			// ���� �Է�
			break;
		case TOKENR_MESH_NUMSKINWEIGHT:
			break;
		case TOKENR_MESH_NUMVERTEX:
			//���ý� ī����
			m_OneMesh->m_mesh_numvertex = Parsing_NumberInt();
			m_OneMesh->m_meshvertex.resize(m_OneMesh->m_mesh_numvertex);
			break;
		case TOKENR_MESH_NUMFACES:
			//�ε��� ī����
			m_OneMesh->m_mesh_numfaces = Parsing_NumberInt();
			m_OneMesh->m_meshface.resize(m_OneMesh->m_mesh_numfaces);
			break;

			/// MESH_VERTEX_LIST

		case TOKENR_MESH_VERTEX_LIST:
			break;
		case TOKENR_MESH_VERTEX:
		{
			ASEParser::Vertex* temp = new ASEParser::Vertex();
			int index = 1;
			index = Parsing_NumberFloat();
			temp->m_pos = Parsing_NumberVector3();
			m_OneMesh->m_meshvertex[index] = temp;
		}
		break;
		/// Bone
		case TOKENR_SKIN_INITTM:
			break;
		case TOKENR_BONE_LIST:
			break;
		case TOKENR_BONE:
		{
			m_OneMesh->m_is_boneObject = true;
			
			m_OneMesh->m_bone = new ASEParser::Bone();
			m_OneMesh->BoneNumber = Parsing_NumberInt();

			m_OneMesh->m_vector_bone_list.push_back(m_OneMesh->m_bone);
			/// ��� ü���� �� �ְ�, Bone�� �����ϰ� �ִٴ� ���� �̰��� ��Ű�� ������Ʈ��� ���̴�.
			// �� �ϳ��� ���� �ӽ� ������ ����, ���Ϳ� �ְ�
			// Bone�� �ѹ��� �о� ����
		}
		break;
		//�� ������ ���� �̸��� �־�� �Ѵ�. ������ {�� �� �� �� �������Ƿ� �ӽ� ������ �����μ� �����ؾ߰���.
		case TOKENR_BONE_NAME:
			m_OneMesh->BoneName = Parsing_String();
			
			m_OneMesh->m_is_BoneList.insert({m_OneMesh->BoneNumber,m_OneMesh->BoneName });
		case TOKENR_BONE_PROPERTY:
			// �� ���� ABSOLUTE�� ������� �ϴµ�, �� ���� ����.
			break;
			// �������� TM_ROW0~3�� �����µ� ���� ���õ�..

		case TOKENR_MESH_WVERTEXS:
			break;

		case TOKENR_MESH_WEIGHT:
		{
			// ���ؽ� �ϳ��� ������ ���� ����Ʈ�� ����
			m_OneMesh->Weightindex = Parsing_NumberInt();
			weightIndex = 0;
			//m_OneMesh->m_wvertex = new ASEParser::VertexWeight();
			//m_OneMesh->m_wvertex->m_wvertex_number = indexnum;
			//m_OneMesh->m_vector_wvertexs.push_back(m_OneMesh->m_wvertex);
		}
		break;
		case TOKENR_BONE_BLENGING_WEIGHT:
		{
			int num = m_OneMesh->Weightindex;

			int WeightNumber = Parsing_NumberInt();
			float weight = Parsing_NumberFloat();

			switch (weightIndex)
			{
			case 0:
				m_OneMesh->m_meshvertex[num]->m_bw1 = weight;
				m_OneMesh->m_meshvertex[num]->m_indicesPointer[weightIndex] = WeightNumber;
				break;
			case 1:
				m_OneMesh->m_meshvertex[num]->m_bw2 = weight;
				m_OneMesh->m_meshvertex[num]->m_indicesPointer[weightIndex]= WeightNumber;
				break;
			case 2:
				m_OneMesh->m_meshvertex[num]->m_bw3 = weight;
				m_OneMesh->m_meshvertex[num]->m_indicesPointer[weightIndex]= WeightNumber;
				break;
			}
			weightIndex++;


			// ��ü �� �ܰ踦 ���°ž�...
			// ����ġ �Ѱ��� ���� ����Ʈ�� �ִ´�
			/// �� ��....
		}
		break;


		/// MESH_FACE_LIST
		case TOKENR_MESH_FACE_LIST:
		{
			int num = 0;
		}

		break;
		case TOKENR_MESH_FACE:
		{
			ASEParser::Face* temp = new ASEParser::Face();
			// Face�� ��ȣ�ε�...
			int index = Parsing_NumberFloat();

			Parsing_String();
			temp->m_vertexindex[0] = Parsing_NumberFloat();
			Parsing_String();
			temp->m_vertexindex[1] = Parsing_NumberFloat();
			Parsing_String();
			temp->m_vertexindex[2] = Parsing_NumberFloat();
			m_OneMesh->m_meshface[index] = temp;
			// A:�� �а�
			// B:
			// C:

			/// (�ڿ� ������ �� ������ default�� ���� ��ŵ�� ���̴�.)
			/// ......

			// ���Ϳ� �־��ش�.
		}
		break;


		case TOKENR_MESH_NORMALS:
		{
			m_OneMesh->m_mesh_facenormal = new Vector3[m_OneMesh->m_mesh_numfaces];
			m_OneMesh->m_mesh_vertexnormal = new Vector3[m_OneMesh->m_mesh_numvertex];
		}
		break;
		case TOKENR_MESH_FACENORMAL:
		{
			FACENOMALindex = Parsing_NumberInt();
			//Vector3 temp;
			//temp.x = Parsing_NumberFloat();
			//temp.y = Parsing_NumberFloat();
			//temp.z = Parsing_NumberFloat();
			//m_OneMesh->m_meshface[FACENOMALindex]->m_normal = temp;
		}
		break;
		case TOKENR_MESH_VERTEXNORMAL:
		{
			static int count = 0;
			int num = Parsing_NumberInt();
			m_OneMesh->m_meshface[FACENOMALindex]->m_vertexindex[count] = num;

			m_OneMesh->m_meshface[FACENOMALindex]->m_normalvertex[count].x = Parsing_NumberFloat();
			m_OneMesh->m_meshface[FACENOMALindex]->m_normalvertex[count].z = Parsing_NumberFloat();
			m_OneMesh->m_meshface[FACENOMALindex]->m_normalvertex[count].y = Parsing_NumberFloat();
			count++;

			if (count >= 3)
			{
				count = 0;
			}
		}
		break;
		case TOKENR_MESH_NUMTVERTEX:
			m_OneMesh->m_mesh_numtvertex = Parsing_NumberInt();
			break;
		case TOKENR_MESH_TVERTLIST:
			m_OneMesh->m_mesh_tvertex.resize(m_OneMesh->m_mesh_numtvertex);
			break;
		case TOKENR_MESH_TVERT:
		{
			ASEParser::COneTVertex* temp = new ASEParser::COneTVertex();
			int index = Parsing_NumberInt();
			temp->m_u = Parsing_NumberFloat();
			temp->m_v = 1 -Parsing_NumberFloat();
			m_OneMesh->m_mesh_tvertex[index] = temp;
		}
		break;
		case TOKENR_MESH_NUMTVFACES:
			m_OneMesh->m_mesh_tvfaces = Parsing_NumberInt();
			break;
		case TOKENR_MESH_TFACE:
		{
			int index = Parsing_NumberInt();
			int V_index00 = 0;
			int V_index01 = 0;
			int V_index02 = 0;
			V_index00 = Parsing_NumberInt();
			V_index01 = Parsing_NumberInt();
			V_index02 = Parsing_NumberInt();

			m_OneMesh->m_meshface[index]->m_TFace[0] = V_index00;
			m_OneMesh->m_meshface[index]->m_TFace[1] = V_index01;
			m_OneMesh->m_meshface[index]->m_TFace[2] = V_index02;
		}
		break;
		case  TOKENR_CAMERA_ANIMATION:
			break;
		case TOKENR_SHAPEOBJECT:
			///���� �ֻ��� ��ü �̰�ü�� parent ������
			///�ϴ��� �׳� �⺻���� �־��ش�
			if (m_OneMesh == nullptr)
			{
				isMesh = true;
				m_OneMesh = new ASEParser::Mesh;

				ASEParser::Vertex* temp = new ASEParser::Vertex;
				//temp->m_indices = 0;
				temp->m_pos = { 0,0,0 };
				ASEParser::Face* face = new ASEParser::Face();
				face->m_vertexindex[0] = 0;
				face->m_vertexindex[1] = 0;
				face->m_vertexindex[2] = 0;

				m_OneMesh->m_meshvertex.push_back(temp);
				m_OneMesh->m_meshface.push_back(face);
				m_MeshList.push_back(m_OneMesh);
			}
			else
			{
				isMesh = true;
				m_MeshList.push_back(m_OneMesh);
				m_OneMesh = new ASEParser::Mesh;

				ASEParser::Vertex* temp = new ASEParser::Vertex;
				//temp->m_indices = 0;
				temp->m_pos = { 0,0,0 };
				ASEParser::Face* face = new ASEParser::Face();
				face->m_vertexindex[0] = 0;
				face->m_vertexindex[1] = 0;
				face->m_vertexindex[2] = 0;

				m_OneMesh->m_meshvertex.push_back(temp);
				m_OneMesh->m_meshface.push_back(face);
				m_MeshList.push_back(m_OneMesh);
			}


			if (isAnime == true)
			{
				m_list_animation.push_back(m_animation);
				isAnime = false;
			}
			break;
		case TOKENR_TM_ANIMATION:
			m_OneMesh->m_isAnimated = true;
			m_animation = new hsAnimation;
			isAnime = true;
			break;
		case TOKENR_CONTROL_POS_SAMPLE:
		{
			CAnimation_pos* pos = new CAnimation_pos;
			pos->m_time = Parsing_NumberInt();
			pos->m_pos.x = Parsing_NumberFloat();
			pos->m_pos.z = Parsing_NumberFloat();
			pos->m_pos.y = Parsing_NumberFloat();

			//�ϴ� ��ġ������ �ִ´�
			m_animation->m_position.push_back(pos);
		}
		break;

		case TOKENR_CONTROL_ROT_SAMPLE:
		{
			m_OneMesh->m_nodename;
			CAnimation_rot* rot = new CAnimation_rot;
			rot->m_time = 0;
			rot->m_time = Parsing_NumberInt();
			rot->m_rot.x = Parsing_NumberFloat();
			rot->m_rot.z = Parsing_NumberFloat();
			rot->m_rot.y = Parsing_NumberFloat();

			rot->m_angle = Parsing_NumberFloat();

			//�ϴ� ��ġ������ �ִ´�
			m_animation->m_rotation.push_back(rot);
		}
		break;

		case TOKENR_CONTROL_SCALE_SAMPLE:
		{
			CAnimation_scl* scl = new CAnimation_scl;
			scl->m_time = Parsing_NumberInt();
			scl->m_scale.x = Parsing_NumberFloat();
			scl->m_scale.z = Parsing_NumberFloat();
			scl->m_scale.y = Parsing_NumberFloat();

			//�ϴ� ��ġ������ �ִ´�
			m_animation->m_scale.push_back(scl);
		}
		break;

		case TOKEND_END:
			Split();
			//m_MeshList.push_back(m_OneMesh);

			//m_OneMesh = new ASEParser::Mesh;

			// �Ƹ��� �̰� ������ ���� ��Ÿ�������ΰ� ������. while�� Ż���ؾ� �ϴµ�?

			//AfxMessageBox("������ ���� �� �� �����ϴ�!", MB_OK, NULL);
			//TRACE("TRACE: �Ľ���: ������ ���� �ý��ϴ�!\n");
 			return;


			break;
			/// ���� �ƹ��͵� �ش����� ������
		default:


			// �ƹ��͵� ���� �ʴ´�.
			break;

		}	// switch()


		///-----------------------------------------------
		/// ���� �ڵ�.
		i++;
		if (i > 1000000)
		{
			// ������ 1000000���̻��̳� �� ������ ����. (�����Ͱ� 100000���� �ƴ��̻�)
			// ���� 1000000�̻� ���Ҵٸ� Ȯ���� ���� ������ �ִ� ���̹Ƿ�
			//TRACE("������ �鸸�� ���ҽ��ϴ�!");
			return;
		}
		/// ���� �ڵ�.
		///-----------------------------------------------

	}		// while()

	// ������� �Դٸ� while()�� ����ٴ� ���̰� �� ����
	// ��ȣ�� �ݾҴٴ� ���̹Ƿ�
	// �������� (����Լ�)


	return;
}


///----------------------------------------------------------------------
/// parsing�� ���� ������ �Լ���
///----------------------------------------------------------------------

// long�� �о �������ش�.
int CASEParser::Parsing_NumberLong()
{
	LONG			token;
	LONG			tempNumber;

	token = m_lexer->GetToken(m_TokenString);	//ASSERT(token == TOKEND_NUMBER);
	tempNumber = strtoul(m_TokenString, NULL, 10);

	return			tempNumber;
}

// float
float CASEParser::Parsing_NumberFloat()
{
	LONG			token;
	float			tempNumber;

	token = m_lexer->GetToken(m_TokenString);	//ASSERT(token == TOKEND_NUMBER);
	tempNumber = (float)atof(m_TokenString);

	return			tempNumber;
}

// String
LPSTR CASEParser::Parsing_String()
{
	/// ��m_TokenString ( char[255] ) �̱� ������ CString�� ������ ���� ���Ŷ� �����ߴµ�, �������� CString�� �� ������� �� ����. �˾Ƽ� �޾Ƶ��̴µ�?
	m_lexer->GetToken(m_TokenString);

	return m_TokenString;
}

int CASEParser::Parsing_NumberInt() {

	LONG			token;
	int				tempNumber;

	token = m_lexer->GetToken(m_TokenString);	//ASSERT(token == TOKEND_NUMBER);
	tempNumber = (int)atoi(m_TokenString);

	return			tempNumber;
}

// 3���� Float�� ���� �ϳ���
Vector3 CASEParser::Parsing_NumberVector3()
{
	LONG			token;
	Vector3			tempVector3;

	token = m_lexer->GetToken(m_TokenString);
	tempVector3.x = (float)atof(m_TokenString);
	token = m_lexer->GetToken(m_TokenString);
	tempVector3.z = (float)atof(m_TokenString);
	token = m_lexer->GetToken(m_TokenString);
	tempVector3.y = (float)atof(m_TokenString);

	return			tempVector3;		// ����ƽ ������ ���۷������ٴ� �� ������ ����.
}

///--------------------------------------------------
/// ���ο��� ������ ����, ����Ʈ�� �ִ´�
///--------------------------------------------------
// �޽ø� �ϳ� �������� �����ϰ�, �� �����͸� ����Ʈ�� �ִ´�.
void CASEParser::Create_onemesh_to_list()
{
	ASEParser::Mesh* temp = new ASEParser::Mesh;
	m_OneMesh = temp;
	m_OneMesh->m_scenedata = m_scenedata;		// Ŭ������ �� ����
	m_MeshList.push_back(m_OneMesh);
}

// ��Ʈ���� �ϳ��� �������� �����ϰ�, �� �����͸� ����Ʈ�� �ִ´�.
void CASEParser::Create_materialdata_to_list()
{
	ASEParser::ASEMaterial* temp = new ASEParser::ASEMaterial;
	m_materialdata = temp;
	m_list_materialdata.push_back(m_materialdata);
}

// �ִϸ��̼ǵ����� �ϳ��� �������� �����ϰ�, �� �����͸� ����Ʈ�� �ִ´�.
void CASEParser::Create_animationdata_to_list()
{
	hsAnimation* temp = new hsAnimation;
	m_animation = temp;
	m_list_animation.push_back(m_animation);
}

// ���� �ϳ���..
void CASEParser::Create_onevertex_to_list()
{
	ASEParser::Vertex* temp = new ASEParser::Vertex;
	m_OneMesh->m_meshvertex.push_back(temp);
}

void CASEParser::Split()
{
	///���ؽ��� ���
	int VCount = m_OneMesh->m_mesh_numvertex;
	///���̽��� ���
	int FCount = m_OneMesh->m_mesh_numfaces;

	///���ý��� �߰��� �ε��� ��ȣ 
	///���� ���ؽ��� �߰��ɲ��� �� ��ȣ ���ķ� �߰��ɰ��̴�
	int Add_index = VCount;

	//���TM
	XMFLOAT4X4 xm;
	xm._11 = m_OneMesh->m_tm_row0.x;		xm._13 = m_OneMesh->m_tm_row0.z; xm._12 = m_OneMesh->m_tm_row0.y;		xm._14 = 0;
	xm._21 = m_OneMesh->m_tm_row2.x;		xm._23 = m_OneMesh->m_tm_row2.z; xm._22 = m_OneMesh->m_tm_row2.y;		xm._24 = 0;
	xm._31 = m_OneMesh->m_tm_row1.x;		xm._33 = m_OneMesh->m_tm_row1.z; xm._32 = m_OneMesh->m_tm_row1.y;		xm._34 = 0;
	xm._41 = m_OneMesh->m_tm_row3.x;		xm._43 = m_OneMesh->m_tm_row3.z; xm._42 = m_OneMesh->m_tm_row3.y;		xm._44 = 1;
	m_OneMesh->W_TM = XMLoadFloat4x4(&xm);

	///�װ�Ƽ�� ������ Ȯ��
	Vector3 X = { xm._11,xm._12,xm._13 };
	Vector3 Y = { xm._21,xm._22,xm._23 };
	Vector3 Z = { xm._31,xm._32,xm._33 };

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
		XMMatrixDecompose(&_S, &_R, &_P, m_OneMesh->W_TM);
	
		XMFLOAT3 V_S;
		XMStoreFloat3(&V_S, _S);
		V_S.x *= -1;
		V_S.y *= -1;
		V_S.z *= -1;
		m_OneMesh->W_TM = XMMatrixScaling(V_S.x, V_S.y, V_S.z) * XMMatrixRotationQuaternion(_R) * XMMatrixTranslationFromVector(_P);
	
		///�븻���� ����
		int FCount = m_OneMesh->m_mesh_numfaces;
		for (int i = 0; i < FCount; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				m_OneMesh->m_meshface[i]->m_normalvertex[j].x *= -1;
				m_OneMesh->m_meshface[i]->m_normalvertex[j].y *= -1;
				m_OneMesh->m_meshface[i]->m_normalvertex[j].z *= -1;
			}
		}
	}

	
	int TextureSize = m_OneMesh->m_mesh_tvertex.size();
	if (TextureSize == 0)
	{
		///�ؽ��� ������ �������
		Off_TextureSplit(Add_index);
	}
	else
	{
		///�ؽ��� ������ �������
		On_TextureSplit(Add_index);
	}
}

void CASEParser::On_TextureSplit(int& AddCount)
{
	int FCount = m_OneMesh->m_mesh_numfaces;
	for (int i = 0; i < FCount; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			///�̰���  ������Ʈ�� Face �ȿ� ���ؽ����� �ε����� ����Ȱ����� �ε����� ����
			int num = m_OneMesh->m_meshface[i]->m_vertexindex[j];
			///�̰��� �ؽ����� �ε��� ��ȣ�� ���´�
			int TFaceIndex = 0;
			TFaceIndex = m_OneMesh->m_meshface[i]->m_TFace[j];


			///�׷��� ���� �ε����� ���ý��� �����´�
			ASEParser::Vertex* ver = m_OneMesh->m_meshvertex[num];
			


			///������ �̸� �޾Ƶ� �븻���͵��� ������
			float X = m_OneMesh->m_meshface[i]->m_normalvertex[j].x;
			float Y = m_OneMesh->m_meshface[i]->m_normalvertex[j].y;
			float Z = m_OneMesh->m_meshface[i]->m_normalvertex[j].z;

			///������ �̸� �޾Ƶ� �ؽ��� ������ ������
			ASEParser::COneTVertex* Cver = m_OneMesh->m_mesh_tvertex[TFaceIndex];


			///���� ã�ں��� ���� �븻�̳� uv���� �ϳ��� ��������ʴٸ� �̹��ý��� ���� ���� ���� ��������
			if (ver->m_normal.x == 0 && ver->m_normal.y == 0 && ver->m_normal.z == 0 &&
				ver->u == 0 && ver->v == 0)
			{
				///���࿡ X, Y, Z �ǰ��� 0�̸� ���� �븻������ ���� ���� ���ؽ����̱� ������ ���� �׳� �־��ָ�ȴ�
				ver->m_normal.x = X;
				ver->m_normal.y = Y;
				ver->m_normal.z = Z;

				///�ؽ��ĵ� ��������
				ver->u = Cver->m_u;
				ver->v = Cver->m_v;

			}
			else if (ver->m_normal.x != X || ver->m_normal.y != Y || ver->m_normal.z != Z ||
				ver->u != Cver->m_u || ver->v != Cver->m_v)
			{
				///�ʱⰪ���� ������ �־��ٰ��̰� �̰��� �븻���� �ϳ��� ���������� �߰�������ϴ� ���ý���

				///���� �߸����°� �����ϱ����� push_back���� ���ϰ� �ε��������� �׳� �־��ٷ�
				m_OneMesh->m_meshvertex.push_back(new ASEParser::Vertex);
				m_OneMesh->m_meshvertex[AddCount]->m_pos = m_OneMesh->m_meshvertex[num]->m_pos;
				m_OneMesh->m_meshvertex[AddCount]->m_normal = { X,Y,Z };

				///���������� uv���� ���� �־� �ش�
				m_OneMesh->m_meshvertex[AddCount]->u = m_OneMesh->m_mesh_tvertex[TFaceIndex]->m_u;
				m_OneMesh->m_meshvertex[AddCount]->v = m_OneMesh->m_mesh_tvertex[TFaceIndex]->m_v;


				m_OneMesh->m_meshvertex[AddCount]->m_bw1 = m_OneMesh->m_meshvertex[num]->m_bw1;
				m_OneMesh->m_meshvertex[AddCount]->m_bw2 = m_OneMesh->m_meshvertex[num]->m_bw2;
				m_OneMesh->m_meshvertex[AddCount]->m_bw3 = m_OneMesh->m_meshvertex[num]->m_bw3;
				
				for (int k = 0; k < 4; k++)
				{
					m_OneMesh->m_meshvertex[AddCount]->m_indicesPointer[k] = m_OneMesh->m_meshvertex[num]->m_indicesPointer[k];
				}

				///�̷��� �߰��� ���ý��� Face�� ��������ش�
				m_OneMesh->m_meshface[i]->m_vertexindex[j] = AddCount;
				AddCount++;
			}
		}
	}


}

void CASEParser::Off_TextureSplit(int& AddCount)
{
	int FCount = m_OneMesh->m_mesh_numfaces;
	for (int i = 0; i < FCount; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			///�̰���  ������Ʈ�� Face �ȿ� ���ؽ����� �ε����� ����Ȱ����� �ε����� ����
			int num = m_OneMesh->m_meshface[i]->m_vertexindex[j];

			///�׷��� ���� �ε����� ���ý��� �����´�
			ASEParser::Vertex* ver = m_OneMesh->m_meshvertex[num];


			///������ �̸� �޾Ƶ� �븻���͵��� ������
			float X = m_OneMesh->m_meshface[i]->m_normalvertex[j].x;
			float Y = m_OneMesh->m_meshface[i]->m_normalvertex[j].y;
			float Z = m_OneMesh->m_meshface[i]->m_normalvertex[j].z;

			///���� ã�ں��� ���� �븻�̳� uv���� �ϳ��� ��������ʴٸ� �̹��ý��� ���� ���� ���� ��������
			if (ver->m_normal.x == 0 && ver->m_normal.y == 0 && ver->m_normal.z == 0)
			{
				///���࿡ X, Y, Z �ǰ��� 0�̸� ���� �븻������ ���� ���� ���ؽ����̱� ������ ���� �׳� �־��ָ�ȴ�
				ver->m_normal.x = X;
				ver->m_normal.y = Y;
				ver->m_normal.z = Z;
			}
			else if (ver->m_normal.x != X || ver->m_normal.y != Y || ver->m_normal.z != Z)
			{
				///�ʱⰪ���� ������ �־��ٰ��̰� �̰��� �븻���� �ϳ��� ���������� �߰�������ϴ� ���ý���

				///���� �߸����°� �����ϱ����� push_back���� ���ϰ� �ε��������� �׳� �־��ٷ�
				m_OneMesh->m_meshvertex.push_back(new ASEParser::Vertex);
				m_OneMesh->m_meshvertex[AddCount]->m_pos = m_OneMesh->m_meshvertex[num]->m_pos;
				m_OneMesh->m_meshvertex[AddCount]->m_normal = { X,Y,Z };
				

				m_OneMesh->m_meshvertex[AddCount]->m_bw1 = m_OneMesh->m_meshvertex[num]->m_bw1;
				m_OneMesh->m_meshvertex[AddCount]->m_bw2 = m_OneMesh->m_meshvertex[num]->m_bw2;
				m_OneMesh->m_meshvertex[AddCount]->m_bw3 = m_OneMesh->m_meshvertex[num]->m_bw3;
				
				for (int k = 0; k < 4; k++)
				{
					m_OneMesh->m_meshvertex[AddCount]->m_indicesPointer[k] = m_OneMesh->m_meshvertex[num]->m_indicesPointer[k];
				}
				

				///�̷��� �߰��� ���ý��� Face�� ��������ش�
				m_OneMesh->m_meshface[i]->m_vertexindex[j] = AddCount;
				AddCount++;
			}
		}
	}
}


















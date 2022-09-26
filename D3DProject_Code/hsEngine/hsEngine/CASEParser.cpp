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
// 로딩한다.
// 이것이 끝났다면 정해진 데이터형에 모든 데이터를 읽어서 들어가 있어야 한다.
//
// Parsing에 대해:
// 항상 느끼는 것이지만 parsing이라는 것은 데이터가 일정 규격에 맞게 들어가 있다는 것을 전제로 한다.
// 하지만, 파일 내부에 순서가 바뀌어 들어가 있는것이 가능하다던지 하는 규칙이 생기면
// 검색하는 루틴이 복잡해지기 마련. 일단은 순서대로 일정 규약으로 들어가 있다는것을 가정하자.
// -> 그래서, 재귀 호출을 하는 하나의 큰 함수로 해결봤다.
// -> depth를 기준으로 오동작에 대한 안전 코드를 넣어야겠다
//---------------------------------------------------------------------------------------------------
bool CASEParser::Load(LPSTR p_File)
{
	/// 0) 파일을 로드한다.
	if (!m_lexer->Open(p_File))
	{
		TRACE("파일을 여는 중에 문제가 발생했습니다!");
		return FALSE;
	}

	/// 1) 다롱이아부지김교수의 파워풀한 재귀함수 한개로 처리 끝!
	Parsing_DivergeRecursiveALL(0);

	return TRUE;
}

/// 1차 변환

// CScenedata 복사
bool CASEParser::TranslateToD3DFormat_scene(ASEParser::Scenedata* pscene)
{
	// 값 복사
	*pscene = this->m_scenedata;
	return TRUE;
}

/// <summary>
/// 메시의 최적화를 해 준다.
/// 노말값, 텍스쳐 좌표에 따라 버텍스를 늘리고, 중첩되는것은 제거하고..
/// </summary>
bool CASEParser::Convert_Optimize(ASEParser::Mesh* pMesh)
{
	// 2021.04.12
	// 구현 제거 (예시용)

	return FALSE;
}

bool CASEParser::ConvertAll(ASEParser::Mesh* pMesh)
{
	// 버텍스들은 일단 모두 복사
	for (unsigned int i = 0; i < pMesh->m_meshvertex.size(); i++)
	{
		ASEParser::Vertex* _pVertex = new ASEParser::Vertex;
		_pVertex->m_pos = pMesh->m_meshvertex[i]->m_pos;

		pMesh->m_opt_vertex.push_back(_pVertex);
	}

	// 나머지는 face를 기준으로 한 인덱스로 찾아야 함
	unsigned int _faceCount = pMesh->m_meshface.size();
	//TRACE("FaceCount : %d", _faceCount);

	for (unsigned int i = 0; i < pMesh->m_meshface.size(); i++)
	{
		//TRACE("FaceNum : %d / %d\n", i, _faceCount);

		for (int j = 0; j < 3; j++)
		{
			ASEParser::Vertex* _nowVertex = pMesh->m_opt_vertex[pMesh->m_meshface[i]->m_vertexindex[j]];

			// face기준으로 버텍스의 노말을 넣어준다.
			// (예시용)
			_nowVertex->m_normal.x = 0;
			_nowVertex->m_normal.y = 0;
			_nowVertex->m_normal.z = 0;
		}
	}

	// 인덱스는 그냥 복사
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
// 재귀 호출됨을 전제로 하는 분기 함수이다.
//
// 일단 읽고, 토큰에 따라 동작한다.
//
// 재귀를 들어가는 조건은 { 을 만날 경우이다.
// 리턴하는 조건은 '}'를 만나거나 TOKEND_END를 만났을 때이다.
//
// 더불어, 재귀를 들어가기 전 토큰을 읽어, 지금 이 함수가 어떤 모드인지를 결정한다.
//
//----------------------------------------------------------------
void CASEParser::Parsing_DivergeRecursiveALL(int depth)
{
	//----------------------------------------------------------------------
	// 지역변수들 선언
	// 재귀 돌리다가 일정 이상 루프를 도는 경우(오류)를 체크하기 위한 변수
	int i = 0;
	// 리스트에 데이터를 넣어야 할 때 쓰이는 리스트 카운터.
	int listcount = 0;		// 매우 C스러운데 다른 해결책이 없을까?
	int FACENOMALindex = 0;
	// 이것은 현 토큰이 어떤 종류인가를 판별해준다.
	// 이것을 멤버 변수로 하고 재귀를 돌리는것은 위험하다? -> 잘못된 재귀방식이다?
	LONG nowtoken;

	//----------------------------------------------------------------------

	Matrix TM1, TM2;
	Quaternion tempQT;
	Quaternion prevQT;
	Quaternion resultQT;

	/// 이것을 하면 한 개의 토큰을 읽고, 그 종류와 내용을 알 수 있다.
	while (nowtoken = m_lexer->GetToken(m_TokenString), nowtoken != TOKEND_BLOCK_END)
	{
		// 일단 한 개의 토큰을 읽고, 그것이 괄호 닫기가 아니라면.
		// 넘어온 토큰에 따라 처리해준다.

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
			Parsing_String();	// 그냥 m_TokenString에 읽어버리는 역할 뿐.
			//AfxMessageBox( m_TokenString, NULL, NULL);		/// 임시로 코멘트를 출력해본다
			break;

			//--------------------
			// SCENE
			//--------------------

		case TOKENR_SCENE:
			//
			break;
		case TOKENR_SCENE_FILENAME:
			m_scenedata.m_filename = Parsing_String();		// 일관성 있는 함수의 사용을 위해 String과 Int도 만들어줬다.
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
			// 그냥 안의 내용을 읽어버린다 }가 나올때까지
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
			//	한 개의 그룹 시작. 이 다음에 이름이 스트링으로 나오기는 하는데.
			break;

		case TOKENR_HELPEROBJECT:
		{
			///가장 최상위 객체 이객체는 parent 가없다
			///일단은 그냥 기본값을 넣어준다
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

			/// 이 토큰을 만났다는건 새로운 메시가 생겼다는 것이다. 지역 변수로 mesh를 하나 선언, 그 포인터를 리스트에 넣고, m_onemesh에 그 포인터를 복사, 그대로 쓰면 될까?
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
			///이오브젝트 위에는 상위오브젝트가 있다

			if (m_OneMesh->m_nodename != "")
			{

				m_OneMesh->m_isparentexist = true;
			}

			break;

			/// NODE_TM
		case TOKENR_NODE_TM:

			//m_parsingmode	=	eGeomobject;
			// (NODE_TM으로 진입 후 NODE_NAME이 한번 더 나온다.)
			// (Animation과도 구별을 해야 하기 때문에 이렇게 모드를 적어준다)

			/// 게다가,
			// 카메라는 NodeTM이 두번 나온다. 두번째라면 넣지 않는다.
			// 아예 이 재귀에서 리턴을 시키고 싶지만. 중간에 읽는것을 무시해야 하기 때문에...
			//if (m_onemesh->m_camera_isloadTarget) {
			//	'}'이 나올때까지 나오는건 무조건 무시! 뭐 이런거 안되나..
			// 재귀구조의 문제점이다....

			break;

		case TOKENR_INHERIT_POS:
			m_OneMesh->m_inherit_pos.x = Parsing_NumberFloat();
			m_OneMesh->m_inherit_pos.z = Parsing_NumberFloat();
			m_OneMesh->m_inherit_pos.y = Parsing_NumberFloat();

			// 카메라는 NodeTM이 두번 나온다. 두번째라면 넣지 않는다.
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
			// 현재 카메라 상태였다면 이미 노드를 읽은 것으로 표시해준다.
			break;


			/// MESH
		case TOKENR_MESH:
			///오브젝트가 1개가 아닐수도있기때문에 
			///mesh가 비어있으면 새롭게만들고 아니면 기존꺼를 리스트에 넣어주고 새로만든다
			break;
		case TOKENR_TIMEVALUE:
			break;
		case TOKENR_MESH_NUMBONE:
			// 이게 있다면 이것은 Skinned Mesh라고 단정을 짓는다.
			// 내용 입력
			break;
		case TOKENR_MESH_NUMSKINWEIGHT:
			break;
		case TOKENR_MESH_NUMVERTEX:
			//버택스 카운터
			m_OneMesh->m_mesh_numvertex = Parsing_NumberInt();
			m_OneMesh->m_meshvertex.resize(m_OneMesh->m_mesh_numvertex);
			break;
		case TOKENR_MESH_NUMFACES:
			//인덱스 카운터
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
			/// 모드 체인지 해 주고, Bone을 소유하고 있다는 것은 이것은 스키닝 오브젝트라는 것이다.
			// 본 하나를 만들어서 임시 포인터 보관, 벡터에 넣고
			// Bone의 넘버를 읽어 주자
		}
		break;
		//이 다음에 본의 이름을 넣어야 한다. 하지만 {를 한 개 더 열었으므로 임시 포인터 변수로서 보관해야겠지.
		case TOKENR_BONE_NAME:
			m_OneMesh->BoneName = Parsing_String();
			
			m_OneMesh->m_is_BoneList.insert({m_OneMesh->BoneNumber,m_OneMesh->BoneName });
		case TOKENR_BONE_PROPERTY:
			// 이 다음 ABSOLUTE가 나오기는 하는데, 쓸 일이 없다.
			break;
			// 다음에는 TM_ROW0~3이 나오는데 역시 무시됨..

		case TOKENR_MESH_WVERTEXS:
			break;

		case TOKENR_MESH_WEIGHT:
		{
			// 버텍스 하나의 정보를 만들어서 리스트에 넣음
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


			// 대체 몇 단계를 들어가는거야...
			// 가중치 한개를 만들어서 리스트에 넣는다
			/// 헥 헥....
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
			// Face의 번호인데...
			int index = Parsing_NumberFloat();

			Parsing_String();
			temp->m_vertexindex[0] = Parsing_NumberFloat();
			Parsing_String();
			temp->m_vertexindex[1] = Parsing_NumberFloat();
			Parsing_String();
			temp->m_vertexindex[2] = Parsing_NumberFloat();
			m_OneMesh->m_meshface[index] = temp;
			// A:를 읽고
			// B:
			// C:

			/// (뒤에 정보가 더 있지만 default에 의해 스킵될 것이다.)
			/// ......

			// 벡터에 넣어준다.
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
			///가장 최상위 객체 이객체는 parent 가없다
			///일단은 그냥 기본값을 넣어준다
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

			//일단 위치값부터 넣는다
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

			//일단 위치값부터 넣는다
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

			//일단 위치값부터 넣는다
			m_animation->m_scale.push_back(scl);
		}
		break;

		case TOKEND_END:
			Split();
			//m_MeshList.push_back(m_OneMesh);

			//m_OneMesh = new ASEParser::Mesh;

			// 아마도 이건 파일의 끝이 나타났을때인것 같은데. while을 탈출해야 하는데?

			//AfxMessageBox("파일의 끝을 본 것 같습니다!", MB_OK, NULL);
			//TRACE("TRACE: 파싱중: 파일의 끝을 봤습니다!\n");
 			return;


			break;
			/// 위의 아무것도 해당하지 않을때
		default:


			// 아무것도 하지 않는다.
			break;

		}	// switch()


		///-----------------------------------------------
		/// 안전 코드.
		i++;
		if (i > 1000000)
		{
			// 루프를 1000000번이상이나 돌 이유가 없다. (데이터가 100000개가 아닌이상)
			// 만약 1000000이상 돌았다면 확실히 뭔가 문제가 있는 것이므로
			//TRACE("루프를 백만번 돌았습니다!");
			return;
		}
		/// 안전 코드.
		///-----------------------------------------------

	}		// while()

	// 여기까지 왔다면 while()을 벗어났다는 것이고 그 말은
	// 괄호를 닫았다는 것이므로
	// 리턴하자 (재귀함수)


	return;
}


///----------------------------------------------------------------------
/// parsing을 위한 단위별 함수들
///----------------------------------------------------------------------

// long을 읽어서 리턴해준다.
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
	/// ※m_TokenString ( char[255] ) 이기 때문에 CString에 넣으면 에러 날거라 생각했는데, 생각보다 CString은 잘 만들어진 것 같다. 알아서 받아들이는데?
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

// 3개의 Float를 벡터 하나로
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

	return			tempVector3;		// 스태틱 변수의 레퍼런스보다는 값 전달을 하자.
}

///--------------------------------------------------
/// 내부에서 뭔가를 생성, 리스트에 넣는다
///--------------------------------------------------
// 메시를 하나 동적으로 생성하고, 그 포인터를 리스트에 넣는다.
void CASEParser::Create_onemesh_to_list()
{
	ASEParser::Mesh* temp = new ASEParser::Mesh;
	m_OneMesh = temp;
	m_OneMesh->m_scenedata = m_scenedata;		// 클래스간 값 복사
	m_MeshList.push_back(m_OneMesh);
}

// 메트리얼 하나를 동적으로 생성하고, 그 포인터를 리스트에 넣는다.
void CASEParser::Create_materialdata_to_list()
{
	ASEParser::ASEMaterial* temp = new ASEParser::ASEMaterial;
	m_materialdata = temp;
	m_list_materialdata.push_back(m_materialdata);
}

// 애니메이션데이터 하나를 동적으로 생성하고, 그 포인터를 리스트에 넣는다.
void CASEParser::Create_animationdata_to_list()
{
	hsAnimation* temp = new hsAnimation;
	m_animation = temp;
	m_list_animation.push_back(m_animation);
}

// 정점 하나를..
void CASEParser::Create_onevertex_to_list()
{
	ASEParser::Vertex* temp = new ASEParser::Vertex;
	m_OneMesh->m_meshvertex.push_back(temp);
}

void CASEParser::Split()
{
	///버텍스의 계수
	int VCount = m_OneMesh->m_mesh_numvertex;
	///페이스의 계수
	int FCount = m_OneMesh->m_mesh_numfaces;

	///버택스가 추가될 인덱스 번호 
	///만약 버텍스가 추가될꺼면 이 번호 이후로 추가될것이당
	int Add_index = VCount;

	//노드TM
	XMFLOAT4X4 xm;
	xm._11 = m_OneMesh->m_tm_row0.x;		xm._13 = m_OneMesh->m_tm_row0.z; xm._12 = m_OneMesh->m_tm_row0.y;		xm._14 = 0;
	xm._21 = m_OneMesh->m_tm_row2.x;		xm._23 = m_OneMesh->m_tm_row2.z; xm._22 = m_OneMesh->m_tm_row2.y;		xm._24 = 0;
	xm._31 = m_OneMesh->m_tm_row1.x;		xm._33 = m_OneMesh->m_tm_row1.z; xm._32 = m_OneMesh->m_tm_row1.y;		xm._34 = 0;
	xm._41 = m_OneMesh->m_tm_row3.x;		xm._43 = m_OneMesh->m_tm_row3.z; xm._42 = m_OneMesh->m_tm_row3.y;		xm._44 = 1;
	m_OneMesh->W_TM = XMLoadFloat4x4(&xm);

	///네거티브 스케일 확인
	Vector3 X = { xm._11,xm._12,xm._13 };
	Vector3 Y = { xm._21,xm._22,xm._23 };
	Vector3 Z = { xm._31,xm._32,xm._33 };

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
		XMMatrixDecompose(&_S, &_R, &_P, m_OneMesh->W_TM);
	
		XMFLOAT3 V_S;
		XMStoreFloat3(&V_S, _S);
		V_S.x *= -1;
		V_S.y *= -1;
		V_S.z *= -1;
		m_OneMesh->W_TM = XMMatrixScaling(V_S.x, V_S.y, V_S.z) * XMMatrixRotationQuaternion(_R) * XMMatrixTranslationFromVector(_P);
	
		///노말값도 변경
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
		///텍스쳐 정보가 없을경우
		Off_TextureSplit(Add_index);
	}
	else
	{
		///텍스쳐 정보가 있을경우
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
			///이곳은  오브젝트의 Face 안에 버텍스들의 인덱스가 저장된곳에서 인덱스를 빼옴
			int num = m_OneMesh->m_meshface[i]->m_vertexindex[j];
			///이곳은 텍스쳐의 인덱스 번호를 빼온다
			int TFaceIndex = 0;
			TFaceIndex = m_OneMesh->m_meshface[i]->m_TFace[j];


			///그렇게 빼온 인덱스로 버택스를 가져온다
			ASEParser::Vertex* ver = m_OneMesh->m_meshvertex[num];
			


			///이전에 미리 받아둔 노말벡터들을 가져옴
			float X = m_OneMesh->m_meshface[i]->m_normalvertex[j].x;
			float Y = m_OneMesh->m_meshface[i]->m_normalvertex[j].y;
			float Z = m_OneMesh->m_meshface[i]->m_normalvertex[j].z;

			///이전에 미리 받아둔 텍스쳐 정보를 가져옴
			ASEParser::COneTVertex* Cver = m_OneMesh->m_mesh_tvertex[TFaceIndex];


			///이제 찾자보자 만약 노말이나 uv값이 하나도 들어있지않다면 이버택스는 아직 값을 넣지 않은것임
			if (ver->m_normal.x == 0 && ver->m_normal.y == 0 && ver->m_normal.z == 0 &&
				ver->u == 0 && ver->v == 0)
			{
				///만약에 X, Y, Z 의값이 0이면 아직 노말값들을 넣지 않은 버텍스들이기 때문에 값을 그냥 넣어주면된다
				ver->m_normal.x = X;
				ver->m_normal.y = Y;
				ver->m_normal.z = Z;

				///텍스쳐도 마찬가지
				ver->u = Cver->m_u;
				ver->v = Cver->m_v;

			}
			else if (ver->m_normal.x != X || ver->m_normal.y != Y || ver->m_normal.z != Z ||
				ver->u != Cver->m_u || ver->v != Cver->m_v)
			{
				///초기값들은 위에서 넣어줄것이고 이곳은 노말값중 하나라도 맞지않으면 추가해줘야하는 버택스임

				///값이 잘못들어가는걸 방지하기위해 push_back으로 안하고 인덱스값으로 그냥 넣어줄래
				m_OneMesh->m_meshvertex.push_back(new ASEParser::Vertex);
				m_OneMesh->m_meshvertex[AddCount]->m_pos = m_OneMesh->m_meshvertex[num]->m_pos;
				m_OneMesh->m_meshvertex[AddCount]->m_normal = { X,Y,Z };

				///마찬가지로 uv값도 같이 넣어 준다
				m_OneMesh->m_meshvertex[AddCount]->u = m_OneMesh->m_mesh_tvertex[TFaceIndex]->m_u;
				m_OneMesh->m_meshvertex[AddCount]->v = m_OneMesh->m_mesh_tvertex[TFaceIndex]->m_v;


				m_OneMesh->m_meshvertex[AddCount]->m_bw1 = m_OneMesh->m_meshvertex[num]->m_bw1;
				m_OneMesh->m_meshvertex[AddCount]->m_bw2 = m_OneMesh->m_meshvertex[num]->m_bw2;
				m_OneMesh->m_meshvertex[AddCount]->m_bw3 = m_OneMesh->m_meshvertex[num]->m_bw3;
				
				for (int k = 0; k < 4; k++)
				{
					m_OneMesh->m_meshvertex[AddCount]->m_indicesPointer[k] = m_OneMesh->m_meshvertex[num]->m_indicesPointer[k];
				}

				///이렇게 추가한 버택스를 Face와 연결시켜준다
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
			///이곳은  오브젝트의 Face 안에 버텍스들의 인덱스가 저장된곳에서 인덱스를 빼옴
			int num = m_OneMesh->m_meshface[i]->m_vertexindex[j];

			///그렇게 빼온 인덱스로 버택스를 가져온다
			ASEParser::Vertex* ver = m_OneMesh->m_meshvertex[num];


			///이전에 미리 받아둔 노말벡터들을 가져옴
			float X = m_OneMesh->m_meshface[i]->m_normalvertex[j].x;
			float Y = m_OneMesh->m_meshface[i]->m_normalvertex[j].y;
			float Z = m_OneMesh->m_meshface[i]->m_normalvertex[j].z;

			///이제 찾자보자 만약 노말이나 uv값이 하나도 들어있지않다면 이버택스는 아직 값을 넣지 않은것임
			if (ver->m_normal.x == 0 && ver->m_normal.y == 0 && ver->m_normal.z == 0)
			{
				///만약에 X, Y, Z 의값이 0이면 아직 노말값들을 넣지 않은 버텍스들이기 때문에 값을 그냥 넣어주면된다
				ver->m_normal.x = X;
				ver->m_normal.y = Y;
				ver->m_normal.z = Z;
			}
			else if (ver->m_normal.x != X || ver->m_normal.y != Y || ver->m_normal.z != Z)
			{
				///초기값들은 위에서 넣어줄것이고 이곳은 노말값중 하나라도 맞지않으면 추가해줘야하는 버택스임

				///값이 잘못들어가는걸 방지하기위해 push_back으로 안하고 인덱스값으로 그냥 넣어줄래
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
				

				///이렇게 추가한 버택스를 Face와 연결시켜준다
				m_OneMesh->m_meshface[i]->m_vertexindex[j] = AddCount;
				AddCount++;
			}
		}
	}
}


















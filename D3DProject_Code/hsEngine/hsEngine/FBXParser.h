#pragma once

class FbxObject;
class FBXParser
{
public:
	FBXParser();
	~FBXParser();
	void init();
	void Delete();

public:
	//매쉬를  로드함
	LoadMesh* Load_MODELS_FBX(string pathName, bool LoadAnimation = false);
	//경로를 미리지정
	void SetLoadPath(string path);
	//애니메이션 정보를 가져옴
	LoadAnimation* GetOneAnimation();
	void AnimeReset();
private:
	///파서 안에서만 사용하는 함수들

	//매쉬정보는 읽지않고 애니메이션만 빼온다
	bool OnlyAnimation;

	//한개의 매쉬를 생성
	LoadData* CreateMesh(FbxNode* node);
	//애니메이션의 상위 내용을 가져온다 
	void GetAnimation();
	//본한개의 애니메이션을 만든다
	void CreateAnimation(FbxNode* node);
	//재귀함수를 타면서 각종 정보들을 가져옴
	void FindNode(FbxNode* nextNode);
	//본을 만들려고 하는데 이미 있는 본인지 확인
	bool FindBone(string name);
	//임시 변수들을 클리어해준다
	void DataClear();
	//재귀를 타고 호출하면서 switch문에 해당하는 함수를 실행
	void FindNodeType_AndStart(int type, FbxNode* node);



	/// 각각의 테이블을 만든다 스플릿하기전 준비
	//본정보 기본테이블
	void CreateWeightTable(FbxNode* node, LoadData* data);
	//버텍스 위치 기본테이블
	void CreateVertexPosTable(FbxMesh* mesh);
	//버텍스 인덱스 기본테이블
	void CreateVertexIndexTable(FbxMesh* mesh);
	//탄젠트 테이블 을 생성
	void CreateTangentTable(FbxMesh* mesh);
	//노말을 가져온다
	XMFLOAT3 GetNomal(FbxMesh* mesh, int pointindex, int vertexCount);
	//uv를 뽑아온다
	XMFLOAT2 GetUV(FbxMesh* mesh, int pointindex, int vertexCount);

	XMFLOAT3 GetBiNomal(FbxMesh* mesh, int pointindex, int vertexCount);
	//텍스쳐를 불러온다
	void GetTexturepath(string fileName, FbxScene* scene);
	//버택스를 불러온다
	void GetVerTex(FbxMesh* mesh);
	//본의 기본정보를 만듬
	void CreateSkeleton(FbxNode* mesh);
	//만들어진 정보들로 버텍스들을 만들어줌
	void Split(FbxMesh* mesh);


	///시스템 설정
	void SceneSetting(FbxScene* Scene);
private:
	FbxManager* FBX_manager = nullptr;
	fbxsdk::FbxScene* m_Scene = nullptr;
	FbxNode* FBX_Node = nullptr;
	FbxMesh* FBX_Mesh = nullptr;
	FbxIOSettings* ios = nullptr;
	FbxImporter* importer = nullptr;

private:
	std::vector<XMFLOAT3>		position;
	std::vector<XMFLOAT3>		nomal;
	std::vector<XMFLOAT2>		uv;
	std::vector<int>			Vertexindex;
	std::vector<vector<UINT> >	bonIndex;
	std::vector<Vector4>		weights;
	std::map<string, LoadData*> SkinListData;
	std::map<string, Bone*>		BoneListData;

	LoadData* data;
	LoadMesh* dataList;

	LoadAnimation* AnimeData;
private:
	//파일 경로를 미리받아놓는다
	string FilePath;
private:
	//struct 변환
	TCHAR* ConvertStringToTCHAR(string& s);
	std::string* ConvertTCharToString(const TCHAR* ptsz);
	void GetPath(const TCHAR* FullPathName, TCHAR* Path);

	XMFLOAT4X4 ChangeDirMatrix(FbxAMatrix fbxTM);
	//네거티브 스케일확인
	XMMATRIX CheckNegative(XMFLOAT4X4& TM);
};
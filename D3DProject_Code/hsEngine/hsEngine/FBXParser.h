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
	//�Ž���  �ε���
	LoadMesh* Load_MODELS_FBX(string pathName, bool LoadAnimation = false);
	//��θ� �̸�����
	void SetLoadPath(string path);
	//�ִϸ��̼� ������ ������
	LoadAnimation* GetOneAnimation();
	void AnimeReset();
private:
	///�ļ� �ȿ����� ����ϴ� �Լ���

	//�Ž������� �����ʰ� �ִϸ��̼Ǹ� ���´�
	bool OnlyAnimation;

	//�Ѱ��� �Ž��� ����
	LoadData* CreateMesh(FbxNode* node);
	//�ִϸ��̼��� ���� ������ �����´� 
	void GetAnimation();
	//���Ѱ��� �ִϸ��̼��� �����
	void CreateAnimation(FbxNode* node);
	//����Լ��� Ÿ�鼭 ���� �������� ������
	void FindNode(FbxNode* nextNode);
	//���� ������� �ϴµ� �̹� �ִ� ������ Ȯ��
	bool FindBone(string name);
	//�ӽ� �������� Ŭ�������ش�
	void DataClear();
	//��͸� Ÿ�� ȣ���ϸ鼭 switch���� �ش��ϴ� �Լ��� ����
	void FindNodeType_AndStart(int type, FbxNode* node);



	/// ������ ���̺��� ����� ���ø��ϱ��� �غ�
	//������ �⺻���̺�
	void CreateWeightTable(FbxNode* node, LoadData* data);
	//���ؽ� ��ġ �⺻���̺�
	void CreateVertexPosTable(FbxMesh* mesh);
	//���ؽ� �ε��� �⺻���̺�
	void CreateVertexIndexTable(FbxMesh* mesh);
	//ź��Ʈ ���̺� �� ����
	void CreateTangentTable(FbxMesh* mesh);
	//�븻�� �����´�
	XMFLOAT3 GetNomal(FbxMesh* mesh, int pointindex, int vertexCount);
	//uv�� �̾ƿ´�
	XMFLOAT2 GetUV(FbxMesh* mesh, int pointindex, int vertexCount);

	XMFLOAT3 GetBiNomal(FbxMesh* mesh, int pointindex, int vertexCount);
	//�ؽ��ĸ� �ҷ��´�
	void GetTexturepath(string fileName, FbxScene* scene);
	//���ý��� �ҷ��´�
	void GetVerTex(FbxMesh* mesh);
	//���� �⺻������ ����
	void CreateSkeleton(FbxNode* mesh);
	//������� ������� ���ؽ����� �������
	void Split(FbxMesh* mesh);


	///�ý��� ����
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
	//���� ��θ� �̸��޾Ƴ��´�
	string FilePath;
private:
	//struct ��ȯ
	TCHAR* ConvertStringToTCHAR(string& s);
	std::string* ConvertTCharToString(const TCHAR* ptsz);
	void GetPath(const TCHAR* FullPathName, TCHAR* Path);

	XMFLOAT4X4 ChangeDirMatrix(FbxAMatrix fbxTM);
	//�װ�Ƽ�� ������Ȯ��
	XMMATRIX CheckNegative(XMFLOAT4X4& TM);
};
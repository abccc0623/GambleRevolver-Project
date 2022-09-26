#pragma once

/*
* ���ҽ��Ŵ����� ���丮 Ŭ���� �ȿ��� �����ɰ���
* ������ ���ҽ��� �־��ִ°� �ٸ�Ŭ���� ���� ���ټ��ֱ⶧���� ex)GameManager �� �ٸ� �Ŵ�������..
* �־��ִ� �Լ��� static����
* ���ҽ��� �������� �Լ����� ��� ���丮 Ŭ�����ȿ��� ȣ��ɰ��̴�
*/
class hsEngine;
class CASEParser;
class ResourceManager
{

public:
	ResourceManager();
	~ResourceManager();

public:
	///Ŭ���� �Լ���
	//��ġ�� ū��� ����..
	void ClearMesh(string Name);
	void ClearAnimation(string Name);
public:
	///���ϰ��
	void SetLoadPath(LOAD_TYPE type, string _path);	
	void SetResource(LOAD_TYPE type,string Name,string ReName);
public:
	///���ҽ� ��������
	ID3D11ShaderResourceView* GetTexture(string Name);
	ID3D11ShaderResourceView* GetNomalTexture(string Name);
	LoadMesh* GetMesh(string Name);
	LoadAnimation* GetAnimation(string Name);
public:
	///�ý���
	//���ҽ� �����
	void Delete();
	void init(hsEngine* _ENGINE);
	//���۰� ���� ���ڻ����� ���� �����´�
	string GetNameString(string Name, char EndStr,char Start);
private:
	//���� �ε带 ���� ASE
	CASEParser* ASE;
	FBXParser* FBX;

	//����
	hsEngine* ENGINE;

	//�Ž��� ���ϰ��
	string Load_ASE_MeshPath;
	string Load_FBX_MeshPath;


	//�ؽ��� ���ϰ��
	string Load_TexturePath;
	//�ؽ��� ���ϰ��
	string Load_NomalTexturePath;
	//�⺻���� ����
	string Load_BaseOBJPath;

	const int ISFBX = 1;
	const int ISASE = 0;
private:
	///����Ʈ��
	//Mesh ������ ����Ʈ
	std::map<string, LoadMesh*> MeshList;
	//Load �ؽ��� ����Ʈ
	std::map<string, ID3D11ShaderResourceView*> TexturesList;
	//Nomal �ؽ��� ����Ʈ
	std::map<string, ID3D11ShaderResourceView*> NomalTexturesList;
	//�ִϸ��̼� ����Ʈ
	std::map<string, LoadAnimation*> AnimationList;
private:
	///�ε� �Լ���
	//�ؽ��ĸ� �ε��Ѵ�
	ID3D11ShaderResourceView* Load_Textures(LPCTSTR path);
	//ASE ���Ϸε� ���� �ε��Ѵ�
	LoadMesh* Load_MODELS_ASE(string path,string ReName);
	//FBX ���Ϸε� ���� �ε��Ѵ�
	LoadMesh* Load_MODELS_FBX(string path, string ReName);
	//�ִϸ��̼� �ε�
	LoadAnimation* Load_Animation(std::list<hsAnimation*>& anim, int keycCount);
};



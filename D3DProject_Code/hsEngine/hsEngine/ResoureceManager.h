#pragma once

/*
* 리소스매니저는 펙토리 클래스 안에서 생성될것임
* 하지만 리소스르 넣어주는건 다른클래스 에서 해줄수있기때문에 ex)GameManager 나 다른 매니저에서..
* 넣어주는 함수는 static으로
* 리소스를 가져오는 함수들은 모두 펙토리 클래스안에서 호출될것이다
*/
class hsEngine;
class CASEParser;
class ResourceManager
{

public:
	ResourceManager();
	~ResourceManager();

public:
	///클리어 함수들
	//덩치가 큰놈들 삭제..
	void ClearMesh(string Name);
	void ClearAnimation(string Name);
public:
	///파일경로
	void SetLoadPath(LOAD_TYPE type, string _path);	
	void SetResource(LOAD_TYPE type,string Name,string ReName);
public:
	///리소스 가져오기
	ID3D11ShaderResourceView* GetTexture(string Name);
	ID3D11ShaderResourceView* GetNomalTexture(string Name);
	LoadMesh* GetMesh(string Name);
	LoadAnimation* GetAnimation(string Name);
public:
	///시스템
	//리소스 지우기
	void Delete();
	void init(hsEngine* _ENGINE);
	//시작과 끝의 문자사이의 값을 가져온다
	string GetNameString(string Name, char EndStr,char Start);
private:
	//파일 로드를 위한 ASE
	CASEParser* ASE;
	FBXParser* FBX;

	//엔진
	hsEngine* ENGINE;

	//매쉬의 파일경로
	string Load_ASE_MeshPath;
	string Load_FBX_MeshPath;


	//텍스쳐 파일경로
	string Load_TexturePath;
	//텍스쳐 파일경로
	string Load_NomalTexturePath;
	//기본도형 폴더
	string Load_BaseOBJPath;

	const int ISFBX = 1;
	const int ISASE = 0;
private:
	///리스트들
	//Mesh 데이터 리스트
	std::map<string, LoadMesh*> MeshList;
	//Load 텍스쳐 리스트
	std::map<string, ID3D11ShaderResourceView*> TexturesList;
	//Nomal 텍스쳐 리스트
	std::map<string, ID3D11ShaderResourceView*> NomalTexturesList;
	//애니메이션 리스트
	std::map<string, LoadAnimation*> AnimationList;
private:
	///로드 함수들
	//텍스쳐를 로드한다
	ID3D11ShaderResourceView* Load_Textures(LPCTSTR path);
	//ASE 파일로된 모델을 로드한다
	LoadMesh* Load_MODELS_ASE(string path,string ReName);
	//FBX 파일로된 모델을 로드한다
	LoadMesh* Load_MODELS_FBX(string path, string ReName);
	//애니메이션 로드
	LoadAnimation* Load_Animation(std::list<hsAnimation*>& anim, int keycCount);
};



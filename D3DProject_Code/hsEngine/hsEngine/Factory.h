#pragma once

//오브젝트를 생성 하고 내보내주는 펙토리 클래스
//편리함을 위해서 펙토리 클래스안에 리소스 매니저가 들어가있다

class Factory
{

public:
	Factory();
	~Factory();
	void init(hsEngine* _ENGINE);

	/// <summary>
	/// 여러개의 매쉬로 만들어지는 오브젝트를 생성 (스키닝 x)
	/// </summary>
	/// <typeparam name="T">생성할 오브젝트 타입</typeparam>
	/// <param name="_bulider">리소스 정보</param>
	/// <returns></returns>
	template<typename T>
	T* CreateObjectList(Bulider _bulider, float x, float y, float z);

	/// <summary>
	/// 여러개의 매쉬로 만들어지는 오브젝트 생성(스키닝 O)
	/// 스키닝이 되는것은 본 리스트가있을것이니 한개의 매쉬로 이루어진 것은없다
	/// </summary>
	/// <typeparam name="T">생성할 오브젝트 타입</typeparam>
	/// <param name="_bulider">리소스 정보</param>
	/// <returns></returns>
	template<typename T>
	T* CreateSkinnedObject(Bulider _bulider, float x, float y, float z);

	/// <summary>
	/// 한개의 오브젝트로 이루어진 매쉬를 생성
	/// </summary>
	/// <typeparam name="T">생성할 오브젝트 타입</typeparam>
	/// <param name="_bulider">리소스 정보</param>
	/// <returns></returns>
	template<typename T>
	T* CreateObject(Bulider _bulider, float x, float y, float z);


	//리소스를 넣어준다
	void SetResourece(LOAD_TYPE Type, string _path,string ReName);
	ID3D11ShaderResourceView* GetTexture(string Name);

	//리소스 경로가 길수도있기때문에 미리 경로를 받아놓는다
	void SetLoadPath(LOAD_TYPE Type, string _path);
	//쉐이더 경로를 미리 지정함
	void SetShaderLoadPath(string _path);

	//매쉬 삭제
	void ClearMesh(string Name);
	//애니메이션 삭제
	void ClearAnimation(string Name);

	//쉐이더 데이터를 가져옴
	ShaderData GetShader(string Name);

	///엔진안에서 기본으로 제공하는 오브젝트들
	//카메라
	hsCamera*		Create_Camera(float x = 0, float y = 0, float z = 0);
	//라이트
	LightManager*	Create_LightManager(float x = 0, float y = 0, float z = 0);
	LightObject*	Create_LightObj(int Type,float x = 0, float y = 0, float z = 0);
	hsDirLight*		Create_DirLight(float x = 0, float y = 0, float z = 0);
	//텍스쳐 오브젝트
	ParticleSystem* Create_Particle(Bulider _bulider, float x = 0, float y = 0, float z = 0);
	TextureObj*		Create_TextureObj(Bulider _bulider,float x = 0, float y = 0, float z = 0);
	Image*			Create_Image(Bulider _bulider, float x = 0, float y = 0, float z = 0);
	SkyBox*			Create_SkyBox(float x=0, float y = 0, float z = 0);
	Grid*			Create_Grid(float SizeX,float SizeY,XMFLOAT4 Color = XMFLOAT4(1,1,1,1),XMFLOAT3 Pos= XMFLOAT3(0,0,0));

	static std::vector<ID3D11InputLayout*>LayoutList;
private:

	//엔진 객체
	hsEngine* ENGINE;

	//리소스매니저 이클래스 안에서 자체적으로 만들것
	ResourceManager* Resource;

	//쉐이더 매니저
	ShaderManager* shaderManager;
};

template<typename T>
inline T* Factory::CreateObjectList(Bulider _bulider, float x, float y, float z)
{
	//////최상위 오브젝트 생성
	LoadMesh*					Mesh		= Resource->GetMesh(_bulider.mesh);
	ID3D11ShaderResourceView*	Textures	= Resource->GetTexture(_bulider.Textures);
	ID3D11ShaderResourceView*	Nomal		= Resource->GetNomalTexture(_bulider.Nomal);
	ID3D11ShaderResourceView*	Mask;
	ID3D11ShaderResourceView*	Sub			= Resource->GetTexture(_bulider.SubTextures);

	ShaderData shader = shaderManager->GetShader(_bulider.shader);
	ShaderData DebugShader = shaderManager->GetShader("Debug");

	hsMeshList* TopObj = new T(x, y, z);
	TopObj->Initialize(ENGINE);


	//최상위 밑으로 하위 오브젝트 생성하기
	for (int i = 0; i < Mesh->meshList.size(); i++)
	{
		LoadData* data = Mesh->meshList[i];

		//혹시라도 버텍스가없는오브젝트가 온다면 그냥 리턴
		if (data->vertex_pos.size() == 0) { continue; };

		HsMesh* temp = new HsMesh();
		temp->SetShaderData(shader);
		temp->SetDebugShader(DebugShader);
		temp->init(ENGINE, data);

		//노말 텍스쳐가 있다면
		if (Nomal != nullptr)
		{
			temp->SetNomalTextures(Nomal);
		}

		//텍스쳐 정보가있다면 넣어준다
		if (Textures != nullptr)
		{
			temp->SetTextures(Textures);
		}

		//서브텍스쳐가 있다면
		if (Sub != nullptr)
		{
			temp->SetSubTextures(Sub);
		}

		TopObj->ChildPush(temp->MyName, temp);
	}


	//이렇게 만든 오브젝트들을 서로 연결
	TopObj->LinkMesh();

	T* TMeshList = static_cast<T*>(TopObj);

	return TMeshList;
}

template<typename T>
inline T* Factory::CreateSkinnedObject(Bulider _bulider, float x, float y, float z)
{
	//리소스 셋팅
	LoadMesh*					Mesh		= Resource->GetMesh(_bulider.mesh);
	ID3D11ShaderResourceView*	Textures	= Resource->GetTexture(_bulider.Textures);
	ID3D11ShaderResourceView*	Nomal		= Resource->GetNomalTexture(_bulider.Nomal);	
	ID3D11ShaderResourceView*	Mask		= nullptr;
	ID3D11ShaderResourceView*	Sky			= nullptr;
	//LoadAnimation*				Anim		= Resource->GetAnimation(_bulider.Anim);

	//쉐이더 
	ShaderData ShData			= shaderManager->GetShader("Skinning");
	ShaderData DebugShaderData	= shaderManager->GetShader("Debug");


	//////최상위 오브젝트 생성
	Skinning* TopObj = new T(x, y, z);
	
	TopObj->SetDebugShader(DebugShaderData);
	TopObj->init(ENGINE);

	//최상위 밑으로 하위 오브젝트 생성하기
	for (int i = 0; i < Mesh->meshList.size(); i++)
	{
		hsBone* temp = new hsBone();
		LoadData* data = Mesh->meshList[i];

		temp->SetShaderData(ShData);
		temp->Initialize(ENGINE, data);

		//노말
		if (Nomal != nullptr)
		{
			temp->SetNomalTextures(Nomal);
		}

		//텍스쳐
		if (Textures != nullptr)
		{
			temp->SetTextures(Textures);
		}

		//최상위 객체 아래로 넣어준다
		TopObj->ChildPush(temp->MyName, temp);
	}

	//이렇게 만든 오브젝트들을 서로 연결
	TopObj->LinkMesh();

	//애니메이션 정보가 있다면 넣어준다
	for (int i = 0; i < _bulider.AnimList.size();i++) 
	{
		LoadAnimation* Anim = Resource->GetAnimation(_bulider.AnimList[i]);
		TopObj->inputTotalAnimeData(_bulider.AnimList[i],Anim);
	}

	T* Tmesh = static_cast<T*>(TopObj);

	return Tmesh;
}

template<typename T>
inline T* Factory::CreateObject(Bulider _bulider, float x, float y, float z)
{
	//ASE를 불러올것인지 FBX를 불러올것인지
	
	LoadMesh*					mesh		= Resource->GetMesh(_bulider.mesh);
	ID3D11ShaderResourceView*	Textures	= Resource->GetTexture(_bulider.Textures);
	ID3D11ShaderResourceView*	Nomal		= Resource->GetNomalTexture(_bulider.Nomal);
	ID3D11ShaderResourceView*	Mask		= nullptr;
	ID3D11ShaderResourceView*	Sub			= Resource->GetTexture(_bulider.SubTextures);

	ShaderData shader			= shaderManager->GetShader(_bulider.shader);
	ShaderData DebugShaderData	= shaderManager->GetShader("Debug");


	//LoadData* data = Mesh->meshList[0];
	//LoadData* data = Resource->LoadList["test"];
	//한개의 매쉬를 만든다

	LoadData* data = mesh->meshList[0];
	HsMesh* temp = new T(x,y,z);
	temp->SetShaderData(shader);
	temp->SetDebugShader(DebugShaderData);
	temp->init(ENGINE, data);
	//레이아웃 정보를 넘겨줌
	//temp->SetLayout(Layout);
	//노말 텍스쳐가 있다면 넣어준다w
	if (Nomal != nullptr)
	{
		temp->SetNomalTextures(Nomal);
	}

	//텍스쳐 정보가있다면 넣어준다
	if (Textures != nullptr)
	{
		temp->SetTextures(Textures);
	}

	if (Sub != nullptr)
	{
		temp->SetSubTextures(Sub);
	}

	
	//반환할 타입으로 다운캐스팅 
	T* Tmesh = static_cast<T*>(temp);

	//위치 조정
	//temp->transfrom.SetPosition(x, y, z);
	Tmesh->transfrom.isHierarchy = false;
	return Tmesh;
}


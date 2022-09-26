#pragma once

//������Ʈ�� ���� �ϰ� �������ִ� ���丮 Ŭ����
//������ ���ؼ� ���丮 Ŭ�����ȿ� ���ҽ� �Ŵ����� ���ִ�

class Factory
{

public:
	Factory();
	~Factory();
	void init(hsEngine* _ENGINE);

	/// <summary>
	/// �������� �Ž��� ��������� ������Ʈ�� ���� (��Ű�� x)
	/// </summary>
	/// <typeparam name="T">������ ������Ʈ Ÿ��</typeparam>
	/// <param name="_bulider">���ҽ� ����</param>
	/// <returns></returns>
	template<typename T>
	T* CreateObjectList(Bulider _bulider, float x, float y, float z);

	/// <summary>
	/// �������� �Ž��� ��������� ������Ʈ ����(��Ű�� O)
	/// ��Ű���� �Ǵ°��� �� ����Ʈ���������̴� �Ѱ��� �Ž��� �̷���� ��������
	/// </summary>
	/// <typeparam name="T">������ ������Ʈ Ÿ��</typeparam>
	/// <param name="_bulider">���ҽ� ����</param>
	/// <returns></returns>
	template<typename T>
	T* CreateSkinnedObject(Bulider _bulider, float x, float y, float z);

	/// <summary>
	/// �Ѱ��� ������Ʈ�� �̷���� �Ž��� ����
	/// </summary>
	/// <typeparam name="T">������ ������Ʈ Ÿ��</typeparam>
	/// <param name="_bulider">���ҽ� ����</param>
	/// <returns></returns>
	template<typename T>
	T* CreateObject(Bulider _bulider, float x, float y, float z);


	//���ҽ��� �־��ش�
	void SetResourece(LOAD_TYPE Type, string _path,string ReName);
	ID3D11ShaderResourceView* GetTexture(string Name);

	//���ҽ� ��ΰ� ������ֱ⶧���� �̸� ��θ� �޾Ƴ��´�
	void SetLoadPath(LOAD_TYPE Type, string _path);
	//���̴� ��θ� �̸� ������
	void SetShaderLoadPath(string _path);

	//�Ž� ����
	void ClearMesh(string Name);
	//�ִϸ��̼� ����
	void ClearAnimation(string Name);

	//���̴� �����͸� ������
	ShaderData GetShader(string Name);

	///�����ȿ��� �⺻���� �����ϴ� ������Ʈ��
	//ī�޶�
	hsCamera*		Create_Camera(float x = 0, float y = 0, float z = 0);
	//����Ʈ
	LightManager*	Create_LightManager(float x = 0, float y = 0, float z = 0);
	LightObject*	Create_LightObj(int Type,float x = 0, float y = 0, float z = 0);
	hsDirLight*		Create_DirLight(float x = 0, float y = 0, float z = 0);
	//�ؽ��� ������Ʈ
	ParticleSystem* Create_Particle(Bulider _bulider, float x = 0, float y = 0, float z = 0);
	TextureObj*		Create_TextureObj(Bulider _bulider,float x = 0, float y = 0, float z = 0);
	Image*			Create_Image(Bulider _bulider, float x = 0, float y = 0, float z = 0);
	SkyBox*			Create_SkyBox(float x=0, float y = 0, float z = 0);
	Grid*			Create_Grid(float SizeX,float SizeY,XMFLOAT4 Color = XMFLOAT4(1,1,1,1),XMFLOAT3 Pos= XMFLOAT3(0,0,0));

	static std::vector<ID3D11InputLayout*>LayoutList;
private:

	//���� ��ü
	hsEngine* ENGINE;

	//���ҽ��Ŵ��� ��Ŭ���� �ȿ��� ��ü������ �����
	ResourceManager* Resource;

	//���̴� �Ŵ���
	ShaderManager* shaderManager;
};

template<typename T>
inline T* Factory::CreateObjectList(Bulider _bulider, float x, float y, float z)
{
	//////�ֻ��� ������Ʈ ����
	LoadMesh*					Mesh		= Resource->GetMesh(_bulider.mesh);
	ID3D11ShaderResourceView*	Textures	= Resource->GetTexture(_bulider.Textures);
	ID3D11ShaderResourceView*	Nomal		= Resource->GetNomalTexture(_bulider.Nomal);
	ID3D11ShaderResourceView*	Mask;
	ID3D11ShaderResourceView*	Sub			= Resource->GetTexture(_bulider.SubTextures);

	ShaderData shader = shaderManager->GetShader(_bulider.shader);
	ShaderData DebugShader = shaderManager->GetShader("Debug");

	hsMeshList* TopObj = new T(x, y, z);
	TopObj->Initialize(ENGINE);


	//�ֻ��� ������ ���� ������Ʈ �����ϱ�
	for (int i = 0; i < Mesh->meshList.size(); i++)
	{
		LoadData* data = Mesh->meshList[i];

		//Ȥ�ö� ���ؽ������¿�����Ʈ�� �´ٸ� �׳� ����
		if (data->vertex_pos.size() == 0) { continue; };

		HsMesh* temp = new HsMesh();
		temp->SetShaderData(shader);
		temp->SetDebugShader(DebugShader);
		temp->init(ENGINE, data);

		//�븻 �ؽ��İ� �ִٸ�
		if (Nomal != nullptr)
		{
			temp->SetNomalTextures(Nomal);
		}

		//�ؽ��� �������ִٸ� �־��ش�
		if (Textures != nullptr)
		{
			temp->SetTextures(Textures);
		}

		//�����ؽ��İ� �ִٸ�
		if (Sub != nullptr)
		{
			temp->SetSubTextures(Sub);
		}

		TopObj->ChildPush(temp->MyName, temp);
	}


	//�̷��� ���� ������Ʈ���� ���� ����
	TopObj->LinkMesh();

	T* TMeshList = static_cast<T*>(TopObj);

	return TMeshList;
}

template<typename T>
inline T* Factory::CreateSkinnedObject(Bulider _bulider, float x, float y, float z)
{
	//���ҽ� ����
	LoadMesh*					Mesh		= Resource->GetMesh(_bulider.mesh);
	ID3D11ShaderResourceView*	Textures	= Resource->GetTexture(_bulider.Textures);
	ID3D11ShaderResourceView*	Nomal		= Resource->GetNomalTexture(_bulider.Nomal);	
	ID3D11ShaderResourceView*	Mask		= nullptr;
	ID3D11ShaderResourceView*	Sky			= nullptr;
	//LoadAnimation*				Anim		= Resource->GetAnimation(_bulider.Anim);

	//���̴� 
	ShaderData ShData			= shaderManager->GetShader("Skinning");
	ShaderData DebugShaderData	= shaderManager->GetShader("Debug");


	//////�ֻ��� ������Ʈ ����
	Skinning* TopObj = new T(x, y, z);
	
	TopObj->SetDebugShader(DebugShaderData);
	TopObj->init(ENGINE);

	//�ֻ��� ������ ���� ������Ʈ �����ϱ�
	for (int i = 0; i < Mesh->meshList.size(); i++)
	{
		hsBone* temp = new hsBone();
		LoadData* data = Mesh->meshList[i];

		temp->SetShaderData(ShData);
		temp->Initialize(ENGINE, data);

		//�븻
		if (Nomal != nullptr)
		{
			temp->SetNomalTextures(Nomal);
		}

		//�ؽ���
		if (Textures != nullptr)
		{
			temp->SetTextures(Textures);
		}

		//�ֻ��� ��ü �Ʒ��� �־��ش�
		TopObj->ChildPush(temp->MyName, temp);
	}

	//�̷��� ���� ������Ʈ���� ���� ����
	TopObj->LinkMesh();

	//�ִϸ��̼� ������ �ִٸ� �־��ش�
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
	//ASE�� �ҷ��ð����� FBX�� �ҷ��ð�����
	
	LoadMesh*					mesh		= Resource->GetMesh(_bulider.mesh);
	ID3D11ShaderResourceView*	Textures	= Resource->GetTexture(_bulider.Textures);
	ID3D11ShaderResourceView*	Nomal		= Resource->GetNomalTexture(_bulider.Nomal);
	ID3D11ShaderResourceView*	Mask		= nullptr;
	ID3D11ShaderResourceView*	Sub			= Resource->GetTexture(_bulider.SubTextures);

	ShaderData shader			= shaderManager->GetShader(_bulider.shader);
	ShaderData DebugShaderData	= shaderManager->GetShader("Debug");


	//LoadData* data = Mesh->meshList[0];
	//LoadData* data = Resource->LoadList["test"];
	//�Ѱ��� �Ž��� �����

	LoadData* data = mesh->meshList[0];
	HsMesh* temp = new T(x,y,z);
	temp->SetShaderData(shader);
	temp->SetDebugShader(DebugShaderData);
	temp->init(ENGINE, data);
	//���̾ƿ� ������ �Ѱ���
	//temp->SetLayout(Layout);
	//�븻 �ؽ��İ� �ִٸ� �־��ش�w
	if (Nomal != nullptr)
	{
		temp->SetNomalTextures(Nomal);
	}

	//�ؽ��� �������ִٸ� �־��ش�
	if (Textures != nullptr)
	{
		temp->SetTextures(Textures);
	}

	if (Sub != nullptr)
	{
		temp->SetSubTextures(Sub);
	}

	
	//��ȯ�� Ÿ������ �ٿ�ĳ���� 
	T* Tmesh = static_cast<T*>(temp);

	//��ġ ����
	//temp->transfrom.SetPosition(x, y, z);
	Tmesh->transfrom.isHierarchy = false;
	return Tmesh;
}


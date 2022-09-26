#pragma once

///Mesh �� ������ �������ִ� MeshList 
class hsMeshList :public GameObject
{

public:
	//�ڽİ�ü �ε���
	int Child_index;

	//������Ʈ�� ȭ�鿡 �׸����ΰ�
	bool isRender;

	//������Ʈ�� ����� ������ �׷��ٰ��ΰ�
	bool isDebug;


public:
	///�⺻ ������
	hsMeshList();
	///�⺻ ��ġ ������
	hsMeshList(float X,float Y,float Z);

	virtual~hsMeshList();

	
	virtual void Initialize(hsEngine* ENGINE);
	///������
	virtual void Render(const hsViewData& view);
	///������Ʈ
	virtual void Update();
	///�����
	virtual void Debug();
	///������Ʈ ����
	virtual void Delete();

	
	//���� ������Ʈ�� �ִ´�
	void SetRasterizerState(ID3D11RasterizerState* RenderState);

	//Mesh�� �̸��� �Է��ؼ� �ִ´�
	void ChildPush(std::string name, GameObject* mesh);

	//Mesh�� �̸��� �Է����� �ʰ� �ִ´�
	void ChildPush(GameObject* mesh);

	//���� ������ �ִ� �Ž����� ���̾����Ű�� ��ũ�����ش�
	void LinkMesh();

	//�ڽ� Mesh�� �̸����� ã�Ƽ� �����´�
	HsMesh* FindChildMesh(std::string Name);

	//�ڽ� Mesh�� �̸���ã�Ƽ� �׾Ʒ��� �ִ´�
	void inputMesh_Child(std::string Name, HsMesh* mesh);

protected:
	hsEngine* ENGINE;
	std::map<std::string,GameObject*> MeshList;

	///�ڽİ�ü���� ������ǥ�� ���Ӱ� ���Ѵ�
	void Child_Local_Update(Transform* Tr);

	///�ڽİ�ü���� ������ǥ�� ���Ӱ� ���Ѵ�
	void Child_World_Update(Transform* Tr);
};
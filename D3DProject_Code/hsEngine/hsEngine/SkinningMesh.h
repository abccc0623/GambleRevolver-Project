#pragma once

class MeshAnimation;
class hsBone;
class hsEngine;
class Skinning : public GameObject, public MeshAnimation, public Debugging
{
public:
	///�ڽİ�ü �ε���
	int Child_index;

	///������Ʈ�� ȭ�鿡 �׸����ΰ�
	bool isRender;

	///������Ʈ�� ����� ������ �׷��ٰ��ΰ�
	bool isDebug;



	virtual void init(hsEngine* ENGINE);
	//������
	virtual void Render(const hsViewData& view);
	//������Ʈ
	virtual void Update();
	//�����
	virtual void Debug();
	//������Ʈ ����
	virtual void Delete();
	
	//Mesh�� �̸��� �Է��ؼ� �ִ´�
	void ChildPush(std::string name, hsBone* mesh);

	//Mesh�� �̸��� �Է����� �ʰ� �ִ´�
	void ChildPush(hsBone* mesh);

	//���� ������ �ִ� �Ž����� ���̾����Ű�� ��ũ�����ش�
	void LinkMesh();

	//�ڽ� Mesh�� �̸����� ã�Ƽ� �����´�
	hsBone* FindChildMesh(std::string Name);

	//�ڽ� Mesh�� �̸���ã�Ƽ� �׾Ʒ��� �ִ´�
	void inputMesh_Child(std::string Name, hsBone* mesh);

	//�ڽİ�ü���� ������ǥ�� ���Ӱ� ���Ѵ�
	void Child_Local_Update(Transform* Tr);

	
	//�ڽİ�ü���� ������ǥ�� ���Ӱ� ���Ѵ�
	void Child_World_Update(Transform* Tr);

	//���� ��Ų�� �������� ����� 
	void Create_Skin_offset(hsBone* Skin);

	//���Ʒ� �ڽ����� ������Ʈ�� �ִ´�
	void Link_Bone_GameObject(string mBoneName,GameObject* obj);
	//��Ų������Ʈ ����Ʈ
	map<string, hsBone*>	SkinList;
private:

protected:
	//�� ������Ʈ ����Ʈ
	std::map<std::string, hsBone*> BoneList;
	hsEngine* ENGINE;
};
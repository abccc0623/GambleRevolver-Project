#pragma once

class MeshAnimation;
class hsBone;
class hsEngine;
class Skinning : public GameObject, public MeshAnimation, public Debugging
{
public:
	///자식객체 인덱스
	int Child_index;

	///오브젝트를 화면에 그릴것인가
	bool isRender;

	///오브젝트의 디버깅 정보를 그려줄것인가
	bool isDebug;



	virtual void init(hsEngine* ENGINE);
	//렌더링
	virtual void Render(const hsViewData& view);
	//업데이트
	virtual void Update();
	//디버깅
	virtual void Debug();
	//오브젝트 삭제
	virtual void Delete();
	
	//Mesh의 이름을 입력해서 넣는다
	void ChildPush(std::string name, hsBone* mesh);

	//Mesh의 이름을 입력하지 않고 넣는다
	void ChildPush(hsBone* mesh);

	//내가 가지고 있는 매쉬들을 하이어라이키로 링크시켜준다
	void LinkMesh();

	//자식 Mesh를 이름으로 찾아서 가져온다
	hsBone* FindChildMesh(std::string Name);

	//자식 Mesh의 이름을찾아서 그아래로 넣는다
	void inputMesh_Child(std::string Name, hsBone* mesh);

	//자식객체들의 로컬좌표를 새롭게 구한다
	void Child_Local_Update(Transform* Tr);

	
	//자식객체들의 월드좌표를 새롭게 구한다
	void Child_World_Update(Transform* Tr);

	//본과 스킨의 오프셋을 만든다 
	void Create_Skin_offset(hsBone* Skin);

	//본아래 자식으로 오브젝트를 넣는다
	void Link_Bone_GameObject(string mBoneName,GameObject* obj);
	//스킨오브젝트 리스트
	map<string, hsBone*>	SkinList;
private:

protected:
	//본 오브젝트 리스트
	std::map<std::string, hsBone*> BoneList;
	hsEngine* ENGINE;
};
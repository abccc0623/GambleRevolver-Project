#pragma once

///Mesh 를 여러개 가지고있는 MeshList 
class hsMeshList :public GameObject
{

public:
	//자식객체 인덱스
	int Child_index;

	//오브젝트를 화면에 그릴것인가
	bool isRender;

	//오브젝트의 디버깅 정보를 그려줄것인가
	bool isDebug;


public:
	///기본 생성자
	hsMeshList();
	///기본 위치 생성자
	hsMeshList(float X,float Y,float Z);

	virtual~hsMeshList();

	
	virtual void Initialize(hsEngine* ENGINE);
	///렌더링
	virtual void Render(const hsViewData& view);
	///업데이트
	virtual void Update();
	///디버깅
	virtual void Debug();
	///오브젝트 삭제
	virtual void Delete();

	
	//랜더 스테이트를 넣는다
	void SetRasterizerState(ID3D11RasterizerState* RenderState);

	//Mesh의 이름을 입력해서 넣는다
	void ChildPush(std::string name, GameObject* mesh);

	//Mesh의 이름을 입력하지 않고 넣는다
	void ChildPush(GameObject* mesh);

	//내가 가지고 있는 매쉬들을 하이어라이키로 링크시켜준다
	void LinkMesh();

	//자식 Mesh를 이름으로 찾아서 가져온다
	HsMesh* FindChildMesh(std::string Name);

	//자식 Mesh의 이름을찾아서 그아래로 넣는다
	void inputMesh_Child(std::string Name, HsMesh* mesh);

protected:
	hsEngine* ENGINE;
	std::map<std::string,GameObject*> MeshList;

	///자식객체들의 로컬좌표를 새롭게 구한다
	void Child_Local_Update(Transform* Tr);

	///자식객체들의 월드좌표를 새롭게 구한다
	void Child_World_Update(Transform* Tr);
};
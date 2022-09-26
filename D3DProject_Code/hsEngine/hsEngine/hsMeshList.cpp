#include "EnginePCH.h"
#include "GameObject.h"
#include "hsMeshList.h"

hsMeshList::hsMeshList()
{
	transfrom.Position = { 0,0,0 };
	transfrom.Rotation = { 0,0,0 };
	transfrom.Scale = { 1,1,1 };
	transfrom.isHierarchy = false;
}

hsMeshList::hsMeshList(float X, float Y, float Z)
{
	transfrom.Position = { X,Y,Z };
	transfrom.Rotation = { 0,0,0 };
	transfrom.Scale = { 1,1,1 };
	transfrom.isHierarchy = false;
}

hsMeshList::~hsMeshList()
{

}

void hsMeshList::Initialize(hsEngine* _ENGINE)
{
	ENGINE = _ENGINE;
}

void hsMeshList::Render(const hsViewData& view)
{
	std::map<string, GameObject*>::iterator it = MeshList.begin();
	for (it; it != MeshList.end(); it++)
	{
		it->second->SetRasterizerState(m_pRenderstate);
		it->second->Render(view);
	}
}

void hsMeshList::Update()
{
	///자식객체들의 월드좌표들은 업데이트 해준다

	Child_World_Update(&transfrom);

	std::map<string, GameObject*>::iterator it = MeshList.begin();
	for (it; it != MeshList.end(); it++)
	{
		it->second->Update();
	}
}

void hsMeshList::Debug()
{
	std::map<string, GameObject*>::iterator it = MeshList.begin();
	for (it; it != MeshList.end(); it++)
	{
		it->second->Debug();
	}
}

void hsMeshList::Delete()
{
	std::map<string, GameObject*>::iterator it = MeshList.begin();
	for (it; it != MeshList.end(); it++)
	{
		delete (it->second);
	}
}

void hsMeshList::SetRasterizerState(ID3D11RasterizerState* RenderState)
{
	std::map<string, GameObject*>::iterator it = MeshList.begin();
	for (it; it != MeshList.end(); it++)
	{
		it->second->SetRasterizerState(RenderState);
	}
}

void hsMeshList::ChildPush(std::string name, GameObject* mesh)
{
	///이름을 입력하고 map에 넣는다
	///이것은 모델을 연결시켜줄때 이름값이 중요하므로
	mesh->transfrom.isHierarchy = true;
	pair<std::string, GameObject*> temp(name, mesh);
	MeshList.insert(temp);

	Child_Local_Update(&transfrom);
}

void hsMeshList::ChildPush(GameObject* mesh)
{
	///이름을 입력하지 않고 map에 넣는다
	///이건 그냥 모델이 아닌 박스와 박스끼리 연결시키는 용도일때 굳이 이름을 넣어줄필요없으니
	std::string hsStr = std::to_string(Child_index);
	mesh->transfrom.isHierarchy = true;

	
	MeshList.insert(pair<std::string, GameObject*>(hsStr, mesh));

	//오브젝트가 들어왔으니 로컬좌표들을 새롭게 셋팅해준다
	Child_Local_Update(&transfrom);
	//
	////링크도 새롭게..
	LinkMesh();
}

void hsMeshList::LinkMesh()
{
	/// 이작업은 처음 모델을 불러왔을떄 해주는게 좋을것같다 만약 객체를 여러개 생성한다고했을떄 한번만 연결해주면 되는걸
	/// 여러번 작업을 해주고있으니... 이건 나중에 바꾸자


	///오브젝트를 만들고 객체들이 	MashList로 들어왔다 
	///이놈들을 서로 이름값을 비교하여 자식과 부모관계를 연결시켜준다

	std::map<string, GameObject*>::iterator it = MeshList.begin();
	for (it; it != MeshList.end(); it++)
	{
		///가장 상위 오브젝트들은 부모가없는 객체들인데 이놈들을 조정하는게 아니라
		///MeshList 오브젝트의 하위 계층으로 놓고 전체를 움직일땐 MeshList의 Transfrom만 움직인다
		if (it->second->ParentName == "TOP_OBJ")
		{
			transfrom.ChildTR.push_back( &(it->second->transfrom));
			it->second->transfrom.ParentTR = &transfrom;
			it->second->transfrom.isHierarchy = true;
		}

		///상위 객체가 아닌 놈들은 부모와 자식을 찾고 서로 연결
		if (MeshList.find(it->second->ParentName) != MeshList.end())
		{
			it->second->transfrom.ParentTR = &(MeshList[it->second->ParentName]->transfrom);
			MeshList[it->second->ParentName]->transfrom.ChildTR.push_back( &(it->second->transfrom));
		}
	}


	///연결이 다 되었다면 자식객체들의 로컬좌표를 구한다
	Child_Local_Update(&transfrom);
}



HsMesh* hsMeshList::FindChildMesh(std::string Name)
{
	///이름으로 자식객체를 찾는다 없다면 false 리턴
	//int num = 0;
	//if (MeshList.find(Name) != MeshList.end())
	//{
	//	return MeshList[Name];
	//}
	//else
	//{
	//	return nullptr;
	//}
	return nullptr;
}

void hsMeshList::inputMesh_Child(std::string Name, HsMesh* mesh)
{
	HsMesh* temp = FindChildMesh(Name);

	mesh->transfrom.isHierarchy = true;

	mesh->transfrom.ParentTR = &(temp->transfrom);
	temp->transfrom.ChildTR.push_back(&(mesh->transfrom));
}

void hsMeshList::Child_Local_Update(Transform* Tr)
{
	///이작업은 한번만 해준다

	if (Tr->ParentTR != nullptr)
	{
		///나의 부모의 월드의 역행렬을 구함
		XMMATRIX TM2_1 = DirectX::XMMatrixInverse(nullptr, Tr->ParentTR->W_TM);

		///나의 로컬은  나의 월드 * 부모의 역행렬
		Tr->LocalTM = Tr->W_TM * TM2_1;
	}
	else
	{
		///나의 로컬은  나의 월드
		Tr->LocalTM = Tr->W_TM;
	}

	///자식이 하나도 없을때 재귀를 종료
	if (Tr->ChildTR.size() == 0)
	{
		return;
	}

	for (int i = 0; i < Tr->ChildTR.size(); i++)
	{
		Child_Local_Update(Tr->ChildTR[i]);
	}
}

void hsMeshList::Child_World_Update(Transform* Tr)
{
	///나의 월드 좌표는 나의 로컬 좌표 * 나의 TM * 부모의 월드좌표이다!
	if (Tr->ParentTR != nullptr)
	{
		Tr->WorldTM = Tr->GetMaster() * Tr->LocalTM * Tr->ParentTR->WorldTM;
	}
	else
	{
		Tr->WorldTM = Tr->GetMaster() * Tr->LocalTM;
	}

	///자식이 없다면 재귀 종료
	if (Tr->ChildTR.size() == 0)
	{
		return;
	}

	///자식객체들도 업데이트 해준다
	for (int i = 0; i < Tr->ChildTR.size(); i++)
	{
		Child_World_Update(Tr->ChildTR[i]);
	}
 }
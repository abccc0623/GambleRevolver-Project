#include "EnginePCH.h"
#include "SkinningMesh.h"

void Skinning::init(hsEngine* _ENGINE)
{
	ENGINE = _ENGINE;
	MeshAnimation::init(ENGINE);
	Debugging::init(_ENGINE);
}

void Skinning::Render(const hsViewData& view)
{
	//스키닝 오브젝트일때
	map<string, hsBone*>::iterator Skinit = SkinList.begin();
	for (Skinit; Skinit != SkinList.end(); Skinit++)
	{
		Skinit->second->Render(view);
	}
}

void Skinning::Update()
{
	Child_World_Update(&transfrom);

	std::map<string, hsBone*>::iterator it = BoneList.begin();
	for (it; it != BoneList.end(); it++)
	{
		hsBone* temp = it->second;
		temp->m_pRenderstate = m_pRenderstate;
		temp->Update();
	}


	map<string, hsBone*>::iterator Skinit = SkinList.begin();
	for (Skinit; Skinit != SkinList.end(); Skinit++)
	{
		Skinit->second->Update();
	}

	mWorld = transfrom.GetMaster();
}

void Skinning::Debug()
{
	Debugging::Update(mWorld);
}

void Skinning::Delete()
{
	std::map<string, hsBone*>::iterator it = BoneList.begin();
	for (it; it != BoneList.end(); it++)
	{
		delete (it->second);
	}
}



void Skinning::ChildPush(std::string name, hsBone* mesh)
{
	//스킨 오브젝트와 본오브젝트를 분리
	if (mesh->isSkinning == true)
	{
		SkinList.insert({name,mesh});
	}
	else if(mesh->isBone == true)
	{
		mesh->transfrom.isHierarchy = true;
		pair<std::string, hsBone*> temp(name, mesh);
		BoneList.insert(temp);
	}
}

void Skinning::ChildPush(hsBone* mesh)
{
	///이름을 입력하지 않고 map에 넣는다
	///이건 그냥 모델이 아닌 박스와 박스끼리 연결시키는 용도일때 굳이 이름을 넣어줄필요없으니
	std::string hsStr = std::to_string(Child_index);
	mesh->transfrom.isHierarchy = true;
	pair<std::string, hsBone*> temp(hsStr, mesh);
	BoneList.insert(temp);
}

void Skinning::LinkMesh()
{
	//본들의 계층구조를 연결

	// 이작업은 처음 모델을 불러왔을떄 해주는게 좋을것같다 만약 객체를 여러개 생성한다고했을떄 한번만 연결해주면 되는걸
	// 여러번 작업을 해주고있으니... 이건 나중에 바꾸자


	//오브젝트를 만들고 객체들이 	MashList로 들어왔다 
	//이놈들을 서로 이름값을 비교하여 자식과 부모관계를 연결시켜준다

	std::map<string, hsBone*>::iterator it = BoneList.begin();
	for (it; it != BoneList.end(); it++)
	{
		//가장 상위 오브젝트들은 부모가없는 객체들인데 이놈들을 조정하는게 아니라
		//MeshList 오브젝트의 하위 계층으로 놓고 전체를 움직일땐 MeshList의 Transfrom만 움직인다
		if (it->second->ParentName == "TOP_OBJ")
		{
			transfrom.ChildTR.push_back( &(it->second->transfrom) );
			it->second->transfrom.ParentTR = &transfrom;
			it->second->transfrom.isHierarchy = true;
		}

		//상위 객체가 아닌 놈들은 부모와 자식을 찾고 서로 연결
		if (BoneList.find(it->second->ParentName) != BoneList.end())
		{
			it->second->transfrom.ParentTR = &(BoneList[it->second->ParentName]->transfrom);
			BoneList[it->second->ParentName]->transfrom.ChildTR.push_back( &(it->second->transfrom) );
		}
	}

	//연결이 다 되었다면 자식객체들의 로컬좌표를 구한다
	Child_Local_Update(&transfrom);
	

	//본과 스킨매쉬의 오프셋을 구한다 스키닝을 하기위해
	map<string, hsBone*>::iterator Skinit = SkinList.begin();
	for (Skinit; Skinit != SkinList.end(); Skinit++)
	{
		Create_Skin_offset(Skinit->second);
	}

	
	MeshAnimation::SetChildBoneList(&BoneList);
}

hsBone* Skinning::FindChildMesh(std::string Name)
{
	///이름으로 자식객체를 찾는다 없다면 false 리턴

	if (BoneList.find(Name) != BoneList.end())
	{
		return BoneList[Name];
	}
	else
	{
		return nullptr;
	}
	return nullptr;
}

void Skinning::inputMesh_Child(std::string Name, hsBone* mesh)
{
	if (mesh->isBone == true)
	{
		hsBone* temp = FindChildMesh(Name);

		mesh->transfrom.isHierarchy = true;

		mesh->transfrom.ParentTR = &(temp->transfrom);
		temp->transfrom.ChildTR.push_back( &(mesh->transfrom));
	}

}

void Skinning::Child_Local_Update(Transform* Tr)
{
	//이작업은 한번만 해준다

	if (Tr->ParentTR != nullptr)
	{
		//나의 부모의 월드의 역행렬을 구함
		XMMATRIX TM2_1 = DirectX::XMMatrixInverse(nullptr, Tr->ParentTR->W_TM);

		//나의 로컬은  나의 월드 * 부모의 역행렬
		Tr->LocalTM = Tr->W_TM * TM2_1;
	}
	else
	{
		//나의 로컬은  나의 월드
		Tr->LocalTM = Tr->W_TM;
	}

	//자식이 하나도 없을때 재귀를 종료
	if (Tr->ChildTR.size() == 0)
	{
		return;
	}

	for (int i = 0; i < Tr->ChildTR.size(); i++)
	{
		Child_Local_Update(Tr->ChildTR[i]);
	}
}



void Skinning::Child_World_Update(Transform* Tr)
{
	//나의 월드 좌표는 나의 로컬 좌표 * 나의 TM * 부모의 월드좌표이다!
	if (Tr->ParentTR != nullptr)
	{
		Tr->WorldTM = Tr->GetMaster() * Tr->LocalTM * Tr->ParentTR->WorldTM;
	}
	else
	{
		Tr->WorldTM = Tr->GetMaster() * Tr->LocalTM;
	}

	//자식이 없다면 재귀 종료
	if (Tr->ChildTR.size() == 0)
	{
		return;
	}

	//자식객체들도 업데이트 해준다
	for (int i = 0; i < Tr->ChildTR.size(); i++)
	{
		Child_World_Update(Tr->ChildTR[i]);
	}
}

void Skinning::Create_Skin_offset(hsBone* Skin)
{
	Skin->BoneList.resize(Skin->BoneNumberList.size());
	std::map<string, int>::iterator Boneit = Skin->BoneNumberList.begin();
	for (Boneit; Boneit != Skin->BoneNumberList.end(); Boneit++)
	{
		string name = Boneit->first;
		int index = Boneit->second;
		Skin->BoneList[index] = FindChildMesh(name);
	}

	///오프셋 크기 만큼 재할당
	std::vector<hsBone*>::iterator it = Skin->BoneList.begin();
	for (it; it != Skin->BoneList.end() ; it++)
	{
		hsBone* tempBone = *(it);
	
		XMMATRIX _BoneNodeTM = tempBone->transfrom.W_TM;
	
		XMMATRIX _SkinWorldTM = Skin->transfrom.W_TM;
		XMMATRIX _SkinWorldTM_Inverse = XMMatrixInverse(nullptr, _SkinWorldTM);
	
		XMMATRIX _BoneoffsetTM = _BoneNodeTM * _SkinWorldTM_Inverse;
		XMMATRIX _BoneoffsetTM_Inverse = XMMatrixInverse(nullptr, _BoneoffsetTM);
	
	
		///구한 오프셋 TM을 스킨매쉬로 넘겨줌
		Skin->SetBoneSkinningTM(_BoneoffsetTM_Inverse);
	}
}

void Skinning::Link_Bone_GameObject(string mBoneName, GameObject* obj)
{
	hsBone* temp  =  FindBone(mBoneName);
	obj->SetParentTR(&(temp->transfrom));
}

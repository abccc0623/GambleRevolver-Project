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
	//��Ű�� ������Ʈ�϶�
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
	//��Ų ������Ʈ�� ��������Ʈ�� �и�
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
	///�̸��� �Է����� �ʰ� map�� �ִ´�
	///�̰� �׳� ���� �ƴ� �ڽ��� �ڽ����� �����Ű�� �뵵�϶� ���� �̸��� �־����ʿ������
	std::string hsStr = std::to_string(Child_index);
	mesh->transfrom.isHierarchy = true;
	pair<std::string, hsBone*> temp(hsStr, mesh);
	BoneList.insert(temp);
}

void Skinning::LinkMesh()
{
	//������ ���������� ����

	// ���۾��� ó�� ���� �ҷ������� ���ִ°� �����Ͱ��� ���� ��ü�� ������ �����Ѵٰ������� �ѹ��� �������ָ� �Ǵ°�
	// ������ �۾��� ���ְ�������... �̰� ���߿� �ٲ���


	//������Ʈ�� ����� ��ü���� 	MashList�� ���Դ� 
	//�̳���� ���� �̸����� ���Ͽ� �ڽİ� �θ���踦 ��������ش�

	std::map<string, hsBone*>::iterator it = BoneList.begin();
	for (it; it != BoneList.end(); it++)
	{
		//���� ���� ������Ʈ���� �θ𰡾��� ��ü���ε� �̳���� �����ϴ°� �ƴ϶�
		//MeshList ������Ʈ�� ���� �������� ���� ��ü�� �����϶� MeshList�� Transfrom�� �����δ�
		if (it->second->ParentName == "TOP_OBJ")
		{
			transfrom.ChildTR.push_back( &(it->second->transfrom) );
			it->second->transfrom.ParentTR = &transfrom;
			it->second->transfrom.isHierarchy = true;
		}

		//���� ��ü�� �ƴ� ����� �θ�� �ڽ��� ã�� ���� ����
		if (BoneList.find(it->second->ParentName) != BoneList.end())
		{
			it->second->transfrom.ParentTR = &(BoneList[it->second->ParentName]->transfrom);
			BoneList[it->second->ParentName]->transfrom.ChildTR.push_back( &(it->second->transfrom) );
		}
	}

	//������ �� �Ǿ��ٸ� �ڽİ�ü���� ������ǥ�� ���Ѵ�
	Child_Local_Update(&transfrom);
	

	//���� ��Ų�Ž��� �������� ���Ѵ� ��Ű���� �ϱ�����
	map<string, hsBone*>::iterator Skinit = SkinList.begin();
	for (Skinit; Skinit != SkinList.end(); Skinit++)
	{
		Create_Skin_offset(Skinit->second);
	}

	
	MeshAnimation::SetChildBoneList(&BoneList);
}

hsBone* Skinning::FindChildMesh(std::string Name)
{
	///�̸����� �ڽİ�ü�� ã�´� ���ٸ� false ����

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
	//���۾��� �ѹ��� ���ش�

	if (Tr->ParentTR != nullptr)
	{
		//���� �θ��� ������ ������� ����
		XMMATRIX TM2_1 = DirectX::XMMatrixInverse(nullptr, Tr->ParentTR->W_TM);

		//���� ������  ���� ���� * �θ��� �����
		Tr->LocalTM = Tr->W_TM * TM2_1;
	}
	else
	{
		//���� ������  ���� ����
		Tr->LocalTM = Tr->W_TM;
	}

	//�ڽ��� �ϳ��� ������ ��͸� ����
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
	//���� ���� ��ǥ�� ���� ���� ��ǥ * ���� TM * �θ��� ������ǥ�̴�!
	if (Tr->ParentTR != nullptr)
	{
		Tr->WorldTM = Tr->GetMaster() * Tr->LocalTM * Tr->ParentTR->WorldTM;
	}
	else
	{
		Tr->WorldTM = Tr->GetMaster() * Tr->LocalTM;
	}

	//�ڽ��� ���ٸ� ��� ����
	if (Tr->ChildTR.size() == 0)
	{
		return;
	}

	//�ڽİ�ü�鵵 ������Ʈ ���ش�
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

	///������ ũ�� ��ŭ ���Ҵ�
	std::vector<hsBone*>::iterator it = Skin->BoneList.begin();
	for (it; it != Skin->BoneList.end() ; it++)
	{
		hsBone* tempBone = *(it);
	
		XMMATRIX _BoneNodeTM = tempBone->transfrom.W_TM;
	
		XMMATRIX _SkinWorldTM = Skin->transfrom.W_TM;
		XMMATRIX _SkinWorldTM_Inverse = XMMatrixInverse(nullptr, _SkinWorldTM);
	
		XMMATRIX _BoneoffsetTM = _BoneNodeTM * _SkinWorldTM_Inverse;
		XMMATRIX _BoneoffsetTM_Inverse = XMMatrixInverse(nullptr, _BoneoffsetTM);
	
	
		///���� ������ TM�� ��Ų�Ž��� �Ѱ���
		Skin->SetBoneSkinningTM(_BoneoffsetTM_Inverse);
	}
}

void Skinning::Link_Bone_GameObject(string mBoneName, GameObject* obj)
{
	hsBone* temp  =  FindBone(mBoneName);
	obj->SetParentTR(&(temp->transfrom));
}

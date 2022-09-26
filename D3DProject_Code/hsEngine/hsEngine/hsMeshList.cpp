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
	///�ڽİ�ü���� ������ǥ���� ������Ʈ ���ش�

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
	///�̸��� �Է��ϰ� map�� �ִ´�
	///�̰��� ���� ��������ٶ� �̸����� �߿��ϹǷ�
	mesh->transfrom.isHierarchy = true;
	pair<std::string, GameObject*> temp(name, mesh);
	MeshList.insert(temp);

	Child_Local_Update(&transfrom);
}

void hsMeshList::ChildPush(GameObject* mesh)
{
	///�̸��� �Է����� �ʰ� map�� �ִ´�
	///�̰� �׳� ���� �ƴ� �ڽ��� �ڽ����� �����Ű�� �뵵�϶� ���� �̸��� �־����ʿ������
	std::string hsStr = std::to_string(Child_index);
	mesh->transfrom.isHierarchy = true;

	
	MeshList.insert(pair<std::string, GameObject*>(hsStr, mesh));

	//������Ʈ�� �������� ������ǥ���� ���Ӱ� �������ش�
	Child_Local_Update(&transfrom);
	//
	////��ũ�� ���Ӱ�..
	LinkMesh();
}

void hsMeshList::LinkMesh()
{
	/// ���۾��� ó�� ���� �ҷ������� ���ִ°� �����Ͱ��� ���� ��ü�� ������ �����Ѵٰ������� �ѹ��� �������ָ� �Ǵ°�
	/// ������ �۾��� ���ְ�������... �̰� ���߿� �ٲ���


	///������Ʈ�� ����� ��ü���� 	MashList�� ���Դ� 
	///�̳���� ���� �̸����� ���Ͽ� �ڽİ� �θ���踦 ��������ش�

	std::map<string, GameObject*>::iterator it = MeshList.begin();
	for (it; it != MeshList.end(); it++)
	{
		///���� ���� ������Ʈ���� �θ𰡾��� ��ü���ε� �̳���� �����ϴ°� �ƴ϶�
		///MeshList ������Ʈ�� ���� �������� ���� ��ü�� �����϶� MeshList�� Transfrom�� �����δ�
		if (it->second->ParentName == "TOP_OBJ")
		{
			transfrom.ChildTR.push_back( &(it->second->transfrom));
			it->second->transfrom.ParentTR = &transfrom;
			it->second->transfrom.isHierarchy = true;
		}

		///���� ��ü�� �ƴ� ����� �θ�� �ڽ��� ã�� ���� ����
		if (MeshList.find(it->second->ParentName) != MeshList.end())
		{
			it->second->transfrom.ParentTR = &(MeshList[it->second->ParentName]->transfrom);
			MeshList[it->second->ParentName]->transfrom.ChildTR.push_back( &(it->second->transfrom));
		}
	}


	///������ �� �Ǿ��ٸ� �ڽİ�ü���� ������ǥ�� ���Ѵ�
	Child_Local_Update(&transfrom);
}



HsMesh* hsMeshList::FindChildMesh(std::string Name)
{
	///�̸����� �ڽİ�ü�� ã�´� ���ٸ� false ����
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
	///���۾��� �ѹ��� ���ش�

	if (Tr->ParentTR != nullptr)
	{
		///���� �θ��� ������ ������� ����
		XMMATRIX TM2_1 = DirectX::XMMatrixInverse(nullptr, Tr->ParentTR->W_TM);

		///���� ������  ���� ���� * �θ��� �����
		Tr->LocalTM = Tr->W_TM * TM2_1;
	}
	else
	{
		///���� ������  ���� ����
		Tr->LocalTM = Tr->W_TM;
	}

	///�ڽ��� �ϳ��� ������ ��͸� ����
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
	///���� ���� ��ǥ�� ���� ���� ��ǥ * ���� TM * �θ��� ������ǥ�̴�!
	if (Tr->ParentTR != nullptr)
	{
		Tr->WorldTM = Tr->GetMaster() * Tr->LocalTM * Tr->ParentTR->WorldTM;
	}
	else
	{
		Tr->WorldTM = Tr->GetMaster() * Tr->LocalTM;
	}

	///�ڽ��� ���ٸ� ��� ����
	if (Tr->ChildTR.size() == 0)
	{
		return;
	}

	///�ڽİ�ü�鵵 ������Ʈ ���ش�
	for (int i = 0; i < Tr->ChildTR.size(); i++)
	{
		Child_World_Update(Tr->ChildTR[i]);
	}
 }
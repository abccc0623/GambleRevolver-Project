#include "EnginePCH.h"
#include "Collider.h"

//�ڽ� ����Ʈ
std::vector<hsBoxCollider*> Collider::BoxColliderList = std::vector<hsBoxCollider*>();
//��ü ����Ʈ
std::vector<hsSphereCollider*> Collider::SphereColliderList = std::vector<hsSphereCollider*>();
Collider::Collider()
{
	BoxCollider.BoundingBoxCollider = BoundingBox();
	SphereCollider.BoundingSphereCollider = BoundingSphere();
	MyIndex = 0;

	//��ü ����
	isRigidbody = false;

	//�浹 ����
	isEnter = false;
	isStay = false;
	isExit = false;
	isPastStay = false;
	MyColliderType = 0;
}

Collider::~Collider()
{
	//�ڱ��ڽ��� ����� ������

	switch (MyColliderType)
	{
	case BOX:
	{
		BoxColliderList[MyIndex] = nullptr;
		break;
	}
	case SPHERE:
	{
		SphereColliderList[MyIndex] = nullptr;
		break;
	}
	}
}

void Collider::SetBoundingBox(XMFLOAT3 _Pos, XMFLOAT3 _Extents)
{
	//�̹� �ٸ�Ÿ������ �����ߴٸ� �����Ű������
	if (MyColliderType != 0) { return; }
	MyColliderType = BOX;


	BoxCollider.BoundingBoxCollider.Center = _Pos;
	BoxCollider.BoundingBoxCollider.Extents = _Extents;

	///����Ʈ�� �ƹ��͵� ���ٸ� �׳� �ִ´�
	if (BoxColliderList.size() == 0)
	{
		BoxColliderList.push_back(&BoxCollider);
		MyIndex = 0;
	}

	///�ƴҰ�� ����ִ� ���͸� ã�Ƽ� �־��ش�
	for (int i = 0; i < BoxColliderList.size(); i++)
	{
		if (BoxColliderList[i] == nullptr)
		{
			BoxColliderList[i] = &BoxCollider;
			MyIndex = i;
			return;
		}
	}

	///���Ϳ� ��� ������ ���¶�� 10���� ������ ������� �־��ش�

	//���͸� ��������� ������
	int pushIndex = BoxColliderList.size();
	for (int j = 0; j < 10; j++)
	{
		//10���� ������ ���θ���� 
		BoxColliderList.push_back(nullptr);
	}
	BoxColliderList[pushIndex] = &BoxCollider;
	MyIndex = pushIndex;
}

void Collider::SetBoundingSphere(XMFLOAT3 _Pos, float _Radius, UINT type)
{
	//�̹� �ٸ�Ÿ������ �����ߴٸ� �����Ű������
	if (MyColliderType != 0) { return; }
	MyColliderType = SPHERE;
	SphereCollider.ObjType = (UINT)type;

	//���浹 ����ü�� �����
	SphereCollider.BoundingSphereCollider.Center = _Pos;
	SphereCollider.BoundingSphereCollider.Radius = _Radius;

	///����Ʈ�� �ƹ��͵� ���ٸ� �׳� �ִ´�
	if (SphereColliderList.size() == 0)
	{
		SphereColliderList.push_back(&SphereCollider);
		MyIndex = 0;
	}

	///�ƴҰ�� ����ִ� ���͸� ã�Ƽ� �־��ش�
	for (int i = 0; i < SphereColliderList.size(); i++)
	{
		if (SphereColliderList[i] == nullptr)
		{
			SphereColliderList[i] = &SphereCollider;
			CollisionList.push_back(&SphereCollider);
			MyIndex = i;
			return;
		}
	}

	///���Ϳ� ��� ������ ���¶�� 10���� ������ ������� �־��ش�

	//���͸� ��������� ������
	int pushIndex = SphereColliderList.size();
	for (int j = 0; j < 10; j++)
	{
		//10���� ������ ���θ���� 
		SphereColliderList.push_back(nullptr);
	}
	SphereColliderList[pushIndex] = &SphereCollider;
	MyIndex = pushIndex;
}

void Collider::Updata(XMFLOAT3 Pos)
{
	//���� ��ü�̸� �浹 �˻縦 ��������
	if (isRigidbody == true) { return; }

	int count = 0;
	switch (MyColliderType)
	{
	case BOX:
	{
		BoxCollision(Pos);
		break;
	}
	case SPHERE:
	{
		SphereCollision(Pos);
		break;
	}
	}





	if (isPastStay == false) //���� ������ �浹 ���ߴ�
	{
		//���� �浹����
		if (isStay == true)
		{
			//���������ӿ����� �浹�� ���߰� ����� �浹��
			isEnter = true;
			isExit = false;
		}
		else
		{
			//���������ӿ����� �浹�� ���߰� ���翡�� �浹�����ʾ�����
			isEnter = false;
			isExit = false;
		}
	}
	else if (isPastStay == true) //���������� �浹�ߴ�
	{
		//���� �浹����
		if (isStay == true)
		{
			//���������ӿ����� �浹���߰� ���翡�� �浹��
			isExit = false;
			isEnter = false;
		}
		else
		{
			//���������ӿ����� �浹���߰� ����� �浹����
			isExit = true;
			isEnter = false;
		}
	}


	//������ �浹���θ� �����ص�
	isPastStay = isStay;
}

void Collider::BoxCollision(XMFLOAT3 Pos)
{
	BoxCollider.BoundingBoxCollider.Center = Pos;
	///�ڽ��� ���� �浹üũ
	vector<hsBoxCollider*>::iterator it = BoxColliderList.begin();
	for (it; it != BoxColliderList.end(); it++)
	{
		//��������� ó������
		if ((*it) == nullptr)
		{
			continue;
		}

		//���ڽŰ��� �˻���������
		if ((*it) == &BoxCollider)
		{
			continue;
		}

		if (BoxCollider.BoundingBoxCollider.Intersects(  (*it)->BoundingBoxCollider  ) == true)
		{
			//�浹�� 
			isStay = true;
		}
		else
		{
			//�浹��������
			isStay = false;
		}
	}
}

void Collider::SphereCollision(XMFLOAT3 Pos)
{
	SphereCollider.BoundingSphereCollider.Center = Pos;
	int num = 0;
	SphereCollider.HitType = NoHit;
	///��ü�� ���� �浹 ����
	vector<hsSphereCollider*>::iterator it = SphereColliderList.begin();
	for (it; it != SphereColliderList.end(); it++)
	{

		//��������� ó������
		if ((*it) == nullptr)
		{
			continue;
		}

		//���ڽŰ��� �˻���������
		if (*it == &SphereCollider)
		{
			continue;
		}

		


		//�����浹�� ����ؾ��� �ϴ��� �׳���
		if (SphereCollider.BoundingSphereCollider.Intersects( (*it)->BoundingSphereCollider ) == true)
		{	
			SphereCollider.HitType |= (*it)->ObjType;

			//�浹�Ѱ�ü�� ����Ʈ�� ��Ƶ�
			//CollisionList.push_back((*it));
		}
	}


	if (SphereCollider.HitType == NoHit)
	{
		isStay = false;
	}
	else
	{
		isStay = true;
	}

}

bool Collider::OnCollisionEnter()
{
	//ó�� �浹������ �ѹ�
	return isEnter;
}



bool Collider::OnCollisionStay()
{
	//��� �浹���ϋ�
	return isStay;
}

bool Collider::OnCollisionExit()
{
	//�浹�� ��������
	return isExit;
}

void Collider::SetRigidbody(bool rigid)
{
	isRigidbody = rigid;
}



UINT Collider::GetCollision()
{
	return MYType;
}

void Collider::PushCollisionList(hsSphereCollider* col)
{
	for (int i = 0; i < CollisionList.size(); i++)
	{
		//������ �浹�ϴ����� ã�Ҵ�
		if (col == CollisionList[i])
		{

		}
	}



}









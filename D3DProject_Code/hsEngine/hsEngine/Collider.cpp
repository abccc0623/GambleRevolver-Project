#include "EnginePCH.h"
#include "Collider.h"

//박스 리스트
std::vector<hsBoxCollider*> Collider::BoxColliderList = std::vector<hsBoxCollider*>();
//구체 리스트
std::vector<hsSphereCollider*> Collider::SphereColliderList = std::vector<hsSphereCollider*>();
Collider::Collider()
{
	BoxCollider.BoundingBoxCollider = BoundingBox();
	SphereCollider.BoundingSphereCollider = BoundingSphere();
	MyIndex = 0;

	//강체 여부
	isRigidbody = false;

	//충돌 여부
	isEnter = false;
	isStay = false;
	isExit = false;
	isPastStay = false;
	MyColliderType = 0;
}

Collider::~Collider()
{
	//자기자신을 지우고 삭제함

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
	//이미 다른타입으로 지정했다면 실행시키지않음
	if (MyColliderType != 0) { return; }
	MyColliderType = BOX;


	BoxCollider.BoundingBoxCollider.Center = _Pos;
	BoxCollider.BoundingBoxCollider.Extents = _Extents;

	///리스트에 아무것도 없다면 그냥 넣는다
	if (BoxColliderList.size() == 0)
	{
		BoxColliderList.push_back(&BoxCollider);
		MyIndex = 0;
	}

	///아닐경우 비어있는 벡터를 찾아서 넣어준다
	for (int i = 0; i < BoxColliderList.size(); i++)
	{
		if (BoxColliderList[i] == nullptr)
		{
			BoxColliderList[i] = &BoxCollider;
			MyIndex = i;
			return;
		}
	}

	///벡터에 모두 가득찬 상태라면 10개의 공간을 더만들고 넣어준다

	//벡터를 더만들기전 사이즈
	int pushIndex = BoxColliderList.size();
	for (int j = 0; j < 10; j++)
	{
		//10개의 공간을 새로만들고 
		BoxColliderList.push_back(nullptr);
	}
	BoxColliderList[pushIndex] = &BoxCollider;
	MyIndex = pushIndex;
}

void Collider::SetBoundingSphere(XMFLOAT3 _Pos, float _Radius, UINT type)
{
	//이미 다른타입으로 지정했다면 실행시키지않음
	if (MyColliderType != 0) { return; }
	MyColliderType = SPHERE;
	SphereCollider.ObjType = (UINT)type;

	//원충돌 구조체를 만든다
	SphereCollider.BoundingSphereCollider.Center = _Pos;
	SphereCollider.BoundingSphereCollider.Radius = _Radius;

	///리스트에 아무것도 없다면 그냥 넣는다
	if (SphereColliderList.size() == 0)
	{
		SphereColliderList.push_back(&SphereCollider);
		MyIndex = 0;
	}

	///아닐경우 비어있는 벡터를 찾아서 넣어준다
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

	///벡터에 모두 가득찬 상태라면 10개의 공간을 더만들고 넣어준다

	//벡터를 더만들기전 사이즈
	int pushIndex = SphereColliderList.size();
	for (int j = 0; j < 10; j++)
	{
		//10개의 공간을 새로만들고 
		SphereColliderList.push_back(nullptr);
	}
	SphereColliderList[pushIndex] = &SphereCollider;
	MyIndex = pushIndex;
}

void Collider::Updata(XMFLOAT3 Pos)
{
	//내가 강체이면 충돌 검사를 하지않음
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





	if (isPastStay == false) //이전 프레임 충돌 안했다
	{
		//현재 충돌여부
		if (isStay == true)
		{
			//이전프레임에서는 충돌을 안했고 현재는 충돌중
			isEnter = true;
			isExit = false;
		}
		else
		{
			//이전프레임에서는 충돌을 안했고 현재에도 충돌하지않았을때
			isEnter = false;
			isExit = false;
		}
	}
	else if (isPastStay == true) //이전프레임 충돌했다
	{
		//현재 충돌여부
		if (isStay == true)
		{
			//이전프레임에서는 충돌을했고 현재에도 충돌중
			isExit = false;
			isEnter = false;
		}
		else
		{
			//이전프레임에서는 충돌을했고 현재는 충돌안함
			isExit = true;
			isEnter = false;
		}
	}


	//현재의 충돌여부를 저장해둠
	isPastStay = isStay;
}

void Collider::BoxCollision(XMFLOAT3 Pos)
{
	BoxCollider.BoundingBoxCollider.Center = Pos;
	///박스와 나의 충돌체크
	vector<hsBoxCollider*>::iterator it = BoxColliderList.begin();
	for (it; it != BoxColliderList.end(); it++)
	{
		//비어있으면 처리안함
		if ((*it) == nullptr)
		{
			continue;
		}

		//내자신과는 검사하지않음
		if ((*it) == &BoxCollider)
		{
			continue;
		}

		if (BoxCollider.BoundingBoxCollider.Intersects(  (*it)->BoundingBoxCollider  ) == true)
		{
			//충돌중 
			isStay = true;
		}
		else
		{
			//충돌하지않음
			isStay = false;
		}
	}
}

void Collider::SphereCollision(XMFLOAT3 Pos)
{
	SphereCollider.BoundingSphereCollider.Center = Pos;
	int num = 0;
	SphereCollider.HitType = NoHit;
	///구체와 나의 충돌 여부
	vector<hsSphereCollider*>::iterator it = SphereColliderList.begin();
	for (it; it != SphereColliderList.end(); it++)
	{

		//비어있으면 처리안함
		if ((*it) == nullptr)
		{
			continue;
		}

		//내자신과는 검사하지않음
		if (*it == &SphereCollider)
		{
			continue;
		}

		


		//다중충돌도 고려해야함 일단은 그냥함
		if (SphereCollider.BoundingSphereCollider.Intersects( (*it)->BoundingSphereCollider ) == true)
		{	
			SphereCollider.HitType |= (*it)->ObjType;

			//충돌한객체를 리스트에 담아둠
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
	//처음 충돌했을때 한번
	return isEnter;
}



bool Collider::OnCollisionStay()
{
	//계속 충돌중일떄
	return isStay;
}

bool Collider::OnCollisionExit()
{
	//충돌이 끝났을때
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
		//이전에 충돌하던놈을 찾았다
		if (col == CollisionList[i])
		{

		}
	}



}









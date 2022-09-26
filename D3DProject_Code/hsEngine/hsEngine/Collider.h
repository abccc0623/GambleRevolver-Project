#pragma once


class hsEngine;

#define  NONE	0
#define  BOX	1
#define  SPHERE 2

enum class OBJECT_TYPE;
class Collider
{
public:
	Collider();
	virtual	~Collider();

	//바운딩 박스의 중심을 넣어준다
	void SetBoundingBox(XMFLOAT3 _Pos,XMFLOAT3 _Extents);
	void SetBoundingSphere(XMFLOAT3 _Pos,float _Radius,UINT type);

	//바운딩 박스 업데이트
	void Updata(XMFLOAT3 Pos);

	///박스와 박스 충돌검사를 한다
	//충돌 하면 true 아니면 false  
	void BoxCollision(XMFLOAT3 Pos);
	void SphereCollision(XMFLOAT3 Pos);



	//처음충돌할때 한번
	bool OnCollisionEnter();

	//충돌하고있을때 계속
	bool OnCollisionStay();

	//충돌이 끝났을때 한번
	bool OnCollisionExit();
	

	//강체인지 아닌지
	void SetRigidbody(bool rigid);

	//나의 테그를 설정한다
	//void SetTag(OBJECT_TYPE TYPE);

	//상태편의 테그를 알아낸다
	UINT GetCollision();


	//박스 범위를 나타내주는 바운딩박스
	hsBoxCollider		BoxCollider;
	hsSphereCollider	SphereCollider;





	const UINT	NoHit		= 0x0000000;
	const UINT	Obj			= 0x0000001;
	const UINT	Player		= 0x0000010;
	const UINT	Wall		= 0x0000100;
	const UINT	Enemy		= 0x0001000;
	const UINT	hitObj		= 0x0010000;
	const UINT  Trigger		= 0x0100000;

	
private:
	//나의 타입saas123
	UINT MYType;
	//충돌한 타입
	UINT HIT_Type;

	//박스 충돌 리스트
	static vector<hsBoxCollider*> BoxColliderList;

	//원충돌 리스트
	static vector<hsSphereCollider*> SphereColliderList;


	vector<hsSphereCollider*> CollisionList;

	//강체인가 아닌가
	bool isRigidbody;

	//현재 리스트에 담겨있는 나의 인덱스
	int MyIndex;

	//처음충돌 여부
	bool isEnter;
	//충돌 여부
	bool isStay;
	//마지막 충돌 여부
	bool isExit;
	//이전프레임 충돌 여부
	bool isPastStay;

	//어떤 충돌을 할것인지
	int MyColliderType;

private:
	void PushCollisionList(hsSphereCollider* col);
};



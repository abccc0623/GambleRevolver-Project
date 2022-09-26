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

	//�ٿ�� �ڽ��� �߽��� �־��ش�
	void SetBoundingBox(XMFLOAT3 _Pos,XMFLOAT3 _Extents);
	void SetBoundingSphere(XMFLOAT3 _Pos,float _Radius,UINT type);

	//�ٿ�� �ڽ� ������Ʈ
	void Updata(XMFLOAT3 Pos);

	///�ڽ��� �ڽ� �浹�˻縦 �Ѵ�
	//�浹 �ϸ� true �ƴϸ� false  
	void BoxCollision(XMFLOAT3 Pos);
	void SphereCollision(XMFLOAT3 Pos);



	//ó���浹�Ҷ� �ѹ�
	bool OnCollisionEnter();

	//�浹�ϰ������� ���
	bool OnCollisionStay();

	//�浹�� �������� �ѹ�
	bool OnCollisionExit();
	

	//��ü���� �ƴ���
	void SetRigidbody(bool rigid);

	//���� �ױ׸� �����Ѵ�
	//void SetTag(OBJECT_TYPE TYPE);

	//�������� �ױ׸� �˾Ƴ���
	UINT GetCollision();


	//�ڽ� ������ ��Ÿ���ִ� �ٿ���ڽ�
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
	//���� Ÿ��saas123
	UINT MYType;
	//�浹�� Ÿ��
	UINT HIT_Type;

	//�ڽ� �浹 ����Ʈ
	static vector<hsBoxCollider*> BoxColliderList;

	//���浹 ����Ʈ
	static vector<hsSphereCollider*> SphereColliderList;


	vector<hsSphereCollider*> CollisionList;

	//��ü�ΰ� �ƴѰ�
	bool isRigidbody;

	//���� ����Ʈ�� ����ִ� ���� �ε���
	int MyIndex;

	//ó���浹 ����
	bool isEnter;
	//�浹 ����
	bool isStay;
	//������ �浹 ����
	bool isExit;
	//���������� �浹 ����
	bool isPastStay;

	//� �浹�� �Ұ�����
	int MyColliderType;

private:
	void PushCollisionList(hsSphereCollider* col);
};



#pragma once

class HsMesh;
class Transform 
{
public:
	Transform();
	~Transform();
public:
	///������Ʈ ��ġ
	XMFLOAT3 Position;
	///������Ʈ ũ�� 
	XMFLOAT3 Scale;
	///������Ʈ ȸ��
	XMFLOAT3 Rotation;

	


	///������Ʈ�� ���� ���͵�
	XMFLOAT3 UP_Vector;
	XMFLOAT3 Look_Vector;
	XMFLOAT3 Right_Vector;

	/// ���� �����ǿ� X Y Z ��ŭ �����ش�
	void SetTranlate(float X,float Y,float Z);
	/// ���� ��ġ�� �������ش�
	void SetPosition(float X, float Y, float Z);
	void SetPosition(XMFLOAT3 pos);


	void SetLocalPosition(float X, float Y, float Z);
	/// ���� ��ġ�� �����´�
	XMFLOAT3 GetPosition();

	/// ���� �����̼� ���� AngleX ,AngleY, AngleZ ��ŭ �����ش�
	void SetRotation(float _AngleX, float _AngleY, float _AngleZ);

	/// ���� ������ ���� X,Y,Z ��ŭ �����ش�
	void SetScale(float X, float Y, float Z);
	

	
	///ũ��,ȸ��,��ġ�� �����ģ ��Ʈ����
	XMMATRIX GetMaster();
	XMMATRIX GetLocalMaster();

	///���� ��ǥ��
	XMFLOAT3 LocalPosition;
	XMFLOAT3 LocalScale;
	XMFLOAT3 LocalRotation;
	
	/// X�� �������� �� ȸ�� ��� ����
	XMMATRIX CreateRotX(float _Angle);
	static XMMATRIX Create_RotX(float _Angle);
	/// Y�� �������� �� ȸ�� ��� ����
	XMMATRIX CreateRotY(float _Angle);
	static XMMATRIX Create_RotY(float _Angle);
	/// X�� �������� �� ȸ�� ��� ����
	XMMATRIX CreateRotZ(float _Angle);
	static XMMATRIX Create_RotZ(float _Angle);


	XMMATRIX TestRot = XMMatrixIdentity();

	///���࿡ ���Ͽ� ����� �����
	/// ��ġ ����� ����
	XMMATRIX SetPosition4X4(XMFLOAT3 _Pos);

	/// ȸ�� ����� ����
	XMMATRIX SetRotation4X4(XMFLOAT3 _Rot);

	/// ȸ�� ����� ���� 
	static XMMATRIX Set_Rotation4X4(XMFLOAT3 _Rot);

	/// ũ�� ����� ����
	XMMATRIX SetScale4X4(XMFLOAT3 _Scale);

	/// ���Ϳ� ��Ŀ���
	XMFLOAT3 GetMultiplication(XMFLOAT3 v, XMMATRIX  xm);

	/// ���ʹϾ��� ���Ϸ������� ����
	XMFLOAT3 QuaternionToEuler(const SimpleMath::Vector4 Q);

	/// ������������ �������ִ� TM�� ��� pos, rot, scl�� �־��ش�
	void QuaternionToEuler(XMMATRIX Tr);

	/// ���� ���Ϸ� �����̼ǰ��� ���ʹϾ����� ����
	XMMATRIX EulerToQuaternion(XMFLOAT3& euler);

	SimpleMath::Quaternion MyEulerToQuaternion();

	/// ��ֶ�����
	float GetNormalize();

	/// ������ �븻������
	float NormalizeAngle(float angle);

	///���ʹϾ�� ���Ϸ��� ��ȯ�Ҷ� �ʿ��Ѱ͵�
	XMFLOAT3 NormalizeAngles(XMFLOAT3 angles);
	

	/// �� ��ü�� ��ӱ����� �Ǿ��ִ���
	bool isHierarchy;

	/// ���� �θ� TR
	Transform* ParentTR;

	/// �ڽİ�ü ����Ʈ
	std::vector<Transform*> ChildTR;

	
	XMMATRIX V_TM;
	XMMATRIX W_TM;

	//�θ��� TM
	XMMATRIX ParentTM;
	//������ TM
	XMMATRIX LocalTM;
	//���� TM
	XMMATRIX WorldTM;
private:


	/// ������ĵ��� ���� ������ ���
	XMMATRIX Master;
};
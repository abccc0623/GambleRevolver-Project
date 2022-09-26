#pragma once

class HsMesh;
class Transform 
{
public:
	Transform();
	~Transform();
public:
	///오브젝트 위치
	XMFLOAT3 Position;
	///오브젝트 크기 
	XMFLOAT3 Scale;
	///오브젝트 회전
	XMFLOAT3 Rotation;

	


	///오브젝트에 로컬 벡터들
	XMFLOAT3 UP_Vector;
	XMFLOAT3 Look_Vector;
	XMFLOAT3 Right_Vector;

	/// 원래 포지션에 X Y Z 만큼 더해준다
	void SetTranlate(float X,float Y,float Z);
	/// 현재 위치를 변경해준다
	void SetPosition(float X, float Y, float Z);
	void SetPosition(XMFLOAT3 pos);


	void SetLocalPosition(float X, float Y, float Z);
	/// 현재 위치를 가져온다
	XMFLOAT3 GetPosition();

	/// 원래 로테이션 값에 AngleX ,AngleY, AngleZ 만큼 더해준다
	void SetRotation(float _AngleX, float _AngleY, float _AngleZ);

	/// 원래 스케일 값에 X,Y,Z 만큼 더해준다
	void SetScale(float X, float Y, float Z);
	

	
	///크기,회전,위치를 모두합친 매트릭스
	XMMATRIX GetMaster();
	XMMATRIX GetLocalMaster();

	///로컬 좌표계
	XMFLOAT3 LocalPosition;
	XMFLOAT3 LocalScale;
	XMFLOAT3 LocalRotation;
	
	/// X축 기준으로 만 회전 행렬 만듬
	XMMATRIX CreateRotX(float _Angle);
	static XMMATRIX Create_RotX(float _Angle);
	/// Y축 기준으로 만 회전 행렬 만듬
	XMMATRIX CreateRotY(float _Angle);
	static XMMATRIX Create_RotY(float _Angle);
	/// X축 기준으로 만 회전 행렬 만듬
	XMMATRIX CreateRotZ(float _Angle);
	static XMMATRIX Create_RotZ(float _Angle);


	XMMATRIX TestRot = XMMatrixIdentity();

	///각축에 대하여 행렬을 만든다
	/// 위치 행렬을 만듬
	XMMATRIX SetPosition4X4(XMFLOAT3 _Pos);

	/// 회전 행렬을 만듬
	XMMATRIX SetRotation4X4(XMFLOAT3 _Rot);

	/// 회전 행렬을 만듬 
	static XMMATRIX Set_Rotation4X4(XMFLOAT3 _Rot);

	/// 크기 행렬을 만듬
	XMMATRIX SetScale4X4(XMFLOAT3 _Scale);

	/// 벡터와 행렬연산
	XMFLOAT3 GetMultiplication(XMFLOAT3 v, XMMATRIX  xm);

	/// 쿼터니언을 오일러값으로 변경
	XMFLOAT3 QuaternionToEuler(const SimpleMath::Vector4 Q);

	/// 내꺼전용으로 가지고있는 TM을 찢어서 pos, rot, scl로 넣어준다
	void QuaternionToEuler(XMMATRIX Tr);

	/// 나의 오일러 로테이션값을 쿼터니언으로 변경
	XMMATRIX EulerToQuaternion(XMFLOAT3& euler);

	SimpleMath::Quaternion MyEulerToQuaternion();

	/// 노멀라이즈
	float GetNormalize();

	/// 각도를 노말라이즈
	float NormalizeAngle(float angle);

	///쿼터니언과 오일러로 변환할때 필요한것들
	XMFLOAT3 NormalizeAngles(XMFLOAT3 angles);
	

	/// 이 객체가 상속구조로 되어있는지
	bool isHierarchy;

	/// 나의 부모 TR
	Transform* ParentTR;

	/// 자식객체 리스트
	std::vector<Transform*> ChildTR;

	
	XMMATRIX V_TM;
	XMMATRIX W_TM;

	//부모의 TM
	XMMATRIX ParentTM;
	//로컬의 TM
	XMMATRIX LocalTM;
	//나의 TM
	XMMATRIX WorldTM;
private:


	/// 각종행렬들을 곱한 마스터 행렬
	XMMATRIX Master;
};
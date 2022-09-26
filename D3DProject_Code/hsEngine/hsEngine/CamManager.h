#pragma once
/// <summary>
/// 카메라를 총괄할 카메라 매니저 
/// </summary>
class CamManager 
{
public:
	CamManager();
	~CamManager();

	//카메라를 넣어준다
	void inputCam(string name, hsCamera* cam);

	//카메라를 가져온다
	hsCamera* GetCam(string name);
	//MainCamera라고 지정된 카메라를 가져온다

	//메인카메라의 Right벡터를 가져온다
	static XMFLOAT3 GetMainCamRightVector();
	//메인카메라의 Up벡터를 가져온다
	//메인카메라의 Look벡터를 가져온다



	//CamManager를 만든다
	void init(hsEngine* _ENGINE, Factory* mFactory);

	//현재 생성되어있는 카메라 개수를 가져옴
	int GetCamCount();

	//메인카메라의 키입력을 담당
	void MainCamInputKeyUpdate();

	//키입력을 받은 메인캠을 지정한다
	void ChoiceMainCam(string CamName);

	//카메라를 만든다
	hsCamera* CreateCamera(string CamName,XMFLOAT3 Pos);
	hsCamera* CreateCamera(string CamName,float x= 0, float y =0, float z=0);

	//메인캠을 가져온다 어디서나
	static hsCamera* GetMainCam();
	XMFLOAT3 GetMouseClickWorldPoint(float Range);
	static void SetOnResizeProj(XMMATRIX _Proj);
private:
	//총 카메라의 개수
	int CameraCount;

	//카메라 리스트
	std::map<string,hsCamera*> CamList;
	

	//키입력을 받은 메인캠
	static hsCamera* MainCam;
	//디버깅을 보여줄 카메라...
	static hsCamera* DebugCam;

	//엔진
	hsEngine* ENGINE;
	//펙토리
	Factory* FACTORY;

	float mLastMousePosX = 0;
	float mLastMousePosY = 0;

	static XMMATRIX mProj;
};
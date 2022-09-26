#pragma once
/// <summary>
/// ī�޶� �Ѱ��� ī�޶� �Ŵ��� 
/// </summary>
class CamManager 
{
public:
	CamManager();
	~CamManager();

	//ī�޶� �־��ش�
	void inputCam(string name, hsCamera* cam);

	//ī�޶� �����´�
	hsCamera* GetCam(string name);
	//MainCamera��� ������ ī�޶� �����´�

	//����ī�޶��� Right���͸� �����´�
	static XMFLOAT3 GetMainCamRightVector();
	//����ī�޶��� Up���͸� �����´�
	//����ī�޶��� Look���͸� �����´�



	//CamManager�� �����
	void init(hsEngine* _ENGINE, Factory* mFactory);

	//���� �����Ǿ��ִ� ī�޶� ������ ������
	int GetCamCount();

	//����ī�޶��� Ű�Է��� ���
	void MainCamInputKeyUpdate();

	//Ű�Է��� ���� ����ķ�� �����Ѵ�
	void ChoiceMainCam(string CamName);

	//ī�޶� �����
	hsCamera* CreateCamera(string CamName,XMFLOAT3 Pos);
	hsCamera* CreateCamera(string CamName,float x= 0, float y =0, float z=0);

	//����ķ�� �����´� ��𼭳�
	static hsCamera* GetMainCam();
	XMFLOAT3 GetMouseClickWorldPoint(float Range);
	static void SetOnResizeProj(XMMATRIX _Proj);
private:
	//�� ī�޶��� ����
	int CameraCount;

	//ī�޶� ����Ʈ
	std::map<string,hsCamera*> CamList;
	

	//Ű�Է��� ���� ����ķ
	static hsCamera* MainCam;
	//������� ������ ī�޶�...
	static hsCamera* DebugCam;

	//����
	hsEngine* ENGINE;
	//���丮
	Factory* FACTORY;

	float mLastMousePosX = 0;
	float mLastMousePosY = 0;

	static XMMATRIX mProj;
};
#pragma once
/// <summary>
/// ��ũ���� ������ ��ũ�� �Ŵ���
/// Screen Ŭ������ ��ӹ��� Ŭ�������� �޾ƿ� ���� ������
/// </summary>
class CamManager;
class ScreenManager
{
public:
	ScreenManager();
	~ScreenManager();
	void init(hsEngine* mEngine, Factory* factory);

	//�����ϸ� �ȵǴ� ������Ʈ��
	void PushDontDestroy(GameObject* obj);
	//�����Ǿ ������������Ʈ��
	void PushObject(GameObject* obj);
	
	//��ũ���� �ִ´�
	void SetScreen(string Name,Screen* mScreen);

	//��ũ���� �̵��Ѵ�
	static void MoveScreen(string Name);

	void MoveScreen();
	//ó�������� ��ũ���� ����
	void ChoiceScreen(Screen* mScreen);
	//������� ������Ʈ���� ������Ʈ
	void Update();
	//������� ������Ʈ���� ������
	void Render(const hsViewData &data);
	//������� ������Ʈ�� �����Ұ��� üũ
	void DeleteCheck();
	void SetCamManager(CamManager* cam);
	void GetObjList(vector<GameObject*>* obj, vector<GameObject*>* DDL);
public:
	///�������� �Լ���
	//�˹��� ��������
	Canvas* GetCanvas();
	//����Ʈ �Ŵ����� ��������
	LightManager* GetLightManager();
	//���ĸŴ����� ��������
	AlphaManager* GetAlphaManager();
private:
	//�����ϸ� �ȵǴ� ������Ʈ��
	vector<GameObject*> DontDestroyList;

	//��ũ�� ����Ʈ
	static map<string, Screen*> ScreenList;

	//���� ��ũ���� ���ӿ�����Ʈ
	vector<GameObject*> NowObjList;

	//�Ѱ��� ��ũ���� ���۵ɶ� init�� ���ֱ�����
	bool isStart;

	//���� ���� ��ũ��
	Screen* NowScreen;

	//������ũ������ �̵��Ұ����� ����
	static bool NextScreen;

	//�̵��� ��ũ�� �̸�
	static string NextScreenName;
private:
	///���� �Ŵ�����

	//����
	hsEngine*	mENGINE;

	//������Ʈ ������ ���� ���丮
	Factory*	mFactory;

	//����Ʈ�� ������ ����Ʈ�Ŵ���
	LightManager* mLight;

	//Ui�� ���� �Ŵ���
	Canvas* canvas;

	//���ĸŴ���
	AlphaManager* mAlpha;

	CamManager* mCam;
};
#pragma once
/// <summary>
/// �Ѱ��� ��ũ�� �̰� ��ӹ޾Ƽ� �Ѱ��� ��������� ���Ŵ����� �����ش�
/// </summary>
/// 
class Canvas;
class AlphaManager;
class CamManager;
class Screen
{
public:
	Screen();
	~Screen();

	//������
	virtual void Create
	(
		hsEngine* Engine,
		Factory* factory,
		LightManager* _Light,
		Canvas* _canvas,
		AlphaManager* _alpha,
		CamManager* Cam,
		vector<GameObject*>* objlist,
		vector<GameObject*>* DDLlist
	);


	virtual void init()	= 0;
	virtual void Updata();
	virtual void Render(const hsViewData& data);
	virtual void Delete();
	virtual void End();

	//�� Ŭ����
	void Clear();

	//������Ʈ ����Ʈ�� �־��ش�
	void PushObjList(GameObject* temp);
	void PushDontDestroyList(GameObject* temp);

	//�⺻���� ����Ʈ ����
	LightObject* CreateLight(float x,float y,float z);
	LightObject* CreateDirLight();
	void CreateGrid(XMFLOAT2 size, XMFLOAT3 pos = { 0,0,0 }, XMFLOAT4 color = { 1,1,1,1 });


	static GameObject* MainPlayer;
	static hsCamera* MainCam;
	static hsCamera* ShadowCam;
	static hsCamera* SubCam;
protected:
	//����
	hsEngine* mENGINE;
	//������Ʈ ������ ���� ���丮
	Factory* mFactory;
	//����Ʈ ������ ���� �Ŵ���
	LightManager* mLight;
	//UI�� ���� �˹���
	Canvas* mCanvas;
	//���ķ����������� �Ŵ���
	AlphaManager* mAlpha;
	//ķ �Ŵ���
	CamManager* mCam;

	//���ǵ��
private:
	//��ũ�� �Ŵ������� �޾ƿ� �����ϸ� �ȵǴ� ����Ʈ��
	vector<GameObject*>* mDontDestroyList;

	//��ũ���Ŵ������� �޾ƿ� ����Ʈ��
	vector<GameObject*>* mObjList;
};

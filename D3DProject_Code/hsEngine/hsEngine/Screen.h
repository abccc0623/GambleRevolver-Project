#pragma once
/// <summary>
/// 한개의 스크린 이걸 상속받아서 한개의 씬을만들고 씬매니저로 보내준다
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

	//씬생성
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

	//씬 클리어
	void Clear();

	//오브젝트 리스트에 넣어준다
	void PushObjList(GameObject* temp);
	void PushDontDestroyList(GameObject* temp);

	//기본적인 라이트 생성
	LightObject* CreateLight(float x,float y,float z);
	LightObject* CreateDirLight();
	void CreateGrid(XMFLOAT2 size, XMFLOAT3 pos = { 0,0,0 }, XMFLOAT4 color = { 1,1,1,1 });


	static GameObject* MainPlayer;
	static hsCamera* MainCam;
	static hsCamera* ShadowCam;
	static hsCamera* SubCam;
protected:
	//엔진
	hsEngine* mENGINE;
	//오브젝트 생성을 위한 펙토리
	Factory* mFactory;
	//라이트 생성을 위한 매니저
	LightManager* mLight;
	//UI를 위한 켄버스
	Canvas* mCanvas;
	//알파랜더링을위한 매니저
	AlphaManager* mAlpha;
	//캠 매니저
	CamManager* mCam;

	//못건들게
private:
	//스크린 매니저에서 받아온 삭제하면 안되는 리스트들
	vector<GameObject*>* mDontDestroyList;

	//스크린매니저에서 받아온 리스트들
	vector<GameObject*>* mObjList;
};

#pragma once
/// <summary>
/// 스크린을 관리할 스크린 매니저
/// Screen 클래스를 상속받은 클래스들을 받아와 씬을 관리함
/// </summary>
class CamManager;
class ScreenManager
{
public:
	ScreenManager();
	~ScreenManager();
	void init(hsEngine* mEngine, Factory* factory);

	//삭제하면 안되는 오브젝트들
	void PushDontDestroy(GameObject* obj);
	//삭제되어도 괜찮은오브젝트들
	void PushObject(GameObject* obj);
	
	//스크린을 넣는다
	void SetScreen(string Name,Screen* mScreen);

	//스크린을 이동한다
	static void MoveScreen(string Name);

	void MoveScreen();
	//처음시작할 스크린을 선택
	void ChoiceScreen(Screen* mScreen);
	//현재씬의 오브젝트들을 업데이트
	void Update();
	//현재씬의 오브젝트들을 랜더링
	void Render(const hsViewData &data);
	//현재씬의 오브젝트중 삭제할것을 체크
	void DeleteCheck();
	void SetCamManager(CamManager* cam);
	void GetObjList(vector<GameObject*>* obj, vector<GameObject*>* DDL);
public:
	///내보내는 함수들
	//켄버스 내보내줌
	Canvas* GetCanvas();
	//라이트 매니저를 내보내줌
	LightManager* GetLightManager();
	//알파매니저를 내보내줌
	AlphaManager* GetAlphaManager();
private:
	//삭제하면 안되는 오브젝트들
	vector<GameObject*> DontDestroyList;

	//스크린 리스트
	static map<string, Screen*> ScreenList;

	//현재 스크린에 게임오브젝트
	vector<GameObject*> NowObjList;

	//한개의 스크린이 시작될때 init을 해주기위해
	bool isStart;

	//지금 현재 스크린
	Screen* NowScreen;

	//다음스크린으로 이동할것인지 여부
	static bool NextScreen;

	//이동할 스크린 이름
	static string NextScreenName;
private:
	///각종 매니저들

	//엔진
	hsEngine*	mENGINE;

	//오브젝트 생성을 위한 펙토리
	Factory*	mFactory;

	//라이트를 관리할 라이트매니저
	LightManager* mLight;

	//Ui를 위한 매니저
	Canvas* canvas;

	//알파매니저
	AlphaManager* mAlpha;

	CamManager* mCam;
};
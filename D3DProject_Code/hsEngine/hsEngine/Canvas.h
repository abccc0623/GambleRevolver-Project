#pragma once
/// <summary>
/// UI들을 관리할매니저 
/// </summary>

class Canvas 
{

public:
	void init(hsEngine* _Engine, Factory *_Factory);



	void Update();
	void Render();
	void Debug();
	void Delete();

	//먼저그릴놈Layout설정과 UI를 넣어준다
	void PushUI(int layout, UI* ui);
	//사용할 레이아웃 사이즈를 지정해줌
	void CreateUILayout(int LayoutSize);
	
	//UI들을 모두지운다
	void ClearUI();


	//UI의 필요한 화면상좌표들을 구한다
	//이거는 윈도우창의 크기가변할떄마다 계산해줘야하기때문에 스테틱으로 엔진의 OnResize가 일어날때 호출될것이다
	static void CreateScreenNumber(int Width,int Height);

	///화면비율에 관한것들
	//가로픽셀크기
	static float OnePixelX;
	static float OnePixelY;

	//화면의 오른쪽위 좌표
	static float MaxX;
	static float MaxY;

	//화면의 왼쪽아래 좌표
	static float MinX;
	static float MinY;

	//이미지의 1:1 비율값
	static float OneSizeX;
	static float OneSizeY;

public:
	///UI 생성 함수들
	Image* Create_Image(string TextureName, int Layout, float Alpha, XMFLOAT3 Pos, XMFLOAT3 Scl = {1,1,1},UI_PIVOT pivot= UI_PIVOT::MID);
	Image* Create_Button(string TextureName, int Layout, float Alpha, XMFLOAT3 Pos, XMFLOAT3 Scl = { 1,1,1 }, UI_PIVOT pivot = UI_PIVOT::MID);
	Slider* Create_Slider(string Back,string Fill,string Front,string hand,XMFLOAT3 Pos,XMFLOAT3 Scl,UI_PIVOT pivot);
private:
	hsEngine* mENGINE;
	Factory* mFactory;


	vector<vector<UI*>> UILauoutList;
};
#pragma once
struct Vertex;
class hsEngine
{
public:
	hsEngine();
	~hsEngine();

	///엔진 초기화
	bool init(HWND _hwnd,int sizeX, int sizeY);

	///엔진 업데이트
	void Update();
	void Debug();

	///엔진 렌더링
	void BeginRender();
	void EndRender();
	void Render();

	///외부에서 쓸 데이터 내보내기
	DeviceData* GetDeviceData();

	///화면 크기에따라 변환
	void OnResize(int sizeX, int sizeY);

	hsKey* GetKey();

	//시간 가져오기
	float GetDeltaTime();
	int GetFPS();

	//폰트 객체
	DXTKFont* GetFont();
	

	bool OnDebug;

	EngineBaseViewData GetViewData();
	float mClientWidth;
	float mClientHeight;
private:
	float FPS;

	/// 각종 랜더 모드를 미리만듬  
	void CreateRenderStates();
	///랜더 타겟뷰 생성
	void CreateRenderTargetView();

	void GameTime();

	//Debug용 변수들
	bool WireframeMode;

	bool mEnable4xMsaa;

	//요건 뭔지 모르것넹..
	UINT m4xMsaaQuality;

	HWND hwnd;
	ID3D11Device*				m_device;			//디바이스
	ID3D11DeviceContext*		m_deviceContext;	//디바이스 컨텍스트
	ID3D11RenderTargetView*		m_renderTargetView;	//랜더 타겟
	ID3D11DepthStencilView*		mDepthStencilView;	//뎁스 스텐실뷰
	D3D11_VIEWPORT				mScreenViewport;	//뷰포트

	BOOL OnPostProcessing;
	//스왑체인
	IDXGISwapChain* mSwapChain;     
	// 뷰포트
	ID3D11DepthStencilState* NormalDSS;
	
	DeviceData* device;

	// Render State
	ID3D11RasterizerState* mWireframeRS;
	ID3D11RasterizerState* mSolidRS;


	//키관련 클래스
	hsKey* InputKey;

	//타임 관련 클래스
	GameTimer* time;

	//폰트
	DXTKFont* m_pFont;
	
	DXGI_ADAPTER_DESC1 m_AdapterDesc1;
	HRESULT GetAdapterInfo();
	float GetAspectRatio();
};
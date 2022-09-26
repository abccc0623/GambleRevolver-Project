#pragma once
struct Vertex;
class hsEngine
{
public:
	hsEngine();
	~hsEngine();

	///���� �ʱ�ȭ
	bool init(HWND _hwnd,int sizeX, int sizeY);

	///���� ������Ʈ
	void Update();
	void Debug();

	///���� ������
	void BeginRender();
	void EndRender();
	void Render();

	///�ܺο��� �� ������ ��������
	DeviceData* GetDeviceData();

	///ȭ�� ũ�⿡���� ��ȯ
	void OnResize(int sizeX, int sizeY);

	hsKey* GetKey();

	//�ð� ��������
	float GetDeltaTime();
	int GetFPS();

	//��Ʈ ��ü
	DXTKFont* GetFont();
	

	bool OnDebug;

	EngineBaseViewData GetViewData();
	float mClientWidth;
	float mClientHeight;
private:
	float FPS;

	/// ���� ���� ��带 �̸�����  
	void CreateRenderStates();
	///���� Ÿ�ٺ� ����
	void CreateRenderTargetView();

	void GameTime();

	//Debug�� ������
	bool WireframeMode;

	bool mEnable4xMsaa;

	//��� ���� �𸣰ͳ�..
	UINT m4xMsaaQuality;

	HWND hwnd;
	ID3D11Device*				m_device;			//����̽�
	ID3D11DeviceContext*		m_deviceContext;	//����̽� ���ؽ�Ʈ
	ID3D11RenderTargetView*		m_renderTargetView;	//���� Ÿ��
	ID3D11DepthStencilView*		mDepthStencilView;	//���� ���ٽǺ�
	D3D11_VIEWPORT				mScreenViewport;	//����Ʈ

	BOOL OnPostProcessing;
	//����ü��
	IDXGISwapChain* mSwapChain;     
	// ����Ʈ
	ID3D11DepthStencilState* NormalDSS;
	
	DeviceData* device;

	// Render State
	ID3D11RasterizerState* mWireframeRS;
	ID3D11RasterizerState* mSolidRS;


	//Ű���� Ŭ����
	hsKey* InputKey;

	//Ÿ�� ���� Ŭ����
	GameTimer* time;

	//��Ʈ
	DXTKFont* m_pFont;
	
	DXGI_ADAPTER_DESC1 m_AdapterDesc1;
	HRESULT GetAdapterInfo();
	float GetAspectRatio();
};
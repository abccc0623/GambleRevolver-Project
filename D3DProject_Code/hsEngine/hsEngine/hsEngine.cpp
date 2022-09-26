// hsEngine.cpp : 정적 라이브러리를 위한 함수를 정의합니다.
#include "EnginePCH.h"


hsEngine::hsEngine()
{
	hwnd = nullptr;

	mSwapChain = nullptr;
	m_device = nullptr;
	m_deviceContext = nullptr;
	
	//m_renderTargetView = nullptr;

	mSwapChain			= nullptr;
	mDepthStencilView	= nullptr;

	mClientWidth	= 1280;
	mClientHeight	= 720;
	

	mWireframeRS = nullptr;
	mSolidRS = nullptr;


	//키인풋
	InputKey = nullptr;

	mEnable4xMsaa = false;

	WireframeMode = false;
}

hsEngine::~hsEngine()
{
	
	if (m_renderTargetView != nullptr)
	{
		m_renderTargetView->Release();
	}

	if (mDepthStencilView != nullptr)
	{
		mDepthStencilView->Release();
	}

	if (m_deviceContext != nullptr)
	{
		m_deviceContext->Release();
	}

	if (m_device != nullptr)
	{
		m_device->Release();
	}


	NormalDSS->Release();
	mWireframeRS->Release();
	mSolidRS->Release();

	//m_renderTargetView->Release();
	//mDepthStencilView->Release();
	mSwapChain->Release();
}

bool hsEngine::init(HWND _hwnd,int sizeX,int sizeY)
{
	UINT createDeviceFlags = 0;
	D3D_FEATURE_LEVEL featureLevel;

	mClientWidth = sizeX;
	mClientHeight = sizeY;

	/*
	* 용책 참고
	* D3D 장치와 장치문맥을 생성하는 함수
	*/

	hwnd = _hwnd;
	HRESULT hr = D3D11CreateDevice
	(
		0,							//디스플레이 어뎁터  0 = 디폴트 
		D3D_DRIVER_TYPE_HARDWARE,	// ???
		0,							// 소프트웨어 구동기 지정
		createDeviceFlags,			// ???
		0,							// 점검순서
		0,							// ???
		D3D11_SDK_VERSION,			//항상 D3D11_SDK_VERSION 을 지정
		&m_device,					//함수가 생성한장치 돌려줌
		&featureLevel,				//null 값으로 한경우 지원하는 가장 높은 기능수준으로
		&m_deviceContext			//원래값 돌려줌
	);

	//생성 여부
	if (FAILED(hr))
	{
		return false;
	}

	//버전확인
	if (featureLevel != D3D_FEATURE_LEVEL_11_0)
	{
		MessageBox(0, L"Direct3D LEVEL 11", 0, MB_OK);
		return 0;
	}


	//4xMSAA 품질 수준 지원 점검??
	//하드웨어가 4xMSAA를 위한 품질 수준을 지원하는지 점검

	UINT m4xMsaaQuality;
	m_device->CheckMultisampleQualityLevels
	(
		DXGI_FORMAT_R8G8_B8G8_UNORM,
		4,
		&m4xMsaaQuality
	);
	
	//assert를 몰라서 써놓음
	//assert는 조건이 맞지않을때 프로그램을 중단하며 참일때는 계속 실행시킴
	//assert(m4xMsaaQuality > 0);

	//교환 사슬 설정?
	//구조체 값 설정
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	//BufferDesc = 생성하고자 하는 후면 버퍼의 속성들을 서술하는 개별적인 구조체
	
	sd.BufferCount = 1;  //교환 사슬에서 사용할 후면 버퍼의 개수
	sd.BufferDesc.Width = mClientWidth;
	sd.BufferDesc.Height = mClientHeight;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; 
	sd.BufferDesc.RefreshRate.Numerator = 60;	
	sd.BufferDesc.RefreshRate.Denominator = 1;   
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; //버퍼의 용도
	sd.OutputWindow = _hwnd; //랜더링 결과를 표시할 창의 핸들

	//sampleDesc = 다중 표본화를 이해 추출할 표본 개수와 품질수준
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;	//
	sd.Windowed = true;			//윈도우 창모드를 원하면 true 전체모드 false
	
	//교환 사슬의 생성
	
	//이과정에서 오류를 피하려면 장치의 생성에 쓰인 IDXGIFactory 인스턴스를 사용해야한다
	//그리고 그 인스턴스를 얻으려면 다음과 같이 일련의 COM 질의 과정을 거쳐야함..
	IDXGIDevice* dxgiDevice = 0;
	m_device->QueryInterface
	(
		__uuidof(IDXGIDevice),
		(void**)&dxgiDevice
	);


	IDXGIAdapter* dxgiAdapter = 0;
	dxgiDevice->GetParent
	(
		__uuidof(IDXGIAdapter),
		(void**)&dxgiAdapter
	);

	//IDXGIFactory 인터페이스 얻음
	IDXGIFactory* dxgiFactory = 0;
	dxgiAdapter->GetParent
	(
		__uuidof(IDXGIFactory),
		(void**)&dxgiFactory
	);

	//사슬 교환 생성
	dxgiFactory->CreateSwapChain(m_device, &sd, &mSwapChain);

	//필요없는건 삭젱
	dxgiAdapter->Release();
	dxgiDevice->Release();
	dxgiFactory->Release();

	CreateRenderTargetView();
	
	CreateRenderStates();

	m_pFont = new DXTKFont();
	m_pFont->Create(m_device, mSolidRS, NormalDSS);

	//키관련 클래스 생성
	InputKey = new hsKey();

	
	device = new DeviceData;
	time = new GameTimer();	
	

	//어뎁터 정보 얻어옴
	GetAdapterInfo();
	time->Start();
	time->Reset();
}

void hsEngine::Update()
{
	GameTime();
	InputKey->KeyUpDate();
	InputKey->UpdataMouseCursor(hwnd);
	OnPostProcessing = true;
	//if (InputKey->GetKeyToggle(VK_F2))
	//{
	//	
	//	OnPostProcessing = true;
	//}
	//else
	//{
	//	OnPostProcessing = false;
	//}
	
}


void hsEngine::Render()
{
	/// 카메라 만드는 중

	//m_deviceContext->OMSetDepthStencilState(NormalDSS, 0);
	//m_deviceContext->RSSetState(0);
}



DeviceData* hsEngine::GetDeviceData()
{
	device->m_device = m_device;
	device->m_deviceContext = m_deviceContext;
	device->mSolidRS		= mSolidRS;
	device->mWireframeRS	= mWireframeRS;
	return device;
}

void hsEngine::OnResize(int sizeX,int sizeY)
{
	mClientWidth = sizeX;
	mClientHeight = sizeY;
	assert(m_deviceContext);
	assert(m_device);
	assert(mSwapChain);

	if (m_renderTargetView != nullptr)
	{
		//랜더 타겟 삭제
		m_renderTargetView->Release();
	}

	if (mDepthStencilView != nullptr)
	{
		//덱스스텐실 삭제
		mDepthStencilView->Release();
	}
		
	//스왑체인 재생성
	mSwapChain->ResizeBuffers(1, mClientWidth, mClientHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

	if (RenderManager::PostProcessing == true)
	{
		RenderManager::GetRM()->PostTextureMapReset(mClientWidth, mClientHeight);
	}

	CreateRenderTargetView();

	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, mDepthStencilView);
	m_deviceContext->RSSetViewports(1, &mScreenViewport);


	// The window resized, so update the aspect ratio and recompute the projection matrix.
	/// 창의 크기가 변했으므로, 종횡비를 업데이트하고 투영 행렬을 재계산한다.
	XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f * MathHelper::Pi, GetAspectRatio(), 1.0f, 3000.0f);
	CamManager::SetOnResizeProj(P);

	//화면이 변했으니 ui에도 창변화가일어날것이다
	Canvas::CreateScreenNumber(sizeX, sizeY);
}

EngineBaseViewData hsEngine::GetViewData()
{
	//엔진의 기본이되는 뷰데이터를 현재는 랜더매니저로 넘겨준다
	//랜더매니저 말고는 쓸때가없을것같음 
	//OnResize 함수가 실행되면 랜더타겟과 스텐실뷰 를 새로 만들기때문에 오류가날수있다..

	EngineBaseViewData Data;
	Data.DSV = mDepthStencilView;
	Data.RTV = m_renderTargetView;
	Data.VP =	mScreenViewport;

	return 	Data;
}

void hsEngine::CreateRenderStates()
{
	D3D11_RASTERIZER_DESC solidDesc;
	ZeroMemory(&solidDesc, sizeof(D3D11_RASTERIZER_DESC));
	solidDesc.FillMode = D3D11_FILL_SOLID;
	solidDesc.CullMode = D3D11_CULL_BACK;
	solidDesc.FrontCounterClockwise = false;
	solidDesc.DepthClipEnable = true;

	HR(m_device->CreateRasterizerState(&solidDesc, &mSolidRS));


	D3D11_RASTERIZER_DESC wireframeDesc;
	ZeroMemory(&wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
	wireframeDesc.FillMode = D3D11_FILL_WIREFRAME;
	wireframeDesc.CullMode = D3D11_CULL_BACK;
	wireframeDesc.FrontCounterClockwise = false;
	wireframeDesc.DepthClipEnable = true;

	HR(m_device->CreateRasterizerState(&wireframeDesc, &mWireframeRS));


	// 폰트용 DSS
	D3D11_DEPTH_STENCIL_DESC equalsDesc;
	ZeroMemory(&equalsDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	equalsDesc.DepthEnable = false;
	equalsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;		// 깊이버퍼에 쓰기는 한다
	equalsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	
	HR(m_device->CreateDepthStencilState(&equalsDesc, &NormalDSS));
}

void hsEngine::CreateRenderTargetView()
{
	//if (mDepthStencilView != nullptr)
	//{
	//	mDepthStencilView->Release();
	//	mDepthStencilView = nullptr;
	//}
	//
	//if (m_renderTargetView != nullptr)
	//{
	//	m_renderTargetView->Release();
	//	m_renderTargetView = nullptr;
	//}


	{
		ID3D11Texture2D* backBuffer;
		// 0 : 후면 버퍼의 색인
		// 1 : 후면 버퍼의 인터페이스 형식
		// 2 : 후면 버퍼의 포인터 반환
		mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
		

		//랜더 타겟뷰를 생성하는 메소드
		// 0 : 랜더 대상으로 사용할 자원
		// 1 : D3D1_RENDER_TARGET_VIEW_DESC 구조체를 가르키는 포인터 (null)로지정해도됨
		// 2 : 생성된 랜더 타겟뷰를 돌려줌
		HR(m_device->CreateRenderTargetView(backBuffer, 0, &m_renderTargetView));

		backBuffer->Release();
	}
	

	//깊이 버퍼는 그냥 깊이 정보를 담는 2차원 텍스쳐이다  2차원 텍스쳐를 생성할때는 D3D11_TEXTURE2D_DESC 구조체를 채우고 
	//CreateTexture2D 메소드를 실행해야한다

	ID3D11Texture2D* mDepthStencilBuffer = nullptr;
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = mClientWidth; //텍스쳐의 너비 (텍셀단위) 텍셀??
	depthStencilDesc.Height = mClientHeight;//텍스쳐의 높이 (텍셀단위) 텍셀??
	depthStencilDesc.MipLevels = 1; //밉맵수준의 개수
	depthStencilDesc.ArraySize = 1; //택스처 배열의 텍스처 개수*깊이*스텐실 버퍼의 경우 텍스처 하나만필요
	//텍셀의 형식을 뜻하는 필드로 DXGI_FORMAT 열거형의 값들 중 하나를 지정한다
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;					//SampleDesc = 다중표본 개수와 품질수듄을 서술하는 구조체
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;			//텍스처의 용도를 뜻하는 필드
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;	//자원을 파이프라인에 어떤식으로 묶을것인지
	depthStencilDesc.CPUAccessFlags = 0;					//CPU가 자원을 접근하는 방식을 결정하는 플래그를 지정
	depthStencilDesc.MiscFlags = 0;
	m_device->CreateTexture2D(&depthStencilDesc, 0, &mDepthStencilBuffer);

	HR(m_device->CreateDepthStencilView(mDepthStencilBuffer, 0, &mDepthStencilView));
	
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, mDepthStencilView);


	//ViewPort
	mScreenViewport.TopLeftX = 0;
	mScreenViewport.TopLeftY = 0;
	mScreenViewport.Width = static_cast<float>(mClientWidth);
	mScreenViewport.Height = static_cast<float>(mClientHeight);
	mScreenViewport.MinDepth = 0.0f;
	mScreenViewport.MaxDepth = 1.0f;

	m_deviceContext->RSSetViewports(1, &mScreenViewport);
	//mDepthStencilBuffer->Release();
}

hsKey* hsEngine::GetKey()
{
	return InputKey;
}

float hsEngine::GetDeltaTime()
{
	return time->DeltaTime();
}

int hsEngine::GetFPS()
{
	return FPS;
}

DXTKFont* hsEngine::GetFont()
{
	return m_pFont;
}

HRESULT hsEngine::GetAdapterInfo()
{
	// DXGI버전별로 다름
	IDXGIAdapter1* pAdapter;
	IDXGIFactory1* pFactory = NULL;

	HRESULT hr = S_OK;

	// DXGIFactory 개체 생성(DXGI.lib 필요)
	if (FAILED(CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&pFactory)))
	{
		return E_FAIL;
	}

	hr = pFactory->EnumAdapters1(0, &pAdapter);

	if (hr != DXGI_ERROR_NOT_FOUND)
	{
		pAdapter->GetDesc1(&m_AdapterDesc1);
	}

	SAFE_RELEASE(pAdapter);
	SAFE_RELEASE(pFactory);

	return S_OK;
}

float hsEngine::GetAspectRatio()
{
	float width = mClientWidth;
	float Height = mClientHeight;
	float Ratio = width / Height;
	return  Ratio;
}

void hsEngine::BeginRender()
{
	if (RenderManager::PostProcessing == true)
	{
		RenderManager::GetRM()->BeginRenderPost();
	}
	else
	{
		XMVECTORF32 DeepDarkGray = { 1.0f, 1.0f, 1.0f, 1.0f };
		m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, mDepthStencilView);
		m_deviceContext->ClearRenderTargetView(m_renderTargetView, DeepDarkGray);
		m_deviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		m_deviceContext->RSSetViewports(1, &mScreenViewport);
	}
}

void hsEngine::EndRender()
{
	assert(mSwapChain);

	if (RenderManager::PostProcessing == true)
	{
		RenderManager::GetRM()->EndRenderPost();
	}
	// 프리젠트 꼭 해줘야 한다.
	HR(mSwapChain->Present(0, 0));
}

void hsEngine::GameTime()
{
	//time->Start();
	// 매 프레임 시간을 계산한다.
	time->Tick();

	// 매 프레임의 deltaTime
	time->DeltaTime();


	if (GetAsyncKeyState(VK_ESCAPE))
	{
		PostQuitMessage(0);
	}

	/// 타이머를 기반으로 FPS, millisecond per frame (1프레임당 elapsedTime = deltaTime)을 계산 해 준다.

	// Code computes the average frames per second, and also the 
	// average time it takes to render one frame.  These stats 
	// are appended to the window caption bar.

	static int frameCnt = 0;
	static float timeElapsed = 0.0f;

	frameCnt++;

	// Compute averages over one second period.
	if ((time->TotalTime() - timeElapsed) >= 1.0f)
	{
		FPS = (float)frameCnt; // fps = frameCnt / 1
		float mspf = 1000.0f / FPS;

		/*
		std::wostringstream outs;
		outs.precision(6);
		outs << mMainWndCaption << L"    "
			<< L"FPS: " << fps << L"    "
			<< L"Frame Time: " << mspf << L" (ms)";
		SetWindowText(mhMainWnd, outs.str().c_str());
		*/

		// Reset for next average.
		frameCnt = 0;
		timeElapsed += 1.0f;
	}


	
}
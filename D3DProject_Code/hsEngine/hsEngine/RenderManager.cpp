#include "EnginePCH.h"
#include "RenderManager.h"

bool RenderManager::PostProcessing = false;
RenderManager* RenderManager::RM = nullptr;
RenderManager::RenderManager()
{
	ENGINE = nullptr;
	Device = nullptr;
	mContext = nullptr;

	//다이나믹 변수들 초기화
	mDynamicCubeDSV = nullptr;
	mDynamicCubeSRV = nullptr;
	for (int i = 0; i < 6; i++)
	{
		mDynamicCubeRTV[i] = nullptr;
	}
	mDynamicCubeVP = D3D11_VIEWPORT();

}

RenderManager* RenderManager::GetRM()
{
	if (RM == nullptr)
	{
		RM = new RenderManager();
	}

	return RM;
}

RenderManager::~RenderManager()
{
	ENGINE = nullptr;
	Device = nullptr;
	mContext = nullptr;

	//포스트 프로세싱 지우기
	mPost->Delete();

	//쉐도우맵 지우기
	mShadow->Delete();

	//디퍼드 지우기
	mDeferred->Delete();

	//Ui지우기
	mUI->Delete();
}

void RenderManager::init(hsEngine* _ENGINE)
{
	ENGINE = _ENGINE;

	Device = ENGINE->GetDeviceData()->m_device;
	mContext = ENGINE->GetDeviceData()->m_deviceContext;

	CreateDepthStencilState();
}

void RenderManager::PostTextureMapReset(float mClientWidth, float mClientHeight)
{
	mPost->Reset(mClientWidth, mClientHeight);
	if (mDeferred != nullptr)
	{
		mDeferred->Reset(mClientWidth, mClientHeight);
	}

	if (mLight != nullptr)
	{
		mLight->Reset(mClientWidth, mClientHeight);
	}

	if (mUI != nullptr)
	{
		mUI->Reset(mClientWidth, mClientHeight);
	}
}

ID3D11ShaderResourceView* RenderManager::TextureRender(hsCamera* Cam, string TextMapName, std::vector<GameObject*>& objList)
{
	TextureMap* temp = TextureList[TextMapName];

	if (temp == nullptr)
	{
		return nullptr;
	}

	ID3D11DepthStencilView* mDSV = temp->GetDSV();
	ID3D11RenderTargetView* mRTV = temp->GetRTV();
	ID3D11ShaderResourceView* mSRV = temp->GetSRV();
	D3D11_VIEWPORT				mVP = temp->GetVP();

	//그릴곳의 랜더타겟과 뎁스 스텐실 뷰포트를 설정해준다
	mContext->OMSetRenderTargets(1, &mRTV, mDSV);
	mContext->RSSetViewports(1, &mVP);

	//이제 그릴것이므로 클리어
	mContext->ClearRenderTargetView(mRTV, reinterpret_cast<const float*>(&Colors::Silver));
	mContext->ClearDepthStencilView(mDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);

	//카메라를 기준으로 모든오브젝트들을 찍는다
	hsViewData data;
	//잠시 메인카메라의 권리를 준다
	Cam->isMain = true;
	//카메라의 위치를 업데이트하기위해
	Cam->Update();
	//카메라의 뷰,프로젝션 메트릭스를 업데이트하기위해
	Cam->Render(data);
	//카메라의 정보를 받아옴
	data.world = Cam->transfrom.GetMaster();
	data.view = Cam->GetView();
	data.proj = Cam->GetProj();
	//랜더링
	for (auto j : objList)
	{
		j->Render(data);
	}
	//메인카메라의 권한을 다시뺏음
	Cam->isMain = false;


	ID3D11RenderTargetView* ENGINE_RTV = mPost->GetRTV();
	ID3D11DepthStencilView* ENGINE_DSV = mPost->GetDSV();
	D3D11_VIEWPORT ENGINE_VP = mPost->GetVP();

	//다시 바인딩
	mContext->OMSetRenderTargets(1, &ENGINE_RTV, ENGINE_DSV);
	mContext->RSSetViewports(1, &ENGINE_VP);


	//그리고 쉐이더 리소스 되돌려줌
	return mSRV;
}
ID3D11ShaderResourceView* RenderManager::DynamicCubeMapRender(hsCamera* Cam[6], std::vector<GameObject*>& objList, XMFLOAT3 Pos)
{
	//오른쪽
	Cam[0]->transfrom.Rotation = { 0, 90, 0 };
	//왼쪽
	Cam[1]->transfrom.Rotation = { 0, -90, 0 };
	//위					 
	Cam[2]->transfrom.Rotation = { -90, 0, 0 };
	//아래						 
	Cam[3]->transfrom.Rotation = { 90, 0, 0 };
	//앞 
	Cam[4]->transfrom.Rotation = { 0, 0, 0 };
	//뒤		    			 
	Cam[5]->transfrom.Rotation = { 0, 180, 0 };

	//카메라의 위치를 옮겨준다
	for (int i = 0; i < 6; i++)
	{
		Cam[i]->transfrom.Position = Pos;
	}

	//카메라와 랜더타겟의 초기값 넣어줌
	hsCamera* cam = Cam[0];
	ID3D11RenderTargetView* renderTargets = mDynamicCubeRTV[0];

	//여기 아래 부터는 그릴 랜더타겟과 뷰포트 설정
	mContext->OMSetRenderTargets(1, &renderTargets, mDynamicCubeDSV);
	mContext->RSSetViewports(1, &mDynamicCubeVP);

	hsViewData data;
	for (int i = 0; i < 6; i++)
	{
		//이번에 그릴 랜더타겟과 카메라 
		renderTargets = mDynamicCubeRTV[i];
		cam = Cam[i];

		//클리어
		mContext->ClearRenderTargetView(mDynamicCubeRTV[i], reinterpret_cast<const float*>(&Colors::Silver));
		//클리어
		mContext->ClearDepthStencilView(mDynamicCubeDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		//랜더타겟을 스텐실뷰와 통합
		mContext->OMSetRenderTargets(1, &renderTargets, mDynamicCubeDSV);

		//모든 오브젝트를 돌면서 현재 카메라 기준으로 찍는다
		data.world = cam->transfrom.GetMaster();
		data.view = cam->GetView();
		data.proj = cam->GetProj();

		for (auto j : objList)
		{
			j->Render(data);
		}

	}

	mContext->GenerateMips(mDynamicCubeSRV);



	ID3D11RenderTargetView* mRTV = ENGINE->GetViewData().RTV;
	ID3D11DepthStencilView* mDSV = ENGINE->GetViewData().DSV;
	D3D11_VIEWPORT mVP = ENGINE->GetViewData().VP;

	//병합
	mContext->OMSetRenderTargets(1, &mRTV, mDSV);
	//원래 그리던 뷰포트로 변환
	mContext->RSSetViewports(1, &mVP);

	return mDynamicCubeSRV;
}
ID3D11ShaderResourceView* RenderManager::ShadowMapRender(hsCamera* Cam, ScreenManager* mScreen)
{
	Off_AlphaBlending();
	On_ZBuffer();

	ID3D11RenderTargetView* mRTV = 0;
	ID3D11DepthStencilView* mDSV = mShadow->GetDSV();
	ID3D11ShaderResourceView* mSRV = mShadow->GetSRV();
	D3D11_VIEWPORT				mVP = mShadow->GetVP();

	////그릴곳의 랜더타겟과 뎁스 스텐실 뷰포트를 설정해준다
	mContext->OMSetRenderTargets(1, &mRTV, mDSV);
	mContext->RSSetViewports(1, &mVP);
	mContext->ClearDepthStencilView(mDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);

	Cam->isMain = true;
	Cam->Update();
	Cam->Render(hsViewData());

	////카메라를 기준으로 모든오브젝트들을 찍는다
	hsViewData data;
	data.world = Cam->transfrom.GetMaster();
	data.view = Cam->GetView();
	data.proj = Cam->GetProj();

	//빛을 쐈을때 오브젝트에 가려진 반대부분에 그림자를 그려줘야함
	//그러기위해 uv좌표를 변경시켜주는 행렬을 넘겨줌
	XMFLOAT4X4 T4x4 =
	{
		0.5f	,0		,0	,0,
		0		,-0.5f	,0	,0,
		0		,0		,1	,0,
		0.5f	,0.5f	,0	,1
	};
	//기본매쉬들을 텍스쳐와 쉐도우의 행렬계산한것을 넘겨준다
	XMMATRIX T = XMLoadFloat4x4(&T4x4);
	XMMATRIX ShadowXM = data.view * data.proj * T;

	ID3D11Buffer* shadowbuffer = nullptr;
	shadowbuffer = Rendering::GetBuffer("ShadowBuffer");
	ShadowData shadow;
	shadow.ShadowMatrix = XMMatrixTranspose(ShadowXM);

	//랜더링
	mScreen->Render(data);
	//이제 그만행..
	Cam->isMain = false;

	mContext->UpdateSubresource(shadowbuffer, 0, nullptr, &shadow, 0, 0);
	mContext->VSSetConstantBuffers(6, 1, &shadowbuffer);
	mContext->PSSetConstantBuffers(6, 1, &shadowbuffer);


	ID3D11RenderTargetView* ENGINE_RTV = mPost->GetRTV();
	ID3D11DepthStencilView* ENGINE_DSV = mPost->GetDSV();
	D3D11_VIEWPORT			ENGINE_VP = mPost->GetVP();
	mContext->OMSetRenderTargets(1, &ENGINE_RTV, ENGINE_DSV);
	mContext->RSSetViewports(1, &ENGINE_VP);

	//쉐도우 바인딩..
	mContext->PSSetShaderResources(4, 1, &mSRV);
	mContext->VSSetShaderResources(4, 1, &mSRV);
	Off_AlphaBlending();
	On_ZBuffer();
	return mSRV;
}
ID3D11ShaderResourceView** RenderManager::DeferredRender(hsCamera* cam, ScreenManager* mScreen)
{
	Off_AlphaBlending();
	On_ZBuffer();
	ID3D11DepthStencilView* mDSV = mDeferred->GetDSV();
	ID3D11ShaderResourceView** mSRV = mDeferred->GetSRV();
	ID3D11RenderTargetView** mRTV = mDeferred->GetRTV();
	D3D11_VIEWPORT				mVP = mDeferred->GetVP();
	LightManager* mLightManager = mScreen->GetLightManager();



	//그릴곳의 랜더타겟과 뎁스 스텐실 뷰포트를 설정해준다
	mContext->OMSetRenderTargets(mDeferred->GetArraySize(), mRTV, mDSV);
	mContext->RSSetViewports(1, &mVP);



	//이제 그릴것이므로 클리어
	for (int i = 0; i < mDeferred->GetArraySize(); i++)
	{
		mContext->ClearRenderTargetView((mRTV[i]), reinterpret_cast<const float*>(&Colors::Black));
		mContext->ClearDepthStencilView(mDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	//오브젝트들의 노말,컬러,등등을 찍기위해 한번 랜더링
	cam->isMain = true;
	cam->Update();
	cam->Render(hsViewData());
	hsViewData data;
	data.world = cam->transfrom.GetMaster();
	data.view = cam->GetView();
	data.proj = cam->GetProj();

	//오브젝트들을 랜더링해준다
	mScreen->Render(data);


	ID3D11DepthStencilView* m_Light_DSV = mLight->GetDSV();
	ID3D11ShaderResourceView* m_Light_SRV = mLight->GetSRV();
	ID3D11RenderTargetView* m_Light_RTV = mLight->GetRTV();


	//그릴곳의 랜더타겟과 뎁스 스텐실 뷰포트를 설정해준다
	mContext->OMSetRenderTargets(1, &m_Light_RTV, m_Light_DSV);
	mContext->RSSetViewports(1, &mVP);

	mContext->PSSetShaderResources(5, 1, &mSRV[0]);		//컬러
	mContext->PSSetShaderResources(6, 1, &mSRV[1]);		//노말
	mContext->PSSetShaderResources(7, 1, &mSRV[2]);		//위치
	mContext->PSSetShaderResources(8, 1, &mSRV[3]);		//탄젠트
	mContext->PSSetShaderResources(10, 1, &mSRV[4]);	//쉐도우
	//이렇게 나온 쉐이더 리로스 바인딩


	////이제 라이팅을 계산하기 위해서 다시 바인딩
	//이제 그릴것이므로 클리어
	mContext->ClearRenderTargetView(m_Light_RTV, reinterpret_cast<const float*>(&Colors::Black));
	mContext->ClearDepthStencilView(m_Light_DSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
	//
	cam->isMain = true;
	cam->Update();
	cam->Render(hsViewData());
	//라이트 랜더링
	mLightManager->Render(data);
	cam->isMain = false;


	//여기까지 했으면 mSRV 배열안에 화면으로 랜더링한 정보들이 들어있다
	ID3D11RenderTargetView* ENGINE_RTV = mPost->GetRTV();
	ID3D11DepthStencilView* ENGINE_DSV = mPost->GetDSV();
	D3D11_VIEWPORT ENGINE_VP = mPost->GetVP();
	mContext->OMSetRenderTargets(1, &ENGINE_RTV, ENGINE_DSV);
	mContext->RSSetViewports(1, &mVP);
	mContext->PSSetShaderResources(9, 1, &m_Light_SRV);	//라이팅


	On_ZBuffer();
	return  mSRV;
}
ID3D11ShaderResourceView* RenderManager::UIRender(hsCamera* cam, ScreenManager* mScreen)
{
	Off_ZBuffer();
	On_UIAlphaBlending();
	ID3D11DepthStencilView* m_UI_DSV = mUI->GetDSV();
	ID3D11ShaderResourceView* m_UI_SRV = mUI->GetSRV();
	ID3D11RenderTargetView* m_UI_RTV = mUI->GetRTV();
	D3D11_VIEWPORT	mVP = mUI->GetVP();


	Canvas* canvas = mScreen->GetCanvas();
	XMVECTORF32 DeepDarkGray = { 0.0f, 0.0f, 0.0f, 0.0f };
	mContext->ClearRenderTargetView(m_UI_RTV, DeepDarkGray);
	mContext->ClearDepthStencilView(m_UI_DSV, D3D11_CLEAR_DEPTH, 1.0f, 0);

	mContext->OMSetRenderTargets(1, &m_UI_RTV, m_UI_DSV);
	mContext->RSSetViewports(1, &mVP);

	cam->transfrom.Position = { 0, 0, 0 };
	cam->transfrom.Rotation = { 0, 0, 0 };

	cam->isMain = true;
	cam->Update();
	cam->Render(hsViewData());

	canvas->Update();
	canvas->Render();
	cam->isMain = false;


	ID3D11RenderTargetView* ENGINE_RTV = mPost->GetRTV();
	ID3D11DepthStencilView* ENGINE_DSV = mPost->GetDSV();
	D3D11_VIEWPORT ENGINE_VP = mPost->GetVP();
	mContext->OMSetRenderTargets(1, &ENGINE_RTV, ENGINE_DSV);
	mContext->RSSetViewports(1, &mVP);

	mContext->PSSetShaderResources(11, 1, &m_UI_SRV);
	mContext->PSSetShaderResources(11, 1, &m_UI_SRV);
	Off_AlphaBlending();
	On_ZBuffer();

	return m_UI_SRV;
}
ID3D11ShaderResourceView* RenderManager::AlphaRender(hsCamera* cam, ScreenManager* mScreen)
{
	On_AlphaBlending();
	Off_ZBuffer();
	ID3D11DepthStencilView* m_Alpha_DSV		= mAlpha->GetDSV();
	ID3D11ShaderResourceView* m_Alpha_SRV	= mAlpha->GetSRV();
	ID3D11RenderTargetView* m_Alpha_RTV		= mAlpha->GetRTV();
	D3D11_VIEWPORT	mVP = mAlpha->GetVP();

	AlphaManager* mAlpha = mScreen->GetAlphaManager();
	XMVECTORF32 DeepDarkGray = { 0.0f, 0.0f, 0.0f, 1.0f };
	mContext->ClearRenderTargetView(m_Alpha_RTV, DeepDarkGray);
	mContext->ClearDepthStencilView(m_Alpha_DSV, D3D11_CLEAR_DEPTH, 1.0f, 0);

	mContext->OMSetRenderTargets(1, &m_Alpha_RTV, m_Alpha_DSV);
	mContext->RSSetViewports(1, &mVP);


	//랜더링
	cam->isMain = true;
	cam->Update();
	cam->Render(hsViewData());
	mAlpha->Render();	
	cam->isMain = false;


	ID3D11RenderTargetView* ENGINE_RTV = mPost->GetRTV();
	ID3D11DepthStencilView* ENGINE_DSV = mPost->GetDSV();
	D3D11_VIEWPORT ENGINE_VP = mPost->GetVP();
	mContext->OMSetRenderTargets(1, &ENGINE_RTV, ENGINE_DSV);
	mContext->RSSetViewports(1, &mVP);

	mContext->PSSetShaderResources(12, 1, &m_Alpha_SRV);
	mContext->PSSetShaderResources(12, 1, &m_Alpha_SRV);
	Off_AlphaBlending();
	Off_ZBuffer();
	return nullptr;
}
void RenderManager::On_ZBuffer()
{
	mContext->OMSetDepthStencilState(m_DepthStencil_ZBuffer_ON_State, 1);
}
void RenderManager::On_UIAlphaBlending()
{
	// 블렌드 인수를 설정합니다.
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	// 알파 블렌딩을 켭니다.
	mContext->OMSetBlendState(m_alphaEnableBlending_ON_UI_State, blendFactor, 0xffffffff);
}
void RenderManager::On_AlphaBlending()
{
	// 블렌드 인수를 설정합니다.
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	// 알파 블렌딩을 켭니다.
	mContext->OMSetBlendState(m_alphaEnableBlending_ON_State, blendFactor, 0xffffffff);
}
void RenderManager::Off_ZBuffer()
{
	mContext->OMSetDepthStencilState(m_DepthStencil_ZBuffer_OFF_State, 1);
}
void RenderManager::Off_AlphaBlending()
{
	// 블렌드 인수를 설정합니다.
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	// 알파 블렌딩을 끕니다.
	mContext->OMSetBlendState(m_alphaEnableBlending_OFF_State, blendFactor, 0xffffffff);
}
void RenderManager::CreateTextureMap(string name, TEXTURE_BUFFER_TYPE type, int bufferSizeX, int bufferSizeY)
{
	//텍스쳐 맵을 생성하고 
	TextureMap* texture = new TextureMap(ENGINE);

	//타입에 따라 버퍼를 생성해줌 많이 사용되는것만
	switch (type)
	{
	case TEXTURE_BUFFER_TYPE::Basic:
		texture->Create_BasicBuffer(bufferSizeX, bufferSizeY);
		break;
	case TEXTURE_BUFFER_TYPE::Depth:
		texture->Create_DepthBuffer(bufferSizeX, bufferSizeY);
		break;
	case TEXTURE_BUFFER_TYPE::CubeMap:
		texture->Create_CubeBuffer(bufferSizeX, bufferSizeY);
		break;
	}
	TextureList.insert({ name,texture });
}
void RenderManager::CreateDepthStencilState()
{

	//Z 버퍼를켠 스테이트생성
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// DepthStencilState 구조체를 작성
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;
	// 픽셀 정면의 스텐실 설정
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	// 픽셀 뒷면의 스텐실 설정
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	HR(Device->CreateDepthStencilState(&depthStencilDesc, &m_DepthStencil_ZBuffer_ON_State));


	//Z 버퍼를끈 스테이트생성
	depthStencilDesc.DepthEnable = false;
	HR(Device->CreateDepthStencilState(&depthStencilDesc, &m_DepthStencil_ZBuffer_OFF_State));



	//알파블랜더 값설정
	D3D11_BLEND_DESC blendStateDescription;
	ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));
	blendStateDescription.AlphaToCoverageEnable = true;
	blendStateDescription.IndependentBlendEnable = true;
	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha	 = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;
	// 블렌드 상태를 생성합니다.
	FAILED(Device->CreateBlendState(&blendStateDescription, &m_alphaEnableBlending_ON_State));


	blendStateDescription;
	ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));
	blendStateDescription.AlphaToCoverageEnable = true;
	blendStateDescription.IndependentBlendEnable = true;
	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_MAX;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;
	// 블렌드 상태를 생성합니다.
	FAILED(Device->CreateBlendState(&blendStateDescription, &m_alphaEnableBlending_ON_UI_State));



	ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));
	blendStateDescription.AlphaToCoverageEnable = false;
	blendStateDescription.IndependentBlendEnable = false;
	blendStateDescription.RenderTarget[0].BlendEnable = false;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;
	FAILED(Device->CreateBlendState(&blendStateDescription, &m_alphaEnableBlending_OFF_State));

}
void RenderManager::Post_Processing_Start(hsCamera* _RenderCam, hsCamera* _LookCam, TextureObj* _text)
{
	//포스트 프로세싱에 필요한 변수들 받아주고
	mRenderCamera = _RenderCam;
	mTextureLookCamera = _LookCam;
	mRenderTextureObj = _text;

	mTextureLookCamera->transfrom.Position = { 0,0, -1 };

	//포스트 프로세싱 생성
	mPost = new TextureMap(ENGINE);
	mPost->Create_BasicBuffer(ENGINE->mClientWidth, ENGINE->mClientHeight);
	PostProcessing = true;

	//쉐도우 맵 생성
	mShadow = new TextureMap(ENGINE);
	mShadow->Create_DepthBuffer(4000, 4000);
}

void RenderManager::Deferred_Render_Start()
{
	//라이트를위한 텍스쳐맵을 생성
	mLight = new TextureMap(ENGINE);
	mLight->Create_BasicBuffer(ENGINE->mClientWidth, ENGINE->mClientHeight);

	//UI를위한 텍스쳐맵을 생성
	mUI = new TextureMap(ENGINE);
	mUI->Create_BasicBuffer(ENGINE->mClientWidth, ENGINE->mClientHeight);

	//알파오브젝트를 위한 텍스쳐맵
	mAlpha = new TextureMap(ENGINE);
	mAlpha->Create_BasicBuffer(ENGINE->mClientWidth, ENGINE->mClientHeight);

	mDeferred = new MultiTextureMap();
	mDeferred->init(ENGINE);
	mDeferred->Create_BasicBuffer(ENGINE->mClientWidth, ENGINE->mClientHeight);
}

void RenderManager::BeginRenderPost()
{
	ID3D11DepthStencilView* mPostDSV = mPost->GetDSV();
	ID3D11RenderTargetView* mPostRTV = mPost->GetRTV();
	ID3D11ShaderResourceView* mPostSRV = mPost->GetSRV();
	D3D11_VIEWPORT				mPostVP = mPost->GetVP();

	//포스트 프로세싱 전용 랜더타겟과 뎁스 바인딩
	mContext->OMSetRenderTargets(1, &mPostRTV, mPostDSV);
	mContext->RSSetViewports(1, &mPostVP);

	//클리어
	XMVECTORF32 DeepDarkGray = { 1, 1, 1, 1.0f };
	mContext->ClearRenderTargetView(mPostRTV, DeepDarkGray);
	mContext->ClearDepthStencilView(mPostDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void RenderManager::EndRenderPost()
{
	//여기까지왔으면 오브젝트 렌더링은 mPostRTV 여기에 그려져있을거임
	ID3D11DepthStencilView* ENGINE_DSV = ENGINE->GetViewData().DSV;
	ID3D11RenderTargetView* ENGINE_RTV = ENGINE->GetViewData().RTV;
	D3D11_VIEWPORT			ENGINE_VP = ENGINE->GetViewData().VP;

	//이제 스왑체인에 연결된 백버퍼로 연결해주고 클리어
	XMVECTORF32 DeepDarkGray = { 0.0f, 0.0f, 0.0f, 1.0f };
	mContext->OMSetRenderTargets(1, &ENGINE_RTV, ENGINE_DSV);



	mContext->RSSetViewports(1, &ENGINE_VP);
	mContext->ClearRenderTargetView(ENGINE_RTV, DeepDarkGray);
	mContext->ClearDepthStencilView(ENGINE_DSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	//텍스쳐로 만든 리소스를 오브젝트로 넘겨줌
	if (mTextureLookCamera != nullptr && mRenderTextureObj != nullptr)
	{
		hsViewData data;

		//텍스쳐를 lookCam위치로 그려야하기때문에 잠깐 메인 카메라의 권한을줌
		mTextureLookCamera->isMain = true;
		//모든쉐이더가 공유하는 cbuffer 카메라 행렬을 카메라의 행렬로 업데이트해줌
		mTextureLookCamera->Render(data);
		//할일다했으니 cbuffer업데이트할 권한을 끊음
		mTextureLookCamera->isMain = false;

		//업데이트된 행렬기준으로 랜더링함
		mRenderTextureObj->Update();
		mRenderTextureObj->SetTextures(mPost->GetSRV());
		mRenderTextureObj->Render(data);
	}
}






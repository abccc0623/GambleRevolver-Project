#include "EnginePCH.h"
#include "RenderManager.h"

bool RenderManager::PostProcessing = false;
RenderManager* RenderManager::RM = nullptr;
RenderManager::RenderManager()
{
	ENGINE = nullptr;
	Device = nullptr;
	mContext = nullptr;

	//���̳��� ������ �ʱ�ȭ
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

	//����Ʈ ���μ��� �����
	mPost->Delete();

	//������� �����
	mShadow->Delete();

	//���۵� �����
	mDeferred->Delete();

	//Ui�����
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

	//�׸����� ����Ÿ�ٰ� ���� ���ٽ� ����Ʈ�� �������ش�
	mContext->OMSetRenderTargets(1, &mRTV, mDSV);
	mContext->RSSetViewports(1, &mVP);

	//���� �׸����̹Ƿ� Ŭ����
	mContext->ClearRenderTargetView(mRTV, reinterpret_cast<const float*>(&Colors::Silver));
	mContext->ClearDepthStencilView(mDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);

	//ī�޶� �������� ��������Ʈ���� ��´�
	hsViewData data;
	//��� ����ī�޶��� �Ǹ��� �ش�
	Cam->isMain = true;
	//ī�޶��� ��ġ�� ������Ʈ�ϱ�����
	Cam->Update();
	//ī�޶��� ��,�������� ��Ʈ������ ������Ʈ�ϱ�����
	Cam->Render(data);
	//ī�޶��� ������ �޾ƿ�
	data.world = Cam->transfrom.GetMaster();
	data.view = Cam->GetView();
	data.proj = Cam->GetProj();
	//������
	for (auto j : objList)
	{
		j->Render(data);
	}
	//����ī�޶��� ������ �ٽû���
	Cam->isMain = false;


	ID3D11RenderTargetView* ENGINE_RTV = mPost->GetRTV();
	ID3D11DepthStencilView* ENGINE_DSV = mPost->GetDSV();
	D3D11_VIEWPORT ENGINE_VP = mPost->GetVP();

	//�ٽ� ���ε�
	mContext->OMSetRenderTargets(1, &ENGINE_RTV, ENGINE_DSV);
	mContext->RSSetViewports(1, &ENGINE_VP);


	//�׸��� ���̴� ���ҽ� �ǵ�����
	return mSRV;
}
ID3D11ShaderResourceView* RenderManager::DynamicCubeMapRender(hsCamera* Cam[6], std::vector<GameObject*>& objList, XMFLOAT3 Pos)
{
	//������
	Cam[0]->transfrom.Rotation = { 0, 90, 0 };
	//����
	Cam[1]->transfrom.Rotation = { 0, -90, 0 };
	//��					 
	Cam[2]->transfrom.Rotation = { -90, 0, 0 };
	//�Ʒ�						 
	Cam[3]->transfrom.Rotation = { 90, 0, 0 };
	//�� 
	Cam[4]->transfrom.Rotation = { 0, 0, 0 };
	//��		    			 
	Cam[5]->transfrom.Rotation = { 0, 180, 0 };

	//ī�޶��� ��ġ�� �Ű��ش�
	for (int i = 0; i < 6; i++)
	{
		Cam[i]->transfrom.Position = Pos;
	}

	//ī�޶�� ����Ÿ���� �ʱⰪ �־���
	hsCamera* cam = Cam[0];
	ID3D11RenderTargetView* renderTargets = mDynamicCubeRTV[0];

	//���� �Ʒ� ���ʹ� �׸� ����Ÿ�ٰ� ����Ʈ ����
	mContext->OMSetRenderTargets(1, &renderTargets, mDynamicCubeDSV);
	mContext->RSSetViewports(1, &mDynamicCubeVP);

	hsViewData data;
	for (int i = 0; i < 6; i++)
	{
		//�̹��� �׸� ����Ÿ�ٰ� ī�޶� 
		renderTargets = mDynamicCubeRTV[i];
		cam = Cam[i];

		//Ŭ����
		mContext->ClearRenderTargetView(mDynamicCubeRTV[i], reinterpret_cast<const float*>(&Colors::Silver));
		//Ŭ����
		mContext->ClearDepthStencilView(mDynamicCubeDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		//����Ÿ���� ���ٽǺ�� ����
		mContext->OMSetRenderTargets(1, &renderTargets, mDynamicCubeDSV);

		//��� ������Ʈ�� ���鼭 ���� ī�޶� �������� ��´�
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

	//����
	mContext->OMSetRenderTargets(1, &mRTV, mDSV);
	//���� �׸��� ����Ʈ�� ��ȯ
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

	////�׸����� ����Ÿ�ٰ� ���� ���ٽ� ����Ʈ�� �������ش�
	mContext->OMSetRenderTargets(1, &mRTV, mDSV);
	mContext->RSSetViewports(1, &mVP);
	mContext->ClearDepthStencilView(mDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);

	Cam->isMain = true;
	Cam->Update();
	Cam->Render(hsViewData());

	////ī�޶� �������� ��������Ʈ���� ��´�
	hsViewData data;
	data.world = Cam->transfrom.GetMaster();
	data.view = Cam->GetView();
	data.proj = Cam->GetProj();

	//���� ������ ������Ʈ�� ������ �ݴ�κп� �׸��ڸ� �׷������
	//�׷������� uv��ǥ�� ��������ִ� ����� �Ѱ���
	XMFLOAT4X4 T4x4 =
	{
		0.5f	,0		,0	,0,
		0		,-0.5f	,0	,0,
		0		,0		,1	,0,
		0.5f	,0.5f	,0	,1
	};
	//�⺻�Ž����� �ؽ��Ŀ� �������� ��İ���Ѱ��� �Ѱ��ش�
	XMMATRIX T = XMLoadFloat4x4(&T4x4);
	XMMATRIX ShadowXM = data.view * data.proj * T;

	ID3D11Buffer* shadowbuffer = nullptr;
	shadowbuffer = Rendering::GetBuffer("ShadowBuffer");
	ShadowData shadow;
	shadow.ShadowMatrix = XMMatrixTranspose(ShadowXM);

	//������
	mScreen->Render(data);
	//���� �׸���..
	Cam->isMain = false;

	mContext->UpdateSubresource(shadowbuffer, 0, nullptr, &shadow, 0, 0);
	mContext->VSSetConstantBuffers(6, 1, &shadowbuffer);
	mContext->PSSetConstantBuffers(6, 1, &shadowbuffer);


	ID3D11RenderTargetView* ENGINE_RTV = mPost->GetRTV();
	ID3D11DepthStencilView* ENGINE_DSV = mPost->GetDSV();
	D3D11_VIEWPORT			ENGINE_VP = mPost->GetVP();
	mContext->OMSetRenderTargets(1, &ENGINE_RTV, ENGINE_DSV);
	mContext->RSSetViewports(1, &ENGINE_VP);

	//������ ���ε�..
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



	//�׸����� ����Ÿ�ٰ� ���� ���ٽ� ����Ʈ�� �������ش�
	mContext->OMSetRenderTargets(mDeferred->GetArraySize(), mRTV, mDSV);
	mContext->RSSetViewports(1, &mVP);



	//���� �׸����̹Ƿ� Ŭ����
	for (int i = 0; i < mDeferred->GetArraySize(); i++)
	{
		mContext->ClearRenderTargetView((mRTV[i]), reinterpret_cast<const float*>(&Colors::Black));
		mContext->ClearDepthStencilView(mDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	//������Ʈ���� �븻,�÷�,����� ������� �ѹ� ������
	cam->isMain = true;
	cam->Update();
	cam->Render(hsViewData());
	hsViewData data;
	data.world = cam->transfrom.GetMaster();
	data.view = cam->GetView();
	data.proj = cam->GetProj();

	//������Ʈ���� ���������ش�
	mScreen->Render(data);


	ID3D11DepthStencilView* m_Light_DSV = mLight->GetDSV();
	ID3D11ShaderResourceView* m_Light_SRV = mLight->GetSRV();
	ID3D11RenderTargetView* m_Light_RTV = mLight->GetRTV();


	//�׸����� ����Ÿ�ٰ� ���� ���ٽ� ����Ʈ�� �������ش�
	mContext->OMSetRenderTargets(1, &m_Light_RTV, m_Light_DSV);
	mContext->RSSetViewports(1, &mVP);

	mContext->PSSetShaderResources(5, 1, &mSRV[0]);		//�÷�
	mContext->PSSetShaderResources(6, 1, &mSRV[1]);		//�븻
	mContext->PSSetShaderResources(7, 1, &mSRV[2]);		//��ġ
	mContext->PSSetShaderResources(8, 1, &mSRV[3]);		//ź��Ʈ
	mContext->PSSetShaderResources(10, 1, &mSRV[4]);	//������
	//�̷��� ���� ���̴� ���ν� ���ε�


	////���� �������� ����ϱ� ���ؼ� �ٽ� ���ε�
	//���� �׸����̹Ƿ� Ŭ����
	mContext->ClearRenderTargetView(m_Light_RTV, reinterpret_cast<const float*>(&Colors::Black));
	mContext->ClearDepthStencilView(m_Light_DSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
	//
	cam->isMain = true;
	cam->Update();
	cam->Render(hsViewData());
	//����Ʈ ������
	mLightManager->Render(data);
	cam->isMain = false;


	//������� ������ mSRV �迭�ȿ� ȭ������ �������� �������� ����ִ�
	ID3D11RenderTargetView* ENGINE_RTV = mPost->GetRTV();
	ID3D11DepthStencilView* ENGINE_DSV = mPost->GetDSV();
	D3D11_VIEWPORT ENGINE_VP = mPost->GetVP();
	mContext->OMSetRenderTargets(1, &ENGINE_RTV, ENGINE_DSV);
	mContext->RSSetViewports(1, &mVP);
	mContext->PSSetShaderResources(9, 1, &m_Light_SRV);	//������


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


	//������
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
	// ���� �μ��� �����մϴ�.
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	// ���� ������ �մϴ�.
	mContext->OMSetBlendState(m_alphaEnableBlending_ON_UI_State, blendFactor, 0xffffffff);
}
void RenderManager::On_AlphaBlending()
{
	// ���� �μ��� �����մϴ�.
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	// ���� ������ �մϴ�.
	mContext->OMSetBlendState(m_alphaEnableBlending_ON_State, blendFactor, 0xffffffff);
}
void RenderManager::Off_ZBuffer()
{
	mContext->OMSetDepthStencilState(m_DepthStencil_ZBuffer_OFF_State, 1);
}
void RenderManager::Off_AlphaBlending()
{
	// ���� �μ��� �����մϴ�.
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	// ���� ������ ���ϴ�.
	mContext->OMSetBlendState(m_alphaEnableBlending_OFF_State, blendFactor, 0xffffffff);
}
void RenderManager::CreateTextureMap(string name, TEXTURE_BUFFER_TYPE type, int bufferSizeX, int bufferSizeY)
{
	//�ؽ��� ���� �����ϰ� 
	TextureMap* texture = new TextureMap(ENGINE);

	//Ÿ�Կ� ���� ���۸� �������� ���� ���Ǵ°͸�
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

	//Z ���۸��� ������Ʈ����
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// DepthStencilState ����ü�� �ۼ�
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;
	// �ȼ� ������ ���ٽ� ����
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	// �ȼ� �޸��� ���ٽ� ����
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	HR(Device->CreateDepthStencilState(&depthStencilDesc, &m_DepthStencil_ZBuffer_ON_State));


	//Z ���۸��� ������Ʈ����
	depthStencilDesc.DepthEnable = false;
	HR(Device->CreateDepthStencilState(&depthStencilDesc, &m_DepthStencil_ZBuffer_OFF_State));



	//���ĺ��� ������
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
	// ���� ���¸� �����մϴ�.
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
	// ���� ���¸� �����մϴ�.
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
	//����Ʈ ���μ��̿� �ʿ��� ������ �޾��ְ�
	mRenderCamera = _RenderCam;
	mTextureLookCamera = _LookCam;
	mRenderTextureObj = _text;

	mTextureLookCamera->transfrom.Position = { 0,0, -1 };

	//����Ʈ ���μ��� ����
	mPost = new TextureMap(ENGINE);
	mPost->Create_BasicBuffer(ENGINE->mClientWidth, ENGINE->mClientHeight);
	PostProcessing = true;

	//������ �� ����
	mShadow = new TextureMap(ENGINE);
	mShadow->Create_DepthBuffer(4000, 4000);
}

void RenderManager::Deferred_Render_Start()
{
	//����Ʈ������ �ؽ��ĸ��� ����
	mLight = new TextureMap(ENGINE);
	mLight->Create_BasicBuffer(ENGINE->mClientWidth, ENGINE->mClientHeight);

	//UI������ �ؽ��ĸ��� ����
	mUI = new TextureMap(ENGINE);
	mUI->Create_BasicBuffer(ENGINE->mClientWidth, ENGINE->mClientHeight);

	//���Ŀ�����Ʈ�� ���� �ؽ��ĸ�
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

	//����Ʈ ���μ��� ���� ����Ÿ�ٰ� ���� ���ε�
	mContext->OMSetRenderTargets(1, &mPostRTV, mPostDSV);
	mContext->RSSetViewports(1, &mPostVP);

	//Ŭ����
	XMVECTORF32 DeepDarkGray = { 1, 1, 1, 1.0f };
	mContext->ClearRenderTargetView(mPostRTV, DeepDarkGray);
	mContext->ClearDepthStencilView(mPostDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void RenderManager::EndRenderPost()
{
	//������������� ������Ʈ �������� mPostRTV ���⿡ �׷�����������
	ID3D11DepthStencilView* ENGINE_DSV = ENGINE->GetViewData().DSV;
	ID3D11RenderTargetView* ENGINE_RTV = ENGINE->GetViewData().RTV;
	D3D11_VIEWPORT			ENGINE_VP = ENGINE->GetViewData().VP;

	//���� ����ü�ο� ����� ����۷� �������ְ� Ŭ����
	XMVECTORF32 DeepDarkGray = { 0.0f, 0.0f, 0.0f, 1.0f };
	mContext->OMSetRenderTargets(1, &ENGINE_RTV, ENGINE_DSV);



	mContext->RSSetViewports(1, &ENGINE_VP);
	mContext->ClearRenderTargetView(ENGINE_RTV, DeepDarkGray);
	mContext->ClearDepthStencilView(ENGINE_DSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	//�ؽ��ķ� ���� ���ҽ��� ������Ʈ�� �Ѱ���
	if (mTextureLookCamera != nullptr && mRenderTextureObj != nullptr)
	{
		hsViewData data;

		//�ؽ��ĸ� lookCam��ġ�� �׷����ϱ⶧���� ��� ���� ī�޶��� ��������
		mTextureLookCamera->isMain = true;
		//��罦�̴��� �����ϴ� cbuffer ī�޶� ����� ī�޶��� ��ķ� ������Ʈ����
		mTextureLookCamera->Render(data);
		//���ϴ������� cbuffer������Ʈ�� ������ ����
		mTextureLookCamera->isMain = false;

		//������Ʈ�� ��ı������� ��������
		mRenderTextureObj->Update();
		mRenderTextureObj->SetTextures(mPost->GetSRV());
		mRenderTextureObj->Render(data);
	}
}






#include "EnginePCH.h"
#include "TextureMap.h"

TextureMap::TextureMap(hsEngine* _ENGINE)
{
	ENGINE		= _ENGINE;
	Device		= ENGINE->GetDeviceData()->m_device;
	mContext	= ENGINE->GetDeviceData()->m_deviceContext;
	

	mDSV = nullptr;
	mRTV = nullptr;
	mSRV = nullptr;
	mVP = D3D11_VIEWPORT();
}

TextureMap::~TextureMap()
{
	Delete();
}

void TextureMap::Reset(int TextureSizeX, int TextureSizeY)
{
	mDSV->Release();
	mRTV->Release();
	mSRV->Release();
	
	mDSV = nullptr;
	mRTV = nullptr;
	mSRV = nullptr;



	D3D11_TEXTURE2D_DESC depthDesc;
	depthDesc.Width = TextureSizeX;					//�ؽ��İ��� 
	depthDesc.Height = TextureSizeY;				//�ؽ��ļ���
	depthDesc.MipLevels = 1;					//�Ӹʼ��� (���� ���ٽ� ���۸� �����ؽ��Ŀ����� �ϳ���������ȴ�)
	depthDesc.ArraySize = 1;					//�ý��� �迭���ý��� ���� (���� ���ٽ� ���۸� �����ؽ��Ŀ����� �ϳ���������ȴ�)
	depthDesc.Format = DXGI_FORMAT_D32_FLOAT;	//�ȼ��� ������ ����
	depthDesc.SampleDesc.Count = 1;
	depthDesc.SampleDesc.Quality = 0;
	depthDesc.Usage = D3D11_USAGE_DEFAULT;
	depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthDesc.CPUAccessFlags = 0;
	depthDesc.MiscFlags = 0;


	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = TextureSizeX;					//�ؽ��İ��� 
	texDesc.Height = TextureSizeY;					//�ؽ��ļ���
	texDesc.MipLevels = 1;					//�Ӹʼ��� (���� ���ٽ� ���۸� �����ؽ��Ŀ����� �ϳ���������ȴ�)
	texDesc.ArraySize = 1;					//�ý��� �迭���ý��� ���� (���� ���ٽ� ���۸� �����ؽ��Ŀ����� �ϳ���������ȴ�)
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	//�ȼ��� ������ ����
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	//���� Ÿ�ٿ� ���� �ؽ��� 2D ������
	ID3D11Texture2D* Tex;
	HR(Device->CreateTexture2D(&texDesc, 0, &Tex));

	ID3D11Texture2D* depth = 0;
	HR(Device->CreateTexture2D(&depthDesc, 0, &depth));

	//����Ÿ�ٰ� 
	HR(Device->CreateRenderTargetView(Tex, 0, &mRTV));


	HR(Device->CreateDepthStencilView(depth, 0, &mDSV));




	//���̴� �� ������ ��������ü
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.TextureCube.MostDetailedMip = 0;
	srvDesc.TextureCube.MipLevels = -1;
	HR(Device->CreateShaderResourceView(Tex, &srvDesc, &mSRV));


	//����Ʈ
	mVP.TopLeftX = 0;
	mVP.TopLeftY = 0;
	mVP.Width = static_cast<float>(TextureSizeX);
	mVP.Height = static_cast<float>(TextureSizeY);
	mVP.MinDepth = 0.0f;
	mVP.MaxDepth = 1.0f;


	Tex->Release();
	depth->Release();
}

void TextureMap::Create_BasicBuffer(int TextureSizeX, int TextureSizeY)
{
	
	D3D11_TEXTURE2D_DESC depthDesc;
	depthDesc.Width = TextureSizeX;					//�ؽ��İ��� 
	depthDesc.Height = TextureSizeY;				//�ؽ��ļ���
	depthDesc.MipLevels = 1;					//�Ӹʼ��� (���� ���ٽ� ���۸� �����ؽ��Ŀ����� �ϳ���������ȴ�)
	depthDesc.ArraySize = 1;					//�ý��� �迭���ý��� ���� (���� ���ٽ� ���۸� �����ؽ��Ŀ����� �ϳ���������ȴ�)
	depthDesc.Format = DXGI_FORMAT_D32_FLOAT;	//�ȼ��� ������ ����
	depthDesc.SampleDesc.Count = 1;
	depthDesc.SampleDesc.Quality = 0;
	depthDesc.Usage = D3D11_USAGE_DEFAULT;
	depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthDesc.CPUAccessFlags = 0;
	depthDesc.MiscFlags = 0;


	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = TextureSizeX;					//�ؽ��İ��� 
	texDesc.Height = TextureSizeY;					//�ؽ��ļ���
	texDesc.MipLevels = 1;					//�Ӹʼ��� (���� ���ٽ� ���۸� �����ؽ��Ŀ����� �ϳ���������ȴ�)
	texDesc.ArraySize = 1;					//�ý��� �迭���ý��� ���� (���� ���ٽ� ���۸� �����ؽ��Ŀ����� �ϳ���������ȴ�)
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	//�ȼ��� ������ ����
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	//���� Ÿ�ٿ� ���� �ؽ��� 2D ������
	ID3D11Texture2D* Tex;
	HR(Device->CreateTexture2D(&texDesc, 0, &Tex));

	ID3D11Texture2D* depth = 0;
	HR(Device->CreateTexture2D(&depthDesc, 0, &depth));

	//����Ÿ�ٰ� 
	HR(Device->CreateRenderTargetView(Tex, 0, &mRTV));


	HR(Device->CreateDepthStencilView(depth, 0, &mDSV));




	//���̴� �� ������ ��������ü
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.TextureCube.MostDetailedMip = 0;
	srvDesc.TextureCube.MipLevels = -1;
	HR(Device->CreateShaderResourceView(Tex, &srvDesc, &mSRV));


	//����Ʈ
	mVP.TopLeftX = 0;
	mVP.TopLeftY = 0;
	mVP.Width = static_cast<float>(TextureSizeX);
	mVP.Height = static_cast<float>(TextureSizeY);
	mVP.MinDepth = 0.0f;
	mVP.MaxDepth = 1.0f;


	Tex->Release();
	depth->Release();
}

void TextureMap::Create_DepthBuffer(int TextureSizeX, int TextureSizeY)
{
	D3D11_TEXTURE2D_DESC TexDesc;
	TexDesc.Width = TextureSizeX;
	TexDesc.Height = TextureSizeY;
	TexDesc.MipLevels = 1;
	TexDesc.ArraySize = 1;
	TexDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	TexDesc.SampleDesc.Count = 1;
	TexDesc.SampleDesc.Quality = 0;
	TexDesc.Usage = D3D11_USAGE_DEFAULT;
	TexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	TexDesc.CPUAccessFlags = 0;
	TexDesc.MiscFlags = 0;
	ID3D11Texture2D* depth = 0;
	HR(Device->CreateTexture2D(&TexDesc, 0, &depth));


	//���ٽ�
	ID3D11Texture2D* depthMap = 0;
	HR(Device->CreateTexture2D(&TexDesc, 0, &depthMap));


	D3D11_DEPTH_STENCIL_VIEW_DESC stencil;
	stencil.Flags = 0;
	stencil.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	stencil.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	stencil.Texture2D.MipSlice = 0;
	HR(Device->CreateDepthStencilView(depthMap, &stencil, &mDSV));
	


	//���̴� �� ������ ��������ü
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.TextureCube.MostDetailedMip = 0;
	srvDesc.TextureCube.MipLevels = -1;
	HR(Device->CreateShaderResourceView(depthMap, &srvDesc, &mSRV));


	////����Ʈ
	mVP.TopLeftX = 0;
	mVP.TopLeftY = 0;
	mVP.Width = static_cast<float>(TextureSizeX);
	mVP.Height = static_cast<float>(TextureSizeY);
	mVP.MinDepth = 0.0f;
	mVP.MaxDepth = 1.0f;
	
	depth->Release();
}

void TextureMap::Create_CubeBuffer(int TextureSizeX, int TextureSizeY)
{
	///����Ÿ���� 6���� �������ϴµ�...
	///�̰Ŷ����� �� �̻����� �ϴ��� ����...
	//D3D11_TEXTURE2D_DESC texDesc;
	//texDesc.Width = 256;
	//texDesc.Height = 256;
	//texDesc.MipLevels = 0;
	//texDesc.ArraySize = 6;
	//texDesc.SampleDesc.Count = 1;
	//texDesc.SampleDesc.Quality = 0;
	//texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//texDesc.Usage = D3D11_USAGE_DEFAULT;
	//texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	//texDesc.CPUAccessFlags = 0;
	//texDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS | D3D11_RESOURCE_MISC_TEXTURECUBE;
	//
	////���� Ÿ�ٿ� ���� �ؽ��� 2D ������
	//ID3D11Texture2D* cubeTex = 0;
	//HR(Device->CreateTexture2D(&texDesc, 0, &cubeTex));
	//
	//for (int i = 0; i < 6; i++)
	//{
	//	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	//	rtvDesc.Format = texDesc.Format;
	//	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
	//	rtvDesc.Texture2DArray.ArraySize = 1;
	//	rtvDesc.Texture2DArray.MipSlice = 0;
	//
	//	//������ �ؽ��Ŀ� ����Ÿ���� ����
	//	rtvDesc.Texture2DArray.FirstArraySlice = i;
	//	HR(Device->CreateRenderTargetView(cubeTex, &rtvDesc, &mDynamicCubeRTV[i]));
	//}
	//
	////���ٽ� �並 ���� 2D�ؽ��ĸ� �ϳ�������
	//D3D11_TEXTURE2D_DESC depthTexDesc;
	//depthTexDesc.Width = 256;
	//depthTexDesc.Height = 256;
	//depthTexDesc.MipLevels = 1;
	//depthTexDesc.ArraySize = 1;
	//depthTexDesc.SampleDesc.Count = 1;
	//depthTexDesc.SampleDesc.Quality = 0;
	//depthTexDesc.Format = DXGI_FORMAT_D32_FLOAT;
	//depthTexDesc.Usage = D3D11_USAGE_DEFAULT;
	//depthTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	//depthTexDesc.CPUAccessFlags = 0;
	//depthTexDesc.MiscFlags = 0;
	//
	////����
	//ID3D11Texture2D* depthTex = 0;
	//HR(Device->CreateTexture2D(&depthTexDesc, 0, &depthTex));
	//
	//
	////���ٽ� �� ����ü ����
	//D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	//dsvDesc.Format = depthTexDesc.Format;
	//dsvDesc.Flags = 0;
	//dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	//dsvDesc.Texture2D.MipSlice = 0;
	////���� ���� �ؽ��Ŀ� ����ü�� ����
	//HR(Device->CreateDepthStencilView(depthTex, &dsvDesc, &mDynamicCubeDSV));
	//
	//
	////���̴� �� ������ ��������ü
	//D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	//srvDesc.Format = texDesc.Format;
	//srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	//srvDesc.TextureCube.MostDetailedMip = 0;
	//srvDesc.TextureCube.MipLevels = -1;
	//HR(Device->CreateShaderResourceView(cubeTex, &srvDesc, &mDynamicCubeSRV));
	//
	//
	//mDynamicCubeVP.TopLeftX = 0;
	//mDynamicCubeVP.TopLeftY = 0;
	//mDynamicCubeVP.Width = static_cast<float>(256);
	//mDynamicCubeVP.Height = static_cast<float>(256);
	//mDynamicCubeVP.MinDepth = 0.0f;
	//mDynamicCubeVP.MaxDepth = 1.0f;
	//
	//
	//cubeTex->Release();
	//depthTex->Release();
}

void TextureMap::Create_MultiRTV(int TextureSizeX, int TextureSizeY)
{
	D3D11_TEXTURE2D_DESC depthDesc;
	depthDesc.Width = TextureSizeX;					//�ؽ��İ��� 
	depthDesc.Height = TextureSizeY;				//�ؽ��ļ���
	depthDesc.MipLevels = 1;					//�Ӹʼ��� (���� ���ٽ� ���۸� �����ؽ��Ŀ����� �ϳ���������ȴ�)
	depthDesc.ArraySize = 1;					//�ý��� �迭���ý��� ���� (���� ���ٽ� ���۸� �����ؽ��Ŀ����� �ϳ���������ȴ�)
	depthDesc.Format = DXGI_FORMAT_D32_FLOAT;	//�ȼ��� ������ ����
	depthDesc.SampleDesc.Count = 1;
	depthDesc.SampleDesc.Quality = 0;
	depthDesc.Usage = D3D11_USAGE_DEFAULT;
	depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthDesc.CPUAccessFlags = 0;
	depthDesc.MiscFlags = 0;


	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = TextureSizeX;						//�ؽ��İ��� 
	texDesc.Height = TextureSizeY;						//�ؽ��ļ���
	texDesc.MipLevels = 1;								//�Ӹʼ��� (���� ���ٽ� ���۸� �����ؽ��Ŀ����� �ϳ���������ȴ�)
	texDesc.ArraySize = 1;								//�ý��� �迭���ý��� ���� (���� ���ٽ� ���۸� �����ؽ��Ŀ����� �ϳ���������ȴ�)
	texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;	//�ȼ��� ������ ����
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	//���� Ÿ�ٿ� ���� �ؽ��� 2D ������
	for (int i = 0; i < BUFFER_SIZE; i++)
	{
		HR(Device->CreateTexture2D(&texDesc, 0, &mMulti_Texture[i]));
	}
	


	ID3D11Texture2D* depth = 0;
	HR(Device->CreateTexture2D(&depthDesc, 0, &depth));

	//����Ÿ�ٰ� 
	for (int i = 0; i < BUFFER_SIZE; i++) {

		HR(Device->CreateRenderTargetView(mMulti_Texture[i], 0, &mMulti_RTV[i]));
	}


	HR(Device->CreateDepthStencilView(depth, 0, &mDSV));




	//���̴� �� ������ ��������ü
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.TextureCube.MostDetailedMip = 0;
	srvDesc.TextureCube.MipLevels = -1;

	for (int i = 0; i < BUFFER_SIZE; i++)
	{
		HR(Device->CreateShaderResourceView(mMulti_Texture[i], &srvDesc, &mMulti_SRV[i]));
	}


	//����Ʈ
	mVP.TopLeftX = 0;
	mVP.TopLeftY = 0;
	mVP.Width = static_cast<float>(TextureSizeX);
	mVP.Height = static_cast<float>(TextureSizeY);
	mVP.MinDepth = 0.0f;
	mVP.MaxDepth = 1.0f;


	//Tex->Release();
	depth->Release();
}

void TextureMap::Delete()
{
	//���� ���ٽ� ����
	if (mDSV != nullptr)
	{
		mDSV->Release();
	}

	//����Ÿ�� ����
	if (mRTV != nullptr)
	{
		mRTV->Release();
	}

	//���ҽ��� ����
	if (mSRV != nullptr)
	{
		mSRV->Release();
	}

	//�������� �����ֱ�..
	ENGINE		= nullptr;
	Device		= nullptr;
	mContext	= nullptr;
}

ID3D11DepthStencilView* TextureMap::GetDSV()
{
	return mDSV;
}

ID3D11RenderTargetView* TextureMap::GetRTV()
{
	return mRTV;
}

ID3D11ShaderResourceView* TextureMap::GetSRV()
{
	return mSRV;
}

D3D11_VIEWPORT TextureMap::GetVP()
{
	return mVP;
}

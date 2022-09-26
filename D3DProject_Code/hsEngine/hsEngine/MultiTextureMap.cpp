#include "EnginePCH.h"
#include "MultiTextureMap.h"

MultiTextureMap::MultiTextureMap()
{
	ENGINE		= nullptr;
	Device		= nullptr;
	mContext	= nullptr;


	mDSV = nullptr;

	for (int i = 0; i < BUFFER_SIZE; i++)
	{
		mMulti_RTV[i]		= nullptr;
		mMulti_SRV[i]		= nullptr;
		mMulti_Texture[i]	= nullptr;
	}
	mVP = D3D11_VIEWPORT();


}

MultiTextureMap::~MultiTextureMap()
{
	ENGINE = nullptr;
	Device = nullptr;
	mContext = nullptr;

}

void MultiTextureMap::init(hsEngine* m_ENGINE)
{
	ENGINE		= m_ENGINE;
	Device		= m_ENGINE->GetDeviceData()->m_device;
	mContext	= m_ENGINE->GetDeviceData()->m_deviceContext;
}

void MultiTextureMap::Reset(int TextureSizeX, int TextureSizeY)
{
	//랜더타겟뷰와 쉐이더 리소스뷰 텍스쳐 뎁스스텐실을 다른크기로 다시만들기위해 일단지운다
	if (mMulti_RTV != nullptr)
	{
		for (int i = 0; i < BUFFER_SIZE; i++)
		{
			mMulti_RTV[i]->Release();
			mMulti_RTV[i] = nullptr;
		}
	};

	if (mMulti_SRV != nullptr)
	{
		for (int i = 0; i < BUFFER_SIZE; i++)
		{
			mMulti_SRV[i]->Release();
			mMulti_SRV[i] = nullptr;
		}
	};

	if (mMulti_Texture != nullptr) 
	{
		for (int i = 0; i < BUFFER_SIZE; i++)
		{
			mMulti_Texture[i]->Release();
			mMulti_Texture[i] = nullptr;
		}
	};

	if (mDSV != nullptr)
	{
		mDSV->Release();
		mDSV = nullptr;
	}
	Create_BasicBuffer(TextureSizeX, TextureSizeY);
}

void MultiTextureMap::Create_BasicBuffer(int TextureSizeX, int TextureSizeY)
{
	D3D11_TEXTURE2D_DESC depthDesc;
	depthDesc.Width = TextureSizeX;					//텍스쳐가로 
	depthDesc.Height = TextureSizeY;				//텍스쳐세로
	depthDesc.MipLevels = 1;					//밉맵수준 (깊이 스텐실 버퍼를 위한텍스쳐에서는 하나만있으면된다)
	depthDesc.ArraySize = 1;					//택스쳐 배열의택스쳐 개수 (깊이 스텐실 버퍼를 위한텍스쳐에서는 하나만있으면된다)
	depthDesc.Format = DXGI_FORMAT_D32_FLOAT;	//픽셀의 형식을 뜻함
	depthDesc.SampleDesc.Count = 1;
	depthDesc.SampleDesc.Quality = 0;
	depthDesc.Usage = D3D11_USAGE_DEFAULT;
	depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthDesc.CPUAccessFlags = 0;
	depthDesc.MiscFlags = 0;


	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = TextureSizeX;					//텍스쳐가로 
	texDesc.Height = TextureSizeY;					//텍스쳐세로
	texDesc.MipLevels = 1;					//밉맵수준 (깊이 스텐실 버퍼를 위한텍스쳐에서는 하나만있으면된다)
	texDesc.ArraySize = 1;					//택스쳐 배열의택스쳐 개수 (깊이 스텐실 버퍼를 위한텍스쳐에서는 하나만있으면된다)
	texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;	//픽셀의 형식을 뜻함
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	//랜더 타겟에 사용될 텍스쳐 2D 생성함
	for (int i = 0; i < BUFFER_SIZE; i++)
	{
		HR(Device->CreateTexture2D(&texDesc, 0, &mMulti_Texture[i]));
	}



	ID3D11Texture2D* depth = 0;
	HR(Device->CreateTexture2D(&depthDesc, 0, &depth));

	//랜더타겟과 
	for (int i = 0; i < BUFFER_SIZE; i++) {

		HR(Device->CreateRenderTargetView(mMulti_Texture[i], 0, &mMulti_RTV[i]));
	}


	HR(Device->CreateDepthStencilView(depth, 0, &mDSV));




	//쉐이더 로 보내줄 정보구조체
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.TextureCube.MostDetailedMip = 0;
	srvDesc.TextureCube.MipLevels = -1;

	for (int i = 0; i < BUFFER_SIZE; i++)
	{
		HR(Device->CreateShaderResourceView(mMulti_Texture[i], &srvDesc, &mMulti_SRV[i]));
	}


	//뷰포트
	mVP.TopLeftX = 0;
	mVP.TopLeftY = 0;
	mVP.Width = static_cast<float>(TextureSizeX);
	mVP.Height = static_cast<float>(TextureSizeY);
	mVP.MinDepth = 0.0f;
	mVP.MaxDepth = 1.0f;


	//Tex->Release();
	depth->Release();
}

void MultiTextureMap::Delete()
{
	for (int i = 0; i < BUFFER_SIZE; i++)
	{
		mMulti_RTV[i]->Release();
		mMulti_SRV[i]->Release();
		mMulti_Texture[i]->Release();
	}
}

const int MultiTextureMap::GetArraySize()
{
	return BUFFER_SIZE;
}

ID3D11DepthStencilView* MultiTextureMap::GetDSV()
{
	return mDSV;
}

ID3D11RenderTargetView** MultiTextureMap::GetRTV()
{
	return mMulti_RTV;
}

ID3D11ShaderResourceView** MultiTextureMap::GetSRV()
{
	return mMulti_SRV;
}

D3D11_VIEWPORT MultiTextureMap::GetVP()
{
	return mVP;
}

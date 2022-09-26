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
	//����Ÿ�ٺ�� ���̴� ���ҽ��� �ؽ��� �������ٽ��� �ٸ�ũ��� �ٽø�������� �ϴ������
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

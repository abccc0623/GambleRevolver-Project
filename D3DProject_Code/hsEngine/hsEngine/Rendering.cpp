#include "EnginePCH.h"
#include "Rendering.h"

std::map<string, ID3D11Buffer*>* Rendering::cBufferList = nullptr;
Rendering::Rendering()
{
	m_ENGINE	= nullptr;
	m_Device	= nullptr;
	m_Context	= nullptr;

	VertexShader	= nullptr;
	PixelShader		= nullptr;
	Layout			= nullptr;
	SamplerState	= nullptr;

	mVB = nullptr;
	mIB = nullptr;

	mMainSRV	= nullptr;
	mNomalSRV	= nullptr;
	mSubSRV		= nullptr;

	UINT stride = 0;
	UINT offset = 0;
	int vcount = 0;
	int icount = 0;
}


Rendering::~Rendering()
{
	m_ENGINE = nullptr;
	m_Device = nullptr;
	m_Context = nullptr;

	VertexShader	= nullptr;
	PixelShader		= nullptr;
	Layout			= nullptr;
	SamplerState	= nullptr;

	mMainSRV	= nullptr;
	mNomalSRV	= nullptr;
	mSubSRV		= nullptr;
}

void Rendering::init(hsEngine* _ENGINE)
{
	m_ENGINE	= _ENGINE;
	m_Device	= m_ENGINE->GetDeviceData()->m_device;
	m_Context	= m_ENGINE->GetDeviceData()->m_deviceContext;
}

void Rendering::MeshRendering(bool DebugMode)
{
	UINT TextureNumber = 0;
	//������ �� �� �ֵ��� �Է� ��������� ���� ���۸� Ȱ������ �����մϴ�.
	//stride= sizeof(Skinnde32);
	m_Context->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
	m_Context->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);
	m_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	if (DebugMode == true)
	{
		m_Context->RSSetState(m_ENGINE->GetDeviceData()->mWireframeRS);
	}
	else
	{
		m_Context->RSSetState(m_ENGINE->GetDeviceData()->mSolidRS);
	}

	//��ǲ ���̾ƿ� ����
	m_Context->IASetInputLayout(Layout);
	
	// �ﰢ���� �׸� ���� ���̴��� �ȼ� ���̴��� �����մϴ�.
	m_Context->VSSetShader(VertexShader,NULL, 0);
	m_Context->PSSetShader(PixelShader, NULL, 0);
	
	SamplerList;
	// �ȼ� ���̴����� ���÷� ���¸� �����մϴ�.
	m_Context->PSSetSamplers(0, 1, &(*SamplerList)["SampleType"]);
	m_Context->PSSetSamplers(1, 1, &(*SamplerList)["samShadow"]);
	
	// �ﰢ���� �׸��ϴ�.
	m_Context->DrawIndexed(icount, 0, 0);
}

void Rendering::Set_CameraBuffer_Parameter(ID3D11Buffer* buffer,XMFLOAT3 Pos, XMMATRIX m_View, XMMATRIX m_Proj)
{
	CameraBuffer dataPtr;
	//ī�޶� ��ü���� ������Ʈ ���ٰ���
	dataPtr.viewproj		= XMMatrixTranspose(m_View* m_Proj);
	dataPtr.CamPos			= Pos;
	dataPtr.ViewMatrix		= XMMatrixTranspose(m_View);

	//���ҽ��� ����
	m_Context->UpdateSubresource(buffer, 0, nullptr, &dataPtr, 0, 0);

	// ���������� ���� ���̴��� ��� ���۸� �ٲ� ������ �ٲߴϴ�.
	m_Context->VSSetConstantBuffers(0, 1, &buffer);
}

void Rendering::Set_ObjectBuffer_Parameter(ID3D11Buffer* buffer,XMMATRIX m_World, XMFLOAT3 TexOffet)
{
	//�ؽ��� ��뿩��
	Texture_Mask =1;
	if (mMainSRV != nullptr)
	{
		Texture_Mask |= TEXTURE_MAIN;
	}
	if (mSubSRV != nullptr)
	{
		Texture_Mask |= TEXTURE_SUB;
	}
	if (mNomalSRV != nullptr)
	{
		Texture_Mask |= TEXTURE_NOMAL;
	}

	m_Context->PSSetShaderResources(TEXTURE_TYPE_MAIN, 1, &mMainSRV);
	m_Context->PSSetShaderResources(TEXTURE_TYPE_SUB, 1, &mSubSRV);
	m_Context->PSSetShaderResources(TEXTURE_TYPE_NOMAL, 1, &mNomalSRV);
	
	//�ؽ����� ������ 
	XMFLOAT4X4 offet = 
	{
		TexOffet.x	,0			,0			,0,
		0			,TexOffet.y	,0			,0,
		0			,0			,TexOffet.z	,0,
		0			,0			,0			,1
	};
	XMMATRIX offetMT = XMLoadFloat4x4(&offet);


	ObjectBuffer dataPtr;
	dataPtr.world			= XMMatrixTranspose(m_World);
	dataPtr.TexMatrix		= XMMatrixTranspose(offetMT);
	dataPtr.Texture_Mask	= Texture_Mask;
	dataPtr.Alpha			= 0.5f;
	//���ҽ��� ����
	m_Context->UpdateSubresource(buffer, 0, nullptr, &dataPtr, 0,0);

	// ���������� ���� ���̴��� ��� ���۸� �ٲ� ������ �ٲߴϴ�.
	m_Context->VSSetConstantBuffers(1, 1, &buffer);
	m_Context->PSSetConstantBuffers(1, 1, &buffer);
}

void Rendering::Set_SkinningBuffer_Parameter(ID3D11Buffer* buffer, XMMATRIX m_World, std::vector<DirectX::XMMATRIX>* boneOffset)
{
	//�ؽ��� ��뿩��
	Texture_Mask = 1;
	if (mMainSRV != nullptr)
	{
		Texture_Mask |= TEXTURE_MAIN;
	}
	if (mSubSRV != nullptr)
	{
		Texture_Mask |= TEXTURE_SUB;
	}
	if (mNomalSRV != nullptr)
	{
		Texture_Mask |= TEXTURE_NOMAL;
	}
	m_Context->PSSetShaderResources(TEXTURE_TYPE_MAIN, 1, &mMainSRV);
	m_Context->PSSetShaderResources(TEXTURE_TYPE_SUB, 1, &mSubSRV);
	m_Context->PSSetShaderResources(TEXTURE_TYPE_NOMAL, 1, &mNomalSRV);

	//�ؽ����� ������ 
	//XMFLOAT4X4 offet =
	//{
	//	TexOffet.x	,0			,0			,0,
	//	0			,TexOffet.y	,0			,0,
	//	0			,0			,TexOffet.z	,0,
	//	0			,0			,0			,1
	//};
	//XMMATRIX offetMT = XMLoadFloat4x4(&offet);


	SkinningBuffer dataPtr;
	dataPtr.SkinningworldMatrix = XMMatrixTranspose(m_World);
	dataPtr.SkinningTexMatrix = XMMatrixTranspose(XMMatrixIdentity());
	for (int i = 0; i < (*boneOffset).size(); i++)
	{
		dataPtr.gBoneTransforms[i] = XMMatrixTranspose((*boneOffset)[i]);
	}
	dataPtr.Texture_Mask = Texture_Mask;

	//���ҽ��� ����
	m_Context->UpdateSubresource(buffer, 0, nullptr, &dataPtr, 0, 0);

	// ���������� ���� ���̴��� ��� ���۸� �ٲ� ������ �ٲߴϴ�.
	m_Context->VSSetConstantBuffers(2, 1, &buffer);
	m_Context->PSSetConstantBuffers(2, 1, &buffer);

}

void Rendering::Set_UIBuffer_Parameter(ID3D11Buffer* buffer, XMMATRIX m_World, XMFLOAT3 TexOffet, float Alpha)
{
	//�ؽ��� ��뿩��
	Texture_Mask = 1;
	if (mMainSRV != nullptr)
	{
		Texture_Mask |= TEXTURE_MAIN;
	}
	if (mSubSRV != nullptr)
	{
		Texture_Mask |= TEXTURE_SUB;
	}
	if (mNomalSRV != nullptr)
	{
		Texture_Mask |= TEXTURE_NOMAL;
	}

	
	m_Context->PSSetShaderResources(TEXTURE_TYPE_MAIN, 1, &mMainSRV);
	m_Context->PSSetShaderResources(TEXTURE_TYPE_SUB, 1, &mSubSRV);
	m_Context->PSSetShaderResources(TEXTURE_TYPE_NOMAL, 1, &mNomalSRV);

	//�ؽ����� ������ 
	XMFLOAT4X4 offet =
	{
		TexOffet.x	,0			,0			,0,
		0			,TexOffet.y	,0			,0,
		0			,0			,TexOffet.z	,0,
		0			,0			,0			,1
	};
	XMMATRIX offetMT = XMLoadFloat4x4(&offet);


	UIbuffer dataPtr;
	dataPtr.UIMatrix = XMMatrixTranspose(m_World);
	dataPtr.UITexMatrix = XMMatrixTranspose(offetMT);
	dataPtr.Alpha = Alpha;

	//���ҽ��� ����
	m_Context->UpdateSubresource(buffer, 0, nullptr, &dataPtr, 0, 0);

	// ���������� ���� ���̴��� ��� ���۸� �ٲ� ������ �ٲߴϴ�.
	m_Context->VSSetConstantBuffers(7, 1, &buffer);
	m_Context->PSSetConstantBuffers(7, 1, &buffer);
}

void Rendering::Set_ParticleBuffer_Parameter(ID3D11Buffer* buffer, XMMATRIX m_World, XMFLOAT3 TexOffet, XMFLOAT4 Color)
{
	//�ؽ��� ��뿩��
	Texture_Mask = 1;
	if (mMainSRV != nullptr)
	{
		Texture_Mask |= TEXTURE_MAIN;
	}
	if (mSubSRV != nullptr)
	{
		Texture_Mask |= TEXTURE_SUB;
	}
	if (mNomalSRV != nullptr)
	{
		Texture_Mask |= TEXTURE_NOMAL;
	}

	m_Context->PSSetShaderResources(TEXTURE_TYPE_MAIN, 1, &mMainSRV);
	m_Context->PSSetShaderResources(TEXTURE_TYPE_SUB, 1, &mSubSRV);
	m_Context->PSSetShaderResources(TEXTURE_TYPE_NOMAL, 1, &mNomalSRV);

	//�ؽ����� ������ 
	XMFLOAT4X4 offet =
	{
		TexOffet.x	,0			,0			,0,
		0			,TexOffet.y	,0			,0,
		0			,0			,TexOffet.z	,0,
		0			,0			,0			,1
	};
	XMMATRIX offetMT = XMLoadFloat4x4(&offet);


	Particlebuffer dataPtr;
	dataPtr.ParticleMatrix = XMMatrixTranspose(m_World);
	dataPtr.ParticleTexMatrix = XMMatrixTranspose(offetMT);
	dataPtr.ParticleColor = Color;

	//���ҽ��� ����
	m_Context->UpdateSubresource(buffer, 0, nullptr, &dataPtr, 0, 0);

	// ���������� ���� ���̴��� ��� ���۸� �ٲ� ������ �ٲߴϴ�.
	m_Context->VSSetConstantBuffers(8, 1, &buffer);
	m_Context->PSSetConstantBuffers(8, 1, &buffer);
}

void Rendering::SetContextBufferList(ShaderData data)
{
	cBufferList = data.cBufferList;
}

void Rendering::SetShaderData(ShaderData data)
{
	VertexShader	= data.VertexShader;
	PixelShader		= data.PixelShader;
	Layout			= data.Layout;
	SamplerState	= data.SamplerState;

	//������۸���Ʈ
	cBufferList = data.cBufferList;
	//���÷� ����Ʈ
	SamplerList = data.SamplerList;
}

ID3D11Buffer* Rendering::GetBuffer(string BufferName)
{
	if (cBufferList->size() == 0) { return nullptr; }
	return (*cBufferList)[BufferName];
}






#include "EnginePCH.h"
#include "TextureShader.h"

TextureShader::TextureShader()
{
	m_vertexShader	= nullptr;
	m_pixelShader	= nullptr;
	m_layout		= nullptr;
	m_matrixBuffer	= nullptr;
	m_sampleState	= nullptr;

	ENGINE		= nullptr;
	Device		= nullptr;
	mContext	= nullptr;
}

TextureShader::~TextureShader()
{


}

void TextureShader::init(hsEngine* Engine)
{
	ENGINE		= Engine;
	Device		= ENGINE->GetDeviceData()->m_device;
	mContext	= ENGINE->GetDeviceData()->m_deviceContext;


	LoadShader(L"../../FX/texture_vs.hlsl", L"../../FX/texture_ps.hlsl");
}

bool TextureShader::LoadShader(const WCHAR* vsFileName, const WCHAR* psFileName)
{
	HRESULT result;

	ID3D10Blob* vertexShaderBuffer = nullptr;
	ID3D10Blob* pixelShaderBuffer = nullptr;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef DEBUG
	dwShaderFlags |= D3DCOMPILE_DEBUG;
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif // DEBUG



	//���ý� ���̴��� �ε� �Ѵ�
	result = D3DCompileFromFile
	(
		vsFileName,							//���̴� �ڵ尡 ����ִ� �����̸�
		NULL,								//���̴� ��ũ�θ� ���� ������������� NULL
		NULL,								//�⺻���� �ڵ鷯�� ���� ������ (�⺻�� NULL)
		"TextureVertexShader",				//���̴� ������ ���۵Ǵ� ���̴� ������ �Լ��� �̸�
		"vs_5_0",							//�������� ���̴� ��� ����
		dwShaderFlags,						//������ ���̴� �÷��� 1
		0,									//������ ���̴� �÷��� 2
		&vertexShaderBuffer,				//��ȯ��
		NULL								//�����޼��� ��ȯ��
	);
	//�����ϸ� ����..
	if (FAILED(result))
	{
		return false;
	}


	//�ȼ� ���̴��� �ε��Ѵ�
	result = D3DCompileFromFile
	(
		psFileName,
		NULL,
		NULL,
		"TexturePixelShader",
		"ps_5_0",
		dwShaderFlags,
		0,
		&pixelShaderBuffer,
		NULL
	);
	//�����ϸ� ����..
	if (FAILED(result))
	{
		return false;
	}


	
	//���۷� ���� ���� ���̴��� �����Ѵ�
	result = Device->CreateVertexShader
	(
		vertexShaderBuffer->GetBufferPointer(), //�����ϵ� ���̴��� ���� ������
		vertexShaderBuffer->GetBufferSize(),	//�����ϵ� �������̴��� ũ��
		NULL,									//Ŭ���� ��ũ �������̽��� ���� ������
		&m_vertexShader							//ID3D11VertexShader �������̽��� ���� ������ �ּ�
	);
	//�����ϸ� ����..
	if (FAILED(result))
	{
		return false;
	}


	// ���ۿ��� �ȼ� ���̴��� �����մϴ�.
	result = Device->CreatePixelShader
	(
		pixelShaderBuffer->GetBufferPointer(),
		pixelShaderBuffer->GetBufferSize(),
		NULL,
		&m_pixelShader
	);
	//�����ϸ� ����..
	if (FAILED(result))
	{
		return false;
	}

	//���� inputLayout �� �������ش� ���� �ǳ� ������.. ��Ű���ҋ� ����ߴ���.. 
	//������ ���������� ������ �̷��� ����ü
	//Layout[0].SemanticName = "POSITION";
	//Layout[0].SemanticIndex = 0;
	//Layout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	//Layout[0].InputSlot = 0;
	//Layout[0].AlignedByteOffset = 0;
	//Layout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	//Layout[0].InstanceDataStepRate = 0;

	D3D11_INPUT_ELEMENT_DESC Layout[2] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	//���̾ƿ��� ��� ���� �����´�
	//��ü ���̾ƿ� ������� �ϳ��� ���̾ƿ� ����� ������ ��������´�..
	UINT numElements = sizeof(Layout) / sizeof(Layout[0]);

	//���� �Է� ���̾ƿ��� �����
	result = Device->CreateInputLayout
	(
		Layout,
		numElements,
		vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(),
		&m_layout
	);
	//���� ����
	if (FAILED(result))
	{
		return false;
	}

	//���̻� ��������ʴ� ���ؽ����̴��� �ȼ����̴� ���۸� ����
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;
	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;


	// ���� ���̴��� �ִ� ��� ��� ������ ����ü�� �ۼ��մϴ�.
	D3D11_BUFFER_DESC matrixBufferDesc;
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;				//������ �б� �� ������ ���
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);		//������ ũ��
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	//���۰� ������ ������ ��ĳ ���ε��Ǵ��� �÷���
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	//CPU �������� �ʿ����� �ʴ°�� CPU������ �÷���
	matrixBufferDesc.MiscFlags = 0;								//��Ÿ �÷��� D3D11_RESOURCE_MISC_FLAG ����
	matrixBufferDesc.StructureByteStride = 0;					//���� �������� ���۰� ����ȭ�� ���۸� ��Ÿ���� ��� ������� ũ��

	// ��� ���� �����͸� ����� ��Ŭ�������� �������̴� ������ۿ� �����Ҽ��ֵ���
	result = Device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
	//���п���
	if (FAILED(result))
	{
		return false;
	}


	//�ؽ��� ���÷� ���� ����ü ����
	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// �ؽ�ó ���÷� ���¸� ����ϴ�.
	result = Device->CreateSamplerState(&samplerDesc, &m_sampleState);
	if (FAILED(result))
	{
		return false;
	}


	//��.. �ϼ�
	return true;
}

bool TextureShader::SetParameters(XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture)
{
	//��� ���۳����� �����ֵ��� ��Ʈ������ ����
	
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);

	// ��� ������ ������ �� �� �ֵ���
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	if (FAILED(mContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
	{
		return false;
	}

	// ��� ������ �����Ϳ� ���� ������ ��������
	MatrixBufferType* dataPtr = (MatrixBufferType*)mappedResource.pData;

	//������ ����
	dataPtr->world		= worldMatrix;
	dataPtr->view		= viewMatrix;
	dataPtr->projection = projectionMatrix;

	// ��� ������ ����� Ǳ�ϴ�.
	mContext->Unmap(m_matrixBuffer, 0);

	// ���� ���̴������� ��� ������ ��ġ�� �����մϴ�.
	unsigned int bufferNumber = 0;

	// ���� ���̴��� ���� ����
	mContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);





	return false;
}

void TextureShader::RenderShader(int indexCount)
{
	// ���� �Է� ���̾ƿ��� �����մϴ�.
	mContext->IASetInputLayout(m_layout);

	// �ﰢ���� �׸� ���� ���̴��� �ȼ� ���̴��� �����մϴ�.
	mContext->VSSetShader(m_vertexShader, NULL, 0);
	mContext->PSSetShader(m_pixelShader, NULL, 0);

	// �ȼ� ���̴����� ���÷� ���¸� �����մϴ�.
	mContext->PSSetSamplers(0, 1, &m_sampleState);

	// �ﰢ���� �׸��ϴ�.
	mContext->DrawIndexed(indexCount, 0, 0);

}

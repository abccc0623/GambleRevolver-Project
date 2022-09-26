
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



	//버택스 쉐이더를 로드 한다
	result = D3DCompileFromFile
	(
		vsFileName,							//쉐이더 코드가 들어있는 파일이름
		NULL,								//쉐이더 매크로를 정의 사용하지않으면 NULL
		NULL,								//기본포함 핸들러에 대한 포인터 (기본값 NULL)
		"TextureVertexShader",				//쉐이더 실행이 시작되는 쉐이더 진입점 함수의 이름
		"vs_5_0",							//컴파일할 쉐이더 대상 버전
		dwShaderFlags,						//컴파일 쉐이더 플레그 1
		0,									//컴파일 쉐이더 플레그 2
		&vertexShaderBuffer,				//반환값
		NULL								//에러메세지 반환값
	);
	//실패하면 리턴..
	if (FAILED(result))
	{
		return false;
	}


	//픽셀 쉐이더를 로드한다
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
	//실패하면 리턴..
	if (FAILED(result))
	{
		return false;
	}


	
	//버퍼로 부터 정점 쉐이더를 생성한다
	result = Device->CreateVertexShader
	(
		vertexShaderBuffer->GetBufferPointer(), //컴파일된 쉐이더의 대한 포인터
		vertexShaderBuffer->GetBufferSize(),	//컴파일된 정점쉐이더의 크기
		NULL,									//클래스 링크 인터페이스에 대한 포인터
		&m_vertexShader							//ID3D11VertexShader 인터페이스에 대한 포인터 주소
	);
	//실패하면 리턴..
	if (FAILED(result))
	{
		return false;
	}


	// 버퍼에서 픽셀 쉐이더를 생성합니다.
	result = Device->CreatePixelShader
	(
		pixelShaderBuffer->GetBufferPointer(),
		pixelShaderBuffer->GetBufferSize(),
		NULL,
		&m_pixelShader
	);
	//실패하면 리턴..
	if (FAILED(result))
	{
		return false;
	}

	//이제 inputLayout 을 지정해준다 내가 맨날 쓰던거.. 스키닝할떄 고생했던거.. 
	//지금은 생략했지만 원래는 이러한 구조체
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

	//레이아웃의 요소 수를 가져온다
	//전체 레이아웃 사이즈에서 하나의 레이아웃 사이즈를 나누면 몇개인지나온다..
	UINT numElements = sizeof(Layout) / sizeof(Layout[0]);

	//정점 입력 레이아웃을 만든다
	result = Device->CreateInputLayout
	(
		Layout,
		numElements,
		vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(),
		&m_layout
	);
	//실패 여부
	if (FAILED(result))
	{
		return false;
	}

	//더이상 사용하지않는 버텍스쉐이더와 픽셀쉐이더 버퍼를 삭제
	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;
	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;


	// 정점 셰이더에 있는 행렬 상수 버퍼의 구조체를 작성합니다.
	D3D11_BUFFER_DESC matrixBufferDesc;
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;				//버퍼의 읽기 및 쓰기의 방법
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);		//버퍼의 크기
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	//버퍼가 파이프 라인의 어캐 바인딩되는지 플래그
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	//CPU 엑세스가 필요하지 않는경우 CPU엑세스 플레그
	matrixBufferDesc.MiscFlags = 0;								//기타 플레그 D3D11_RESOURCE_MISC_FLAG 참조
	matrixBufferDesc.StructureByteStride = 0;					//버퍼 구조에서 버퍼가 구조화된 버퍼를 나타내는 경우 각요소의 크기

	// 상수 버퍼 포인터를 만들어 이클래스에서 정점쉐이더 상수버퍼에 접근할수있도록
	result = Device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
	//실패여부
	if (FAILED(result))
	{
		return false;
	}


	//텍스쳐 샘플러 상태 구조체 생성
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

	// 텍스처 샘플러 상태를 만듭니다.
	result = Device->CreateSamplerState(&samplerDesc, &m_sampleState);
	if (FAILED(result))
	{
		return false;
	}


	//하.. 완성
	return true;
}

bool TextureShader::SetParameters(XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture)
{
	//상수 버퍼내용을 쓸수있도록 매트릭스로 변경
	
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);

	// 상수 버퍼의 내용을 쓸 수 있도록
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	if (FAILED(mContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
	{
		return false;
	}

	// 상수 버퍼의 데이터에 대한 포인터 가져오기
	MatrixBufferType* dataPtr = (MatrixBufferType*)mappedResource.pData;

	//데이터 전달
	dataPtr->world		= worldMatrix;
	dataPtr->view		= viewMatrix;
	dataPtr->projection = projectionMatrix;

	// 상수 버퍼의 잠금을 풉니다.
	mContext->Unmap(m_matrixBuffer, 0);

	// 정점 셰이더에서의 상수 버퍼의 위치를 설정합니다.
	unsigned int bufferNumber = 0;

	// 정점 쉐이더의 값을 전달
	mContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);





	return false;
}

void TextureShader::RenderShader(int indexCount)
{
	// 정점 입력 레이아웃을 설정합니다.
	mContext->IASetInputLayout(m_layout);

	// 삼각형을 그릴 정점 셰이더와 픽셀 셰이더를 설정합니다.
	mContext->VSSetShader(m_vertexShader, NULL, 0);
	mContext->PSSetShader(m_pixelShader, NULL, 0);

	// 픽셀 쉐이더에서 샘플러 상태를 설정합니다.
	mContext->PSSetSamplers(0, 1, &m_sampleState);

	// 삼각형을 그립니다.
	mContext->DrawIndexed(indexCount, 0, 0);

}

#include "EnginePCH.h"
#include "ShaderManager.h"

ID3D11ClassInstance** ShaderManager::g_DynamicLinkageArray = nullptr;
ShaderManager::ShaderManager()
{
	ENGINE = nullptr;
	Device = nullptr;
	mContext = nullptr;

	ShaderFilePath = "../../FX/";
}


ShaderManager::~ShaderManager()
{
	ENGINE = nullptr;
	Device = nullptr;
	mContext = nullptr;
}

void ShaderManager::init(hsEngine* _ENGINE)
{
	ENGINE		= _ENGINE;
	Device		= ENGINE->GetDeviceData()->m_device;
	mContext	= ENGINE->GetDeviceData()->m_deviceContext;


	//기본적으로 제공하는 오브젝트들만 넣어줌
	LoadShader("basic_vs.hlsl", "basic_ps.hlsl", "Basic");

	LoadShader("Skinning_vs.hlsl", "Skinning_ps.hlsl", "Skinning");

	LoadShader("sky_vs.hlsl", "sky_ps.hlsl", "Sky");

	LoadShader("texture_vs.hlsl", "texture_ps.hlsl", "Texture");
	
	LoadShader("debug_vs.hlsl", "debug_ps.hlsl", "Debug");

	LoadShader("Deferred_vs.hlsl", "Deferred_ps.hlsl", "Deferred");

	LoadShader("DirLight_vs.hlsl", "DirLight_ps.hlsl", "DirLight");

	LoadShader("Light_vs.hlsl", "Light_ps.hlsl", "Light");

	LoadShader("Post_vs.hlsl", "Post_ps.hlsl", "Post");

	LoadShader("Particle_vs.hlsl", "Particle_ps.hlsl", "Particle");
}

void ShaderManager::Delete()
{

	//샘플러 삭제
	std::map<string, ID3D11SamplerState*>::iterator temp02 = samplerList.begin();
	for (temp02; temp02 != samplerList.end(); temp02++)
	{
		ID3D11SamplerState* k = temp02->second;
		temp02->second = nullptr;
		k->Release();
	}
	samplerList.clear();

}

bool ShaderManager::LoadShader(string vsFileName, string psFileName,string ShaderName)
{
	ShaderData Data;
	ShaderList.insert({ShaderName,Data });

	//버택스 쉐이더를 로드한다 이떄 인풋레이아웃 로드
	LoadVertexShader(vsFileName, ShaderName);

	//픽셀 쉐이더를 로드한다
	LoadPixelShader(psFileName, ShaderName);

	//샘플러를 만들어줌
	//CreateSamPler(ShaderName);

	//상수버퍼와 샘플러 리스트는 그냥 주소값만 가져옴
	Data.cBufferList = &CbufferList;
	Data.SamplerList = &samplerList;

	return true;
}

bool ShaderManager::CreateSamPler(string shaderName)
{
	//일단 쉐이더 열고
	D3D11_SHADER_DESC desc;
	HR(ReFlector->GetDesc(&desc));
	
	//바인딩된 리소스의 수
	UINT Count = desc.BoundResources;
	
	for (int i = 0; i < Count; i++)
	{
		D3D11_SHADER_INPUT_BIND_DESC temp;
		ReFlector->GetResourceBindingDesc(i, &temp);
		//바인딩된 놈의 타입과 이름
		UINT	type = temp.Type;
		string	name = temp.Name;

		//이놈은 셈플러이다
		if (type == D3D_SIT_SAMPLER)
		{
			//만약 같은이름의샘플러가 이미있다면 그냥 아무것도 안함
			if (samplerList.find(name) != samplerList.end())
			{
				continue;
			}
		
			UINT num = temp.NumSamples;
			temp.uFlags;
			temp.BindPoint;
			temp.BindCount;
		
			
		
			//텍스쳐 샘플러 상태 구조체 생성
			ID3D11SamplerState* sampler;
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

			ID3D11SamplerState* sampler00;
			D3D11_SAMPLER_DESC sampler00Desc;
			sampler00Desc.Filter = D3D11_FILTER_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
			sampler00Desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
			sampler00Desc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
			sampler00Desc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
			sampler00Desc.MipLODBias = 0.0f;
			sampler00Desc.MaxAnisotropy = 1;
			sampler00Desc.ComparisonFunc = D3D11_COMPARISON_LESS;
			sampler00Desc.BorderColor[0] = 0;
			sampler00Desc.BorderColor[1] = 0;
			sampler00Desc.BorderColor[2] = 0;
			sampler00Desc.BorderColor[3] = 1;
			sampler00Desc.MinLOD = 0;
			sampler00Desc.MaxLOD = D3D11_FLOAT32_MAX;


		
			// 텍스처 샘플러 상태를 만듭니다.
			HR(Device->CreateSamplerState(&samplerDesc, &sampler));
			HR(Device->CreateSamplerState(&sampler00Desc, &sampler00));
		
			samplerList.insert({ name,sampler });
			samplerList.insert({ "samShadow",sampler00 });
		}
	}

	return true;
}

void ShaderManager::SetShaderFilePath(string path)
{
	ShaderFilePath = path;
}

void ShaderManager::CreateConstantBuffer()
{
	//일단 쉐이더연다

	//변수의 타입
	D3D_SHADER_VARIABLE_TYPE var_type;

	//쉐이더의 정보를 가져온다
	D3D11_SHADER_DESC desc;
	HR(ReFlector->GetDesc(&desc));

	//쉐이더의 cbuffer의 수
	UINT CBufferCount = desc.ConstantBuffers;
	//c버퍼를 연다!!
	//버퍼의 개수만큼 cbuffer를 가져옴
	for (int i = 0; i < CBufferCount; i++)
	{
		ID3D11ShaderReflectionConstantBuffer* Cbuffer = ReFlector->GetConstantBufferByIndex(i);
		D3D11_SHADER_BUFFER_DESC buffer_Desc;
		HR(Cbuffer->GetDesc(&buffer_Desc));
		string name = buffer_Desc.Name;

		//만약 이미버퍼에 들어가있는 상수버퍼라면
		if (CbufferList.find(name) != CbufferList.end())
		{
			continue;
		}

		//총 전체의 사이즈
	
		UINT size = buffer_Desc.Size;
		UINT plags = buffer_Desc.uFlags;
		//변수의 개수
		UINT var = buffer_Desc.Variables;
		

		ID3D11Buffer* m_GlobalBuffer;
		D3D11_BUFFER_DESC matrixBufferDesc;
		matrixBufferDesc.Usage					= D3D11_USAGE_DEFAULT;
		matrixBufferDesc.ByteWidth				= size;
		matrixBufferDesc.BindFlags				= D3D11_BIND_CONSTANT_BUFFER;
		matrixBufferDesc.CPUAccessFlags			= 0;
		matrixBufferDesc.MiscFlags				= 0;
		matrixBufferDesc.StructureByteStride	= 0;

		//버퍼를 만들어줌
		HR(Device->CreateBuffer(&matrixBufferDesc, NULL, &m_GlobalBuffer));

		//상수버퍼를 넣어줌
		CbufferList.insert({ name ,m_GlobalBuffer });
	}
}

void ShaderManager::CreateInputLayout(string shaderName,ID3D10Blob* vertexBuffer)
{
	//쉐이더의 정보를 가져온다
	D3D11_SHADER_DESC desc;
	HR(ReFlector->GetDesc(&desc));


	//m_Reflector->GetV
	//쉐이더의 Layout의 수
	UINT inputLayCount = desc.InputParameters;
	desc.InputPrimitive;

	std::vector<D3D11_INPUT_ELEMENT_DESC> DescList;

	int TypeSize = 0;
	int index = 0;
	

	DescList.resize(inputLayCount);
	for (int i = 0; i < inputLayCount; i++)
	{
		//D3D11_INPUT_ELEMENT_DESC* DescList = new D3D11_INPUT_ELEMENT_DESC;
		//test.push_back(DescList);


		D3D11_SIGNATURE_PARAMETER_DESC desc;
		ReFlector->GetInputParameterDesc(i, &desc);


		int number = (int)desc.ComponentType;

		//타입의 사이즈를 구한다 ex)float2사이즈 , float3의 사이즈
		BYTE numberMask = desc.Mask;

		DXGI_FORMAT FormatNumver = FindFormat(number, numberMask, desc);

		//한개의 layout 구조체 완성..
		DescList[i].SemanticName = desc.SemanticName;
		DescList[i].SemanticIndex = 0;
		DescList[i].Format = FormatNumver;
		DescList[i].InputSlot = 0;
		DescList[i].AlignedByteOffset = TypeSize;
		DescList[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		DescList[i].InstanceDataStepRate = 0;

		
		//이자료형의 끝을 알려주기위해 다음 자료형이 시작하는부분을 넣어줌
		switch (numberMask)
		{
		case 0b00000001:
			TypeSize += 4;
			break;
		case 0b00000011:
			TypeSize += 8;
			break;
		case 0b00000111:
			TypeSize += 12;
			break;
		case 0b00001111:
			TypeSize += 16;
			break;
		}
	}
	
	ID3D10Blob* vertexShaderBuffer = vertexBuffer;
	ID3D11InputLayout* m_layout = nullptr;

	HRESULT result;

	//정점 입력 레이아웃을 만든다
	result = Device->CreateInputLayout
	(
		&DescList[0],
		inputLayCount,
		vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(),
		&m_layout
	);

	//실패 여부
	if (FAILED(result))
	{
		return;
	}
	ShaderList[shaderName].Layout = m_layout;
	
}

DXGI_FORMAT ShaderManager::FindFormat(int number, BYTE Mask, D3D11_SIGNATURE_PARAMETER_DESC desc)
{
	DXGI_FORMAT FormatType = DXGI_FORMAT();

	if (number == D3D_REGISTER_COMPONENT_FLOAT32)
	{
		number+1;
		switch (Mask)
		{
		case 0b00000001:
			FormatType = DXGI_FORMAT_R32_FLOAT;
			break;
		case 0b00000011:
			FormatType = DXGI_FORMAT_R32G32_FLOAT;
			break;
		case 0b00000111:
			FormatType = DXGI_FORMAT_R32G32B32_FLOAT;
			break;
		case 0b00001111:
			FormatType = DXGI_FORMAT_R32G32B32A32_FLOAT;
			break;
		default:
			break;
		}
	}
	else if (number == D3D_REGISTER_COMPONENT_UINT32)
	{
		switch (Mask)
		{
		case 0b00000001:
			FormatType = DXGI_FORMAT_R32_UINT;
			break;
		case 0b00000011:
			FormatType = DXGI_FORMAT_R32G32_UINT;
			break;
		case 0b00000111:
			FormatType = DXGI_FORMAT_R32G32B32_UINT;
			break;
		case 0b00001111:
			FormatType = DXGI_FORMAT_R32G32B32A32_UINT;
			break;
		default:
			break;
		}
	}
	else if (number == D3D_REGISTER_COMPONENT_SINT32)
	{
		switch (Mask)
		{
		case 0b00000001:
			FormatType = DXGI_FORMAT_R32_SINT;
			break;
		case 0b00000011:
			FormatType = DXGI_FORMAT_R32G32_SINT;
			break;
		case 0b00000111:
			FormatType = DXGI_FORMAT_R32G32B32_SINT;
			break;
		case 0b00001111:
			FormatType = DXGI_FORMAT_R32G32B32A32_SINT;
			break;
		default:
			break;
		}
	}
	return FormatType;
}

ShaderData ShaderManager::GetShader(string ShaderName)
{
	//쉐이더의 정보를 넘겨준다
	ShaderData data;
	data.VertexShader	= ShaderList[ShaderName].VertexShader;
	data.PixelShader	= ShaderList[ShaderName].PixelShader;
	data.Layout			= ShaderList[ShaderName].Layout;
	data.SamplerState	= samplerList[ShaderName];

	data.cBufferList	= &CbufferList;
	data.SamplerList	= &samplerList;
	return data;
}

ShaderData ShaderManager::GetContextBufferList()
{
	ShaderData data;
	data.cBufferList = &CbufferList;

	return data;
}

bool ShaderManager::LoadVertexShader(string vsFileName,string ShaderName)
{
	HRESULT result;
	ID3D11VertexShader* m_vertexShader;
	ID3D10Blob* _vertexShaderBuffer;

	//string 형을 LPCTSTR형으로 바꿔줌
	string temp = "";
	temp += ShaderFilePath;
	temp += vsFileName;
	CString lpctpath = temp.c_str();
	LPCTSTR path = lpctpath;
	////

	//플래그 설정
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef DEBUG
	dwShaderFlags |= D3DCOMPILE_DEBUG;
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif 

	//버택스 쉐이더를 로드 한다
	result = D3DCompileFromFile
	(
		path,								//쉐이더 코드가 들어있는 파일이름
		NULL,								//쉐이더 매크로를 정의 사용하지않으면 NULL
		D3D_COMPILE_STANDARD_FILE_INCLUDE,	//기본포함 핸들러에 대한 포인터 (기본값 NULL)
		"main",								//쉐이더 실행이 시작되는 쉐이더 진입점 함수의 이름
		"vs_5_0",							//컴파일할 쉐이더 대상 버전
		dwShaderFlags,						//컴파일 쉐이더 플레그 1
		0,									//컴파일 쉐이더 플레그 2
		&_vertexShaderBuffer,				//반환값
		NULL								//에러메세지 반환값
	);
	//실패하면 리턴..
	if (FAILED(result))
	{
		return false;
	}

	//링커를 생성
	ID3D11ClassLinkage* g_pPsClassLinkage = nullptr;
	Device->CreateClassLinkage(&g_pPsClassLinkage);
	

	//버퍼로 부터 정점 쉐이더를 생성한다
	result = Device->CreateVertexShader
	(
		_vertexShaderBuffer->GetBufferPointer(), //컴파일된 쉐이더의 대한 포인터
		_vertexShaderBuffer->GetBufferSize(),	//컴파일된 정점쉐이더의 크기
		g_pPsClassLinkage,						//클래스 링크 인터페이스에 대한 포인터
		&m_vertexShader							//ID3D11VertexShader 인터페이스에 대한 포인터 주소
	);
	//실패하면 리턴..
	if (FAILED(result))
	{
		return false;
	}

	HR(D3DReflect
	(
		_vertexShaderBuffer->GetBufferPointer(),
		_vertexShaderBuffer->GetBufferSize(),
		IID_ID3D11ShaderReflection,
		(void**)&ReFlector
	));


	ShaderList[ShaderName].VertexShader = m_vertexShader;

	//인풋레이아웃을 생성한다
	CreateInputLayout(ShaderName,_vertexShaderBuffer);

	//상수버퍼를 생성한다 이미 있는 버퍼라면 생성하지않음..
	CreateConstantBuffer();

	//샘플러를 생성한다 이미있는 샘플러라면 생성하지않는다
	CreateSamPler(ShaderName);

	//파일을 모두 읽었으니 삭제
	ReFlector->Release();
	ReFlector = nullptr;
	return true;
}

bool ShaderManager::LoadPixelShader(string psFileName,string ShaderName)
{
	HRESULT result;
	ID3D11PixelShader*	m_pixelShader		= nullptr;
	ID3D10Blob*			pixelShaderBuffer	= nullptr;

	string temp = "";
	temp += ShaderFilePath;
	temp += psFileName;
	CString lpctpath = temp.c_str();
	LPCTSTR path = lpctpath;

	//플래그 설정
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef DEBUG
	dwShaderFlags |= D3DCOMPILE_DEBUG;
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif 

	result = D3DCompileFromFile
	(
		path,
		NULL,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main",
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

	//링커를 생성
	ID3D11ClassLinkage* g_pPsClassLinkage = nullptr;
	Device->CreateClassLinkage(&g_pPsClassLinkage);



	// 버퍼에서 픽셀 쉐이더를 생성합니다.
	result = Device->CreatePixelShader
	(
		pixelShaderBuffer->GetBufferPointer(),
		pixelShaderBuffer->GetBufferSize(),
		g_pPsClassLinkage,
		&m_pixelShader
	);

	
	HR(D3DReflect
	(
		pixelShaderBuffer->GetBufferPointer(),
		pixelShaderBuffer->GetBufferSize(),
		IID_ID3D11ShaderReflection,
		(void**)&ReFlector
	));
	
	//실패하면 리턴..
	if (FAILED(result))
	{
		return false;
	}

	CreateSamPler(ShaderName);
	CreateConstantBuffer();
	ShaderList[ShaderName].PixelShader = m_pixelShader;


	ReFlector->Release();
	ReFlector = nullptr;
	return true;
}

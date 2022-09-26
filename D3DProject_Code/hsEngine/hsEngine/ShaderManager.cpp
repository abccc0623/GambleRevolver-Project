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


	//�⺻������ �����ϴ� ������Ʈ�鸸 �־���
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

	//���÷� ����
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

	//���ý� ���̴��� �ε��Ѵ� �̋� ��ǲ���̾ƿ� �ε�
	LoadVertexShader(vsFileName, ShaderName);

	//�ȼ� ���̴��� �ε��Ѵ�
	LoadPixelShader(psFileName, ShaderName);

	//���÷��� �������
	//CreateSamPler(ShaderName);

	//������ۿ� ���÷� ����Ʈ�� �׳� �ּҰ��� ������
	Data.cBufferList = &CbufferList;
	Data.SamplerList = &samplerList;

	return true;
}

bool ShaderManager::CreateSamPler(string shaderName)
{
	//�ϴ� ���̴� ����
	D3D11_SHADER_DESC desc;
	HR(ReFlector->GetDesc(&desc));
	
	//���ε��� ���ҽ��� ��
	UINT Count = desc.BoundResources;
	
	for (int i = 0; i < Count; i++)
	{
		D3D11_SHADER_INPUT_BIND_DESC temp;
		ReFlector->GetResourceBindingDesc(i, &temp);
		//���ε��� ���� Ÿ�԰� �̸�
		UINT	type = temp.Type;
		string	name = temp.Name;

		//�̳��� ���÷��̴�
		if (type == D3D_SIT_SAMPLER)
		{
			//���� �����̸��ǻ��÷��� �̹��ִٸ� �׳� �ƹ��͵� ����
			if (samplerList.find(name) != samplerList.end())
			{
				continue;
			}
		
			UINT num = temp.NumSamples;
			temp.uFlags;
			temp.BindPoint;
			temp.BindCount;
		
			
		
			//�ؽ��� ���÷� ���� ����ü ����
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


		
			// �ؽ�ó ���÷� ���¸� ����ϴ�.
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
	//�ϴ� ���̴�����

	//������ Ÿ��
	D3D_SHADER_VARIABLE_TYPE var_type;

	//���̴��� ������ �����´�
	D3D11_SHADER_DESC desc;
	HR(ReFlector->GetDesc(&desc));

	//���̴��� cbuffer�� ��
	UINT CBufferCount = desc.ConstantBuffers;
	//c���۸� ����!!
	//������ ������ŭ cbuffer�� ������
	for (int i = 0; i < CBufferCount; i++)
	{
		ID3D11ShaderReflectionConstantBuffer* Cbuffer = ReFlector->GetConstantBufferByIndex(i);
		D3D11_SHADER_BUFFER_DESC buffer_Desc;
		HR(Cbuffer->GetDesc(&buffer_Desc));
		string name = buffer_Desc.Name;

		//���� �̹̹��ۿ� ���ִ� ������۶��
		if (CbufferList.find(name) != CbufferList.end())
		{
			continue;
		}

		//�� ��ü�� ������
	
		UINT size = buffer_Desc.Size;
		UINT plags = buffer_Desc.uFlags;
		//������ ����
		UINT var = buffer_Desc.Variables;
		

		ID3D11Buffer* m_GlobalBuffer;
		D3D11_BUFFER_DESC matrixBufferDesc;
		matrixBufferDesc.Usage					= D3D11_USAGE_DEFAULT;
		matrixBufferDesc.ByteWidth				= size;
		matrixBufferDesc.BindFlags				= D3D11_BIND_CONSTANT_BUFFER;
		matrixBufferDesc.CPUAccessFlags			= 0;
		matrixBufferDesc.MiscFlags				= 0;
		matrixBufferDesc.StructureByteStride	= 0;

		//���۸� �������
		HR(Device->CreateBuffer(&matrixBufferDesc, NULL, &m_GlobalBuffer));

		//������۸� �־���
		CbufferList.insert({ name ,m_GlobalBuffer });
	}
}

void ShaderManager::CreateInputLayout(string shaderName,ID3D10Blob* vertexBuffer)
{
	//���̴��� ������ �����´�
	D3D11_SHADER_DESC desc;
	HR(ReFlector->GetDesc(&desc));


	//m_Reflector->GetV
	//���̴��� Layout�� ��
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

		//Ÿ���� ����� ���Ѵ� ex)float2������ , float3�� ������
		BYTE numberMask = desc.Mask;

		DXGI_FORMAT FormatNumver = FindFormat(number, numberMask, desc);

		//�Ѱ��� layout ����ü �ϼ�..
		DescList[i].SemanticName = desc.SemanticName;
		DescList[i].SemanticIndex = 0;
		DescList[i].Format = FormatNumver;
		DescList[i].InputSlot = 0;
		DescList[i].AlignedByteOffset = TypeSize;
		DescList[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		DescList[i].InstanceDataStepRate = 0;

		
		//���ڷ����� ���� �˷��ֱ����� ���� �ڷ����� �����ϴºκ��� �־���
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

	//���� �Է� ���̾ƿ��� �����
	result = Device->CreateInputLayout
	(
		&DescList[0],
		inputLayCount,
		vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(),
		&m_layout
	);

	//���� ����
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
	//���̴��� ������ �Ѱ��ش�
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

	//string ���� LPCTSTR������ �ٲ���
	string temp = "";
	temp += ShaderFilePath;
	temp += vsFileName;
	CString lpctpath = temp.c_str();
	LPCTSTR path = lpctpath;
	////

	//�÷��� ����
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef DEBUG
	dwShaderFlags |= D3DCOMPILE_DEBUG;
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif 

	//���ý� ���̴��� �ε� �Ѵ�
	result = D3DCompileFromFile
	(
		path,								//���̴� �ڵ尡 ����ִ� �����̸�
		NULL,								//���̴� ��ũ�θ� ���� ������������� NULL
		D3D_COMPILE_STANDARD_FILE_INCLUDE,	//�⺻���� �ڵ鷯�� ���� ������ (�⺻�� NULL)
		"main",								//���̴� ������ ���۵Ǵ� ���̴� ������ �Լ��� �̸�
		"vs_5_0",							//�������� ���̴� ��� ����
		dwShaderFlags,						//������ ���̴� �÷��� 1
		0,									//������ ���̴� �÷��� 2
		&_vertexShaderBuffer,				//��ȯ��
		NULL								//�����޼��� ��ȯ��
	);
	//�����ϸ� ����..
	if (FAILED(result))
	{
		return false;
	}

	//��Ŀ�� ����
	ID3D11ClassLinkage* g_pPsClassLinkage = nullptr;
	Device->CreateClassLinkage(&g_pPsClassLinkage);
	

	//���۷� ���� ���� ���̴��� �����Ѵ�
	result = Device->CreateVertexShader
	(
		_vertexShaderBuffer->GetBufferPointer(), //�����ϵ� ���̴��� ���� ������
		_vertexShaderBuffer->GetBufferSize(),	//�����ϵ� �������̴��� ũ��
		g_pPsClassLinkage,						//Ŭ���� ��ũ �������̽��� ���� ������
		&m_vertexShader							//ID3D11VertexShader �������̽��� ���� ������ �ּ�
	);
	//�����ϸ� ����..
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

	//��ǲ���̾ƿ��� �����Ѵ�
	CreateInputLayout(ShaderName,_vertexShaderBuffer);

	//������۸� �����Ѵ� �̹� �ִ� ���۶�� ������������..
	CreateConstantBuffer();

	//���÷��� �����Ѵ� �̹��ִ� ���÷���� ���������ʴ´�
	CreateSamPler(ShaderName);

	//������ ��� �о����� ����
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

	//�÷��� ����
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
	//�����ϸ� ����..
	if (FAILED(result))
	{
		return false;
	}

	//��Ŀ�� ����
	ID3D11ClassLinkage* g_pPsClassLinkage = nullptr;
	Device->CreateClassLinkage(&g_pPsClassLinkage);



	// ���ۿ��� �ȼ� ���̴��� �����մϴ�.
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
	
	//�����ϸ� ����..
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

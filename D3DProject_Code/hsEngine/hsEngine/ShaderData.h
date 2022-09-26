#pragma once

/// <summary>
/// ��ü���� �� ������ ����
/// </summary>
struct ShaderData
{
	ShaderData()
	{
		VertexShader	= nullptr;
		PixelShader		= nullptr;
		Layout			= nullptr;
		SamplerState	= nullptr;
		cBufferList		= nullptr;
		SamplerList		= nullptr;
	}

	~ShaderData()
	{
		VertexShader	= nullptr;
		PixelShader		= nullptr;
		Layout			= nullptr;
		SamplerState	= nullptr;
		cBufferList		= nullptr;
		SamplerList		= nullptr;
	}

	ID3D11VertexShader* VertexShader;
	ID3D11PixelShader*	PixelShader;
	ID3D11InputLayout*	Layout;
	ID3D11SamplerState* SamplerState;

	//������ۿ� ���÷��� �����ϱ⶧���� ����Ʈ�� �ּҰ�������
	map<string, ID3D11Buffer*>*			cBufferList;
	map<string, ID3D11SamplerState*>*	SamplerList;
};


//�������
struct cBuffer
{
	cBuffer()
	{
		BindPoint	= -1;
		Buffer		= nullptr;
	}
	~cBuffer()
	{
		BindPoint	= -1;
		Buffer		= nullptr;
	}
	UINT BindPoint;
	ID3D11Buffer* Buffer;
	string Name;
};



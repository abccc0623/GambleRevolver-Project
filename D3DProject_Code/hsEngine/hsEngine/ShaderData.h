#pragma once

/// <summary>
/// 객체에게 줄 정보들 모음
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

	//상수버퍼와 샘플러는 공유하기때문에 리스트의 주소값을전달
	map<string, ID3D11Buffer*>*			cBufferList;
	map<string, ID3D11SamplerState*>*	SamplerList;
};


//상수버퍼
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



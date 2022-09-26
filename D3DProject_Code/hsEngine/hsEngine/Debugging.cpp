#include "EnginePCH.h"
#include "Debugging.h"

Debugging::Debugging(float x, float y, float z)
{
	m_ENGINE	= nullptr;
	m_Device	= nullptr;
	m_Context	= nullptr;
}

Debugging::~Debugging()
{
	vertices.clear();
	indices.clear();

	m_ENGINE	= nullptr;
	m_Device	= nullptr;
	m_Context	= nullptr;
}

void Debugging::init(hsEngine* Data)
{
	m_ENGINE	= Data;
	m_Device	= m_ENGINE->GetDeviceData()->m_device;
	m_Context	= m_ENGINE->GetDeviceData()->m_deviceContext;

	DebugObjbuffer = (*m_ShaderData.cBufferList)["ObjectBuffer"];
}

void Debugging::Update(XMMATRIX World)
{
	///쉐이더 데이터
	UINT stride = sizeof(Debug32);
	UINT offset = 0;

	ObjectBuffer dataPtr;
	dataPtr.world			= XMMatrixTranspose(World);
	dataPtr.TexMatrix		= XMMatrixTranspose(XMMatrixIdentity());
	dataPtr.Texture_Mask	= 0x00000000;

	//리소스를 복사
	m_Context->UpdateSubresource(DebugObjbuffer, 0, nullptr, &dataPtr, 0, 0);

	// 마지막으로 정점 셰이더의 상수 버퍼를 바뀐 값으로 바꿉니다.
	m_Context->VSSetConstantBuffers(1, 1, &DebugObjbuffer);

	m_Context->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
	m_Context->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);
	m_Context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	
	m_Context->RSSetState(m_ENGINE->GetDeviceData()->mWireframeRS);
	m_Context->IASetInputLayout(m_ShaderData.Layout);

	// 삼각형을 그릴 정점 셰이더와 픽셀 셰이더를 설정합니다.
	m_Context->VSSetShader(m_ShaderData.VertexShader, NULL, 0);
	m_Context->PSSetShader(m_ShaderData.PixelShader, NULL, 0);

	// 픽셀 쉐이더에서 샘플러 상태를 설정합니다.
	m_Context->PSSetSamplers(0, 1, &(m_ShaderData.SamplerState));

	// 삼각형을 그립니다.
	m_Context->DrawIndexed(icount, 0, 0);
}


void Debugging::Create_Box_Buffers(XMFLOAT3 _Centor, XMFLOAT3 _Extents , XMFLOAT4 Color)
{
	//기존에 있던 버택스에서 박스 버텍스를 추가해줌


	Debug32 temp[8];
	temp[0].Pos = { _Extents.x+_Centor.x,_Extents.y+_Centor.y,-_Extents.z+ _Centor.z };
	temp[0].Color = Color;

	temp[1].Pos = { -_Extents.x + _Centor.x, _Extents.y + _Centor.y,-_Extents.z + _Centor.z};
	temp[1].Color = Color;


	temp[2].Pos = { _Extents.x + _Centor.x,-_Extents.y + _Centor.y,-_Extents.z + _Centor.z };
	temp[2].Color = Color;									

	temp[3].Pos = { -_Extents.x + _Centor.x,-_Extents.y + _Centor.y,-_Extents.z + _Centor.z };
	temp[3].Color = Color;


	temp[4].Pos = { _Extents.x + _Centor.x, _Extents.y + _Centor.y,_Extents.z + _Centor.z};
	temp[4].Color = Color;									

	temp[5].Pos = { -_Extents.x + _Centor.x, _Extents.y + _Centor.y,_Extents.z + _Centor.z };
	temp[5].Color = Color;

	temp[6].Pos = { _Extents.x + _Centor.x,-_Extents.y + _Centor.y,_Extents.z + _Centor.z };
	temp[6].Color = Color;									


	temp[7].Pos = { -_Extents.x + _Centor.x,-_Extents.y + _Centor.y,_Extents.z + _Centor.z };
	temp[7].Color = Color;

	for (int i= 0; i < 8; i++)
	{
		vertices.push_back(temp[i]);
	}

	int inSize = indices.size();

	UINT in_temp[24];
	in_temp[0] = 0 + inSize;		in_temp[1] = 1 + inSize;
	in_temp[2] = 2 + inSize;		in_temp[3] = 3 + inSize;
	in_temp[4] = 0 + inSize;		in_temp[5] = 2 + inSize;
	in_temp[6] = 1 + inSize;		in_temp[7] = 3 + inSize;
	in_temp[8] = 4 + inSize;		in_temp[9] = 5 + inSize;
	in_temp[10] = 6 + inSize;		in_temp[11] = 7 + inSize;
	in_temp[12] = 4 + inSize;		in_temp[13] = 6 + inSize;
	in_temp[14] = 5 + inSize;		in_temp[15] = 7 + inSize;
	in_temp[16] = 0 + inSize;		in_temp[17] = 4 + inSize;
	in_temp[18] = 1 + inSize;		in_temp[19] = 5 + inSize;
	in_temp[20] = 2 + inSize;		in_temp[21] = 6 + inSize;
	in_temp[22] = 3 + inSize;		in_temp[23] = 7 + inSize;


	for (int i= 0; i < 24; i++)
	{
		indices.push_back(in_temp[i]);
	}
}

void Debugging::Create_2D_Y_Circle_Buffers(XMFLOAT3 _Centor, float Radius, XMFLOAT4 Color)
{

	int verCount = 180;
	int inCount = verCount * 2;
	int inSize = vertices.size();

	Debug32 temp00[180];
	UINT in_temp00[360];
	for (int i = 0; i < verCount; i++)
	{
		float angle = i * 2;
		float x = Radius * cos(angle * 3.14f / 180);
		float z = Radius * -sin(angle * 3.14f / 180);
		float y = 0;

		temp00[i].Pos = { _Centor.x + x,_Centor.y + y,_Centor.z + z };
		temp00[i].Color = Color;
	}
	for (int i = 0; i < verCount; i++)
	{
		vertices.push_back(temp00[i]);
	}

	int num = 1;
	for (int i = 1; i < inCount; i += 2)
	{
		in_temp00[i] = num + inSize;
		in_temp00[i + 1] = num + inSize;
		num++;
	}
	in_temp00[0] = 0 + inSize;
	in_temp00[inCount - 1] = 0 + inSize;
	for (int i = 0; i < inCount; i++)
	{
		indices.push_back(in_temp00[i]);
	}

}

void Debugging::Create_2D_X_Circle_Buffers(XMFLOAT3 _Centor, float Radius, XMFLOAT4 Color)
{
	int verCount = 180;
	int inCount = verCount * 2;


	Debug32 temp[180];
	UINT in_temp[360];

	int inSize = vertices.size();

	for (int i = 0; i < verCount; i++)
	{
		float angle = i * 2;
		float x = Radius * cos(angle * 3.14f / 180);
		float y = Radius * -sin(angle * 3.14f / 180);
		float z = 0;

		temp[i].Pos = { _Centor.x + x,_Centor.y + y,_Centor.z + z };
		temp[i].Color = Color;
	}
	for (int i = 0; i < verCount; i++)
	{
		vertices.push_back(temp[i]);
	}

	int num = 1;
	for (int i = 1; i < inCount; i += 2)
	{
		in_temp[i] = num + inSize;
		in_temp[i + 1] = num + inSize;
		num++;
	}
	in_temp[0] = 0 + inSize;
	in_temp[inCount - 1] = 0 + inSize;
	for (int i = 0; i < inCount; i++)
	{
		indices.push_back(in_temp[i]);
	}
	
}

void Debugging::Create_2D_Z_Circle_Buffers(XMFLOAT3 _Centor, float Radius, XMFLOAT4 Color)
{
	int verCount = 180;
	int inCount = verCount * 2;
	int inSize = vertices.size();

	Debug32 temp00[180];
	UINT in_temp00[360];
	for (int i = 0; i < verCount; i++)
	{
		float angle = i * 2;
		float y = Radius * cos(angle * 3.14f / 180);
		float z = Radius * -sin(angle * 3.14f / 180);
		float x = 0;

		temp00[i].Pos = { _Centor.x + x,_Centor.y + y,_Centor.z + z };
		temp00[i].Color = Color;
	}
	for (int i = 0; i < verCount; i++)
	{
		vertices.push_back(temp00[i]);
	}

	int num = 1;
	for (int i = 1; i < inCount; i += 2)
	{
		in_temp00[i] = num + inSize;
		in_temp00[i + 1] = num + inSize;
		num++;
	}
	in_temp00[0] = 0 + inSize;
	in_temp00[inCount - 1] = 0 + inSize;
	for (int i = 0; i < inCount; i++)
	{
		indices.push_back(in_temp00[i]);
	}
}

void Debugging::Create_2D_X_Rect_Buffer(XMFLOAT3 _Centor, XMFLOAT2 _Extents, XMFLOAT4 Color)
{

	Debug32 temp[4];
	temp[0].Pos = { _Extents.x + _Centor.x,_Extents.y + _Centor.y,_Centor.z };
	temp[0].Color = Color;

	temp[1].Pos = { -_Extents.x + _Centor.x, _Extents.y + _Centor.y,_Centor.z };
	temp[1].Color = Color;


	temp[2].Pos = { _Extents.x + _Centor.x,-_Extents.y + _Centor.y, _Centor.z };
	temp[2].Color = Color;

	temp[3].Pos = { -_Extents.x + _Centor.x,-_Extents.y + _Centor.y, _Centor.z };
	temp[3].Color = Color;


	for (int i = 0; i < 4; i++)
	{
		vertices.push_back(temp[i]);
	}

	int inSize = indices.size();
	UINT in_temp[8];
	in_temp[0] = 0 + inSize;		in_temp[1] = 1 + inSize;
	in_temp[2] = 2 + inSize;		in_temp[3] = 3 + inSize;
	in_temp[4] = 0 + inSize;		in_temp[5] = 2 + inSize;
	in_temp[6] = 1 + inSize;		in_temp[7] = 3 + inSize;

	for (int i = 0; i < 8; i++)
	{
		indices.push_back(in_temp[i]);
	}
}

void Debugging::Create_2D_Y_Rect_Buffer(XMFLOAT3 _Center, XMFLOAT2 _Extents, XMFLOAT4 Color)
{
	Create_Line_Buffers(XMFLOAT3(_Center.x + _Extents.x, _Center.y, _Center.z + _Extents.y), XMFLOAT3(_Center.x - _Extents.x, _Center.y, _Center.z + _Extents.y), Color);
	Create_Line_Buffers(XMFLOAT3(_Center.x + _Extents.x, _Center.y, _Center.z - _Extents.y), XMFLOAT3(_Center.x - _Extents.x, _Center.y, _Center.z - _Extents.y), Color);

	Create_Line_Buffers(XMFLOAT3(_Center.x + _Extents.x, _Center.y, _Center.z + _Extents.y), XMFLOAT3(_Center.x + _Extents.x, _Center.y, _Center.z - _Extents.y), Color);
	Create_Line_Buffers(XMFLOAT3(_Center.x - _Extents.x, _Center.y, _Center.z - _Extents.y), XMFLOAT3(_Center.x - _Extents.x, _Center.y, _Center.z + _Extents.y), Color);
}

void Debugging::Create_2D_Z_Rect_Buffer(XMFLOAT3 _Center, XMFLOAT2 _Extents, XMFLOAT4 Color)
{
	Create_Line_Buffers(XMFLOAT3(_Center.x, _Center.y + _Extents.x, _Center.z + _Extents.y), XMFLOAT3(_Center.x, _Center.y + _Extents.x, _Center.z - _Extents.y), Color);
	Create_Line_Buffers(XMFLOAT3(_Center.x, _Center.y - _Extents.x, _Center.z + _Extents.y), XMFLOAT3(_Center.x, _Center.y - _Extents.x, _Center.z - _Extents.y), Color);
	
	Create_Line_Buffers(XMFLOAT3(_Center.x, _Center.y + _Extents.x, _Center.z - _Extents.y), XMFLOAT3(_Center.x, _Center.y - _Extents.x, _Center.z - _Extents.y), Color);
	Create_Line_Buffers(XMFLOAT3(_Center.x, _Center.y + _Extents.x, _Center.z + _Extents.y), XMFLOAT3(_Center.x, _Center.y - _Extents.x, _Center.z + _Extents.y), Color);
}

void Debugging::Create_Circle_Buffers(XMFLOAT3 _Center, float Radius, XMFLOAT4 Color)
{
	Create_2D_Y_Circle_Buffers(_Center, Radius, Color);
	Create_2D_X_Circle_Buffers(_Center, Radius, Color);
	Create_2D_Z_Circle_Buffers(_Center, Radius, Color);
}

void Debugging::Create_Line_Buffers(XMFLOAT3 start, XMFLOAT3 end, XMFLOAT4 Color)
{
	//기존에 있던 버택스에서 라인 버텍스를 추가해줌

	int inSize = indices.size();
	int vsize = vertices.size();

	Debug32 temp[2];
	temp[0].Pos = start;			
	temp[0].Color = Color;
	temp[1].Pos = end;
	temp[1].Color = Color;


	for (int i = 0; i < 2; i++)
	{
		vertices.push_back(temp[i]);
	}

	UINT in_temp[2];
	in_temp[0] = 0 + vsize;
	in_temp[1] = 1 + vsize;
	
	for (int i = 0; i < 2; i++)
	{
		indices.push_back(in_temp[i]);
	}

}

void Debugging::CreateDebugBuffers()
{
	vcount = vertices.size();
	icount = indices.size();
	
	
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Debug32) * (vcount);
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices[0];
	HR(m_Device->CreateBuffer(&vbd, &vinitData, &mVB));
	
	
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * icount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];
	HR(m_Device->CreateBuffer(&ibd, &iinitData, &mIB));
}

void Debugging::SetDebugShader(ShaderData data)
{
	m_ShaderData.VertexShader		= data.VertexShader;
	m_ShaderData.PixelShader		= data.PixelShader;
	m_ShaderData.Layout				= data.Layout;
	m_ShaderData.SamplerState		= data.SamplerState;

	//상수버퍼리스트
	m_ShaderData.cBufferList		= data.cBufferList;
	//샘플러 리스트
	m_ShaderData.SamplerList		= data.SamplerList;
}



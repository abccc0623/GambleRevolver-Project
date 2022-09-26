#pragma once
/// <summary>
/// 하나의 매쉬를 랜더링해줄 클래스
/// 이클래스를 상속받았다는건 랜더링할 오브젝트라는것
/// </summary>
class Rendering
{
public:
	Rendering();
	~Rendering();

	void init(hsEngine* _ENGINE);

	//인덱스 버퍼와 버택스버퍼를 생성
	template<typename T1, typename T2>
	void Create_IB_VB_Buffer(std::vector<T1>& t1, std::vector<T2>& t2);


	//랜더링을 한다
	void MeshRendering(bool DebugMode = false);

	//많이 사용되는 파라미터만 정리
	void Set_CameraBuffer_Parameter(ID3D11Buffer* buffer, XMFLOAT3 Pos, XMMATRIX m_View, XMMATRIX m_Proj);
	void Set_ObjectBuffer_Parameter(ID3D11Buffer* buffer, XMMATRIX m_World, XMFLOAT3 TexOffet);
	void Set_SkinningBuffer_Parameter(ID3D11Buffer* buffer, XMMATRIX m_World,std::vector<DirectX::XMMATRIX>* boneOffset);
	void Set_UIBuffer_Parameter(ID3D11Buffer* buffer, XMMATRIX m_World, XMFLOAT3 TexOffet, float Alpha);
	void Set_ParticleBuffer_Parameter(ID3D11Buffer* buffer, XMMATRIX m_World, XMFLOAT3 TexOffet, XMFLOAT4 Color);

	template<typename T>
	static void Set_UpdateBuffer_Paramater(ID3D11DeviceContext* Context, string ContextBufferName, T UpDatebuffer, UINT bufferNumber);

	//버퍼만 업데이트 하고싶을때..
	void SetContextBufferList(ShaderData data);

	//쉐이더를 넣어준다
	virtual void SetShaderData(ShaderData data);
	///주의※
	/// <summary>
	/// 쉐이더는 사용하고있는 cbuffer만 바인드되어서 읽어온다 
	/// 즉 사용하지 않는버퍼들은 가져올수 없다는것
	/// 만약 이함수를 써서 버퍼를가져오려고 했을때 제대로 가져오지못한다면 
	/// 쉐이더쪽에서 가져올 상수버퍼를 사용하고있는지 확인하자
	/// </summary>
	/// <param name="BufferName">상수 버퍼의 이름</param>
	/// <returns></returns>
	static ID3D11Buffer* GetBuffer(string BufferName);

	float Alpha;
private:
	hsEngine*				m_ENGINE;			// 엔진
	ID3D11Device*			m_Device;			// D3D11 디바이스
	ID3D11DeviceContext*	m_Context;			// 디바이스 컨텍스트

	ID3D11Buffer* mVB;	//버텍스 버퍼
	ID3D11Buffer* mIB;	//인덱스 버퍼
	

	///쉐이더 데이터
	UINT stride;
	UINT offset;

	int vcount;
	int icount;


	//텍스쳐가 있는지없는지 여부
	UINT Texture_Mask			= 0x00000000;
	const UINT TEXTURE_MAIN		= 0x00000001;
	const UINT TEXTURE_SUB		= 0x00000010;
	const UINT TEXTURE_NOMAL	= 0x00000100;

protected:
	ID3D11VertexShader*		VertexShader;	//버택스 쉐이더
	ID3D11PixelShader*		PixelShader;	//픽셀 쉐이더
	ID3D11InputLayout*		Layout;			//인풋레이아웃
	ID3D11SamplerState*		SamplerState;	//셈플 스테이트

	static map<string, ID3D11Buffer*>*	cBufferList;
	map<string, ID3D11SamplerState*>*	SamplerList;

	//메인 텍스쳐
	ID3D11ShaderResourceView* mMainSRV;
	//노말 텍스쳐
	ID3D11ShaderResourceView* mNomalSRV;
	//서브 텍스쳐
	ID3D11ShaderResourceView* mSubSRV;


};

template<typename T1, typename T2>
inline void Rendering::Create_IB_VB_Buffer(std::vector<T1>& t1,std::vector<T2>& t2)
{
	vcount = t1.size();
	icount = t2.size();

	//버택스버퍼 생성
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(T1) * vcount;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &t1[0];
	HR(m_Device->CreateBuffer(&vbd, &vinitData, &mVB));


	//인덱스 버퍼 생성
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(T2) * icount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &t2[0];
	HR(m_Device->CreateBuffer(&ibd, &iinitData, &mIB));

	stride = sizeof(T1);
}

template<typename T>
inline void Rendering::Set_UpdateBuffer_Paramater(ID3D11DeviceContext* Context,string ContextBufferName, T UpDatebuffer,UINT bufferNumber)
{
	ID3D11Buffer* cBuffer = GetBuffer(ContextBufferName);
	if (cBuffer == nullptr)
	{
		return; 
	}
	Context->UpdateSubresource(&cBuffer, 0, nullptr, &UpDatebuffer, 0, 0);
	m_Context->VSSetConstantBuffers(bufferNumber, 1, &cBuffer);
	m_Context->VSSetConstantBuffers(bufferNumber, 1, &cBuffer);
}



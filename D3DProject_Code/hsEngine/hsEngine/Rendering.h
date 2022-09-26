#pragma once
/// <summary>
/// �ϳ��� �Ž��� ���������� Ŭ����
/// ��Ŭ������ ��ӹ޾Ҵٴ°� �������� ������Ʈ��°�
/// </summary>
class Rendering
{
public:
	Rendering();
	~Rendering();

	void init(hsEngine* _ENGINE);

	//�ε��� ���ۿ� ���ý����۸� ����
	template<typename T1, typename T2>
	void Create_IB_VB_Buffer(std::vector<T1>& t1, std::vector<T2>& t2);


	//�������� �Ѵ�
	void MeshRendering(bool DebugMode = false);

	//���� ���Ǵ� �Ķ���͸� ����
	void Set_CameraBuffer_Parameter(ID3D11Buffer* buffer, XMFLOAT3 Pos, XMMATRIX m_View, XMMATRIX m_Proj);
	void Set_ObjectBuffer_Parameter(ID3D11Buffer* buffer, XMMATRIX m_World, XMFLOAT3 TexOffet);
	void Set_SkinningBuffer_Parameter(ID3D11Buffer* buffer, XMMATRIX m_World,std::vector<DirectX::XMMATRIX>* boneOffset);
	void Set_UIBuffer_Parameter(ID3D11Buffer* buffer, XMMATRIX m_World, XMFLOAT3 TexOffet, float Alpha);
	void Set_ParticleBuffer_Parameter(ID3D11Buffer* buffer, XMMATRIX m_World, XMFLOAT3 TexOffet, XMFLOAT4 Color);

	template<typename T>
	static void Set_UpdateBuffer_Paramater(ID3D11DeviceContext* Context, string ContextBufferName, T UpDatebuffer, UINT bufferNumber);

	//���۸� ������Ʈ �ϰ������..
	void SetContextBufferList(ShaderData data);

	//���̴��� �־��ش�
	virtual void SetShaderData(ShaderData data);
	///���ǡ�
	/// <summary>
	/// ���̴��� ����ϰ��ִ� cbuffer�� ���ε�Ǿ �о�´� 
	/// �� ������� �ʴ¹��۵��� �����ü� ���ٴ°�
	/// ���� ���Լ��� �Ἥ ���۸����������� ������ ����� �����������Ѵٸ� 
	/// ���̴��ʿ��� ������ ������۸� ����ϰ��ִ��� Ȯ������
	/// </summary>
	/// <param name="BufferName">��� ������ �̸�</param>
	/// <returns></returns>
	static ID3D11Buffer* GetBuffer(string BufferName);

	float Alpha;
private:
	hsEngine*				m_ENGINE;			// ����
	ID3D11Device*			m_Device;			// D3D11 ����̽�
	ID3D11DeviceContext*	m_Context;			// ����̽� ���ؽ�Ʈ

	ID3D11Buffer* mVB;	//���ؽ� ����
	ID3D11Buffer* mIB;	//�ε��� ����
	

	///���̴� ������
	UINT stride;
	UINT offset;

	int vcount;
	int icount;


	//�ؽ��İ� �ִ��������� ����
	UINT Texture_Mask			= 0x00000000;
	const UINT TEXTURE_MAIN		= 0x00000001;
	const UINT TEXTURE_SUB		= 0x00000010;
	const UINT TEXTURE_NOMAL	= 0x00000100;

protected:
	ID3D11VertexShader*		VertexShader;	//���ý� ���̴�
	ID3D11PixelShader*		PixelShader;	//�ȼ� ���̴�
	ID3D11InputLayout*		Layout;			//��ǲ���̾ƿ�
	ID3D11SamplerState*		SamplerState;	//���� ������Ʈ

	static map<string, ID3D11Buffer*>*	cBufferList;
	map<string, ID3D11SamplerState*>*	SamplerList;

	//���� �ؽ���
	ID3D11ShaderResourceView* mMainSRV;
	//�븻 �ؽ���
	ID3D11ShaderResourceView* mNomalSRV;
	//���� �ؽ���
	ID3D11ShaderResourceView* mSubSRV;


};

template<typename T1, typename T2>
inline void Rendering::Create_IB_VB_Buffer(std::vector<T1>& t1,std::vector<T2>& t2)
{
	vcount = t1.size();
	icount = t2.size();

	//���ý����� ����
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(T1) * vcount;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &t1[0];
	HR(m_Device->CreateBuffer(&vbd, &vinitData, &mVB));


	//�ε��� ���� ����
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



#pragma once
/// <summary>
/// ���̴��� ������ ���̴� �Ŵ���
/// D3DƩ�丮���� �����Ͽ� �������
/// ���ý� ���̴� �ȼ����̴� ���÷� ��ǲ���̾ƿ��� ���� ���� ���� ��ġ�¹��
/// </summary>

class  ShaderManager
{
public:
	ShaderManager();
	~ShaderManager();

	void init(hsEngine* ENGINE);
	void Delete();


	//���̴� �ε�
	bool LoadShader(string vsFileName,string psFileName, string ShaderName);

	//���̾ƿ��� ������ش�

	//���÷��� ������ش�
	bool CreateSamPler(string shaderName);

	//���̴��� �ҷ��� ��θ� �̸� �����Ѵ�
	void SetShaderFilePath(string path);



	static ID3D11ClassInstance** g_DynamicLinkageArray;
	ShaderData GetShader(string ShaderName);
	ShaderData GetContextBufferList();
private:
	//�ε� ���ý� ���̴�
	bool LoadVertexShader(string vsFileName, string ShaderName);
	//�ε� �ȼ� ���̴�
	bool LoadPixelShader(string psFileName, string ShaderName);




	/// <summary>
	/// ��ǲ���̾ƿ��� �����
	/// </summary>
	//���̴����� ��ǲ���̾ƿ� ������ �о�´�
	void CreateInputLayout(string shaderName,ID3D10Blob* vertexBuffer);

	//���̾ƿ��� ���鶧 �ʿ��� ���������� ã�´�
	DXGI_FORMAT FindFormat(int number, BYTE size, D3D11_SIGNATURE_PARAMETER_DESC desc);



	/// <summary>
	/// ������۸� �����
	/// </summary>
	void CreateConstantBuffer();

private:
	//���� ������
	hsEngine*				ENGINE;		// ����
	ID3D11Device*			Device;		// D3D11 ����̽�
	ID3D11DeviceContext*	mContext;	// ����̽� ���ؽ�Ʈ

	//�ҷ��� ���̴� ������ ���
	string ShaderFilePath;
	///���ý�,�ȼ�,���÷�,���̾ƿ��� �����̸��� map���� ��� ���̴��� ������
	
	//���÷��� ��ü
	ID3D11ShaderReflection* ReFlector;

	//���ø���Ʈ
	map<string, ID3D11SamplerState*> samplerList;

	//��� ���� ����Ʈ
	map<string, ID3D11Buffer*> CbufferList;

	
	map<string, ShaderData> ShaderList;
};


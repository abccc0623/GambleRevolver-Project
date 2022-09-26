#pragma once

class TextureShader
{
public:
	TextureShader();
	~TextureShader();
	void init(hsEngine* Engine);
	bool SetParameters(XMMATRIX worldMatrix, XMMATRIX viewMatrix,XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture);
	void RenderShader(int indexCount);

private:
	bool LoadShader(const WCHAR* vsFileName,const WCHAR *psFileName);

	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	//���� ������
	hsEngine*				ENGINE;		// ����
	ID3D11Device*			Device;		// D3D11 ����̽�
	ID3D11DeviceContext*	mContext;	// ����̽� ���ؽ�Ʈ
private:
	//���̴� ������
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader*	m_pixelShader;
	ID3D11InputLayout*	m_layout;
	ID3D11Buffer*		m_matrixBuffer;
	ID3D11SamplerState* m_sampleState;
};


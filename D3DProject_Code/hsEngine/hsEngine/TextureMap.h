#pragma once
/// <summary>
/// ����Ÿ��, ���ٽ� ,����Ʈ, ���̴����ҽ� ���� �����ϱ����� ���� ��ü
/// </summary>
class TextureMap
{
public:
	TextureMap(hsEngine* _ENGINE);
	~TextureMap();


	//â�̺�������� ������� ���½�Ű�� �ٽø���
	void Reset(int TextureSizeX = 256, int TextureSizeY = 256);

	//�⺻���� ������ ����� ����
	void Create_BasicBuffer(int TextureSizeX = 256,int TextureSizeY = 256);

	//���� ����
	void Create_DepthBuffer(int TextureSizeX = 256,int TextureSizeY = 256);

	//6���� ���� ť��� ����
	void Create_CubeBuffer(int TextureSizeX = 256, int TextureSizeY = 256);

	void Create_MultiRTV(int TextureSizeX = 256, int TextureSizeY = 256);

	//���ҽ��� ����
	void Delete();
	ID3D11RenderTargetView*		mMulti_RTV[3];
	ID3D11ShaderResourceView*	mMulti_SRV[3];
	ID3D11Texture2D* 			mMulti_Texture[3];
public:
	//�б���������
	ID3D11DepthStencilView*		GetDSV();
	ID3D11RenderTargetView*		GetRTV();
	ID3D11ShaderResourceView*	GetSRV();
	D3D11_VIEWPORT				GetVP();
private:
	hsEngine*				ENGINE;
	ID3D11Device*			Device;
	ID3D11DeviceContext*	mContext;

	//���� ���ٽ�
	ID3D11DepthStencilView*		mDSV;
	//���� Ÿ�ٺ�
	ID3D11RenderTargetView*		mRTV;
	//���̴� ���ҽ�
	ID3D11ShaderResourceView*	mSRV;
	//����Ʈ
	D3D11_VIEWPORT				mVP;


	ID3D11DepthStencilState* m_DepthStencilState;
};
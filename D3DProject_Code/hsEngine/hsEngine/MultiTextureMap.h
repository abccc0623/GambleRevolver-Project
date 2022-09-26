#pragma once
/// <summary>
/// �������� ����Ÿ�ٰ� ���ҽ��並 �����ϱ����� ������� Ŭ����
/// </summary>
/// 

//������ �ѻ�����
const int BUFFER_SIZE = 5;
class MultiTextureMap
{
public:
	MultiTextureMap();
	~MultiTextureMap();

	void init(hsEngine* m_ENGINE);
	void Reset(int TextureSizeX, int TextureSizeY);

	//�������Ѵ�
	void Create_BasicBuffer(int TextureSizeX, int TextureSizeY);
	//����
	void Delete();
	
	//�迭 ������
	const int GetArraySize();

	ID3D11DepthStencilView*		GetDSV();
	ID3D11RenderTargetView**	GetRTV();
	ID3D11ShaderResourceView**	GetSRV();
	D3D11_VIEWPORT				GetVP();
private:
	hsEngine*				ENGINE;
	ID3D11Device*			Device;
	ID3D11DeviceContext*	mContext;

	D3D11_VIEWPORT				mVP;
	ID3D11DepthStencilView*		mDSV;
	ID3D11RenderTargetView*		mMulti_RTV[BUFFER_SIZE];
	ID3D11ShaderResourceView*	mMulti_SRV[BUFFER_SIZE];
	ID3D11Texture2D*			mMulti_Texture[BUFFER_SIZE];

};
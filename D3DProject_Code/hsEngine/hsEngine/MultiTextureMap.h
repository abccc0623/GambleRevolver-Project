#pragma once
/// <summary>
/// 여러개의 랜더타겟과 리소스뷰를 관리하기위해 만들어진 클래스
/// </summary>
/// 

//버퍼의 총사이즈
const int BUFFER_SIZE = 5;
class MultiTextureMap
{
public:
	MultiTextureMap();
	~MultiTextureMap();

	void init(hsEngine* m_ENGINE);
	void Reset(int TextureSizeX, int TextureSizeY);

	//생성을한다
	void Create_BasicBuffer(int TextureSizeX, int TextureSizeY);
	//삭제
	void Delete();
	
	//배열 사이즈
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
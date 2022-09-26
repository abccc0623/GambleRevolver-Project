#pragma once
/// <summary>
/// 랜더타겟, 스텐실 ,뷰포트, 쉐이더리소스 등을 관리하기위해 만든 객체
/// </summary>
class TextureMap
{
public:
	TextureMap(hsEngine* _ENGINE);
	~TextureMap();


	//창이변했을경우 모든뷰들을 리셋시키고 다시만듬
	void Reset(int TextureSizeX = 256, int TextureSizeY = 256);

	//기본적인 형태의 뷰들을 생성
	void Create_BasicBuffer(int TextureSizeX = 256,int TextureSizeY = 256);

	//뎁스 버퍼
	void Create_DepthBuffer(int TextureSizeX = 256,int TextureSizeY = 256);

	//6개의 방향 큐브맵 버퍼
	void Create_CubeBuffer(int TextureSizeX = 256, int TextureSizeY = 256);

	void Create_MultiRTV(int TextureSizeX = 256, int TextureSizeY = 256);

	//리소스뷰 삭제
	void Delete();
	ID3D11RenderTargetView*		mMulti_RTV[3];
	ID3D11ShaderResourceView*	mMulti_SRV[3];
	ID3D11Texture2D* 			mMulti_Texture[3];
public:
	//읽기전용으로
	ID3D11DepthStencilView*		GetDSV();
	ID3D11RenderTargetView*		GetRTV();
	ID3D11ShaderResourceView*	GetSRV();
	D3D11_VIEWPORT				GetVP();
private:
	hsEngine*				ENGINE;
	ID3D11Device*			Device;
	ID3D11DeviceContext*	mContext;

	//뎁스 스텐실
	ID3D11DepthStencilView*		mDSV;
	//랜더 타겟뷰
	ID3D11RenderTargetView*		mRTV;
	//쉐이더 리소스
	ID3D11ShaderResourceView*	mSRV;
	//뷰포트
	D3D11_VIEWPORT				mVP;


	ID3D11DepthStencilState* m_DepthStencilState;
};
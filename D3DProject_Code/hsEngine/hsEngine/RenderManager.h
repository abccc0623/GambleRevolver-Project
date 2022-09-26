#pragma once
/// <summary>
/// 후처리의 랜더링을 총괄할 매니저
/// 싱글톤으로 만듬
/// </summary>
class RenderManager
{
	RenderManager();
	static RenderManager* RM;
public:
	static RenderManager* GetRM();
	~RenderManager();

	void init(hsEngine* _ENGINE);
	void PostTextureMapReset(float mClientWidth, float mClientHeight);

public:
	///랜더링 함수들

	// 다이나믹 큐브맵핑 쉐이더 리소스 가져오기 텍스쳐큐브로 된것
	ID3D11ShaderResourceView* DynamicCubeMapRender(hsCamera* Cam[6], std::vector<GameObject*>& objList, XMFLOAT3 Pos);

	//텍스쳐 랜더링
	ID3D11ShaderResourceView* TextureRender(hsCamera* Cam, string TextMapName, std::vector<GameObject*>& objList);

	// 디퍼드를위한 랜더링
	ID3D11ShaderResourceView** DeferredRender(hsCamera* cam, ScreenManager* mScreen);

	// 그림자 맵핑을 위한 쉐도우 랜더링
	ID3D11ShaderResourceView* ShadowMapRender(hsCamera* Cam, ScreenManager* mScreen);

	// UI 랜더링
	ID3D11ShaderResourceView* UIRender(hsCamera* cam, ScreenManager* mScreen);

	// 알파 오브젝트 랜더링
	ID3D11ShaderResourceView* AlphaRender(hsCamera* cam, ScreenManager* mScreen);
public:
	///ON_OFF함수들
	//z 버퍼를 켠다
	void On_ZBuffer();
	void On_UIAlphaBlending();
	void On_AlphaBlending();

	//z 버퍼를 끈다
	void Off_ZBuffer();
	void Off_AlphaBlending();

	//이함수는 엔진안에서 실행될것
	void BeginRenderPost();
	void EndRenderPost();
public:
	///준비 함수들
	// 포스트 프로세싱을 위한 준비작업
	void Post_Processing_Start(hsCamera* _RenderCam, hsCamera* _LookCam, TextureObj* _text);

	//다이나믹 큐브맵핑을 위한 랜더타겟과 뷰포트 스텐실 버퍼를 생성
	void CreateDynamicCubeMapBuffer();

	//텍스쳐 만들어줌
	void CreateTextureMap(string name, TEXTURE_BUFFER_TYPE type, int bufferSizeX = 256, int bufferSizeY = 256);

	//뎁스 스텐실 z버퍼를 켜는 상태를 만들어줌
	void CreateDepthStencilState();

	// 디퍼드 을 위한 준비작업을한다
	void Deferred_Render_Start();

	//포스트프로세싱을 하는중인지 아닌지
	static bool PostProcessing;
private:
	hsEngine* ENGINE;			// 엔진에 필요한것들
	ID3D11Device* Device;			// 디바이스
	ID3D11DeviceContext* mContext;		// 디바이스 컨텍스트


	//다이나믹 큐브맵을 위한 변수들
	ID3D11DepthStencilView* mDynamicCubeDSV;
	ID3D11ShaderResourceView* mDynamicCubeSRV;
	ID3D11RenderTargetView* mDynamicCubeRTV[6];
	D3D11_VIEWPORT				mDynamicCubeVP;

private:
	///각각의 텍스쳐맵
	//포스트 프로세싱 관련 뷰
	TextureMap* mPost;
	TextureMap* mUI;
	TextureMap* mShadow;
	TextureMap* mLight;
	TextureMap* mAlpha;


	//디퍼드를위한 멀티 텍스쳐맵
	MultiTextureMap* mDeferred;

	//다른텍스쳐맵을 관리
	map<string, TextureMap*> TextureList;

	//오브젝트들을 이카메라 기준으로 그릴것이다
	hsCamera* mRenderCamera;

	//그려진오브젝트를 texture로 만들고 그것을 오브젝트 형식으로 만듬
	TextureObj* mRenderTextureObj;

	//텍스쳐오브젝트 를 랜더링할카메라
	hsCamera* mTextureLookCamera;
private:
	///각각의 상태들
	//온오프 상태의 뎁스스텐실 상태를 미리 만들어둠
	ID3D11DepthStencilState* m_DepthStencil_ZBuffer_OFF_State;
	ID3D11DepthStencilState* m_DepthStencil_ZBuffer_ON_State;

	ID3D11BlendState* m_alphaEnableBlending_ON_State;
	ID3D11BlendState* m_alphaEnableBlending_ON_UI_State;
	ID3D11BlendState* m_alphaEnableBlending_OFF_State;
};
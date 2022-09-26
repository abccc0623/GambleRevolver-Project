#pragma once
/// <summary>
/// ��ó���� �������� �Ѱ��� �Ŵ���
/// �̱������� ����
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
	///������ �Լ���

	// ���̳��� ť����� ���̴� ���ҽ� �������� �ؽ���ť��� �Ȱ�
	ID3D11ShaderResourceView* DynamicCubeMapRender(hsCamera* Cam[6], std::vector<GameObject*>& objList, XMFLOAT3 Pos);

	//�ؽ��� ������
	ID3D11ShaderResourceView* TextureRender(hsCamera* Cam, string TextMapName, std::vector<GameObject*>& objList);

	// ���۵带���� ������
	ID3D11ShaderResourceView** DeferredRender(hsCamera* cam, ScreenManager* mScreen);

	// �׸��� ������ ���� ������ ������
	ID3D11ShaderResourceView* ShadowMapRender(hsCamera* Cam, ScreenManager* mScreen);

	// UI ������
	ID3D11ShaderResourceView* UIRender(hsCamera* cam, ScreenManager* mScreen);

	// ���� ������Ʈ ������
	ID3D11ShaderResourceView* AlphaRender(hsCamera* cam, ScreenManager* mScreen);
public:
	///ON_OFF�Լ���
	//z ���۸� �Ҵ�
	void On_ZBuffer();
	void On_UIAlphaBlending();
	void On_AlphaBlending();

	//z ���۸� ����
	void Off_ZBuffer();
	void Off_AlphaBlending();

	//���Լ��� �����ȿ��� ����ɰ�
	void BeginRenderPost();
	void EndRenderPost();
public:
	///�غ� �Լ���
	// ����Ʈ ���μ����� ���� �غ��۾�
	void Post_Processing_Start(hsCamera* _RenderCam, hsCamera* _LookCam, TextureObj* _text);

	//���̳��� ť������� ���� ����Ÿ�ٰ� ����Ʈ ���ٽ� ���۸� ����
	void CreateDynamicCubeMapBuffer();

	//�ؽ��� �������
	void CreateTextureMap(string name, TEXTURE_BUFFER_TYPE type, int bufferSizeX = 256, int bufferSizeY = 256);

	//���� ���ٽ� z���۸� �Ѵ� ���¸� �������
	void CreateDepthStencilState();

	// ���۵� �� ���� �غ��۾����Ѵ�
	void Deferred_Render_Start();

	//����Ʈ���μ����� �ϴ������� �ƴ���
	static bool PostProcessing;
private:
	hsEngine* ENGINE;			// ������ �ʿ��Ѱ͵�
	ID3D11Device* Device;			// ����̽�
	ID3D11DeviceContext* mContext;		// ����̽� ���ؽ�Ʈ


	//���̳��� ť����� ���� ������
	ID3D11DepthStencilView* mDynamicCubeDSV;
	ID3D11ShaderResourceView* mDynamicCubeSRV;
	ID3D11RenderTargetView* mDynamicCubeRTV[6];
	D3D11_VIEWPORT				mDynamicCubeVP;

private:
	///������ �ؽ��ĸ�
	//����Ʈ ���μ��� ���� ��
	TextureMap* mPost;
	TextureMap* mUI;
	TextureMap* mShadow;
	TextureMap* mLight;
	TextureMap* mAlpha;


	//���۵带���� ��Ƽ �ؽ��ĸ�
	MultiTextureMap* mDeferred;

	//�ٸ��ؽ��ĸ��� ����
	map<string, TextureMap*> TextureList;

	//������Ʈ���� ��ī�޶� �������� �׸����̴�
	hsCamera* mRenderCamera;

	//�׷���������Ʈ�� texture�� ����� �װ��� ������Ʈ �������� ����
	TextureObj* mRenderTextureObj;

	//�ؽ��Ŀ�����Ʈ �� ��������ī�޶�
	hsCamera* mTextureLookCamera;
private:
	///������ ���µ�
	//�¿��� ������ �������ٽ� ���¸� �̸� ������
	ID3D11DepthStencilState* m_DepthStencil_ZBuffer_OFF_State;
	ID3D11DepthStencilState* m_DepthStencil_ZBuffer_ON_State;

	ID3D11BlendState* m_alphaEnableBlending_ON_State;
	ID3D11BlendState* m_alphaEnableBlending_ON_UI_State;
	ID3D11BlendState* m_alphaEnableBlending_OFF_State;
};
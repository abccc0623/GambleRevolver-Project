#pragma once
class hsEngine;
class hsBone : public GameObject ,public  Rendering,public Debugging
{
public:
	hsBone();
	~hsBone();

	// �ʱ�ȭ
	virtual void Initialize(hsEngine* ENGINE, LoadData* _LoadData);



	virtual void Update();
	virtual void Render(const hsViewData& view);
	virtual void Delete();
	virtual void Debug();



	ID3D11RasterizerState* m_pRenderstate;			/// �ܺο��� �����ؼ� ������ ��Ȳ�� ���� ������. ���̴����� �ص� �ȴ�.

	bool isParent;
	bool isSkinning;
	bool isAnime;
	bool isBone;



	std::string ParentName;
	std::string MyName;

	hsAnimation* Anime;
	int POS_index = 0;
	int ROT_index = 0;
	bool POS_ANIMATION_END = false;
	bool ROT_ANIMATION_END = false;

	

	bool MeshAnimation(int FPS);
	void SetTextures(ID3D11ShaderResourceView* textures);
	void SetNomalTextures(ID3D11ShaderResourceView* textures);
	void SetBoneSkinningTM(XMMATRIX offset);
	void SetBoneMesh(hsBone* Bone);
	void AnimationReset();
	
	//���ε����� �̸��� ����ִ� ����Ʈ
	std::map<string,int> BoneNumberList;
	//�� �ε���
	std::vector<hsBone*> BoneList;
	//���� ������TM
	std::vector<XMMATRIX> BoneOffsetTM;

	map<string, hsAnimation*> AnimationList;

	int StartFrame	= 0;
	int EndFrame	= 0;
	int FrameSpeed	= 0;
private:
	int IndexCount;
	XMMATRIX XM_POS = XMMatrixIdentity();
	XMMATRIX XM_ROT = XMMatrixIdentity();
	XMMATRIX XM_SCL = XMMatrixIdentity();
protected:
	ID3D11Buffer* m_Skinning;
	XMFLOAT4X4 mTexTransform;

	XMMATRIX worldViewProj;
	Material Mat;

	std::map<std::string, HsMesh*> MeshList;
};


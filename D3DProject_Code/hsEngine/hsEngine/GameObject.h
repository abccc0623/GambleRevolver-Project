#pragma once
///������ �ֻ���������Ʈ�� GameObject���� ������ ���� �������� ������ ����ִµ�
///������ Ŭ������ �ϳ������� ��ӽ�Ű�°� ������..

class GameObject
{
public:
	GameObject();
	virtual~GameObject();

	///��ġ��
	Transform transfrom;

	///�������� �Ұ��ΰ�
	bool isRender;

	//���� ������Ʈ
	void SetRasterizerState(ID3D11RasterizerState* render);

	//�ڱ��ڽ��� mWorld����� ���ϴ� ������Ʈ
	virtual void Update() = 0;

	//ī�޶��� view proj�� �޾ƿ� �׷���
	virtual void Render(const hsViewData& view)= 0;
	//����
	virtual void Delete()	= 0;
	//�����
	virtual void Debug()	= 0;

	
	//���������� ���..
	std::string ParentName;
	std::string MyName;

	//������ �Ұ��� ����
	bool OnRender;
	bool GetDeleteCheck();
	bool isDelete;

	float Alpha;
	void SetParentTR(Transform* tr);
	void UpdateParentTr();
protected:
	hsEngine*				ENGINE;			// ����
	ID3D11Device*			Device;			// D3D11 ����̽�
	ID3D11DeviceContext*	mContext;		// ����̽� ���ؽ�Ʈ

	XMMATRIX mWorld;	//���� ���
	XMMATRIX mView;		//�� ���
	XMMATRIX mProj;		//�������� ���

	//���� ������Ʈ
	ID3D11RasterizerState* m_pRenderstate;

	Transform* ParentTranform = nullptr;
private:
};
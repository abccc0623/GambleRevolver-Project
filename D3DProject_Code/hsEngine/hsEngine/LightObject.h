#pragma once
class LightObject :public GameObject, public Debugging
{
public:
	LightObject(float x = 0, float y = 0, float z = 0);
	~LightObject();
	


	void init(hsEngine* _ENGINE, int type);
	virtual void Render(const hsViewData& view) override;
	virtual void Debug()override;
	virtual void Update()override;
	virtual void Delete()override;


	const int GetType();
public:
	///����Ʈ ����Ʈ
public:
	///�𷺼� ����Ʈ

	//�𷺼� ����Ʈ ����
	void SetDirLight(DirectionalLight DirLight);
	//�����Ʈ ����ŧ�� ��ǻ��
	void SetDirLight(XMFLOAT4 A, XMFLOAT4 D, XMFLOAT4 S);
	//�𷺼� ����Ʈ �÷�
	void SetDirLightColor(XMFLOAT3 color);
	//�𷺼� ����Ʈ ��������
	DirectionalLight* GetDirLight();
public:
	///����Ʈ ����Ʈ

	//����Ʈ����Ʈ ����
	void SetPointLight(hsPointLight Light);
	//�����Ʈ ����ŧ�� ��ǻ��
	void SetPointLight(XMFLOAT4 A, XMFLOAT4 D, XMFLOAT4 S);
	//����Ʈ ����Ʈ ����
	void SetPointLightRange(float _Range);
	//����Ʈ ����Ʈ �÷�
	void SetPointLightColor(XMFLOAT3 color);
	//����Ʈ ����Ʈ ��������
	const hsPointLight* GetPointLight();
private:
	int Myindex = 0;

	//�Ÿ���������� �ϴ� �� �־��

	//����Ʈ ����Ʈ Ÿ�Ա���ü
	hsPointLight PointLight;

	//�𷺼� ����Ʈ Ÿ�� ����ü
	DirectionalLight DirLight;
	//hsSpotLight		SpritLight;

	//����Ʈ�� Ÿ��
	int LightType;

	// GameObject��(��) ���� ��ӵ�
};
#pragma once

///��� ����Ʈ ���� ������ ����Ʈ�Ŵ��� ���۵� ������������ ȭ������ �׷����ؼ� 
///��ü �ε������ۿ� ���ؽ����۸� �����ϰ� �������� ����
class Factory;
class LightObject;
class LightManager : public GameObject,public Rendering
{

public:
	LightManager(float x=0,float y = 0,float z = 0);
	virtual~LightManager();

	// GameObject��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void Render(const hsViewData& view) override;
	virtual void Delete() override;
	virtual void Debug() override;
	void init(hsEngine* ENGINE,Factory* factory);


	void PushLightObject(LightObject* obj);
	
	ID3D11ShaderResourceView* mSRV;
private:
	Factory* mFactory;


	//����Ʈ ����ü
	XMFLOAT4 Ambient;
	XMFLOAT4 Diffuse;
	XMFLOAT4 Specular;
	XMFLOAT3 Att;
	float Range;
	XMFLOAT3 Position;

	//static vector<LightManager> PointLightList;

	//���͸���
	Material Mat;

	ID3D11Buffer* PointLightbuffer;
	ID3D11Buffer* objectbuffer;
	ID3D11Buffer* Dirbuffer;

	
	//����Ʈ ����Ʈ ����Ʈ
	vector<LightObject*>PointLightList;
	//����Ʈ ����Ʈ ����Ʈ
	vector<LightObject*>SpotLightList;
	//�𷺼� ����Ʈ 
	LightObject* DirLight;
};
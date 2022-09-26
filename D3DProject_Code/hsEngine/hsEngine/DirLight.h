#pragma once
class hsDirLight : public GameObject,public Rendering,public Debugging
{
public:
	hsDirLight();
	hsDirLight(float x, float y, float z);
	virtual~hsDirLight();

	// GameObject을(를) 통해 상속됨
	virtual void Update() override;
	virtual void Render(const hsViewData& view) override;
	virtual void Delete() override;
	virtual void Debug() override;


	void init(hsEngine* ENGINE);
	void SetColor(float R, float G, float B);
	void SetRot(float rot);
private:
	ID3D11Buffer* Dirbuffer;

	DirectionalLight* mDirLights;
	Material Mat;
	

	float rot;


	XMFLOAT4 Ambient;
	XMFLOAT4 Diffuse;
	XMFLOAT4 Specular;
	XMFLOAT3 Direction;
};
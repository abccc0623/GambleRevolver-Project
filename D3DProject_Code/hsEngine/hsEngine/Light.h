#pragma once
class Light :public HsMesh,public Rendering,public Debugging
{
public: 
	Light(float x = 0, float y = 0, float z = 0);
	~Light();
	void init(hsEngine* _ENGINE);

	// GameObject��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void Render(const hsViewData& view) override;
	virtual void Delete() override;
	virtual void Debug() override;

	
};
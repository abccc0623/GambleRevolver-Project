
#pragma once
class Debugging;
class hsSpotLight : public GameObject
{

public:
	hsSpotLight(float x = 0, float y = 0, float z = 0);
	virtual~hsSpotLight();

	// GameObject을(를) 통해 상속됨
	virtual void Update() override;
	virtual void Render(const hsViewData& view) override;
	virtual void Delete() override;
	virtual void Debug() override;
	void init(hsEngine* ENGINE);
	void SetColor(float R, float G, float B);
	void SetRange(float _Range);
private:
	SpotLight mSportLight;
	Material Mat;
};
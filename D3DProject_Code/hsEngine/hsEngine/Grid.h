#pragma once

//#include "DX11Define.h"
//#include "./FL/d3dx11Effect.h"	// effect, tech


class Grid : public GameObject , public Debugging
{
public:
	Grid(float x = 0, float y = 0,float z = 0);
	virtual~Grid();
public:
	void init(hsEngine* _ENGINE);
	virtual void Update() override;

	virtual void Delete() override;

	virtual void Debug() override;

	virtual void Render(const hsViewData& view) override;

	void SetOption(float X, float Y, XMFLOAT4 Color);
private:
	float SizeX;
	float SizeY;
	XMFLOAT4 m_Color;
};


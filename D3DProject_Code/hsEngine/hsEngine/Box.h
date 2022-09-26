#pragma once

/// <summary>
/// 기본 도형
/// </summary>
class Box: public HsMesh ,public Collider
{
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};
public:
	Box(float X = 0, float Y = 0, float Z = 0);
	virtual ~Box();
public:
	

public:
	virtual void init(hsEngine* _Engine, LoadData* data);
	virtual void Update();
	virtual void Render(const hsViewData& view);
	virtual void Debug();

	ID3D11Buffer* boxbuffer;
private:
	
};


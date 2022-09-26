#pragma once
class HPBox :public HsMesh , public Collider
{
public:
	HPBox(float x = 0, float y = 0, float z = 0);
	virtual ~HPBox();
	virtual void init(hsEngine* ENGINE, LoadData* Data);
	virtual void Debug();
	virtual void Update();
	virtual void Render();

	

};
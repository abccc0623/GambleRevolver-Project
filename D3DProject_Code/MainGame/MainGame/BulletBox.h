#pragma once
class BulletBox :public HsMesh ,public Debugging ,public Collider
{
public:
	BulletBox(float x=0, float y=0, float z=0);
	virtual ~BulletBox();
	virtual void init(hsEngine* ENGINE, LoadData* Data);
	virtual void Debug();
	virtual void Update();
	virtual void Render();
private:

};


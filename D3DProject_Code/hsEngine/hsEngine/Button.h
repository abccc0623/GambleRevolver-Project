#pragma once
class Button:public UI
{
public:
	Button(float x = 0, float y = 0,float z = 0);
	virtual~Button();
	void init(hsEngine* _ENGINE);

	//충돌범위를 지정한다
	void SetRect(float Left, float Top, float Right, float Bottom);
	//충돌 범위를 자동으로 지정한다
	void SetAutoRect();


	virtual void Update();
	virtual void Render();
	virtual void Delete();
private:
	XMFLOAT4 Rect;

};
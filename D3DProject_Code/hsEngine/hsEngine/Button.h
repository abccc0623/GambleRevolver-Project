#pragma once
class Button:public UI
{
public:
	Button(float x = 0, float y = 0,float z = 0);
	virtual~Button();
	void init(hsEngine* _ENGINE);

	//�浹������ �����Ѵ�
	void SetRect(float Left, float Top, float Right, float Bottom);
	//�浹 ������ �ڵ����� �����Ѵ�
	void SetAutoRect();


	virtual void Update();
	virtual void Render();
	virtual void Delete();
private:
	XMFLOAT4 Rect;

};
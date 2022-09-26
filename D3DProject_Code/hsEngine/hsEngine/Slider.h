#pragma once

class Slider:public UI
{
public:
	Slider(float x = 0,float y = 0,float z = 0);
	~Slider();
	
	Transform transform;

	virtual void Update();
	virtual void Render();
	virtual void Delete();


	//ü������ �κ��� �̹���
	void SetFillImg(Image* img);
	//�����̴��� �̹���
	void SetSliderImg(Image* img);
	//�ڵ� �̹����� �־��ش�
	void SetHandImg(Image* img);
	//�����̴��� ������̹���
	void SetBackGroundImg(Image* img);

	//������ �־��ֱ�
	void SetHandleValue(float value);
	float GetHandleValue();
private:
	//�ڿ��׷��� �̹���
	Image* Fill;
	//�տ� ���� �̹���
	Image* Slider_Front;
	//�����̴� �ڿ������̹���
	Image* Slider_Back;
	//������ �κ��̵� �̹���
	Image* Hand;



	XMFLOAT2 SliderStartPos;
	XMFLOAT2 SliderEndPos;

	//�����̴� ä�����ºκп� ��ġ
	float Slider_Fill_Pos;
	//�����̴� ä�����ºκп� ������
	float Slider_Fild_Scl;
	//�����̴� �ڵ� ����
	float Slider_Value;
};
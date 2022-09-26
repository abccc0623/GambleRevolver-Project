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


	//체워지는 부분의 이미지
	void SetFillImg(Image* img);
	//슬라이더의 이미지
	void SetSliderImg(Image* img);
	//핸들 이미지를 넣어준다
	void SetHandImg(Image* img);
	//슬라이더의 가장뒤이미지
	void SetBackGroundImg(Image* img);

	//벨류값 넣어주기
	void SetHandleValue(float value);
	float GetHandleValue();
private:
	//뒤에그려질 이미지
	Image* Fill;
	//앞에 나올 이미지
	Image* Slider_Front;
	//슬라이더 뒤에나올이미지
	Image* Slider_Back;
	//손잡이 부분이될 이미지
	Image* Hand;



	XMFLOAT2 SliderStartPos;
	XMFLOAT2 SliderEndPos;

	//슬라이더 채워지는부분에 위치
	float Slider_Fill_Pos;
	//슬라이더 채워지는부분에 스케일
	float Slider_Fild_Scl;
	//슬라이더 핸들 벨류
	float Slider_Value;
};
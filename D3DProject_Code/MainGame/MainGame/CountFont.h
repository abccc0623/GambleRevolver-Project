#pragma once
//불렛의 총알계수를 보여주기위해 이미지폰트를 만든다 
#include <stack>
class CountFont :public UI
{
public:
	CountFont(float x= 0, float y = 0, float z = 0);
	virtual~CountFont();

	void init(hsEngine* _ENGINE);
	virtual void Update();
	virtual void Render();
	virtual void Delete();
	

	//이미지폰트로 바꿀 숫자를 넣어준다
	void SetCount(int num);
	//이미지를 오브젝트를넣어준다
	void SetImgObj
	(	
		Image* One,
		Image* Two,
		Image* Three,
		Image* Four,
		Image* Five,
		Image* Six,
		Image* Seven,
		Image* Eight,
		Image* Nine,
		Image* Zero
	);

	void SetImg
	(
		ID3D11ShaderResourceView* One,
		ID3D11ShaderResourceView* Two,
		ID3D11ShaderResourceView* Three,
		ID3D11ShaderResourceView* Four,
		ID3D11ShaderResourceView* Five,
		ID3D11ShaderResourceView* Six,
		ID3D11ShaderResourceView* Seven,
		ID3D11ShaderResourceView* Eight,
		ID3D11ShaderResourceView* Nine,
		ID3D11ShaderResourceView* Zero
	);


private:
	int number[10];
	//카운터 이미지들의 리스트
	Image* CountImgList[10];
	ID3D11ShaderResourceView* IMG[10];
	//숫자를 이미지로 바꾼값들

};
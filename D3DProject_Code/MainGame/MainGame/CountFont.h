#pragma once
//�ҷ��� �Ѿ˰���� �����ֱ����� �̹�����Ʈ�� ����� 
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
	

	//�̹�����Ʈ�� �ٲ� ���ڸ� �־��ش�
	void SetCount(int num);
	//�̹����� ������Ʈ���־��ش�
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
	//ī���� �̹������� ����Ʈ
	Image* CountImgList[10];
	ID3D11ShaderResourceView* IMG[10];
	//���ڸ� �̹����� �ٲ۰���

};
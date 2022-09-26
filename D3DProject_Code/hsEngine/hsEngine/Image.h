#pragma once
class Image :public UI
{
public:
	Image(float x = 0, float y = 0, float z = 0);
	virtual~Image();

	
	void init(hsEngine* _ENGINE);
	virtual void Delete();
	virtual void Update();
	bool FullScreen = false;
private:
	//�̹����� �����Ҽ� �ֱ⶧���� ����� �̹����� ��� �޾Ƴ��������ְ�

	map<string,ID3D11ShaderResourceView*> ImgList;
};

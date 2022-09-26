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
	//이미지를 변경할수 있기때문에 사용할 이미지는 모두 받아놓을수도있게

	map<string,ID3D11ShaderResourceView*> ImgList;
};

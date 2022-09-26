#include "EnginePCH.h"
#include "Image.h"

Image::Image(float x, float y, float z)
{
	transform.Position = { x,y,z };
}

Image::~Image()
{


}

void Image::init(hsEngine* _ENGINE)
{
	UI::init(_ENGINE);
}

void Image::Delete()
{
	//이미지들을 모두 놓아준다...
	map<string, ID3D11ShaderResourceView*>::iterator it = ImgList.begin();
	for (it; it != ImgList.end(); it++)
	{
		(*it).second = nullptr;
	}
}

void Image::Update()
{
	if (FullScreen == true) 
	{
		mWorld = transform.GetMaster();
	}
	else
	{
		UI::Update();
	}
}


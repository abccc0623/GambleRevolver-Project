#pragma once
///������ ��ŸƮ ��ũ��
class Title :  public Screen
{
public:
	Title();
	virtual ~Title();


	virtual void init() override;
	virtual void End();
	virtual void Updata();


	int count = 0;
	Image* GameStartImg;
private:
};

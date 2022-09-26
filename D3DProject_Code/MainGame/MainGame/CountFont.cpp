#include "pch.h"
#include "CountFont.h"

CountFont::CountFont(float x, float y, float z)
{
	transform.Position = { x,y,z };
}

CountFont::~CountFont()
{
}

void CountFont::init(hsEngine* _ENGINE)
{
	UI::init(_ENGINE);
	//최대 10번째 자리수까지 지원
}

void CountFont::Update()
{
	XMFLOAT3 Pos = transform.Position;
	XMFLOAT3 Rot = transform.Rotation;
	XMFLOAT3 Scl = transform.Scale;


	for (int i = 0; i < 10; i++)
	{
		if (CountImgList[i] == nullptr) { continue; }

		Pos.x += 40;
		CountImgList[i]->transform.Position = Pos;
		CountImgList[i]->transform.Rotation = Rot;
		CountImgList[i]->transform.Scale = Scl;

		CountImgList[i]->Update();
	}
}

void CountFont::Render()
{
	XMFLOAT3 Pos = transform.Position;
	XMFLOAT3 Rot = transform.Rotation;
	XMFLOAT3 Scl = transform.Scale;


	for(int i = 0; i < 10; i++)
	{
		if (CountImgList[i] == nullptr) { continue; }

		if (number[i] != -1)
		{
			CountImgList[i]->Render();
		}
	}
}

void CountFont::Delete()
{


}

void CountFont::SetCount(int num)
{
	//일단 숫자의자리수를 알아내자..

	
	//숫자를 string 형으로 변경 더쉽게 가기위해
	std::string temp = std::to_string(num);

	//string의 사이즈는 숫자의 자리수
	
	//아스키코드표로 보면 48 = 숫자 0
	char startChar = 48;

	
	//아스키코드의 숫자시작부분이 48
	//나의 숫자에서 시작부분을빼주면
	//그것이 이번자리에 들어가는 숫자인거임
	//ex) 48 - 52(숫자 4) = 4가나온다 

	//숫자의 자리수 만큼 돈다
	

	for (int i = 0; i < 10;i++)
	{
		int index = -1;
		if (i < temp.size())
		{
			index = temp[i] - startChar;
			CountImgList[i]->SetTexture(IMG[index]);
		}
		number[i] = index;
	}



	//이미지 폰트 완성
}

void CountFont::SetImgObj(Image* One, Image* Two, Image* Three, Image* Four, Image* Five, Image* Six, Image* Seven, Image* Eight, Image* Nine, Image* Zero)
{
	//어캐하면 깔끔하게 밀어넣을까..
	//vector<image*> 로받을까 했지만 밖에서 리스트를 또만들고
	//넣어주는작업을해야하는데 그게더귀찮아보임

	
	//이미지들을 맞게 넣어준다
	CountImgList[0] = Zero;
	CountImgList[1] = One;
	CountImgList[2] = Two;
	CountImgList[3] = Three;
	CountImgList[4] = Four;
	CountImgList[5] = Five;
	CountImgList[6] = Six;
	CountImgList[7] = Seven;
	CountImgList[8] = Eight;
	CountImgList[9] = Nine;
}

void CountFont::SetImg
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
)
{
	IMG[0] = Zero;
	IMG[1] = One;
	IMG[2] = Two;
	IMG[3] = Three;
	IMG[4] = Four;
	IMG[5] = Five;
	IMG[6] = Six;
	IMG[7] = Seven;
	IMG[8] = Eight;
	IMG[9] = Nine;
}

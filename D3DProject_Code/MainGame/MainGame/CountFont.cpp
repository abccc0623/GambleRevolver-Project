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
	//�ִ� 10��° �ڸ������� ����
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
	//�ϴ� �������ڸ����� �˾Ƴ���..

	
	//���ڸ� string ������ ���� ������ ��������
	std::string temp = std::to_string(num);

	//string�� ������� ������ �ڸ���
	
	//�ƽ�Ű�ڵ�ǥ�� ���� 48 = ���� 0
	char startChar = 48;

	
	//�ƽ�Ű�ڵ��� ���ڽ��ۺκ��� 48
	//���� ���ڿ��� ���ۺκ������ָ�
	//�װ��� �̹��ڸ��� ���� �����ΰ���
	//ex) 48 - 52(���� 4) = 4�����´� 

	//������ �ڸ��� ��ŭ ����
	

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



	//�̹��� ��Ʈ �ϼ�
}

void CountFont::SetImgObj(Image* One, Image* Two, Image* Three, Image* Four, Image* Five, Image* Six, Image* Seven, Image* Eight, Image* Nine, Image* Zero)
{
	//��ĳ�ϸ� ����ϰ� �о������..
	//vector<image*> �ι����� ������ �ۿ��� ����Ʈ�� �Ǹ����
	//�־��ִ��۾����ؾ��ϴµ� �װԴ������ƺ���

	
	//�̹������� �°� �־��ش�
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

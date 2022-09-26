#pragma once
enum class LOAD_TYPE
{
	TEXTURES,
	TEXTMESH,
	ANIMATION,
	NOMAL,
	FBX,
	ASE,
};

enum class LAYOUT_TYPE
{
	BASIC,
	SKINNING,
	LIGHT,
	hsDEBUG,
	TEXTURE
};

enum class POST_TYPE
{
	BASIC			=0,
	BLUR_Gaussian	=1,
	BLUR_Radial		=2,
	SHADOW			=3,
};

enum class TEXTURE_BUFFER_TYPE
{
	Basic,
	Depth,
	CubeMap
};

enum class UI_PIVOT
{
	//������ 
	LEFT_TOP,
	//���� �Ʒ�
	LEFT_BOTTOM,
	//������ ��
	RIGHT_TOP,
	//�����ʾƷ�
	RIGHT_BOTTOM,
	//�߾��� ����
	MID_UP,
	//�߾��� ������
	MID_RIGHT,
	//�߾��� ����
	MID_LEFT,
	//�߾��� �Ʒ���
	MID_BOTTOM,
	//�߾�
	MID,
};





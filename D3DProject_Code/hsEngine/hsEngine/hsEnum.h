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
	//왼쪽위 
	LEFT_TOP,
	//왼쪽 아래
	LEFT_BOTTOM,
	//오른쪽 위
	RIGHT_TOP,
	//오른쪽아래
	RIGHT_BOTTOM,
	//중앙의 위쪽
	MID_UP,
	//중앙의 오른쪽
	MID_RIGHT,
	//중앙의 왼쪽
	MID_LEFT,
	//중앙의 아래쪽
	MID_BOTTOM,
	//중앙
	MID,
};





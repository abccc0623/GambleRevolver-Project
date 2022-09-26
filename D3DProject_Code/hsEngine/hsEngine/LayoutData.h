#pragma once

/// <summary>
/// 엔진에서 제공하는 오브젝트들의 기본 inputlayout을 생성해주는곳
/// </summary>

//기본 도형의 Layout 타입
struct Basic32
{
	XMFLOAT3 Pos;
	XMFLOAT3 Normal;
	XMFLOAT3 tagantW;
	XMFLOAT2 Tex;
};

//배경 오브젝트의 layout타입
struct Sky32
{
	XMFLOAT3 Pos;
};

//스키닝 오브젝트의 layout타입
struct Skinnde32
{
	XMFLOAT3 Pos;
	XMFLOAT3 Normal;
	XMFLOAT4 Weights;
	XMFLOAT3 tagantW;
	XMFLOAT2 Tex;
	UINT BoneIndex[4];
};

//텍스쳐 오브젝트의 layout타입
struct Texture32
{
	XMFLOAT4 Pos;
	XMFLOAT2 Tex;
};

//디버깅 오브젝트의 layout타입
struct Debug32
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;
};

struct Deferred32
{
	XMFLOAT4 Pos;
	XMFLOAT2 Tex;
	XMFLOAT3 Nomal;
	XMFLOAT3 Tangent;
};
struct Light32
{
	XMFLOAT4 Pos;
	XMFLOAT2 Tex;
};
struct UI32
{
	XMFLOAT4 Pos;
	XMFLOAT2 Tex;
};
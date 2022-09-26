#pragma once

/// <summary>
/// �������� �����ϴ� ������Ʈ���� �⺻ inputlayout�� �������ִ°�
/// </summary>

//�⺻ ������ Layout Ÿ��
struct Basic32
{
	XMFLOAT3 Pos;
	XMFLOAT3 Normal;
	XMFLOAT3 tagantW;
	XMFLOAT2 Tex;
};

//��� ������Ʈ�� layoutŸ��
struct Sky32
{
	XMFLOAT3 Pos;
};

//��Ű�� ������Ʈ�� layoutŸ��
struct Skinnde32
{
	XMFLOAT3 Pos;
	XMFLOAT3 Normal;
	XMFLOAT4 Weights;
	XMFLOAT3 tagantW;
	XMFLOAT2 Tex;
	UINT BoneIndex[4];
};

//�ؽ��� ������Ʈ�� layoutŸ��
struct Texture32
{
	XMFLOAT4 Pos;
	XMFLOAT2 Tex;
};

//����� ������Ʈ�� layoutŸ��
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
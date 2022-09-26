#pragma once

///--------------------------------------------------
/// Animation Data�� �����ϱ� ���� Ŭ����
///--------------------------------------------------




#include "EnginePCH.h"

class CAnimation_pos 
{
public:
	float m_time;
	Vector3	m_pos;
};

class CAnimation_rot 
{
public:
	float		m_time;
	Vector3		m_rot;
	float		m_angle;
	Quaternion	m_rotQT_accumulation;		// ��뺯���̱� ������ ���� ����� �����ؼ� ������ �ִ´� (�� ���ݱ����� ���뷮���� ��ȯ�Ѵ�)
};

class CAnimation_scl 
{

public:
	float		m_time;
	Vector3		m_scale;
	Vector3		m_scaleaxis;
	float		m_scaleaxisang;
	Quaternion	m_scalerotQT_accumulation;
};


///--------------------------------------------------
/// �� Ŭ������ ������ Animation Ŭ����
///--------------------------------------------------
class hsAnimation 
{

public:

	// �ִϸ��̼� �������� �̸�. ������ ���������̴�
	string						m_nodename;
	vector<CAnimation_pos*>		m_position;
	vector<CAnimation_rot*>		m_rotation;
	vector<CAnimation_scl*>		m_scale;

	int		m_ticksperFrame;	// �� �������� ƽ ũ��. �̰��� AnimationTable�� ������ ������ ������Ʈ���� �ٸ� �� �ִ�.
	int		m_minKeyframe;		// Ű �����Ӱ��� �ּ�ũ��. �̰��� ���ǰͰ� �� ��ġ������ �ʴ´�.
	int		m_MaxKeyframe;
};
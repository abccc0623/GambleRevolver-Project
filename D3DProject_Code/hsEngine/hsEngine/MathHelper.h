//***************************************************************************************
// MathHelper.h by Frank Luna (C) 2011 All Rights Reserved.
//
// Helper math class.
//***************************************************************************************

#ifndef MATHHELPER_H
#define MATHHELPER_H

#include <Windows.h>

// LEHIDE
//#include <xnamath.h>
#include <DirectXMath.h>
using namespace DirectX;

class MathHelper
{
public:
	// Returns random float in [0, 1).
	static float RandF()
	{
		return (float)(rand()) / (float)RAND_MAX;
	}

	// Returns random float in [a, b).
	static float RandF(float a, float b)
	{
		return a + RandF()*(b-a);
	}

	template<typename T>
	static T Min(const T& a, const T& b)
	{
		return a < b ? a : b;
	}

	template<typename T>
	static T Max(const T& a, const T& b)
	{
		return a > b ? a : b;
	}
	 
	template<typename T>
	static T Lerp(const T& a, const T& b, float t)
	{
		return a + (b-a)*t;
	}

	template<typename T>
	static T Clamp(const T& x, const T& low, const T& high)
	{
		return x < low ? low : (x > high ? high : x); 
	}

	// Returns the polar angle of the point (x,y) in [0, 2*PI).
	static float AngleFromXY(float x, float y);

	static XMMATRIX InverseTranspose(CXMMATRIX M)
	{
		// Inverse-transpose is just applied to normals.  So zero out 
		// translation row so that it doesn't get into our inverse-transpose
		// calculation--we don't want the inverse-transpose of the translation.
		XMMATRIX A = M;
		A.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

		XMVECTOR det = XMMatrixDeterminant(A);
		return XMMatrixTranspose(XMMatrixInverse(&det, A));
	}

	///쿼터니언을 오일러값으로 바꾸는 함수 인터넷에서 찾음
	static void QuaternionToEuler(const SimpleMath::Vector4 Q , SimpleMath::Vector3& euler)
	{
		double x, y, z, w;
		x = Q.x;
		y = Q.y;
		z = Q.z;
		w = Q.w;

		double sqx = x * x;
		double sqy = y * y;
		double sqz = z * z;
		double sqw = w * w;


		euler.x = (double)(atan2(2.0 * (y * z + x * w), (-sqx - sqy + sqz + sqw) * Pi));
		euler.y = (double)(asin(-2.0 * (x*z - y * w)) *Pi);
		euler.z = (double)(atan2(2.0 * (x*y + z*w), (sqx - sqy - sqz + sqw) * Pi));
	}

	static XMVECTOR RandUnitVec3();
	static XMVECTOR RandHemisphereUnitVec3(XMVECTOR n);

	static const float Infinity;
	static const float Pi;


};

#endif // MATHHELPER_H
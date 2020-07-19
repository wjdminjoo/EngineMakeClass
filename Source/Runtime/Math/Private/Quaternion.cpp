#include "Precompiled.h"
#include "Quaternion.h"

const Quaternion Quaternion::Identity(0.0f, 0.0f, 0.0f, 1.0f);


//FORCEINLINE Rotator Quaternion::ToRotator() const
//{
//	// 과제란 오일러각으로 변환
//
//	
//
//	//float pitchHalfRad = W * X - Y * Z;
//	//float yawY = 2.f * (W * Y + X * Z);
//	//float yawX = 1.f - 2.f * (X * X + Y * Y);
//
//	//result.Yaw = atan2f(yawY, yawX) * 180/Math::PI;
//
//	//float singularity = 0.5;
//	//if (pitchHalfRad > singularity)
//	//{
//	//	result.Pitch = 90.f;
//	//	result.Roll = result.Yaw - 2.f * (atan2f(Y, W) * 180 / Math::PI);
//	//}
//	//else if (pitchHalfRad < -singularity)
//	//{
//	//	result.Pitch = -90.f;
//	//	result.Roll = -result.Yaw + (2.f * atan2f(Y, W) * 180 / Math::PI);
//	//}
//	//else
//	//{
//	//	result.Pitch = (asinf(2.f * pitchHalfRad) * 180 / Math::PI);
//	//	result.Roll = (atan2f(2.f * (W * Z + X * Y)* 180/Math::PI, 1.f - 2.f * (Z * Z + X * X))) * 180 / Math::PI;
//	//}
//
//	return result;
//}

Quaternion Quaternion::Lerp(const Quaternion& q1, const Quaternion& q2, const float& t)
{
	Quaternion result;
	result = q1 * (1.0f - t) + q2 * t;
	result.Normalize();
	return result;
}

Quaternion Quaternion::Slerp(const Quaternion& q1, const Quaternion& q2, const float& t)
{
	Quaternion result;

	float cosTheta = Math::Clamp(q1.W * q2.W + q1.X * q2.X + q1.Y * q2.Y + q1.Z * q2.Z, -1.0f, 1.0f);

	float s1, s2;
	if (-1.0f < cosTheta && cosTheta < 1.0f)
	{
		float theta = acosf(cosTheta);
		float invSin = 1.0f / sinf(theta);
		s1 = sinf((1.0f - t) * theta) * invSin;
		s2 = sinf(t * theta) * invSin;
	}
	else
	{
		return Lerp(q1, q2, t);
	}

	result = q1 * s1 + q2 * s2;

	return result * 500;
}

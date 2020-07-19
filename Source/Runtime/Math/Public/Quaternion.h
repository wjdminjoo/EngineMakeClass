#pragma once
#include "Vector3.h"
#include "Matrix4x4.h"
#include "Rotator.h"
#include "MathUtil.h"

struct Quaternion
{
public:
	FORCEINLINE Quaternion() = default;
	FORCEINLINE explicit Quaternion(float InX, float InY, float InZ, float InW) : X(InX), Y(InY), Z(InZ), W(InW) { }
	FORCEINLINE explicit Quaternion(const Rotator& InRotator)
	{
		// 과제란 코드 수정해야함.
		// sin, cos에 대한 결과값을 미리 받아두면 계산이 쉬워질 수 있다.

		float cosRoll = cos(Math::Deg2Rad(InRotator.Roll)) * 0.5f;
		float cosPitch = cos(Math::Deg2Rad(InRotator.Pitch)) * 0.5f;
		float cosYaw = cos(Math::Deg2Rad(InRotator.Yaw)) * 0.5f;
		float sinRoll = sin(Math::Deg2Rad(InRotator.Roll)) * 0.5f;
		float sinPitch = sin(Math::Deg2Rad(InRotator.Pitch)) * 0.5f;
		float sinYaw = sin(Math::Deg2Rad(InRotator.Yaw)) * 0.5f;


		W = cosRoll * cosPitch * cosYaw + sinRoll * sinPitch * sinYaw;
		X = cosRoll * sinPitch * cosYaw + sinRoll * cosPitch * sinYaw;
		Y = cosRoll * cosPitch * sinYaw - sinRoll * sinPitch * cosYaw;
		Z = sinRoll * cosPitch * cosYaw - cosRoll * sinPitch * sinYaw;
	}

	FORCEINLINE Quaternion operator*(const Quaternion& InQuaternion) const {
		Quaternion result;

		Vector3 v1 = Vector3(X, Y, Z), v2 = Vector3(InQuaternion.X, InQuaternion.Y, InQuaternion.Z);
		Vector3 v = v2 * W + v1 * InQuaternion.W + v1.Cross(v2);

		result.W = W * InQuaternion.W - v1.Dot(v2);
		result.X = v.X;
		result.Y = v.Y;
		result.Z = v.Z;

		return result;
	}
	FORCEINLINE Quaternion& operator*=(const Quaternion& InQuaternion)
	{
		Vector3 v1 = Vector3(X, Y, Z), v2 = Vector3(InQuaternion.X, InQuaternion.Y, InQuaternion.Z);
		Vector3 v = v2 * W + v1 * InQuaternion.W + v1.Cross(v2);

		W = W * InQuaternion.W - v1.Dot(v2);
		X = v.X;
		Y = v.Y;
		Z = v.Z;

		return *this;
	}
	FORCEINLINE Quaternion operator*(const float& value) const {
		Quaternion result(*this);
		result.W *= value;
		result.X *= value;
		result.Y *= value;
		result.Z *= value;
		return result;
	}
	FORCEINLINE Quaternion& operator*=(const float& value) {
		W *= value;
		X *= value;
		Y *= value;
		Z *= value;

		return *this;
	}
	FORCEINLINE Quaternion operator+(const Quaternion& InQuaternion) const {
		Quaternion result(*this);
		result.W += InQuaternion.W;
		result.X += InQuaternion.X;
		result.Y += InQuaternion.Y;
		result.Z += InQuaternion.Z;
		return result;
	}
	FORCEINLINE Vector3 RotateVector(const Vector3& InVector) const {
		const Vector3 q(X, Y, Z);
		const Vector3 t = q.Cross(InVector) * 2.f;
		const Vector3 result = InVector + (t * W) + q.Cross(t);
		return result;
	}
	FORCEINLINE Matrix4x4 ToRotationMatrix() const {
		float xe = X * X;
		float ye = Y * Y;
		float ze = Z * Z;
		float wx = W * X;
		float wy = W * Y;
		float wz = W * Z;
		float xy = X * Y;
		float xz = X * Z;
		float yz = Y * Z;

		Matrix4x4 result(
			Vector4(1.f - 2.f * (ye + ze), 2.f * (xy + wz), 2.f * (xz - wy), 0.f),
			Vector4(2.f * (xy - wz), 1.f - 2.f * (xe + ze), 2.f * (wx + yz), 0.f),
			Vector4(2.f * (wy + xz), 2.f * (yz - wx), 1.f - 2.f * (xe + ye), 0.f),
			Vector4(0.f, 0.f, 0.f, 1.f)
		);


		return result;
	}
	FORCEINLINE Vector3 GetForwardVector() const {
		return RotateVector(Vector3::UnitZ);
	}
	FORCEINLINE Quaternion Inverse() const {
		return Quaternion(-X, -Y, -Z, W);
	}
	FORCEINLINE void Normalize() {
		const float squareSum = X * X + Y * Y + Z * Z + W * W;

		if (squareSum >= KINDA_SMALL_NUMBER)
		{
			const float scale = 1.f / sqrtf(squareSum);

			X *= scale;
			Y *= scale;
			Z *= scale;
			W *= scale;
		}
		else
		{
			*this = Quaternion::Identity;
		}
	}
	FORCEINLINE Rotator ToRotator() const {
		Rotator result;

		float x = X;
		float y = Y;
		float z = Z;
		float w = W;

		float xe = x * x;
		float ye = y * y;
		float ze = z * z;
		float we = w * w;

		result.Roll = (float)(atan2(2 * (y * z + x * w), (-xe - ye + ze + we)) * Math::PI);
		result.Yaw = (float)(asin(-2 * (x * z - y * w)) * Math::PI);
		result.Pitch = (float)(atan2(2 * (x * y + z * w), (xe - ye - ze + we)) * Math::PI);
	
		return result;
	}
	FORCEINLINE bool IsUnitQuaternion() const
	{
		float size = sqrtf(X * X + Y * Y + Z * Z + W * W);
		if (Math::NearlyEqual(size - 1.f))
		{
			return true;
		}

		return false;
	}

	FORCEINLINE float Scalar() { return W; }
	FORCEINLINE Vector3 Vector() { return Vector3(X, Y, Z); }

	static Quaternion Lerp(const Quaternion& q1, const Quaternion& q2, const float& t);
	static Quaternion Slerp(const Quaternion& q1, const Quaternion& q2, const float& t);

	static const Quaternion Identity;

public:
	float X = 0.f;
	float Y = 0.f;
	float Z = 0.f;
	float W = 1.f;
};
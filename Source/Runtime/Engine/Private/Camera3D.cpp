#include "Precompiled.h"


Matrix4x4 Camera3D::GetViewMatrix() const
{
	Matrix4x4 inverseRotation = Matrix4x4(Vector4(_Transform._Right, false), Vector4(_Transform._Up, false), Vector4(_Transform._Forward, false), Vector4::UnitW).Tranpose();

	// transform ¿ªÇà·Ä
	Matrix4x4 inverseTranslate =  Matrix4x4(Vector4::UnitX, Vector4::UnitY, Vector4::UnitZ, Vector4(-_Transform.GetLocalPosition()));
	return inverseRotation * inverseTranslate;
}

void Camera3D::SetLookAtRotation(const Vector3& InTargetPosition)
{
	_Transform._Forward = (_Transform._Position - InTargetPosition).Normalize();
	if (_Transform._Forward.EqualsInTolerance(Vector3::UnitY) ||
		_Transform._Forward.EqualsInTolerance(-Vector3::UnitY))
	{
		_Transform._Right = Vector3::UnitX;
	}
	else
	{
		_Transform._Right = Vector3::UnitY.Cross(_Transform._Forward).Normalize();
	}
	_Transform._Up = _Transform._Forward.Cross(_Transform._Right).Normalize();
}

Matrix4x4 Camera3D::GetProjectionMatrix(int InScreenSizeX, int InScreenSizeY) const
{

	// Open GL : -1 ~ 1
	// DirectX : 0 ~ 1
	// This : -1 ~ 1

	static float invA = (float)InScreenSizeY / (float)InScreenSizeX;
	static float d = 1.0f / tanf(Math::Deg2Rad(FOV) * 0.5f);

	float invNF = 1.f / (NearZ - FarZ);
	float k = (FarZ + NearZ) * invNF;
	float l = (2 * FarZ * NearZ) * invNF;

	return Matrix4x4(
		Vector4::UnitX * invA * d,
		Vector4::UnitY * d,
		Vector4(0.f, 0.f, k, -1.0f),
		Vector4(0.f, 0.f, l, 0.f));
}




#include "Precompiled.h"
#include "Transform3D.h"
#include <assert.h>

Matrix4x4 Transform3D::GetModelingMatrix() const
{
	Matrix4x4 translate = Matrix4x4(Vector4::UnitX, Vector4::UnitY, Vector4::UnitZ, Vector4(_Position, true));
	Matrix4x4 scale = Matrix4x4(Vector4::UnitX * _Scale.X, Vector4::UnitY * _Scale.Y, Vector4::UnitZ * _Scale.Z, Vector4::UnitW);
	return translate * scale;
}

void Transform3D::CalculateLocalAxis()
{

}
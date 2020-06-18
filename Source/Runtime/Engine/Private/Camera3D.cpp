#include "Precompiled.h"
#include "Mesh3D.h"
#include "Transform3D.h"
#include "GameObject3D.h"
#include "Camera3D.h"

Matrix4x4 Camera3D::GetViewMatrix() const
{
	// transform ¿ªÇà·Ä
	return Matrix4x4(Vector4::UnitX, Vector4::UnitY, Vector4(-_Transform._Position), Vector4::One);
}


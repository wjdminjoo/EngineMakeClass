#include "Precompiled.h"
#include "Mesh.h"
#include "Transform2D.h"
#include "GameObject2D.h"
#include "Camera2D.h"

Matrix3x3 Camera2D::GetViewMatrix() const
{
	// transform ¿ªÇà·Ä
	return Matrix3x3(Vector3::UnitX, Vector3::UnitY, Vector3(-_Transform.GetPosition()));
}

void Camera2D::SetCameraViewSize(const ScreenPoint& size)
{

	Vector2 halfSize = Vector2(size.X, size.Y) * 0.5f;



}

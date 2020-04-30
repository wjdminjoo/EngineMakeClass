#include <Precompiled.h>
#include <Transform.h>

Matrix3x3 Transform::GetModelingMatrix() const
{
	Matrix3x3 translateMatrix(Vector3::UnitX, Vector3::UnitY, Vector3(Position, true));

	float cosTheta = cosf(Math::Deg2Rad(Rotation));
	float sinTheta = sinf(Math::Deg2Rad(Rotation));
	Matrix3x3 rotateMatrix(Vector3(Vector2(cosTheta, sinTheta), false), 
						   Vector3(Vector2(-sinTheta, cosTheta), false), 
						   Vector3::UnitZ);

	Matrix3x3 scaleMatrix(Vector3::UnitX * Scale._X, Vector3::UnitY * Scale._Y, Vector3::UnitZ);

	return  translateMatrix * rotateMatrix * scaleMatrix;


}

void Transform::CalculateLocalAxis()
{
	float rotationInRadian = Math::Deg2Rad(Rotation);
	// right = cos, sin
	// up = -sin, cos
	float cosTheta = cosf(Math::Deg2Rad(Rotation));
	float sinTheta = sinf(Math::Deg2Rad(Rotation));
	Right = Vector2(cosTheta, sinTheta);
	Up = Vector2(-sinTheta, cosTheta);
}

#include "Precompiled.h"
#include "Transform3D.h"
#include <assert.h>

Vector3 Transform3D::GetWorldPosition() const
{
	return _WorldPosition;
}

Vector3 Transform3D::GetWorldScale() const
{
	return _WorldScale;
}

Quaternion Transform3D::GetWorldRotation() const
{
	return _WorldRotation;
}

Matrix4x4 Transform3D::GetModelingMatrix() const
{
	Matrix4x4 translate = Matrix4x4(Vector4::UnitX, Vector4::UnitY, Vector4::UnitZ, Vector4(_Position, true));
	Matrix4x4 rotationMat(Vector4(_Right, false), Vector4(_Up, false), Vector4(_Forward, false), Vector4::UnitW);
	Matrix4x4 scale = Matrix4x4(Vector4::UnitX * _Scale.X, Vector4::UnitY * _Scale.Y, Vector4::UnitZ * _Scale.Z, Vector4::UnitW);
	return translate * rotationMat * scale;
}

Matrix4x4 Transform3D::GetInvModelingMatrix() const
{
	Matrix4x4 translateMat(Vector4::UnitX, Vector4::UnitY, Vector4::UnitZ, Vector4(-_Position));

	Matrix4x4 rotationMat = _WorldRotation.ToRotationMatrix();
	rotationMat = rotationMat.Tranpose();

	Matrix4x4 scaleMat(Vector4::UnitX * (1.f / _WorldScale.X), Vector4::UnitY * (1.f / _WorldScale.Y), Vector4::UnitZ * (1.f / _WorldScale.Z), Vector4::UnitW);
	return scaleMat * rotationMat * translateMat;
}

Matrix4x4 Transform3D::GetWorldModelingMatrix() const
{
	return _TRS;
}

Matrix4x4 Transform3D::GetInvWorldModelingMatrix() const
{
	Matrix4x4 translateMat(Vector4::UnitX, Vector4::UnitY, Vector4::UnitZ, Vector4(-_WorldPosition));

	Matrix4x4 rotationMat = _Rotation.ToRotationMatrix();
	rotationMat = rotationMat.Tranpose();

	Matrix4x4 scaleMat(Vector4::UnitX * (1.f / _Scale.X), Vector4::UnitY * (1.f / _Scale.Y), Vector4::UnitZ * (1.f / _Scale.Z), Vector4::UnitW);

	Matrix4x4 result = scaleMat * rotationMat * translateMat;
	if (_ParentPtr)
	{
		result = _ParentPtr->GetInvWorldModelingMatrix() * result;
	}
	return result;
}

bool Transform3D::SetParent(Transform3D* InTransformPtr)
{
	if (InTransformPtr == nullptr)
		return false;

	for (Transform3D* current = InTransformPtr; current != nullptr; current = current->GetParent())
	{
		if (this == current)
			return false;
	}

	if (_ParentPtr != nullptr)
	{
		auto it = std::find(_Children.begin(), _Children.end(), this);
		assert(it != _ParentPtr->_Children.end());
		_ParentPtr->_Children.erase(it);
		_ParentPtr = nullptr;
	}

	InTransformPtr->_Children.push_back(this);
	_ParentPtr = InTransformPtr;

	Vector3 invScale = Vector3(1.f / _ParentPtr->_WorldScale.X, 1.f / _ParentPtr->_WorldScale.Y, 1.f / _ParentPtr->_WorldScale.Z);
	_Scale = Vector3(_Scale.X * invScale.X, _Scale.Y * invScale.Y, _Scale.Z * invScale.Z);
	_Rotation *= _ParentPtr->_WorldRotation.Inverse();

	_Position = _ParentPtr->GetInvWorldModelingMatrix() * _Position;
	Update();

	return true;
}

void Transform3D::Update()
{
	CalculateLocalAxis();
	CalculateMatrix();

	for (auto&& i : _Children)
	{
		i->Update();
	}
}

void Transform3D::CalculateLocalAxis()
{
	float cy, sy, cp, sp, cr, sr;
	Math::GetSinCos(sy, cy, _Rotation.Y);
	Math::GetSinCos(sp, cp, _Rotation.Z);
	Math::GetSinCos(sr, cr, _Rotation.X);

	_Right = Vector3(cy * cr + sy * sp * sr, cp * sr, -sy * cr + cy * sp * sr);
	_Up = Vector3(-cy * sr + sy * sp * cr, cp * cr, sy * sr + cy * sp * cr);
	_Forward = Vector3(sy * cp, -sp, cy * cp);
}

void Transform3D::CalculateMatrix()
{
	_LocalTRS = GetModelingMatrix();

	if (_ParentPtr == nullptr)
	{
		_WorldScale = _Scale;
		_WorldRotation = _Rotation;
		_WorldPosition = _Position;
		_TRS = _LocalTRS;
	}
	else
	{
		_WorldScale = Vector3(_ParentPtr->_WorldScale.X * _Scale.X, _ParentPtr->_WorldScale.Y * _Scale.Y, _ParentPtr->_WorldScale.Z * _Scale.Z);
		_WorldRotation = _ParentPtr->_WorldRotation + _Rotation;
		_WorldPosition = _ParentPtr->_TRS * _Position;
		_TRS = _ParentPtr->_TRS * _LocalTRS;
	}
}

void Transform3D::SetPosition(const Vector3& InPosition)
{
	_Position = InPosition;
	Update();
}

void Transform3D::AddPosition(const Vector3& InDeltaPosition)
{
	_Position += InDeltaPosition;
	Update();
}

void Transform3D::SetScale(const Vector3& InScale)
{
	_Scale = InScale;
	Update();
}

void Transform3D::AddScale(const Vector3& InDeltaScale)
{
}

void Transform3D::AddLocalYawRotation(float yaw)
{
	_Rotation *= Quaternion(Rotator(yaw, 0, 0));
	Update();
}

void Transform3D::AddLocalRollRotation(float roll)
{
	_Rotation *= Quaternion(Rotator(0, roll, 0));
	Update();
}

void Transform3D::AddLocalPitchRotation(float pitch)
{
	_Rotation *= Quaternion(Rotator(0, 0, pitch));
	Update();
}

void Transform3D::SetRotation(const Rotator& InDegree)
{
	_Rotation = Quaternion(InDegree);
	Update();
}

void Transform3D::SetRotation(const Quaternion& InQuaternion)
{
	_Rotation = InQuaternion;
	Update();
}

void Transform3D::SetRotation(const Vector3& InForwardVector)
{
}

void Transform3D::AddRotation(Rotator InDeltaDegree)
{
	_Rotation *= Quaternion(InDeltaDegree);
	Update();
}

void Transform3D::SetWorldScale(const Vector3& InScale)
{
	_WorldScale = InScale;
	if (_ParentPtr == nullptr)
	{
		_Scale = _WorldScale;
	}
	else
	{
		Vector3 invScale = Vector3(1.f / _ParentPtr->_WorldScale.X, 1.f / _ParentPtr->_WorldScale.Y, 1.f / _ParentPtr->_WorldScale.Z);
		_Scale = Vector3(_WorldScale.X * invScale.X, _WorldScale.Y * invScale.Y, _WorldScale.Z * invScale.Z);
	}
	Update();
}

void Transform3D::SetWorldRotation(const Rotator& InDegree)
{
	_WorldRotation = Quaternion(InDegree);
	if (_ParentPtr == nullptr)
	{
		_Rotation = _WorldRotation;
	}
	else
	{
		_Rotation = _WorldRotation * _ParentPtr->_WorldRotation.Inverse();
	}
	Update();
}

void Transform3D::SetWorldRotation(const Quaternion& InQuaternion)
{
	_WorldRotation = InQuaternion;
	if (_ParentPtr == nullptr)
	{
		_Rotation = _WorldRotation;
	}
	else
	{
		_Rotation = _WorldRotation * _ParentPtr->_WorldRotation.Inverse();
	}
	Update();
}

void Transform3D::SetWorldRotation(const Vector3& InForwardVector)
{
}

void Transform3D::AddWorldRotation(Rotator& InDeltaDegree)
{
	_WorldRotation *= Quaternion(InDeltaDegree);
	if (_ParentPtr == nullptr)
	{
		_Rotation = _WorldRotation;
	}
	else
	{
		_Rotation = _WorldRotation * _ParentPtr->_WorldRotation.Inverse();
	}
	Update();
}

void Transform3D::SetWorldPosition(const Vector3& InPosition)
{
	_WorldPosition = InPosition;
	if (_ParentPtr == nullptr)
	{
		_Position = _WorldPosition;
	}
	else
	{
		_Position = _ParentPtr->GetInvWorldModelingMatrix() * _WorldPosition;
	}
	Update();
}

void Transform3D::AddWorldPosition(const Vector3& InDeltaPosition)
{
	_WorldPosition += InDeltaPosition;
	if (_ParentPtr == nullptr)
	{
		_Position = _WorldPosition;
	}
	else
	{
		_Position = _ParentPtr->GetInvWorldModelingMatrix() * _WorldPosition;
	}
	Update();
}
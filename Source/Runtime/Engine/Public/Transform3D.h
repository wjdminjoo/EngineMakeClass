#pragma once
class Transform3D
{
public:
	Transform3D() = default;

public:
	void SetPosition(const Vector3& InPosition) { _Position = InPosition; }
	void SetScale(const Vector3& InScale) { _Scale = InScale; }
	void SetRoation(const Rotator& InRotator) { _Rotation = InRotator; CalculateLocalAxis(); }

	void AddPosition(const Vector3& InDeltaPosition) { _Position += InDeltaPosition; }
	void AddYawRoation(float InDegree) { _Rotation.Yaw += InDegree; _Rotation.Clamp(); CalculateLocalAxis(); }
	void AddRollRoation(float InDegree) { _Rotation.Roll += InDegree; _Rotation.Clamp(); CalculateLocalAxis(); }
	void AddPitchRoation(float InDegree) { _Rotation.Pitch += InDegree; _Rotation.Clamp(); CalculateLocalAxis(); }
	Vector3 GetLocalPosition() const { return _Position; }
	Vector3 GetLocalScale() const {	return _Scale; }
	Rotator GetLocalRotation() const { return _Rotation; }

	Matrix4x4 GetModelingMatrix() const;
	const Vector3& GetLocalX() const { return _Right; }
	const Vector3& GetLocalY() const { return _Up; }
	const Vector3& GetLocalZ() const { return _Forward; }

	

private:
	void CalculateLocalAxis();

	Vector3 _Position = Vector3::Zero;
	Rotator _Rotation;
	Vector3 _Scale = Vector3::One;

	Vector3 _Right = Vector3::UnitX;
	Vector3 _Up = Vector3::UnitY;
	Vector3 _Forward = Vector3::UnitZ;

	friend class GameObject3D;
	friend class Camera3D;
};
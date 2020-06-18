#pragma once

class Camera3D : GameObject3D {
public:
	Camera3D() = default;
	Transform3D& GetTransform() { return _Transform; }
	Matrix4x4 GetViewMatrix() const;
	void SetLookAtRotation(const Vector3& InTargetPosition);
private:
	
	Transform3D _Transform;
};
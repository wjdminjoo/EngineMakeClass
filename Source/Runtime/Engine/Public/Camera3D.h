#pragma once

class Camera3D : GameObject3D {
public:
	Camera3D() = default;
	Transform3D& GetTransform() { return _Transform; }
	Matrix4x4 GetViewMatrix() const;
	void SetLookAtRotation(const Vector3& InTargetPosition);
	Matrix4x4 GetProjectionMatrix(int InScreenSizeX, int InScreenSizeY) const;
	float GetFov() const { return FOV; }
	float GetNearZ() const { return NearZ; }
	float GetFarZ() const { return FarZ; }
private:
	
	Transform3D _Transform;
	float FOV = 60.f;
	float NearZ = 5.5f;
	float FarZ = 1000.f;
};
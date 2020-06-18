#pragma once

class Camera3D : public GameObject3D {
public:
	Camera3D() = default;
	Transform3D& GetTransform() { return _Transform; }
	Matrix4x4 GetViewMatrix() const;
private:
	
};
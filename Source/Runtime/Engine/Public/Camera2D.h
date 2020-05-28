#pragma once

class Camera2D : public GameObject2D {
public:
	Camera2D() = default;

	Matrix3x3 GetViewMatrix() const;
	
	void SetCameraViewSize(const ScreenPoint& size);
	inline void SetCameraCircleBound(float InRad) { CircleBound.Radius = InRad; };
	inline const Circle& GetCircleBound() { return CircleBound; }

private:
	Circle CircleBound;
};
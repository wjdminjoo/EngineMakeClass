#pragma once

class Camera2D : public GameObject2D {
public:
	Camera2D() : CircleBound(Circle()){}
	Matrix3x3 GetViewMatrix() const;
	
	void SetCameraViewSize(const ScreenPoint& size);
	inline void SetCameraCircleBound(float InRad) { CircleBound.Radius = InRad; };
	inline const Circle& GetCircleBound() { return CircleBound; }
	inline Rectangle& GetRectBound() { return RectBound; }
private:
	Circle CircleBound;
	Rectangle RectBound;
};
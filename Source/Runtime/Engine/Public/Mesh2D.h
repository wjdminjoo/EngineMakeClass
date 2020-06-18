#pragma once

class Mesh2D {
public:
	Mesh2D(){}
	~Mesh2D(){}

	void CalcuBound();
	const Circle& GetCircleBound() const { return CircleBound; }
	const Rectangle GetRectBound() const { return RectBound; }

	std::vector<Vector2> _Vertices;
	std::vector<int> _Indices;

private:
	Circle CircleBound;
	Rectangle RectBound;
};
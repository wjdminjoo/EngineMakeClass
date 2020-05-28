#pragma once

class Mesh {
public:
	Mesh(){}
	~Mesh(){}

	void CalcuBound();
	const Circle& GetCircleBound() const { return CircleBound; }

	std::vector<Vector2> _Vertices;
	std::vector<int> _Indices;

private:
	Circle CircleBound;

};
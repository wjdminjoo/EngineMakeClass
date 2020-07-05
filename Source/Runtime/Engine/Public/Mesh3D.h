#pragma once

class Mesh3D {
public:
	Mesh3D(){}
	~Mesh3D(){}

	
	std::vector<Vector3> _Vertices;
	std::vector<int> _Indices;


	Sphere GetSphereBound() const { return _SphereBound; }

private:
	Sphere _SphereBound;
};
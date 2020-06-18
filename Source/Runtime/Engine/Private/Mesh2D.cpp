#include"Precompiled.h"
#include "Mesh2D.h"

void Mesh2D::CalcuBound()
{
	CircleBound = Circle(_Vertices.data(), _Vertices.size());
	RectBound = Rectangle(_Vertices.data(), _Vertices.size());
}

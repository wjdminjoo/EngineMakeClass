#include"Precompiled.h"
#include "Mesh.h"

void Mesh::CalcuBound()
{
	CircleBound = Circle(_Vertices.data(), _Vertices.size());
}

#include "Precompiled.h"
#include "Sphere.h"

Sphere::Sphere(const Vector3* InVertices, int InVertexCount)
{
	Vector3 sumPosition;
	for (int vi = 0; vi < InVertexCount; ++vi)
	{
		sumPosition += InVertices[vi];
	}

	Center = sumPosition / (float)InVertexCount;

	float maxSize = 0.f;
	for (int vi = 0; vi < InVertexCount; ++vi)
	{
		float sizeSquared = (Center - InVertices[vi]).SizeSquared();
		if (sizeSquared > maxSize)
		{
			maxSize = sizeSquared;
		}
	}

	Radius = sqrtf(maxSize);
}
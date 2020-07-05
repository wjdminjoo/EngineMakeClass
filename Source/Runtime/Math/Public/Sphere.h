#pragma once

#include "Vector4.h"

struct Sphere
{
public:
	FORCEINLINE Sphere() = default;
	FORCEINLINE Sphere(const Sphere& InCircle) : Center(InCircle.Center), Radius(InCircle.Radius) {};
	Sphere(const Vector3* InVertices, int InCount);

	FORCEINLINE bool IsInside(const Vector3& InVector) const;
	FORCEINLINE bool Intersect(const Sphere& InCircle) const;

public:
	Vector3 Center = Vector3::Zero;
	float Radius = 0.f;
};

FORCEINLINE bool Sphere::IsInside(const Vector3& InVector) const
{
	return ((Center - InVector).SizeSquared() <= (Radius * Radius));
}

FORCEINLINE bool Sphere::Intersect(const Sphere& InCircle) const
{
	float twoRadiusSum = Radius + InCircle.Radius;
	return (Center - InCircle.Center).SizeSquared() < (twoRadiusSum * twoRadiusSum);
}
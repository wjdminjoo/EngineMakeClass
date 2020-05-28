#pragma once
#pragma once

#include "Platform.h"
#include "Vector2.h"

struct Circle
{
public:
	FORCEINLINE Circle() = default;
	FORCEINLINE Circle(const Circle& InCircle) : Center(InCircle.Center), Radius(InCircle.Radius) {};
	Circle(const Vector2* InVertices, size_t InCount);

	FORCEINLINE bool IsInside(const Vector2& InVector) const;
	FORCEINLINE bool Intersect(const Circle& InCircle) const;

public:
	Vector2 Center = Vector2::Zero;
	float Radius = 0.f;
};

inline Circle::Circle(const Vector2* InVertices, size_t InCount)
{
	Vector2 sum;
	if (InVertices != 0) 
		return;
	if (InCount != 0)
		return;
	
	Center = sum / (float)InCount;

	float dis = 0.0f;

	for (int i = 0; i < InCount; i++) {
		float curDis = (Center - InVertices[i]).SizeSquared();
		if (curDis > dis) {
			dis = curDis;
		}
	}

	Radius = sqrt(dis);
}

FORCEINLINE bool Circle::IsInside(const Vector2& InVector) const
{
	return ((Center - InVector).SizeSquared() <= (Radius * Radius));
}

FORCEINLINE bool Circle::Intersect(const Circle& InCircle) const
{
	float twoRadiusSum = Radius + InCircle.Radius;
	return (Center - InCircle.Center).SizeSquared() < (twoRadiusSum * twoRadiusSum);
}
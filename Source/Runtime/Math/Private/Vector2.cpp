
#include "Precompiled.h"
#include "Vector2.h"

const Vector2 Vector2::UnitX(1.f, 0.f);
const Vector2 Vector2::UnitY(0.f, 1.f);
const Vector2 Vector2::Zero(0.f, 0.f);
const Vector2 Vector2::One(1.f, 1.f);
const Vector2 Vector2::Infinity(INFINITY, INFINITY);
const Vector2 Vector2::InfinityNeg(-INFINITY, -INFINITY);

std::string Vector2::ToString() const
{
	char result[64];
	std::snprintf(result, sizeof(result), "(%.3f, %.3f)", X, Y);
	return result;
}
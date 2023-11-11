#pragma once

#include "../Engine/MathUtil.h"

class Plane {
public:
	Vector3 normal = { 0.0f, 1.0f, 0.0f };
	float distance = 0.0f;
	Vector3 point = { 0.0f, 0.0f, 0.0f };

	Plane() {
	}
	//Plane(const Vector3& Normal, const float& Dist) : normal(Normal), distance(Dist) {}

	Plane(const Vector3& Normal, const Vector3& Point) :
		normal(Normal.normalized()),
		distance(Vector3::dot(Normal, Point)),
		point(Point) {}

	float GetSignedDistanceToPlane(const Vector3& point) const {
		return Vector3::dot(normal, point) - distance;
	}
};
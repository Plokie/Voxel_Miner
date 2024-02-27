#pragma once

#include "../Engine/MathUtil.h"
#include "Plane.h"

class Camera;

class Frustum {
public:
	Plane top;
	Plane bottom;
	Plane right;
	Plane left;
	Plane farPlane;
	Plane nearPlane;

	Frustum() {}

	static Frustum CreateFrustumFromCamera(const Camera& cam, float nearZ, float farZ);
};
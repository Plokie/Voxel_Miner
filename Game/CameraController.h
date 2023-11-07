#pragma once

#include "../Engine/Engine.h"
#include "Physics/AABB.h"

class CameraController : public Object3D {
	AABB aabb;

	void Start() override;
	void Update(float dTime) override;
};
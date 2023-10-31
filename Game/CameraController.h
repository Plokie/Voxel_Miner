#pragma once

#include "../Engine/Engine.h"

class CameraController : public Object3D {
	void Start() override;
	void Update(float dTime) override;
};
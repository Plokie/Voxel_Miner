#pragma once

#include "../Engine/Engine.h"
#include "../Engine/Object3D.h"

class CameraController : public Object3D {
	void Start() override;
	void Update(float dTime) override;
};
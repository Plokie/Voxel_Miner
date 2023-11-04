#pragma once

#include "../Engine/Object3D.h"
#include "../Engine/Input.h"

class TitleScreen : public Object3D {
	void Start() override;
	void Update(float dt) override;
};
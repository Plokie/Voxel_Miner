#pragma once

#include "Transform.h"

class World {
public:
	Transform camera;

	World() {
		camera = Transform();
	}
};
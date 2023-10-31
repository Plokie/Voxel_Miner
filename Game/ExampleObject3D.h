#pragma once

#include "../Engine/Engine.h"

class ExampleObject3D : public Object3D {
public:
	ExampleObject3D(float x, float y, float z) : rotationDirection(Vector3(x,y,z)) {}

	Vector3 rotationDirection = Vector3();


	void Update(float dTime) override;
	void Start() override;
};
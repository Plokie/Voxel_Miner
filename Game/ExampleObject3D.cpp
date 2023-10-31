#include "ExampleObject3D.h"

void ExampleObject3D::Start()
{
}


void ExampleObject3D::Update(float dTime)
{
	//transform.rotation.x += 5.f * dTime;
	transform.rotation += this->rotationDirection * dTime;
}


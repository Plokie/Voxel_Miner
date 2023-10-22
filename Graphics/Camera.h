#pragma once
#include <DirectXMath.h>
#include "../Engine/Transform.h"
using namespace DirectX;

class Camera
{
private:
	XMMATRIX projectionMatrix;
public:
	Camera();
	void SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ);

	Transform transform;

	const XMMATRIX& GetProjectionMatrix() const;
};
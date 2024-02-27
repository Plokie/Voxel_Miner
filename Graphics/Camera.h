#pragma once
#include <DirectXMath.h>
#include "../Engine/Transform.h"
#include "../Engine/AABB.h"
#include "Frustum.h"
using namespace DirectX;

class Camera
{
private:
	XMMATRIX projectionMatrix;
	Frustum viewFrustum;

	float fovRadians = 1.57f;
	float orthoSize = 1.f;
	float aspectRatio = 1.7777f;
	float nearZ = 0.01f;
	float farZ = 1000.0f;

public:
	Camera();
	void SetProjectionValues(float fovDegrees, float aspectRatio, float nearZ, float farZ);
	void SetProjectionOthographic(float size, float nearZ, float farZ);

	bool IsAABBInFrustum(const AABB& aabb);

	void UpdateViewFrustum();

	const float GetOrthographicSize() const;

	Transform transform;

	const XMMATRIX& GetProjectionMatrix() const;
	void SetProjectionMatrix(const XMMATRIX& mx);
};
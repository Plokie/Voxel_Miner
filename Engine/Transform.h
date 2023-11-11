#pragma once

#include <DirectXMath.h>
#include "MathUtil.h"

using namespace DirectX;

class Transform {
private:
	const XMVECTOR DEFAULT_FORWARD_VECTOR = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	const XMVECTOR DEFAULT_UP_VECTOR = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	const XMVECTOR DEFAULT_BACKWARD_VECTOR = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
	const XMVECTOR DEFAULT_LEFT_VECTOR = XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);
	const XMVECTOR DEFAULT_RIGHT_VECTOR = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
public:
	Vector3 position;
	Vector3 rotation; //todo: make quaternion to avoid gimbal lock
	Vector3 scale;


	Transform() {
		position = Vector3(0.0f, 0.0f, 0.0f);
		rotation = Vector3(0.0f, 0.0f, 0.0f);
		scale = Vector3(1.0f, 1.0f, 1.0f);
	}

	XMMATRIX mxView(); // For camera
	XMMATRIX mx();

	Vector3 basis(const float& x, const float& y, const float& z) const;
	Vector3 basis(const Vector3& v) const;
	Vector3 forward() const;
	Vector3 back() const;
	Vector3 left() const;
	Vector3 right() const;
	Vector3 up() const;
	Vector3 down() const;

	void TranslateLocal(float x, float y, float z);

	void operator=(const Transform& rhs) {
		this->position = rhs.position;
		this->rotation = rhs.rotation;
		this->scale = rhs.scale;
	}
};
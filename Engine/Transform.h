#pragma once

#include <DirectXMath.h>
#include "MathUtil.h"

using namespace DirectX;

class Transform {
private:
public:
	Vector3 position;
	Vector3 rotation; //todo: make quaternion to avoid gimbal lock
	Vector3 scale;

	Transform() {
		position = Vector3(0.0f, 0.0f, 0.0f);
		rotation = Vector3(0.0f, 0.0f, 0.0f);
		scale = Vector3(1.0f, 1.0f, 1.0f);
	}

	XMMATRIX mx() {
		XMMATRIX mx = XMMatrixIdentity();
		mx *= XMMatrixScaling(scale.x, scale.y, scale.z);
		mx *= XMMatrixTranslation(position.x, position.y, position.z);
		mx *= XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
	}
};
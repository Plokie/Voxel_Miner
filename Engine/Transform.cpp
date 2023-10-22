#include "Transform.h"

const XMMATRIX& Transform::mxView() {
	XMMATRIX mx = XMMatrixIdentity();

	mx *= XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);

	XMVECTOR fwdTarget = XMVector3TransformCoord(DEFAULT_FORWARD_VECTOR, mx);
	fwdTarget += position.xmVec();
	
	XMVECTOR upDir = XMVector3TransformCoord(DEFAULT_UP_VECTOR, mx);
	
	return XMMatrixLookAtLH(position.xmVec(), fwdTarget, upDir);
}

const XMMATRIX& Transform::mx() {
	XMMATRIX mx = XMMatrixIdentity();

	mx *= XMMatrixScaling(scale.x, scale.y, scale.z);
	mx *= XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
	mx *= XMMatrixTranslation(position.x, position.y, position.z);

	return mx;
}

Vector3 Transform::basis(float x, float y, float z) {
	return Vector3(XMVector3TransformCoord(XMVectorSet(x, y, z, 0.f), XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z)));
}

Vector3 Transform::forward() {
	return basis(0.f, 0.f, 1.f);
}

Vector3 Transform::back() {
	return basis(0.f, 0.f, -1.f);
}

Vector3 Transform::left() {
	return basis(-1.f, 0.f, 0.f);
}

Vector3 Transform::right() {
	return basis(1.f, 0.f, 0.f);
}

Vector3 Transform::up() {
	return basis(0.f, 1.f, 0.f);
}

Vector3 Transform::down() {
	return basis(0.f, -1.f, 0.f);
}
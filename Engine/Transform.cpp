#include "Transform.h"

XMMATRIX Transform::mxView() {
	XMMATRIX mx = XMMatrixIdentity();

	mx *= XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);

	XMVECTOR fwdTarget = XMVector3TransformCoord(DEFAULT_FORWARD_VECTOR, mx);
	fwdTarget += position;
	
	XMVECTOR upDir = XMVector3TransformCoord(DEFAULT_UP_VECTOR, mx);
	
	return XMMatrixLookAtLH(position, fwdTarget, upDir);
}

XMMATRIX Transform::mx() {
	XMMATRIX mx = XMMatrixIdentity();

	mx *= XMMatrixScaling(scale.x, scale.y, scale.z);
	mx *= XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
	mx *= XMMatrixTranslation(position.x, position.y, position.z);

	return mx;
}

Vector3 Transform::basis(const float& x, const float& y, const float& z) const {
	return Vector3(XMVector3TransformCoord(XMVectorSet(x, y, z, 0.f), XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z)));
}

Vector3 Transform::basis(const Vector3& v) const {
	return basis(v.x, v.y, v.z);
}

Vector3 Transform::forward() const {
	return basis(0.f, 0.f, 1.f);
}

Vector3 Transform::back() const {
	return basis(0.f, 0.f, -1.f);
}

Vector3 Transform::left() const {
	return basis(-1.f, 0.f, 0.f);
}

Vector3 Transform::right() const {
	return basis(1.f, 0.f, 0.f);
}

Vector3 Transform::up() const {
	return basis(0.f, 1.f, 0.f);
}

Vector3 Transform::down() const {
	return basis(0.f, -1.f, 0.f);
}

void Transform::TranslateLocal(float x, float y, float z)
{
	position += basis(x, y, z);
}
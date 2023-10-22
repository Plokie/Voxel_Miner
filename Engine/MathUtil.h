#pragma once

#include <DirectXMath.h>

using namespace DirectX;

struct Vector3 {
public:
	float x=0, y=0, z=0;

	Vector3() : x(0.0f), y(0.0f), z(0.0f){}
	Vector3(float X, float Y, float Z) : x(X), y(Y), z(Z){}

	Vector3 operator+(Vector3& a) {
		return Vector3(x + a.x, y + a.y, z + a.z);
	}
	Vector3 operator-(Vector3& a) {
		return Vector3(x - a.x, y - a.y, z - a.z);
	}
	Vector3 operator/(float a) {
		return Vector3(x / a, y / a, z / a);
	}
	Vector3 operator*(float a) {
		return Vector3(x * a, y * a, z * a);
	}

	Vector3 operator^(Vector3& a) { //Scale vector
		return Vector3(x * a.x, y * a.y, z * a.z);
	}

	XMVECTOR xmVec() {
		return XMVectorSet(x, y, z, 0.0f);
	}
};
#pragma once

#include <DirectXMath.h>

using namespace DirectX;
using namespace std;

struct Vector3 {
public:
	float x=0, y=0, z=0;

	Vector3() : x(0.0f), y(0.0f), z(0.0f){}
	Vector3(float X, float Y, float Z) : x(X), y(Y), z(Z){}

	Vector3(const XMVECTOR& xmv) {
		x = XMVectorGetX(xmv);
		y = XMVectorGetY(xmv);
		z = XMVectorGetZ(xmv);
	}

	static const Vector3 Zero() {
		return Vector3(0.f, 0.f, 0.f);
	}

	Vector3 operator+(Vector3& a) {
		return Vector3(x + a.x, y + a.y, z + a.z);
	}
	Vector3& operator+=(const Vector3& a) {
		this->x += a.x;
		this->y += a.y;
		this->z += a.z;
		return *this;
	}

	Vector3 operator-(Vector3& a) {
		return Vector3(x - a.x, y - a.y, z - a.z);
	}
	Vector3& operator-=(const Vector3& a) {
		this->x -= a.x;
		this->y -= a.y;
		this->z -= a.z;
		return *this;
	}

	Vector3 operator/(float& a) {
		return Vector3(x / a, y / a, z / a);
	}
	Vector3& operator/=(const float& a) {
		this->x /= a;
		this->y /= a;
		this->z /= a;
		return *this;
	}

	Vector3 operator*(float& a) {
		return Vector3(x * a, y * a, z * a);
	}
	Vector3& operator*=(const float& a) {
		this->x *= a;
		this->y *= a;
		this->z *= a;
		return *this;
	}

	Vector3 operator^(Vector3& a) { //Scale vector
		return Vector3(x * a.x, y * a.y, z * a.z);
	}
	Vector3& operator^=(const Vector3& a) { //Scale vector
		this->x *= a.x;
		this->y *= a.y;
		this->z *= a.z;
		return *this;
	}

	const XMVECTOR& xmVec() {
		return XMVectorSet(x, y, z, 0.0f);
	}
};
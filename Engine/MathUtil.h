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

	Vector3 operator/(const float& a) {
		return Vector3(x / a, y / a, z / a);
	}
	Vector3& operator/=(const float& a) {
		this->x /= a;
		this->y /= a;
		this->z /= a;
		return *this;
	}



	Vector3 operator*(const float& a) {
		return Vector3(x * a, y * a, z * a);
	}
	Vector3& operator*=(const float& a) {
		this->x *= a;
		this->y *= a;
		this->z *= a;
		return *this;
	}

	Vector3 operator^(const Vector3& a) { //Scale vector
		return Vector3(x * a.x, y * a.y, z * a.z);
	}
	Vector3& operator^=(const Vector3& a) { //Scale vector
		this->x *= a.x;
		this->y *= a.y;
		this->z *= a.z;
		return *this;
	}

	float sqrMagnitude() const {
		return (x * x) + (y * y) + (z * z);
	}

	float magnitude() const {
		return sqrtf(sqrMagnitude());
	}

	Vector3 normalized() const {
		return Vector3(x,y,z) / magnitude();
	}

	const XMVECTOR& xmVec() {
		return XMVectorSet(x, y, z, 0.0f);
	}
};

struct Vector2 {
public:
	float x = 0, y = 0;

	Vector2() : x(0.0f), y(0.0f) {}
	Vector2(float X, float Y) : x(X), y(Y) {}

	Vector2(const XMVECTOR& xmv) {
		x = XMVectorGetX(xmv);
		y = XMVectorGetY(xmv);
	}

	static const Vector2 Zero() {
		return Vector2(0.f, 0.f);
	}

	Vector2 operator+(Vector2& a) {
		return Vector2(x + a.x, y + a.y);
	}
	Vector2& operator+=(const Vector2& a) {
		this->x += a.x;
		this->y += a.y;
		return *this;
	}

	Vector2 operator-(Vector2& a) {
		return Vector2(x - a.x, y - a.y);
	}
	Vector2& operator-=(const Vector2& a) {
		this->x -= a.x;
		this->y -= a.y;
		return *this;
	}

	Vector2 operator/(const float& a) {
		return Vector2(x / a, y / a);
	}
	Vector2& operator/=(const float& a) {
		this->x /= a;
		this->y /= a;
		return *this;
	}

	Vector2 operator*(const float& a) {
		return Vector2(x * a, y * a);
	}
	Vector2& operator*=(const float& a) {
		this->x *= a;
		this->y *= a;
		return *this;
	}

	Vector2 operator^(const Vector2& a) { //Scale vector
		return Vector2(x * a.x, y * a.y);
	}
	Vector2& operator^=(const Vector2& a) { //Scale vector
		this->x *= a.x;
		this->y *= a.y;
		return *this;
	}

	float sqrMagnitude() const {
		return (x * x) + (y * y);
	}

	float magnitude() const {
		return sqrtf(sqrMagnitude());
	}

	Vector2 normalized() const {
		return Vector2(x, y) / magnitude();
	}

	const XMVECTOR& xmVec() {
		return XMVectorSet(x, y, 0.0f, 0.0f);
	}
};
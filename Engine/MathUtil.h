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

	/// <summary>
	/// This isn't a bitwise XOR, it scales the vector by another
	/// </summary>
	/// <param name="a"></param>
	/// <returns></returns>
	Vector3 operator^(const Vector3& a) { //Scale vector
		return Vector3(x * a.x, y * a.y, z * a.z);
	}
	// This isn't a bitwise XOR, it scales the vector by another
	Vector3& operator^=(const Vector3& a) { //Scale vector
		this->x *= a.x;
		this->y *= a.y;
		this->z *= a.z;
		return *this;
	}

	float operator[](int index) {
		switch (index) {
		case 0: return x;
		case 1: return y;
		case 2: return z;
		default:throw "Index out of range";
		}
		
	}

	float sqrMagnitude() const {
		return (x * x) + (y * y) + (z * z);
	}

	float magnitude() const {
		return sqrtf(sqrMagnitude());
	}

	float sqrDistTo(Vector3& otherPos) {
		return ((otherPos.x - x) * (otherPos.x - x)) +
			((otherPos.y - y) * (otherPos.y - y)) +
			((otherPos.z - z) * (otherPos.z - z));
	}

	Vector3 normalized() const {
		return Vector3(x,y,z) / magnitude();
	}

	/*const XMVECTOR& xmVec() {
		return XMVectorSet(x, y, z, 0.0f);
	}*/

	operator XMVECTOR() const {
		return XMVectorSet(x, y, z, 0.0f);
	}
};
//
//struct Vector3Int {
//public:
//	int x = 0, y = 0, z = 0;
//
//	Vector3Int() : x(0), y(0), z(0) {}
//	Vector3Int(int X, int Y, int Z) : x(X), y(Y), z(Z) {}
//
//	Vector3Int(const XMVECTOR& xmv) {
//		x = XMVectorGetX(xmv);
//		y = XMVectorGetY(xmv);
//		z = XMVectorGetZ(xmv);
//	}
//
//	static const Vector3Int Zero() {
//		return Vector3Int(0, 0, 0);
//	}
//
//	Vector3Int operator+(Vector3Int& a) {
//		return Vector3Int(x + a.x, y + a.y, z + a.z);
//	}
//	Vector3Int& operator+=(const Vector3Int& a) {
//		this->x += a.x;
//		this->y += a.y;
//		this->z += a.z;
//		return *this;
//	}
//
//	Vector3Int operator-(Vector3Int& a) {
//		return Vector3Int(x - a.x, y - a.y, z - a.z);
//	}
//	Vector3Int& operator-=(const Vector3Int& a) {
//		this->x -= a.x;
//		this->y -= a.y;
//		this->z -= a.z;
//		return *this;
//	}
//
//	Vector3Int operator/(const float& a) {
//		return Vector3Int(x / a, y / a, z / a);
//	}
//	Vector3Int& operator/=(const float& a) {
//		this->x /= a;
//		this->y /= a;
//		this->z /= a;
//		return *this;
//	}
//
//
//
//	Vector3Int operator*(const float& a) {
//		return Vector3Int(x * a, y * a, z * a);
//	}
//	Vector3Int& operator*=(const float& a) {
//		this->x *= a;
//		this->y *= a;
//		this->z *= a;
//		return *this;
//	}
//
//	/// <summary>
//	/// This isn't a bitwise XOR, it scales the vector by another
//	/// </summary>
//	/// <param name="a"></param>
//	/// <returns></returns>
//	Vector3Int operator^(const Vector3Int& a) { //Scale vector
//		return Vector3Int(x * a.x, y * a.y, z * a.z);
//	}
//	// This isn't a bitwise XOR, it scales the vector by another
//	Vector3Int& operator^=(const Vector3Int& a) { //Scale vector
//		this->x *= a.x;
//		this->y *= a.y;
//		this->z *= a.z;
//		return *this;
//	}
//
//	float operator[](int index) {
//		switch(index) {
//		case 0: return x;
//		case 1: return y;
//		case 2: return z;
//		default:throw "Index out of range";
//		}
//
//	}
//
//	float sqrMagnitude() const {
//		return (x * x) + (y * y) + (z * z);
//	}
//
//	float magnitude() const {
//		return sqrtf(sqrMagnitude());
//	}
//
//	float sqrDistTo(Vector3& otherPos) {
//		return ((otherPos.x - x) * (otherPos.x - x)) +
//			((otherPos.y - y) * (otherPos.y - y)) +
//			((otherPos.z - z) * (otherPos.z - z));
//	}
//
//	Vector3 normalized() const {
//		return Vector3(x, y, z) / magnitude();
//	}
//
//	/*const XMVECTOR& xmVec() {
//		return XMVectorSet(x, y, z, 0.0f);
//	}*/
//
//	operator XMVECTOR() const {
//		return XMVectorSet(x, y, z, 0.0f);
//	}
////};

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
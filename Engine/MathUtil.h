#pragma once

#include <string>
#include <tuple>
#include <vector>
#include <DirectXMath.h>

using namespace DirectX;
using namespace std;

inline int FloorAbs(float v) {
	return ((v < 0) ? -1 : 1) * static_cast<int>(floor(abs(v)));
}
inline int FloorMod(const int& a, const int& b) {
	return (a % b + b) % b;
}

inline float frac(float f) {
	float ff = abs(f) - floorf(abs(f));
	if(f < 0) return 1.0f - ff;
	return ff;
}

inline float lerp(float a, float b, float t) {
	return a + t * (b - a);
}
//
//namespace Utility {
//	template<typename T>
//	T WeightedRandomPick(std::vector<pair<T, float>> weights) {
//
//	}
//
//};

class Utility {
public:
	template<typename T>
	static T WeightedRandomPick(const std::vector<pair<T, int>>& weights, float randomValue = -999.f) {
		if(randomValue == -999.f) {
			randomValue = (rand() % 1000) / 1000.f;
		}

		int sumOfWeights = 0;
		for(const pair<T, int>& pair : weights) {
			sumOfWeights += pair.second;
		}

		int rand = static_cast<int>(sumOfWeights * randomValue);
		for(const pair<T, int>& pair : weights) {
			if(rand < pair.second) return pair.first;
			rand -= pair.second;
		}
		return weights[0].first;
	}
};

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

	string ToString() {
		return to_string(x) + ", " + to_string(y) + ", " + to_string(z);
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

	float& operator[](int index) {
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

	float sqrDistTo(const Vector3& otherPos) const {
		return ((otherPos.x - x) * (otherPos.x - x)) +
			((otherPos.y - y) * (otherPos.y - y)) +
			((otherPos.z - z) * (otherPos.z - z));
	}

	static float Distance(const Vector3& lhs, const Vector3& rhs) {
		return static_cast<float>(sqrt(lhs.sqrDistTo(rhs)));
	}

	Vector3 normalized() const {
		float mag = magnitude();
		if(mag == 0) return Vector3(0,0,0);
		return Vector3(x,y,z) / mag;
	}

	bool operator!=(const Vector3& a) {
		return x != a.x || y != a.y || z != a.z;
	}

	static float dot(const Vector3& lhs, const Vector3& rhs) {
		return (lhs.x * rhs.x) + (lhs.y * rhs.y) + (lhs.z * rhs.z);
	};

	static Vector3 cross(const Vector3& lhs, const Vector3& rhs) {
		return Vector3(
			lhs.y * rhs.z - lhs.z * rhs.y,
			lhs.z * rhs.x - rhs.x * lhs.z,
			lhs.x * rhs.y - lhs.y * rhs.x
		);
	}

	static Vector3 vecLerp(const Vector3& a, const Vector3& b, float t) {
		return {
			lerp(a.x, b.x, t),
			lerp(a.y, b.y, t),
			lerp(a.z, b.z, t)
		};
	}

	/*const XMVECTOR& xmVec() {
		return XMVectorSet(x, y, z, 0.0f);
	}*/

	//static Vector3 FromInt(const Vector3Int& v) {
	//	return Vector3(static_cast<float>(v.x), static_cast<float>(v.y), static_cast<float>(v.z));
	//}


	operator XMVECTOR() const {
		return XMVectorSet(x, y, z, 0.0f);
	}

	explicit operator XMFLOAT3() const{
		return {x,y,z};
	}

	XMVECTOR AsXmVector() const {
		return XMVectorSet(x, y, z, 0.0f);
	}
};

struct Vector3Int {
public:
	int x = 0, y = 0, z = 0;

	Vector3Int() : x(0), y(0), z(0) {}
	Vector3Int(int X, int Y, int Z) : x(X), y(Y), z(Z) {}

	Vector3Int(const XMVECTOR& xmv) {
		x = static_cast<int>(XMVectorGetX(xmv));
		y = static_cast<int>(XMVectorGetY(xmv));
		z = static_cast<int>(XMVectorGetZ(xmv));
	}

	Vector3Int(const tuple<int, int, int>& tuple) {
		x = get<0>(tuple);
		y = get<1>(tuple);
		z = get<2>(tuple);
	}

	static const Vector3Int Zero() {
		return Vector3Int(0, 0, 0);
	}

	string ToString() const{
		return to_string(x) + ", " + to_string(y) + ", " + to_string(z);
	}

	static Vector3Int RoundToInt(const Vector3& v) {
		return Vector3Int(static_cast<int>(roundf(v.x)), static_cast<int>(roundf(v.y)), static_cast<int>(roundf(v.z)));
	}

	static Vector3Int FloorToInt(const Vector3& v) {
		return Vector3Int(static_cast<int>(floorf(v.x)), static_cast<int>(floorf(v.y)), static_cast<int>(floorf(v.z)));
	}

	Vector3Int operator+(const Vector3Int& a) {
		return Vector3Int(x + a.x, y + a.y, z + a.z);
	}
	Vector3Int operator+(const Vector3& a) {
		return Vector3Int(static_cast<int>(x + a.x), static_cast<int>(y + a.y), static_cast<int>(z + a.z));
	}
	Vector3Int& operator+=(const Vector3Int& a) {
		this->x += a.x;
		this->y += a.y;
		this->z += a.z;
		return *this;
	}

	Vector3Int operator-(const Vector3Int& a) {
		return Vector3Int(x - a.x, y - a.y, z - a.z);
	}
	Vector3Int operator-(const Vector3& a) {
		return Vector3Int(static_cast<int>(x - a.x), static_cast<int>(y - a.y), static_cast<int>(z - a.z));
	}
	Vector3Int& operator-=(const Vector3Int& a) {
		this->x -= a.x;
		this->y -= a.y;
		this->z -= a.z;
		return *this;
	}

	Vector3Int operator/(const int& a) {
		return Vector3Int(x / a, y / a, z / a);
	}
	Vector3Int& operator/=(const int& a) {
		this->x /= a;
		this->y /= a;
		this->z /= a;
		return *this;
	}

	bool operator!=(const Vector3Int& a) {
		return x != a.x || y != a.y || z != a.z;
	}
	bool operator==(const Vector3Int& a) const {
		return x == a.x && y == a.y && z == a.z;
	}

	Vector3Int operator*(const int& a) {
		return Vector3Int(x * a, y * a, z * a);
	}
	Vector3Int& operator*=(const int& a) {
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
	Vector3Int operator^(const Vector3Int& a) { //Scale vector
		return Vector3Int(x * a.x, y * a.y, z * a.z);
	}
	// This isn't a bitwise XOR, it scales the vector by another
	Vector3Int& operator^=(const Vector3Int& a) { //Scale vector
		this->x *= a.x;
		this->y *= a.y;
		this->z *= a.z;
		return *this;
	}

	int operator[](int index) {
		switch(index) {
		case 0: return x;
		case 1: return y;
		case 2: return z;
		default:throw "Index out of range";
		}
	}

	//float sqrMagnitude() const {
	//	return (x * x) + (y * y) + (z * z);
	//}

	//float magnitude() const {
	//	return sqrtf(sqrMagnitude());
	//}

	//float sqrDistTo(Vector3& otherPos) {
	//	return ((otherPos.x - x) * (otherPos.x - x)) +
	//		((otherPos.y - y) * (otherPos.y - y)) +
	//		((otherPos.z - z) * (otherPos.z - z));
	//}

	//Vector3 normalized() const {
	//	return Vector3(x, y, z) / magnitude();
	//}

	/*const XMVECTOR& xmVec() {
		return XMVectorSet(x, y, z, 0.0f);
	}*/

	operator tuple<int, int, int>() const {
		return tuple<int, int, int>(x, y, z);
	}

	operator Vector3() const {
		return Vector3(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));
	}

	explicit operator XMVECTOR() const {
		return XMVectorSet(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z), 0.0f);
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

	Vector2 operator+(const Vector2& a) {
		return Vector2(x + a.x, y + a.y);
	}
	Vector2& operator+=(const Vector2& a) {
		this->x += a.x;
		this->y += a.y;
		return *this;
	}

	Vector2 operator-(const Vector2& a) {
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
	const Vector2& operator*(const float& a) const {
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

	static float dot(const Vector2& lhs, const Vector2& rhs) {
		return (lhs.x * rhs.x) + (lhs.y * rhs.y);
	};

	float sqrMagnitude() const {
		return (x * x) + (y * y);
	}

	float magnitude() const {
		return sqrtf(sqrMagnitude());
	}

	Vector2 normalized() const {
		float mag = magnitude();
		if(mag == 0) return Vector2(0.f, 0.f);
		return Vector2(x, y) / mag;
	}

	bool operator==(const Vector2& a) {
		return x == a.x && y == a.y;
	}

	bool operator!=(const Vector2& a) {
		return x != a.x || y != a.y;
	}

	const XMVECTOR& xmVec() {
		return XMVectorSet(x, y, 0.0f, 0.0f);
	}

	explicit operator XMVECTOR() const {
		return XMVectorSet(x, y, 0.0f, 0.0f);
	}

	operator XMFLOAT2() const {
		return XMFLOAT2(x, y);
	}
};

struct Vector2Int {
public:
	int x = 0, y = 0;

	Vector2Int() : x(0), y(0) {}
	Vector2Int(int X, int Y) : x(X), y(Y) {}

	Vector2Int(const XMVECTOR& xmv) {
		x = (int)XMVectorGetX(xmv);
		y = (int)XMVectorGetY(xmv);
	}

	static const Vector2Int Zero() {
		return Vector2Int(0, 0);
	}

	Vector2Int operator+(const Vector2Int& a) {
		return Vector2Int(x + a.x, y + a.y);
	}
	Vector2Int& operator+=(const Vector2Int& a) {
		this->x += a.x;
		this->y += a.y;
		return *this;
	}

	Vector2Int operator-(const Vector2Int& a) {
		return Vector2Int(x - a.x, y - a.y);
	}
	Vector2Int& operator-=(const Vector2Int& a) {
		this->x -= a.x;
		this->y -= a.y;
		return *this;
	}

	Vector2Int operator/(const int& a) {
		return Vector2Int(x / a, y / a);
	}
	Vector2Int& operator/=(const int& a) {
		this->x /= a;
		this->y /= a;
		return *this;
	}

	Vector2Int operator*(const int& a) {
		return Vector2Int(x * a, y * a);
	}
	const Vector2Int& operator*(const int& a) const {
		return Vector2Int(x * a, y * a);
	}

	Vector2Int& operator*=(const int& a) {
		this->x *= a;
		this->y *= a;
		return *this;
	}

	Vector2Int operator^(const Vector2Int& a) { //Scale vector
		return Vector2Int(x * a.x, y * a.y);
	}
	Vector2Int& operator^=(const Vector2Int& a) { //Scale vector
		this->x *= a.x;
		this->y *= a.y;
		return *this;
	}

	//static float dot(const Vector2Int& lhs, const Vector2Int& rhs) {
	//	return (lhs.x * rhs.x) + (lhs.y * rhs.y);
	//};

	//float sqrMagnitude() const {
	//	return (x * x) + (y * y);
	//}

	//float magnitude() const {
	//	return sqrtf(sqrMagnitude());
	//}

	//Vector2Int normalized() const {
	//	float mag = magnitude();
	//	if(mag == 0) return Vector2Int(0.f, 0.f);
	//	return Vector2Int(x, y) / mag;
	//}

	bool operator==(const Vector2Int& a) {
		return x == a.x && y == a.y;
	}

	bool operator!=(const Vector2Int& a) {
		return x != a.x || y != a.y;
	}

	//const XMVECTOR& xmVec() {
	//	return XMVectorSet(x, y, 0.0f, 0.0f);
	//}

	//explicit operator XMVECTOR() const {
	//	return XMVectorSet(x, y, 0.0f, 0.0f);
	//}

	//operator XMFLOAT2() const {
	//	return XMFLOAT2(x, y);
	//}
};
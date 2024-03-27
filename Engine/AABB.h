#pragma once

#include <vector>
#include "MathUtil.h"
#include "../Graphics/Plane.h"

using namespace std;

struct AABB {
private:
	Vector3 center = { 0.f, 0.f, 0.f };
	Vector3 halfSize = { 0.0f, 0.0f, 0.0f };
	
	// READ ONLY, written to automatically
	Vector3 min = { 0.f, 0.f, 0.f }; 
	Vector3 max = { 0.f, 0.f, 0.f };
	
	void RecalculateMinMax() {
		min = center - halfSize;
		max = center + halfSize;
	}

	void RecalculateMinMax(AABB* target) const {
		target->min = target->center - target->halfSize;
		target->max = target->center + target->halfSize;
	}
public:

	AABB() {
		RecalculateMinMax();
	}
	AABB(const Vector3& center, const Vector3& halfSize) {
		this->center = center;
		this->halfSize = halfSize;
		RecalculateMinMax();
	}
	AABB(const Vector2& center, const Vector2& halfSize) {
		this->center = Vector3(center.x, center.y, 0.f);
		this->halfSize = Vector3(halfSize.x, halfSize.y, 1.f);
		RecalculateMinMax();
	}

	static AABB FromMinMax(const Vector3& min, const Vector3& max) {
		AABB newAABB = AABB(Vector3::Zero(), Vector3::Zero());

		newAABB.halfSize = (max - min) / 2.f;
		newAABB.center = min + newAABB.halfSize;
		newAABB.min = min;
		newAABB.max = max;
		return newAABB;
	}

	static AABB FromMinMax(const Vector2& min, const Vector2& max) {
		return FromMinMax(Vector3(min.x, min.y, -1.f), Vector3(max.x, max.y, 1.f));
	}

	void SetPosition(const Vector3& pos) {
		this->center = pos;
		RecalculateMinMax();
	}
	void MovePosition(const Vector3& pos) {
		this->center += pos;
		RecalculateMinMax();
	}
	Vector3 GetPosition() const {
		return this->center;
	}

	Vector3 GetHalfSize() const {
		return this->halfSize;
	}


	void GetMinMax(Vector3* min, Vector3* max) const {
		*min = this->min;
		*max = this->max;
	}

	bool IsOnOrForwardPlane(const Plane& plane) const {
		float r = halfSize.x * abs(plane.normal.x) + halfSize.y * abs(plane.normal.y) + halfSize.z * abs(plane.normal.z);
		float s = Vector3::dot(plane.normal, center) - plane.distance;
		return -r <= s;
	}

	bool IntersectsPlane(const Plane& plane) const {
		float r = halfSize.x * abs(plane.normal.x) + halfSize.y * abs(plane.normal.y) + halfSize.z * abs(plane.normal.z);
		float s = Vector3::dot(plane.normal, center);
		return abs(s) <= r;
	}

	bool Intersects(const AABB& b) {
		Vector3 min, max;
		AABB mk_diff = minkowski_difference(*this, b);
		mk_diff.RecalculateMinMax();
		mk_diff.GetMinMax(&min, &max);

		//return mk_diff.IsPointWithin(Vector3::Zero());

		return
			min.x <= 0 &&
			max.x >= 0 &&
			min.y <= 0 &&
			max.y >= 0 &&
			min.z <= 0 &&
			max.z >= 0;
	}

	bool Intersects(const vector<AABB>& vec) {
		for(const AABB& aabb : vec) {
			if(this->Intersects(aabb)) return true;
		}
		return false;
	}

	bool IntersectsRay(const Vector3& origin, const Vector3& direction);


	bool IsPointWithin(const Vector3& p) const {
		Vector3 min, max;
		GetMinMax(&min, &max);

		return
			p.x >= min.x &&
			p.x <= max.x &&
			p.y >= min.y &&
			p.y <= max.y &&
			p.z >= min.z &&
			p.z <= max.z;
	}

	static AABB minkowski_difference(const AABB& a, const AABB& b) {
		AABB result = AABB();
		result.center = a.center - b.center;
		result.halfSize = a.halfSize + b.halfSize;

		result.RecalculateMinMax();

		return result;
	}

	static Vector3 penetration_vector(const AABB& minkowski_aabb) {
		Vector3 r;
		Vector3 min, max;
		minkowski_aabb.GetMinMax(&min, &max);

		float min_dist = fabsf(min.x);
		r.x = min.x;
		r.y = 0;
		r.z = 0;

		// Check the X dimension
		if(fabsf(max.x) < min_dist) {
			min_dist = fabsf(max.x);
			r.x = max.x;
		}

		// Check the Y dimension
		if(fabsf(min.y) < min_dist) {
			min_dist = fabsf(min.y);
			r.x = 0;
			r.y = min.y;
			r.z = 0;
		}

		if(fabsf(max.y) < min_dist) {
			min_dist = fabsf(max.y);
			r.x = 0;
			r.y = max.y;
			r.z = 0;
		}

		// Check the Z dimension
		if(fabsf(min.z) < min_dist) {
			min_dist = fabsf(min.z);
			r.x = 0;
			r.y = 0;
			r.z = min.z;
		}

		if(fabsf(max.z) < min_dist) {
			r.x = 0;
			r.y = 0;
			r.z = max.z;
		}
		return r;
	}

	//void PushFrom(const AABB& b) {
	//	//if(min.x < b.max.x && min.x > center.x) min.x = b.max.x;
	//	//if(max.x > b.min.x && max.x < center.x)
	//	if(b.IsPointWithin())

	//	
	//}
};
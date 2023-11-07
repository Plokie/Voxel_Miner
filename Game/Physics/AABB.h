#pragma once

#include "../../Engine/MathUtil.h"

struct AABB {
private:
	Vector3 center = { 0.f, 1.f, 0.f };
	Vector3 halfSize = { 0.5f, 1.f, 0.5f };
	
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

	//void SetMin(const Vector3& v);
	//void SetMax(const Vector3& v);

	//const Vector3& GetMin();
	//const Vector3& GetMax();

	AABB() {
		RecalculateMinMax();
	}
	AABB(const Vector3& center, const Vector3& halfSize) {
		this->center = center;
		this->halfSize = halfSize;
		RecalculateMinMax();
	}

	void SetPosition(const Vector3& pos) {
		this->center = pos;
	}
	void MovePosition(const Vector3& pos) {
		this->center += pos;
	}
	Vector3 GetPosition() const {
		return this->center;
	}


	void GetMinMax(Vector3* min, Vector3* max) const {
		*min = this->min;
		*max = this->max;
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

	bool IsPointWithin(const Vector3& p) {
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

	static const AABB& minkowski_difference(const AABB& a, const AABB& b) {
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
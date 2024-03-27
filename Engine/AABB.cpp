#include "AABB.h"

bool AABB::IntersectsRay(const Vector3& origin, const Vector3& direction) {
	RecalculateMinMax();

	Vector3 invDir = Vector3(1.f / direction.x, 1.f / direction.y, 1.f / direction.z);
	Vector3 tMin = (min - origin) * invDir;
	Vector3 tMax = (max - origin) * invDir;

	Vector3 tEnter = Vector3(fmin(tMin.x, tMax.x), fmin(tMin.y, tMax.y), fmin(tMin.z, tMax.z));
	Vector3 tExit = Vector3(fmax(tMin.x, tMax.x), fmax(tMin.y, tMax.y), fmax(tMin.z, tMax.z));

	float tEnterMax = fmax(fmax(tEnter.x, tEnter.y), tEnter.z);
	float tExitMin = fmin(fmin(tExit.x, tExit.y), tExit.z);

	return tEnterMax < tExitMin;
}

//void AABB::RecalculateCenter()
//{
//	center = (min + max) / 2.f;
//}
//
//void AABB::SetMin(const Vector3& v)
//{
//	this->min = v;
//	RecalculateCenter();
//}
//
//void AABB::SetMax(const Vector3& v)
//{
//	this->max = v;
//	RecalculateCenter();
//}
//
//const Vector3& AABB::GetMin()
//{
//	return this->min;
//}
//
//const Vector3& AABB::GetMax()
//{
//	return this->max;
////}
//
//const Vector3& AABB::GetHalfSize()
//{
//    return this->halfSize;
//}

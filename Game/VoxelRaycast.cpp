#include "VoxelRaycast.h"

#include "ChunkManager.h"

////finds the smallest possible t such that s + t * ds is an integer
//static Vector3 intbound(Vector3 s, Vector3 ds) {
//	Vector3 v = Vector3();
//
//	v.x = (ds.x > 0 ? (ceilf(s.x) - s.x) : (s.x - floorf(s.x))) / fabsf(ds.x);
//	v.y = (ds.y > 0 ? (ceilf(s.y) - s.y) : (s.y - floorf(s.y))) / fabsf(ds.y);
//	v.z = (ds.z > 0 ? (ceilf(s.z) - s.z) : (s.z - floorf(s.z))) / fabsf(ds.z);
//    
//	return v;
//}

bool VoxelRay::Cast(VoxelRay* ray, ChunkManager* chunkManager, float max_dist, Vector3Int* outPosition, BlockID* outBlock, Vector3Int* outNormal)
{
	//Vector3Int currentVoxel = Vector3Int((int)floor(ray->origin.x), (int)floor(ray->origin.y), (int)floor(ray->origin.z));
	//Vector3Int step = Vector3Int(sign(ray->direction), sign(ray->direction), sign(ray->direction));
	//Vector3 tMax = intbound(ray->origin, ray->direction);
	//Vector3Int tDelta;
	float distance_traversed_sqr = 0.0f;
	const float max_dist_sqr = max_dist * max_dist;

	Vector3Int pos = Vector3Int((int)floor(ray->origin.x), (int)floor(ray->origin.y), (int)floor(ray->origin.z));
	const Vector3Int step = Vector3Int(
		(ray->direction.x >0) ? 1 : -1, 
		(ray->direction.y > 0) ? 1 : -1,
		(ray->direction.z > 0) ? 1 : -1
	);

	Vector3 tMax = Vector3(step.x / ray->direction.x, step.y / ray->direction.y, step.z / ray->direction.z);
	const Vector3 tDelta = Vector3(abs(1.0f / ray->direction.x), abs(1.0f / ray->direction.y), abs(1.0f / ray->direction.z));
	Vector3Int normal = Vector3Int(0, 0, 0);

	while(distance_traversed_sqr < max_dist_sqr) {
		BlockID currentBlock = chunkManager->GetBlockAtWorldPos(pos);
		if(currentBlock != AIR) {
			if(outPosition!=nullptr) *outPosition = pos;
			if(outBlock != nullptr) *outBlock = currentBlock;
			if(outNormal != nullptr) *outNormal = normal;
			return true;
		}

		if(tMax.x < tMax.y) {
			if(tMax.x < tMax.z) {
				pos.x += step.x;
				tMax.x += tDelta.x;
				normal = Vector3Int(-step.x, 0, 0);
			}
			else {
				pos.z += step.z;
				tMax.z += tDelta.z;
				normal = Vector3Int(0, 0, -step.z);
			}
		}
		else if(tMax.y < tMax.z) {
			pos.y += step.y;
			tMax.y += tDelta.y;
			normal = Vector3Int(0, -step.y, 0);
		}
		else {
			pos.z += step.z;
			tMax.z += tDelta.z;
			normal = Vector3Int(0, 0, -step.z);
		}

		distance_traversed_sqr = ((pos.x - ray->origin.x) * (pos.x - ray->origin.x)) + ((pos.y - ray->origin.y) * (pos.y - ray->origin.y)) + ((pos.z - ray->origin.z) * (pos.z - ray->origin.z));
	}

	return false;
}

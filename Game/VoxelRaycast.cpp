#include "VoxelRaycast.h"

#include "ChunkManager.h"

/// <summary>
/// Casts a ray of VoxelRay
/// </summary>
/// <param name="ray">: VoxelRay reference. Consists of Origin and Direction</param>
/// <param name="chunkManager">: Reference to ChunkManager to request world data</param>
/// <param name="max_dist">: Max ray reach distance</param>
/// <param name="outPosition">: Out: Voxel Index</param>
/// <param name="outBlock">: Out: Block at Voxel Index</param>
/// <param name="outNormal">: Out: Face normal</param>
/// <returns></returns>
bool VoxelRay::Cast(const VoxelRay* ray, ChunkManager* chunkManager, float max_dist, Vector3Int* outPosition, BlockID* outBlock, Vector3Int* outNormal) {
	const Vector3 _pos1 = ray->origin;
	const Vector3 _pos2 = ray->origin + (ray->direction * max_dist);

	Vector3Int pos = Vector3Int::FloorToInt(_pos1);

	const Vector3Int end = Vector3Int::FloorToInt(_pos2);

	const Vector3Int d = Vector3Int(
		((_pos1.x < _pos2.x) ? 1 : ((_pos1.x > _pos2.x) ? -1 : 0)),
		((_pos1.y < _pos2.y) ? 1 : ((_pos1.y > _pos2.y) ? -1 : 0)),
		((_pos1.z < _pos2.z) ? 1 : ((_pos1.z > _pos2.z) ? -1 : 0))
	);

	const Vector3 deltat = Vector3(
		1.0f / abs(_pos2.x - _pos1.x),
		1.0f / abs(_pos2.y - _pos1.y),
		1.0f / abs(_pos2.z - _pos1.z)
	);

	const Vector3 min = Vector3Int::FloorToInt(_pos1);
	const Vector3 max = min + Vector3(1.f, 1.f, 1.f);

	Vector3 t = Vector3(
		((_pos1.x > _pos2.x) ? (_pos1.x - min.x) : (max.x - _pos1.x)) * deltat.x,
		((_pos1.y > _pos2.y) ? (_pos1.y - min.y) : (max.y - _pos1.y)) * deltat.y,
		((_pos1.z > _pos2.z) ? (_pos1.z - min.z) : (max.z - _pos1.z)) * deltat.z
	);

	Vector3Int normal = Vector3Int(0, 0, 0);

	while(true) {
		BlockID currentBlock = chunkManager->GetBlockAtWorldPos(pos);
		if(currentBlock != AIR && currentBlock != WATER) { // If hit a block that isnt replaceable or air
			const Block& def = BlockDef::GetDef(currentBlock);
			
			if(def.GetBlockShapeID() != BLOCKSHAPE_BLOCK) {
				// perform ray-aabb intersection test with block's custom AABBs
				const BlockShape& blockShape = BlockShape::blockShapes[def.GetBlockShapeID()];
				bool collides = false;
				
				for(AABB aabb : blockShape.GetAABBs()) {
					aabb.MovePosition(pos);
					if(aabb.IntersectsRay(ray->origin, ray->direction)) {
						collides = true;
						break;
					}
				}
				
				if(!collides) //if ray doesnt intersect with block's aabbs
					goto skip; //skip the return and continue
				
			}
			
			// set available output variables 
			if(outPosition != nullptr) *outPosition = pos;
			if(outBlock != nullptr) *outBlock = currentBlock;
			if(outNormal != nullptr) *outNormal = normal;
			return true;

			skip: __nop();
		}

		if(t.x <= t.y && t.x <= t.z) {
			if(pos.x == end.x) break;
			t.x += deltat.x;
			pos.x += d.x;

			normal = Vector3Int(-d.x, 0, 0);
		}
		else if(t.y <= t.z) {
			if(pos.y == end.y) break;
			t.y += deltat.y;
			pos.y += d.y;

			normal = Vector3Int(0, -d.y, 0);
		}
		else {
			if(pos.z == end.z) break;
			t.z += deltat.z;
			pos.z += d.z;

			normal = Vector3Int(0, 0, -d.z);
		}
	}

	return false;
}
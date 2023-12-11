#include "Entity.h"

#include "ChunkManager.h"

#define AABB_RANGE Vector3Int(1,2,1)
vector<AABB> Entity::GetNearbyAABBs(ChunkManager* chunkManager) {
	vector<AABB> ret;

	for(int z = -AABB_RANGE.z; z < AABB_RANGE.z + 1; z++) {
		for(int y = -AABB_RANGE.y; y < AABB_RANGE.y + 1; y++) {
			for(int x = -AABB_RANGE.x; x < AABB_RANGE.x + 1; x++) {
				Vector3 offset = Vector3(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));
				Vector3Int playerBlockPos = Vector3Int::FloorToInt(transform.position);
				Vector3Int blockPos = playerBlockPos + offset;
				BlockID block = chunkManager->GetBlockAtWorldPos(blockPos);
				if(BlockDef::GetDef(block).IsOpaque()) {
					ret.push_back(AABB(Vector3(static_cast<float>(blockPos.x), static_cast<float>(blockPos.y), static_cast<float>(blockPos.z)) + Vector3(0.5f, 0.5f, 0.5f), Vector3(0.5f, 0.5f, 0.5f)));
				}
			}
		}
	}

	return ret;
}

Entity::Entity() {
}

void Entity::Start() {
	chunkManager = Engine::Get()->GetScene("game")->GetObject3D<ChunkManager>("AChunkManager");
}

void Entity::Update(float dTime) {
	vector<AABB> blocks = GetNearbyAABBs(chunkManager);


	bool isGrounded = false;
	//for(const AABB& blockAABB : blocks) {
	//	for(const Vector3& v : groundCheckPoints) {
	//		if(blockAABB.IsPointWithin(transform.position - v)) {
	//			isGrounded = true;
	//			break;
	//		}
	//	}
	//}
	// AABB COLLISION CHECK
	aabb.SetPosition(transform.position);
	for(const AABB& blockAABB : blocks) {
		if(aabb.Intersects(blockAABB)) {
			transform.position += AABB::penetration_vector(AABB::minkowski_difference(blockAABB, aabb));
			aabb.SetPosition(transform.position);
			isGrounded = true;
		}
	}

	if(!isGrounded) {
		velocity.y += gravity * dTime;
		velocity.y = max(velocity.y, terminalVelocity);
	}
	else {
		velocity.y = -3.0f * dTime; //Small nudge to ground level, nothing noticable
	}

	transform.position += velocity * dTime;




}

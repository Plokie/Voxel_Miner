#include "Entity.h"

#include "ChunkManager.h"
#include "PlayerController.h"

#define AABB_RANGE Vector3Int(1,2,1)
// Gets nearby Block AABBs
vector<AABB> Entity::GetNearbyAABBs(ChunkManager* chunkManager) {
	vector<AABB> ret;

	for(int z = -AABB_RANGE.z; z < AABB_RANGE.z + 1; z++) {
		for(int y = -AABB_RANGE.y; y < AABB_RANGE.y + 1; y++) {
			for(int x = -AABB_RANGE.x; x < AABB_RANGE.x + 1; x++) {
				Vector3 offset = Vector3(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));
				Vector3Int playerBlockPos = Vector3Int::FloorToInt(transform.position);
				Vector3Int blockPos = playerBlockPos + offset;
				BlockID block = chunkManager->GetBlockAtWorldPos(blockPos);
				if(BlockDef::GetDef(block).GetDrawType() == B_OPAQUE) {
					ret.push_back(AABB(Vector3(static_cast<float>(blockPos.x), static_cast<float>(blockPos.y), static_cast<float>(blockPos.z)) + Vector3(0.5f, 0.5f, 0.5f), Vector3(0.5f, 0.5f, 0.5f)));
				}
			}
		}
	}

	return ret;
}

void Entity::RecalculateGroundCheck() {
	const Vector3 aabbHalfSize = aabb.GetHalfSize();
	const Vector3 groundCheckBase = Vector3(0, aabbHalfSize.y, 0);
	groundCheckPoints[0] = groundCheckBase + (Vector3(aabbHalfSize.x, 0, aabbHalfSize.z) * 0.95f);
	groundCheckPoints[1] = groundCheckBase + (Vector3(aabbHalfSize.x, 0, -aabbHalfSize.z) * 0.95f);
	groundCheckPoints[2] = groundCheckBase + (Vector3(-aabbHalfSize.x, 0, aabbHalfSize.z) * 0.95f);
	groundCheckPoints[3] = groundCheckBase + (Vector3(-aabbHalfSize.x, 0, -aabbHalfSize.z) * 0.95f);
}

Entity::Entity() {
}

void Entity::Start() {
	chunkManager = Engine::Get()->GetScene("game")->GetObject3D<ChunkManager>("AChunkManager");
	aabb = AABB(Vector3(0.f,0.f,0.f), Vector3(.5f, .5f, .5f));

	RecalculateGroundCheck();

	//todo: entity manager to request nearby entities and test collisions with them
	playerController = Engine::Get()->GetScene("game")->GetObject3D<PlayerController>("PlayerController");
}

void Entity::Update(float dTime) {
	vector<AABB> blocks = GetNearbyAABBs(chunkManager);

	// Basic check if object is grounded
	bool isGrounded = false;
	for(const AABB& blockAABB : blocks) {
		for(const Vector3& v : groundCheckPoints) {
			if(blockAABB.IsPointWithin(transform.position - v)) {
				isGrounded = true;
				break;
			}
		}
	}
	
	// Downward velocity
	if(!isGrounded) {
		velocity.y += gravity * dTime;
		velocity.y = max(velocity.y, terminalVelocity);
	}
	else {
		velocity.y = -3.0f * dTime; //Small nudge to ground level, nothing noticable
	}

	transform.position += velocity * dTime;

	//todo: entity manager collision test instead of direct
	// Entity (player only atm) collision check
	if (aabb.Intersects(playerController->GetAABB())) {
		transform.position += AABB::penetration_vector(AABB::minkowski_difference(playerController->GetAABB(), aabb));
		aabb.SetPosition(transform.position);
	}

	// Block collision check
	aabb.SetPosition(transform.position);
	for(const AABB& blockAABB : blocks) {
		if(aabb.Intersects(blockAABB)) {
			transform.position += AABB::penetration_vector(AABB::minkowski_difference(blockAABB, aabb));
			aabb.SetPosition(transform.position);
		}
	}
}

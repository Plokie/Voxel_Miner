#pragma once

#include "../Engine/Object3D.h"

class ChunkManager;

class Entity : public Object3D {
private:
	const float gravity = -32.f;
	const float terminalVelocity = -78.4f;
	Vector3 velocity;
	AABB aabb;
	ChunkManager* chunkManager = nullptr;



	/*const Vector3 groundCheckPoints[4] = {
		Vector3((playerHalfExtents.x - COLLISION_CHECK_INSET), 1.62f,  (playerHalfExtents.z - COLLISION_CHECK_INSET)),
		Vector3(-(playerHalfExtents.x - COLLISION_CHECK_INSET), 1.62f,  (playerHalfExtents.z - COLLISION_CHECK_INSET)),
		Vector3((playerHalfExtents.x - COLLISION_CHECK_INSET), 1.62f, -(playerHalfExtents.z - COLLISION_CHECK_INSET)),
		Vector3(-(playerHalfExtents.x - COLLISION_CHECK_INSET), 1.62f, -(playerHalfExtents.z - COLLISION_CHECK_INSET)),
	};*/

	vector<AABB> GetNearbyAABBs(ChunkManager* chunkManager);
public:

	Entity();


	void Start() override;
	void Update(float dt) override;
};
#pragma once

#include "../Engine/Engine.h"
#include "../Engine/AABB.h"
#include "Blocks.h"
#include "../Engine/Label.h"

class ChunkManager;

class CameraController : public Object3D {
	//const float gravity = -9.81f;
	const float gravity = -32.f;
	const float terminalVelocity = -78.4f;
	const float jumpHeight = 1.25f;
	const float jumpVelocity = static_cast<float>(sqrt(2.0f * abs(gravity) * jumpHeight));
	const Vector3 playerHalfExtents = Vector3(0.3f, 0.9f, 0.3f);
	Object3D* blockSelectRef;
	Label* fpsCounter;
	Label* worldPosLabel;

#define COLLISION_CHECK_INSET 0.05f


	bool freeCam = false;

	BlockID TEMPcurrentBlockID = GRASS;

	const Vector3 groundCheckPoints[4] = {
		Vector3( (playerHalfExtents.x - COLLISION_CHECK_INSET), 1.62f,  (playerHalfExtents.z - COLLISION_CHECK_INSET)),
		Vector3(-(playerHalfExtents.x - COLLISION_CHECK_INSET), 1.62f,  (playerHalfExtents.z - COLLISION_CHECK_INSET)),
		Vector3( (playerHalfExtents.x - COLLISION_CHECK_INSET), 1.62f, -(playerHalfExtents.z - COLLISION_CHECK_INSET)),
		Vector3(-(playerHalfExtents.x - COLLISION_CHECK_INSET), 1.62f, -(playerHalfExtents.z - COLLISION_CHECK_INSET)),
	};

	Vector3 velocity;
	AABB aabb;

	vector<AABB> GetNearbyAABBs(ChunkManager* chunkManager);

	void Start() override;
	void Update(float dTime) override;
};
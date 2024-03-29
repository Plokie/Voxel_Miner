#pragma once

#include "../Engine/Engine.h"
#include "../Engine/AABB.h"
#include "Blocks.h"

class Label;
class ChunkManager;
class Inventory;
class PlayerData;
class HeldItem;

class PlayerController : public Object3D {
	bool enabled = false;

	//const float gravity = -9.81f;
	const float gravity = -32.f;
	const float terminalVelocity = -78.4f;
	const float liquidTerminalVelocity = -2.f;
	const float jumpHeight = 1.25f;
	const float walkingSpeed = 4.317f;
	const float jumpVelocity = static_cast<float>(sqrt(2.0f * abs(gravity) * jumpHeight));
	const Vector3 playerHalfExtents = Vector3(0.3f, 0.9f, 0.3f);
	Object3D* blockSelectRef = nullptr;
	Object3D* insideBlock = nullptr;
	Label* fpsCounter = nullptr;
	Label* worldPosLabel = nullptr;
	PlayerData* _pCurrentPlayerData = nullptr;
	Object3D* _pHeldItem = nullptr;

	float currentTerminalVelocity = terminalVelocity;

	float timeSinceLastJump = 0.f;
	//Inventory* _pInventory;

	ChunkManager* chunkManager = nullptr;

#define COLLISION_CHECK_INSET 0.08f


	bool freeCam = false;

	const Vector3 groundCheckPoints[4] = {
		Vector3((playerHalfExtents.x - COLLISION_CHECK_INSET), 1.62f,  (playerHalfExtents.z - COLLISION_CHECK_INSET)),
		Vector3(-(playerHalfExtents.x - COLLISION_CHECK_INSET), 1.62f,  (playerHalfExtents.z - COLLISION_CHECK_INSET)),
		Vector3((playerHalfExtents.x - COLLISION_CHECK_INSET), 1.62f, -(playerHalfExtents.z - COLLISION_CHECK_INSET)),
		Vector3(-(playerHalfExtents.x - COLLISION_CHECK_INSET), 1.62f, -(playerHalfExtents.z - COLLISION_CHECK_INSET)),
	};

	Vector3 velocity;
	AABB aabb;

	vector<AABB> GetNearbyAABBs(ChunkManager* chunkManager, vector<AABB>* outDamageAABBs = nullptr, vector<AABB>* outLiquidAABBs = nullptr);

	void Start() override;
	void Update(float dTime) override;
public:

	const AABB& GetAABB() { return aabb; }

	~PlayerController();
};
#pragma once

#include "../Engine/Object3D.h"

class ChunkManager;
class PlayerController;

class Entity : public Object3D {
private:
	const float gravity = -32.f;
	const float terminalVelocity = -78.4f;
	Vector3 velocity = Vector3(0.f,0.f,0.f);
	AABB aabb;
	ChunkManager* chunkManager = nullptr;
	Vector3 groundCheckPoints[4];
	PlayerController* playerController = nullptr;

	vector<AABB> GetNearbyAABBs(ChunkManager* chunkManager);
	void RecalculateGroundCheck();
public:

	Entity();


	void Start() override;
	void Update(float dt) override;
};
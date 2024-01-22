#pragma once

#include "../Engine/Object3D.h"

class ChunkManager;
class PlayerController;

class Entity : public Object3D {
protected:
	const float gravity = -32.f;
	const float terminalVelocity = -78.4f;
	Vector3 velocity = Vector3(0.f,0.f,0.f);
	AABB aabb;
	ChunkManager* chunkManager = nullptr;
	Vector3 groundCheckPoints[4];
	PlayerController* playerController = nullptr;
	float lifetimeCooldown = 0.2f;

	vector<AABB> GetNearbyAABBs(ChunkManager* chunkManager, vector<AABB>* liquidAABBs);
	void RecalculateGroundCheck();
public:

	Entity();

	//todo: base entity class for player to handle collisions as generic events
	virtual void OnCollide(PlayerController* pc);

	//void SetVelocity(float x, float y, float z) {
	//	velocity.x = x;
	//	velocity.y = y;
	//	velocity.z = z;
	//}
	//void SetVelocity(Vector3 vel) { SetVelocity(vel.x, vel.y, vel.z); }

	void Start() override;
	void Update(float dt) override;
};
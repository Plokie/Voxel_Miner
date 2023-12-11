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

	vector<AABB> GetNearbyAABBs(ChunkManager* chunkManager);
public:

	Entity();


	void Start() override;
	void Update(float dt) override;
};
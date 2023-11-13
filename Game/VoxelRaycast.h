#pragma once

#include "Blocks.h"
#include "../Engine/MathUtil.h"

class ChunkManager;

class VoxelRay {
public:
	Vector3 origin, direction;

	VoxelRay(Vector3 Origin, Vector3 Direction) : origin(Origin), direction(Direction) {}

	static bool Cast(const VoxelRay* ray, ChunkManager* chunkManager, float max_dist, Vector3Int* outPosition = nullptr, BlockID* outBlock = nullptr, Vector3Int* outNormal = nullptr);
};
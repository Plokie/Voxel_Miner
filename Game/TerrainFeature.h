#pragma once

#include <map>

#include "../Engine/MathUtil.h"
#include "Blocks.h"

#include "../Engine/AABB.h"

class Chunk;

using namespace std;

class TerrainFeature {
private:
	Vector3Int origin;
	Chunk* parentChunk;
	Vector3Int originChunkIndex;

	float _minBoundX,_minBoundY,_minBoundZ;
	float _maxBoundX,_maxBoundY,_maxBoundZ;

	map<tuple<int, int, int>, BlockID> _localStructure;

	// Registers the placement of a block relative to the terrain feature
	void PutBlock(const Vector3Int& p, const BlockID block);
	// Registers the placement of a block relative to the terrain feature
	void PutBlock(const int x, const int y, const int z, const BlockID block);

	// Creates local instance of terrain feature
	virtual void _GenerateFunc();
public:
	void Generate(Vector3Int origin, Chunk* chunk);

	BlockID PopBlockWorld(const float x, const float y, const float z);

	bool IsPosWithinBounds(const Vector3Int& pos);
	bool IsPosWithinBounds(const float x, const float y, const float z);

	bool IsChunkIndexWithinBounds(const Vector3Int& chunkIndex);
};
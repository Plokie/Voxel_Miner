#pragma once

#include <stack>

#include "../Engine/Engine.h"
#include "../Engine/ThreadPool.h"

class Chunk;
class ChunkManager;

struct LightNode {
	Vector3Int localIndexPos;
	Chunk* chunk;

	LightNode(const Vector3Int& index, Chunk* chunk) :
		localIndexPos(index),
		chunk(chunk) {}

	LightNode(const int& x, const int& y, const int& z, Chunk* chunk) :
		localIndexPos(x, y, z),
		chunk(chunk) {}
};

struct RemoveLightNode {
	Vector3Int localIndexPos;
	Chunk* chunk;
	short val;

	RemoveLightNode(const Vector3Int& localIndexPos, Chunk* chunk, short val) :
		localIndexPos(localIndexPos),
		chunk(chunk),
		val(val) {}
};

/*

Lighting master thread
	Block light can stay mostly the same



*/

class VoxelLighting {
private:
	vector<thread> lightingThreadVec = {};

	ChunkManager* chunkManager = nullptr;
	bool isRunning = true;

	//std::mutex blockQueueMutex = {};
	stack<LightNode> blockLightQueue = {};
	queue<RemoveLightNode> removeBlockLightQueue = {};

	void _Thread();

	void TryFloodBlockLightTo(const Vector3Int& index, const int& currentLevel, Chunk* chunk);
	void TryRemoveBlockLight(const Vector3Int& index, const int& currentLevel, Chunk* chunk);
public:
	VoxelLighting(ChunkManager* chunkManager);
	~VoxelLighting();

	void QueueBlockLight(const LightNode& light);
	void QueueRemoveBlockLight(const RemoveLightNode& remLight);

	void PopBlockLightQueue() {
		if(blockLightQueue.size() > 0) blockLightQueue.pop();
	}

	void StartThread();
};
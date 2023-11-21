#pragma once

#include <Windows.h>
#include <thread>
#include <atomic>
#include <vector>
#include <map>
#include <queue>
#include <stack>
#include "../Engine/MathUtil.h"

using namespace std;

class ChunkManager;
class Chunk;

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

	RemoveLightNode(const Vector3Int& localIndexPos, Chunk* chunk, short val): 
		localIndexPos(localIndexPos),
		chunk(chunk),
		val(val){}
};

class Lighting {
	ChunkManager* chunkManager;
	vector<thread> _lightingThreads = {};
	atomic<bool> _isRunning{ true };
	SRWLOCK lightQueueMutex;
	stack<LightNode> lightBfsQueue = {};
	queue<RemoveLightNode> removeLightBfsQueue = {};

	stack<LightNode> skyLightQueue = {};
	queue<RemoveLightNode> removeSkyLightQueue = {};

	map<Chunk*, bool> chunkIndexRebuildQueue = {};
	queue<Chunk*> newSkyChunks = {};

	void TryFloodLightTo(const Vector3Int& index, const int& currentLevel, Chunk* chunk);
	void TryRemoveLight(const Vector3Int& index, const int& currentLevel, Chunk* chunk);

	void TryFloodSkyLightTo(const Vector3Int& index, const int& currentLevel, Chunk* chunk, bool isDown=false);
	void TryRemoveSkyLight(const Vector3Int& index, const int& currentLevel, Chunk* chunk, bool isDown=false);
public:
	Lighting(ChunkManager* chunkManager);

	void QueueNewChunk(Chunk* chunk);

	void QueueLight(const LightNode& light);
	void QueueSkyLight(const LightNode& light);
	void QueueRemoveLight(const RemoveLightNode& remLight);
	void QueueRemoveSkyLight(const RemoveLightNode& remLight);

	void PopLightQueue() {
		if(lightBfsQueue.size()>0)
			lightBfsQueue.pop();
	}

	void StartThread();

	void LightingThread();

	~Lighting();
};
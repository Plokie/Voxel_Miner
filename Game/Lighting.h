#pragma once

#include <Windows.h>
#include <thread>
#include <atomic>
#include <vector>
#include <map>
#include <queue>
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
	queue<LightNode> lightBfsQueue = {};
	queue<RemoveLightNode> removeLightBfsQueue = {};

	map<Chunk*, bool> chunkIndexRebuildQueue = {};

	void TryFloodLightTo(const Vector3Int& index, const int& currentLevel, Chunk* chunk);
	void TryRemoveLight(const Vector3Int& index, const int& currentLevel, Chunk* chunk);
public:
	Lighting(ChunkManager* chunkManager);

	void QueueLight(const LightNode& light);
	void QueueRemoveLight(const RemoveLightNode& remLight);

	void PopLightQueue() {
		if(lightBfsQueue.size()>0)
			lightBfsQueue.pop();
	}

	void StartThread();

	void LightingThread();

	~Lighting();
};
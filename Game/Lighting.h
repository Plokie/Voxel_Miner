#pragma once

#include <Windows.h>
#include <thread>
#include <atomic>
#include <vector>
#include <queue>
#include "../Engine/MathUtil.h"

using namespace std;

class ChunkManager;
class Chunk;

struct LightNode {
	Vector3Int indexPos;
	Chunk* chunk;

	LightNode(const Vector3Int& index, Chunk* chunk) :
		indexPos(index),
		chunk(chunk) {}

	LightNode(const int& x, const int& y, const int& z, Chunk* chunk) :
		indexPos(x, y, z),
		chunk(chunk) {}
};


class Lighting {
	ChunkManager* chunkManager;
	vector<thread> _lightingThreads = {};
	atomic<bool> _isRunning{ true };
	SRWLOCK lightQueueMutex;
	queue<LightNode> lightBfsQueue = {};

	void TryFloodLightTo(const Vector3Int& worldPos, const int& currentLevel);
public:
	Lighting(ChunkManager* chunkManager);

	void QueueLight(const LightNode& light);

	void StartThread();

	void LightingThread();

	~Lighting();
};
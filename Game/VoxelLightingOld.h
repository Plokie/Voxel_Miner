//#pragma once
//
//#include <stack>
//
//#include "../Engine/Engine.h"
//#include "../Engine/ThreadPool.h"
//
//class Chunk;
//class ChunkManager;
//
//struct LightNode {
//	Vector3Int localIndexPos;
//	Chunk* chunk;
//
//	LightNode(const Vector3Int& index, Chunk* chunk) :
//		localIndexPos(index),
//		chunk(chunk) {}
//
//	LightNode(const int& x, const int& y, const int& z, Chunk* chunk) :
//		localIndexPos(x, y, z),
//		chunk(chunk) {}
//};
//
//struct RemoveLightNode {
//	Vector3Int localIndexPos;
//	Chunk* chunk;
//	short val;
//
//	RemoveLightNode(const Vector3Int& localIndexPos, Chunk* chunk, short val) :
//		localIndexPos(localIndexPos),
//		chunk(chunk),
//		val(val) {}
//};
//
///*
//
//Lighting master thread
//	Block light can stay mostly the same
//
//
//
//*/
//
//class VoxelLighting {
//private:
//	vector<thread> lightingThreadVec = {};
//
//	ChunkManager* chunkManager = nullptr;
//	bool isRunning = true;
//
//	stack<LightNode> blockLightQueue = {};
//	queue<RemoveLightNode> removeBlockLightQueue = {};
//
//	stack<LightNode> skyLightQueue = {};
//	queue<RemoveLightNode> removeSkyLightQueue = {};
//
//	void _ThreadBlock();
//	void _ThreadSky();
//
//	void TryFloodBlockLightTo(const Vector3Int& index, const int& currentLevel, Chunk* chunk);
//	void TryRemoveBlockLight(const Vector3Int& index, const int& currentLevel, Chunk* chunk);
//
//	void TryFloodSkyLightTo(const Vector3Int& index, const int& currentLevel, Chunk* chunk, bool isDown=false);
//	void TryRemoveSkyLight(const Vector3Int& index, const int& currentLevel, Chunk* chunk);
//public:
//	std::mutex skyLightQueueMutex = {};
//
//	VoxelLighting(ChunkManager* chunkManager);
//	~VoxelLighting();
//
//	void QueueBlockLight(const LightNode& light);
//	void QueueRemoveBlockLight(const RemoveLightNode& remLight);
//
//	void QueueSkyLight(const LightNode& light);
//	void QueueRemoveSkyLight(const RemoveLightNode& remLight);
//
//	void PopBlockLightQueue() {
//		if(blockLightQueue.size() > 0) blockLightQueue.pop();
//	}
//
//	//void StartThread();
//};
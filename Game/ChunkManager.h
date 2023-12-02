#pragma once

#include <map>
#include <queue>

#include "Blocks.h"
#include "../Engine/Engine.h"
#include "../Engine/ThreadPool.h"

#define CHUNKLOAD_AREA_X 6
#define CHUNKLOAD_AREA_Z 6
#define CHUNKLOAD_FIXED_NY 6
#define CHUNKLOAD_FIXED_PY 4

class Chunk;

class ChunkManager : public Object3D {
private:
	Engine* engine;
	Transform* camTrans;

	ThreadPool* threadPool;

	map<tuple<int, int, int>, Chunk*> chunkMap;

	std::mutex newChunkQueueMutex;
	queue<Chunk*> newChunkQueue;

	std::mutex regenQueueMutex;
	queue<Chunk*> regenQueue;

	vector<thread> chnkMgrThread;

	bool isRunning = true; //well then you better go catch it
public:
	void CreateChunk(const int x, const int y, const int z);

	void Thread();

	//Lighting* GetLighting() const {
	//	return this->lighting;
	//}

	Chunk* GetChunk(tuple<int, int, int> index) {
		if(chunkMap.find(index) != chunkMap.end()) {
			return chunkMap[index];
		}
		return nullptr;
	}
	Chunk* GetChunk(Vector3Int index) {
		if(chunkMap.find(index) != chunkMap.end()) {
			return chunkMap[index];
		}
		return nullptr;
	}

	map<tuple<int, int, int>, Chunk*>& GetChunkMap() {
		return chunkMap;
	}


	// long name because really shouldnt use this in most cases, but it /does/ have its use
	static Vector3Int ChunkFloorPosForPositionCalculation(Vector3 worldPosition);
	static tuple<int, int, int> ToChunkIndexPositionTuple(const int& x, const int& y, const int& z);

	BlockID GetBlockAtWorldPos(const int& x, const int& y, const int& z);
	BlockID GetBlockAtWorldPos(const Vector3Int& v);
	void SetBlockAtWorldPos(const int& x, const int& y, const int& z, const BlockID& id);
	void SetBlockAtWorldPos(const Vector3Int& pos, const BlockID& id);

	int GetBlockLightAtWorldPos(const int& x, const int& y, const int& z) const;
	int GetBlockLightAtWorldPos(const Vector3Int& p) const;
	void SetBlockLightAtWorldPos(const int& x, const int& y, const int& z, const int& val) const;
	void SetBlockLightAtWorldPos(const Vector3Int& p, const int& val) const;

	int GetSkyLightAtWorldPos(const int& x, const int& y, const int& z);
	int GetSkyLightAtWorldPos(const Vector3Int& p);
	void SetSkyLightAtWorldPos(const int& x, const int& y, const int& z, const int& val);
	void SetSkyLightAtWorldPos(const Vector3Int& p, const int& val);

	short GetRawLightAtWorldPos(const int& x, const int& y, const int& z);

	void QueueRegen(Chunk*);
	void QueueRegen(const Vector3Int&);

	void Update(float dTime) override;
	void Start() override;

	~ChunkManager();
};
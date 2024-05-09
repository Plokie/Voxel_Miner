#pragma once

#include <map>
#include <queue>

#include "Blocks.h"
#include "../Engine/Engine.h"
#include "../Engine/ThreadPool.h"
#include "ChunkGenerationPhase.h"

// faster to load while debugging. plus debug builds are way slower
#ifdef _DEBUG
#define CHUNKLOAD_AREA_X 5
#define CHUNKLOAD_AREA_Z 5
#else
#define CHUNKLOAD_AREA_X 12
#define CHUNKLOAD_AREA_Z 12
#endif


#define CHUNKLOAD_FIXED_NY 10
#define CHUNKLOAD_FIXED_PY 4

constexpr int minLoadCount = CHUNKLOAD_AREA_X * CHUNKLOAD_AREA_Z * (CHUNKLOAD_FIXED_PY + CHUNKLOAD_FIXED_NY);

class Chunk;
class Camera;

class ChunkManager : public Object3D {
private:
	Engine* engine = nullptr;
	//Transform* camTrans = nullptr;
	Camera* cam = nullptr;

	ThreadPool* threadPool = nullptr;

	std::mutex chunkMapMutex;
	map<tuple<int, int, int>, Chunk*> chunkMap;

	//std::mutex newChunkQueueMutex;
	//queue<Chunk*> newChunkQueue;

	bool CanServiceQueue(GENERATION_PHASE phase);


	//std::mutex newChunk_BlockDataPhase

	map<GENERATION_PHASE, pair<std::mutex, queue<Chunk*>>> chunkGenerationQueues;

	std::mutex regenQueueMutex;
	queue<pair<Chunk*,int>> regenQueue;

	vector<thread> chnkMgrThread;

	bool isRunning = true; //well then you better go catch it

	mutex _numChunksLoadedMutex;
	int _numChunksLoaded = 0;
public:
	void CreateChunk(const int x, const int y, const int z);

	void Thread();

	const float LoadMinAreaProgress() const { return _numChunksLoaded / (float)minLoadCount; }
	const bool HasLoadedMinArea() const { return _numChunksLoaded > minLoadCount; }
	const int TotalChunksLoaded() const { return _numChunksLoaded; }

	void QueueChunkToGenerationPhase(Chunk* chunk, GENERATION_PHASE phase);

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

	BlockID GetBlockAtWorldPos(const int& x, const int& y, const int& z, bool worldgenFallback = true);
	BlockID GetBlockAtWorldPos(const Vector3Int& v, bool worldgenFallback = true);
	void SetBlockAtWorldPos(const int& x, const int& y, const int& z, const BlockID& id);
	void SetBlockAtWorldPos(const Vector3Int& pos, const BlockID& id);

	int GetBlockLightAtWorldPos(const int& x, const int& y, const int& z);
	int GetBlockLightAtWorldPos(const Vector3Int& p);
	void SetBlockLightAtWorldPos(const int& x, const int& y, const int& z, const int& val) const;
	void SetBlockLightAtWorldPos(const Vector3Int& p, const int& val) const;

	int GetSkyLightAtWorldPos(const int& x, const int& y, const int& z);
	int GetSkyLightAtWorldPos(const Vector3Int& p);
	void SetSkyLightAtWorldPos(const int& x, const int& y, const int& z, const int& val, bool update = true);
	void SetSkyLightAtWorldPos(const Vector3Int& p, const int& val, bool update=true);

	short GetRawLightAtWorldPos(const int& x, const int& y, const int& z);

	void QueueRegen(Chunk*, int priority=0);
	void QueueRegen(const Vector3Int&, int priority=0);

	void Update(float dTime) override;
	void Start() override;

	~ChunkManager();
};
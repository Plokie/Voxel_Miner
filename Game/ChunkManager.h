#pragma once

#include <vector>
#include <map>
#include <tuple>
#include <thread>
#include <atomic>
#include <stack>
#include <queue>

#include "../Engine/Engine.h"
#include "Chunk.h"
#include "WorldGen.h"
#include "../Engine/MathUtil.h"
#include "Lighting.h"
#include "../Engine/ThreadPool.h"


#define CHUNKLOAD_AREA_X 5
#define CHUNKLOAD_AREA_NY 3
#define CHUNKLOAD_AREA_PY 4
#define CHUNKLOAD_AREA_Z 5

#define CHUNKLOAD_FIXED_NY 6
#define CHUNKLOAD_FIXED_PY 4

using namespace std;

class ChunkManager : public Object3D {
private:
	Transform* pCameraTransform = nullptr;
	Engine* pEngine = nullptr;

	map<tuple<int, int, int>, Chunk*> chunkMap = {};

	Chunk* CreateChunk(int x, int y, int z);

	vector<thread> _chunkLoaderThreads = {};

	ThreadPool meshBuilderPool;

	atomic<bool> _isRunning{true};

	Lighting* lighting = nullptr;

	// To use less Mutexes and better performance, could just delete the queue from the chunkMap
	//SRWLOCK _rebuildQueueMutex; // Permission to push / pop from rebuild queue
	//atomic<vector<Chunk*>> _rebuildQueue{ {} }; // List of chunks that are awaiting re-build on the chunk builder thread

	SRWLOCK rebuildQueueMutex;
	stack<Chunk*> rebuildQueue;
public:

	Lighting* GetLighting() const {
		return this->lighting;
	}

	Chunk* GetChunk(tuple<int,int,int> index) {
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


	void TryRegen(Vector3Int chunkCoords);
	void TryRegen(Chunk* chunk);
	// long name because really shouldnt use this in most cases, but it /does/ have its use
	static Vector3Int ChunkFloorPosForPositionCalculation(Vector3 worldPosition);
	static Vector3Int ToChunkIndexPosition(const int& x, const int& y, const int& z);

	// Used because faster when operating with
	static tuple<int,int,int> ToChunkIndexPositionTuple(const int& x, const int& y, const int& z);

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

	short GetRawLightAtWorldPos(const int& x, const int& y, const int& z) const;

	static ChunkManager* Create(Transform* cameraTransform);
	void Init(Transform* cameraTransform);

	void LoaderThreadFunc(Transform* camTransform, map<tuple<int, int, int>, Chunk*>* pChunkMap);

	void Start() override;
	void Update(float) override;

	~ChunkManager();
};

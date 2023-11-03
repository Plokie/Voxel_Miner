#pragma once

#include <vector>
#include <map>
#include <tuple>
#include <thread>
#include <atomic>

#include "../Engine/Engine.h"
#include "Chunk.h"
#include "WorldGen.h"

#define CHUNKLOAD_AREA_X 3
#define CHUNKLOAD_AREA_Y 3
#define CHUNKLOAD_AREA_Z 3

using namespace std;

class ChunkManager : public Object3D {
private:
	Transform* pCameraTransform = nullptr;
	Engine* pEngine = nullptr;

	map<tuple<int, int, int>, Chunk*> chunkMap = {};

	void CreateChunk(int x, int y, int z);

	vector<thread> _chunkLoaderThreads = {};

	atomic<bool> _isRunning{true};

public:
	static const Vector3Int& WorldToIndexPosition(Vector3 worldPosition);

	BlockID GetBlockAtWorldPos(int x, int y, int z);

	static ChunkManager* Create(Transform* cameraTransform);
	void Init(Transform* cameraTransform);

	void LoaderThreadFunc(Transform* camTransform, map<tuple<int, int, int>, Chunk*>* pChunkMap);

	void Start() override;
	void Update(float) override;

	~ChunkManager();
};

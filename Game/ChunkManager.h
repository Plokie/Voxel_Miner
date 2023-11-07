#pragma once

#include <vector>
#include <map>
#include <tuple>
#include <thread>
#include <atomic>

#include "../Engine/Engine.h"
#include "Chunk.h"
#include "WorldGen.h"
#include "../Engine/MathUtil.h"

#define CHUNKLOAD_AREA_X 5
#define CHUNKLOAD_AREA_Y 3
#define CHUNKLOAD_AREA_Z 5

using namespace std;

class ChunkManager : public Object3D {
private:
	Transform* pCameraTransform = nullptr;
	Engine* pEngine = nullptr;

	map<tuple<int, int, int>, Chunk*> chunkMap = {};

	Chunk* CreateChunk(int x, int y, int z);

	vector<thread> _chunkLoaderThreads = {};

	atomic<bool> _isRunning{true};

public:
	// long name because really shouldnt use this in most cases, but it /does/ have its use
	static Vector3Int ChunkFloorPosForPositionCalculation(Vector3 worldPosition);
	static Vector3Int ToChunkIndexPosition(const int& x, const int& y, const int& z);
	
	BlockID GetBlockAtWorldPos(int x, int y, int z);
	BlockID GetBlockAtWorldPos(Vector3Int v);

	static ChunkManager* Create(Transform* cameraTransform);
	void Init(Transform* cameraTransform);

	void LoaderThreadFunc(Transform* camTransform, map<tuple<int, int, int>, Chunk*>* pChunkMap);

	void Start() override;
	void Update(float) override;

	~ChunkManager();
};

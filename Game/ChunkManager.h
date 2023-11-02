#pragma once

#include <vector>
#include <map>
#include <tuple>

#include "../Engine/Engine.h"
#include "Chunk.h"

#define CHUNKLOAD_AREA 4

using namespace std;

class ChunkManager : public Object3D {
private:
	Transform* pCameraTransform = nullptr;
	Engine* pEngine = nullptr;

	map<tuple<int, int, int>, Chunk*> chunkMap = {};

	void CreateChunk(int x, int y, int z);
public:
	static const Vector3Int& WorldToIndexPosition(Vector3 worldPosition);

	static ChunkManager* Create(Transform* cameraTransform);
	void Init(Transform* cameraTransform);

	void Start() override;
	void Update(float) override;

	~ChunkManager();
};

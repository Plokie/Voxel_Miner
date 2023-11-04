#pragma once

#include "../Engine/Engine.h"
#include "Blocks.h"
#include "WorldGen.h"

#define CHUNKSIZE_X 32
#define CHUNKSIZE_Y 32
#define CHUNKSIZE_Z 32

using namespace std;

class ChunkManager;

class Chunk : public Object3D {
private:
	void PushChunkMesh(vector<Vertex>& vertices, vector<DWORD>& indices, bool isTransparent = false);
	bool RenderBlockFaceAgainst(BlockID currentBlock, const int x, const int y, const int z);
	void MakeVoxel(const BlockID blockID, const int x, const int y, const int z, vector<Vertex>& vertices, vector<DWORD>& indices);
	ChunkManager* chunkManager = nullptr;
public:
	Vector3Int chunkIndexPosition = Vector3Int();

	USHORT blockData[CHUNKSIZE_X][CHUNKSIZE_Y][CHUNKSIZE_Z] = {};

	void BuildMesh();

	void Load();

	void Start() override;
	void Update(float dTime) override;

	Chunk(Vector3Int ChunkIndexPos, ChunkManager* chnkMgr): chunkIndexPosition(ChunkIndexPos), chunkManager(chnkMgr){}

	void Release();

	//~Chunk() {
	//	//delete[CHUNKSIZE_X * CHUNKSIZE_Y * CHUNKSIZE_Z] blockData;

	//	/*for(Model* model : models) {
	//		model->~Model();
	//	}*/
	//}
};
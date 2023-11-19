#pragma once

#include <queue>
#include "../Engine/Engine.h"
#include "Blocks.h"
#include "WorldGen.h"

#define CHUNKSIZE_X 16
#define CHUNKSIZE_Y 16
#define CHUNKSIZE_Z 16

using namespace std;

class ChunkManager;
class ChunkLightData;

class Chunk : public Object3D {
public:
	enum MESHFLAG {
		SOLID,
		TRANS,
		WATER,
		SHELL
	};
private:
	void PushChunkMesh(vector<Vertex>& vertices, vector<DWORD>& indices, MESHFLAG isTransparent = SOLID);
	bool RenderBlockFaceAgainst(BlockID currentBlock, const int x, const int y, const int z);
	void MakeVoxel(const BlockID blockID, const int x, const int y, const int z, vector<Vertex>& vertices, vector<DWORD>& indices);
	ChunkManager* chunkManager = nullptr;
	Object3D* player = nullptr;
	UINT8 lightLevel[CHUNKSIZE_X][CHUNKSIZE_Y][CHUNKSIZE_Z] = {};
public:

	Vector3Int chunkIndexPosition = Vector3Int();

	USHORT blockData[CHUNKSIZE_X][CHUNKSIZE_Y][CHUNKSIZE_Z] = {};

	void BuildMesh();

	/// <summary>
	/// 
	/// </summary>
	/// <returns>If the chunk had pre-existing data loaded into it</returns>
	bool Load();

	void Start() override;
	void Update(float dTime) override;

	int GetBlockLight(const int& x, const int& y, const int& z);
	int GetSkyLight(const int& x, const int& y, const int& z);
	int GetBlockLightIncludingNeighbours(const int& x, const int& y, const int& z);
	void SetBlockLightIncludingNeighbours(const int& x, const int& y, const int& z, const int& val);

	void CorrectIndexForNeighbours(const int& x, const int& y, const int& z, Chunk** outChunk, Vector3Int* outIndex);
	void CorrectIndexForNeighbours(const Vector3Int& index, Chunk** outChunk, Vector3Int* outIndex);

	BlockID GetBlockIncludingNeighbours(const int& x, const int& y, const int& z);

	void SetBlockLight(const int& x, const int& y, const int& z, const int& val);
	void SetBlockLightNoUpdate(const int& x, const int& y, const int& z, const int& val);
	void SetSkyLight(const int& x, const int& y, const int& z, const int& val);

	Chunk(Vector3Int ChunkIndexPos, ChunkManager* chnkMgr): chunkIndexPosition(ChunkIndexPos), chunkManager(chnkMgr){}

	void Release();

	~Chunk() {

		for(Model*& model : models) {
			//model->~Model();
			delete model;
			model = nullptr;
		}
		models.clear();
	}
};
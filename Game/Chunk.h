#pragma once

#include "../Engine/Engine.h"
#include "Blocks.h"
//#include "BlockData.h"
#include "ChunkGenerationPhase.h"

#define CHUNKSIZE_X 16
#define CHUNKSIZE_Y 16
#define CHUNKSIZE_Z 16

class ChunkManager;
class BlockData;
class Chunk;

struct LightNode {
	Vector3Int index;
	Chunk* owner;

	LightNode(){}
	LightNode(Vector3Int index, Chunk* owner) : index(index), owner(owner){}
};

class Chunk : public Object3D {
public:
	

private:
	ChunkManager* chunkManager = nullptr;

	UINT8 lightLevel[CHUNKSIZE_X][CHUNKSIZE_Y][CHUNKSIZE_Z] = {};

	unsigned int fillIndices[CHUNKSIZE_X][CHUNKSIZE_Y][CHUNKSIZE_Z] = {};
	unsigned int currentFillindex = 1;

	queue<LightNode> lightBlockQueue = {};
	queue<LightNode> lightSunQueue = {};


	const float secondsPerTick = 0.05f;
	float tickTimer = 0.f;
	bool busy = false;


	void PushChunkMesh(vector<Vertex>& vertices, vector<DWORD>& indices, MESHFLAG isTransparent = SOLID);
	bool RenderBlockFaceAgainst(BlockID currentBlock, Vector3Int pos, Vector3Int dir);
	void PushBlockShapeFaces(const BlockID blockID, const Vector3Int& pos, const Vector3Int& lightSampleDir, const vector<BlockFace>& faces, vector<Vertex>& vertices, vector<DWORD>& indices);
	void MakeVoxel(const BlockID blockID, const int x, const int y, const int z, vector<Vertex>& vertices, vector<DWORD>& indices);
	void CorrectIndexForNeighbours(const int& x, const int& y, const int& z, Chunk** outChunk, Vector3Int* outIndex);
	void CorrectIndexForNeighbours(const Vector3Int& index, Chunk** outChunk, Vector3Int* outIndex);

	void TryFloodFillGraph(int x, int y, int z);
	//Vector3Int LocalToWorld(const int x, const int y, const int z);
public:

	bool IsBusy() const { return busy; }
	void SetBusy(bool busy) { this->busy = busy; }
	// input direction :: hash map of output directions visible from input direction
	map<tuple<int, int, int>, map<tuple<int, int, int>,UINT8>> visibilityGraph;
	//bool hasLoadedBlockData = false;

	GENERATION_PHASE currentGenerationPhase = BLOCKDATA;

	void GenerateBlockData();
	void BuildVisibilityGraph();
	void BuildMesh();

	bool IsChunkVisibleFromChunk(Vector3Int tryFindThisChunk, Vector3Int fromChunk);

	void Finalize();

	Vector3Int indexPosition = { 0,0,0 };
	BlockID blockData[CHUNKSIZE_X][CHUNKSIZE_Y][CHUNKSIZE_Z] = {};
	map<tuple<int, int, int>, BlockData*> blockDataData; // bad name

	// Lighting
	int GetBlockLight(const int& x, const int& y, const int& z);
	int GetBlockLightIncludingNeighbours(const int& x, const int& y, const int& z);
	void SetBlockLightIncludingNeighbours(const int& x, const int& y, const int& z, const int& val);

	int GetSkyLight(const int& x, const int& y, const int& z);
	int GetSkyLightIncludingNeighbours(const int& x, const int& y, const int& z);
	void SetSkyLightIncludingNeighbours(const int& x, const int& y, const int& z, const int& val, bool update=true);

	short GetRawLightIncludingNeighbours(const int& x, const int& y, const int& z);
	short GetRawLight(const int& x, const int& y, const int& z);

	void SetBlockLight(const int& x, const int& y, const int& z, const int& val);
	void SetBlockLightNoUpdate(const int& x, const int& y, const int& z, const int& val);
	void SetSkyLight(const int& x, const int& y, const int& z, const int& val, bool update=true);

	void InitSkyLight();

	void RecalculateBlockLighting(bool rootChunk=true);
	void RecalculateSkyLighting();


	// World 
	BlockID GetBlockIncludingNeighbours(const int& x, const int& y, const int& z);



	void Start() override;
	void Update(float dTime) override;

	Chunk(Vector3Int ChunkIndexPos, ChunkManager* chnkMgr) : Object3D(), indexPosition(ChunkIndexPos), chunkManager(chnkMgr) {}
	~Chunk();
};
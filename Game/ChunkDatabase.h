#pragma once

#include "../Engine/Engine.h"
#include <string>
#include <sstream>

#include "Blocks.h"
#include "ChunkManager.h"
#include "Chunk.h"
#include <iostream>
#include <map>
#include <fstream>
#include "../Engine/json.hpp"

using namespace std;

//fwd dec
class Chunk;

class WorldData {
	string name;
	vector<Vector3Int> hash;
};

class ChunkDatabase {
private:
	//Data of currently-loaded chunks (reduce number of disk read/writes for active chunks)
	// Data is to be saved into files when a chunk is unloaded from this
	// This is to be written into when a chunk with data on file is loaded
	//vector<USHORT*> runtimeChunkData = {}; //todo: replace void* with serializable ChunkData* struct/class

	//todo: create event that is called when a chunk is unloaded


	string worldName = "World";

	void SaveChunkIntoFile(const Vector3Int& chunkIndex, BlockID chunkDataArray[CHUNKSIZE_X][CHUNKSIZE_Y][CHUNKSIZE_Z]);
	void LoadChunkFromFile(const Vector3Int& chunkIndex, BlockID chunkDataArray[CHUNKSIZE_X][CHUNKSIZE_Y][CHUNKSIZE_Z]);

	void TryLoadChunkHash();

	bool hasLoadedChunkHash = false;

	static ChunkDatabase* _Instance;
public:
	SRWLOCK chunkHashMutex = {};
	map<tuple<int, int, int>, Chunk*> chunkHash = {}; // todo: move back to private

	ChunkDatabase();
	~ChunkDatabase();

	static ChunkDatabase* Get();

	static void Init();

	void SaveWorldData();

	void SaveChunks();

	void UnloadChunk(const Vector3Int& chunkIndex);

	/// <summary>
	/// Checks if a chunk is indexed within a world
	/// </summary>
	/// <param name="worldName">Name of world</param>
	/// <param name="chunkIndex">Position index of chunk</param>
	/// <returns></returns>
	bool DoesDataExistForChunk(const Vector3Int& chunkIndex);

	/// <summary>
	/// Loads data from database into a Chunk's data array
	/// </summary>
	/// <param name="worldName">Name of world to read from</param>
	/// <param name="chunkIndex">Position index of chunk</param>
	/// <param name="chunkDataArray">Array to load chunk data into</param>
	void LoadChunkDataInto(const Vector3Int& chunkIndex, Chunk* chunk);

	/// <summary>
	/// Saves chunk data into database
	/// </summary>
	/// <param name="worldName"></param>
	/// <param name="chunkIndex"></param>
	/// <param name="chunkDataArray"></param>
	void SaveChunkData(const Vector3Int& chunkIndex, Chunk* chunk);
};
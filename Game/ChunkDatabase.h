#pragma once

#include "../Engine/Engine.h"
#include <string>

#include "Blocks.h"
#include "ChunkManager.h"
#include "Chunk.h"

//fwd dec
class Chunk;

class ChunkDatabase {
private:

	//Data of currently-loaded chunks (reduce number of disk read/writes for active chunks)
	// Data is to be saved into files when a chunk is unloaded from this
	// This is to be written into when a chunk with data on file is loaded
	vector<void*> runtimeChunkData = {}; //todo: replace void* with serializable ChunkData* struct/class

	//todo: create event that is called when a chunk is unloaded

	static ChunkDatabase* _Instance;
public:
	ChunkDatabase();

	ChunkDatabase* Get();

	/// <summary>
	/// Checks if a chunk is indexed within a world
	/// </summary>
	/// <param name="worldName">Name of world</param>
	/// <param name="chunkIndex">Position index of chunk</param>
	/// <returns></returns>
	bool DoesDataExistForChunk(const string& worldName, const Vector3Int& chunkIndex);

	/// <summary>
	/// Loads data from database into a Chunk's data array
	/// </summary>
	/// <param name="worldName">Name of world to read from</param>
	/// <param name="chunkIndex">Position index of chunk</param>
	/// <param name="chunkData">Array to load chunk data into</param>
	void LoadChunkDataInto(const string& worldName, const Vector3Int& chunkIndex, USHORT chunkData[CHUNKSIZE_X][CHUNKSIZE_Y][CHUNKSIZE_Z]);

	/// <summary>
	/// Saves chunk data into database
	/// </summary>
	/// <param name="worldName"></param>
	/// <param name="chunkIndex"></param>
	/// <param name="chunkData"></param>
	void SaveChunkData(const string& worldName, const Vector3Int& chunkIndex, USHORT chunkData[CHUNKSIZE_X][CHUNKSIZE_Y][CHUNKSIZE_Z]);
};
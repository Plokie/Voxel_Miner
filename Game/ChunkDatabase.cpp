#include "ChunkDatabase.h"

ChunkDatabase* ChunkDatabase::_Instance;

ChunkDatabase::ChunkDatabase()
{
}

ChunkDatabase* ChunkDatabase::Get()
{
	return this->_Instance;
}

bool ChunkDatabase::DoesDataExistForChunk(const string& worldName, const Vector3Int& chunkIndex)
{
	return false;
}

void ChunkDatabase::LoadChunkDataInto(const string& worldName, const Vector3Int& chunkIndex, USHORT chunkData[CHUNKSIZE_X][CHUNKSIZE_Y][CHUNKSIZE_Z])
{
}

void ChunkDatabase::SaveChunkData(const string& worldName, const Vector3Int& chunkIndex, USHORT chunkData[CHUNKSIZE_X][CHUNKSIZE_Y][CHUNKSIZE_Z])
{
}

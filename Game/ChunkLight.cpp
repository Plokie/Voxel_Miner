#include "Chunk.h"

#include "ChunkManager.h"

int Chunk::GetBlockLight(const int& x, const int& y, const int& z)
{
	return this->lightLevel[x][y][z] & 0x0F;
}

int Chunk::GetBlockLightIncludingNeighbours(const int& x, const int& y, const int& z)
{
	if(x < 0 || x>CHUNKSIZE_X - 1 || y < 0 || y>CHUNKSIZE_Y - 1 || z < 0 || z>CHUNKSIZE_Z - 1) // sample from another chunk
	{
		Vector3Int chunkPosition = Vector3Int(indexPosition.x * CHUNKSIZE_X, indexPosition.y * CHUNKSIZE_Y, indexPosition.z * CHUNKSIZE_Z);
		return chunkManager->GetBlockLightAtWorldPos(x + chunkPosition.x, y + chunkPosition.y, z + chunkPosition.z);
	}
	else {
		return GetBlockLight(x, y, z);
	}
}

void Chunk::SetBlockLightIncludingNeighbours(const int& x, const int& y, const int& z, const int& val)
{
	if(x < 0 || x>CHUNKSIZE_X - 1 || y < 0 || y>CHUNKSIZE_Y - 1 || z < 0 || z>CHUNKSIZE_Z - 1)
	{
		Vector3Int chunkPosition = Vector3Int(indexPosition.x * CHUNKSIZE_X, indexPosition.y * CHUNKSIZE_Y, indexPosition.z * CHUNKSIZE_Z);
		chunkManager->SetBlockLightAtWorldPos(x + chunkPosition.x, y + chunkPosition.y, z + chunkPosition.z, val);
	}
	else {
		SetBlockLight(x, y, z, val);
	}
}

void Chunk::SetBlockLight(const int& x, const int& y, const int& z, const int& val)
{
	this->lightLevel[x][y][z] = (this->lightLevel[x][y][z] & 0xF0) | val;

	//chunkManager->GetLighting()->QueueLight(LightNode(x, y, z, this));
}

void Chunk::SetBlockLightNoUpdate(const int& x, const int& y, const int& z, const int& val) {
	this->lightLevel[x][y][z] = (this->lightLevel[x][y][z] & 0xF0) | val;
}

int Chunk::GetSkyLight(const int& x, const int& y, const int& z)
{
	return (this->lightLevel[x][y][z] & 0xF0) >> 4;
}

int Chunk::GetSkyLightIncludingNeighbours(const int& x, const int& y, const int& z)
{
	if(x < 0 || x>CHUNKSIZE_X - 1 || y < 0 || y>CHUNKSIZE_Y - 1 || z < 0 || z>CHUNKSIZE_Z - 1) // sample from another chunk
	{
		Vector3Int chunkPosition = Vector3Int(indexPosition.x * CHUNKSIZE_X, indexPosition.y * CHUNKSIZE_Y, indexPosition.z * CHUNKSIZE_Z);
		return chunkManager->GetSkyLightAtWorldPos(x + chunkPosition.x, y + chunkPosition.y, z + chunkPosition.z);
	}
	else {
		return GetSkyLight(x, y, z);
	}
}

void Chunk::SetSkyLightIncludingNeighbours(const int& x, const int& y, const int& z, const int& val)
{
	if(x < 0 || x>CHUNKSIZE_X - 1 || y < 0 || y>CHUNKSIZE_Y - 1 || z < 0 || z>CHUNKSIZE_Z - 1)
	{
		Vector3Int chunkPosition = Vector3Int(indexPosition.x * CHUNKSIZE_X, indexPosition.y * CHUNKSIZE_Y, indexPosition.z * CHUNKSIZE_Z);
		chunkManager->SetSkyLightAtWorldPos(x + chunkPosition.x, y + chunkPosition.y, z + chunkPosition.z, val);
	}
	else {
		SetSkyLight(x, y, z, val);
	}
}

short Chunk::GetRawLightIncludingNeighbours(const int& x, const int& y, const int& z)
{
	if(x < 0 || x>CHUNKSIZE_X - 1 || y < 0 || y>CHUNKSIZE_Y - 1 || z < 0 || z>CHUNKSIZE_Z - 1) // sample from another chunk
	{
		Vector3Int chunkPosition = Vector3Int(indexPosition.x * CHUNKSIZE_X, indexPosition.y * CHUNKSIZE_Y, indexPosition.z * CHUNKSIZE_Z);
		return chunkManager->GetRawLightAtWorldPos(x + chunkPosition.x, y + chunkPosition.y, z + chunkPosition.z);
	}
	else {
		return GetRawLight(x, y, z);
	}
}

short Chunk::GetRawLight(const int& x, const int& y, const int& z)
{
	return this->lightLevel[x][y][z];
}


void Chunk::SetSkyLight(const int& x, const int& y, const int& z, const int& val)
{
	this->lightLevel[x][y][z] = (this->lightLevel[x][y][z] & 0xF) | (val << 4);

	//chunkManager->GetLighting()->QueueSkyLight(LightNode(x, y, z, this));
}
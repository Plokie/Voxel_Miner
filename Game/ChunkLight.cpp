#include "Chunk.h"

#include "ChunkManager.h"
#include "VoxelLighting.h"

void Chunk::InitSkyLight() {
	for(int x = 0; x < CHUNKSIZE_X; x++) {
		for(int y = 0; y < CHUNKSIZE_Y; y++) {
			for(int z = 0; z < CHUNKSIZE_Z; z++) {
				SetSkyLight(x, y, z, 15);
			}
		}
	}


	//if(indexPosition.y < -1) return; // meh

	//if(indexPosition.y == CHUNKLOAD_FIXED_PY) {

	//}

	//Chunk* top = chunkManager->GetChunk(Vector3Int(indexPosition.x, CHUNKLOAD_FIXED_PY, indexPosition.z));
	//unique_lock<std::mutex> lock(chunkManager->GetLighting()->skyLightQueueMutex);

	//int breakCond = 100;
	//do {
	//	top = chunkManager->GetChunk(Vector3Int( indexPosition.x, CHUNKLOAD_FIXED_PY, indexPosition.z ));
	//	this_thread::sleep_for(chrono::milliseconds(10));
	//	if(breakCond-- <= 0) break;
	//} while(top == nullptr);
	
	//if(top == nullptr || top->pendingDeletion) return;
		//top = this;

	//if(indexPosition.y == CHUNKLOAD_FIXED_PY) {
		//for(int x = 0; x < CHUNKSIZE_X; x++) {
		//	for(int z = 0; z < CHUNKSIZE_Z; z++) {
		//		if(BlockDef::GetDef(top->blockData[x][CHUNKSIZE_Y - 1][z]).IsOpaque()) continue;

		//		SetSkyLight(x, CHUNKSIZE_Y - 1, z, 15);
		//		chunkManager->GetLighting()->QueueSkyLight(LightNode(x, CHUNKSIZE_Y - 1, z, top));
		//	}
		//}
	//}

	//if(indexPosition.y == CHUNKLOAD_FIXED_PY) {
	//	//unique_lock<std::mutex> lock(chunkManager->GetLighting()->skyLightQueueMutex);
	//	for(int x = 0; x < CHUNKSIZE_X; x++) {
	//		for(int z = 0; z < CHUNKSIZE_Z; z++) {
	//			int y = CHUNKSIZE_Y;
	//			BlockID block = AIR;

	//			for(;;) {
	//				if(this->pendingDeletion) return;

	//				block = GetBlockIncludingNeighbours(x, --y, z);
	//				if(BlockDef::GetDef(block).IsOpaque() || y < -CHUNKSIZE_Y * 5) break;

	//				SetSkyLightIncludingNeighbours(x, y, z, 15, false);
	//				//size_t oldLen = skyLightQueue.size();
	//				//chunk->SetSkyLightIncludingNeighbours(x, y, z, 15);
	//				//if(skyLightQueue.size() > oldLen) {

	//				//	floodedChunksHash[skyLightQueue.top().chunk] = true; // Dont re-flood this chunk
	//				//	chunkIndexRebuildQueue[skyLightQueue.top().chunk] = true;
	//				//	skyLightQueue.pop();
	//				//}
	//			}
	//			//SetSkyLight(x, CHUNKSIZE_Y - 1, z, 15);
	//		}
	//	}
	//}


	//for(int x = 0; x < CHUNKSIZE_X; x++) {
	//	for(int z = 0; z < CHUNKSIZE_Z; z++) {
	//		int y = CHUNKSIZE_Y;
	//		BlockID block = AIR;

	//		for(;;) {
	//			block = top->GetBlockIncludingNeighbours(x, --y, z);
	//			if(BlockDef::GetDef(block).IsOpaque() || y < -CHUNKSIZE_Y * 5) break;

	//			top->SetSkyLightIncludingNeighbours(x, y, z, 15);
	//			//size_t oldLen = skyLightQueue.size();
	//			//chunk->SetSkyLightIncludingNeighbours(x, y, z, 15);
	//			//if(skyLightQueue.size() > oldLen) {

	//			//	floodedChunksHash[skyLightQueue.top().chunk] = true; // Dont re-flood this chunk
	//			//	chunkIndexRebuildQueue[skyLightQueue.top().chunk] = true;
	//			//	skyLightQueue.pop();
	//			//}
	//		}
	//	}
	//}

	//for(int x = 0; x < CHUNKSIZE_X; x++) {
	//	for(int y = 0; y < CHUNKSIZE_Y; y++) {
	//		for(int z = 0; z < CHUNKSIZE_Z; z++) {
	//			int light = GetSkyLight(x, y, z);

	//			if(light < 15) continue;

	//			if(
	//				GetSkyLightIncludingNeighbours(x - 1, y, z) == 0 ||
	//				GetSkyLightIncludingNeighbours(x + 1, y, z) == 0 ||
	//				GetSkyLightIncludingNeighbours(x, y - 1, z) == 0 ||
	//				GetSkyLightIncludingNeighbours(x, y + 1, z) == 0 ||
	//				GetSkyLightIncludingNeighbours(x, y, z - 1) == 0 ||
	//				GetSkyLightIncludingNeighbours(x, y, z + 1) == 0
	//				) {
	//				SetSkyLight(x, y, z, light);
	//			}
	//		}
	//	}
	//}
}

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

	chunkManager->GetLighting()->QueueBlockLight(LightNode(x, y, z, this));
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

void Chunk::SetSkyLightIncludingNeighbours(const int& x, const int& y, const int& z, const int& val, bool update)
{
	if(x < 0 || x>CHUNKSIZE_X - 1 || y < 0 || y>CHUNKSIZE_Y - 1 || z < 0 || z>CHUNKSIZE_Z - 1)
	{
		Vector3Int chunkPosition = Vector3Int(indexPosition.x * CHUNKSIZE_X, indexPosition.y * CHUNKSIZE_Y, indexPosition.z * CHUNKSIZE_Z);
		chunkManager->SetSkyLightAtWorldPos(x + chunkPosition.x, y + chunkPosition.y, z + chunkPosition.z, val, update);
	}
	else {
		SetSkyLight(x, y, z, val, update);
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


void Chunk::SetSkyLight(const int& x, const int& y, const int& z, const int& val, bool update)
{
	this->lightLevel[x][y][z] = (this->lightLevel[x][y][z] & 0xF) | (val << 4);

	//if(update) chunkManager->GetLighting()->QueueSkyLight(LightNode(x, y, z, this));
}
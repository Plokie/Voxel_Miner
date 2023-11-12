#include "Lighting.h"

#include "Chunk.h"
#include "ChunkManager.h"



Lighting::Lighting(ChunkManager* chunkManager)
{
	this->chunkManager = chunkManager;

	InitializeSRWLock(&lightQueueMutex);
}

void Lighting::QueueLight(const LightNode& light)
{
	//AcquireSRWLockExclusive(&lightQueueMutex);
	lightBfsQueue.emplace(light);
	//ReleaseSRWLockExclusive(&lightQueueMutex);
}

void Lighting::StartThread()
{
	_lightingThreads.emplace_back([&]() { LightingThread(); });
}

void noop() {

}

void Lighting::TryFloodLightTo(const Vector3Int& localPos, const int& currentLevel, Chunk* chunk)
{
	if(
		!BlockDef::GetDef(chunkManager->GetBlockAtWorldPos(localPos)).IsSolid() &&
		chunk->GetBlockLightIncludingNeighbours(localPos.x, localPos.y, localPos.z) + 2 <= currentLevel
		)
	{
		//chunk->GetBlockLightIncludingNeighbours(worldPos, currentLevel - 1);
		//if(localPos.x < 0 || localPos.z < 0 || localPos.y < 0) {
		//	noop();
		//}
		chunk->SetBlockLightIncludingNeighbours(localPos.x, localPos.y, localPos.z, currentLevel - 1);
	}
}

void Lighting::LightingThread()
{
	while(_isRunning) {
		//AcquireSRWLockExclusive(&lightQueueMutex);
		while(!lightBfsQueue.empty()) {
			LightNode& node = lightBfsQueue.front();

			Vector3Int index = node.indexPos;

			// issue when local index.y == 1, 2 and maybe 3
			if(index.y == 1)
				noop();

			Chunk* chunk = node.chunk;
			int lightLevel = chunk->GetBlockLight(index.x, index.y, index.z);

			lightBfsQueue.pop();

			if(chunkIndexRebuildQueue.find(chunk) == chunkIndexRebuildQueue.end())
				chunkIndexRebuildQueue[chunk] = true;

			//todo: dont use get block light at world pos for other blocks within the same chunk
			TryFloodLightTo(index + Vector3Int(-1, 0, 0), lightLevel, chunk);
			TryFloodLightTo(index + Vector3Int(1, 0, 0), lightLevel, chunk);
			TryFloodLightTo(index + Vector3Int(0, -1, 0), lightLevel, chunk);
			TryFloodLightTo(index + Vector3Int(0, 1, 0), lightLevel, chunk);
			TryFloodLightTo(index + Vector3Int(0, 0, -1), lightLevel, chunk);
			TryFloodLightTo(index + Vector3Int(0, 0, 1), lightLevel, chunk);


		}
		//ReleaseSRWLockExclusive(&lightQueueMutex);

		for(const pair<Chunk*, bool>& pair : chunkIndexRebuildQueue) {
			pair.first->BuildMesh();
		}
		chunkIndexRebuildQueue.clear();
	}
}

Lighting::~Lighting()
{
	_isRunning = false;
	for(thread& thread : _lightingThreads) {
		thread.join();
	}
}

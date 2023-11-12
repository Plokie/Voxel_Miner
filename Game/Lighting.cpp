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
	AcquireSRWLockExclusive(&lightQueueMutex);
	lightBfsQueue.emplace(light);
	ReleaseSRWLockExclusive(&lightQueueMutex);
}

void Lighting::StartThread()
{
	_lightingThreads.emplace_back([&]() { LightingThread(); });
}

void Lighting::TryFloodLightTo(const Vector3Int& worldPos, const int& currentLevel)
{
	if(
		!BlockDef::GetDef(chunkManager->GetBlockAtWorldPos(worldPos)).IsSolid() &&
		chunkManager->GetBlockLightAtWorldPos(worldPos) + 2 <= currentLevel
		)
	{
		chunkManager->SetBlockLightAtWorldPos(worldPos, currentLevel - 1);
	}
}

void Lighting::LightingThread()
{
	while(_isRunning) {
		AcquireSRWLockExclusive(&lightQueueMutex);
		while(!lightBfsQueue.empty()) {
			LightNode& node = lightBfsQueue.front();

			Vector3Int index = node.indexPos;
			Chunk* chunk = node.chunk;
			int lightLevel = chunk->GetBlockLight(index.x, index.y, index.z);

			lightBfsQueue.pop();

			//todo: dont use get block light at world pos for other blocks within the same chunk
			TryFloodLightTo(index + Vector3Int(-1, 0, 0), lightLevel);
			TryFloodLightTo(index + Vector3Int(1, 0, 0), lightLevel);
			TryFloodLightTo(index + Vector3Int(0, -1, 0), lightLevel);
			TryFloodLightTo(index + Vector3Int(0, 1, 0), lightLevel);
			TryFloodLightTo(index + Vector3Int(0, 0, -1), lightLevel);
			TryFloodLightTo(index + Vector3Int(0, 0, 1), lightLevel);


		}
		ReleaseSRWLockExclusive(&lightQueueMutex);
	}
}

Lighting::~Lighting()
{
	_isRunning = false;
	for(thread& thread : _lightingThreads) {
		thread.join();
	}
}

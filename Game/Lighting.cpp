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

void Lighting::QueueRemoveLight(const RemoveLightNode& remLight)
{
	removeLightBfsQueue.emplace(remLight);
}

void Lighting::StartThread()
{
	_lightingThreads.emplace_back([&]() { LightingThread(); });
}

void Lighting::TryFloodLightTo(const Vector3Int& neighbourIndex, const int& currentLevel, Chunk* chunk)
{
	BlockID block = chunk->GetBlockIncludingNeighbours(neighbourIndex.x, neighbourIndex.y, neighbourIndex.z);
	int light = chunk->GetBlockLightIncludingNeighbours(neighbourIndex.x, neighbourIndex.y, neighbourIndex.z);

	// Checks if the neighbour block is transparent and if neighbour light level +1 is less than current level
	if(!BlockDef::GetDef(block).IsSolid() && light + 2 <= currentLevel)
	{
		// Spreads light to the available neighbour, but with a light level decreased from the current tile
		// This function appends even more to the lightBfsQueue, continuuing the while loop until all light has spread
		chunk->SetBlockLightIncludingNeighbours(neighbourIndex.x, neighbourIndex.y, neighbourIndex.z, currentLevel - 1);
	}
}

void Lighting::TryRemoveLight(const Vector3Int& neighbourIndex, const int& currentLevel, Chunk* chunk)
{
	BlockID block = chunk->GetBlockIncludingNeighbours(neighbourIndex.x, neighbourIndex.y, neighbourIndex.z);
	int neighbourLevel = chunk->GetBlockLightIncludingNeighbours(neighbourIndex.x, neighbourIndex.y, neighbourIndex.z);

	if(neighbourLevel != 0 && neighbourLevel < currentLevel) {
		chunk->SetBlockLightIncludingNeighbours(neighbourIndex.x, neighbourIndex.y, neighbourIndex.z, 0);

		// Take the data from the front of the lightQueue which was just added in the previous func call
		Vector3Int realIndex = lightBfsQueue.top().localIndexPos;
		Chunk* realChunk = lightBfsQueue.top().chunk;

		// Since the SetBlockLight appends to the lightBfsQueue, we don't want this to happen
		// And must immediately remove it from the queue
		lightBfsQueue.pop();

		// Add a new remove light queue for the neighbour (to remove ITS neighbours)
		removeLightBfsQueue.emplace(realIndex, realChunk, neighbourLevel);
	}
	else if(neighbourLevel >= currentLevel) { // if neighbour light is brigther than current light, then re-spread the light back over this block'
		// Queues neighbour to be recalculated
		chunk->SetBlockLightIncludingNeighbours(neighbourIndex.x, neighbourIndex.y, neighbourIndex.z, neighbourLevel);
	}
}

void Lighting::LightingThread()
{

	while(_isRunning) {
		while (!removeLightBfsQueue.empty()) {
			RemoveLightNode& node = removeLightBfsQueue.front();
			Vector3Int index = node.localIndexPos;
			Chunk* chunk = node.chunk;
			int lightLevel = node.val;

			removeLightBfsQueue.pop();

			if (node.chunk == nullptr) continue;

			// Add to Map of chunks to be queued to rebuilt
			chunkIndexRebuildQueue[chunk] = true;

			TryRemoveLight(index + Vector3Int(-1, 0, 0), lightLevel, chunk);
			TryRemoveLight(index + Vector3Int(1, 0, 0), lightLevel, chunk);
			TryRemoveLight(index + Vector3Int(0, -1, 0), lightLevel, chunk);
			TryRemoveLight(index + Vector3Int(0, 1, 0), lightLevel, chunk);
			TryRemoveLight(index + Vector3Int(0, 0, -1), lightLevel, chunk);
			TryRemoveLight(index + Vector3Int(0, 0, 1), lightLevel, chunk);
		}

		//AcquireSRWLockExclusive(&lightQueueMutex);
 		while(!lightBfsQueue.empty()) {
			//debugCooldown = 10;
			LightNode& node = lightBfsQueue.top();

			Vector3Int index = node.localIndexPos;
			Chunk* chunk = node.chunk;
			lightBfsQueue.pop();

			if(chunk == nullptr) continue;

			int lightLevel = chunk->GetBlockLight(index.x, index.y, index.z);

			// Add to Map of chunks to be queued to rebuilt
			chunkIndexRebuildQueue[chunk] = true;

			//todo: dont use get block light at world pos for other blocks within the same chunk
			TryFloodLightTo(index + Vector3Int(-1, 0, 0), lightLevel, chunk);
			TryFloodLightTo(index + Vector3Int(1, 0, 0), lightLevel, chunk);
			TryFloodLightTo(index + Vector3Int(0, -1, 0), lightLevel, chunk);
			TryFloodLightTo(index + Vector3Int(0, 1, 0), lightLevel, chunk);
			TryFloodLightTo(index + Vector3Int(0, 0, -1), lightLevel, chunk);
			TryFloodLightTo(index + Vector3Int(0, 0, 1), lightLevel, chunk);
		}

		if(!chunkIndexRebuildQueue.empty()) {
			//AcquireSRWLockExclusive(&chunkManager->destroy);
			for(const pair<Chunk*, bool>& pair : chunkIndexRebuildQueue) {
				chunkManager->rebuildQueue.push(pair.first);
			}
			//ReleaseSRWLockExclusive(&pair.first->gAccessMutex);
			chunkIndexRebuildQueue.clear();
		}
	}
}

Lighting::~Lighting()
{
	_isRunning = false;
	for(thread& thread : _lightingThreads) {
		thread.join();
	}
}

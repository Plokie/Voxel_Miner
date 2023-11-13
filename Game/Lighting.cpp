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
	lightBfsQueue.push(light);
	//ReleaseSRWLockExclusive(&lightQueueMutex);
}

void Lighting::QueueRemoveLight(const RemoveLightNode& remLight)
{
	removeLightBfsQueue.push(remLight);
}

void Lighting::StartThread()
{
	_lightingThreads.emplace_back([&]() { LightingThread(); });
}

void noop() {

}

void Lighting::TryFloodLightTo(const Vector3Int& neighbourPos, const int& currentLevel, Chunk* chunk)
{
	BlockID block = chunk->GetBlockIncludingNeighbours(neighbourPos.x, neighbourPos.y, neighbourPos.z);
	int light = chunk->GetBlockLightIncludingNeighbours(neighbourPos.x, neighbourPos.y, neighbourPos.z);

	if(
		!BlockDef::GetDef(block).IsSolid() &&
		light + 2 <= currentLevel
		//chunk->GetBlockLight(localPos.x, localPos.y, localPos.z) + 2 <= currentLevel
		)
	{

		chunk->SetBlockLightIncludingNeighbours(neighbourPos.x, neighbourPos.y, neighbourPos.z, currentLevel - 1);
		//chunk->SetBlockLight(localPos.x, localPos.y, localPos.z, currentLevel - 1);
		//lightBfsQueue.emplace(localPos, chunk);
	}
}

void Lighting::TryRemoveLight(const Vector3Int& neighbourIndex, const int& currentLevel, Chunk* chunk)
{
	int neighbourLevel = chunk->GetBlockLightIncludingNeighbours(neighbourIndex.x, neighbourIndex.y, neighbourIndex.z);

	if(neighbourLevel != 0 && neighbourLevel < currentLevel) {
		 chunk->SetBlockLightIncludingNeighbours(neighbourIndex.x, neighbourIndex.y, neighbourIndex.z, 0);

		 Vector3Int realIndex = lightBfsQueue.front().localIndexPos;
		 Chunk* realChunk = lightBfsQueue.front().chunk;
		 lightBfsQueue.pop(); // Remove the block we just added from the light queue
		//Also yoink the index and chunk information, and its correct for the neighbours

		// error because outside of chunk borders, chunk is pointing to the incorrect chunk i think
		 removeLightBfsQueue.emplace(realIndex, realChunk, neighbourLevel);
	}
	else if(neighbourLevel >= currentLevel) { // if neighbour light is brigther than current light, then re-spread the light back over this block
		chunk->SetBlockLightIncludingNeighbours(neighbourIndex.x, neighbourIndex.y, neighbourIndex.z, neighbourLevel); // Tells it to re-flood the light


		//Vector3Int realIndex = lightBfsQueue.front().localIndexPos;
		//Chunk* realChunk = lightBfsQueue.front().chunk;
		//lightBfsQueue.pop(); // Remove the block we just added from the light queue
		//Also yoink the index and chunk information, and its correct for the neighbours

		// error because outside of chunk borders, chunk is pointing to the incorrect chunk i think
		//lightBfsQueue.emplace(realIndex, realChunk);
	}
}

void Lighting::LightingThread()
{
	while(_isRunning) {
		
		//AcquireSRWLockExclusive(&lightQueueMutex);
		while(!lightBfsQueue.empty()) {
			LightNode& node = lightBfsQueue.front();

			Vector3Int index = node.localIndexPos;
			Chunk* chunk = node.chunk;
			lightBfsQueue.pop(); //dequeue before empty???????????????? what???????????? WHAT???? but AAGGGGGGGG

			int lightLevel = chunk->GetBlockLight(index.x, index.y, index.z);

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

		while (!removeLightBfsQueue.empty()) {
			RemoveLightNode& node = removeLightBfsQueue.front();

			Vector3Int index = node.localIndexPos;
			Chunk* chunk = node.chunk;
			int lightLevel = node.val;
			removeLightBfsQueue.pop();

			if (chunkIndexRebuildQueue.find(chunk) == chunkIndexRebuildQueue.end())
				chunkIndexRebuildQueue[chunk] = true;

			TryRemoveLight(index + Vector3Int(1, 0, 0), lightLevel, chunk);
			TryRemoveLight(index + Vector3Int(-1, 0, 0), lightLevel, chunk);
			TryRemoveLight(index + Vector3Int(0, 1, 0), lightLevel, chunk);
			TryRemoveLight(index + Vector3Int(0, -1, 0), lightLevel, chunk);
			TryRemoveLight(index + Vector3Int(0, 0, 1), lightLevel, chunk);
			TryRemoveLight(index + Vector3Int(0, 0, -1), lightLevel, chunk);
		}




		for(const pair<Chunk*, bool>& pair : chunkIndexRebuildQueue) {
			AcquireSRWLockExclusive(&pair.first->gAccessMutex);
			pair.first->BuildMesh();
			ReleaseSRWLockExclusive(&pair.first->gAccessMutex);
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

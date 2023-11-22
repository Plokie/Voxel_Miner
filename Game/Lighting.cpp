#include "Lighting.h"

#include "Chunk.h"
#include "ChunkManager.h"



Lighting::Lighting(ChunkManager* chunkManager)
{
	this->chunkManager = chunkManager;

	InitializeSRWLock(&lightQueueMutex);
}

void Lighting::QueueNewChunk(Chunk* chunk)
{
	newSkyChunks.push(chunk);
}

void Lighting::QueueLight(const LightNode& light)
{
	//AcquireSRWLockExclusive(&lightQueueMutex);
	lightBfsQueue.emplace(light);
	//ReleaseSRWLockExclusive(&lightQueueMutex);
}

void Lighting::QueueSkyLight(const LightNode& light)
{
	skyLightQueue.emplace(light);
}

void Lighting::QueueRemoveLight(const RemoveLightNode& remLight)
{
	removeLightBfsQueue.emplace(remLight);
}

void Lighting::QueueRemoveSkyLight(const RemoveLightNode& remLight)
{
	removeSkyLightQueue.emplace(remLight);
}

void Lighting::StartThread()
{
	_lightingThreads.emplace_back([&]() { LightingThread(); });
}

void Lighting::TryFloodLightTo(const Vector3Int& neighbourIndex, const int& currentLevel, Chunk* chunk)
{
	const BlockID block = chunk->GetBlockIncludingNeighbours(neighbourIndex.x, neighbourIndex.y, neighbourIndex.z);
	const int light = chunk->GetBlockLightIncludingNeighbours(neighbourIndex.x, neighbourIndex.y, neighbourIndex.z);
	const Block& def = BlockDef::GetDef(block);

	// Checks if the neighbour block is transparent and if neighbour light level +2 is less than current level
	if((!def.IsOpaque() || def.LightValue()) && light + 2 <= currentLevel)
	{
		// Spreads light to the available neighbour, but with a light level decreased from the current tile
		// This function appends even more to the lightBfsQueue, continuuing the while loop until all light has spread
		chunk->SetBlockLightIncludingNeighbours(neighbourIndex.x, neighbourIndex.y, neighbourIndex.z, currentLevel - 1);
	}
}

void Lighting::TryRemoveLight(const Vector3Int& neighbourIndex, const int& currentLevel, Chunk* chunk)
{
	//BlockID block = chunk->GetBlockIncludingNeighbours(neighbourIndex.x, neighbourIndex.y, neighbourIndex.z);
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

void Lighting::TryFloodSkyLightTo(const Vector3Int& neighbourIndex, const int& currentLevel, Chunk* chunk, bool isDown)
{
	const BlockID block = chunk->GetBlockIncludingNeighbours(neighbourIndex.x, neighbourIndex.y, neighbourIndex.z);
	const int light = chunk->GetSkyLightIncludingNeighbours(neighbourIndex.x, neighbourIndex.y, neighbourIndex.z);
	const Block& def = BlockDef::GetDef(block);

	// Checks if the neighbour block is transparent and if neighbour light level +2 is less than current level
	if((!def.IsOpaque() || def.LightValue()) && ((light + 2 <= currentLevel) || isDown))
	{
		// Spreads light to the available neighbour, but with a light level decreased from the current tile
		// This function appends even more to the lightBfsQueue, continuuing the while loop until all light has spread
		chunk->SetSkyLightIncludingNeighbours(neighbourIndex.x, neighbourIndex.y, neighbourIndex.z, currentLevel - (isDown ? 0 : 1));
		//skyLightQueue.emplace(neighbourIndex, chunk);

	}
}

void Lighting::TryRemoveSkyLight(const Vector3Int& neighbourIndex, const int& currentLevel, Chunk* chunk, bool isDown)
{
	//BlockID block = chunk->GetBlockIncludingNeighbours(neighbourIndex.x, neighbourIndex.y, neighbourIndex.z);
	int neighbourLevel = chunk->GetSkyLightIncludingNeighbours(neighbourIndex.x, neighbourIndex.y, neighbourIndex.z);

	if((neighbourLevel != 0 && neighbourLevel < currentLevel) || (currentLevel == 15 && isDown)) {
		chunk->SetSkyLightIncludingNeighbours(neighbourIndex.x, neighbourIndex.y, neighbourIndex.z, 0);

		// Take the data from the front of the lightQueue which was just added in the previous func call
		Vector3Int realIndex = skyLightQueue.top().localIndexPos;
		Chunk* realChunk = skyLightQueue.top().chunk;

		// Since the SetBlockLight appends to the lightBfsQueue, we don't want this to happen
		// And must immediately remove it from the queue
		skyLightQueue.pop();

		// Add a new remove light queue for the neighbour (to remove ITS neighbours)
		removeSkyLightQueue.emplace(realIndex, realChunk, neighbourLevel);
	}
	else if(neighbourLevel >= currentLevel) { // if neighbour light is brigther than current light, then re-spread the light back over this block'
		// Queues neighbour to be recalculated
		chunk->SetSkyLightIncludingNeighbours(neighbourIndex.x, neighbourIndex.y, neighbourIndex.z, neighbourLevel);
		skyLightQueue.emplace(neighbourIndex, chunk);
	}
}

void Lighting::LightingThread()
{
	SRWLOCK* pDestroyMutex = Engine::Get()->GetDestroyObjectsMutex();
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

		while(!newSkyChunks.empty()) {
			Chunk* chunk = newSkyChunks.front();
			AcquireSRWLockExclusive(&chunk->gAccessMutex);

			if(chunk->chunkIndexPosition.y == 1) { // Only do inital light flooding for chunks in sky

				for(int x = 0; x < CHUNKSIZE_X; x++) {
					for(int z = 0; z < CHUNKSIZE_Z; z++) {
						int y = CHUNKSIZE_Y;
						BlockID block = AIR;

						//chunk->SetSkyLight(x, CHUNKSIZE_Y-1, z, 15);

						for (;;) {
							block = chunk->GetBlockIncludingNeighbours(x, --y, z);
							if (block != AIR || y < -CHUNKSIZE_Y*3) break;

							//chunk->SetSkyLight(x, y, z, 15);
							int oldLen = skyLightQueue.size();
							chunk->SetSkyLightIncludingNeighbours(x, y, z, 15);
							if(skyLightQueue.size() > oldLen)
								skyLightQueue.pop();
						}
					}
				}
			}
			ReleaseSRWLockExclusive(&chunk->gAccessMutex);
			skyChunksToFlood.emplace(chunk);
			newSkyChunks.pop();
		}


		while(!skyChunksToFlood.empty()) {
			Chunk* chunk = skyChunksToFlood.front();
			AcquireSRWLockExclusive(&chunk->gAccessMutex);
			// Find sky light levels of 15 neighbouring levels of 0 to flood
			for(int x = 0; x < CHUNKSIZE_X; x++) {
				for(int y = 0; y < CHUNKSIZE_Y; y++) {
					for(int z = 0; z < CHUNKSIZE_Z; z++) {
						if(chunk == nullptr) continue;
						if(chunk->GetSkyLight(x, y, z) < 15) continue;

						if(chunk->GetSkyLightIncludingNeighbours(x - 1, y, z) == 0) {
							skyLightQueue.emplace(x, y, z, chunk);
						}
						if(chunk->GetSkyLightIncludingNeighbours(x + 1, y, z) == 0) {
							skyLightQueue.emplace(x, y, z, chunk);
						}
						if(chunk->GetSkyLightIncludingNeighbours(x, y - 1, z) == 0) {
							skyLightQueue.emplace(x, y, z, chunk);
						}
						if(chunk->GetSkyLightIncludingNeighbours(x, y + 1, z) == 0) {
							skyLightQueue.emplace(x, y, z, chunk);
						}
						if(chunk->GetSkyLightIncludingNeighbours(x, y, z - 1) == 0) {
							skyLightQueue.emplace(x, y, z, chunk);
						}
						if(chunk->GetSkyLightIncludingNeighbours(x, y, z + 1) == 0) {
							skyLightQueue.emplace(x, y, z, chunk);
						}
					}
				}
			}
			ReleaseSRWLockExclusive(&chunk->gAccessMutex);
			skyChunksToFlood.pop();
		}

		////AcquireSRWLockExclusive(pDestroyMutex);
		//while(!removeSkyLightQueue.empty()) {
		//	RemoveLightNode& node = removeSkyLightQueue.front();
		//	Vector3Int index = node.localIndexPos;
		//	Chunk* chunk = node.chunk;
		//	int lightLevel = node.val;

		//	removeSkyLightQueue.pop();

		//	if(node.chunk == nullptr) continue;

		//	// Add to Map of chunks to be queued to rebuilt
		//	chunkIndexRebuildQueue[chunk] = true;



		//	// incoming bug: because sunlight spreads downward infinitely, so should the removal of it

		//	//TryRemoveSkyLight(index + Vector3Int(-1, 0, 0), lightLevel, chunk);
		//	//TryRemoveSkyLight(index + Vector3Int(1, 0, 0), lightLevel, chunk);
		//	//TryRemoveSkyLight(index + Vector3Int(0, -1, 0), lightLevel, chunk,true);
		//	//TryRemoveSkyLight(index + Vector3Int(0, 1, 0), lightLevel, chunk);
		//	//TryRemoveSkyLight(index + Vector3Int(0, 0, -1), lightLevel, chunk);
		//	//TryRemoveSkyLight(index + Vector3Int(0, 0, 1), lightLevel, chunk);
		//}

		//ReleaseSRWLockExclusive(pDestroyMutex);
		while(!skyLightQueue.empty()) {
			//debugCooldown = 10;
			LightNode& node = skyLightQueue.top();

			Vector3Int index = node.localIndexPos;
			Chunk* chunk = node.chunk;
			skyLightQueue.pop();
			AcquireSRWLockExclusive(&chunk->gAccessMutex);

			if(chunk == nullptr) continue;

			int lightLevel = chunk->GetSkyLight(index.x, index.y, index.z);

			// Add to Map of chunks to be queued to rebuilt
			chunkIndexRebuildQueue[chunk] = true;
			
			//AcquireSRWLockExclusive(pDestroyMutex);
			TryFloodSkyLightTo(index + Vector3Int(-1, 0, 0), lightLevel, chunk);
			TryFloodSkyLightTo(index + Vector3Int(1, 0, 0), lightLevel, chunk);
			TryFloodSkyLightTo(index + Vector3Int(0, -1, 0), lightLevel, chunk,true);
			//TryFloodSkyLightTo(index + Vector3Int(0, 1, 0), lightLevel, chunk);
			TryFloodSkyLightTo(index + Vector3Int(0, 0, -1), lightLevel, chunk);
			TryFloodSkyLightTo(index + Vector3Int(0, 0, 1), lightLevel, chunk);

			ReleaseSRWLockExclusive(&chunk->gAccessMutex);
		}
		//ReleaseSRWLockExclusive(pDestroyMutex);

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

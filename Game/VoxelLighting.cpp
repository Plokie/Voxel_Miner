#include "VoxelLighting.h"

#include "Blocks.h"
#include "Chunk.h"
#include "ChunkManager.h"

void VoxelLighting::_Thread()
{
	map<Chunk*, bool> chunkIndexRebuildQueue = {};
	while(isRunning) {
		while(!removeBlockLightQueue.empty()) {
			RemoveLightNode& node = removeBlockLightQueue.front();
			Vector3Int index = node.localIndexPos;
			Chunk* chunk = node.chunk;
			int lightLevel = node.val;

			removeBlockLightQueue.pop();

			if(node.chunk == nullptr) continue;

			// Add to Map of chunks to be queued to rebuilt
			chunkIndexRebuildQueue[chunk] = true;

			TryRemoveBlockLight(index + Vector3Int(-1, 0, 0), lightLevel, chunk);
			TryRemoveBlockLight(index + Vector3Int(1, 0, 0), lightLevel, chunk);
			TryRemoveBlockLight(index + Vector3Int(0, -1, 0), lightLevel, chunk);
			TryRemoveBlockLight(index + Vector3Int(0, 1, 0), lightLevel, chunk);
			TryRemoveBlockLight(index + Vector3Int(0, 0, -1), lightLevel, chunk);
			TryRemoveBlockLight(index + Vector3Int(0, 0, 1), lightLevel, chunk);
		}

		while(!blockLightQueue.empty()) {
			//debugCooldown = 10;
			LightNode& node = blockLightQueue.top();

			Vector3Int index = node.localIndexPos;
			Chunk* chunk = node.chunk;
			blockLightQueue.pop();

			if(chunk == nullptr) continue;

			int lightLevel = chunk->GetBlockLight(index.x, index.y, index.z);

			// Add to Map of chunks to be queued to rebuilt
			chunkIndexRebuildQueue[chunk] = true;

			//todo: dont use get block light at world pos for other blocks within the same chunk
			TryFloodBlockLightTo(index + Vector3Int(-1, 0, 0), lightLevel, chunk);
			TryFloodBlockLightTo(index + Vector3Int(1, 0, 0), lightLevel, chunk);
			TryFloodBlockLightTo(index + Vector3Int(0, -1, 0), lightLevel, chunk);
			TryFloodBlockLightTo(index + Vector3Int(0, 1, 0), lightLevel, chunk);
			TryFloodBlockLightTo(index + Vector3Int(0, 0, -1), lightLevel, chunk);
			TryFloodBlockLightTo(index + Vector3Int(0, 0, 1), lightLevel, chunk);
		}

		if(!chunkIndexRebuildQueue.empty()) {
			//AcquireSRWLockExclusive(&chunkManager->destroy);
			for(const pair<Chunk*, bool>& pair : chunkIndexRebuildQueue) {
				//chunkManager->rebuildQueue.push(pair.first);
				chunkManager->QueueRegen(pair.first);
			}
			//ReleaseSRWLockExclusive(&pair.first->gAccessMutex);
			chunkIndexRebuildQueue.clear();
		}
	}
}

void VoxelLighting::TryFloodBlockLightTo(const Vector3Int& neighbourIndex, const int& currentLevel, Chunk* chunk)
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

void VoxelLighting::TryRemoveBlockLight(const Vector3Int& neighbourIndex, const int& currentLevel, Chunk* chunk)
{
	int neighbourLevel = chunk->GetBlockLightIncludingNeighbours(neighbourIndex.x, neighbourIndex.y, neighbourIndex.z);

	if(neighbourLevel != 0 && neighbourLevel < currentLevel) {
		chunk->SetBlockLightIncludingNeighbours(neighbourIndex.x, neighbourIndex.y, neighbourIndex.z, 0);

		// Take the data from the front of the lightQueue which was just added in the previous func call
		Vector3Int realIndex = blockLightQueue.top().localIndexPos;
		Chunk* realChunk = blockLightQueue.top().chunk;

		// Since the SetBlockLight appends to the lightBfsQueue, we don't want this to happen
		// And must immediately remove it from the queue
		blockLightQueue.pop();

		// Add a new remove light queue for the neighbour (to remove ITS neighbours)
		removeBlockLightQueue.emplace(realIndex, realChunk, neighbourLevel);
	}
	else if(neighbourLevel >= currentLevel) { // if neighbour light is brigther than current light, then re-spread the light back over this block'
		// Queues neighbour to be recalculated
		chunk->SetBlockLightIncludingNeighbours(neighbourIndex.x, neighbourIndex.y, neighbourIndex.z, neighbourLevel);
	}
}

VoxelLighting::VoxelLighting(ChunkManager* chunkManager) : chunkManager(chunkManager) {}

VoxelLighting::~VoxelLighting() {
	isRunning = false;
	for(auto& thread : lightingThreadVec) {
		thread.join();
	}
}

void VoxelLighting::QueueBlockLight(const LightNode& light)
{
	blockLightQueue.emplace(light);
}

void VoxelLighting::QueueRemoveBlockLight(const RemoveLightNode& remLight)
{
	removeBlockLightQueue.emplace(remLight);
}

void VoxelLighting::StartThread()
{
	lightingThreadVec.emplace_back([&] { _Thread(); });
}

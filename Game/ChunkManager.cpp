#include "ChunkManager.h"

#include "Chunk.h"
#include "WorldGen.h"
#include "../Engine/Engine.h"
#include "ChunkDatabase.h"
#include "VoxelLighting.h"
#include "../Engine/ThreadUtil.h"

void ChunkManager::CreateChunk(const int x, const int y, const int z)
{
	tuple<int, int, int> pos = { x,y,z };
	if(chunkMap.find(pos) == chunkMap.end()) {
		Chunk* newChunk = new Chunk(pos, this);

		{	unique_lock<std::mutex> lock(gAccessMutex);
			chunkMap[pos] = newChunk;
		}

		newChunk->transform.position = Vector3(static_cast<float>(CHUNKSIZE_X * x), static_cast<float>(CHUNKSIZE_Y * y), static_cast<float>(CHUNKSIZE_Z * z));

		newChunkQueue.push(newChunk);

		threadPool->Queue([=] {
			//unique_lock<std::mutex> lock(newChunk->gAccessMutex);
			if(ChunkDatabase::Get()->DoesDataExistForChunk({ x,y,z })) 
				ChunkDatabase::Get()->LoadChunkDataInto({ x,y,z }, newChunk);
			else newChunk->GenerateBlockData();
			
			newChunk->hasLoadedBlockData = true;
			
			newChunk->InitSkyLight();
		}, y == CHUNKLOAD_FIXED_PY); // If the chunk is at the TOP, the higher priority value means it will be completed last (or it should be...)
	}
}

Vector3Int ChunkManager::ChunkFloorPosForPositionCalculation(Vector3 worldPosition) {
	return Vector3Int(
		static_cast<int>(floorf(worldPosition.x / CHUNKSIZE_X)),
		static_cast<int>(floorf(worldPosition.y / CHUNKSIZE_Y)),
		static_cast<int>(floorf(worldPosition.z / CHUNKSIZE_Z))
	);
}

void ChunkManager::Update(float dTime) {
	if(Input::IsKeyPressed('I')) {
		ChunkDatabase::Get()->Close(); // doenst actually close but saves loaded chunk data to disk
	}
}

void ChunkManager::Thread() {

	while(isRunning) {
		Vector3Int camIndex = ChunkFloorPosForPositionCalculation(camTrans->position);

		if(newChunkQueue.empty())
			for(int y = 1 - CHUNKLOAD_FIXED_NY; y < CHUNKLOAD_FIXED_PY + 1; y++) {
				for(int x = 1 - CHUNKLOAD_AREA_X; x < CHUNKLOAD_AREA_X + 1; x++) {
					for(int z = 1 - CHUNKLOAD_AREA_Z; z < CHUNKLOAD_AREA_Z + 1; z++) {
						CreateChunk(camIndex.x + x, y, camIndex.z + z);
					}
				}
			}

		if(!threadPool->IsBusy() && !newChunkQueue.empty()) {
			while(!newChunkQueue.empty()) {
				Chunk* chunk = newChunkQueue.front();
				threadPool->Queue([=] {

					chunk->Finalize();
					//chunk->InitSkyLight();
					//_numChunksLoaded++;
					unique_lock<mutex> lock(this->_numChunksLoadedMutex);
					this->_numChunksLoaded++;
				});
				//_numChunksLoaded++;
				newChunkQueue.pop();
			}
		}

		{	unique_lock<std::mutex> lock(regenQueueMutex);
			while(!regenQueue.empty()) {
				pair<Chunk*,int> queuePair = regenQueue.front();
				threadPool->Queue([queuePair] {
					if(queuePair.first == nullptr || queuePair.first->pendingDeletion) return;

					unique_lock<std::mutex> lock(queuePair.first->gAccessMutex);
					queuePair.first->BuildMesh();
				}, queuePair.second);
				regenQueue.pop();
			}
		}

		for(auto it = chunkMap.cbegin(); it != chunkMap.cend();) {
			const pair<tuple<int, int, int>, Chunk*>& pair = *it;

			if(pair.second == nullptr || pair.second->pendingDeletion || !pair.second->hasRanStartFunction) {
				++it;
				continue;
			}

			Vector3Int indexPos = pair.first;

			if(
				abs(indexPos.x - camIndex.x) > CHUNKLOAD_AREA_X ||
				abs(indexPos.z - camIndex.z) > CHUNKLOAD_AREA_Z
				) { // Erase chunk from map (it is out of range)

					{
						//unique_lock<std::mutex> lock(pair.second->gAccessMutex);
						unique_lock<std::mutex> destroyLock(*Engine::Get()->GetDestroyObjectsMutex());

						ChunkDatabase::Get()->UnloadChunk(indexPos);
						engine->DestroyObject3D(pair.second); // Delete chunk in-engine (adds to queue)
					}

					{	unique_lock<std::mutex> lock(gAccessMutex);
						chunkMap.erase(it++);
					}
			}
			else { // Increment iterator 
				++it;
			}
		}

		
	}
}

void ChunkManager::Start()
{
	ChunkDatabase::Init();

	threadPool = new ThreadPool();
	threadPool->thread_count = 6u;
	threadPool->namingScheme = "ChnkMgr Worker";
	threadPool->Init();

	lighting = new VoxelLighting(this);
	lighting->StartThread();

	engine = Engine::Get();
	camTrans = &Graphics::Get()->camera.transform;

	//chnkMgrThread = thread(ChunkManager::Thread, this);
	chnkMgrThread.emplace_back([&] {
		Thread();
	});

	SetThreadName(&chnkMgrThread[0], "ChunkManager Master");
}

ChunkManager::~ChunkManager()
{
	//ChunkDatabase::Get()->SaveChunks();
	//ChunkDatabase::Get()->SaveWorldData();
	ChunkDatabase::Get()->Close();

	isRunning = false;
	for(auto& thread : chnkMgrThread) {
		thread.join();
	}

	threadPool->Stop();
	delete threadPool;

	if(lighting) delete lighting;
}

tuple<int, int, int> ChunkManager::ToChunkIndexPositionTuple(const int& x, const int& y, const int& z)
{
	return tuple<int, int, int>(
		(x - ((CHUNKSIZE_X - 1) * static_cast<int>(x < 0))) / CHUNKSIZE_X,
		(y - ((CHUNKSIZE_Y - 1) * static_cast<int>(y < 0))) / CHUNKSIZE_Y,
		(z - ((CHUNKSIZE_Z - 1) * static_cast<int>(z < 0))) / CHUNKSIZE_Z
	);
}

BlockID ChunkManager::GetBlockAtWorldPos(const int& x, const int& y, const int& z)
{

	if(this == nullptr || this->pendingDeletion) return WorldGen::GetBlockAt(x, y, z);
	tuple<int, int, int> chunkIndex = ToChunkIndexPositionTuple(x, y, z);

	// If chunk is loaded
	if(chunkMap.find(chunkIndex) != chunkMap.end()) {
		Vector3Int localVoxelPos = Vector3Int(FloorMod(x, CHUNKSIZE_X), FloorMod(y, CHUNKSIZE_Y), FloorMod(z, CHUNKSIZE_Z));

		//AcquireSRWLockExclusive(&this->gAccessMutex);
		//unique_lock<std::mutex> lock(this->gAccessMutex);
		Chunk* chunk = chunkMap[chunkIndex];
		if(!(chunk == nullptr || chunk->pendingDeletion)) {
			if (!chunk->hasLoadedBlockData) return WorldGen::GetBlockAt(x, y, z);
			//TryAcquireSRWLockExclusive(&chunk->gAccessMutex);

			// Okay this might seem redundant, but i promise its not (i think)
			// this basically makes it so nothing else can modify it WHILE its being read from
			// but if somethings already writing to it, we can read it anyway
			//bool didMutex = TryAcquireSRWLockExclusive(&chunk->gAccessMutex);
			bool didMutex = chunk->gAccessMutex.try_lock();

			BlockID blockID = static_cast<BlockID>(chunk->blockData[localVoxelPos.x][localVoxelPos.y][localVoxelPos.z]);
			//if(didMutex) ReleaseSRWLockExclusive(&chunk->gAccessMutex);
			if(didMutex) chunk->gAccessMutex.unlock();
			//ReleaseSRWLockExclusive(&chunk->gAccessMutex);
			return blockID;
		}
	}

	//todo: read from chunk cache of height,temp,moist samples?
	return WorldGen::GetBlockAt(x, y, z);
}

BlockID ChunkManager::GetBlockAtWorldPos(const Vector3Int& v) {
	return GetBlockAtWorldPos(v.x, v.y, v.z);
}

void ChunkManager::SetBlockAtWorldPos(const int& x, const int& y, const int& z, const BlockID& id) {
	Vector3Int chunkIndex = ToChunkIndexPositionTuple(x, y, z);
	if(chunkMap.count(chunkIndex)) {
		Vector3Int localVoxelPos = Vector3Int(FloorMod(x, CHUNKSIZE_X), FloorMod(y, CHUNKSIZE_Y), FloorMod(z, CHUNKSIZE_Z));

		Chunk*& chunk = chunkMap[chunkIndex];
		if(chunk == nullptr) {
			assert(false);
			return;
		}

		const BlockID oldBlock = (BlockID)chunk->blockData[localVoxelPos.x][localVoxelPos.y][localVoxelPos.z];

		const Block& newBlockDef = BlockDef::GetDef(id);
		const Block& oldBlockDef = BlockDef::GetDef(oldBlock);

		chunk->blockData[localVoxelPos.x][localVoxelPos.y][localVoxelPos.z] = id;

		//lighting->QueueRemoveSkyLight({ localVoxelPos, chunk, (short)15 });
		//lighting->QueueSkyLight({ localVoxelPos, chunk });

		// If the new block placed had a light value
		if(newBlockDef.LightValue() != 0) {
			chunkMap[chunkIndex]->SetBlockLight(localVoxelPos.x, localVoxelPos.y, localVoxelPos.z, newBlockDef.LightValue());
		}

		// If a light is being removed
		if(oldBlockDef.LightValue() != 0 && id == AIR) {
			lighting->QueueRemoveBlockLight({ localVoxelPos, chunk, static_cast<short>(oldBlockDef.LightValue()) });


			chunk->SetBlockLight(localVoxelPos.x, localVoxelPos.y, localVoxelPos.z, 0);
			lighting->PopBlockLightQueue();
		}


		if(oldBlockDef.LightValue() && newBlockDef.LightValue()) {
			lighting->QueueRemoveBlockLight({ localVoxelPos, chunk, (short)chunk->GetBlockLight(localVoxelPos.x, localVoxelPos.y, localVoxelPos.z) });

			if(id != AIR) {
				chunk->SetBlockLight(localVoxelPos.x, localVoxelPos.y, localVoxelPos.z, 0);
			}
			else {
				chunk->SetBlockLight(localVoxelPos.x, localVoxelPos.y, localVoxelPos.z, chunk->GetBlockLight(localVoxelPos.x, localVoxelPos.y, localVoxelPos.z));
			}
		}

		//rebuildQueue.push(chunk);
		//chunk->BuildMesh_PoolFunc(false);
		//TryRegen(chunk, true);
		QueueRegen(chunk, -1);


		if(localVoxelPos.x == 0) { // Regen -x neighbour
			QueueRegen(chunkIndex + Vector3(-1, 0, 0));
		}
		else if(localVoxelPos.x == CHUNKSIZE_X - 1) { // Regen +x neighbour
			QueueRegen(chunkIndex + Vector3(1, 0, 0));
		}

		if(localVoxelPos.y == 0) {  // -y border
			QueueRegen(chunkIndex + Vector3(0, -1, 0));
		}
		else if(localVoxelPos.y == CHUNKSIZE_Y - 1) { // +y border
			QueueRegen(chunkIndex + Vector3(0, 1, 0));
		}

		if(localVoxelPos.z == 0) {	// -z border
			QueueRegen(chunkIndex + Vector3(0, 0, -1));
		}
		else if(localVoxelPos.z == CHUNKSIZE_Z - 1) { // +z border
			QueueRegen(chunkIndex + Vector3(0, 0, 1));
		}

		ChunkDatabase::Get()->SaveChunkData(chunkIndex, chunkMap[chunkIndex]);
	}
}

void ChunkManager::SetBlockAtWorldPos(const Vector3Int& pos, const BlockID& id) {
	SetBlockAtWorldPos(pos.x, pos.y, pos.z, id);
}

void ChunkManager::QueueRegen(Chunk* p, int priority)
{
	if(p == nullptr || p->pendingDeletion) return;
	unique_lock<std::mutex> regenLock(regenQueueMutex);
	regenQueue.push({ p, priority });
}

void ChunkManager::QueueRegen(const Vector3Int& index, int priority)
{
	unique_lock<std::mutex> regenLock(regenQueueMutex);
	auto it = chunkMap.find(index);
	if(it != chunkMap.end()) {
		if(it->second == nullptr || it->second->pendingDeletion) return;
		regenQueue.push({ it->second, priority });
	}
}

int ChunkManager::GetBlockLightAtWorldPos(const int& x, const int& y, const int& z) const
{
	tuple<int, int, int> chunkIndex = ToChunkIndexPositionTuple(x, y, z);

	// If chunk is loaded
	if(chunkMap.find(chunkIndex) != chunkMap.end()) {
		Vector3Int localVoxelPos = Vector3Int(FloorMod(x, CHUNKSIZE_X), FloorMod(y, CHUNKSIZE_Y), FloorMod(z, CHUNKSIZE_Z));

		Chunk* chunk = chunkMap.at(chunkIndex);
		if(chunk == nullptr || chunk->pendingDeletion) return -1;
		//AcquireSRWLockExclusive(&chunk->gAccessMutex);
		int light = 0;
		{	unique_lock<std::mutex> lock(chunk->gAccessMutex);
		light = chunk->GetBlockLight(localVoxelPos.x, localVoxelPos.y, localVoxelPos.z);
		}
		//ReleaseSRWLockExclusive(&chunk->gAccessMutex);


		return light;
	}
	return 0;
}

int ChunkManager::GetBlockLightAtWorldPos(const Vector3Int& p) const
{
	return GetBlockLightAtWorldPos(p.x, p.y, p.z);
}

void ChunkManager::SetBlockLightAtWorldPos(const int& x, const int& y, const int& z, const int& val) const {
	tuple<int, int, int> chunkIndex = ToChunkIndexPositionTuple(x, y, z);
	if(chunkMap.find(chunkIndex) != chunkMap.end()) {
		Vector3Int localVoxelPos = Vector3Int(FloorMod(x, CHUNKSIZE_X), FloorMod(y, CHUNKSIZE_Y), FloorMod(z, CHUNKSIZE_Z));

		Chunk* chunk = chunkMap.at(chunkIndex);
		if(chunk == nullptr || chunk->pendingDeletion) return;

		chunk->SetBlockLight(localVoxelPos.x, localVoxelPos.y, localVoxelPos.z, val);
	}
}

void ChunkManager::SetBlockLightAtWorldPos(const Vector3Int& p, const int& val) const {
	SetBlockLightAtWorldPos(p.x, p.y, p.z, val);
}

int ChunkManager::GetSkyLightAtWorldPos(const int& x, const int& y, const int& z)
{
	tuple<int, int, int> chunkIndex = ToChunkIndexPositionTuple(x, y, z);

	// If chunk is loaded
	if(chunkMap.find(chunkIndex) != chunkMap.end()) {
		Vector3Int localVoxelPos = Vector3Int(FloorMod(x, CHUNKSIZE_X), FloorMod(y, CHUNKSIZE_Y), FloorMod(z, CHUNKSIZE_Z));

		Chunk* chunk = chunkMap[chunkIndex];
		if(chunk == nullptr || chunk->pendingDeletion) return -1;

		int light = 0;
		{	unique_lock<std::mutex> lock(chunk->gAccessMutex);
		light = chunk->GetSkyLight(localVoxelPos.x, localVoxelPos.y, localVoxelPos.z);
		}
		return light;
	}
	return -1;
}

int ChunkManager::GetSkyLightAtWorldPos(const Vector3Int& p)
{
	return GetSkyLightAtWorldPos(p.x, p.y, p.z);
}

void ChunkManager::SetSkyLightAtWorldPos(const int& x, const int& y, const int& z, const int& val, bool update) {
	tuple<int, int, int> chunkIndex = ToChunkIndexPositionTuple(x, y, z);
	if(chunkMap.find(chunkIndex) != chunkMap.end()) {
		Vector3Int localVoxelPos = Vector3Int(FloorMod(x, CHUNKSIZE_X), FloorMod(y, CHUNKSIZE_Y), FloorMod(z, CHUNKSIZE_Z));

		Chunk* chunk = chunkMap[chunkIndex];
		//if(Engine::Get()->IsObjDeleted(chunk)) return;

		if(chunk == nullptr || chunk->pendingDeletion) return;

		//AcquireSRWLockExclusive(&chunk->gAccessMutex);
		{	unique_lock<std::mutex> lock(chunk->gAccessMutex);
			chunk->SetSkyLight(localVoxelPos.x, localVoxelPos.y, localVoxelPos.z, val);
		}
		//ReleaseSRWLockExclusive(&chunk->gAccessMutex);
	}
}

void ChunkManager::SetSkyLightAtWorldPos(const Vector3Int& p, const int& val, bool update)
{
	SetSkyLightAtWorldPos(p.x, p.y, p.z, val, update);
}

short ChunkManager::GetRawLightAtWorldPos(const int& x, const int& y, const int& z)
{
	tuple<int, int, int> chunkIndex = ToChunkIndexPositionTuple(x, y, z);
	auto it = chunkMap.find(chunkIndex);
	if(it != chunkMap.end()) {
		Vector3Int localVoxelPos = Vector3Int(FloorMod(x, CHUNKSIZE_X), FloorMod(y, CHUNKSIZE_Y), FloorMod(z, CHUNKSIZE_Z));

		Chunk* chunk = it->second;
		//{	unique_lock<std::mutex> lock(gAccessMutex);
		//	chunk = chunkMap.at(chunkIndex);
		//}
		if(chunk == nullptr || chunk->pendingDeletion) return 0x00;
		//AcquireSRWLockExclusive(&chunk->gAccessMutex);
		//chunk->SetSkyLight(localVoxelPos.x, localVoxelPos.y, localVoxelPos.z, val);
		return chunk->GetRawLight(localVoxelPos.x, localVoxelPos.y, localVoxelPos.z);
		//ReleaseSRWLockExclusive(&chunk->gAccessMutex);
	}
	return 0x00;
}
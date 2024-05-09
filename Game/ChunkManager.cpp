#include "ChunkManager.h"

#include "Chunk.h"
#include "WorldGen.h"
#include "../Engine/Engine.h"
#include "ChunkDatabase.h"
#include "../Engine/ThreadUtil.h"

#include <tuple>

#include "BlockData.h"

bool ChunkManager::CanServiceQueue(GENERATION_PHASE phase)
{
	if(phase == BLOCKDATA) return true;

	bool arePriorQueuesEmpty = true;
	for(int i = 0; i < phase; i++) {
		queue<Chunk*>& queue = chunkGenerationQueues[(GENERATION_PHASE)i].second;

		if(queue.size() > 0) {
			arePriorQueuesEmpty = false;
			break;
		}
	}

	return arePriorQueuesEmpty;
}

void ChunkManager::CreateChunk(const int x, const int y, const int z)
{
	tuple<int, int, int> pos = { x,y,z };
	if(chunkMap.find(pos) == chunkMap.end()) {
		Chunk* newChunk = new Chunk(pos, this);

		{	unique_lock<std::mutex> lock(chunkMapMutex);
			chunkMap[pos] = newChunk;
		}

		newChunk->transform.position = Vector3(static_cast<float>(CHUNKSIZE_X * x), static_cast<float>(CHUNKSIZE_Y * y), static_cast<float>(CHUNKSIZE_Z * z));
		newChunk->indexPosition = Vector3Int(x, y, z);
		//newChunk->renderType = ONCE_PER_FRAME;
		//newChunkQueue.push(newChunk);

		QueueChunkToGenerationPhase(newChunk, BLOCKDATA);
		
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

	ChunkDatabase::Get()->Update(dTime); // Handles autosaving

	Vector3Int camIndex = ChunkFloorPosForPositionCalculation(cam->transform.position);

	return;
	//auto findIt = chunkMap.find(camIndex);

	//map<tuple<int, int, int>, bool> renderedHash = {};
	//renderedHash[camIndex];

	//if(findIt != chunkMap.end()) {
	//	
	//	Vector3 samplePoint = cam->transform.position - (cam->transform.forward() * 16.f);
	//	Vector3Int behindChunk = ChunkFloorPosForPositionCalculation(samplePoint);

	//	auto findMap = findIt->second->visibilityGraph.find(behindChunk);
	//	if(findMap != findIt->second->visibilityGraph.end()) {

	//		for(const auto& pair : findMap->second) {
	//			renderedHash[pair.first];
	//		}
	//	}
	//	
	//	/*for(const auto& pairMapHash : findIt->second->visibilityGraph) {
	//		
	//	}*/

	//}

	//for(auto& kvp : chunkMap) {
	//	if(kvp.second == nullptr || kvp.second->pendingDeletion || !kvp.second->hasRanStartFunction) {
	//		continue;
	//	}

	//	auto findVisited = renderedHash.find(kvp.second->indexPosition);
	//	kvp.second->doRender = findVisited != renderedHash.end();

	//}
	//return;



	// https://tomcc.github.io/2014/08/31/visibility-2.html
	//	 navigate visiblity graphs
	

	map<tuple<int, int, int>, bool> renderedHash = {};

	const Vector3Int neighbourOffsets[] = {
		Vector3Int(1,0,0),
		Vector3Int(-1,0,0),
		Vector3Int(0,0,1),
		Vector3Int(0,0,-1),
		Vector3Int(0,1,0),
		Vector3Int(0,-1,0),
	};

	//queue of steps { chunk we want to visit : chunk we came from }
	queue<pair<Vector3Int, Vector3Int>> bfs;
	bfs.push({ camIndex, camIndex });

	while(!bfs.empty()) {
		pair<Vector3Int, Vector3Int> front = bfs.front();
		bfs.pop();
		Vector3Int& targetChunk = front.first;
		Vector3Int& previousChunk = front.second;

		Chunk* pTargetChunk;
		auto findIt = chunkMap.find(targetChunk);
		if(findIt == chunkMap.end()) {
			continue;
		}
		pTargetChunk = findIt->second;

		renderedHash[targetChunk] = true;

		//Vector3 delta = ((Vector3)(targetChunk - previousChunk)).normalized();

		for(const Vector3Int& offset : neighbourOffsets) {
			Vector3Int nextIndex = (targetChunk + offset);

			Vector3 delta = ((Vector3)(nextIndex - previousChunk)).normalized();

			float dot = Vector3::dot(offset, cam->transform.forward());

			if(dot > 0.0f) {
					

				if(pTargetChunk->IsChunkVisibleFromChunk(nextIndex, previousChunk)) {
					auto findIt = chunkMap.find(nextIndex);
					if(findIt != chunkMap.end()) {
						
						if(cam->IsAABBInFrustum(findIt->second->cullBox)) {
							
							if(renderedHash.find(nextIndex) == renderedHash.end()) {
								bfs.push({nextIndex, targetChunk});
							}
						}

					}
				}
			}
		}

		
	}

	for(auto& kvp : chunkMap) {
		if(kvp.second == nullptr || kvp.second->pendingDeletion || !kvp.second->hasRanStartFunction) {
			continue;
		}

		auto findVisited = renderedHash.find(kvp.second->indexPosition);
		kvp.second->doRender = findVisited != renderedHash.end();

	}
}

void ChunkManager::Thread() {
	Vector3Int prevCamIndex = ChunkFloorPosForPositionCalculation(cam->transform.position);
	bool forceRegenerateVisibility = false;
	while(isRunning) {
		Vector3Int camIndex = ChunkFloorPosForPositionCalculation(cam->transform.position);

		//if(newChunkQueue.empty())
		for(int y = 1 - CHUNKLOAD_FIXED_NY; y < CHUNKLOAD_FIXED_PY + 1; y++) {
			for(int x = 1 - CHUNKLOAD_AREA_X; x < CHUNKLOAD_AREA_X + 1; x++) {
				for(int z = 1 - CHUNKLOAD_AREA_Z; z < CHUNKLOAD_AREA_Z + 1; z++) {
					CreateChunk(camIndex.x + x, y, camIndex.z + z);
				}
			}
		}

		queue<Chunk*>& blockdataPendingQueue = chunkGenerationQueues[BLOCKDATA].second;
		while(!blockdataPendingQueue.empty()) {
			Chunk* chunk = blockdataPendingQueue.front();
			chunk->SetBusy(true);

			threadPool->Queue([=] {

				if(ChunkDatabase::Get()->DoesDataExistForChunk(chunk->indexPosition)) {
					ChunkDatabase::Get()->LoadChunkDataInto(chunk->indexPosition, chunk);

					this->QueueRegen(chunk->indexPosition + Vector3Int(1, 0, 0));
					this->QueueRegen(chunk->indexPosition + Vector3Int(-1, 0, 0));
					this->QueueRegen(chunk->indexPosition + Vector3Int(0, 1, 0));
					this->QueueRegen(chunk->indexPosition + Vector3Int(0, -1, 0));
					this->QueueRegen(chunk->indexPosition + Vector3Int(0, 0, 1));
					this->QueueRegen(chunk->indexPosition + Vector3Int(0, 0, -1));
				}
				else chunk->GenerateBlockData();

				chunk->InitSkyLight();

				chunk->currentGenerationPhase = VISIBILITY_GRAPH;
				this->QueueChunkToGenerationPhase(chunk, VISIBILITY_GRAPH);

				chunk->SetBusy(false);

			}, chunk->indexPosition.y == CHUNKLOAD_FIXED_PY); // If the chunk is at the TOP, the higher priority value means it will be completed last (or it should be...)

			blockdataPendingQueue.pop();
		}

		if(CanServiceQueue(VISIBILITY_GRAPH)) { // Makes it wait until the queues preceeding this phase are empty
			queue<Chunk*>& visPendingQueue = chunkGenerationQueues[VISIBILITY_GRAPH].second;
			while(!visPendingQueue.empty()) {
				unique_lock<mutex> lock(chunkGenerationQueues[VISIBILITY_GRAPH].first);

				Chunk* chunk = visPendingQueue.front();

				if(chunk != nullptr) {
					
					threadPool->Queue([=] {
						chunk->BuildVisibilityGraph();

						chunk->currentGenerationPhase = LIGHTING;
						this->QueueChunkToGenerationPhase(chunk, LIGHTING);
					});

				}
				visPendingQueue.pop();
			}
		}

		//0xF ends up in the queue sometimes on release builds hmm
		if(CanServiceQueue(LIGHTING)) { // Makes it wait until the queues preceeding this phase are empty
			queue<Chunk*>& lightingPendingQueue = chunkGenerationQueues[LIGHTING].second;
			while(!lightingPendingQueue.empty()) {
				unique_lock<mutex> lock(chunkGenerationQueues[LIGHTING].first);
				Chunk* chunk = lightingPendingQueue.front();

				if(chunk != nullptr) {
					// do work here

					chunk->currentGenerationPhase = MESH;
					QueueChunkToGenerationPhase(chunk, MESH);
			
				}
				lightingPendingQueue.pop();
			}
		}

		if(CanServiceQueue(MESH)) {
			queue<Chunk*>& meshPendingQueue = chunkGenerationQueues[MESH].second;
			while(!meshPendingQueue.empty()) {
				unique_lock<mutex> lock(chunkGenerationQueues[MESH].first);
				Chunk* chunk = meshPendingQueue.front();
				threadPool->Queue([=, &forceRegenerateVisibility] {
					chunk->Finalize();
					forceRegenerateVisibility = true;

					unique_lock<mutex> lock(this->_numChunksLoadedMutex);
					this->_numChunksLoaded++;
					chunk->currentGenerationPhase = DONE;

				});

				meshPendingQueue.pop();
			}
		}

		{	unique_lock<std::mutex> lock(regenQueueMutex);
			while(!regenQueue.empty()) {
				forceRegenerateVisibility = true;
				pair<Chunk*,int> queuePair = regenQueue.front();
				threadPool->Queue([queuePair] {
					if(queuePair.first == nullptr || queuePair.first->pendingDeletion) return;

					unique_lock<std::mutex> lock(queuePair.first->gAccessMutex);
					queuePair.first->BuildVisibilityGraph();
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

				if(!pair.second->IsBusy()) { //if chunk isnt busy
					{
						//unique_lock<std::mutex> lock(pair.second->gAccessMutex);
						unique_lock<std::mutex> destroyLock(*Engine::Get()->GetDestroyObjectsMutex());

						ChunkDatabase::Get()->UnloadChunk(indexPos);
						engine->DestroyObject3D(pair.second); // Delete chunk in-engine (adds to queue)
					}

					{	unique_lock<std::mutex> lock(chunkMapMutex);
						//.lock();
						chunkMap.erase(it++);
					}
				}
			}
			else { // Increment iterator 
				++it;
			}
		}

		//


		
		if(prevCamIndex != camIndex || forceRegenerateVisibility) {

			const Vector3Int neighbourOffsets[] = {
				Vector3Int(1,0,0),
				Vector3Int(-1,0,0),
				Vector3Int(0,0,1),
				Vector3Int(0,0,-1),
				Vector3Int(0,1,0),
				Vector3Int(0,-1,0),
			};

			map<tuple<int, int, int>, bool> renderedHash = {};

			//Vector3 forward = camTrans->forward();
			
			for(const Vector3& forward : neighbourOffsets) {
				//queue of steps { chunk we want to visit : chunk we came from }
				queue<pair<Vector3Int, Vector3Int>> bfs;

				bfs.push({ camIndex, camIndex });

				while(!bfs.empty()) {
					pair<Vector3Int, Vector3Int> front = bfs.front();
					bfs.pop();
					Vector3Int& targetChunk = front.first;
					Vector3Int& previousChunk = front.second;

					if(targetChunk != camIndex ) {
					//if(Vector3::Distance(targetChunk, camIndex) > 2.f) {
						auto find = renderedHash.find(targetChunk);
						if(find != renderedHash.end()) continue;
					}

					renderedHash[targetChunk]=true;

					if(Input::IsKeyHeld('N')) {
						for(auto& kvp : chunkMap) {
							if(kvp.second == nullptr || kvp.second->pendingDeletion || !kvp.second->hasRanStartFunction) {
								continue;
							}

							auto findVisited = renderedHash.find(kvp.second->indexPosition);
							kvp.second->doRender = findVisited != renderedHash.end();
						}
						__nop();
					}

					for(const Vector3& offset : neighbourOffsets) {
						Vector3Int nextChunk = targetChunk + offset;
						Vector3 direction = Vector3(Vector3(nextChunk.x, nextChunk.y, nextChunk.z) - Vector3(camIndex.x, camIndex.y, camIndex.z)).normalized();
						
						//float OdotV = Vector3::dot(offset, forward);
						float OdotV = Vector3::dot(direction, forward);

						if(OdotV > 0.501f) { // 1.


							Chunk* pTargetChunk = nullptr;
							auto findChunk = chunkMap.find(targetChunk);
							if(findChunk != chunkMap.end()) {
								pTargetChunk = findChunk->second;

								// skip incomplete chunks
								if(pTargetChunk == nullptr || pTargetChunk->pendingDeletion || !pTargetChunk->hasRanStartFunction) continue;


								if(pTargetChunk->IsChunkVisibleFromChunk(nextChunk, previousChunk)) { //2.
									
									

									bfs.push({ targetChunk + offset, targetChunk });
								}
							}
						}
					}
				}
			}


			for(auto& kvp : chunkMap) {
				if(kvp.second == nullptr || kvp.second->pendingDeletion || !kvp.second->hasRanStartFunction) {
					continue;
				}

				auto findVisited = renderedHash.find(kvp.second->indexPosition);
				kvp.second->doRender = findVisited != renderedHash.end();
			}
		}

#if 0
		// Chunk visibility occlusion recalculation
		if(prevCamIndex != camIndex || forceRegenerateVisibility) { // only do it if the camera crosses a chunk border
			Vector3 camFwd = camTrans->forward(); // maybe use in future for dot product calc

			map<tuple<int, int, int>, UINT8> visitedHash = {}; // list of chunks that have been crossed
			queue<pair<tuple<int, int, int>, tuple<int, int, int>>> visibilityBfs; // queue of target chunk : chunk we came from
			visibilityBfs.push({ camIndex, camIndex }); // add the current chunk to the queue

			const Vector3Int neighbourOffsets[] = {
				Vector3Int(1,0,0),
				Vector3Int(-1,0,0),
				Vector3Int(0,1,0),
				Vector3Int(0,-1,0),
				Vector3Int(0,0,1),
				Vector3Int(0,0,-1),
			};

			while(!visibilityBfs.empty()) {
				// get information then pop queue
				auto& front = visibilityBfs.front();
				Vector3Int chunkWeAreVisiting = front.first;
				Vector3Int chunkWeCameFrom = front.second;
				visibilityBfs.pop();

				//auto findVisited = visitedHash.find(chunkWeAreVisiting);
				//if(findVisited == visitedHash.end() || (findVisited!=visitedHash.end() && findVisited->second<6)) { // if the chunk hasnt been visited already
					// this check is wrong and results in errors
					// todo: properly implement correct filters

				visitedHash[chunkWeAreVisiting]++; // register as crossed chunk

				Chunk* pChunkWeAreVisiting = nullptr;
				auto findVisit = chunkMap.find(chunkWeAreVisiting);
				if(findVisit != chunkMap.end()) { // get the chunk pointer
					pChunkWeAreVisiting = findVisit->second;

					// skip incomplete chunks
					if(pChunkWeAreVisiting == nullptr || pChunkWeAreVisiting->pendingDeletion || !pChunkWeAreVisiting->hasRanStartFunction) continue;



					//todo: add more filters
					for(const Vector3Int& offset : neighbourOffsets) { // check all neighbours
						if(chunkWeAreVisiting + offset == chunkWeCameFrom) continue; // Dont go back
						
						float dirDotView = Vector3::dot(Vector3(chunkWeAreVisiting + offset) - Vector3(chunkWeCameFrom), camFwd);
						if(dirDotView < 0.f) continue;

						const Vector3 chunkSize = Vector3(static_cast<float>(CHUNKSIZE_X), static_cast<float>(CHUNKSIZE_Y), static_cast<float>(CHUNKSIZE_Z));
						Vector3 chunkWorldPos = Vector3(Vector3(chunkWeAreVisiting) + Vector3(offset)) ^ chunkSize;

						if(!Graphics::Get()->camera.IsAABBInFrustum(AABB(chunkWorldPos + (Vector3(0.5f, 0.5f, 0.5f) ^ chunkSize), (Vector3(0.5f, 0.5f, 0.5f) ^ chunkSize))))
							continue;

						if(pChunkWeAreVisiting->IsChunkVisibleFromChunk(chunkWeAreVisiting + offset, chunkWeCameFrom)) {
							visibilityBfs.push({ chunkWeAreVisiting + offset, chunkWeAreVisiting });
						}
					}
				}
				//}
			}

			for(auto& kvp : chunkMap) {
				if(kvp.second == nullptr || kvp.second->pendingDeletion || !kvp.second->hasRanStartFunction) {
					continue;
				}

				auto findVisited = visitedHash.find(kvp.second->indexPosition);
				kvp.second->doRender = findVisited != visitedHash.end();
			}
		}
#endif


		//
		forceRegenerateVisibility = false;
		prevCamIndex = camIndex;
	}
}

void ChunkManager::QueueChunkToGenerationPhase(Chunk* chunk, GENERATION_PHASE phase)
{
	//auto findIt = chunkGenerationQueues.find(phase);
	//if(findIt == chunkGenerationQueues.end()) {
	//	//chunkGenerationQueues[phase] = pair<std::mutex, queue<Chunk*>>({}, {});
	//	//chunkGenerationQueues[phase] = tuple<std::mutex, queue<Chunk*>>(std::mutex(), queue<Chunk*>());
	//	chunkGenerationQueues[phase];
	//}

	//pair<Chunk::GENERATION_PHASE, tuple<std::mutex, queue<Chunk*>>>& refPair = chunkGenerationQueues[phase];
	auto& refPair = chunkGenerationQueues[phase]; //does this construct it?
	
	std::unique_lock<std::mutex> lock(refPair.first);
	refPair.second.push(chunk);
}

void ChunkManager::Start()
{
	ChunkDatabase::Init();

	threadPool = new ThreadPool();
	threadPool->thread_count = 6u;
	threadPool->namingScheme = "ChnkMgr Worker";
	threadPool->Init();

	engine = Engine::Get();
	cam = &Graphics::Get()->camera;

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

	threadPool->Stop();

	isRunning = false;
	for(auto& thread : chnkMgrThread) {
		thread.join();
	}

	delete threadPool;

	ChunkDatabase::Get()->Release();
}

tuple<int, int, int> ChunkManager::ToChunkIndexPositionTuple(const int& x, const int& y, const int& z)
{
	return tuple<int, int, int>(
		(x - ((CHUNKSIZE_X - 1) * static_cast<int>(x < 0))) / CHUNKSIZE_X,
		(y - ((CHUNKSIZE_Y - 1) * static_cast<int>(y < 0))) / CHUNKSIZE_Y,
		(z - ((CHUNKSIZE_Z - 1) * static_cast<int>(z < 0))) / CHUNKSIZE_Z
	);
}

BlockID ChunkManager::GetBlockAtWorldPos(const int& x, const int& y, const int& z, bool worldgenFallback)
{

	if(this == nullptr || this->pendingDeletion) return WorldGen::GetBlockAt(x, y, z);
	tuple<int, int, int> chunkIndex = ToChunkIndexPositionTuple(x, y, z);

	// If chunk is loaded
	//bool tryLockChunkMap = chunkMapMutex.try_lock();
	chunkMapMutex.lock();
	auto itFind = chunkMap.find(chunkIndex);
	if(itFind != chunkMap.end()) {
		Vector3Int localVoxelPos = Vector3Int(FloorMod(x, CHUNKSIZE_X), FloorMod(y, CHUNKSIZE_Y), FloorMod(z, CHUNKSIZE_Z));

		Chunk* chunk = itFind->second;
		chunkMapMutex.unlock();

		if(!(chunk == nullptr || chunk->pendingDeletion || chunk->currentGenerationPhase==BLOCKDATA)) {
			bool didMutex = chunk->gAccessMutex.try_lock();

			BlockID blockID = static_cast<BlockID>(chunk->blockData[localVoxelPos.x][localVoxelPos.y][localVoxelPos.z]);
			if(didMutex) chunk->gAccessMutex.unlock();

			return blockID;
		}

		if(worldgenFallback) {
			return WorldGen::GetBlockAt(x, y, z);
		}
		else {
			return ERR;
		}
	}
	chunkMapMutex.unlock();

	if(worldgenFallback) {
		//todo: read from chunk cache of height,temp,moist samples?
		return WorldGen::GetBlockAt(x, y, z);
	}
	else {
		return ERR;
	}
}

BlockID ChunkManager::GetBlockAtWorldPos(const Vector3Int& v, bool worldgenFallback) {
	return GetBlockAtWorldPos(v.x, v.y, v.z, worldgenFallback);
}

void ChunkManager::SetBlockAtWorldPos(const int& x, const int& y, const int& z, const BlockID& id) {
	Vector3Int chunkIndex = ToChunkIndexPositionTuple(x, y, z);

	//unique_lock<std::mutex> lock(chunkMapMutex);

	auto findIt = chunkMap.find(chunkIndex);
	if(findIt != chunkMap.end()) {
		Vector3Int localVoxelPos = Vector3Int(FloorMod(x, CHUNKSIZE_X), FloorMod(y, CHUNKSIZE_Y), FloorMod(z, CHUNKSIZE_Z));

		Chunk*& chunk = findIt->second;
		if(chunk == nullptr) {
			assert(false);
			return;
		}

		const BlockID oldBlock = (BlockID)chunk->blockData[localVoxelPos.x][localVoxelPos.y][localVoxelPos.z];

		const Block& newBlockDef = BlockDef::GetDef(id);
		const Block& oldBlockDef = BlockDef::GetDef(oldBlock);

		chunk->blockData[localVoxelPos.x][localVoxelPos.y][localVoxelPos.z] = id;


		if(id == AIR) { // if block with blockdata is being removed
			auto findIt = chunk->blockDataData.find({ x,y,z });
			if(findIt != chunk->blockDataData.end()) { // if blockData exists for this block
				findIt->second->TryDropItems({ (float)x,(float)y,(float)z });
			}

			BlockID blockAbove = chunk->GetBlockIncludingNeighbours(localVoxelPos.x, localVoxelPos.y + 1, localVoxelPos.z);
			if(blockAbove != AIR) {
				const Block& blockDefAbove = BlockDef::GetDef(blockAbove);
				if(blockDefAbove.HasTag(BT_GROUNDED)) {
					SetBlockAtWorldPos(x, y+1, z, AIR);
				}
			}
		}

		//lighting->QueueRemoveSkyLight({ localVoxelPos, chunk, (short)15 });
		//lighting->QueueSkyLight({ localVoxelPos, chunk });

		// If the new block placed had a light value
		if(newBlockDef.LightValue() != 0) {
			chunk->SetBlockLight(localVoxelPos.x, localVoxelPos.y, localVoxelPos.z, newBlockDef.LightValue());
			chunk->RecalculateBlockLighting();
		}
		//else {
		//	chunk->SetBlockLight(localVoxelPos.x, localVoxelPos.y, localVoxelPos.z, 0);
		//}

		//// If a light is being removed
		//if(oldBlockDef.LightValue() != 0 && id == AIR) {
		//	lighting->QueueRemoveBlockLight({ localVoxelPos, chunk, static_cast<short>(oldBlockDef.LightValue()) });


		//	chunk->SetBlockLight(localVoxelPos.x, localVoxelPos.y, localVoxelPos.z, 0);
		//	lighting->PopBlockLightQueue();
		//}


		//if(oldBlockDef.LightValue() && newBlockDef.LightValue()) {
		//	lighting->QueueRemoveBlockLight({ localVoxelPos, chunk, (short)chunk->GetBlockLight(localVoxelPos.x, localVoxelPos.y, localVoxelPos.z) });

		//	if(id != AIR) {
		//		chunk->SetBlockLight(localVoxelPos.x, localVoxelPos.y, localVoxelPos.z, 0);
		//	}
		//	else {
		//		chunk->SetBlockLight(localVoxelPos.x, localVoxelPos.y, localVoxelPos.z, chunk->GetBlockLight(localVoxelPos.x, localVoxelPos.y, localVoxelPos.z));
		//	}
		//}

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
		if(it->second == nullptr || it->second->pendingDeletion || it->second->currentGenerationPhase==BLOCKDATA) return;
		regenQueue.push({ it->second, priority });
	}
}

int ChunkManager::GetBlockLightAtWorldPos(const int& x, const int& y, const int& z)
{
	tuple<int, int, int> chunkIndex = ToChunkIndexPositionTuple(x, y, z);

	unique_lock<std::mutex> lock(chunkMapMutex);
	// If chunk is loaded
	auto itFind = chunkMap.find(chunkIndex);
	if(itFind != chunkMap.end()) {
		Vector3Int localVoxelPos = Vector3Int(FloorMod(x, CHUNKSIZE_X), FloorMod(y, CHUNKSIZE_Y), FloorMod(z, CHUNKSIZE_Z));

		Chunk* chunk = itFind->second;
		if(chunk == nullptr || chunk->pendingDeletion) return -1;
		//AcquireSRWLockExclusive(&chunk->gAccessMutex);
		int light = 0;
		//{	unique_lock<std::mutex> lock(chunk->gAccessMutex);
		light = chunk->GetBlockLight(localVoxelPos.x, localVoxelPos.y, localVoxelPos.z);
		//}
		//ReleaseSRWLockExclusive(&chunk->gAccessMutex);


		return light;
	}
	return 15;
}

int ChunkManager::GetBlockLightAtWorldPos(const Vector3Int& p)
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

	unique_lock<std::mutex> lock(chunkMapMutex);
	// If chunk is loaded
	auto findIt = chunkMap.find(chunkIndex);
	if(findIt != chunkMap.end()) {
		Vector3Int localVoxelPos = Vector3Int(FloorMod(x, CHUNKSIZE_X), FloorMod(y, CHUNKSIZE_Y), FloorMod(z, CHUNKSIZE_Z));

		Chunk* chunk = findIt->second;
		if(chunk == nullptr || chunk->pendingDeletion) return -1;

		int light = 0;
		{	//unique_lock<std::mutex> lock(chunk->gAccessMutex);
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
	
	unique_lock<std::mutex> lock(chunkMapMutex);
	auto findIt = chunkMap.find(chunkIndex);
	if(findIt != chunkMap.end()) {
		Vector3Int localVoxelPos = Vector3Int(FloorMod(x, CHUNKSIZE_X), FloorMod(y, CHUNKSIZE_Y), FloorMod(z, CHUNKSIZE_Z));

		Chunk* chunk = findIt->second;
		//if(Engine::Get()->IsObjDeleted(chunk)) return;

		if(chunk == nullptr || chunk->pendingDeletion) return;

		//AcquireSRWLockExclusive(&chunk->gAccessMutex);
		{	//unique_lock<std::mutex> lock(chunk->gAccessMutex);
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
	unique_lock<std::mutex> lock(chunkMapMutex);
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
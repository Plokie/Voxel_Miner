#include "ChunkManager.h"

#include "ChunkDatabase.h"

Chunk* ChunkManager::CreateChunk(int x, int y, int z)
{
	// if chunk already exists, return
	if (chunkMap.count(tuple<int, int, int>(x, y, z))) {
		return nullptr;
	}

	Chunk* newChunk = new Chunk(Vector3Int(x, y, z), this);
		
	InitializeSRWLock(&newChunk->gAccessMutex);
	//Chunk* newChunk = (Chunk*)this->pEngine->GetCurrentScene()->CreateObject3D(
	//	new Chunk(Vector3Int(x, y, z), this), // chunk instance w/ initialised values (position and reference to ChunkManager)
	//	"_c" + to_string(x) + "_" + to_string(y) + "_" + to_string(z) // Object3D name (using index as string)
	//);
	
	// something is happening between line 9 and inside Load() (when setting block data) which is corrupting the chunk
	// was it deleted on the previous frame?

	AcquireSRWLockExclusive(&newChunk->gAccessMutex);

	newChunk->transform.position = Vector3(static_cast<float>(CHUNKSIZE_X * x), static_cast<float>(CHUNKSIZE_Y * y), static_cast<float>(CHUNKSIZE_Z * z));
	bool didChunkExistAlready = newChunk->Load();
	ReleaseSRWLockExclusive(&newChunk->gAccessMutex);

	this->chunkMap[tuple<int, int, int>(x, y, z)] = newChunk;

	if(didChunkExistAlready) { // If chunk had data that existed in the databse
		TryRegen(Vector3Int(x+1, y, z)); // regenerate neighbouring chunks that might have old neighbour data
		TryRegen(Vector3Int(x-1, y, z));
		TryRegen(Vector3Int(x, y+1, z));
		TryRegen(Vector3Int(x, y-1, z));
		TryRegen(Vector3Int(x, y, z+1));
		TryRegen(Vector3Int(x, y, z-1));
	}

	


	this->pEngine->GetCurrentScene()->CreateObject3D(
		newChunk,
		"_c" + to_string(x) + "_" + to_string(y) + "_" + to_string(z)
	);

	return newChunk;
}

Vector3Int ChunkManager::ChunkFloorPosForPositionCalculation(Vector3 worldPosition) {
	return Vector3Int(
		static_cast<int>(floorf(worldPosition.x / CHUNKSIZE_X)), 
		static_cast<int>(floorf(worldPosition.y / CHUNKSIZE_Y)), 
		static_cast<int>(floorf(worldPosition.z / CHUNKSIZE_Z))
	);
}

Vector3Int ChunkManager::ToChunkIndexPosition(const int& x, const int& y, const int& z) {
	// this could really be done using a ternary operator, and would be more readable,
	// however because this function is used so much, this is much faster since theres no branching
	return Vector3Int(
		(x - ((CHUNKSIZE_X - 1) * static_cast<int>(x < 0))) / CHUNKSIZE_X,
		(y - ((CHUNKSIZE_Y - 1) * static_cast<int>(y < 0))) / CHUNKSIZE_Y,
		(z - ((CHUNKSIZE_Z - 1) * static_cast<int>(z < 0))) / CHUNKSIZE_Z
	);
}

tuple<int, int, int> ChunkManager::ToChunkIndexPositionTuple(const int& x, const int& y, const int& z)
{
	return tuple<int, int, int>(
		(x - ((CHUNKSIZE_X - 1) * static_cast<int>(x < 0))) / CHUNKSIZE_X,
		(y - ((CHUNKSIZE_Y - 1) * static_cast<int>(y < 0))) / CHUNKSIZE_Y,
		(z - ((CHUNKSIZE_Z - 1) * static_cast<int>(z < 0))) / CHUNKSIZE_Z
	);
}

BlockID ChunkManager::GetBlockAtWorldPos(const int& x, const int& y, const int& z) const
{
	tuple<int,int,int> chunkIndex = ToChunkIndexPositionTuple(x, y, z);

	// If chunk is loaded
	if(chunkMap.find(chunkIndex) != chunkMap.end()) {
		Vector3Int localVoxelPos = Vector3Int(FloorMod(x, CHUNKSIZE_X), FloorMod(y, CHUNKSIZE_Y), FloorMod(z, CHUNKSIZE_Z));
		
		Chunk* chunk = chunkMap.at(chunkIndex);
		AcquireSRWLockExclusive(&chunk->gAccessMutex);
		BlockID blockID = static_cast<BlockID>(chunk->blockData[localVoxelPos.x][localVoxelPos.y][localVoxelPos.z]);
		ReleaseSRWLockExclusive(&chunk->gAccessMutex);
		return blockID;
	}
	else { // If chunk isn't loaded, sample from world gen instead (next best thing)

		//todo: read from chunk cache of height,temp,moist samples?
		return WorldGen::GetBlockAt(x, y, z);

	}

}

BlockID ChunkManager::GetBlockAtWorldPos(const Vector3Int& v) const {
	return GetBlockAtWorldPos(v.x, v.y, v.z);
}

void ChunkManager::TryRegen(Vector3Int chunkCoords) {
	if(chunkMap.count(chunkCoords)) {
		chunkMap[chunkCoords]->BuildMesh();
	}
}


void ChunkManager::SetBlockAtWorldPos(const int& x, const int& y, const int& z, const BlockID& id) {
	Vector3Int chunkIndex = ToChunkIndexPosition(x, y, z);
	if(chunkMap.count(chunkIndex)) {
		Vector3Int localVoxelPos = Vector3Int(FloorMod(x, CHUNKSIZE_X), FloorMod(y, CHUNKSIZE_Y), FloorMod(z, CHUNKSIZE_Z));

		chunkMap[chunkIndex]->blockData[localVoxelPos.x][localVoxelPos.y][localVoxelPos.z] = (USHORT)id;

		//TODO: READ LIGHT DATA FROM BLOCK DATA, THIS IS JUST A TEST
		if(id == LAMP) chunkMap[chunkIndex]->SetBlockLight(localVoxelPos.x, localVoxelPos.y, localVoxelPos.z, 15);

		chunkMap[chunkIndex]->BuildMesh();


		if(localVoxelPos.x == 0) { // Regen -x neighbour
			TryRegen(chunkIndex + Vector3(-1, 0, 0));
		}
		else if(localVoxelPos.x == CHUNKSIZE_X - 1) { // Regen +x neighbour
			TryRegen(chunkIndex + Vector3(1, 0, 0));
		}

		if(localVoxelPos.y == 0) {  // -y border
			TryRegen(chunkIndex + Vector3(0, -1, 0));
		}
		else if(localVoxelPos.y == CHUNKSIZE_Y - 1) { // +y border
			TryRegen(chunkIndex + Vector3(0, 1, 0));
		}

		if(localVoxelPos.z == 0) {	// -z border
			TryRegen(chunkIndex + Vector3(0, 0, -1));
		}
		else if(localVoxelPos.z == CHUNKSIZE_Z - 1) { // +z border
			TryRegen(chunkIndex + Vector3(0, 0, 1));
		}

		ChunkDatabase::Get()->SaveChunkData(chunkIndex, chunkMap[chunkIndex]);
	}
}

void ChunkManager::SetBlockAtWorldPos(const Vector3Int& pos, const BlockID& id) {
	SetBlockAtWorldPos(pos.x, pos.y, pos.z, id);
}



int ChunkManager::GetBlockLightAtWorldPos(const int& x, const int& y, const int& z) const
{
	tuple<int, int, int> chunkIndex = ToChunkIndexPositionTuple(x, y, z);

	// If chunk is loaded
	if(chunkMap.find(chunkIndex) != chunkMap.end()) {
		Vector3Int localVoxelPos = Vector3Int(FloorMod(x, CHUNKSIZE_X), FloorMod(y, CHUNKSIZE_Y), FloorMod(z, CHUNKSIZE_Z));

		Chunk* chunk = chunkMap.at(chunkIndex);
		AcquireSRWLockExclusive(&chunk->gAccessMutex);
		int light = chunk->GetBlockLight(localVoxelPos.x, localVoxelPos.y, localVoxelPos.z);
		ReleaseSRWLockExclusive(&chunk->gAccessMutex);
		return light;
	}
	return 0;
}

int ChunkManager::GetBlockLightAtWorldPos(const Vector3Int& p) const
{
	return GetBlockAtWorldPos(p.x,p.y,p.z);
}

void ChunkManager::SetBlockLightAtWorldPos(const int& x, const int& y, const int& z, const int& val) const {
	tuple<int, int, int> chunkIndex = ToChunkIndexPositionTuple(x, y, z);
	if(chunkMap.find(chunkIndex) != chunkMap.end()) {
		Vector3Int localVoxelPos = Vector3Int(FloorMod(x, CHUNKSIZE_X), FloorMod(y, CHUNKSIZE_Y), FloorMod(z, CHUNKSIZE_Z));

		Chunk* chunk = chunkMap.at(chunkIndex);
		AcquireSRWLockExclusive(&chunk->gAccessMutex);
		chunk->SetBlockLight(localVoxelPos.x, localVoxelPos.y, localVoxelPos.z, val);
		ReleaseSRWLockExclusive(&chunk->gAccessMutex);
	}
}

void ChunkManager::SetBlockLightAtWorldPos(const Vector3Int& p, const int& val) const
{
	SetBlockLightAtWorldPos(p.x, p.y, p.z, val);
}

ChunkManager* ChunkManager::Create(Transform* cameraTransform) {
	ChunkManager* chnkMgr = new ChunkManager();
	chnkMgr->Init(cameraTransform);
	return chnkMgr;
}

void ChunkManager::Init(Transform* cameraTransform)
{
	this->pCameraTransform = cameraTransform;
	this->pEngine = Engine::Get();
}

void ChunkManager::LoaderThreadFunc(Transform* camTransform, map<tuple<int,int,int>, Chunk*>* pChunkMap)
{
	Engine* engine = Engine::Get();
	SRWLOCK* pDestroyMutex = engine->GetDestroyObjectsMutex();
	//SRWLOCK* pCreateMutex = engine->GetCreateObjectsMutex();

	while(_isRunning) {
		Vector3Int camIndex = ChunkFloorPosForPositionCalculation(camTransform->position);

		for(int y = 1 - CHUNKLOAD_AREA_NY; y < CHUNKLOAD_AREA_PY + 1; y++) {
			for(int x = 1 - CHUNKLOAD_AREA_X; x < CHUNKLOAD_AREA_X + 1; x++) {
				for(int z = 1 - CHUNKLOAD_AREA_Z; z < CHUNKLOAD_AREA_Z + 1; z++) {
					CreateChunk(camIndex.x + x, camIndex.y + y, camIndex.z + z);

					if(!_isRunning) return;
				}
			}
		}

		AcquireSRWLockExclusive(pDestroyMutex);
		for(auto it = pChunkMap->cbegin(); it != pChunkMap->cend();) {
			const pair<tuple<int, int, int>, Chunk*>& pair = *it;

			Vector3Int indexPos = Vector3Int(get<0>(pair.first), get<1>(pair.first), get<2>(pair.first));

			if(
				abs(indexPos.x - camIndex.x) > CHUNKLOAD_AREA_X ||
				indexPos.y - camIndex.y > CHUNKLOAD_AREA_PY ||
				camIndex.y - indexPos.y  > CHUNKLOAD_AREA_NY ||
				abs(indexPos.z - camIndex.z) > CHUNKLOAD_AREA_Z
				) { // Erase chunk from map (it is out of range)

				ChunkDatabase::Get()->UnloadChunk(indexPos);
				engine->DestroyObject3D(pair.second); // Delete chunk in-engine (adds to queue)

				pChunkMap->erase(it++);
			}
			else { // Increment iterator 
				++it;
			}
		}
		ReleaseSRWLockExclusive(pDestroyMutex);
	}
}

void ChunkManager::Start()
{
	ChunkDatabase::Init();
	if (this->pEngine == nullptr || this->pCameraTransform == nullptr ) {
		exit(204); 
	}

	_chunkLoaderThreads.emplace_back([&]() { LoaderThreadFunc(pCameraTransform, &chunkMap); });
	lighting = new Lighting(this);
	lighting->StartThread();
}

void ChunkManager::Update(float dt)
{

}

ChunkManager::~ChunkManager()
{
	_isRunning = false;
	for(thread& thread : _chunkLoaderThreads) {
		thread.join();
	}

	if(lighting) delete lighting;

	ChunkDatabase::Get()->SaveChunks();
	ChunkDatabase::Get()->SaveWorldData();

	//for(const pair<tuple<int, int, int>, Chunk*>& pair : this->chunkMap) {
	//	delete pair.second;
	//}
}

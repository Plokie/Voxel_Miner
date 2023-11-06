#include "ChunkManager.h"

Chunk* ChunkManager::CreateChunk(int x, int y, int z)
{
	// if chunk already exists, return
	if (chunkMap.count(tuple<int, int, int>(x, y, z))) {
		return nullptr;
	}

	Chunk* newChunk = (Chunk*)this->pEngine->GetCurrentScene()->CreateObject3D(
		new Chunk(Vector3Int(x, y, z), this), // chunk instance w/ initialised values (position and reference to ChunkManager)
		"_c" + to_string(x) + "_" + to_string(y) + "_" + to_string(z) // Object3D name (using index as string)
	);
	
	// something is happening between line 9 and inside Load() (when setting block data) which is corrupting the chunk
	// was it deleted on the previous frame?

	AcquireSRWLockExclusive(&newChunk->gAccessMutex);

	newChunk->transform.position = Vector3(static_cast<float>(CHUNKSIZE_X * x), static_cast<float>(CHUNKSIZE_Y * y), static_cast<float>(CHUNKSIZE_Z * z));
	newChunk->Load();

	this->chunkMap[tuple<int, int, int>(x, y, z)] = newChunk;

	ReleaseSRWLockExclusive(&newChunk->gAccessMutex);

	return newChunk;
}

const Vector3Int& ChunkManager::WorldToIndexPosition(Vector3 worldPosition)
{
	return Vector3Int(
		static_cast<int>(floorf(worldPosition.x / CHUNKSIZE_X)), 
		static_cast<int>(floorf(worldPosition.y / CHUNKSIZE_Y)), 
		static_cast<int>(floorf(worldPosition.z / CHUNKSIZE_Z))
	);
}

int FloorMod(const int& a, const int& b) {
	return (a % b + b) % b;
}

Vector3Int ToChunkPosition(int x, int y, int z) {
	// weird because of negative numbers
	//return Vector3Int(0, 0, 0);

	//return Vector3Int(
	//	x / CHUNKSIZE_X,
	//	y / CHUNKSIZE_Y,
	//	z / CHUNKSIZE_Z
	//);

	//if (x < 0 && x % CHUNKSIZE_X == 0) {
	//	x += CHUNKSIZE_X;
	//}
	//if (y < 0 && y % CHUNKSIZE_Y == 0) {
	//	y += CHUNKSIZE_Y;
	//}
	//if (z < 0 && z % CHUNKSIZE_Z == 0) {
	//	z += CHUNKSIZE_Z;
	//}

	//return Vector3Int(
	//	(int)floorf((float)(x - (int)(x < 0)) / (float)CHUNKSIZE_X),
	//	(int)floorf((float)(y - (int)(y < 0)) / (float)CHUNKSIZE_Y),
	//	(int)floorf((float)(z - (int)(z < 0)) / (float)CHUNKSIZE_Z)
	//);

	/*
	return Vector3Int(
		(x < 0) ? floorf(x-1 / CHUNKSIZE_X) : ceilf(x / CHUNKSIZE_X),
		(y < 0) ? floorf(y-1 / CHUNKSIZE_Y) : ceilf(y / CHUNKSIZE_Y),
		(z < 0) ? floorf(z-1 / CHUNKSIZE_Z) : ceilf(z / CHUNKSIZE_Z)
	);*/

	return Vector3Int(
		((x < 0) ? ((x - CHUNKSIZE_X) / CHUNKSIZE_X) : (x / CHUNKSIZE_X)),
		((y < 0) ? ((y - CHUNKSIZE_Y) / CHUNKSIZE_Y) : (y / CHUNKSIZE_Y)),
		((z < 0) ? ((z - CHUNKSIZE_Z) / CHUNKSIZE_Z) : (z / CHUNKSIZE_Z))
	);
}



BlockID ChunkManager::GetBlockAtWorldPos(int x, int y, int z)
{
	return WorldGen::GetBlockAt(x, y, z);
	// broke at 40 4 -96
	//Vector3Int chunkIndex = tuple<int, int, int>(x / CHUNKSIZE_X, y / CHUNKSIZE_Y, z / CHUNKSIZE_Z);
	Vector3Int chunkIndex = ToChunkPosition(x, y, z);
	if(chunkMap.count(chunkIndex)) {
		Vector3Int localVoxelPos = Vector3Int(FloorMod(x, CHUNKSIZE_X), FloorMod(y, CHUNKSIZE_Y), FloorMod(z, CHUNKSIZE_Z));
		
		BlockID debugBlockID = (BlockID)chunkMap[chunkIndex]->blockData[localVoxelPos.x][localVoxelPos.y][localVoxelPos.z];
		return debugBlockID;
	}
	else {

		//todo: read from chunk cache of height samples?
		return WorldGen::GetBlockAt(x, y, z);

	}

}

BlockID ChunkManager::GetBlockAtWorldPos(Vector3Int v) {
	return GetBlockAtWorldPos(v.x, v.y, v.z);
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
		Vector3Int camIndex = WorldToIndexPosition(camTransform->position);

		for(int y = 1 - CHUNKLOAD_AREA_Y; y < CHUNKLOAD_AREA_Y + 1; y++) {
			for(int x = 1 - CHUNKLOAD_AREA_X; x < CHUNKLOAD_AREA_X + 1; x++) {
				for(int z = 1 - CHUNKLOAD_AREA_Z; z < CHUNKLOAD_AREA_Z + 1; z++) {
					//AcquireSRWLockExclusive(pDestroyMutex);
					CreateChunk(camIndex.x + x, camIndex.y + y, camIndex.z + z);
					//ReleaseSRWLockExclusive(pDestroyMutex);


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
				abs(indexPos.y - camIndex.y) > CHUNKLOAD_AREA_Y ||
				abs(indexPos.z - camIndex.z) > CHUNKLOAD_AREA_Z
				) { // Erase chunk from map (it is out of range)

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
	if (this->pEngine == nullptr || this->pCameraTransform == nullptr ) {
		exit(204); 
	}

	_chunkLoaderThreads.emplace_back([&]() { LoaderThreadFunc(pCameraTransform, &chunkMap); });
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
	//for(const pair<tuple<int, int, int>, Chunk*>& pair : this->chunkMap) {
	//	delete pair.second;
	//}
}

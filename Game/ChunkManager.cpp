#include "ChunkManager.h"

void ChunkManager::CreateChunk(int x, int y, int z)
{
	// if chunk already exists, return
	if (chunkMap.count(tuple<int, int, int>(x, y, z))) {
		return;
	}

	Chunk* newChunk = (Chunk*)this->pEngine->CreateObject3D(
		new Chunk(Vector3Int(x, y, z), this), // chunk instance w/ initialised values (position and reference to ChunkManager)
		"_c" + to_string(x) + "_" + to_string(y) + "_" + to_string(z) // Object3D name (using index as string)
	);

	newChunk->transform.position = Vector3(static_cast<float>(CHUNKSIZE_X * x), static_cast<float>(CHUNKSIZE_Y * y), static_cast<float>(CHUNKSIZE_Z * z));

	this->chunkMap[tuple<int, int, int>(x, y, z)] = newChunk;
}

const Vector3Int& ChunkManager::WorldToIndexPosition(Vector3 worldPosition)
{
	return Vector3Int(
		static_cast<int>(floorf(worldPosition.x / CHUNKSIZE_X)), 
		static_cast<int>(floorf(worldPosition.y / CHUNKSIZE_Y)), 
		static_cast<int>(floorf(worldPosition.z / CHUNKSIZE_Z))
	);
}

Vector3Int ToChunkPosition(int x, int y, int z) {
	// weird because of negative numbers
	//return Vector3Int(0, 0, 0);

	return Vector3Int(
		((x < 0) ? ((x - CHUNKSIZE_X) / CHUNKSIZE_X) : (x / CHUNKSIZE_X)),
		((y < 0) ? ((y - CHUNKSIZE_Y) / CHUNKSIZE_Y) : (y / CHUNKSIZE_Y)),
		((z < 0) ? ((z - CHUNKSIZE_Z) / CHUNKSIZE_Z) : (z / CHUNKSIZE_Z))
	);
}

int FloorMod(const int& a, const int& b) {
	return (a % b + b) % b;
}

BlockID ChunkManager::GetBlockAtWorldPos(int x, int y, int z)
{
	//Vector3Int chunkIndex = tuple<int, int, int>(x / CHUNKSIZE_X, y / CHUNKSIZE_Y, z / CHUNKSIZE_Z);
	Vector3Int chunkIndex = ToChunkPosition(x, y, z);
	if(chunkMap.count(chunkIndex)) {
		Vector3Int localVoxelPos = Vector3Int(FloorMod(x, CHUNKSIZE_X), FloorMod(y, CHUNKSIZE_Y), FloorMod(z, CHUNKSIZE_Z));
		return (BlockID)chunkMap[chunkIndex]->blockData[localVoxelPos.x][localVoxelPos.y][localVoxelPos.z];
	}
	else {

		//todo: read from chunk cache of height samples?
		return WorldGen::GetBlockAt(x, y, z);

	}

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

	//CreateChunk(0, 0, 0);
}

void ChunkManager::Update(float dt)
{
	//Vector3Int cameraIndexPosition = WorldToIndexPosition(pCameraTransform->position);

	//for(int y = 1 - CHUNKLOAD_AREA; y < CHUNKLOAD_AREA + 1; y++) {
	//	for(int x = 1 - CHUNKLOAD_AREA; x < CHUNKLOAD_AREA + 1; x++) {
	//		for(int z = 1 - CHUNKLOAD_AREA; z < CHUNKLOAD_AREA + 1; z++) {
	//			CreateChunk(cameraIndexPosition.x + x, cameraIndexPosition.y + y, cameraIndexPosition.z + z);
	//		}
	//	}
	//}
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

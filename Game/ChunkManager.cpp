#include "ChunkManager.h"

void ChunkManager::CreateChunk(int x, int y, int z)
{
	// if chunk already exists, return
	if (chunkMap.count(tuple<int, int, int>(x, y, z))) {
		return;
	}

	Chunk* newChunk = (Chunk*)this->pEngine->CreateObject3D(new Chunk(), "chunk" + to_string(x) + "," + to_string(y) + "," + to_string(z));
	//Chunk* newChunk = this->pEngine->CreateObject3D<Chunk>("chunk" + to_string(x) + "," + to_string(y) + "," + to_string(z));

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

void ChunkManager::Start()
{
	if (this->pEngine == nullptr || this->pCameraTransform == nullptr ) {
		exit(204); 
	}

	//CreateChunk(0, 0, 0);
}

void ChunkManager::Update(float dt)
{
	Vector3Int cameraIndexPosition = WorldToIndexPosition(pCameraTransform->position);

	for (int y = 1-CHUNKLOAD_AREA; y < CHUNKLOAD_AREA+1; y++) {
		for (int x = 1-CHUNKLOAD_AREA; x < CHUNKLOAD_AREA+1; x++) {
			for (int z = 1-CHUNKLOAD_AREA; z < CHUNKLOAD_AREA+1; z++) {
				CreateChunk(cameraIndexPosition.x + x, cameraIndexPosition.y + y, cameraIndexPosition.z + z);
			}
		}
	}
}

ChunkManager::~ChunkManager()
{
	//for(const pair<tuple<int, int, int>, Chunk*>& pair : this->chunkMap) {
	//	delete pair.second;
	//}
}

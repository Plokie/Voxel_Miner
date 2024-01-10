#include "Chunk.h"

#include "WorldGen.h"
#include "ChunkManager.h"
#include "BlockAction.h"
#include "ChunkDatabase.h"
#include "BlockData.h"

void Chunk::Generate()
{
	//GenerateBlockData();
	BuildMesh();

	Engine::Get()->GetCurrentScene()->CreateObject3D(this, "c_" + indexPosition.ToString());
}

void Chunk::Finalize() {
	BuildMesh();
	

	{
		//unique_lock<std::mutex> lock(Engine::Get()->GetCurrentScene()->createObjectMutex);
		Engine::Get()->GetScene("game")->QueueCreateObject3D(this, "c_" + indexPosition.ToString());
	}
}

void Chunk::Start() {
	this->cullBox = AABB(transform.position + Vector3(CHUNKSIZE_X/2, CHUNKSIZE_Y / 2, CHUNKSIZE_Z / 2), Vector3(CHUNKSIZE_X / 2, CHUNKSIZE_Y / 2, CHUNKSIZE_Z / 2));
}

void Chunk::Update(float dTime) {
	tickTimer += dTime;

	if(tickTimer > secondsPerTick) {
		tickTimer = 0.f;

		for(const auto& _blockData : blockDataData) {
			Vector3Int voxelWorldPos = { get<0>(_blockData.first), get<1>(_blockData.first), get<2>(_blockData.first) };
			Vector3Int localVoxelPos = Vector3Int(FloorMod(voxelWorldPos.x, CHUNKSIZE_X), FloorMod(voxelWorldPos.y, CHUNKSIZE_Y), FloorMod(voxelWorldPos.z, CHUNKSIZE_Z));

			BlockID blockID = blockData[localVoxelPos.x][localVoxelPos.y][localVoxelPos.z];

			BlockTick::CallBlockTick(blockID, chunkManager, voxelWorldPos, this, _blockData.second);
		}
	}


}

Chunk::~Chunk() {
	for(Model*& model : models) {
		//model->~Model();
		delete model;
		model = nullptr;
	}
	models.clear();

	for(const auto& kvp : blockDataData) {
		delete kvp.second;
	}
	blockDataData.clear();
}

void Chunk::GenerateBlockData()
{
	int worldX, worldY, worldZ;
	for(int z = 0; z < CHUNKSIZE_Z; z++) {
		worldZ = z + (indexPosition.z * CHUNKSIZE_Z);

		for(int x = 0; x < CHUNKSIZE_X; x++) {
			worldX = x + (indexPosition.x * CHUNKSIZE_X);
			float heightSample = WorldGen::SampleWorldHeight(worldX, worldZ);
			float tempSample = WorldGen::SampleTemperature(worldX, worldZ);
			float moistSample = WorldGen::SampleMoisture(worldX, worldZ);

			const Biome& biome = Biome::Get(tempSample, moistSample);


			for(int y = 0; y < CHUNKSIZE_Y; y ++) {
				worldY = y + (indexPosition.y * CHUNKSIZE_Y);

				if(indexPosition.y == -(CHUNKLOAD_FIXED_NY - 1) && y == 0) {
					blockData[x][y][z] = BLACKSTONE;
					continue;
				}

				BlockID newBlock = WorldGen::GetBlockGivenHeight(worldX, worldY, worldZ, static_cast<int>(heightSample), biome, moistSample);

				blockData[x][y][z] = newBlock;
			}
		}
	}
}
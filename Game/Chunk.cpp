#include "Chunk.h"

#include "WorldGen.h"
#include "ChunkManager.h"
#include "BlockAction.h"
#include "ChunkDatabase.h"
#include "BlockData.h"

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
					blockData[x][y][z] = BEDROCK;
					continue;
				}

				BlockID newBlock = WorldGen::GetBlockGivenHeight(worldX, worldY, worldZ, static_cast<int>(heightSample), biome, moistSample);

				blockData[x][y][z] = newBlock;
			}
		}
	}
}

// based on https://tomcc.github.io/2014/08/31/visibility-1.html
void Chunk::TryFloodFillGraph(int x, int y, int z) {
	const Block& parentBlockDef = BlockDef::GetDef(blockData[x][y][z]);
	if(fillIndices[x][y][z] == 0 && !parentBlockDef.GetDrawType()==DRAW_TYPE::B_OPAQUE) {

		//if(indexPosition == Vector3Int(-4, -1, 0))
		//	__nop();

		//vector<tuple<int, int, int>> chunkNeighboursMet = {};
		map<tuple<int, int, int>, void*> chunkNeighboursMet = {};
		std::queue<tuple<int, int, int>> fillQueue;

		fillQueue.push({ x,y,z });
		// populate chunkNeighboursMet map with keys of chunkIndices of neighbouring chunks
		// we met on this flood fill
		while(!fillQueue.empty()) {
			tuple<int, int, int> currentIndex = fillQueue.front();
			Vector3Int currentIndexv3 = currentIndex;

			const Block& def = BlockDef::GetDef(blockData[currentIndexv3.x][currentIndexv3.y][currentIndexv3.z]);

			if(fillIndices[currentIndexv3.x][currentIndexv3.y][currentIndexv3.z] == 0 && !def.GetDrawType()==B_OPAQUE) { // If this index is unvisited
				fillIndices[currentIndexv3.x][currentIndexv3.y][currentIndexv3.z] = currentFillindex;

				if(currentIndexv3.x <= 0) {
					chunkNeighboursMet[indexPosition + Vector3Int(-1, 0, 0)] = 0;
				}
				else {
					fillQueue.push(currentIndexv3 + Vector3Int(-1, 0, 0));
					if(currentIndexv3.x >= CHUNKSIZE_X - 1) {
						chunkNeighboursMet[indexPosition + Vector3Int(1, 0, 0)] = 0;
					}
					else fillQueue.push(currentIndexv3 + Vector3Int(1, 0, 0));

				}

				if(currentIndexv3.y <= 0) {
					chunkNeighboursMet[indexPosition + Vector3Int(0, -1, 0)] = 0;
				}
				else {
					fillQueue.push(currentIndexv3 + Vector3Int(0, -1, 0));
					if(currentIndexv3.y >= CHUNKSIZE_Y - 1) {
						chunkNeighboursMet[indexPosition + Vector3Int(0, 1, 0)] = 0;
					}
					else fillQueue.push(currentIndexv3 + Vector3Int(0, 1, 0));
				}

				if(currentIndexv3.z <= 0) {
					chunkNeighboursMet[indexPosition + Vector3Int(0, 0, -1)] = 0;
				}
				else {
					fillQueue.push(currentIndexv3 + Vector3Int(0, 0, -1));
					if(currentIndexv3.z >= CHUNKSIZE_Z - 1) {
						chunkNeighboursMet[indexPosition + Vector3Int(0, 0, 1)] = 0;
					}
					else fillQueue.push(currentIndexv3 + Vector3Int(0, 0, 1));
				}
			}
			
			fillQueue.pop();
		}

		for(auto& kvp : chunkNeighboursMet) {
			tuple<int, int, int> incomingDir = kvp.first;

			for(auto& otherKvp : chunkNeighboursMet) {
				if(otherKvp.first == incomingDir) continue;

				visibilityGraph[incomingDir][otherKvp.first] = 0;
			}
		}

		currentFillindex++;
	}
}

void Chunk::BuildVisibilityGraph()
{
	currentFillindex = 1;
	ZeroMemory(fillIndices, sizeof(fillIndices));

	for(int z = 0; z < CHUNKSIZE_Z; z++) {
		for(int x = 0; x < CHUNKSIZE_X; x++) {
			for(int y = 0; y < CHUNKSIZE_Y; y++) {
				bool xEdge = x == 0 || x == CHUNKSIZE_X - 1;
				bool yEdge = y == 0 || y == CHUNKSIZE_Y - 1;
				bool zEdge = z == 0 || z == CHUNKSIZE_Z - 1;

				if(xEdge||yEdge||zEdge)
					TryFloodFillGraph(x, y, z);
			}
		}
	}
}

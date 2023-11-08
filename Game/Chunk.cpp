#include "Chunk.h"

#include "ChunkManager.h"



bool Chunk::RenderBlockFaceAgainst(BlockID currentBlock, const int x, const int y, const int z) {
	const bool isCurrentBlockSolid = BlockDef::GetDef(currentBlock).IsSolid();
	if(x < 0 || x>CHUNKSIZE_X-1 || y < 0 || y>CHUNKSIZE_Y-1 || z < 0 || z>CHUNKSIZE_Z-1) // sample from another chunk
	{
		Vector3Int chunkPosition = Vector3Int(chunkIndexPosition.x * CHUNKSIZE_X, chunkIndexPosition.y * CHUNKSIZE_Y, chunkIndexPosition.z * CHUNKSIZE_Z);
		BlockID neighborBlock = chunkManager->GetBlockAtWorldPos(x + chunkPosition.x, y + chunkPosition.y, z + chunkPosition.z);

		const bool isNeighborSolid(BlockDef::GetDef(neighborBlock).IsSolid());

		return (isCurrentBlockSolid && !isNeighborSolid) || (!isCurrentBlockSolid && neighborBlock == AIR);
		//return isNeighborSolid==isCurrentBlockSolid || neighborBlock != AIR;
	}
	else {
		//todo: block def containing if block is solid and return that
		BlockID neighborBlock = (BlockID)blockData[x][y][z];
		const bool isNeighborSolid(BlockDef::GetDef(neighborBlock).IsSolid());


		return (isCurrentBlockSolid && !isNeighborSolid) || (!isCurrentBlockSolid && neighborBlock == AIR);
	}

}

void PushIndices(size_t size, vector<DWORD>& indices) {
	int int_size = static_cast<int>(size);
	indices.push_back(int_size + 0);
	indices.push_back(int_size + 1);
	indices.push_back(int_size + 2);
	indices.push_back(int_size + 0);
	indices.push_back(int_size + 2);
	indices.push_back(int_size + 3);
}

Vector2 ConvertUVIdToAtlasUV(int uvIdX, int uvIdY) {
	return Vector2(
		(static_cast<float>(uvIdX) * ATLAS_TILE_SIZE) / static_cast<float>(ATLAS_SIZE),
		(static_cast<float>(uvIdY) * ATLAS_TILE_SIZE) / static_cast<float>(ATLAS_SIZE)
	);
}

void PushFace(vector<Vertex>& vertices,
	BlockID id,
	int x, int y, int z,
	float ax, float ay, float az,
	float bx, float by, float bz,
	float cx, float cy, float cz,
	float dx, float dy, float dz,
	float nx, float ny, float nz
) {
	const Block& blockDef = BlockDef::GetDef(id);

	Vector2 uv;
	if(ny > 0.5f) {
		uv = ConvertUVIdToAtlasUV(blockDef.GetTopUVidx(), blockDef.GetTopUVidy());
	}
	else if(ny < -0.5f) {
		uv = ConvertUVIdToAtlasUV(blockDef.GetBottUVidx(), blockDef.GetBottUVidy());
	}
	else {
		uv = ConvertUVIdToAtlasUV(blockDef.GetSideUVidx(), blockDef.GetSideUVidy());
	}
	

	const float uvTileSize = static_cast<float>(ATLAS_TILE_SIZE) / static_cast<float>(ATLAS_SIZE);

	vertices.push_back(Vertex((float)x + ax, (float)y + ay, (float)z + az, nx, ny, nz, 0.f, uvTileSize, uv.x, uv.y));
	vertices.push_back(Vertex((float)x + bx, (float)y + by, (float)z + bz, nx, ny, nz, 0.f, 0.f, uv.x, uv.y));
	vertices.push_back(Vertex((float)x + cx, (float)y + cy, (float)z + cz, nx, ny, nz, uvTileSize, 0.f, uv.x, uv.y));
	vertices.push_back(Vertex((float)x + dx, (float)y + dy, (float)z + dz, nx, ny, nz, uvTileSize, uvTileSize, uv.x, uv.y));
}


void Chunk::MakeVoxel(const BlockID blockID, const int x, const int y, const int z, vector<Vertex>& vertices, vector<DWORD>& indices) {
	// positive x and negative x IsBlockSolid condition
	bool px = RenderBlockFaceAgainst(blockID, x+1, y, z);
	bool nx = RenderBlockFaceAgainst(blockID, x-1, y, z);

	bool py = RenderBlockFaceAgainst(blockID, x, y+1, z);
	bool ny = RenderBlockFaceAgainst(blockID, x, y-1, z);

	bool pz = RenderBlockFaceAgainst(blockID, x, y, z+1);
	bool nz = RenderBlockFaceAgainst(blockID, x, y, z-1);

	// If all blocks surrounding this block are solid, we dont want to build a mesh
	// and can exit immediately
	//if(px && nx && py && ny && pz && nz) return; 

	if(px) {
		PushIndices(vertices.size(), indices);
		PushFace(vertices, blockID,
			x, y, z,
			1,0,0,
			1,1,0,
			1,1,1,
			1,0,1,

			1,0,0
		);
	}
	if(nx) {
		PushIndices(vertices.size(), indices);
		PushFace(vertices, blockID,
			x, y, z,
			0, 0, 1,
			0, 1, 1,
			0, 1, 0,
			0, 0, 0,

			-1, 0, 0
		);
	}
	if(py) {
		PushIndices(vertices.size(), indices);
		PushFace(vertices, blockID,
			x, y, z,
			0,	1,	0,
			0,	1,	1,
			1,	1,	1,
			1,	1,	0,

			0, 1, 0
		);
	}
	if(ny) {
		PushIndices(vertices.size(), indices);
		PushFace(vertices, blockID,
			x, y, z,
			0, 0, 1,
			0, 0, 0,
			1,  0, 0,
			1, 0, 1,

			0, -1, 0
		);
	}
	if(pz) {
		PushIndices(vertices.size(), indices);
		PushFace(vertices, blockID,
			x, y, z,
			1, 0, 1,
			1, 1, 1,
			0, 1, 1,
			0, 0, 1,

			0, 0, 1
		);
	}
	if(nz) {
		PushIndices(vertices.size(), indices);

		PushFace(vertices, blockID,
			x, y, z,
			0, 0, 0,
			0, 1, 0,
			1, 1, 0,
			1, 0, 0,

			0, 0, -1
		);
	}
}

void Chunk::PushChunkMesh(vector<Vertex>& vertices, vector<DWORD>& indices, MESHFLAG meshFlag)
{
	size_t vertSize = vertices.size();
	size_t indSize = indices.size();
	size_t modelCount = models.size();

	if(vertSize > 0 && indSize > 0) {
		models.push_back(Model::Create(Graphics::Get()->GetDevice()));
		models[modelCount]->SetTexture(0, "atlas");
		models[modelCount]->SetTransparent(meshFlag > 0);
		if(meshFlag == WATER) {
			models[modelCount]->SetVertexShader(0, "watervertexshader");
		}

		Mesh* newMesh = new Mesh();
		newMesh->Init(Graphics::Get()->GetDevice());

		newMesh->_isProceduralMesh = true;
		// it was at this moment that i discovered that vectors are NOT the same as linked lists
		// and blew my mind
		newMesh->LoadVertices(&vertices[0], static_cast<int>(vertices.size()));
		newMesh->LoadIndices(&indices[0], static_cast<int>(indices.size()));

		models[modelCount]->SetMesh(newMesh);
	}
}

void Chunk::BuildMesh()
{
	for(Model* model : models) {
		model->ReleaseMesh();
		
		//delete model;
	}
	models.clear();

	vector<Vertex> solidVertices = {};
	vector<DWORD> solidIndices = {};

	//proud of them
	vector<Vertex> transVertices = {};
	vector<DWORD> transIndices = {};

	vector<Vertex> waterVertices = {};
	vector<DWORD> waterIndices = {};

	//todo: optimised chunk building (not looping through every single block, most of them are invisible)
	for(int y = 0; y < CHUNKSIZE_Y; y++) {
		for(int z = 0; z < CHUNKSIZE_Z; z++) {
			for(int x = 0; x < CHUNKSIZE_X; x++) {
				BlockID blockid = (BlockID)blockData[x][y][z];
				if(blockid == BlockID::AIR) continue;

				if(BlockDef::GetDef(blockid).IsSolid()) {
					MakeVoxel(blockid, x, y, z, solidVertices, solidIndices);
				}
				else if(blockid == BlockID::WATER)
				{
					MakeVoxel(blockid, x, y, z, waterVertices, waterIndices);
				}
				else{
					MakeVoxel(blockid, x, y, z, transVertices, transIndices);
				}
			}
		}
	}

	PushChunkMesh(solidVertices, solidIndices);
	PushChunkMesh(transVertices, transIndices, TRANS);
	PushChunkMesh(waterVertices, waterIndices, WATER);

#ifdef _DEBUG
#if 0
	{
		size_t modelCount = models.size();
		models.push_back(Model::Create(Graphics::Get()->GetDevice()));
		models[modelCount]->SetTexture(0, "chunkborder");
		models[modelCount]->SetMesh("chunkborder");
		models[modelCount]->SetTransparent(true);
	}
#endif
#endif
}

void Chunk::Load()
{
	if(this == nullptr) return;
	//debug init chunk
	int worldX = 0, worldY = 0, worldZ = 0;
	//SRWLOCK* pMutex = Engine::Get()->GetDestroyObjectsMutex();
	//Engine::Get()->sceneObjects;

	//AcquireSRWLockExclusive(pMutex);
	for(int z = 0; z < CHUNKSIZE_Z; z++) {
		worldZ = z + (chunkIndexPosition.z * CHUNKSIZE_Z);
		for(int x = 0; x < CHUNKSIZE_X; x++) {
			worldX = x + (chunkIndexPosition.x * CHUNKSIZE_X);
			float heightSample = WorldGen::SampleWorldHeight(worldX, worldZ);
			for(int y = 0; y < CHUNKSIZE_Y; y++) {
				worldY = y + (chunkIndexPosition.y * CHUNKSIZE_Y);
				
				// some kind of thread thing is going wrong here
				blockData[x][y][z] = WorldGen::GetBlockGivenHeight(worldX, worldY, worldZ, static_cast<int>(heightSample));

			}
		}
	}
	//ReleaseSRWLockExclusive(pMutex);
	//blockData[0][0][0] = BlockID::AIR;
	//blockData[0][0][0] = BlockID::GRASS;

	//ZeroMemory(&blockData, CHUNKSIZE_X * CHUNKSIZE_Y * CHUNKSIZE_Z);
	//memset(&blockData, 1, CHUNKSIZE_X * CHUNKSIZE_Y * CHUNKSIZE_Z);
	//if(!isChunkAllAir)
	BuildMesh();
}

void Chunk::Start() {

}

void Chunk::Update(float dTime)
{
	//transform.rotation.y += 5.f * dTime;
}

void Chunk::Release() {
	//delete[CHUNKSIZE_X * CHUNKSIZE_Y * CHUNKSIZE_Z] blockData;
	//delete blockData;
}

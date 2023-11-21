#include "Chunk.h"

#include "ChunkManager.h"
#include "ChunkDatabase.h"
#include "VoxelRaycast.h"


bool Chunk::RenderBlockFaceAgainst(BlockID currentBlock, const int x, const int y, const int z) {
	const bool isCurrentBlockSolid = BlockDef::GetDef(currentBlock).IsOpaque();
	BlockID neighborBlock = GetBlockIncludingNeighbours(x, y, z);
	bool isNeighborSolid = BlockDef::GetDef(neighborBlock).IsOpaque();
	return (isCurrentBlockSolid && !isNeighborSolid) || (!isCurrentBlockSolid && neighborBlock == AIR);
}



void Chunk::CorrectIndexForNeighbours(const int& x, const int& y, const int& z, Chunk** outChunk, Vector3Int* outIndex){
	if(x < 0 || x>CHUNKSIZE_X - 1 || y < 0 || y>CHUNKSIZE_Y - 1 || z < 0 || z>CHUNKSIZE_Z - 1) // sample from another chunk
	{
		Vector3Int chunkPosition = Vector3Int(chunkIndexPosition.x * CHUNKSIZE_X, chunkIndexPosition.y * CHUNKSIZE_Y, chunkIndexPosition.z * CHUNKSIZE_Z);
		tuple<int, int, int> chunkIndex = ChunkManager::ToChunkIndexPositionTuple(x, y, z);
		if(chunkManager->GetChunkMap().find(chunkIndex) != chunkManager->GetChunkMap().end()) {
			*outIndex = Vector3Int(FloorMod(x + chunkPosition.x, CHUNKSIZE_X), FloorMod(y + chunkPosition.y, CHUNKSIZE_Y), FloorMod(z + chunkPosition.z, CHUNKSIZE_Z));
			*outChunk = chunkManager->GetChunk(chunkIndex);

			return;
		}
		*outChunk = nullptr;
		*outIndex = Vector3Int(0, 0, 0);
	}
	else {
		//return (BlockID)blockData[x][y][z];
		*outChunk = this;
		*outIndex = Vector3Int(x, y, z);
	}
}

void Chunk::CorrectIndexForNeighbours(const Vector3Int& index, Chunk** outChunk, Vector3Int* outIndex)
{
	CorrectIndexForNeighbours(index.x, index.y, index.z, outChunk, outIndex);
}

BlockID Chunk::GetBlockIncludingNeighbours(const int& x, const int& y, const int& z)
{
	if(x < 0 || x>CHUNKSIZE_X - 1 || y < 0 || y>CHUNKSIZE_Y - 1 || z < 0 || z>CHUNKSIZE_Z - 1) // sample from another chunk
	{
		Vector3Int chunkPosition = Vector3Int(chunkIndexPosition.x * CHUNKSIZE_X, chunkIndexPosition.y * CHUNKSIZE_Y, chunkIndexPosition.z * CHUNKSIZE_Z);
		return chunkManager->GetBlockAtWorldPos(x + chunkPosition.x, y + chunkPosition.y, z + chunkPosition.z);
	}
	else {
		return (BlockID)blockData[x][y][z];
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
	float x,  float y,  float z,
	float ax, float ay, float az,
	float bx, float by, float bz,
	float cx, float cy, float cz,
	float dx, float dy, float dz,
	float nx, float ny, float nz,
	int lightLevel
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
	
	Vector3 normal = Vector3(nx, ny, nz).normalized();
	normal *= (static_cast<float>(lightLevel) / 15.0f) + 0.1f;

	const float uvTileSize = static_cast<float>(ATLAS_TILE_SIZE) / static_cast<float>(ATLAS_SIZE);

	vertices.push_back(Vertex((float)x + ax, (float)y + ay, (float)z + az, normal.x, normal.y, normal.z, 0.f, uvTileSize, uv.x, uv.y));
	vertices.push_back(Vertex((float)x + bx, (float)y + by, (float)z + bz, normal.x, normal.y, normal.z, 0.f, 0.f, uv.x, uv.y));
	vertices.push_back(Vertex((float)x + cx, (float)y + cy, (float)z + cz, normal.x, normal.y, normal.z, uvTileSize, 0.f, uv.x, uv.y));
	vertices.push_back(Vertex((float)x + dx, (float)y + dy, (float)z + dz, normal.x, normal.y, normal.z, uvTileSize, uvTileSize, uv.x, uv.y));
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
		//int light = max(GetBlockLightIncludingNeighbours(x+1, y, z), GetSkyLightIncludingNeighbours(x+1, y, z));
		short rawLight = GetRawLightIncludingNeighbours(x + 1, y, z);
		int light = max((rawLight & 0xF0) >> 4, rawLight & 0x0F); // sky, block

		PushIndices(vertices.size(), indices);
		PushFace(vertices, blockID,
			(float)x, (float)y, (float)z,
			1, 0, 0,
			1, 1, 0,
			1, 1, 1,
			1, 0, 1,

			1, 0, 0,
			light
		);
	}
	if(nx) {
		short rawLight = GetRawLightIncludingNeighbours(x - 1, y, z);
		int light = max((rawLight & 0xF0) >> 4, rawLight & 0x0F); // sky, block

		PushIndices(vertices.size(), indices);
		PushFace(vertices, blockID,
			(float)x, (float)y, (float)z,
			0, 0, 1,
			0, 1, 1,
			0, 1, 0,
			0, 0, 0,

			-1, 0, 0,
			light
		);
	}
	if(py) {
		short rawLight = GetRawLightIncludingNeighbours(x, y + 1, z);
		int light = max((rawLight & 0xF0) >> 4, rawLight & 0x0F); // sky, block

		PushIndices(vertices.size(), indices);
		PushFace(vertices, blockID,
			(float)x, (float)y, (float)z,
			0,	1,	0,
			0,	1,	1,
			1,	1,	1,
			1,	1,	0,

			0, 1, 0,
			light
		);
	}
	if(ny) {
		short rawLight = GetRawLightIncludingNeighbours(x, y - 1, z);
		int light = max((rawLight & 0xF0) >> 4, rawLight & 0x0F); // sky, block

		PushIndices(vertices.size(), indices);
		PushFace(vertices, blockID,
			(float)x, (float)y, (float)z,
			0, 0, 1,
			0, 0, 0,
			1,  0, 0,
			1, 0, 1,

			0, -1, 0,
			light
		);
	}
	if(pz) {
		short rawLight = GetRawLightIncludingNeighbours(x, y, z+1);
		int light = max((rawLight & 0xF0) >> 4, rawLight & 0x0F); // sky, block

		PushIndices(vertices.size(), indices);
		PushFace(vertices, blockID,
			(float)x, (float)y, (float)z,
			1, 0, 1,
			1, 1, 1,
			0, 1, 1,
			0, 0, 1,

			0, 0, 1,
			light
		);
	}
	if(nz) {
		short rawLight = GetRawLightIncludingNeighbours(x, y, z - 1);
		int light = max((rawLight & 0xF0) >> 4, rawLight & 0x0F); // sky, block

		PushIndices(vertices.size(), indices);
		PushFace(vertices, blockID,
			(float)x, (float)y, (float)z,
			0, 0, 0,
			0, 1, 0,
			1, 1, 0,
			1, 0, 0,

			0, 0, -1,
			light
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
		Model* newModel = models[modelCount];
		newModel->SetTexture(0, "atlas");


		switch (meshFlag) {
		case TRANS:
			newModel->SetTransparent(true);
			break;
		case WATER:
			newModel->SetTransparent(true);
			newModel->SetVertexShader(0, "watervertexshader");
			break;
		case SHELL:
			newModel->SetTransparent(true);
			newModel->SetVertexShader(0, "vertexshellgrass");
			newModel->SetPixelShader(0, "pixelshellgrass");
			break;
		default: break;
		}


		Mesh* newMesh = new Mesh();
		newMesh->Init(Graphics::Get()->GetDevice());

		newMesh->_isProceduralMesh = true;
		// it was at this moment that i discovered that vectors are NOT the same as linked lists
		// and blew my mind
		newMesh->LoadVertices(&vertices[0], static_cast<int>(vertices.size()));
		newMesh->LoadIndices(&indices[0], static_cast<int>(indices.size()));

		newModel->SetMesh(newMesh);
	}
}

void Chunk::BuildMesh()
{
	for(Model*& model : models) {
		model->ReleaseMesh();
		model = nullptr;
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

	const int shellCount = 5;
	const float shellPixelHeight = 2.f;
	const float shellSeperation = ((1.f / 16.f) * shellPixelHeight) / static_cast<float>(shellCount);

	vector<vector<Vertex>> grassShellsVertices = {};
	vector<vector<DWORD>> grassShellsIndices = {};

	for(int i = 0; i < shellCount; i++) {
		grassShellsVertices.push_back({});
		grassShellsIndices.push_back({});
	}

	//todo: optimised chunk building (not looping through every single block, most of them are invisible)
	for(int y = 0; y < CHUNKSIZE_Y; y++) {
		for(int z = 0; z < CHUNKSIZE_Z; z++) {
			for(int x = 0; x < CHUNKSIZE_X; x++) {
				BlockID blockid = (BlockID)blockData[x][y][z];
				if(blockid == BlockID::AIR) continue;

				if(highestBlock[x][z] < y) highestBlock[x][z] = y;

				if(BlockDef::GetDef(blockid).IsOpaque()) {
					MakeVoxel(blockid, x, y, z, solidVertices, solidIndices);

					if(blockid == GRASS && RenderBlockFaceAgainst(blockid, x, y + 1, z)) {
						//int light = chunkManager->GetBlockLightAtWorldPos(x, y + 1, z);
						short rawLight = GetRawLightIncludingNeighbours(x, y+1, z);
						int light = max((rawLight & 0xF0) >> 4, rawLight & 0x0F); // sky, block

						for(int i = 1; i < shellCount+1; i++) {
							int index = i - 1; // get compiler to shut up about "sub expression overflow" false positive
							PushIndices(grassShellsVertices[index].size(), grassShellsIndices[index]);
							PushFace(grassShellsVertices[index], blockid,
								(float)x, (float)y + ((float)shellSeperation * i), (float)z,
								0, 1, 0,
								0, 1, 1,
								1, 1, 1,
								1, 1, 0,
								0, 1, 0,
								light
							);
						}
					}
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

	for(int i = 0; i < shellCount; i++) {
		PushChunkMesh(grassShellsVertices[i], grassShellsIndices[i], SHELL);
	}

	//PushChunkMesh(grassShellVertices, grassShellIndices, SHELL);
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

bool Chunk::Load()
{
	if(this == nullptr) return false;

	int worldX = 0, worldY = 0, worldZ = 0;
	bool returnVal = false;
	
	memset(this->lightLevel, 0, sizeof(this->lightLevel));
	this->cullBox = AABB(transform.position + Vector3(8.f, 8.f, 8.f), Vector3(8.f, 8.f, 8.f));

	if (ChunkDatabase::Get()->DoesDataExistForChunk(chunkIndexPosition)) {
		ChunkDatabase::Get()->LoadChunkDataInto(chunkIndexPosition, this);
		returnVal = true;
	}
	else 
	{
		for(int z = 0; z < CHUNKSIZE_Z; z++) {
			worldZ = z + (chunkIndexPosition.z * CHUNKSIZE_Z);
			for(int x = 0; x < CHUNKSIZE_X; x++) {
				worldX = x + (chunkIndexPosition.x * CHUNKSIZE_X);
				float heightSample = WorldGen::SampleWorldHeight(worldX, worldZ);
				float tempSample = WorldGen::SampleTemperature(worldX, worldZ);
				float moistSample = WorldGen::SampleMoisture(worldX, worldZ);

				for(int y = 0; y < CHUNKSIZE_Y; y++) {
					worldY = y + (chunkIndexPosition.y * CHUNKSIZE_Y);
				
					// some kind of thread thing is going wrong here
					blockData[x][y][z] = WorldGen::GetBlockGivenHeight(worldX, worldY, worldZ, static_cast<int>(heightSample), tempSample, moistSample);

				}
			}
		}
	}
	BuildMesh();
	return returnVal;
}

void Chunk::Start() {
	this->cullBox = AABB(transform.position + Vector3(8.f, 8.f, 8.f), Vector3(8.f, 8.f, 8.f));
	this->player = Engine::Get()->GetCurrentScene()->GetObject3D("CameraController");

	chunkManager->GetLighting()->QueueNewChunk(this);
}

void Chunk::Update(float dTime){}

void Chunk::Release() {
	//delete blockData;
}

int Chunk::GetBlockLight(const int& x, const int& y, const int& z)
{
	return this->lightLevel[x][y][z] & 0x0F;
}

int Chunk::GetBlockLightIncludingNeighbours(const int& x, const int& y, const int& z)
{
	if(x < 0 || x>CHUNKSIZE_X - 1 || y < 0 || y>CHUNKSIZE_Y - 1 || z < 0 || z>CHUNKSIZE_Z - 1) // sample from another chunk
	{
		Vector3Int chunkPosition = Vector3Int(chunkIndexPosition.x * CHUNKSIZE_X, chunkIndexPosition.y * CHUNKSIZE_Y, chunkIndexPosition.z * CHUNKSIZE_Z);
		return chunkManager->GetBlockLightAtWorldPos(x + chunkPosition.x, y + chunkPosition.y, z + chunkPosition.z);
	}
	else {
		return GetBlockLight(x, y, z);
	}
}

void Chunk::SetBlockLightIncludingNeighbours(const int& x, const int& y, const int& z, const int& val)
{
	if(x < 0 || x>CHUNKSIZE_X - 1 || y < 0 || y>CHUNKSIZE_Y - 1 || z < 0 || z>CHUNKSIZE_Z - 1)
	{
		Vector3Int chunkPosition = Vector3Int(chunkIndexPosition.x * CHUNKSIZE_X, chunkIndexPosition.y * CHUNKSIZE_Y, chunkIndexPosition.z * CHUNKSIZE_Z);
		chunkManager->SetBlockLightAtWorldPos(x + chunkPosition.x, y + chunkPosition.y, z + chunkPosition.z, val);
	}
	else {
		SetBlockLight(x, y, z, val);
	}
}

void Chunk::SetBlockLight(const int& x, const int& y, const int& z, const int& val)
{
	this->lightLevel[x][y][z] = (this->lightLevel[x][y][z] & 0xF0) | val;

	 chunkManager->GetLighting()->QueueLight(LightNode(x, y, z, this));
}

void Chunk::SetBlockLightNoUpdate(const int& x, const int& y, const int& z, const int& val) {
	this->lightLevel[x][y][z] = (this->lightLevel[x][y][z] & 0xF0) | val;
}

int Chunk::GetSkyLight(const int& x, const int& y, const int& z)
{
	return (this->lightLevel[x][y][z] & 0xF0) >> 4;
}

int Chunk::GetSkyLightIncludingNeighbours(const int& x, const int& y, const int& z)
{
	if(x < 0 || x>CHUNKSIZE_X - 1 || y < 0 || y>CHUNKSIZE_Y - 1 || z < 0 || z>CHUNKSIZE_Z - 1) // sample from another chunk
	{
		Vector3Int chunkPosition = Vector3Int(chunkIndexPosition.x * CHUNKSIZE_X, chunkIndexPosition.y * CHUNKSIZE_Y, chunkIndexPosition.z * CHUNKSIZE_Z);
		return chunkManager->GetSkyLightAtWorldPos(x + chunkPosition.x, y + chunkPosition.y, z + chunkPosition.z);
	}
	else {
		return GetSkyLight(x, y, z);
	}
}

void Chunk::SetSkyLightIncludingNeighbours(const int& x, const int& y, const int& z, const int& val)
{
	if(x < 0 || x>CHUNKSIZE_X - 1 || y < 0 || y>CHUNKSIZE_Y - 1 || z < 0 || z>CHUNKSIZE_Z - 1)
	{
		Vector3Int chunkPosition = Vector3Int(chunkIndexPosition.x * CHUNKSIZE_X, chunkIndexPosition.y * CHUNKSIZE_Y, chunkIndexPosition.z * CHUNKSIZE_Z);
		chunkManager->SetSkyLightAtWorldPos(x + chunkPosition.x, y + chunkPosition.y, z + chunkPosition.z, val);
	}
	else {
		SetSkyLight(x, y, z, val);
	}
}

short Chunk::GetRawLightIncludingNeighbours(const int& x, const int& y, const int& z)
{
	if(x < 0 || x>CHUNKSIZE_X - 1 || y < 0 || y>CHUNKSIZE_Y - 1 || z < 0 || z>CHUNKSIZE_Z - 1) // sample from another chunk
	{
		Vector3Int chunkPosition = Vector3Int(chunkIndexPosition.x * CHUNKSIZE_X, chunkIndexPosition.y * CHUNKSIZE_Y, chunkIndexPosition.z * CHUNKSIZE_Z);
		return chunkManager->GetRawLightAtWorldPos(x + chunkPosition.x, y + chunkPosition.y, z + chunkPosition.z);
	}
	else {
		return GetRawLight(x, y, z);
	}
}

short Chunk::GetRawLight(const int& x, const int& y, const int& z)
{
	return this->lightLevel[x][y][z];
}


void Chunk::SetSkyLight(const int& x, const int& y, const int& z, const int& val)
{
	this->lightLevel[x][y][z] = (this->lightLevel[x][y][z] & 0xF) | (val << 4);

	//chunkManager->GetLighting()->QueueSkyLight(LightNode(x, y, z, this));
}
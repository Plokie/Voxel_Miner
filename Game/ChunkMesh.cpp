#include "Chunk.h"

#include "ChunkManager.h"

bool Chunk::RenderBlockFaceAgainst(BlockID currentBlock, const int x, const int y, const int z) {
	const bool isCurrentBlockSolid = BlockDef::GetDef(currentBlock).GetDrawType() == B_OPAQUE;
	BlockID neighborBlock = GetBlockIncludingNeighbours(x, y, z);
	bool isNeighborSolid = BlockDef::GetDef(neighborBlock).GetDrawType() == B_OPAQUE;
	return (isCurrentBlockSolid && !isNeighborSolid) || (!isCurrentBlockSolid && neighborBlock == AIR);
}



void Chunk::CorrectIndexForNeighbours(const int& x, const int& y, const int& z, Chunk** outChunk, Vector3Int* outIndex) {
	if(x < 0 || x>CHUNKSIZE_X - 1 || y < 0 || y>CHUNKSIZE_Y - 1 || z < 0 || z>CHUNKSIZE_Z - 1) // sample from another chunk
	{
		Vector3Int chunkPosition = Vector3Int(indexPosition.x * CHUNKSIZE_X, indexPosition.y * CHUNKSIZE_Y, indexPosition.z * CHUNKSIZE_Z);
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
		Vector3Int chunkPosition = Vector3Int(indexPosition.x * CHUNKSIZE_X, indexPosition.y * CHUNKSIZE_Y, indexPosition.z * CHUNKSIZE_Z);
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
	float x, float y, float z,
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
	bool px = RenderBlockFaceAgainst(blockID, x + 1, y, z);
	bool nx = RenderBlockFaceAgainst(blockID, x - 1, y, z);

	bool py = RenderBlockFaceAgainst(blockID, x, y + 1, z);
	bool ny = RenderBlockFaceAgainst(blockID, x, y - 1, z);

	bool pz = RenderBlockFaceAgainst(blockID, x, y, z + 1);
	bool nz = RenderBlockFaceAgainst(blockID, x, y, z - 1);

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
			0, 1, 0,
			0, 1, 1,
			1, 1, 1,
			1, 1, 0,

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
			1, 0, 0,
			1, 0, 1,

			0, -1, 0,
			light
		);
	}
	if(pz) {
		short rawLight = GetRawLightIncludingNeighbours(x, y, z + 1);
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
		//AcquireSRWLockExclusive(&modelsMutex);
		

		Model* newModel = Model::Create(Graphics::Get()->GetDevice());
		newModel->SetTexture(0, "atlas");

		newModel->SetPixelShader(0, "chunkpixelshader");

		switch(meshFlag) {
		case TRANS:
			newModel->SetTransparent(true);
			break;
		case LIQUID:
			newModel->SetTransparent(true);
			newModel->SetVertexShader(0, "watervertexshader");
			break;
		case SHELL:
			//newModel->SetTransparent(true);
			newModel->SetVertexShader(0, "vertexshellgrass");
			newModel->SetPixelShader(0, "pixelshellgrass");
			break;
		case LEAVES:
			newModel->SetVertexShader(0, "vertexshaderleaves");
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
		models.push_back(newModel);
		//ReleaseSRWLockExclusive(&modelsMutex);
	}
}

void Chunk::BuildMesh() {
	for(Model*& model : this->models) {
		model->ReleaseMesh();
		model = nullptr;
		//delete model;
	}
	this->models.clear();
	this->models.reserve(5);

	const int reserveSizeSolid = (CHUNKSIZE_X * CHUNKSIZE_Y * CHUNKSIZE_Z) / 2;
	const int reserveSizeTrans = (CHUNKSIZE_X * CHUNKSIZE_Y * CHUNKSIZE_Z) / 4;
	const int reserveSizeShellLayer = (CHUNKSIZE_X * CHUNKSIZE_Z);

	vector<Vertex> solidVertices = {};
	vector<DWORD> solidIndices = {};
	solidVertices.reserve(reserveSizeSolid);
	solidIndices.reserve(reserveSizeSolid);

	//proud of them
	vector<Vertex> transVertices = {};
	vector<DWORD> transIndices = {};
	transVertices.reserve(reserveSizeTrans);
	transIndices.reserve(reserveSizeTrans);

	vector<Vertex> waterVertices = {};
	vector<DWORD> waterIndices = {};
	waterVertices.reserve((CHUNKSIZE_X * CHUNKSIZE_Z) / 2);
	waterIndices.reserve((CHUNKSIZE_X * CHUNKSIZE_Z) / 2);

	vector<Vertex> leavesVertices = {};
	vector<DWORD> leavesIndices = {};

	const int shellCount = 5;
	const float shellPixelHeight = 2.f;
	const float shellSeperation = ((1.f / 16.f) * shellPixelHeight) / static_cast<float>(shellCount);

	vector<Vertex> grassShellVertices = {};
	vector<DWORD> grassShellIndices = {};

	//todo: optimised chunk building (not looping through every single block, most of them are invisible)
	for(int y = 0; y < CHUNKSIZE_Y; y++) {
		for(int z = 0; z < CHUNKSIZE_Z; z++) {
			for(int x = 0; x < CHUNKSIZE_X; x++) {
				BlockID blockid = (BlockID)this->blockData[x][y][z];
				if(blockid == BlockID::AIR) continue;
				const Block& def = BlockDef::GetDef(blockid);
				
				if(blockid == BlockID::WATER || blockid == LAVA)
				{
					this->MakeVoxel(blockid, x, y, z, waterVertices, waterIndices);
				}
				else if(def.GetDrawType() == B_OPAQUE || def.GetDrawType() == B_CLIP) {
					if(blockid == BlockID::OAK_LEAVES || blockid == BlockID::BIRCH_LEAVES || blockid == BlockID::SPRUCE_LEAVES || blockid == BlockID::CHERRY_LEAVES) 
						this->MakeVoxel(blockid, x, y, z, leavesVertices, leavesIndices);
					else
						this->MakeVoxel(blockid, x, y, z, solidVertices, solidIndices);

#if 1
					if(def.HasShell() && this->RenderBlockFaceAgainst(blockid, x, y + 1, z)) {
						//int light = chunkManager->GetBlockLightAtWorldPos(x, y + 1, z);
						short rawLight = this->GetRawLightIncludingNeighbours(x, y + 1, z);
						int light = max(
							static_cast<int>(((rawLight & 0xF0) >> 4) * 1.0f), //sky light todo: replace *1.0f with sunlight strength / time of day
							rawLight & 0x0F //block light
						);

						for(int i = 1; i < shellCount + 1; i++) {
							PushIndices(grassShellVertices.size(), grassShellIndices);
							PushFace(grassShellVertices, blockid,
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
#endif
				}
				else {
					this->MakeVoxel(blockid, x, y, z, transVertices, transIndices);
				}
			}
		}
	}

	this->PushChunkMesh(solidVertices, solidIndices);

	this->PushChunkMesh(grassShellVertices, grassShellIndices, Chunk::MESHFLAG::SHELL);

	//this->PushChunkMesh(transVertices, transIndices, Chunk::MESHFLAG::TRANS);
	this->PushChunkMesh(waterVertices, waterIndices, Chunk::MESHFLAG::LIQUID);
	this->PushChunkMesh(leavesVertices, leavesIndices, Chunk::MESHFLAG::LEAVES);
}
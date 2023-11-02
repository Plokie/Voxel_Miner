#include "Chunk.h"


bool Chunk::IsBlockSolid(const int x, const int y, const int z) {
	if(x < 0 || x>15 || y < 0 || y>15 || z < 0 || z>15) // sample from another chunk
	{
		return false;
	}
	else {
		//todo: block def containing if block is solid and return that
		return BlockDef::GetDef((BlockID)blockData[x][y][z]).IsSolid();
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

#define ATLAS_SIZE 128
#define ATLAS_TILE_SIZE 16

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
	//const Vector2 topUV = ConvertUVIdToAtlasUV(blockDef.GetTopUVidx(), blockDef.GetTopUVidy());
	//const Vector2 sideUV = ConvertUVIdToAtlasUV(blockDef.GetSideUVidx(), blockDef.GetSideUVidy());
	//const Vector2 bottUV = ConvertUVIdToAtlasUV(blockDef.GetBottUVidx(), blockDef.GetBottUVidy());
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
	bool px = IsBlockSolid(x+1, y, z);
	bool nx = IsBlockSolid(x-1, y, z);

	bool py = IsBlockSolid(x, y+1, z);
	bool ny = IsBlockSolid(x, y-1, z);

	bool pz = IsBlockSolid(x, y, z+1);
	bool nz = IsBlockSolid(x, y, z-1);

	// If all blocks surrounding this block are solid, we dont want to build a mesh
	// and can exit immediately
	//if(px && nx && py && ny && pz && nz) return; 

	if(!px) {
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
	if(!nx) {
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
	if(!py) {
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
	if(!ny) {
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
	if(!pz) {
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
	if(!nz) {
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

void Chunk::BuildMesh()
{
	vector<Vertex> vertices = {};
	vector<DWORD> indices = {};

	//todo: optimised chunk building (not looping through every single block, most of them are invisible)
	for(int y = 0; y < CHUNKSIZE_Y; y++) {
		for(int z = 0; z < CHUNKSIZE_Z; z++) {
			for(int x = 0; x < CHUNKSIZE_X; x++) {
				if(blockData[x][y][z] == BlockID::AIR) continue;

				MakeVoxel((BlockID)blockData[x][y][z], x, y, z, vertices, indices);


			}
		}
	}

	size_t size = vertices.size();

	if(vertices.size() > 0 && indices.size() > 0) {
		/*if(models.size() <= 0) models.push_back(->Init(Graphics::Get()->GetDevice()); {
			Model* newModel
		}*/
		if(models.size() <= 0) models.push_back(Model::Create(Graphics::Get()->GetDevice()));
		//models[0]->SetMesh();
		models[0]->SetTexture(0, "atlas");

		Mesh* newMesh = new Mesh();
		newMesh->Init(Graphics::Get()->GetDevice());

		// it was at this moment that i discovered that vectors are NOT the same as linked lists
		// and blew my mind
		newMesh->LoadVertices(&vertices[0], static_cast<int>(vertices.size()));
		newMesh->LoadIndices(&indices[0], static_cast<int>(indices.size()));
		
		models[0]->SetMesh(newMesh);
	}

}

void Chunk::Start()
{
	//debug init chunk
	for(int y = 0; y < CHUNKSIZE_Y; y++) {
		for(int z = 0; z < CHUNKSIZE_Z; z++) {
			for(int x = 0; x < CHUNKSIZE_X; x++) {
				blockData[x][y][z] = BlockID::GRASS;
			}
		}
	}

	BuildMesh();
}

void Chunk::Update(float dTime)
{
	//transform.rotation.y += 5.f * dTime;
}

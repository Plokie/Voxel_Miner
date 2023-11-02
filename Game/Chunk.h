#pragma once

#include "../Engine/Engine.h"
#include "Blocks.h"

#define CHUNKSIZE_X 16
#define CHUNKSIZE_Y 16
#define CHUNKSIZE_Z 16

using namespace std;

class Chunk : public Object3D {
private:
	bool IsBlockSolid(const int x, const int y, const int z);
	void MakeVoxel(const BlockID blockID, const int x, const int y, const int z, vector<Vertex>& vertices, vector<DWORD>& indices);
public:
	USHORT blockData[CHUNKSIZE_X][CHUNKSIZE_Y][CHUNKSIZE_Z];

	void BuildMesh();

	void Start() override;
	void Update(float dTime) override;

	~Chunk() {
		//delete[CHUNKSIZE_X * CHUNKSIZE_Y * CHUNKSIZE_Z] blockData;

		/*for(Model* model : models) {
			model->~Model();
		}*/
	}
};
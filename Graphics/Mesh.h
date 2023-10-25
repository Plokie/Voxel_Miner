#pragma once

#include <Windows.h>
#include <vector>
#include <d3d11.h>
#include <WICTextureLoader.h>
#include <DDSTextureLoader.h>
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"DirectXTK.lib")
#pragma comment(lib,"DXGI.lib")

#include "Vertex.h"

class Mesh {
	Vertex* pVertices = nullptr;
	int vertexCount = 0;

	int* pIndices = nullptr;
	int indexCount = 0;

	ID3D11Buffer* vertexBuffer = nullptr;
	ID3D11Buffer* indexBuffer = nullptr;

	ID3D11Device* pDevice = nullptr;

public:
	Mesh* Init(ID3D11Device* device);

	void LoadDefaultCube();

	void LoadVertices(Vertex vert[], int vertCount);
	void LoadIndices(int ind[], int indCount);

	void LoadBuffers();

	~Mesh();
};
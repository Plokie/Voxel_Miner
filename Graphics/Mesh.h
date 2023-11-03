#pragma once

#include <Windows.h>
#include <vector>
#include <d3d11.h>
#include <string>
#include <WICTextureLoader.h>
#include <DDSTextureLoader.h>
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"DirectXTK.lib")
#pragma comment(lib,"DXGI.lib")

#include "Vertex.h"
#include "ExampleMeshes.h"

using namespace std;

/// <summary>
/// Purely a collection of indices and vertices
/// </summary>
class Mesh {
	//Vertex* pVertices = nullptr;
	int vertexCount = 0;

	//DWORD* pIndices = nullptr;


	ID3D11Device* pDevice = nullptr;

	int indexCount = 0;
	ID3D11Buffer* indexBuffer = nullptr;
public:
	ID3D11Buffer* vertexBuffer = nullptr; // public because i think IASetVertexBuffers needs a direct reference (not thru a func)

	bool _isProceduralMesh = false;

	Mesh* Init(ID3D11Device* device);

	void LoadDefaultCube();

	void LoadVertices(Vertex vert[], int vertCount);
	void LoadIndices(DWORD ind[], int indCount);

	//void LoadFromFile(wstring path);

	//void LoadBuffers();

	ID3D11Buffer* GetVertexBuffer() const;
	ID3D11Buffer* GetIndexBuffer() const;
	const int GetIndexCount() const;

	~Mesh();
};
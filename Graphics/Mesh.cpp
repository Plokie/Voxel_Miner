#include "Mesh.h"

bool CreateBuffer(ID3D11Device* pDevice, D3D11_USAGE usage, SIZE_T stride, UINT bindFlags, UINT CPUAccessFlags, ID3D11Buffer** targetBuffer, void* arrSrc = nullptr) {
	//BUFFER
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC)); // Clear out any garbage
	desc.Usage = usage;
	desc.ByteWidth = (UINT)stride;
	desc.BindFlags = bindFlags;
	desc.CPUAccessFlags = CPUAccessFlags;
	desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	if(arrSrc != nullptr) {
		data.pSysMem = arrSrc;
	}

	HRESULT hr = pDevice->CreateBuffer(&desc, arrSrc == nullptr ? 0 : &data, targetBuffer);
	if(FAILED(hr)) {
		exit(42);
		return false;
	}// Failed to create buffer
	return true;
}

Mesh* Mesh::Init(ID3D11Device* device) {
	pDevice = device;
	return this;
}

void Mesh::LoadDefaultCube() {
	LoadVertices(exampleCubeVertices, ARRAYSIZE(exampleCubeVertices));
	LoadIndices(exampleCubeIndices, ARRAYSIZE(exampleCubeIndices));
	//LoadBuffers();
}

void Mesh::LoadVertices(Vertex vert[], int vertCount) {
	//if(pVertices != nullptr) {
	//	delete[vertexCount] pVertices;
	//}
	//pVertices = vert;
	vertexCount = vertCount;

	if(vertexBuffer) vertexBuffer->Release();
	delete vertexBuffer;
	CreateBuffer(pDevice, D3D11_USAGE_DEFAULT, sizeof(Vertex) * vertexCount, D3D11_BIND_VERTEX_BUFFER, 0, &vertexBuffer, vert);
}
void Mesh::LoadIndices(DWORD ind[], int indCount) {
	//if(pIndices != nullptr) {
	//	delete[indexCount] pIndices;
	//}
	//pIndices = ind;
	indexCount = indCount;

	if(indexBuffer) indexBuffer->Release();
	delete indexBuffer;
	CreateBuffer(pDevice, D3D11_USAGE_DEFAULT, sizeof(int) * indexCount, D3D11_BIND_INDEX_BUFFER, 0, &indexBuffer, ind);
}

//void Mesh::LoadFromFile(wstring path) {
//	
//}

//void Mesh::LoadBuffers() {
//	if(vertexBuffer) vertexBuffer->Release();
//	if(indexBuffer) indexBuffer->Release();
//	delete vertexBuffer;
//	delete indexBuffer;
//	CreateBuffer(pDevice, D3D11_USAGE_DEFAULT, sizeof(Vertex) * vertexCount, D3D11_BIND_VERTEX_BUFFER, 0, &vertexBuffer, pVertices);
//	CreateBuffer(pDevice, D3D11_USAGE_DEFAULT, sizeof(int) * indexCount, D3D11_BIND_INDEX_BUFFER, 0, &indexBuffer, pIndices);
//}

ID3D11Buffer* Mesh::GetVertexBuffer() const {
	return vertexBuffer;
}

ID3D11Buffer* Mesh::GetIndexBuffer() const {
	return indexBuffer;
}

const int Mesh::GetIndexCount() const {
	return indexCount;
}

Mesh::~Mesh() {
	if (this == nullptr) return;
	//if(pVertices != nullptr) {
	//	delete[vertexCount] pVertices;
	//}
	//if(pIndices != nullptr) {
	//	delete[indexCount] pIndices;
	//}
	if (vertexBuffer)  {
		vertexBuffer->Release();
		vertexBuffer = nullptr;
	}
	if (indexBuffer) {
		indexBuffer->Release();
		indexBuffer = nullptr;
	}
}
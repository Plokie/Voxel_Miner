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
	Vertex cubeVertices[] = {
		Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f), //bottom-left		0  -Z
		Vertex(-1.0f,  1.0f, -1.0f, 0.0f, 0.0f), //top-left			1
		Vertex(1.0f,  1.0f, -1.0f, 1.0f, 0.0f), //top-right		2
		Vertex(1.0f, -1.0f, -1.0f, 1.0f, 1.0f), //bottom-right		3

		Vertex(1.0f, -1.0f, 1.0f,	0.0f, 1.0f), //bottom-left		4  +Z
		Vertex(1.0f,  1.0f, 1.0f,	0.0f, 0.0f), //top-left			5
		Vertex(-1.0f,  1.0f, 1.0f,	1.0f, 0.0f), //top-right			6
		Vertex(-1.0f, -1.0f, 1.0f,	1.0f, 1.0f), //bottom-right		7

		Vertex(1.0f, -1.0f, -1.0f,	0.0f, 1.0f), //bottom-left		8  +X
		Vertex(1.0f,  1.0f, -1.0f,	0.0f, 0.0f), //top-left			9
		Vertex(1.0f,  1.0f, 1.0f,	1.0f, 0.0f), //top-right		10
		Vertex(1.0f, -1.0f, 1.0f,	1.0f, 1.0f), //bottom-right		11

		Vertex(-1.0f, -1.0f, 1.0f,	0.0f, 1.0f), //bottom-left		12  -X
		Vertex(-1.0f,  1.0f, 1.0f,	0.0f, 0.0f), //top-left			13
		Vertex(-1.0f,  1.0f, -1.0f, 1.0f, 0.0f), //top-right		14
		Vertex(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f), //bottom-right		15

		Vertex(-1.0f,  1.0f, -1.0f, 0.0f, 1.0f), //					16 +Y
		Vertex(-1.0f,  1.0f, 1.0f,	0.0f, 0.0f), //					17
		Vertex(1.0f,  1.0f, 1.0f,	1.0f, 0.0f), //					18
		Vertex(1.0f,  1.0f, -1.0f,	1.0f, 1.0f), //					19

		Vertex(-1.0f, -1.0f, 1.0f,	0.0f, 1.0f), //					20 -Y
		Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 0.0f), //					21
		Vertex(1.0f, -1.0f, -1.0f,  1.0f, 0.0f), //					22
		Vertex(1.0f, -1.0f, 1.0f,	1.0f, 1.0f), //					23
	};

	int cubeIndices[] = {
		0,1,2, //-Z
		0,2,3,

		4,5,6, //+Z
		4,6,7,

		8,9,10, //+X
		8,10,11,

		12,13,14, //-X
		12,14,15,

		16,17,18, //+Y
		16,18,19,

		20,21,22, //-Y
		20,22,23
	};

	LoadVertices(cubeVertices, ARRAYSIZE(cubeVertices));
	LoadIndices(cubeIndices, ARRAYSIZE(cubeIndices));
	LoadBuffers();
}

void Mesh::LoadVertices(Vertex vert[], int vertCount) {
	//if(pVertices != nullptr) {
	//	delete[vertexCount] pVertices;
	//}
	pVertices = vert;
	vertexCount = vertCount;
}
void Mesh::LoadIndices(int ind[], int indCount) {
	//if(pIndices != nullptr) {
	//	delete[indexCount] pIndices;
	//}
	pIndices = ind;
	indexCount = indCount;
}

void Mesh::LoadBuffers() {
	if(vertexBuffer) vertexBuffer->Release();
	if(indexBuffer) indexBuffer->Release();
	delete vertexBuffer;
	delete indexBuffer;
	CreateBuffer(pDevice, D3D11_USAGE_DEFAULT, sizeof(Vertex) * vertexCount, D3D11_BIND_VERTEX_BUFFER, 0, &vertexBuffer, pVertices);
	CreateBuffer(pDevice, D3D11_USAGE_DEFAULT, sizeof(int) * indexCount, D3D11_BIND_INDEX_BUFFER, 0, &indexBuffer, pIndices);
}

Mesh::~Mesh() {
	//if(pVertices != nullptr) {
	//	delete[vertexCount] pVertices;
	//}
	//if(pIndices != nullptr) {
	//	delete[indexCount] pIndices;
	//}
	if(vertexBuffer) vertexBuffer->Release();
	if(indexBuffer) indexBuffer->Release();
}
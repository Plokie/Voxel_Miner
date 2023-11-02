#pragma once

#include <DirectXMath.h>

class Vertex {
public:
	Vertex() : pos(0.0f,0.0f,0.0f), uv(0,0), normal(0,0,0), uvOffset(0,0){}
	Vertex(float x, float y, float z) : pos(x, y, z), uv(0,0), normal(0,0,0), uvOffset(0, 0) {}
	Vertex(float x, float y, float z, float u, float v) : pos(x, y, z), uv(u, v), normal(0,0,0), uvOffset(0, 0) {}
	Vertex(float x, float y, float z, float nx, float ny, float nz, float u, float v) : pos(x, y, z), normal(nx, ny, nz), uv(u, v), uvOffset(0, 0) {}
	Vertex(float x, float y, float z, float nx, float ny, float nz, float u, float v, float ou, float ov) : pos(x, y, z), normal(nx, ny, nz), uv(u, v), uvOffset(ou, ov) {}
	//Vertex(float x, float y, float z, float r, float g, float b) : pos(x, y, z), col(r,g,b){}


	DirectX::XMFLOAT3 pos;
	//DirectX::XMFLOAT3 col;
	DirectX::XMFLOAT2 uv;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT2 uvOffset;
};
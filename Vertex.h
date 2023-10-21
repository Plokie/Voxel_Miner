#pragma once

#include <DirectXMath.h>

class Vertex {
public:
	Vertex() : pos(0.0f,0.0f,0.0f), col(1,1,1){}
	Vertex(float x, float y, float z) : pos(x, y, z), col(1, 1, 1) {}
	Vertex(float x, float y, float z, float r, float g, float b) : pos(x, y, z), col(r,g,b){}


	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 col;
};
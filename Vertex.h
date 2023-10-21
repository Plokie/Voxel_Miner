#pragma once

#include <DirectXMath.h>

class Vertex {
public:
	Vertex() : pos(0.0f,0.0f), col(1,1,1){}
	Vertex(float x, float y) : pos(x, y), col(1, 1, 1) {}
	Vertex(float x, float y, float r, float g, float b) : pos(x, y), col(r,g,b){}


	DirectX::XMFLOAT2 pos;
	DirectX::XMFLOAT3 col;
};
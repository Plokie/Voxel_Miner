#pragma once

#include <DirectXMath.h>

class Vertex {
	Vertex(){}
	Vertex(float x, float y) : pos(x, y){}

	DirectX::XMFLOAT2 pos;
};
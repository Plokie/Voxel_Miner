#pragma once


#include <d3d11.h>
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"DirectXTK.lib")
#pragma comment(lib,"DXGI.lib")

#include "../Engine/MathUtil.h"

using namespace DirectX;

// also change Graphics.h
#define MAX_SHADOW_CASCADES 4

struct VSCbuffer_Light {
	XMMATRIX lightViewMx[MAX_SHADOW_CASCADES];
	XMMATRIX lightProjMx[MAX_SHADOW_CASCADES];
	float lightSizes[MAX_SHADOW_CASCADES];
};

struct VSCbuffer_Model {
	XMMATRIX modelMx;
	XMMATRIX viewMx;
	XMMATRIX projMx;
	float time;
};

struct VSCbuffer_Camera {
	XMMATRIX viewMx;
	XMMATRIX projMx;
};

struct CB_VS_pixelshader {
	float alpha = 1.0f;
	Vector2 uvOffset = { 0.f, 0.f };
	float pad;
	//XMMATRIX lightMx;
	//XMMATRIX lightMxInv;
};
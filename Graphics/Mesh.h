#pragma once

#include <Windows.h>
#include <vector>
#include <d3d11.h>
#include <WICTextureLoader.h>
#include <DDSTextureLoader.h>
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"DirectXTK.lib")
#pragma comment(lib,"DXGI.lib")

#include "Shaders.h"
#include "Vertex.h"
#include "..\Engine\Transform.h"

using namespace DirectX;

struct CB_VS_vertexshader {
	DirectX::XMMATRIX mx;
};

struct CB_VS_pixelshader {
	float alpha = 1.0f;
};


class Mesh {
private:
	bool CreateBuffer(D3D11_USAGE usage, SIZE_T stride, UINT bindFlags, UINT CPUAccessFlags, ID3D11Buffer** targetBuffer, void* arrSrc = nullptr) {
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

		HRESULT hr = pDevice->CreateBuffer(&desc, arrSrc==nullptr?0:&data, targetBuffer);
		if(FAILED(hr)) {
			exit(42);
			return false;
		}// Failed to create buffer
		return true;
	}

	ID3D11Buffer* vertexBuffer = nullptr;
	ID3D11Buffer* indexBuffer = nullptr;
	ID3D11Buffer* constantBuffer = nullptr;
	ID3D11Buffer* alphaBuffer = nullptr;
	ID3D11Device* pDevice = nullptr;

public:

	SIZE_T indexCount = 0;
	float alpha = 1.0f;

	void Init(ID3D11Device* device) {
		pDevice = device;

		Vertex vertices[] = {
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

		DWORD indices[] = {
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

		//ARRAYSIZE(v), ARRAYSIZE(indices)
		this->indexCount = ARRAYSIZE(indices);

		CreateBuffer(D3D11_USAGE_DEFAULT, sizeof(Vertex) * ARRAYSIZE(vertices), D3D11_BIND_VERTEX_BUFFER, 0, &vertexBuffer, vertices);
		CreateBuffer(D3D11_USAGE_DEFAULT, sizeof(DWORD) * ARRAYSIZE(indices), D3D11_BIND_INDEX_BUFFER, 0, &indexBuffer, indices);
		CreateBuffer(D3D11_USAGE_DYNAMIC, sizeof(CB_VS_vertexshader) + (16 - (sizeof(CB_VS_vertexshader) % 16)), D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, &constantBuffer);
		CreateBuffer(D3D11_USAGE_DYNAMIC, sizeof(CB_VS_pixelshader) + (16 - (sizeof(CB_VS_pixelshader) % 16)), D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, &alphaBuffer);
	}

	bool IsTransparent() {
		return alpha < 1.f;
	}

	void Draw(ID3D11DeviceContext* deviceCtx, XMMATRIX selfMx, XMMATRIX baseMx) {
		CB_VS_vertexshader mxData;
		CB_VS_pixelshader alphaData;

		alphaData.alpha = alpha;
		
		mxData.mx = selfMx * baseMx;

		//Convert to row-major for HLSL
		mxData.mx = DirectX::XMMatrixTranspose(mxData.mx); 

		D3D11_MAPPED_SUBRESOURCE map;
		HRESULT hr = deviceCtx->Map(constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
		CopyMemory(map.pData, &mxData, sizeof(CB_VS_vertexshader));
		deviceCtx->Unmap(constantBuffer, 0);
		deviceCtx->VSSetConstantBuffers(0, 1, &constantBuffer);

		deviceCtx->Map(alphaBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
		CopyMemory(map.pData, &alphaData, sizeof(CB_VS_pixelshader));
		deviceCtx->Unmap(alphaBuffer, 0);
		deviceCtx->PSSetConstantBuffers(0, 1, &alphaBuffer);


		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		deviceCtx->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

		deviceCtx->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);


		deviceCtx->DrawIndexed((UINT)indexCount, 0, 0);
	}

	Mesh() {

	}

	~Mesh() {
		if(vertexBuffer) vertexBuffer->Release();
		if(indexBuffer) indexBuffer->Release();
		if(constantBuffer) constantBuffer->Release();
	}
};
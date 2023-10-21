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

class VBO {
private:
	bool CreateBuffer(ID3D11Device* device, UINT stride, UINT bindFlags, ID3D11Buffer** targetBuffer, void* arr, UINT exitCode = 1337) {
		//BUFFER
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC)); // Clear out any garbage
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = stride;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
		data.pSysMem = arr;

		HRESULT hr = device->CreateBuffer(&desc, &data, targetBuffer);
		if(FAILED(hr)) {
			exit(exitCode);
			return false;
		}// Failed to create buffer
		return true;
	}


public:
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	SIZE_T indexCount = 0;

	void Populate(ID3D11Device* device, Vertex vertices[], DWORD indices[], SIZE_T vertexCount, SIZE_T indexCount) {
		//indexCount = sizeof(DWORD) / sizeof(indices);
		this->indexCount = indexCount;

		CreateBuffer(device, sizeof(Vertex) * vertexCount, D3D11_BIND_VERTEX_BUFFER, &(vertexBuffer), vertices, 40);
		CreateBuffer(device, sizeof(DWORD) * indexCount, D3D11_BIND_INDEX_BUFFER, &(indexBuffer), indices, 41);
	}

	void Draw(ID3D11DeviceContext* deviceCtx) {
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		deviceCtx->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
		deviceCtx->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

		deviceCtx->DrawIndexed(indexCount, 0, 0);
	}



	~VBO() {
		if(vertexBuffer) vertexBuffer->Release();
		if(indexBuffer) indexBuffer->Release();
	}
};
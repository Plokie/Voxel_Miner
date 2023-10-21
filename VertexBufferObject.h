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

using namespace DirectX;

struct CB_VS_vertexshader {
	DirectX::XMMATRIX mx;
};


class VBO {
private:
	bool CreateBuffer(ID3D11Device* device, UINT stride, UINT bindFlags, ID3D11Buffer** targetBuffer, void* arr, UINT exitCode) {
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

	bool CreateCBuffer(ID3D11Device* device) {
		D3D11_BUFFER_DESC cbd;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0;
		cbd.ByteWidth = static_cast<UINT>(sizeof(CB_VS_vertexshader) + (16 - (sizeof(CB_VS_vertexshader) % 16)));
		cbd.StructureByteStride = 0;

		HRESULT hr = device->CreateBuffer(&cbd, 0, &constantBuffer);
		if(FAILED(hr)) {
			exit(42);
			return false;
		}
		return true;
	}


public:
	ID3D11Buffer* vertexBuffer = nullptr;
	ID3D11Buffer* indexBuffer = nullptr;
	ID3D11Buffer* constantBuffer = nullptr;

	SIZE_T indexCount = 0;

	float debug = 0.0f;

	void Populate(ID3D11Device* device, Vertex vertices[], DWORD indices[], SIZE_T vertexCount, SIZE_T indexCount) {
		//indexCount = sizeof(DWORD) / sizeof(indices);
		this->indexCount = indexCount;

		CreateBuffer(device, static_cast<UINT>(sizeof(Vertex) * vertexCount), D3D11_BIND_VERTEX_BUFFER, &(vertexBuffer), vertices, 40);
		CreateBuffer(device, static_cast<UINT>(sizeof(DWORD) * indexCount), D3D11_BIND_INDEX_BUFFER, &(indexBuffer), indices, 41);
		/*CreateBuffer(
			device, 
			sizeof(CB_VS_c_vertexshader) + (16 - (sizeof(CB_VS_c_vertexshader)%16)), 
			D3D11_BIND_CONSTANT_BUFFER, 
			&(constantBuffer), 
			0, 
			42, 
			D3D11_USAGE_DYNAMIC, 
			D3D11_CPU_ACCESS_WRITE
		);*/
		CreateCBuffer(device);
	}

	void Draw(ID3D11DeviceContext* deviceCtx, XMMATRIX baseMx, float dTime) {
		CB_VS_vertexshader data;
		
		debug += dTime;

		//Local transformations
		data.mx = XMMatrixIdentity();
		data.mx *= XMMatrixRotationRollPitchYaw(0.0f, debug, 0.0f); //Rotate Y-axis 45 deg
		//data.mx = XMMatrixScaling(0.5f, 1.0f, 1.0f);
		//data.mx *= XMMatrixTranslation(0.0f, -0.5f, 0.0f);

		//Apply world + view + project matrix
		data.mx *= baseMx;

		//Convert to row-major for HLSL
		data.mx = DirectX::XMMatrixTranspose(data.mx); 

		D3D11_MAPPED_SUBRESOURCE map;
		HRESULT hr = deviceCtx->Map(constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
		CopyMemory(map.pData, &data, sizeof(CB_VS_vertexshader));
		deviceCtx->Unmap(constantBuffer, 0);
		deviceCtx->VSSetConstantBuffers(0, 1, &constantBuffer);


		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		deviceCtx->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
		deviceCtx->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

		deviceCtx->DrawIndexed((UINT)indexCount, 0, 0);
	}



	~VBO() {
		if(vertexBuffer) vertexBuffer->Release();
		if(indexBuffer) indexBuffer->Release();
		if(constantBuffer) constantBuffer->Release();
	}
};
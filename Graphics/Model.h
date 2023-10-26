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
#include "Resources.h"
#include "Mesh.h"

using namespace DirectX;

struct CB_VS_vertexshader {
	DirectX::XMMATRIX mx;
	XMMATRIX modelMx;
};

struct CB_VS_pixelshader {
	float alpha = 1.0f;
};


class Model {
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

	/*ID3D11Buffer* vertexBuffer = nullptr;
	ID3D11Buffer* indexBuffer = nullptr;*/

	Mesh* mesh = nullptr;

	ID3D11Buffer* constantBuffer = nullptr;
	ID3D11Buffer* alphaBuffer = nullptr;
	ID3D11Device* pDevice = nullptr;

	ID3D11ShaderResourceView* tex = nullptr;
	VertexShader* vertexShader = nullptr;
	PixelShader* pixelShader = nullptr;

public:

	//SIZE_T indexCount = 0;
	float alpha = 1.0f;

	void Init(ID3D11Device* device);

	bool IsTransparent() {
		return alpha < 1.f;
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="deviceCtx"></param>
	/// <param name="modelMx">local object mx</param>
	/// <param name="worldMx">cam view mx * cam proj mx</param>
	void Draw(ID3D11DeviceContext* deviceCtx, XMMATRIX modelMx, XMMATRIX worldMx);

	void SetTexture(int idx, ID3D11ShaderResourceView* tex) {
		this->tex = tex;
	}
	void SetTexture(int idx, string name) {
		SetTexture(idx, Resources::GetTexture(name));
	}

	void SetPixelShader(int idx, PixelShader* pixelShader) {
		this->pixelShader = pixelShader;
	}
	void SetPixelShader(int idx, string name) {
		SetPixelShader(idx, Resources::GetPixelShader(name));
	}

	void SetVertexShader(int idx, VertexShader* vertexShader) {
		this->vertexShader = vertexShader;
	}
	void SetVertexShader(int idx, string name) {
		SetVertexShader(idx, Resources::GetVertexShader(name));
	}
	void SetMesh(Mesh* mesh) {
		this->mesh = mesh;
	}
	void SetMesh(string name) {
		SetMesh(Resources::GetMesh(name));
	}

	Model() {

	}

	~Model() {
		/*if(vertexBuffer) vertexBuffer->Release();
		if(indexBuffer) indexBuffer->Release();*/
		if(constantBuffer) constantBuffer->Release();
	}
};
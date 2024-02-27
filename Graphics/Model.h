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
#include "DefaultCbuffers.h"

using namespace DirectX;

class Camera;

enum MODEL_FLAG : unsigned int {
	MF_NONE = 0,
	MF_TRANSPARENT = 1,
	MF_DO_NOT_WRITE_TO_SUN_DEPTH = 2,
};

/// <summary>
/// Contains a mesh and additional const buffers, textures and shaders used to render the mesh
/// </summary>
class Model {
private:
	//bool CreateBuffer(D3D11_USAGE usage, SIZE_T stride, UINT bindFlags, UINT CPUAccessFlags, ID3D11Buffer** targetBuffer, void* arrSrc = nullptr) {
	//	//BUFFER
	//	D3D11_BUFFER_DESC desc;
	//	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC)); // Clear out any garbage
	//	desc.Usage = usage;
	//	desc.ByteWidth = (UINT)stride;
	//	desc.BindFlags = bindFlags;
	//	desc.CPUAccessFlags = CPUAccessFlags;
	//	desc.MiscFlags = 0;

	//	D3D11_SUBRESOURCE_DATA data;
	//	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	//	if(arrSrc != nullptr) {
	//		data.pSysMem = arrSrc;
	//	}

	//	HRESULT hr = pDevice->CreateBuffer(&desc, arrSrc==nullptr?0:&data, targetBuffer);
	//	if(FAILED(hr)) {
	//		exit(42);
	//		return false;
	//	}// Failed to create buffer
	//	return true;
	//}

	/*ID3D11Buffer* vertexBuffer = nullptr;
	ID3D11Buffer* indexBuffer = nullptr;*/

	Mesh* mesh = nullptr;

	ID3D11Buffer* vsCbuffer_Model = nullptr;
	ID3D11Buffer* psCbuffer = nullptr;
	ID3D11Device* pDevice = nullptr;

	ID3D11ShaderResourceView* tex = nullptr;
	VertexShader* vertexShader = nullptr;
	PixelShader* pixelShader = nullptr;

	Vector2 uvOffset = { 0.f, 0.f };
	unsigned int flags = 0;

	void SetupCbuffers(ID3D11DeviceContext* deviceCtx, const XMMATRIX& modelMx, const XMMATRIX& viewMx, const XMMATRIX& projMx);

public:
	//SRWLOCK gAcquireMutex;
	//SIZE_T indexCount = 0;
	//todo: materials
	float alpha = 1.0f;

	void SetFlags(unsigned int flags);
	bool HasFlag(MODEL_FLAG flag) const;


	Model* Init(ID3D11Device* device);

	bool IsTransparent() {
		return alpha < 1.f;
	}
	void SetTransparent(bool _IsTransparent) {
		this->alpha = _IsTransparent ? 0.99f : 1.0f;
	}

	void SetUVOffset(const Vector2& uvOffset) {
		this->uvOffset = uvOffset;
	}

	static Model* Create(ID3D11Device* device) {
		Model* newModel = new Model();
		newModel->Init(device);

		newModel->SetTexture(0, "err");
		newModel->SetPixelShader(0, "pixelshader");
		newModel->SetVertexShader(0, "vertexshader");

		return newModel;
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="deviceCtx"></param>
	/// <param name="modelMx">local object mx</param>
	/// <param name="worldMx">cam view mx * cam proj mx</param>
	//void Draw(ID3D11DeviceContext* deviceCtx, XMMATRIX modelMx, XMMATRIX worldMx);

	void Draw(ID3D11DeviceContext* deviceCtx, const XMMATRIX& modelMx, const XMMATRIX& viewMx, const XMMATRIX& projMx);
	void Draw(ID3D11DeviceContext* deviceCtx, const XMMATRIX& modelMx, const XMMATRIX& viewMx, const XMMATRIX& projMx, ID3D11PixelShader* ps, ID3D11VertexShader* vs);
	void DrawShadows(ID3D11DeviceContext* deviceCtx, const XMMATRIX& modelMx, ID3D11PixelShader* ps, ID3D11VertexShader* vs, Camera** cameras, ID3D11DepthStencilView** stencils, D3D11_VIEWPORT* viewports, int amount);

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
	Mesh* GetMesh() {
		return this->mesh;
	}

	// ONLY CALL IF THE MESH IS CREATED AT RUNTIME!
	void ReleaseMesh();

	Model() {

	}

	~Model();
};
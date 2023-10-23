#pragma once

#include <map>
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
#include "Mesh.h"
#include "Camera.h"
#include "../Engine/Object3D.h"
//#include "../Engine/World.h"

using namespace DirectX;
using namespace std;

class Graphics {
private:
	ID3D11Device* device;						// 
	ID3D11DeviceContext* deviceCtx;				// Resource allocation
	IDXGISwapChain* swapChain;					// Used to swap frames while rendering. Buffered rendering
	ID3D11RenderTargetView* renderTargetView;	// Render target window

	VertexShader vertexShader;
	PixelShader pixelShader;

	//todo: make into vector of buffers (or gameobjects with meshes/vbos)
	//Mesh* mesh;

	ID3D11DepthStencilView* depthStencilView;
	ID3D11Texture2D* depthBuffer;

	ID3D11DepthStencilState* depthStencilState;
	ID3D11DepthStencilState* alphaDepthStencilState;

	ID3D11RasterizerState* rasterizerState;

	IDXGIAdapter* targetAdapter;

	ID3D11SamplerState* samplerState;
	ID3D11ShaderResourceView* tex;

	ID3D11BlendState* blendState;

	bool ChooseAdapter();
	bool SetupSwapChain(HWND hwnd);
	bool SetupDepthBuffer();
	bool SetupDepthStencil();
	bool SetupAlphaDepthStencil();
	bool SetupViewport();
	bool SetupRasterizer();
	bool SetupSamplerState();
	bool SetupBlendState();


	bool InitResolution(HWND hwnd);

	bool InitDX(HWND hwnd);
	bool InitShaders();
	bool InitScene();
public:
	Camera camera;
	int windowWidth=0, windowHeight=0;

	bool CreateBuffer(UINT stride, UINT bindFlags, ID3D11Buffer** targetBuffer, void* arr, UINT exitCode);

	bool Init(HWND hwnd, int width, int height);

	void Render(map<string, Object3D*>& sceneObjects);

	void SortObjects(vector<Object3D*>& objects, int startIndex, int endIndex);

	ID3D11Device* GetDevice() {
		return device;
	}

	~Graphics() {
		if(device) device->Release();
		if(deviceCtx) deviceCtx->Release();
		if(swapChain) swapChain->Release();
		if(renderTargetView) renderTargetView->Release();

		if(depthStencilView) depthStencilView->Release();
		if(depthBuffer) depthBuffer->Release();
		if(depthStencilState) depthStencilState->Release();

		if(rasterizerState) rasterizerState->Release();

		if(samplerState) samplerState->Release();
		if(tex) tex->Release();

		if (blendState) blendState->Release();
		//if(mesh) delete mesh;
	}
};
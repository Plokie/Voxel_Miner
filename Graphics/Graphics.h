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
#include "VertexBufferObject.h"

using namespace DirectX;

class Graphics {
private:
	ID3D11Device* device;						// 
	ID3D11DeviceContext* deviceCtx;				// Resource allocation
	IDXGISwapChain* swapChain;					// Used to swap frames while rendering. Buffered rendering
	ID3D11RenderTargetView* renderTargetView;	// Render target window

	VertexShader vertexShader;
	PixelShader pixelShader;

	//todo: make into vector of buffers (or gameobjects with meshes/vbos)
	VBO* vbo;

	ID3D11DepthStencilView* depthStencilView;
	ID3D11Texture2D* depthBuffer;
	ID3D11DepthStencilState* depthStencilState;

	ID3D11RasterizerState* rasterizerState;

	IDXGIAdapter* targetAdapter;

	ID3D11SamplerState* samplerState;
	ID3D11ShaderResourceView* tex;

	bool ChooseAdapter();
	bool SetupSwapChain(HWND hwnd);
	bool SetupDepthBuffer();
	bool SetupDepthStencil();
	bool SetupViewport();
	bool SetupRasterizer();
	bool SetupSamplerState();


	bool InitResolution(HWND hwnd);

	bool InitDX(HWND hwnd);
	bool InitShaders();
	bool InitScene();
public:
	int windowWidth=0, windowHeight=0;

	bool CreateBuffer(UINT stride, UINT bindFlags, ID3D11Buffer** targetBuffer, void* arr, UINT exitCode);

	bool Init(HWND hwnd, int width, int height);

	void Render(float dTime);

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

		if(vbo) delete vbo;
	}
};
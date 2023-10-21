#pragma once

#include <Windows.h>
#include <vector>
#include <d3d11.h>
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"DirectXTK.lib")
#pragma comment(lib,"DXGI.lib")

#include "Shaders.h"
#include "Vertex.h"

class Graphics {
private:
	ID3D11Device* device;						// 
	ID3D11DeviceContext* deviceCtx;				// Resource allocation
	IDXGISwapChain* swapChain;					// Used to swap frames while rendering. Buffered rendering
	ID3D11RenderTargetView* renderTargetView;	// Render target window

	VertexShader vertexShader;
	PixelShader pixelShader;

	//todo: make into vector
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* vertexBuffer2;

	ID3D11DepthStencilView* depthStencilView;
	ID3D11Texture2D* depthBuffer;
	ID3D11DepthStencilState* depthStencilState;

	ID3D11RasterizerState* rasterizerState;

	bool InitDX(HWND hwnd, int width, int height);
	bool InitShaders();
	bool InitScene();
public:
	bool Init(HWND hwnd, int width, int height);

	void TestRender();

	~Graphics() {
		if(device) device->Release();
		if(deviceCtx) deviceCtx->Release();
		if(swapChain) swapChain->Release();
		if(renderTargetView) renderTargetView->Release();

		if(vertexBuffer) vertexBuffer->Release();
		if(vertexBuffer2) vertexBuffer2->Release();

		if(depthStencilView) depthStencilView->Release();
		if(depthBuffer) depthBuffer->Release();
		if(depthStencilState) depthStencilState->Release();

		if(rasterizerState) rasterizerState->Release();
	}
};
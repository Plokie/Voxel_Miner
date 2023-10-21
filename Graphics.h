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

	//todo: make into vector of buffers
	/*ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;*/
	VBO* vbo;

	ID3D11DepthStencilView* depthStencilView;
	ID3D11Texture2D* depthBuffer;
	ID3D11DepthStencilState* depthStencilState;

	ID3D11RasterizerState* rasterizerState;

	IDXGIAdapter* targetAdapter;

	ID3D11SamplerState* samplerState;
	ID3D11ShaderResourceView* tex;

	bool ChooseAdapter();
	bool SetupSwapChain(HWND hwnd, int width, int height);
	bool SetupDepthBuffer(int width, int height);
	bool SetupDepthStencil();
	bool SetupViewport(int width, int height);
	bool SetupRasterizer();
	bool SetupSamplerState();


	bool InitResolution(HWND hwnd, int width, int height);

	bool InitDX(HWND hwnd, int width, int height);
	bool InitShaders();
	bool InitScene();
public:

	bool CreateBuffer(UINT stride, UINT bindFlags, ID3D11Buffer** targetBuffer, void* arr, UINT exitCode);

	bool Init(HWND hwnd, int width, int height);

	void Render();

	~Graphics() {
		if(device) device->Release();
		if(deviceCtx) deviceCtx->Release();
		if(swapChain) swapChain->Release();
		if(renderTargetView) renderTargetView->Release();

		/*if(vertexBuffer) vertexBuffer->Release();
		if(indexBuffer) vertexBuffer->Release();*/
		//if(vertexBuffer2) vertexBuffer2->Release();

		if(depthStencilView) depthStencilView->Release();
		if(depthBuffer) depthBuffer->Release();
		if(depthStencilState) depthStencilState->Release();

		if(rasterizerState) rasterizerState->Release();

		if(samplerState) samplerState->Release();
		if(tex) tex->Release();

		if(vbo) delete vbo;
	}
};
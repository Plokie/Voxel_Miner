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
#include "Model.h"
#include "Camera.h"
#include "../Engine/Object3D.h"
//#include "../Engine/World.h"

using namespace DirectX;
using namespace std;

#define LAYOUTSIZE 3

class Graphics {
private:
	ID3D11Device* device;						// 
	ID3D11DeviceContext* deviceCtx;				// Resource allocation
	IDXGISwapChain* swapChain;					// Used to swap frames while rendering. Buffered rendering
	ID3D11RenderTargetView* renderTargetView;	// Render target window

	//todo: Make into resource collection of shaders and textures
	VertexShader defaultVertexShader;
	PixelShader defaultPixelShader;
	ID3D11ShaderResourceView* errTex;
	D3D11_INPUT_ELEMENT_DESC layout[LAYOUTSIZE];

	ID3D11DepthStencilView* depthStencilView;
	ID3D11Texture2D* depthBuffer;

	ID3D11DepthStencilState* depthStencilState;
	ID3D11DepthStencilState* alphaDepthStencilState;

	ID3D11RasterizerState* rasterizerState;

	IDXGIAdapter* targetAdapter;

	ID3D11SamplerState* samplerState;
	

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

	void ResizeSwapchain();

	bool InitDX(HWND hwnd);
	bool InitShaders();
	bool InitScene();

	static Graphics* _Instance;
public:
	Camera camera;
	int windowWidth=0, windowHeight=0;

	static Graphics* Get() {
		return _Instance;
	}

	bool CreateBuffer(UINT stride, UINT bindFlags, ID3D11Buffer** targetBuffer, void* arr, UINT exitCode);

	bool Init(HWND hwnd, int width, int height);

	bool OnResize(HWND hwnd, int width, int height);

	void Render(map<string, Object3D*>& sceneObjects);

	void SortObjects(vector<Object3D*>& objects, int startIndex, int endIndex);

	ID3D11Device* GetDevice() { return device; }

	D3D11_INPUT_ELEMENT_DESC* GetLayout() { return layout; }

	ID3D11DeviceContext* GetDeviceCtx() { return deviceCtx; }

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
		if(errTex) errTex->Release();

		if (blendState) blendState->Release();
	}
};
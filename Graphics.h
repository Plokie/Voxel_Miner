#pragma once

#include <Windows.h>
#include <vector>
#include <d3d11.h>
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"DirectXTK.lib")
#pragma comment(lib,"DXGI.lib")

class Graphics {
private:
	ID3D11Device* device;						// 
	ID3D11DeviceContext* deviceCtx;				// Resource allocation
	IDXGISwapChain* swapChain;					// Used to swap frames while rendering. Buffered rendering
	ID3D11RenderTargetView* renderTargetView;	// Render target window

	bool InitDX(HWND hwnd, int width, int height);
public:
	bool Init(HWND hwnd, int width, int height);

	void TestRender();

	~Graphics();
};
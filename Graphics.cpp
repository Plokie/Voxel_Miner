#include "Graphics.h"

using namespace std;
//using namespace DirectX;

bool Graphics::InitDX(HWND hwnd, int width, int height) {
	// Get current DXGI factory (i think its like a state of the system devices)
	IDXGIFactory* dxFactory = nullptr;
	HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&dxFactory);
	if(FAILED(hr)) {
		exit(10);
	}

	// Iterator variables for looping through the adapters
	IDXGIAdapter* itAdapter;
	DXGI_ADAPTER_DESC itDesc;
	
	// Stores the current largest adapter (GPU) found (and its info)
	SIZE_T mostMem = 0;
	IDXGIAdapter* bestAdapter = nullptr;
	DXGI_ADAPTER_DESC bestAdapterDesc;
	
	UINT index = 0; // Loop through each adapter
	while(SUCCEEDED(dxFactory->EnumAdapters(index, &itAdapter))) {
		itAdapter->GetDesc(&itDesc); //Get desc, and compare video mem
		if(itDesc.DedicatedVideoMemory > mostMem) {
			// If its bigger than current, use this as best gpu
			mostMem = itDesc.DedicatedVideoMemory;
			bestAdapter = itAdapter;
			bestAdapterDesc = itDesc;
		}

		index++;
	}
	dxFactory->Release();
	itAdapter->Release();

	if(mostMem == 0) { // something has gone terribly wrong (no gpus!) (or the gpu that does exist has 0 vram)
		exit(11);
		return false;
	}

	//MessageBox(0, bestAdapterDesc.Description, 0, 0); //debug box showing best gpu

	// Create swapchain description to set swapchain information when creating device
	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	scd.BufferDesc.Width = width;
	scd.BufferDesc.Height = height;
	scd.BufferDesc.RefreshRate.Numerator = 60;	//VSync refresh rate if used/needed
	scd.BufferDesc.RefreshRate.Denominator = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = 0;

	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.BufferCount = 1;
	
	scd.OutputWindow = hwnd;
	scd.Windowed = true;
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; //Allows to swtich between fullscreen, windowed, borderless, etc....

	hr = D3D11CreateDeviceAndSwapChain(
		bestAdapter,
		D3D_DRIVER_TYPE_UNKNOWN,
		NULL,						//Software driver someting
		NULL,						//FLAGS
		NULL,						//FEATURE LEVELS
		NULL,						//FEATURE LEVELS SIZE
		D3D11_SDK_VERSION,			//sdk ver
		&scd,
		&this->swapChain,
		&this->device,
		NULL,						//SUPPORTED FEATURE LEVELS
		&this->deviceCtx
	);

	if(FAILED(hr)) { // uh oh, swap chain setup failed
		exit(6);
		return false;
	}

	//
	ID3D11Texture2D* backBuffer;
	hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
	if(FAILED(hr)) {
		exit(12); //Failed to get or set back buffer
		return false;
	}

	//
	hr = device->CreateRenderTargetView(backBuffer, NULL, &renderTargetView);
	if(FAILED(hr)) {
		exit(13); //Failed to create render target
		return false;
	}

	deviceCtx->OMSetRenderTargets(1, &renderTargetView, 0);
	
	return true;
}

bool Graphics::Init(HWND hwnd, int width, int height) {
	//
	// Extra stuff here, non-init-dx related (shaders etc)
	//
	
	if(!InitDX(hwnd, width, height)) {
		return false;
	}
	return true;
}

void Graphics::TestRender() {
	float bgCol[] = {1.0, 0.6, 1.0, 1.0};
	deviceCtx->ClearRenderTargetView(renderTargetView, bgCol);

	swapChain->Present(0, NULL);
}

Graphics::~Graphics() {
	if(device) device->Release();
	if(deviceCtx) deviceCtx->Release();
	if(swapChain) swapChain->Release();
	if(renderTargetView) renderTargetView->Release();
}
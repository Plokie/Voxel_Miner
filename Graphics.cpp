#include "Graphics.h"

using namespace std;
//using namespace DirectX;

bool Graphics::ChooseAdapter() {
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
	targetAdapter = nullptr;
	DXGI_ADAPTER_DESC bestAdapterDesc;

	UINT index = 0; // Loop through each adapter
	while(SUCCEEDED(dxFactory->EnumAdapters(index, &itAdapter))) {
		itAdapter->GetDesc(&itDesc); //Get desc, and compare video mem
		if(itDesc.DedicatedVideoMemory > mostMem) {
			// If its bigger than current, use this as best gpu
			mostMem = itDesc.DedicatedVideoMemory;
			targetAdapter = itAdapter;
			bestAdapterDesc = itDesc;
		}

		index++;
	}
	dxFactory->Release();
	//itAdapter->Release();

	if(mostMem == 0) { // something has gone terribly wrong (no gpus!) (or the gpu that does exist has 0 vram)
		exit(11);
		return false;
	}
	return true;
}

bool Graphics::SetupSwapChain(HWND hwnd, int width, int height) {
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

	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		targetAdapter,
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

	return true;
}

bool Graphics::SetupDepthBuffer(int width, int height) {
	D3D11_TEXTURE2D_DESC dDesc;
	dDesc.Width = width;
	dDesc.Height = height;
	dDesc.MipLevels = 1;
	dDesc.ArraySize = 1;
	dDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dDesc.SampleDesc.Count = 1;
	dDesc.SampleDesc.Quality = 0;
	dDesc.Usage = D3D11_USAGE_DEFAULT;
	dDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	dDesc.CPUAccessFlags = 0;
	dDesc.MiscFlags = 0;

	HRESULT hr = device->CreateTexture2D(&dDesc, 0, &depthBuffer);
	if(FAILED(hr)) {
		exit(14);
		return false;
	}

	hr = device->CreateDepthStencilView(depthBuffer, 0, &depthStencilView);
	if(FAILED(hr)) {
		exit(15);
		return false;
	}
	return true;
}

bool Graphics::SetupDepthStencil() {
	D3D11_DEPTH_STENCIL_DESC dsDesc;
	ZeroMemory(&dsDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	HRESULT hr = device->CreateDepthStencilState(&dsDesc, &depthStencilState);
	if(FAILED(hr)) {
		exit(16);
		return false;
	}
	return true;
}

bool Graphics::SetupViewport(int width, int height) {
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = width;
	viewport.Height = height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	deviceCtx->RSSetViewports(1, &viewport);
	return true;
}

bool Graphics::SetupRasterizer() {
	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	HRESULT hr = device->CreateRasterizerState(&rasterizerDesc, &rasterizerState);
	if(FAILED(hr)) {
		exit(17); return false; //Failed to create rasterizer state
	}
	return true;
}

bool Graphics::SetupSamplerState() {
	D3D11_SAMPLER_DESC sd;
	ZeroMemory(&sd, sizeof(D3D11_SAMPLER_DESC));
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sd.MinLOD = 0;
	sd.MaxLOD = D3D11_FLOAT32_MAX;
	
	HRESULT hr = device->CreateSamplerState(&sd, &samplerState);
	if(FAILED(hr)) {
		exit(18); return false;
	}
	return true;
}

// Can be called when resolution changed
bool Graphics::InitResolution(HWND hwnd, int width, int height) {
	SetupSwapChain(hwnd, width, height);

	SetupDepthBuffer(width, height);

	deviceCtx->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

	SetupDepthStencil();

	SetupViewport(width, height);

	SetupRasterizer();
	return true;
}

bool Graphics::InitDX(HWND hwnd, int width, int height) {
	ChooseAdapter();

	//MessageBox(0, targetAdapter.Description, 0, 0); //debug box showing best gpu name

	InitResolution(hwnd, width, height);

	SetupSamplerState();
	
	return true;
}

bool Graphics::InitShaders() {

	// INPUT ASSEMBLER ---

	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0},
		//{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	
	UINT numElements = ARRAYSIZE(layout);

	
	// ---

	// INIT VERTEX SHADERS ---------
	std::wstring shaderFolder = L"";
#pragma region GetShaderPath
#ifdef _DEBUG
		shaderFolder = L"x64\\Debug\\";
#else
		shaderFolder = L"x64\\Release\\";
#endif


	if(!vertexShader.Init(device, shaderFolder + L"vertexshader.cso", layout, numElements)) {
		exit(20);
		return false;
	}
	// ------------

	// INIT PIXEL SHADERS ------------

	if(!pixelShader.Init(device, shaderFolder + L"pixelshader.cso")) {
		exit(21);
		return false;
	}

	// --------------------
	

	return true;
}

bool Graphics::CreateBuffer(UINT stride, UINT bindFlags, ID3D11Buffer** targetBuffer, void* arr, UINT exitCode = 1337) {
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

bool Graphics::InitScene() {
	Vertex v[] = {
		Vertex(-1.0f, -1.0f, 1.0f, 0.0f, 1.0f), //bottom-left
		Vertex(-1.0f,  1.0f, 1.0f, 0.0f, 0.0f), //top-left
		Vertex( 1.0f,  1.0f, 1.0f, 1.0f, 0.0f), //top-right
		Vertex( 1.0f, -1.0f, 1.0f, 1.0f, 1.0f), //bottom-right
	};

	DWORD indices[] = {
		0,1,2,
		0,2,3,
	};

	

	vbo->Populate(device, v, indices, ARRAYSIZE(v), ARRAYSIZE(indices));

	// Load png tex
	//hr = CreateWICTextureFromFile(device, L"Data\\Textures\\img.png", nullptr, &tex);

	// Load dds tex (faster + accurate colour space)
	HRESULT hr = CreateDDSTextureFromFile(device, L"Data\\Textures\\img.dds", nullptr, &tex, 0, 0); 

	if(FAILED(hr)) exit(41);

	return true;
}

bool Graphics::Init(HWND hwnd, int width, int height) {
	vbo = new VBO();

	if(!InitDX(hwnd, width, height)) {
		return false;
	}

	if(!InitShaders()) {
		return false;
	}

	if(!InitScene()) {
		return false;
	}

	return true;
}

void Graphics::Render() {
	float bgCol[] = {1.0, 0.6, 1.0, 1.0};
	deviceCtx->ClearRenderTargetView(renderTargetView, bgCol);
	deviceCtx->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	//

	deviceCtx->IASetInputLayout(vertexShader.GetInputLayout());
	deviceCtx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceCtx->RSSetState(rasterizerState);

	deviceCtx->OMSetDepthStencilState(depthStencilState, 0);

	deviceCtx->PSSetSamplers(0, 1, &samplerState);

	deviceCtx->VSSetShader(vertexShader.GetShader(), NULL, 0);
	deviceCtx->PSSetShader(pixelShader.GetShader(), NULL, 0);

	deviceCtx->PSSetShaderResources(0, 1, &tex);

	
	// DRAW SCENE

	/*UINT stride = sizeof(Vertex);
	UINT offset = 0;
	deviceCtx->IASetVertexBuffers(0, 1, &vbo->vertexBuffer, &stride, &offset);
	deviceCtx->IASetIndexBuffer(vbo->indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	deviceCtx->DrawIndexed(6, 0, 0);*/

	vbo->Draw(deviceCtx);

	//


	swapChain->Present(0, NULL);
}


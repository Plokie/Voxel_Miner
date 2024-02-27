#include "Graphics.h"
#include "../Engine/Input.h"
//#include "../DebugMsg.h"


using namespace std;
//using namespace DirectX;

Graphics* Graphics::_Instance;

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

bool Graphics::SetupSwapChain(HWND hwnd, IDXGISwapChain** outSwapChain) {
	// Create swapchain description to set swapchain information when creating device
	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	scd.BufferDesc.Width = windowWidth;
	scd.BufferDesc.Height = windowHeight;
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
		outSwapChain,
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
	hr = (*outSwapChain)->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
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

	backBuffer->Release();

	return true;
}

void Graphics::ResizeSwapchain() {
	HRESULT hr = this->swapChain->ResizeBuffers(1, this->windowWidth, this->windowHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
	if (FAILED(hr)) {
		exit(11); return;
	}
	
	ID3D11Texture2D* backBuffer;
	hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
	if (FAILED(hr)) {
		exit(12); //Failed to get or set back buffer
		return;
	}

	hr = device->CreateRenderTargetView(backBuffer, NULL, &renderTargetView);
	if (FAILED(hr)) {
		exit(13); //Failed to create render target
		return;
	}
	backBuffer->Release();
}

bool Graphics::SetupDepthBuffer() {
	D3D11_TEXTURE2D_DESC dDesc;
	dDesc.Width = windowWidth;
	dDesc.Height = windowHeight;
	dDesc.MipLevels = 1;
	dDesc.ArraySize = 1;
	dDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dDesc.SampleDesc.Count = 1;
	dDesc.SampleDesc.Quality = 0;
	dDesc.Usage = D3D11_USAGE_DEFAULT;
	dDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	dDesc.CPUAccessFlags = 0;
	dDesc.MiscFlags = 0;

	//dDesc.SampleDesc.Count = 4;
	//dDesc.SampleDesc.Quality = 1;

	HRESULT hr = device->CreateTexture2D(&dDesc, 0, &this->depthBuffer);
	if(FAILED(hr)) {
		exit(14);
		return false;
	}

	hr = device->CreateDepthStencilView(depthBuffer, 0, &this->depthStencilView);
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
	//dsDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;

	HRESULT hr = device->CreateDepthStencilState(&dsDesc, &this->depthStencilState);
	if(FAILED(hr)) {
		exit(16);
		return false;
	}
	return true;
}

bool Graphics::SetupAlphaDepthStencil() {
	D3D11_DEPTH_STENCIL_DESC dsDesc;
	ZeroMemory(&dsDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	//dsDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;
	//dsDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;

	HRESULT hr = device->CreateDepthStencilState(&dsDesc, &alphaDepthStencilState);
	if(FAILED(hr)) {
		exit(161);
		return false;
	}
	return true;
}

bool Graphics::SetupViewport() {
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (float)windowWidth;
	viewport.Height = (float)windowHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	deviceCtx->RSSetViewports(1, &viewport);
	return true;
}

bool Graphics::SetupRasterizer() {
	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	//rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
	//rasterizerDesc.CullMode = D3D11_CULL_NONE;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.DepthClipEnable = true;
	//rasterizerDesc.AntialiasedLineEnable = TRUE;
	//rasterizerDesc.MultisampleEnable = TRUE;
	//rasterizerDesc.
	HRESULT hr = device->CreateRasterizerState(&rasterizerDesc, &rasterizerState);
	if(FAILED(hr)) {
		exit(17); return false; //Failed to create rasterizer state
	}
	return true;
}

bool Graphics::SetupSamplerStateLinear() {
	D3D11_SAMPLER_DESC sd;
	ZeroMemory(&sd, sizeof(D3D11_SAMPLER_DESC));
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	//sd.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
	//sd.Filter = D3D11_FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR;
	//sd.Filter = D3D11_FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT;
	//sd.Filter = D3D11_FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT;
	//sd.Filter = D3D11_FILTER::D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
	//sd.Filter = D3D11_FILTER::D3D11_FILTER_COMPARISON_ANISOTROPIC;
	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sd.MinLOD = 0;
	sd.MaxLOD = D3D11_FLOAT32_MAX;
	
	HRESULT hr = device->CreateSamplerState(&sd, &samplerStateLinear);
	if(FAILED(hr)) {
		exit(18); return false;
	}
	return true;
}

bool Graphics::SetupSamplerStatePoint() {
	D3D11_SAMPLER_DESC sd;
	ZeroMemory(&sd, sizeof(D3D11_SAMPLER_DESC));
	//sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	//sd.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	sd.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
	// 
	//sd.Filter = D3D11_FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR;
	//sd.Filter = D3D11_FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT;
	//sd.Filter = D3D11_FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT;
	//sd.Filter = D3D11_FILTER::D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
	//sd.Filter = D3D11_FILTER::D3D11_FILTER_COMPARISON_ANISOTROPIC;
	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sd.MinLOD = 0;
	sd.MaxLOD = D3D11_FLOAT32_MAX;

	HRESULT hr = device->CreateSamplerState(&sd, &samplerStatePoint);
	if(FAILED(hr)) {
		exit(18); return false;
	}
	return true;
}

bool Graphics::SetupBlendState() {
	D3D11_BLEND_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BLEND_DESC));
	
	D3D11_RENDER_TARGET_BLEND_DESC rtbd;
	rtbd.BlendEnable = true;
	rtbd.SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA; // Alpha source (is just the alpha input)
	rtbd.DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
	rtbd.BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	rtbd.SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
	rtbd.DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
	rtbd.BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	rtbd.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL;

	desc.RenderTarget[0] = rtbd;

	HRESULT hr = device->CreateBlendState(&desc, &blendState);
	if (FAILED(hr)) {
		exit(19);
		return false;
	}
	return true;
}

bool Graphics::SetupSpriteBatch() {
	
	this->spriteBatch = new SpriteBatch(this->deviceCtx);

	//this->testSpriteFont = new SpriteFont(this->device, L"Data\\Fonts\\algerian.spritefont");
	
	return true;
}

bool Graphics::CreateBuffer(D3D11_USAGE usage, SIZE_T stride, UINT bindFlags, UINT CPUAccessFlags, ID3D11Buffer** targetBuffer, void* arrSrc) {
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

	HRESULT hr = Graphics::Get()->device->CreateBuffer(&desc, arrSrc == nullptr ? 0 : &data, targetBuffer);
	if(FAILED(hr)) {
		exit(42);
		return false;
	}// Failed to create buffer
	return true;
}

bool Graphics::SetupDefaultCbuffers()
{
	CreateBuffer(D3D11_USAGE_DYNAMIC, sizeof(VSCbuffer_Camera) + (16 - (sizeof(VSCbuffer_Camera) % 16)), D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, &vsCbuffer_Camera);
	CreateBuffer(D3D11_USAGE_DYNAMIC, sizeof(VSCbuffer_Light) + (16 - (sizeof(VSCbuffer_Light) % 16)), D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, &vsCbuffer_Light);

	return true;
}

void Graphics::InitializeShadowmapSampler()
{
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.BorderColor[0] = 1.0f;
	samplerDesc.BorderColor[1] = 1.0f;
	samplerDesc.BorderColor[2] = 1.0f;
	samplerDesc.BorderColor[3] = 1.0f;
	samplerDesc.MinLOD = 0.f;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	samplerDesc.MipLODBias = 0.f;
	samplerDesc.MaxAnisotropy = 0;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS;
	//samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	//samplerDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
	samplerDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
	//samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	//samplerDesc.Filter = D3D11_FILTER_

	HRESULT hr = device->CreateSamplerState(&samplerDesc, &shadowSamplerState);
	if(FAILED(hr)) {
		exit(53);
	}

	//

	D3D11_RASTERIZER_DESC rastDesc;
	ZeroMemory(&rastDesc, sizeof(D3D11_RASTERIZER_DESC));
	rastDesc.FillMode = D3D11_FILL_SOLID;
	//rastDesc.CullMode = D3D11_CULL_BACK;
	rastDesc.CullMode = D3D11_CULL_NONE;
	rastDesc.DepthClipEnable = true;
	hr = device->CreateRasterizerState(&rastDesc, &shadowRastState);
	if(FAILED(hr)) {
		exit(54);
	}
}

bool Graphics::SetupShadowmapBuffer(ID3D11Texture2D** depthTex, ID3D11DepthStencilView** depthStencilView, ID3D11ShaderResourceView** depthResourceView, D3D11_VIEWPORT* viewport, int resX, int resY)
{
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
	desc.Format = DXGI_FORMAT_R32_TYPELESS;
	//desc.Format = DXGI_FORMAT_R24G8_TYPELESS;

	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	//desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
	desc.Width = static_cast<UINT>(resX);
	desc.Height = static_cast<UINT>(resY);

	HRESULT hr = device->CreateTexture2D(&desc, nullptr, depthTex);
	if (FAILED(hr)) {
		exit(50);
		return false;
	}
	//

	D3D11_DEPTH_STENCIL_VIEW_DESC stencilDesc;
	ZeroMemory(&stencilDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	stencilDesc.Format = DXGI_FORMAT_D32_FLOAT;
	//stencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	//stencilDesc.Flags
	stencilDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	stencilDesc.Texture2D.MipSlice = 0;

	hr = device->CreateDepthStencilView(*depthTex, &stencilDesc, depthStencilView);
	//hr = device->CreateDepthStencilView(shadowDepthTex, 0, &shadowDepthView);
	if(FAILED(hr)) exit(51);

	D3D11_SHADER_RESOURCE_VIEW_DESC resDesc;
	ZeroMemory(&resDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	resDesc.Format = DXGI_FORMAT_R32_FLOAT;
	//resDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;

	resDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	resDesc.Texture2D.MipLevels = desc.MipLevels;
	resDesc.Texture2D.MostDetailedMip = 0;

	hr = device->CreateShaderResourceView(*depthTex, &resDesc, depthResourceView);
	if(FAILED(hr)) exit(52);

	ZeroMemory(viewport, sizeof(D3D11_VIEWPORT));
	viewport->Width = static_cast<float>(resX);
	viewport->Height = static_cast<float>(resY);
	viewport->MinDepth = 0.f;
	viewport->MaxDepth = 1.f;

	return true;
}

void Graphics::AddShadowCamera(float size)
{
	Camera* newShadowCam = new Camera();

	newShadowCam->SetProjectionOthographic(size, 1.f, 800.f);
	//newShadowCam->SetProjectionMatrix(projection);

	for(auto& pCam : shadowCameras) {
		if(pCam == nullptr) {
			pCam = newShadowCam;
			return;
		}
	}

	//MAX SHADOW CAMERAS / CASCADES
	assert(false);

	//shadowCameras.push_back(newShadowCam);
}

bool Graphics::InitResolution(HWND hwnd) {
	SetupSwapChain(hwnd, &this->swapChain);

	SetupDepthBuffer();

	deviceCtx->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

	SetupViewport();

	SetupRasterizer();
	return true;
}

bool Graphics::InitDX(HWND hwnd) {
	ChooseAdapter();

	//MessageBox(0, targetAdapter.Description, 0, 0); //debug box showing best gpu name

	InitResolution(hwnd);

	SetupDepthStencil();

	SetupAlphaDepthStencil();

	SetupDefaultCbuffers();
	SetupSamplerStateLinear();
	SetupSamplerStatePoint();

	SetupBlendState();

	SetupSpriteBatch();


	InitializeShadowmapSampler();
	

	// move to setup shadow cameras function

	const int shadowRes[] = {
		8000, 2000, 1000, 500
	};

	for(int i = 0; i < ARRAYSIZE(shadowRes); i++) {
		SetupShadowmapBuffer(&shadowDepthTex[i], &shadowDepthView[i], &shadowResourceView[i], &shadowViewport[i], shadowRes[i], shadowRes[i]);
	}
	//SetupShadowmapBuffer(&shadowDepthTex[0], &shadowDepthView[0], &shadowResourceView[0], &shadowViewport[0], 4000, 4000);
	

	
	return true;
}

bool Graphics::InitShaders() {

	// INPUT ASSEMBLER ---

	D3D11_INPUT_ELEMENT_DESC newLayout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXOFFSET", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0},
		//{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	memcpy(&layout, &newLayout, sizeof(newLayout));
	
	UINT numElements = ARRAYSIZE(layout);

	
	// ---

	std::wstring shaderFolder = L"Data\\Shaders\\";
	// INIT DEFAULT VERTEX SHADER ---------

	if(!defaultVertexShader.Init(device, shaderFolder + L"vertexshader.cso", layout, numElements)) {
		exit(20);
		return false;
	}
	// ------------

	// INIT DEFAULT PIXEL SHADER ------------

	if(!defaultPixelShader.Init(device, shaderFolder + L"pixelshader.cso")) {
		exit(21);
		return false;
	}

	// --------------------

	if(!shadowVertexShader.Init(device, shaderFolder + L"shadowvertexshader.cso", layout, numElements)) {
		exit(22);
	}
	if(!shadowPixelShader.Init(device, shaderFolder + L"shadowpixelshader.cso")) {
		exit(23);
	}
	

	return true;
}


bool Graphics::InitScene() {
	//mesh->Init(device);

	// Load png tex
	//hr = CreateWICTextureFromFile(device, L"Data\\Textures\\img.png", nullptr, &tex);

	// Load dds tex (faster + accurate colour space)
	HRESULT hr = CreateDDSTextureFromFile(device, L"Data\\Textures\\err.dds", nullptr, &errTex, 0, 0); 
	if(FAILED(hr)) exit(41);

	camera.transform.position = Vector3(0.0f, 0.0f, -6.0f);
	camera.SetProjectionValues(90.f, static_cast<float>(windowWidth) / static_cast<float>(windowHeight), 0.05f, 1000.f);

	sun.position = Vector3(0.f, 0.f, 0.f);
	sun.rotation = Vector3(45.f, 45.f, 0.f);

	//shadowCamera.SetProjectionMatrix(XMMatrixOrthographicLH(100.f, 100.f, 0.01f, 800.f));
	AddShadowCamera(100.f);
	//AddShadowCamera(30.f);
	//AddShadowCamera(100.f);
	//AddShadowCamera(500.f);

	
	
	


	return true;
}


bool Graphics::Init(HWND hwnd, int width, int height) {
	//mesh = new Mesh();

	_Instance = this;

	windowWidth = width;
	windowHeight = height;

	InitializeSRWLock(&gRenderingMutex);

	if(!InitDX(hwnd)) {
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

bool Graphics::OnResize(HWND hwnd, int width, int height) {
	if (this == nullptr) return false;

	if(this->renderTargetView) this->renderTargetView->Release();
	if(this->depthBuffer) this->depthBuffer->Release();
	if(this->depthStencilView) this->depthStencilView->Release();
	if(this->depthStencilState) this->depthStencilState->Release();
	if(this->alphaDepthStencilState) this->alphaDepthStencilState->Release();

	windowWidth = width;
	windowHeight = height;

	ResizeSwapchain();

	SetupDepthStencil();
	SetupAlphaDepthStencil();

	SetupDepthBuffer();
	
	deviceCtx->OMSetRenderTargets(1, &this->renderTargetView, this->depthStencilView);

	SetupViewport();

	return true;
}

void Graphics::RenderShadowMap(Scene* scene)
{
	const float bg[] = { 0.f, 0.f, 0.f, 1.0f };

	D3D11_MAPPED_SUBRESOURCE resMap;

	VSCbuffer_Light lightData;
	ZeroMemory(&lightData, sizeof(VSCbuffer_Light));

	//shadowCamera.transform.position = camera.transform.position;

	sun.position = camera.transform.position;

	int index = 0;
	for(auto pShadowCam : shadowCameras) {
		if(pShadowCam == nullptr) continue;

		deviceCtx->ClearDepthStencilView(shadowDepthView[index], D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		deviceCtx->OMSetRenderTargets(0, nullptr, shadowDepthView[index]);

		deviceCtx->RSSetViewports(1, &shadowViewport[index]);
		deviceCtx->RSSetState(shadowRastState);


		deviceCtx->PSSetShader(shadowPixelShader.GetShader(), nullptr, 0);
		deviceCtx->VSSetShader(shadowVertexShader.GetShader(), nullptr, 0);

		pShadowCam->transform.position = sun.position + (sun.back() * 400.f);
		pShadowCam->transform.rotation = sun.rotation;
		pShadowCam->UpdateViewFrustum();

		lightData.lightViewMx[index] = XMMatrixTranspose(pShadowCam->transform.mxView());
		lightData.lightProjMx[index] = XMMatrixTranspose(pShadowCam->GetProjectionMatrix());
		lightData.lightSizes[index] = pShadowCam->GetOrthographicSize();
		
		index++;
	}

	for(auto it = scene->GetSceneObjects3D()->begin(); it != scene->GetSceneObjects3D()->end(); it++) {
		if(it->second == nullptr) continue;
		if(/*(it->second->cullBox.GetHalfSize().magnitude() == 0.0f || shadowCameras[index-1]->IsAABBInFrustum(it->second->cullBox)) &&*/ it->second->doRender)
			it->second->DrawShadows(deviceCtx, nullptr, nullptr, MF_DO_NOT_WRITE_TO_SUN_DEPTH, shadowCameras, shadowDepthView, shadowViewport, index);

	}


	//	// CAMERA DATA
	//	//VSCbuffer_Camera camData;
	//	//ZeroMemory(&camData, sizeof(VSCbuffer_Camera));
	//	//camData.viewMx = XMMatrixTranspose(pShadowCam->transform.mxView());
	//	//camData.projMx = XMMatrixTranspose(pShadowCam->GetProjectionMatrix());
	//	//HRESULT hr = deviceCtx->Map(vsCbuffer_Camera, 0, D3D11_MAP_WRITE_DISCARD, 0, &resMap);
	//	//CopyMemory(resMap.pData, &camData, sizeof(VSCbuffer_Camera));
	//	//deviceCtx->Unmap(vsCbuffer_Camera, 0);
	//	//deviceCtx->VSSetConstantBuffers(1, 1, &vsCbuffer_Camera);

	//	for(auto it = scene->GetSceneObjects3D()->begin(); it != scene->GetSceneObjects3D()->end(); it++) {
	//		if(it->second == nullptr) continue;
	//		//if((it->second->cullBox.GetHalfSize().magnitude() == 0.0f || pShadowCam->IsAABBInFrustum(it->second->cullBox)) && it->second->doRender)
	//			//it->second->Draw(deviceCtx, pShadowCam->transform.mxView(), pShadowCam->GetProjectionMatrix(), nullptr, nullptr, MF_DO_NOT_WRITE_TO_SUN_DEPTH);
	//	}
	//}

	HRESULT hr = deviceCtx->Map(vsCbuffer_Light, 0, D3D11_MAP_WRITE_DISCARD, 0, &resMap);
	CopyMemory(resMap.pData, &lightData, sizeof(VSCbuffer_Light));
	deviceCtx->Unmap(vsCbuffer_Light, 0);
	deviceCtx->VSSetConstantBuffers(2, 1, &vsCbuffer_Light);



}


void Graphics::Render(Scene* scene) {
	D3D11_MAPPED_SUBRESOURCE resMap;
	unique_lock<std::mutex> lock(scene->createObjectMutex);

	deviceCtx->IASetInputLayout(defaultVertexShader.GetInputLayout());
	deviceCtx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//float bgCol[] = {1.0, 0.6, 1.0, 1.0};
	//const float bgCol[] = { 145.f / 255.f, 217.f / 255.f, 1.0f, 1.0f };
	//const float bgCol[] = { 4.f / 255.f, 2.f / 255.f, 26.0f / 255.f, 1.0f };	
	
	
	deviceCtx->ClearRenderTargetView(renderTargetView, &scene->clearColor.x);
	deviceCtx->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	//deviceCtx->ClearRenderTargetView()


	//

	deviceCtx->OMSetDepthStencilState(depthStencilState, 0);
	deviceCtx->OMSetBlendState(blendState, NULL, 0xFFFFFFFF);


	RenderShadowMap(scene);

	deviceCtx->OMSetRenderTargets(1, &renderTargetView, depthStencilView);
	
	
	/*ID3D11RenderTargetView* targets[] = {renderTargetView, shadowRenderTarget};
	deviceCtx->OMSetRenderTargets(2, targets, depthStencilView);*/

	deviceCtx->RSSetState(rasterizerState);
	deviceCtx->RSSetViewports(1, &viewport);

	deviceCtx->PSSetSamplers(0, 1, &samplerStatePoint);
	deviceCtx->PSSetSamplers(1, 1, &shadowSamplerState);

	deviceCtx->VSSetShader(defaultVertexShader.GetShader(), NULL, 0);
	deviceCtx->PSSetShader(defaultPixelShader.GetShader(), NULL, 0);

	deviceCtx->PSSetShaderResources(0, 1, &errTex);
	deviceCtx->PSSetShaderResources(1, MAX_SHADOW_CASCADES, &shadowResourceView[0]);

	// set cbuffers for this frame

	// CAMERA DATA didnt work, but i feel like it could in the future, so leaving here for now
	//VSCbuffer_Camera camData;
	//ZeroMemory(&camData, sizeof(VSCbuffer_Camera));
	//camData.viewMx = XMMatrixTranspose(camera.transform.mxView());
	//camData.projMx = XMMatrixTranspose(camera.GetProjectionMatrix());
	//HRESULT hr = deviceCtx->Map(vsCbuffer_Camera, 0, D3D11_MAP_WRITE_DISCARD, 0, &resMap);
	//CopyMemory(resMap.pData, &camData, sizeof(VSCbuffer_Camera));
	//deviceCtx->Unmap(vsCbuffer_Camera, 0);
	//deviceCtx->VSSetConstantBuffers(1, 1, &vsCbuffer_Camera);

	//

	XMMATRIX worldMx = XMMatrixIdentity();

	// DRAW SCENE

	camera.UpdateViewFrustum();

	vector<tuple<Model*, XMMATRIX, Object3D*>> transparentModels = {}; // Transparent meshes to be drawn AFTER the opaque geometry
	vector<Object3D*> objects = {};
	objects.reserve(scene->GetSceneObjects3D()->size());

	//todo: precompute sceneObjects values vector whenever an object is appended or removed
	for(map<string, Object3D*>::iterator it = scene->GetSceneObjects3D()->begin(); it != scene->GetSceneObjects3D()->end(); ++it) {

		// if object has an AABB, and is visible by the camera
		if(it->second != nullptr) {
			if((it->second->cullBox.GetHalfSize().magnitude() == 0.0f || camera.IsAABBInFrustum(it->second->cullBox)) && it->second->doRender) {
				if(it->second->renderType == ONCE_PER_FRAME) it->second->doRender = false;
				objects.push_back(it->second);
			}
		}


		// aka: dont queue objects hidden to the camera to be drawn
	}

	Sort3DObjects(objects, 0, (int)(objects.size() - 1));

	for(auto it = objects.begin(); it != objects.end(); ++it) {

		{	unique_lock<std::mutex> lock((*it)->gAccessMutex);
			if ((*it)->Draw(deviceCtx, camera.transform.mxView(), camera.GetProjectionMatrix(), &transparentModels)) {
				//If it drew something, return back to default error textures+shaders afterwards (so we can see missing tex objects)
				deviceCtx->PSSetShaderResources(0, 1, &errTex);
				deviceCtx->VSSetShader(defaultVertexShader.GetShader(), NULL, 0);
				deviceCtx->PSSetShader(defaultPixelShader.GetShader(), NULL, 0);
			}
		}
	}

	// Set depth stencil to alpha geometry mode
	deviceCtx->OMSetDepthStencilState(alphaDepthStencilState, 0);

	//Draw alpha geometry
	for(auto it = transparentModels.begin(); it!=transparentModels.end(); ++it) {
		//AcquireSRWLockExclusive(it->first)
		Model*& model = get<0>(*it);
		Object3D*& obj = get<2>(*it);
		if (model->GetMesh() == nullptr || obj == nullptr || obj->pendingDeletion ) continue;

		{	unique_lock<std::mutex> lock(obj->gAccessMutex);
			model->Draw(deviceCtx, get<1>(*it), camera.transform.mxView(), camera.GetProjectionMatrix());
			deviceCtx->PSSetShaderResources(0, 1, &errTex);
			deviceCtx->VSSetShader(defaultVertexShader.GetShader(), NULL, 0);
			deviceCtx->PSSetShader(defaultPixelShader.GetShader(), NULL, 0);

		}

	}

	ID3D11ShaderResourceView* nullSRV[] = { nullptr };
	deviceCtx->PSSetShaderResources(1, 1, nullSRV);


	//

	//deviceCtx->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

	//

	vector<Object2D*> sorted2Dobjects = {};
	sorted2Dobjects.reserve(scene->GetSceneObjects2D()->size());
	for(auto it = scene->GetSceneObjects2D()->begin(); it != scene->GetSceneObjects2D()->end(); it++) {
		if(it->second->DoDraw()) sorted2Dobjects.push_back(it->second);
	}

	Sort2DObjects(sorted2Dobjects, 0, (int)(sorted2Dobjects.size() - 1));

	this->spriteBatch->Begin(SpriteSortMode_Deferred, nullptr, samplerStatePoint);

	//this->testSpriteFont->DrawString(spriteBatch, L"Hello, world!", XMFLOAT2(0, 0));

	//for (map<string, Object2D*>::iterator it = scene->GetSceneObjects2D()->begin(); it != scene->GetSceneObjects2D()->end(); ++it) {
	//	it->second->Draw(this->spriteBatch);
	//}
	for (auto it = sorted2Dobjects.begin(); it != sorted2Dobjects.end(); ++it) {
		(*it)->Draw(this->spriteBatch);
	}


	this->spriteBatch->End();


	swapChain->Present(0, NULL);
}

void Graphics::Sort3DObjects(vector<Object3D*>& objects, int start, int end) {
	// QUICKSORT OBJECTS BY DISTANCE TO CAMERA

	if(start >= end) return;

	int p = 0;
	{ // PARTITION
		float pivot = objects[start]->GetClosestDistance(camera.transform.position);

		int count = 0;
		for(int i = start + 1; i <= end; i++) {
			if(objects[i]->GetClosestDistance(camera.transform.position) > pivot)
				count++;
		}

		int pivotIndex = start + count;
		//swap(objects[pivotIndex], objects[start]);
		iter_swap(objects.begin() + pivotIndex, objects.begin() + start);

		int i = start, j = end;
		while(i < pivotIndex && j > pivotIndex) {
			while(objects[i]->GetClosestDistance(camera.transform.position) > pivot) {
				i++;
			}
			while(objects[j]->GetClosestDistance(camera.transform.position) <= pivot) {
				j--;
			}
			if(i < pivotIndex && j > pivotIndex) {
				iter_swap(objects.begin() + i++, objects.begin() + j--);
			}
		}

		p = pivotIndex;
	}

	Sort3DObjects(objects, start, p - 1);
	Sort3DObjects(objects, p + 1, end);


	//
}

void Graphics::Sort2DObjects(vector<Object2D*>& objects, int start, int end) {
	// QUICKSORT OBJECTS BY DISTANCE TO CAMERA

	if(start >= end) return;

	int p = 0;
	{ // PARTITION
		float pivot = objects[start]->GetDepth();

		int count = 0;
		for(int i = start + 1; i <= end; i++) {
			if(objects[i]->GetDepth() < pivot)
				count++;
		}

		int pivotIndex = start + count;
		//swap(objects[pivotIndex], objects[start]);
		iter_swap(objects.begin() + pivotIndex, objects.begin() + start);

		int i = start, j = end;
		while(i < pivotIndex && j > pivotIndex) {
			while(objects[i]->GetDepth() < pivot) {
				i++;
			}
			while(objects[j]->GetDepth() >= pivot) {
				j--;
			}
			if(i < pivotIndex && j > pivotIndex) {
				iter_swap(objects.begin() + i++, objects.begin() + j--);
			}
		}

		p = pivotIndex;
	}

	Sort2DObjects(objects, start, p - 1);
	Sort2DObjects(objects, p + 1, end);


	//
}

Graphics::~Graphics()
{
	if(device) device->Release();
	if(deviceCtx) deviceCtx->Release();
	if(swapChain) swapChain->Release();
	if(renderTargetView) renderTargetView->Release();

	if(depthStencilView) depthStencilView->Release();
	if(depthBuffer) depthBuffer->Release();
	if(depthStencilState) depthStencilState->Release();

	if(rasterizerState) rasterizerState->Release();

	for(auto& pShadowDepthTex : shadowDepthTex) {
		if(pShadowDepthTex == nullptr) continue;
		pShadowDepthTex->Release();
		//delete pShadowDepthTex;
		pShadowDepthTex = nullptr;
	}

	for(auto& pShadowDepthView : shadowDepthView) {
		if(pShadowDepthView == nullptr) continue;
		pShadowDepthView->Release();
		//delete pShadowDepthView;
		pShadowDepthView = nullptr;
	}

	for(auto& pShadowResourceView : shadowResourceView) {
		if(pShadowResourceView == nullptr) continue;
		pShadowResourceView->Release();
		//delete pShadowResourceView;
		pShadowResourceView = nullptr;
	}

	if(shadowSamplerState) shadowSamplerState->Release();
	if(shadowRastState) shadowRastState->Release();

	for(auto& shadowCam : shadowCameras) {
		delete shadowCam;
		shadowCam = nullptr;
	}

	// Errors?????????????????
	//if(samplerStateLinear) samplerStateLinear->Release();

	//if(samplerStatePoint) samplerStatePoint->Release();

	if(errTex) errTex->Release();

	//if (spriteBatch) delete spriteBatch;

	//if (testSpriteFont) delete testSpriteFont;

	if(blendState) blendState->Release();
}

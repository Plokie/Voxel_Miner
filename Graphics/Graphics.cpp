#include "Graphics.h"
#include "../Engine/Input.h"
#include "../DebugMsg.h"


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

bool Graphics::SetupSwapChain(HWND hwnd) {
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
	D3D11_VIEWPORT viewport;
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

bool Graphics::InitResolution(HWND hwnd) {
	SetupSwapChain(hwnd);

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

	SetupSamplerStateLinear();
	SetupSamplerStatePoint();

	SetupBlendState();

	SetupSpriteBatch();
	
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

	// INIT VERTEX SHADERS ---------
	std::wstring shaderFolder = L"";
#pragma region GetShaderPath
#ifdef _DEBUG
		shaderFolder = L"x64\\Debug\\";
#else
		shaderFolder = L"x64\\Release\\";
#endif


	if(!defaultVertexShader.Init(device, shaderFolder + L"vertexshader.cso", layout, numElements)) {
		exit(20);
		return false;
	}
	// ------------

	// INIT PIXEL SHADERS ------------

	if(!defaultPixelShader.Init(device, shaderFolder + L"pixelshader.cso")) {
		exit(21);
		return false;
	}

	// --------------------
	

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

void Graphics::Render(Scene* scene) {
	//float bgCol[] = {1.0, 0.6, 1.0, 1.0};
	const float bgCol[] = { 145.f / 255.f, 217.f / 255.f, 1.0f, 1.0f };
	//const float bgCol[] = { 4.f / 255.f, 2.f / 255.f, 26.0f / 255.f, 1.0f };
	deviceCtx->ClearRenderTargetView(renderTargetView, bgCol);
	deviceCtx->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	//

	deviceCtx->IASetInputLayout(defaultVertexShader.GetInputLayout());
	deviceCtx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceCtx->RSSetState(rasterizerState);

	deviceCtx->OMSetDepthStencilState(depthStencilState, 0);
	deviceCtx->OMSetBlendState(blendState, NULL, 0xFFFFFFFF);

	deviceCtx->PSSetSamplers(0, 1, &samplerStatePoint);

	deviceCtx->VSSetShader(defaultVertexShader.GetShader(), NULL, 0);
	deviceCtx->PSSetShader(defaultPixelShader.GetShader(), NULL, 0);

	deviceCtx->PSSetShaderResources(0, 1, &errTex);

	XMMATRIX worldMx = XMMatrixIdentity();

	// DRAW SCENE

	camera.UpdateViewFrustum();

	vector<tuple<Model*,XMMATRIX, Object3D*>> transparentModels = {}; // Transparent meshes to be drawn AFTER the opaque geometry
	vector<Object3D*> objects = {};

	//camera.viewFrustum = Frustum::CreateFrustumFromCamera(camera, )

	//AcquireSRWLockExclusive(&gRenderingMutex);

	//todo: precompute sceneObjects values vector whenever an object is appended or removed
	for(map<string, Object3D*>::iterator it = scene->GetSceneObjects3D()->begin(); it != scene->GetSceneObjects3D()->end(); ++it) {
		//AcquireSRWLockExclusive(&it->second->gAccessMutex);
		// if object has an AABB, and is visible by the camera
		if(it->second != nullptr)
		if((it->second->cullBox.GetHalfSize().magnitude() == 0.0f || camera.IsAABBInFrustum(it->second->cullBox)) && it->second->doRender )
			objects.push_back(it->second);
		//AcquireSRWLockExclusive(&it->second->gAccessMutex);

		// aka: dont queue objects hidden to the camera to be drawn
	}

	SortObjects(objects, 0, (int)(objects.size() - 1));

	for(vector<Object3D*>::iterator it = objects.begin(); it != objects.end(); ++it) {
		if(TryAcquireSRWLockExclusive(&(*it)->gAccessMutex)) {
			if ((*it)->Draw(deviceCtx, worldMx * camera.transform.mxView() * camera.GetProjectionMatrix(), &transparentModels)) {
				//If it drew something, return back to default error textures+shaders afterwards (so we can see missing tex objects)
				deviceCtx->PSSetShaderResources(0, 1, &errTex);
				deviceCtx->VSSetShader(defaultVertexShader.GetShader(), NULL, 0);
				deviceCtx->PSSetShader(defaultPixelShader.GetShader(), NULL, 0);
			}
			ReleaseSRWLockExclusive(&(*it)->gAccessMutex);
		}
	}

	// Set depth stencil to alpha geometry mode
	deviceCtx->OMSetDepthStencilState(alphaDepthStencilState, 0);

	//Draw alpha geometry
	for(vector<tuple<Model*, XMMATRIX, Object3D*>>::iterator it = transparentModels.begin(); it!=transparentModels.end(); ++it) {
		//AcquireSRWLockExclusive(it->first)
		Model*& model = get<0>(*it);
		Object3D*& obj = get<2>(*it);
		if (model->GetMesh() == nullptr || obj == nullptr || obj->pendingDeletion ) continue;

		if(TryAcquireSRWLockExclusive(&obj->gAccessMutex)) {
			model->Draw(deviceCtx, get<1>(*it), worldMx * camera.transform.mxView() * camera.GetProjectionMatrix());
			deviceCtx->PSSetShaderResources(0, 1, &errTex);
			deviceCtx->VSSetShader(defaultVertexShader.GetShader(), NULL, 0);
			deviceCtx->PSSetShader(defaultPixelShader.GetShader(), NULL, 0);

			ReleaseSRWLockExclusive(&obj->gAccessMutex);
		}

	}

	//


	this->spriteBatch->Begin();

	//this->testSpriteFont->DrawString(spriteBatch, L"Hello, world!", XMFLOAT2(0, 0));

	//todo: z/depth sort
	for (map<string, Object2D*>::iterator it = scene->GetSceneObjects2D()->begin(); it != scene->GetSceneObjects2D()->end(); ++it) {
		it->second->Draw(this->spriteBatch);
	}
	//ReleaseSRWLockExclusive(&gRenderingMutex);


	this->spriteBatch->End();

	swapChain->Present(0, NULL);
}

void Graphics::SortObjects(vector<Object3D*>& objects, int start, int end) {
	// QUICKSORT OBJECTS BY DISTANCE TO CAMERA

	if(start >= end) return;

	int p = 0;
	{ // PARTITION
		float pivot = objects[start]->transform.position.sqrDistTo(camera.transform.position);

		int count = 0;
		for(int i = start + 1; i <= end; i++) {
			if(objects[i]->transform.position.sqrDistTo(camera.transform.position) > pivot)
				count++;
		}

		int pivotIndex = start + count;
		//swap(objects[pivotIndex], objects[start]);
		iter_swap(objects.begin() + pivotIndex, objects.begin() + start);

		int i = start, j = end;
		while(i < pivotIndex && j > pivotIndex) {
			while(objects[i]->transform.position.sqrDistTo(camera.transform.position) > pivot) {
				i++;
			}
			while(objects[j]->transform.position.sqrDistTo(camera.transform.position) <= pivot) {
				j--;
			}
			if(i < pivotIndex && j > pivotIndex) {
				iter_swap(objects.begin() + i++, objects.begin() + j--);
			}
		}

		p = pivotIndex;
	}

	SortObjects(objects, start, p - 1);
	SortObjects(objects, p + 1, end);


	//
}
#pragma once

#include <map>
#include <Windows.h>
#include <vector>
#include <d3d11.h>
#include <WICTextureLoader.h>
#include <DDSTextureLoader.h>
#include <SpriteBatch.h>
#include <SpriteFont.h>
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"DirectXTK.lib")
#pragma comment(lib,"DXGI.lib")

#include "Shaders.h"
#include "Vertex.h"
#include "Model.h"
#include "Camera.h"
#include "../Engine/Scene.h"
#include "../Engine/Object3D.h"
#include "../Engine/AABB.h"
//#include "../Engine/World.h"

using namespace DirectX;
using namespace std;

#define LAYOUTSIZE 4

//also change Model.h
#define MAX_SHADOW_CASCADES 4

//struct ShadowMap_CBuff {
//	XMFLOAT4X4 projection;
//	XMFLOAT4X4 view;
//	XMFLOAT4 pos;
//};

class Graphics {
private:
	ID3D11Device* device = nullptr;						// 
	ID3D11DeviceContext* deviceCtx = nullptr;				// Resource allocation
	IDXGISwapChain* swapChain = nullptr;					// Used to swap frames while rendering. Buffered rendering
	ID3D11RenderTargetView* renderTargetView = nullptr;	// Render target window

	D3D11_VIEWPORT viewport;

	//todo: Make into resource collection of shaders and textures
	VertexShader defaultVertexShader;
	PixelShader defaultPixelShader;
	ID3D11ShaderResourceView* errTex = nullptr;
	D3D11_INPUT_ELEMENT_DESC layout[LAYOUTSIZE];

	ID3D11DepthStencilView* depthStencilView = nullptr;
	ID3D11Texture2D* depthBuffer = nullptr;

	ID3D11Texture2D* shadowDepthTex[MAX_SHADOW_CASCADES] = {};
	ID3D11DepthStencilView* shadowDepthView[MAX_SHADOW_CASCADES] = {};
	ID3D11ShaderResourceView* shadowResourceView[MAX_SHADOW_CASCADES] = {};
	ID3D11SamplerState* shadowSamplerState = nullptr;
	ID3D11RasterizerState* shadowRastState = nullptr;
	D3D11_VIEWPORT shadowViewport[MAX_SHADOW_CASCADES] = {};
	//ShadowMap_CBuff shadowCbufferData;

	VertexShader shadowVertexShader;
	PixelShader shadowPixelShader;

	ID3D11Buffer* vsCbuffer_Light;
	ID3D11Buffer* vsCbuffer_Camera;

	//ID3D11Buffer* shadowCbuffer;
	//ID3D10Blob* shadowBuffer;
	//XMMATRIX shadowMx;

	ID3D11DepthStencilState* depthStencilState = nullptr;
	ID3D11DepthStencilState* alphaDepthStencilState = nullptr;

	ID3D11RasterizerState* rasterizerState = nullptr;

	IDXGIAdapter* targetAdapter = nullptr;

	ID3D11SamplerState* samplerStateLinear = nullptr;
	ID3D11SamplerState* samplerStatePoint = nullptr;
	
	SpriteBatch* spriteBatch = nullptr;

	ID3D11BlendState* blendState = nullptr;

	

	bool ChooseAdapter();
	bool SetupSwapChain(HWND hwnd, IDXGISwapChain** outSwapChain);
	bool SetupDepthBuffer();
	bool SetupDepthStencil();
	bool SetupAlphaDepthStencil();
	bool SetupViewport();
	bool SetupRasterizer();
	bool SetupSamplerStateLinear();
	bool SetupSamplerStatePoint();
	bool SetupBlendState();
	bool SetupSpriteBatch();
	bool SetupDefaultCbuffers();


	void InitializeShadowmapSampler();
	bool SetupShadowmapBuffer(ID3D11Texture2D** depthTex, ID3D11DepthStencilView** depthStencilView, ID3D11ShaderResourceView** depthResourceView, D3D11_VIEWPORT* shadowViewport, int resX, int resY);
	void AddShadowCamera(float size);

	bool InitResolution(HWND hwnd);

	void ResizeSwapchain();

	bool InitDX(HWND hwnd);
	bool InitShaders();
	bool InitScene();
	void RenderShadowMap(Scene* scene);

	static Graphics* _Instance;
public:
	SRWLOCK gRenderingMutex;
	Camera camera;

	static bool CreateBuffer(D3D11_USAGE usage, SIZE_T stride, UINT bindFlags, UINT CPUAccessFlags, ID3D11Buffer** targetBuffer, void* arrSrc = nullptr);

	Camera* shadowCameras[MAX_SHADOW_CASCADES];

	Transform sun;
	int windowWidth=0, windowHeight=0;

	static Graphics* Get() {
		return _Instance;
	}

	//bool CreateBuffer(UINT stride, UINT bindFlags, ID3D11Buffer** targetBuffer, void* arr, UINT exitCode);

	bool Init(HWND hwnd, int width, int height);

	bool OnResize(HWND hwnd, int width, int height);

	void Render(Scene* scene);

	void Sort3DObjects(vector<Object3D*>& objects, int startIndex, int endIndex);
	void Sort2DObjects(vector<Object2D*>& objects, int startIndex, int endIndex);

	//ID3D11ShaderResourceView* GetShadowResourceView() const {
	//	return shadowResourceView;
	//}

	ID3D11Device* GetDevice() { return device; }

	D3D11_INPUT_ELEMENT_DESC* GetLayout() { return layout; }

	ID3D11DeviceContext* GetDeviceCtx() { return deviceCtx; }

	~Graphics();
};
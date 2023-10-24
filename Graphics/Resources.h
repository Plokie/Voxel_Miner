#pragma once

#include <vector>
#include <map>
#include <WICTextureLoader.h>
#include <DDSTextureLoader.h>
#include <d3d11.h>

#include "Shaders.h"


#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"DirectXTK.lib")
#pragma comment(lib,"DXGI.lib")

using namespace std;
using namespace DirectX;

class Resources {
private:
	ID3D11Device* pDevice = nullptr;
	D3D11_INPUT_ELEMENT_DESC* layout = nullptr;
	int layoutCount = 0;

	map<string, PixelShader*> pixelShaders = {};
	map<string, VertexShader*> vertexShaders = {};
	map<string, ID3D11ShaderResourceView*> textures = {};

	Resources(){}

	static Resources* _Instance;

	static void InitiatedExitCheck();
public:
	Resources(Resources& other) = delete;
	void operator=(const Resources&) = delete;

	static void Init(ID3D11Device* device, D3D11_INPUT_ELEMENT_DESC layout[], int layoutCount);

	static void LoadTexture(const wchar_t* path, string name);
	static void LoadPixelShader(const wchar_t* path, string name);
	static void LoadVertexShader(const wchar_t* path, string name);

	static ID3D11ShaderResourceView* GetTexture(string name);
	static PixelShader* GetPixelShader(string name);
	static VertexShader* GetVertexShader(string name);

	~Resources();
};
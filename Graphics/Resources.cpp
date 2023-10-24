#include "Resources.h"

Resources* Resources::_Instance;

/// <summary>
/// Checks if _Instance is instantiated, if its not it exits the program. It'd be a fatal error anyway
/// </summary>
void Resources::InitiatedExitCheck() {
	if (_Instance == nullptr || _Instance->pDevice == nullptr) {
		exit(100); //Resources not initialised
	}
}

void Resources::Init(ID3D11Device* device, D3D11_INPUT_ELEMENT_DESC layout[], int layoutCount) {
	if (_Instance == nullptr) {
		_Instance = new Resources();
	}

	_Instance->pDevice = device;
	_Instance->layout = layout;
	_Instance->layoutCount = layoutCount;
}

void Resources::LoadTexture(const wchar_t* path, string name) {
	InitiatedExitCheck();

	_Instance->textures[name] = nullptr;

	HRESULT hr = CreateDDSTextureFromFile(_Instance->pDevice, path, nullptr, &_Instance->textures[name], 0, 0);
	if (FAILED(hr)) exit(101);
}

#ifdef _DEBUG
wstring shaderFolder = L"x64\\Debug\\";
#else
wstring shaderFolder = L"x64\\Release\\";
#endif

//if (!vertexShader.Init(device, shaderFolder + L"vertexshader.cso", layout, numElements)) {
//	exit(20);
//}
//// ------------

//// INIT PIXEL SHADERS ------------

//if (!pixelShader.Init(device, shaderFolder + L"pixelshader.cso")) {
//	exit(21);
//}

void Resources::LoadPixelShader(const wchar_t* path, string name) {
	InitiatedExitCheck();

	_Instance->pixelShaders[name] = new PixelShader();

	_Instance->pixelShaders[name]->Init(_Instance->pDevice, shaderFolder + path);
}

void Resources::LoadVertexShader(const wchar_t* path, string name) {
	InitiatedExitCheck();

	_Instance->vertexShaders[name] = new VertexShader();

	_Instance->vertexShaders[name]->Init(_Instance->pDevice, shaderFolder + path, _Instance->layout, _Instance->layoutCount);
}

ID3D11ShaderResourceView* Resources::GetTexture(string name) {
	InitiatedExitCheck();

	if (!_Instance->textures.count(name)) { //If key doesnt exist
		return nullptr;
	}

	return _Instance->textures[name];
}

PixelShader* Resources::GetPixelShader(string name) {
	InitiatedExitCheck();

	if (!_Instance->pixelShaders.count(name)) { //If key doesnt exist
		return nullptr;
	}

	return _Instance->pixelShaders[name];
}

VertexShader* Resources::GetVertexShader(string name) {
	InitiatedExitCheck();

	if (!_Instance->vertexShaders.count(name)) { //If key doesnt exist
		return nullptr;
	}

	return _Instance->vertexShaders[name];
}

Resources::~Resources() {
	for (pair<string,ID3D11ShaderResourceView*> texPair : _Instance->textures) {
		texPair.second->Release();
		delete texPair.second;
	}
	for (pair<string, PixelShader*> pair : _Instance->pixelShaders) {
		delete pair.second;
	}
	for (pair<string, VertexShader*> pair : _Instance->vertexShaders) {
		delete pair.second;
	}
}
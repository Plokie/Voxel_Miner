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

void Resources::Init(ID3D11Device* device, ID3D11DeviceContext* deviceCtx, D3D11_INPUT_ELEMENT_DESC layout[], int layoutCount) {
	if (_Instance == nullptr) {
		_Instance = new Resources();
	}

	_Instance->pDevice = device;
	_Instance->pDeviceCtx = deviceCtx;
	_Instance->layout = layout;
	_Instance->layoutCount = layoutCount;
}

void Resources::LoadTexture(const wchar_t* path, string name) {
	InitiatedExitCheck();

	_Instance->textures[name] = nullptr;

	HRESULT hr = CreateDDSTextureFromFile(_Instance->pDevice, path, nullptr, &_Instance->textures[name], 0, 0);
	if (FAILED(hr)) exit(101);

	//_Instance->pDeviceCtx->GenerateMips(_Instance->textures[name]);

	
}
//wstring shaderFolder = L"Data\\Shaders\\";
////#ifdef _DEBUG
////wstring shaderFolder = L"x64\\Debug\\";
////#else
////wstring shaderFolder = L"x64\\Release\\";
////#endif

void Resources::LoadPixelShader(const wchar_t* path, string name) {
	InitiatedExitCheck();

	_Instance->pixelShaders[name] = new PixelShader();

	_Instance->pixelShaders[name]->Init(_Instance->pDevice, path);
}

void Resources::LoadVertexShader(const wchar_t* path, string name) {
	InitiatedExitCheck();

	_Instance->vertexShaders[name] = new VertexShader();

	_Instance->vertexShaders[name]->Init(_Instance->pDevice, path, _Instance->layout, _Instance->layoutCount);
}

void Resources::LoadMesh(string name) {
	InitiatedExitCheck();

	_Instance->meshes[name] = new Mesh();
	_Instance->meshes[name]->Init(_Instance->pDevice);
	_Instance->meshes[name]->LoadDefaultCube(); //TODO: load mesh either from array or from file(?)
}

void Resources::LoadMesh(Vertex vertices[], int vertCount, DWORD indices[], int indexCount, string name) {
	InitiatedExitCheck();

	Mesh* newMesh = new Mesh();
	_Instance->meshes[name] = newMesh;
	newMesh->Init(_Instance->pDevice);
	newMesh->LoadVertices(vertices, vertCount);
	newMesh->LoadIndices(indices, indexCount);
}

void Resources::LoadFont(const wchar_t* fontPath, string name)
{
	InitiatedExitCheck();

	_Instance->spriteFonts[name] = new SpriteFont(_Instance->pDevice, fontPath);
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

Mesh* Resources::GetMesh(string name) {
	InitiatedExitCheck();

	if(!_Instance->meshes.count(name)) {
		return nullptr;
	}

	return _Instance->meshes[name];
}

SpriteFont* Resources::GetFont(string name)
{
	InitiatedExitCheck();
	auto it = _Instance->spriteFonts.find(name);

	if(it != _Instance->spriteFonts.end()) {
		return it->second;
	}
	return nullptr;
}

Resources::~Resources() {
	for (const pair<string,ID3D11ShaderResourceView*>& texPair : _Instance->textures) {
		texPair.second->Release();
		delete texPair.second;
	}
	for (const pair<string, PixelShader*>& pair : _Instance->pixelShaders) {
		delete pair.second;
	}
	for (const pair<string, VertexShader*>& pair : _Instance->vertexShaders) {
		delete pair.second;
	}
	for(const pair<string, Mesh*>& pair : _Instance->meshes) {
		delete pair.second;
	}
	for(const auto& pair : _Instance->spriteFonts) {
		delete pair.second;
	}
}
#pragma once
#include <string>
#pragma comment(lib, "D3DCompiler.lib")
#include <d3d11.h>
#include <d3dcompiler.h>

class VertexShader {
private:
	ID3D11VertexShader* shader;
	ID3D10Blob* shader_buffer;
	ID3D11InputLayout* input_layout;
public:
	bool Init(ID3D11Device* device, std::wstring path, D3D11_INPUT_ELEMENT_DESC* desc, UINT numElements);
	ID3D11VertexShader* GetShader();
	ID3D10Blob* GetBuffer();
	ID3D11InputLayout* GetInputLayout() { return input_layout; };

	~VertexShader() {
		if(shader) shader->Release();
		if(shader_buffer) shader_buffer->Release();
	}
};

class PixelShader {
private:
	ID3D11PixelShader* shader;
	ID3D10Blob* shader_buffer;
public:
	bool Init(ID3D11Device* device, std::wstring path);

	ID3D11PixelShader* GetShader() { return shader; }
	ID3D10Blob* GetBuffer() { return shader_buffer; }

};
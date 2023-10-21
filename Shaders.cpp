#include "Shaders.h"

bool VertexShader::Init(ID3D11Device* device, std::wstring path, D3D11_INPUT_ELEMENT_DESC* desc, UINT numElements) {
	HRESULT hr = D3DReadFileToBlob(path.c_str(), &shader_buffer);
	if(FAILED(hr)) exit(30); //Failed to load shader
	
	hr = device->CreateVertexShader(shader_buffer->GetBufferPointer(), shader_buffer->GetBufferSize(), NULL, &shader);
	if(FAILED(hr)) exit(31);

	hr = device->CreateInputLayout(desc, numElements, shader_buffer->GetBufferPointer(), shader_buffer->GetBufferSize(), &input_layout);
	if(FAILED(hr)) {
		exit(32); //Failed to create shader layout buffer
		return false;
	}

	return true;
}

ID3D11VertexShader* VertexShader::GetShader() {
	return shader;
}

ID3D10Blob* VertexShader::GetBuffer() {
	return shader_buffer;
}



bool PixelShader::Init(ID3D11Device* device, std::wstring path) {
	HRESULT hr = D3DReadFileToBlob(path.c_str(), &shader_buffer);
	if(FAILED(hr)) exit(40); //Failed to load shader

	hr = device->CreatePixelShader(shader_buffer->GetBufferPointer(), shader_buffer->GetBufferSize(), NULL, &shader);
	if(FAILED(hr)) exit(41);

	return true;
}
#include "Model.h"

#include "../Engine/Engine.h"

Model* Model::Init(ID3D11Device* device) {
	pDevice = device;

	CreateBuffer(D3D11_USAGE_DYNAMIC, sizeof(CB_VS_vertexshader) + (16 - (sizeof(CB_VS_vertexshader) % 16)), D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, &constantBuffer);
	CreateBuffer(D3D11_USAGE_DYNAMIC, sizeof(CB_VS_pixelshader) + (16 - (sizeof(CB_VS_pixelshader) % 16)), D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, &alphaBuffer);
	return this;
}

void Model::Draw(ID3D11DeviceContext* deviceCtx, XMMATRIX modelMx, XMMATRIX worldMx) {
	if(mesh == nullptr) return;
	
	CB_VS_vertexshader mxData;
	CB_VS_pixelshader alphaData;

	alphaData.alpha = alpha;

	mxData.mx = modelMx * worldMx;
	mxData.modelMx = modelMx;

	//Convert to row-major for HLSL
	mxData.mx = DirectX::XMMatrixTranspose(mxData.mx);
	mxData.modelMx = XMMatrixTranspose(mxData.modelMx);

	mxData.time = Engine::Get()->GetTotalElapsedTime();

	D3D11_MAPPED_SUBRESOURCE map;
	HRESULT hr = deviceCtx->Map(constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
	CopyMemory(map.pData, &mxData, sizeof(CB_VS_vertexshader));
	deviceCtx->Unmap(constantBuffer, 0);
	deviceCtx->VSSetConstantBuffers(0, 1, &constantBuffer);

	deviceCtx->Map(alphaBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
	CopyMemory(map.pData, &alphaData, sizeof(CB_VS_pixelshader));
	deviceCtx->Unmap(alphaBuffer, 0);
	deviceCtx->PSSetConstantBuffers(0, 1, &alphaBuffer);

	if(mesh == nullptr) return;

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	deviceCtx->IASetVertexBuffers(0, 1, &mesh->vertexBuffer, &stride, &offset);
	deviceCtx->IASetIndexBuffer(mesh->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

	if (pixelShader != nullptr)
		deviceCtx->PSSetShader(pixelShader->GetShader(), NULL, 0);

	if (vertexShader != nullptr)
		deviceCtx->VSSetShader(vertexShader->GetShader(), NULL, 0);

	if (tex != nullptr)
		deviceCtx->PSSetShaderResources(0, 1, &tex);
	
	//ID3D11Resource* pRes = nullptr;
	//D3D11_TEXTURE2D_DESC tempDesc{};
	//tex->GetResource(&pRes);
	//ID3D11Texture2D* pTex = (ID3D11Texture2D*)pRes;

	//pTex->GetDesc(&tempDesc);
	if(mesh == nullptr) return;

	deviceCtx->DrawIndexed((UINT)mesh->GetIndexCount(), 0, 0);
}

void Model::ReleaseMesh()
{
	if(this->mesh != nullptr) {
		delete this->mesh;

		this->mesh = nullptr;
	}
}

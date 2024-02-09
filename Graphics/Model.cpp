#include "Model.h"

#include "../Engine/Engine.h"

Model* Model::Init(ID3D11Device* device) {
	pDevice = device;

	//InitializeSRWLock(&gAcquireMutex);

	CreateBuffer(D3D11_USAGE_DYNAMIC, sizeof(CB_VS_vertexshader) + (16 - (sizeof(CB_VS_vertexshader) % 16)), D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, &constantBuffer);
	CreateBuffer(D3D11_USAGE_DYNAMIC, sizeof(CB_VS_pixelshader) + (16 - (sizeof(CB_VS_pixelshader) % 16)), D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, &alphaBuffer);
	return this;
}
 
void Model::Draw(ID3D11DeviceContext* deviceCtx, XMMATRIX modelMx, XMMATRIX viewMx, XMMATRIX projMx) {
	Draw(deviceCtx, modelMx, viewMx, projMx, pixelShader?pixelShader->GetShader():nullptr, vertexShader?vertexShader->GetShader():nullptr);
	
	/*if(mesh == nullptr) return;

	CB_VS_vertexshader vsData;
	CB_VS_pixelshader psData;
	
	vsData.modelMx = XMMatrixTranspose(modelMx);
	vsData.viewMx = XMMatrixTranspose(viewMx);
	vsData.projMx = XMMatrixTranspose(projMx);
	vsData.lightViewMx = XMMatrixTranspose(Graphics::Get()->shadowCamera.transform.mxView());
	vsData.lightProjMx = XMMatrixTranspose(Graphics::Get()->shadowCamera.GetProjectionMatrix());
	vsData.time = Engine::Get()->GetTotalElapsedTime();

	psData.alpha = alpha;
	psData.uvOffset = uvOffset;
	
	D3D11_MAPPED_SUBRESOURCE map;
	HRESULT hr = deviceCtx->Map(constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
	CopyMemory(map.pData, &vsData, sizeof(CB_VS_vertexshader));
	deviceCtx->Unmap(constantBuffer, 0);
	deviceCtx->VSSetConstantBuffers(0, 1, &constantBuffer);
	
	deviceCtx->Map(alphaBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
	CopyMemory(map.pData, &psData, sizeof(CB_VS_pixelshader));
	deviceCtx->Unmap(alphaBuffer, 0);
	deviceCtx->PSSetConstantBuffers(0, 1, &alphaBuffer);
	
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	deviceCtx->IASetVertexBuffers(0, 1, &mesh->vertexBuffer, &stride, &offset);
	deviceCtx->IASetIndexBuffer(mesh->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);
	
	if(pixelShader) deviceCtx->PSSetShader(pixelShader->GetShader(), NULL, 0);
	if(vertexShader) deviceCtx->VSSetShader(vertexShader->GetShader(), NULL, 0);
	if(tex) deviceCtx->PSSetShaderResources(0, 1, &tex);
		
		
	
	deviceCtx->DrawIndexed((UINT)mesh->GetIndexCount(), 0, 0);*/
}

void Model::Draw(ID3D11DeviceContext* deviceCtx, XMMATRIX modelMx, XMMATRIX viewMx, XMMATRIX projMx, ID3D11PixelShader* ps, ID3D11VertexShader* vs) {
	if(mesh == nullptr) return;

	CB_VS_vertexshader vsData;
	CB_VS_pixelshader psData;

	vsData.modelMx = XMMatrixTranspose(modelMx);
	vsData.viewMx = XMMatrixTranspose(viewMx);
	vsData.projMx = XMMatrixTranspose(projMx);
	vsData.lightViewMx = XMMatrixTranspose(Graphics::Get()->shadowCamera.transform.mxView());
	vsData.lightProjMx = XMMatrixTranspose(Graphics::Get()->shadowCamera.GetProjectionMatrix());
	vsData.time = Engine::Get()->GetTotalElapsedTime();

	psData.alpha = alpha;
	psData.uvOffset = uvOffset;

	D3D11_MAPPED_SUBRESOURCE map;
	HRESULT hr = deviceCtx->Map(constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
	CopyMemory(map.pData, &vsData, sizeof(CB_VS_vertexshader));
	deviceCtx->Unmap(constantBuffer, 0);
	deviceCtx->VSSetConstantBuffers(0, 1, &constantBuffer);

	deviceCtx->Map(alphaBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
	CopyMemory(map.pData, &psData, sizeof(CB_VS_pixelshader));
	deviceCtx->Unmap(alphaBuffer, 0);
	deviceCtx->PSSetConstantBuffers(0, 1, &alphaBuffer);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	deviceCtx->IASetVertexBuffers(0, 1, &mesh->vertexBuffer, &stride, &offset);
	deviceCtx->IASetIndexBuffer(mesh->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

	if(ps) deviceCtx->PSSetShader(ps, NULL, 0);
	if(vs) deviceCtx->VSSetShader(vs, NULL, 0);
	if(tex) deviceCtx->PSSetShaderResources(0, 1, &tex);



	deviceCtx->DrawIndexed((UINT)mesh->GetIndexCount(), 0, 0);






	//if(mesh == nullptr) return;

	////ShadowMap_CBuff cBuffData;

	////XMStoreFloat4x4(&cBuffData.projection, XMMatrixTranspose(projMx));
	////XMStoreFloat4x4(&cBuffData.view, XMMatrixTranspose(viewMx));
	////XMStoreFloat4(&cBuffData.pos, Graphics::Get()->shadowCamera.transform.position);

	//D3D11_MAPPED_SUBRESOURCE map;
	////deviceCtx->UpdateSubresource(&map, 0, NULL, &shadowCbuff, 0, 0);
	//deviceCtx->Map(constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
	////CopyMemory(map.pData, &cBuffData, sizeof(ShadowMap_CBuff));
	//deviceCtx->Unmap(constantBuffer, 0);

	//deviceCtx->VSSetConstantBuffers(0, 1, &constantBuffer);
	//deviceCtx->PSSetConstantBuffers(0, 1, &constantBuffer);

	//UINT stride = sizeof(Vertex);
	//UINT offset = 0;
	//deviceCtx->IASetVertexBuffers(0, 1, &mesh->vertexBuffer, &stride, &offset);
	//deviceCtx->IASetIndexBuffer(mesh->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

	//if(ps) deviceCtx->PSSetShader(ps, NULL, 0);
	//if(vs) deviceCtx->VSSetShader(vs, NULL, 0);

	//deviceCtx->DrawIndexed((UINT)mesh->GetIndexCount(), 0, 0);
}

void Model::ReleaseMesh()
{
	if (this == nullptr) return;
	if(this->mesh != nullptr) {
		//this->mesh->~Mesh();
		delete this->mesh;

		this->mesh = nullptr;
	}
}

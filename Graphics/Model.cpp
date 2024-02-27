#include "Model.h"

#include "../Engine/Engine.h"
#include "Camera.h"

void Model::SetupCbuffers(ID3D11DeviceContext* deviceCtx, const XMMATRIX& modelMx, const XMMATRIX& viewMx, const XMMATRIX& projMx)
{
	VSCbuffer_Model vsData;
	CB_VS_pixelshader psData;

	vsData.modelMx = XMMatrixTranspose(modelMx);
	vsData.viewMx = XMMatrixTranspose(viewMx);
	vsData.projMx = XMMatrixTranspose(projMx);
	vsData.time = Engine::Get()->GetTotalElapsedTime();

	psData.alpha = alpha;
	psData.uvOffset = uvOffset;

	D3D11_MAPPED_SUBRESOURCE map;

	HRESULT hr = deviceCtx->Map(vsCbuffer_Model, 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
	CopyMemory(map.pData, &vsData, sizeof(VSCbuffer_Model));
	deviceCtx->Unmap(vsCbuffer_Model, 0);
	deviceCtx->VSSetConstantBuffers(0, 1, &vsCbuffer_Model);

	deviceCtx->Map(psCbuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
	CopyMemory(map.pData, &psData, sizeof(CB_VS_pixelshader));
	deviceCtx->Unmap(psCbuffer, 0);
	deviceCtx->PSSetConstantBuffers(0, 1, &psCbuffer);
}

void Model::SetFlags(unsigned int flags)
{
	this->flags = flags;
}

bool Model::HasFlag(MODEL_FLAG flag) const
{
	return flags & flag;
}

Model* Model::Init(ID3D11Device* device) {
	pDevice = device;

	//InitializeSRWLock(&gAcquireMutex);

	Graphics::CreateBuffer(D3D11_USAGE_DYNAMIC, sizeof(VSCbuffer_Model) + (16 - (sizeof(VSCbuffer_Model) % 16)), D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, &vsCbuffer_Model);
	Graphics::CreateBuffer(D3D11_USAGE_DYNAMIC, sizeof(CB_VS_pixelshader) + (16 - (sizeof(CB_VS_pixelshader) % 16)), D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, &psCbuffer);
	return this;
}
 
void Model::Draw(ID3D11DeviceContext* deviceCtx, const XMMATRIX& modelMx, const XMMATRIX& viewMx, const XMMATRIX& projMx) {
	Draw(deviceCtx, modelMx, viewMx, projMx, pixelShader?pixelShader->GetShader():nullptr, vertexShader?vertexShader->GetShader():nullptr);
}

void Model::Draw(ID3D11DeviceContext* deviceCtx, const XMMATRIX& modelMx, const XMMATRIX& viewMx, const XMMATRIX& projMx, ID3D11PixelShader* ps, ID3D11VertexShader* vs) {
	if(mesh == nullptr) return;

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	deviceCtx->IASetVertexBuffers(0, 1, &mesh->vertexBuffer, &stride, &offset);
	deviceCtx->IASetIndexBuffer(mesh->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

	SetupCbuffers(deviceCtx, modelMx, viewMx, projMx);

	if(ps)
		deviceCtx->PSSetShader(ps, NULL, 0);
	if(vs)
		deviceCtx->VSSetShader(vs, NULL, 0);
	if(tex)
		deviceCtx->PSSetShaderResources(0, 1, &tex);

	deviceCtx->DrawIndexed((UINT)mesh->GetIndexCount(), 0, 0);
}

void Model::DrawShadows(ID3D11DeviceContext* deviceCtx, const XMMATRIX& modelMx, ID3D11PixelShader* ps, ID3D11VertexShader* vs, Camera** cameras, ID3D11DepthStencilView** stencils, D3D11_VIEWPORT* viewports, int amount)
{
	if(mesh == nullptr) return;

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	deviceCtx->IASetVertexBuffers(0, 1, &mesh->vertexBuffer, &stride, &offset);
	deviceCtx->IASetIndexBuffer(mesh->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

	if(ps)
		deviceCtx->PSSetShader(ps, NULL, 0);
	if(vs)
		deviceCtx->VSSetShader(vs, NULL, 0);

	for(int i = 0; i < amount; i++) {
		SetupCbuffers(deviceCtx, modelMx, cameras[i]->transform.mxView(), cameras[i]->GetProjectionMatrix());

		deviceCtx->DrawIndexed((UINT)mesh->GetIndexCount(), 0, 0);
	}
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

Model::~Model()
{
	/*if(vertexBuffer) vertexBuffer->Release();
		if(indexBuffer) indexBuffer->Release();*/
	if(vsCbuffer_Model) vsCbuffer_Model->Release();
	if(psCbuffer) psCbuffer->Release();

	if(mesh != nullptr)
		if(mesh->_isProceduralMesh) {
			ReleaseMesh();
		}
}

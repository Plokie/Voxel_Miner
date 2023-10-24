#include "Mesh.h"

void Mesh::Init(ID3D11Device* device) {
	pDevice = device;

	Vertex vertices[] = {
		Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f), //bottom-left		0  -Z
		Vertex(-1.0f,  1.0f, -1.0f, 0.0f, 0.0f), //top-left			1
		Vertex(1.0f,  1.0f, -1.0f, 1.0f, 0.0f), //top-right		2
		Vertex(1.0f, -1.0f, -1.0f, 1.0f, 1.0f), //bottom-right		3

		Vertex(1.0f, -1.0f, 1.0f,	0.0f, 1.0f), //bottom-left		4  +Z
		Vertex(1.0f,  1.0f, 1.0f,	0.0f, 0.0f), //top-left			5
		Vertex(-1.0f,  1.0f, 1.0f,	1.0f, 0.0f), //top-right			6
		Vertex(-1.0f, -1.0f, 1.0f,	1.0f, 1.0f), //bottom-right		7

		Vertex(1.0f, -1.0f, -1.0f,	0.0f, 1.0f), //bottom-left		8  +X
		Vertex(1.0f,  1.0f, -1.0f,	0.0f, 0.0f), //top-left			9
		Vertex(1.0f,  1.0f, 1.0f,	1.0f, 0.0f), //top-right		10
		Vertex(1.0f, -1.0f, 1.0f,	1.0f, 1.0f), //bottom-right		11

		Vertex(-1.0f, -1.0f, 1.0f,	0.0f, 1.0f), //bottom-left		12  -X
		Vertex(-1.0f,  1.0f, 1.0f,	0.0f, 0.0f), //top-left			13
		Vertex(-1.0f,  1.0f, -1.0f, 1.0f, 0.0f), //top-right		14
		Vertex(-1.0f, -1.0f, -1.0f, 1.0f, 1.0f), //bottom-right		15

		Vertex(-1.0f,  1.0f, -1.0f, 0.0f, 1.0f), //					16 +Y
		Vertex(-1.0f,  1.0f, 1.0f,	0.0f, 0.0f), //					17
		Vertex(1.0f,  1.0f, 1.0f,	1.0f, 0.0f), //					18
		Vertex(1.0f,  1.0f, -1.0f,	1.0f, 1.0f), //					19

		Vertex(-1.0f, -1.0f, 1.0f,	0.0f, 1.0f), //					20 -Y
		Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 0.0f), //					21
		Vertex(1.0f, -1.0f, -1.0f,  1.0f, 0.0f), //					22
		Vertex(1.0f, -1.0f, 1.0f,	1.0f, 1.0f), //					23
	};

	DWORD indices[] = {
		0,1,2, //-Z
		0,2,3,

		4,5,6, //+Z
		4,6,7,

		8,9,10, //+X
		8,10,11,

		12,13,14, //-X
		12,14,15,

		16,17,18, //+Y
		16,18,19,

		20,21,22, //-Y
		20,22,23
	};

	this->indexCount = ARRAYSIZE(indices);

	CreateBuffer(D3D11_USAGE_DEFAULT, sizeof(Vertex) * ARRAYSIZE(vertices), D3D11_BIND_VERTEX_BUFFER, 0, &vertexBuffer, vertices);
	CreateBuffer(D3D11_USAGE_DEFAULT, sizeof(DWORD) * ARRAYSIZE(indices), D3D11_BIND_INDEX_BUFFER, 0, &indexBuffer, indices);
	CreateBuffer(D3D11_USAGE_DYNAMIC, sizeof(CB_VS_vertexshader) + (16 - (sizeof(CB_VS_vertexshader) % 16)), D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, &constantBuffer);
	CreateBuffer(D3D11_USAGE_DYNAMIC, sizeof(CB_VS_pixelshader) + (16 - (sizeof(CB_VS_pixelshader) % 16)), D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, &alphaBuffer);
}

void Mesh::Draw(ID3D11DeviceContext* deviceCtx, XMMATRIX selfMx, XMMATRIX baseMx) {
	CB_VS_vertexshader mxData;
	CB_VS_pixelshader alphaData;

	alphaData.alpha = alpha;

	mxData.mx = selfMx * baseMx;

	//Convert to row-major for HLSL
	mxData.mx = DirectX::XMMatrixTranspose(mxData.mx);

	D3D11_MAPPED_SUBRESOURCE map;
	HRESULT hr = deviceCtx->Map(constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
	CopyMemory(map.pData, &mxData, sizeof(CB_VS_vertexshader));
	deviceCtx->Unmap(constantBuffer, 0);
	deviceCtx->VSSetConstantBuffers(0, 1, &constantBuffer);

	deviceCtx->Map(alphaBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &map);
	CopyMemory(map.pData, &alphaData, sizeof(CB_VS_pixelshader));
	deviceCtx->Unmap(alphaBuffer, 0);
	deviceCtx->PSSetConstantBuffers(0, 1, &alphaBuffer);


	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	deviceCtx->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	deviceCtx->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	if (pixelShader != nullptr)
		deviceCtx->PSSetShader(pixelShader->GetShader(), NULL, 0);

	if (vertexShader != nullptr)
		deviceCtx->VSSetShader(vertexShader->GetShader(), NULL, 0);

	if (tex != nullptr)
		deviceCtx->PSSetShaderResources(0, 1, &tex);

	deviceCtx->DrawIndexed((UINT)indexCount, 0, 0);
}
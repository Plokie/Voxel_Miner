#pragma once

#include "Transform.h"
#include "..\Graphics\Mesh.h"

class Object3D {
public:
	Transform transform;
	Mesh* mesh = nullptr;

	Object3D(){}
	Object3D(ID3D11Device* device) {
		mesh = new Mesh();
		mesh->Init(device);
	}

	void Draw(ID3D11DeviceContext* deviceCtx, XMMATRIX baseMx);

	~Object3D() {
		if(mesh) mesh->~Mesh();
	}
};
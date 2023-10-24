#pragma once

#include "Transform.h"
#include "..\Graphics\Model.h"

class Object3D {
public:
	Transform transform;
	//Mesh* mesh = nullptr;

	//ID3D11Device* deviceRef = nullptr;

	vector<Model*> meshes = {};

	Object3D(){}
	Object3D(ID3D11Device* device) {
		//deviceRef = device;

		Model* newMesh = new Model();
		newMesh->Init(device);
		meshes.push_back(newMesh);
	
	}

	bool Draw(ID3D11DeviceContext* deviceCtx, XMMATRIX worldMx, vector<pair<Model*, XMMATRIX>>* transparentMeshes);

	virtual void Update(float dTime);

	//virtual ~Object3D() {};
	virtual ~Object3D() {
		//if(mesh) mesh->~Mesh();
		for (Model* mesh : meshes) {
			mesh->~Model();
		}
	};
};
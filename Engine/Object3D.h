#pragma once

#include "Transform.h"
#include "..\Graphics\Mesh.h"

class Object3D {
public:
	Transform transform;
	//Mesh* mesh = nullptr;

	ID3D11Device* deviceRef = nullptr;

	vector<Mesh*> meshes = {};

	Object3D(){}
	Object3D(ID3D11Device* device) {
		deviceRef = device;

		Mesh* newMesh = new Mesh();
		newMesh->Init(device);
		meshes.push_back(newMesh);
	
	}

	bool Draw(ID3D11DeviceContext* deviceCtx, XMMATRIX worldMx, vector<pair<Mesh*, XMMATRIX>>* transparentMeshes);

	~Object3D() {
		//if(mesh) mesh->~Mesh();
		for(Mesh* mesh : meshes) {
			mesh->~Mesh();
		}
	}
};
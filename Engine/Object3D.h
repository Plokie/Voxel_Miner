#pragma once

#include "Transform.h"
#include "..\Graphics\Mesh.h"

class Object3D {
public:
	Transform transform;
	//Mesh* mesh = nullptr;

	vector<Mesh*> meshes = {};

	Object3D(){}
	Object3D(ID3D11Device* device) {
		//mesh = new Mesh();
		//mesh->Init(device);
		Mesh* newMesh = new Mesh();
		newMesh->Init(device);
		meshes.push_back(newMesh);
	
	}

	void Draw(ID3D11DeviceContext* deviceCtx, XMMATRIX baseMx, vector<pair<Mesh*, XMMATRIX>>* transparentMeshes);

	~Object3D() {
		//if(mesh) mesh->~Mesh();
		for(Mesh* mesh : meshes) {
			mesh->~Mesh();
		}
	}
};
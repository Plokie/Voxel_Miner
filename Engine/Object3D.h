#pragma once

#include "Transform.h"
#include "..\Graphics\Model.h"

class Object3D {
public:
	bool hasRanStartFunction = false;

	Transform transform = Transform();
	//Mesh* mesh = nullptr;

	//ID3D11Device* deviceRef = nullptr;

	vector<Model*> models = {};

	Object3D(){}
	Object3D(ID3D11Device* device);

	bool Draw(ID3D11DeviceContext* deviceCtx, XMMATRIX worldMx, vector<pair<Model*, XMMATRIX>>* transparentModels);

	Model* AddModel(ID3D11Device* device);

	virtual void Start();
	virtual void Update(float dTime);

	//virtual ~Object3D() {};
	virtual ~Object3D() {
		//if(mesh) mesh->~Mesh();
		for (Model* model : models) {

			model->~Model();
		}
	};
};
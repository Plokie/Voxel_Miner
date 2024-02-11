#pragma once

#include <mutex>

#include "Transform.h"
#include "..\Graphics\Model.h"
#include "AABB.h"

class Object3D {
public:
	std::mutex gAccessMutex = {};
	std::mutex modelsMutex = {};

	bool doRender = true;
	bool pendingDeletion = false;
	bool importantRenderPass = false;

	// The general AABB box describing the max area of the object
	// Used for frustum culling
	AABB cullBox = {Vector3(0.f,0.f,0.f), Vector3(0.f,0.f,0.f)};

	bool hasRanStartFunction = false;

	Transform transform = Transform();

	vector<Model*> models = {};

	Object3D(){}
	Object3D(ID3D11Device* device);

	// Finds the closest distance to another position (takes into account AABB)
	const float GetClosestDistance(const Vector3& otherPos);

	bool Draw(ID3D11DeviceContext* deviceCtx, XMMATRIX viewMx, XMMATRIX projMx, vector<tuple<Model*, XMMATRIX, Object3D*>>* transparentModels);
	bool Draw(ID3D11DeviceContext* deviceCtx, XMMATRIX viewMx, XMMATRIX projMx, ID3D11PixelShader* ps, ID3D11VertexShader* vs, unsigned int modelFlagFilter);

	Model* AddModel(ID3D11Device* device);

	virtual void Start();
	virtual void Update(float dTime);

	//virtual ~Object3D() {};
	virtual ~Object3D() {
		//if(mesh) mesh->~Mesh();
		for (Model* model : models) {

			//model->~Model();
			delete model;
			model = nullptr;
		}
	};
};
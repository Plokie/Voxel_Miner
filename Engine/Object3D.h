#pragma once

#include <mutex>

#include "Transform.h"
#include "..\Graphics\Model.h"
#include "AABB.h"

enum OBJ_RENDER_TYPE : unsigned char {
	NORMAL,
	ONCE_PER_FRAME
};

class Object3D {
public:
	std::mutex gAccessMutex = {};
	std::mutex modelsMutex = {};

	bool doRender = true;
	bool pendingDeletion = false;
	bool importantRenderPass = false;
	OBJ_RENDER_TYPE renderType = OBJ_RENDER_TYPE::NORMAL;

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

	bool Draw(ID3D11DeviceContext* deviceCtx, const XMMATRIX& viewMx, const XMMATRIX& projMx, vector<tuple<Model*, XMMATRIX, Object3D*>>* transparentModels);
	bool Draw(ID3D11DeviceContext* deviceCtx, const XMMATRIX& viewMx, const XMMATRIX& projMx, ID3D11PixelShader* ps, ID3D11VertexShader* vs, unsigned int modelFlagFilter);
	bool DrawShadows(ID3D11DeviceContext* deviceCtx, ID3D11PixelShader* ps, ID3D11VertexShader* vs, unsigned int modelFlagFilter, Camera** cameras, ID3D11DepthStencilView** stencils, D3D11_VIEWPORT* viewports, int amount);

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
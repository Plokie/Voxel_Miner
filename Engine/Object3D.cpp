#include "Object3D.h"

Object3D::Object3D(ID3D11Device* device) {
	//deviceRef = device;

	AddModel(device);
}

Model* Object3D::AddModel(ID3D11Device* device) {
	Model* newMesh = new Model();
	newMesh->Init(device);
	models.push_back(newMesh);
	return newMesh;
}

bool Object3D::Draw(ID3D11DeviceContext* deviceCtx, XMMATRIX worldMx, vector<tuple<Model*, XMMATRIX, Object3D*>>* transparentModels) {
	//AcquireSRWLockExclusive(&this->gAccessMutex);
	bool didDraw = false;

	//AcquireSRWLockExclusive(&modelsMutex);

	{	//unique_lock<std::mutex> lock(modelsMutex);
		for(Model* model : models) {
			if (model == nullptr) continue;
			if (!model->IsTransparent()) //If object is opaque, draw immediately upon request
			{
				model->Draw(deviceCtx, transform.mx(), worldMx);
				didDraw = true;
			}
			else //if objects contains transparency, queue to be rendered after opaque geometry
			{
				transparentModels->push_back(tuple<Model*, XMMATRIX, Object3D*>(model, transform.mx(), this));
			}
		}
	}
	//ReleaseSRWLockExclusive(&modelsMutex);

	//ReleaseSRWLockExclusive(&this->gAccessMutex);

	return didDraw;
}

void Object3D::Update(float dTime) {}

void Object3D::Start() {}
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

const float Object3D::GetClosestDistance(const Vector3& otherPos)
{
	//todo: find closest distance to aabb
	//return transform.GetPosition().sqrDistTo(otherPos);
		return transform.GetPosition().sqrDistTo(otherPos);
	//if(cullBox.GetHalfSize().magnitude() == 0.0f) { // if there is no aabb, return distance to position
	//}
	////else if(cullBox.IsPointWithin(otherPos)){
	////	//cullBox.penetration_vector()
	////	return 0.0f;
	////}
	//else {
	//	Vector3 aabbMin, aabbMax;
	//	cullBox.GetMinMax(&aabbMin, &aabbMax);

	//	Vector3 closestPoint;
	//	closestPoint.x = max(aabbMin.x, min(otherPos.x, aabbMax.x));
	//	closestPoint.y = max(aabbMin.y, min(otherPos.y, aabbMax.y));
	//	closestPoint.z = max(aabbMin.z, min(otherPos.z, aabbMax.z));

	//	return closestPoint.sqrDistTo(otherPos);
	//}
}

bool Object3D::Draw(ID3D11DeviceContext* deviceCtx, XMMATRIX viewMx, XMMATRIX projMx, vector<tuple<Model*, XMMATRIX, Object3D*>>* transparentModels) {
	//AcquireSRWLockExclusive(&this->gAccessMutex);
	bool didDraw = false;

	//AcquireSRWLockExclusive(&modelsMutex);

	{	//unique_lock<std::mutex> lock(modelsMutex);
		for(Model* model : models) {
			if (model == nullptr) continue;
			if (!model->IsTransparent()) //If object is opaque, draw immediately upon request
			{
				model->Draw(deviceCtx, transform.mx(), viewMx, projMx);
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

bool Object3D::Draw(ID3D11DeviceContext* deviceCtx, XMMATRIX viewMx, XMMATRIX projMx, ID3D11PixelShader* ps, ID3D11VertexShader* vs) {
	for(Model* model : models) {

		if(model == nullptr) continue;
		
		if(!model->IsTransparent())
			model->Draw(deviceCtx, transform.mx(), viewMx, projMx, ps, vs);
	}

	return true;
}

void Object3D::Update(float dTime) {}

void Object3D::Start() {}
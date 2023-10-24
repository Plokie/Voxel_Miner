#include "Object3D.h"


bool Object3D::Draw(ID3D11DeviceContext* deviceCtx, XMMATRIX worldMx, vector<pair<Model*, XMMATRIX>>* transparentModels) {
	bool didDraw = false;

	for(Model* model : models) {
		if (!model->IsTransparent()) //If object is opaque, draw immediately upon request
		{
			model->Draw(deviceCtx, transform.mx(), worldMx);
			didDraw = true;
		}
		else //if objects contains transparency, queue to be rendered after opaque geometry
		{
			transparentModels->push_back(pair<Model*, XMMATRIX>(model, transform.mx()));
		}
	}
	return didDraw;
}

void Object3D::Update(float dTime)
{
}

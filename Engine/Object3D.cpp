#include "Object3D.h"


bool Object3D::Draw(ID3D11DeviceContext* deviceCtx, XMMATRIX worldMx, vector<pair<Model*, XMMATRIX>>* transparentMeshes) {
	bool didDraw = false;

	for(Model* mesh : meshes) {
		if (!mesh->IsTransparent()) //If object is opaque, draw immediately upon request
		{
			mesh->Draw(deviceCtx, transform.mx(), worldMx);
			didDraw = true;
		}
		else //if objects contains transparency, queue to be rendered after opaque geometry
		{
			transparentMeshes->push_back(pair<Model*, XMMATRIX>(mesh, transform.mx()));
		}
	}
	return didDraw;
}

void Object3D::Update(float dTime)
{
}

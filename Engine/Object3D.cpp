#include "Object3D.h"


bool Object3D::Draw(ID3D11DeviceContext* deviceCtx, XMMATRIX worldMx, vector<pair<Mesh*, XMMATRIX>>* transparentMeshes) {
	bool didDraw = false;

	for(Mesh* mesh : meshes) {
		if (!mesh->IsTransparent()) //If object is opaque, draw immediately upon request
		{
			mesh->Draw(deviceCtx, transform.mx(), worldMx);
			didDraw = true;
		}
		else //if objects contains transparency, queue to be rendered after opaque geometry
		{
			transparentMeshes->push_back(pair<Mesh*, XMMATRIX>(mesh, transform.mx()));
		}
	}
	return didDraw;
}
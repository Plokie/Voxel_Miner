#include "Object3D.h"


void Object3D::Draw(ID3D11DeviceContext* deviceCtx, XMMATRIX baseMx, vector<pair<Mesh*, XMMATRIX>>* transparentMeshes) {
	/*if(mesh != nullptr) {
		mesh->Draw(deviceCtx, transform.mx(), baseMx);
	}*/
	for(Mesh* mesh : meshes) {
		//mesh->Draw(deviceCtx, transform.mx(), baseMx);

		if(!mesh->IsTransparent()) //If object is opaque, draw immediately upon request
			mesh->Draw(deviceCtx, transform.mx(), baseMx);
		else //if objects contains transparency, queue to be rendered after opaque geometry
			transparentMeshes->push_back(pair<Mesh*, XMMATRIX>(mesh, transform.mx()));
	}
}
#include "Object3D.h"


void Object3D::Draw(ID3D11DeviceContext* deviceCtx, XMMATRIX baseMx) {
	if(mesh != nullptr) {
		mesh->Draw(deviceCtx, transform.mx(), baseMx);
	}
}
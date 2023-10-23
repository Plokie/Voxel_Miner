#include "Object3D.h"


void Object3D::Draw(ID3D11DeviceContext* deviceCtx, XMMATRIX baseMx, Vector3 camPos) {
	if(mesh != nullptr) {
		mesh->Draw(deviceCtx, transform.mx(), baseMx, camPos);
	}
}